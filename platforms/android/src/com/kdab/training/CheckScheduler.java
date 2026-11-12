package com.kdab.training;

import android.Manifest;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;

import androidx.work.Constraints;
import androidx.work.ExistingWorkPolicy;
import androidx.work.NetworkType;
import androidx.work.OneTimeWorkRequest;
import androidx.work.WorkManager;

import java.util.concurrent.TimeUnit;

/**
 * Queues the next background profile check.
 *
 * This used to be an AlarmManager exact alarm, which needs SCHEDULE_EXACT_ALARM - a
 * Play-restricted permission intended for alarm clocks and calendars, not for polling.
 * WorkManager needs no permission at all, survives reboot without a BOOT_COMPLETED
 * receiver, and can wait for connectivity instead of firing HTTPS requests into the void.
 *
 * A check wakes the phone, starts the process and reads the player's page from the game
 * server, so one is queued only for when it could have something to say (nextCheckDelaySec):
 * none while logged out or without the notification permission, none before the silence
 * after an unanswered notification is over, and a rare one while the caster server pushes
 * turns and invites itself (https://github.com/Pz1c/WavingHands/issues/336), which leaves
 * the check the finished battles and the reminder. MainActivity.onPause starts the chain,
 * each run queues the next one from CheckStatusWorker.
 */
public class CheckScheduler {
    private static final String TAG = "WarlocksDuel.CheckScheduler";
    private static final String WORK_NAME = "warlocks_duel_check_status";

    private static final int HOUR_SEC = 60 * 60;
    private static final int DAY_SEC = 24 * HOUR_SEC;
    // Once a notification is up CheckStatus.performCheck posts nothing more for this long,
    // unless the player has been in the app since (muted).
    public static final int MUTE_SEC = 8 * HOUR_SEC;
    // Past the end of that silence by more than the grain of the stored timestamps (128 s,
    // they go through a float), so the check queued for it finds the silence over.
    private static final int MUTE_MARGIN_SEC = 5 * 60;
    // On screen nothing is checked and onPause queues the real check; this one is only
    // there for a process that died without an onPause.
    private static final int FOREGROUND_SEC = HOUR_SEC;
    // {away for less than, wait}, without the server's push: the check is the only source of
    // notifications, so it starts a minute after the player left and backs off as they stay
    // away. Past the first day these are the tiers of the caster server's own battle watch.
    private static final int[][] POLL_LADDER = {
        {120, 60}, {180, 120}, {240, 180}, {300, 240}, {360, 300}, {600, 600}, {900, 900},
        {1200, 1200}, {1800, 1800}, {DAY_SEC, HOUR_SEC}, {3 * DAY_SEC, 3 * HOUR_SEC}
    };
    private static final int POLL_SLOWEST_SEC = DAY_SEC;
    // With the server's push only a finished battle and the reminder are left to find.
    private static final int PUSH_POLL_SEC = 3 * HOUR_SEC;

    public static void schedule(Context context) {
        try {
            int next_alert_in_sec = getNextAlertTimeoutSec(context);
            Log.d(TAG, "next_alert_in_sec = " + next_alert_in_sec);
            if (next_alert_in_sec <= 0) {
                cancel(context);
                return;
            }

            Constraints constraints = new Constraints.Builder()
                    .setRequiredNetworkType(NetworkType.CONNECTED)
                    .build();
            OneTimeWorkRequest request = new OneTimeWorkRequest.Builder(CheckStatusWorker.class)
                    .setInitialDelay(next_alert_in_sec, TimeUnit.SECONDS)
                    .setConstraints(constraints)
                    .build();
            WorkManager.getInstance(context)
                    .enqueueUniqueWork(WORK_NAME, ExistingWorkPolicy.REPLACE, request);
        } catch (Exception e) {
            e.printStackTrace();
        }
        Log.d(TAG, "finish");
    }

    public static void cancel(Context context) {
        try {
            WorkManager.getInstance(context).cancelUniqueWork(WORK_NAME);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // POST_NOTIFICATIONS is a genuine runtime permission from Android 13 on, so the answer
    // can change at any time from system settings - always ask, never cache.
    public static boolean canPostNotifications(Context context) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.TIRAMISU) {
            return true;
        }
        try {
            return context.checkSelfPermission(Manifest.permission.POST_NOTIFICATIONS)
                    == PackageManager.PERMISSION_GRANTED;
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    // Seconds to the next check, 0 when none is to be queued.
    public static int getNextAlertTimeoutSec(Context context) {
        try {
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            return nextCheckDelaySec(System.currentTimeMillis() / 1000L,
                    sharedPreferences.getInt("app_last_activity", 0),
                    sharedPreferences.getInt("last_notification", 0),
                    !sharedPreferences.getString("check_url", "").isEmpty(),
                    canPostNotifications(context),
                    CheckStatus.serverPushActive(sharedPreferences),
                    MainActivity.isForeground);
        } catch (Exception e) {
            e.printStackTrace();
            return HOUR_SEC;
        }
    }

    // The rule of CheckStatus.performCheck: a notification is up and the player has not been
    // in the app since, so nothing more is posted until MUTE_SEC have passed. Unix seconds.
    public static boolean muted(long now, long lastActivity, long lastNotification) {
        return (lastNotification > 0) && (lastNotification >= lastActivity)
                && (now - lastNotification <= MUTE_SEC);
    }

    // Seconds until a check could have something to say, 0 when there is nothing to wait
    // for. No Android in here; the times are unix seconds.
    public static int nextCheckDelaySec(long now, long lastActivity, long lastNotification,
                                        boolean loggedIn, boolean canNotify, boolean pushActive,
                                        boolean foreground) {
        if (!loggedIn || !canNotify) {
            return 0;
        }
        if (foreground) {
            return FOREGROUND_SEC;
        }
        if (muted(now, lastActivity, lastNotification)) {
            return (int) Math.min(lastNotification + MUTE_SEC - now, MUTE_SEC) + MUTE_MARGIN_SEC;
        }
        long idle_sec = (lastActivity > 0) ? (now - lastActivity) : 0;
        if (pushActive) {
            return (idle_sec < 3 * DAY_SEC) ? PUSH_POLL_SEC : POLL_SLOWEST_SEC;
        }
        for (int[] step : POLL_LADDER) {
            if (idle_sec < step[0]) {
                return step[1];
            }
        }
        return POLL_SLOWEST_SEC;
    }
}
