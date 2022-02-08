package com.kdab.training;

import org.qtproject.qt.android.bindings.QtActivity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;

public class MainActivity extends QtActivity {
    private static final int STORAGE_PERMISSION_CODE = 1;
    private static final String TAG = "WarlockDuelActivity";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // prepare service
        AlarmReceiver alarm = new AlarmReceiver();
        alarm.setAlarm(this, true);

        // Get the intent that started this activity
        Intent intent = getIntent();
        Uri data = intent.getData();
        Bundle bundle = intent.getExtras();
        Log.d(TAG, "check intent");
        Log.d(TAG, "data: " + data.toString());
        Log.d(TAG, "extras: " + bundle.toString());
    }
}
