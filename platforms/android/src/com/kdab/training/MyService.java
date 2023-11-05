// java file goes in android/src/com/kdab/training/MyService.java
package com.kdab.training;

import android.content.Context;
import android.content.Intent;
import android.util.Log;
import org.qtproject.qt.android.bindings.QtService;
import android.app.Notification;
import android.app.PendingIntent;
import android.os.IBinder;
import java.util.concurrent.TimeUnit;

public class MyService extends QtService
{
    private static final String TAG = "WarlocksDuelService";
    private static final String LOG_TAG = "WarlocksDuelService";

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "Destroying Service");
    }

    @Override
    /*public int onStartCommand(Intent intent, int flags, int startId) {
        int ret = super.onStartCommand(intent, flags, startId);
        Log.i(TAG, "onStartCommand: " + ret);
        // Do some work

        return START_REDELIVER_INTENT;
    }*/

    public int onStartCommand(Intent intent, int flags, int startId) {
    Log.d(LOG_TAG, "MyService onStartCommand");
    try {
        readFlags(flags);
        MyRun mr = new MyRun(startId);
        new Thread(mr).start();
    } catch (Exception e) {
        e.printStackTrace();
        //return "";
    }
    return START_STICKY;
  }

  public IBinder onBind(Intent arg0) {
    return null;
  }

  void readFlags(int flags) {
      try {
    if ((flags&START_FLAG_REDELIVERY) == START_FLAG_REDELIVERY)
      Log.d(LOG_TAG, "START_FLAG_REDELIVERY");
    if ((flags&START_FLAG_RETRY) == START_FLAG_RETRY)
      Log.d(LOG_TAG, "START_FLAG_RETRY");
      } catch (Exception e) {
          e.printStackTrace();
          //return "";
      }
  }

  class MyRun implements Runnable {

    int startId;

    public MyRun(int startId) {
      try {
          this.startId = startId;
          Log.d(LOG_TAG, "MyRun#" + startId + " create");
      } catch (Exception e) {
          e.printStackTrace();
          //return "";
      }
    }

    public void run() {
      Log.d(LOG_TAG, "MyRun#" + startId + " start");
      try {
        while(true) {
          TimeUnit.SECONDS.sleep(60);
          Log.d(LOG_TAG, "MyRun#.work");
        }
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
      stop();
    }

    void stop() {
        try{
            Log.d(LOG_TAG, "MyRun#" + startId + " end, stopSelfResult("
            + startId + ") = " + stopSelfResult(startId));
        } catch (Exception e) {
          e.printStackTrace();
          //return "";
        }
    }
  }
}
