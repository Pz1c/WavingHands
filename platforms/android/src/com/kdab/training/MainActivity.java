package com.kdab.training;

import android.os.Bundle;
import org.qtproject.qt.android.bindings.QtActivity;

public class MainActivity extends QtActivity {
    private static final int STORAGE_PERMISSION_CODE = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        AlarmReceiver alarm = new AlarmReceiver();
        alarm.setAlarm(this);
    }
}