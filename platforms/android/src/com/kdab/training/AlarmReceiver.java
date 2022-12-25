package com.kdab.training;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;
import android.os.Build;

public class AlarmReceiver extends BroadcastReceiver
{
    private static final String TAG = "WarlockDuel.AlarmReceiver";

    @Override
    public void onReceive(Context context, Intent intent)
    {
        Intent in = new Intent(context, CheckStatus.class);
        context.startService(in);
        setAlarm(context, false);
    }

    public void setAlarm(Context context, boolean Initial)
    {
        Log.d(TAG, "setAlarm");
        AlarmManager am = (AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
        Intent i = new Intent(context, AlarmReceiver.class);
        PendingIntent pi;// = PendingIntent.getBroadcast(context, 0, i, 0);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
          pi = PendingIntent.getBroadcast(context, 0, i, PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_MUTABLE);
        } else {
          pi = PendingIntent.getBroadcast(context, 0, i, PendingIntent.FLAG_UPDATE_CURRENT);
        }

        assert am != null;
        int next_alert_in_sec = getNextAlertTimeoutSec(context, Initial);
        Log.d(TAG, "next_alert_in_sec = " + next_alert_in_sec);
        am.cancel(pi); // https://developer.android.com/reference/android/app/AlarmManager#cancel(android.app.PendingIntent)
        am.setExactAndAllowWhileIdle(AlarmManager.RTC_WAKEUP, (System.currentTimeMillis()/1000L + next_alert_in_sec) *1000L, pi); //Next alarm in 15s
        Log.d(TAG, "finish");
    }

    public static int getLastActivity(Context context) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          int res = sharedPreferences.getInt("app_last_activity", 0);
          return res;
        } catch (Exception e) {
            e.printStackTrace();
            return 0;
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
