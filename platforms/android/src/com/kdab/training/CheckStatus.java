package com.kdab.training;

import android.app.Service;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;
import android.widget.Toast;
import android.content.SharedPreferences;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.app.NotificationChannel;
import net.is.games.WarlocksDuel.R;
import android.os.Bundle;
import android.view.View;

class MyJavaNatives
{
    // declare the native method
    public static native void checkWarlockProfile(String url);
}

public class CheckStatus extends Service {

    // This method run only one time. At the first time of service created and running
    @Override
    public void onCreate() {
        HandlerThread thread = new HandlerThread("ServiceStartArguments", Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();
        Log.d("CheckStatus.onCreate()", "After service created");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        //Here is the source of the TOASTS :D
        String check_url = getCheckUrl(getApplicationContext());
        Toast.makeText(this, "Freshly Made toast!\n"+check_url, Toast.LENGTH_SHORT).show();
        MyJavaNatives.checkWarlockProfile(check_url);

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // We don't provide binding
        return null;
    }

    public static String getCheckUrl(Context context) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          String res = sharedPreferences.getString("check_url", "");
          return res;
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }
}
