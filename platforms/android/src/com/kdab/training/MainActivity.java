package com.kdab.training;

import org.qtproject.qt.android.bindings.QtActivity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;
import android.net.Uri;
import android.content.SharedPreferences;

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
        if (data != null) {
            Log.d(TAG, "data: " + data.toString());
        }
        if (bundle != null) {
            Log.d(TAG, "extras: " + bundle.toString());
            int action_type = bundle.getInt("action_type");
            int battle_id = bundle.getInt("battle_id");
            Log.d(TAG, "action_type: " + action_type);
            Log.d(TAG, "battle_id: " + battle_id);
        }
    }

    @Override
    public void onResume () {
        super.onResume();

        Intent intent = getIntent();
        Uri data = intent.getData();
        Bundle bundle = intent.getExtras();
        Log.d(TAG, "onResume check intent");
        if (data != null) {
            Log.d(TAG, "onResume data: " + data.toString());
        }
        if (bundle != null) {
            Log.d(TAG, "onResume extras: " + bundle.toString());
            int action_type = bundle.getInt("action_type");
            int battle_id = bundle.getInt("battle_id");
            Log.d(TAG, "onResume action_type: " + action_type);
            Log.d(TAG, "onResume battle_id: " + battle_id);
            if (battle_id > 0) {
                try {
                    SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    editor.putInt("action_type", action_type);
                    editor.putInt("battle_id", battle_id);
                    editor.commit();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
