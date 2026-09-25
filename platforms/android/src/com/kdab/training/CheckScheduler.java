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
 * The timing ladder itself is unchanged: getNextAlertTimeoutSec() decides how long to
 * wait, and each run queues the next one from CheckStatusWorker.
 */
public class CheckScheduler {
    private static final String TAG = "WarlocksDuel.CheckScheduler";
    private static final String WORK_NAME = "warlocks_duel_check_status";

    public static void schedule(Context context, boolean Initial) {
        try {
            int next_alert_in_sec = getNextAlertTimeoutSec(context, Initial);
            Log.d(TAG, "next_alert_in_sec = " + next_alert_in_sec);

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

    public static int getNextAlertTimeoutSec(Context context, boolean Initial) {
        int curr_time = Math.round(System.currentTimeMillis()/1000L);
        int last_activity, last_notification;
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          last_activity = sharedPreferences.getInt("app_last_activity", 0);
          last_notification = sharedPreferences.getInt("last_notification", 0);
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
        }
        if ((last_notification != 0) && (last_activity != 0) && (last_notification > last_activity)) {
            return 30 * 60;
        }

        if (Initial || (last_activity == 0)) {
            return 60;
        }
        int idle_sec = curr_time - last_activity;
        if ((idle_sec <= 0) || ((idle_sec) >= 60 && (idle_sec < 120))) {
            return 60;
        }
        if ((idle_sec >= 120) && (idle_sec < 180)) {
            return 120;
        }
        if ((idle_sec >= 180) && (idle_sec < 240)) {
            return 180;
        }
        if ((idle_sec >= 240) && (idle_sec < 300)) {
            return 240;
        }
        if ((idle_sec >= 300) && (idle_sec < 360)) {
            return 300;
        }
        if ((idle_sec >= 360) && (idle_sec < 600)) {
            return 600;
        }
        if ((idle_sec >= 600) && (idle_sec < 900)) {
            return 900;
        }
        if ((idle_sec >= 900) && (idle_sec < 1200)) {
            return 1200;
        }
        if ((idle_sec >= 1200) && (idle_sec < 1800)) {
            return 1800;
        }
        //if ((idle_sec >= 1800)) {
            return 3600;
        //}
    }
}
