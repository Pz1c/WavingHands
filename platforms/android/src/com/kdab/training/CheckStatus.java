package com.kdab.training;

import android.app.Service;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Process;
import android.util.Log;
import android.widget.Toast;



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
        Toast.makeText(this, "Freshly Made toast!", Toast.LENGTH_SHORT).show();
        
        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // We don't provide binding
        return null;
    }

}