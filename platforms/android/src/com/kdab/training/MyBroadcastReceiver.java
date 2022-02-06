package com.kdab.training;

import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;
import android.util.Log;
import android.app.Service;
import android.os.Build;
import com.kdab.training.AlarmReceiver;

public class MyBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "MyBroadcastReceiver";
    private static final int START_ID = 888888;

    @Override
    public void onReceive(Context context, Intent intent) {
        //Intent startServiceIntent = new Intent(context, MyService.class);
        //context.startService(startServiceIntent);
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//            context.startForegroundService(new Intent(context, MyService.class));
//        } else {
//            context.startService(new Intent(context, MyService.class));
//        }
        AlarmReceiver alarm = new AlarmReceiver();
        alarm.setAlarm(context, true);
    }

    public static void startService(Context context, String log_string) {
        Log.i(TAG, "startService " + log_string);
        //Intent startServiceIntent = new Intent(context, MyService.class);
        //context.startService(startServiceIntent/*, START_NOT_STICKY, START_ID*/);
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//            context.startForegroundService(new Intent(context, MyService.class));
//        } else {
//            context.startService(new Intent(context, MyService.class));
//        }

        AlarmReceiver alarm = new AlarmReceiver();
        alarm.setAlarm(context, true);
    }
}
