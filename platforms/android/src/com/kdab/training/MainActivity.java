package com.kdab.training;

import org.qtproject.qt.android.bindings.QtActivity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;
import android.net.Uri;
import android.content.SharedPreferences;

import com.android.installreferrer.api.InstallReferrerClient;
import com.android.installreferrer.api.InstallReferrerStateListener;
import com.android.installreferrer.api.ReferrerDetails;

public class MainActivity extends QtActivity {
    private static final int STORAGE_PERMISSION_CODE = 1;
    private static final String TAG = "WarlockDuelActivity";

    // variable for install referer client.
    InstallReferrerClient referrerClient;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // prepare service
        AlarmReceiver alarm = new AlarmReceiver();
        alarm.setAlarm(this, true);

        int need_check_ref = 0;
        try {
            Context context = getApplicationContext();
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            need_check_ref = sharedPreferences.getInt("referrer_checked", 0);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (need_check_ref == 0) {
            // on below line we are building our install referrer client and building it.
            referrerClient = InstallReferrerClient.newBuilder(this).build();

            // on below line we are starting its connection.
            referrerClient.startConnection(new InstallReferrerStateListener() {
                @Override
                public void onInstallReferrerSetupFinished(int responseCode) {
                    // this method is called when install referer setup is finished.
                    switch (responseCode) {
                        // we are using switch case to check the response.
                        case InstallReferrerClient.InstallReferrerResponse.OK:
                            // this case is called when the status is OK and
                            ReferrerDetails response = null;
                            try {
                                // on below line we are getting referrer details
                                // by calling get install referrer.
                                response = referrerClient.getInstallReferrer();
                                // on below line we are getting referrer url.
                                String referrerUrl = response.getInstallReferrer();
                                // on below line we are getting referrer click time.
                                long referrerClickTime = response.getReferrerClickTimestampSeconds();
                                // on below line we are getting app install time
                                long appInstallTime = response.getInstallBeginTimestampSeconds();
                                // on below line we are getting our time when
                                // user has used our apps instant experience.
                                boolean instantExperienceLaunched = response.getGooglePlayInstantParam();
                                // on below line we are getting our
                                // apps install referrer.
                                //refrer = response.getInstallReferrer();

                                // on below line we are setting all detail to our text view.
                                //refrerTV.setText("Referrer is : \n" + referrerUrl + "\n" + "Referrer Click Time is : " + referrerClickTime + "\nApp Install Time : " + appInstallTime);
                                Log.d(TAG, "Referrer is : \n" + referrerUrl + "\n" + "Referrer Click Time is : " + referrerClickTime + "\nApp Install Time : " + appInstallTime);

                                try {
                                    Context context = getApplicationContext();
                                    SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
                                    SharedPreferences.Editor editor = sharedPreferences.edit();
                                    editor.putString("referrer", referrerUrl);
                                    //editor.putInt("referrer_checked", 1);
                                    editor.commit();
                                } catch (Exception e) {
                                    e.printStackTrace();
                                }
                            } catch (Exception e) {
                                // handling error case.
                                e.printStackTrace();
                            }
                            break;
                        case InstallReferrerClient.InstallReferrerResponse.FEATURE_NOT_SUPPORTED:
                            // API not available on the current Play Store app.
                            try {
                                Context context = getApplicationContext();
                                SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
                                SharedPreferences.Editor editor = sharedPreferences.edit();
                                editor.putInt("referrer_checked", 1);
                                editor.commit();
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                            break;
                        case InstallReferrerClient.InstallReferrerResponse.SERVICE_UNAVAILABLE:
                            // Connection couldn't be established.
                            //Toast.makeText(MainActivity.this, "Fail to establish connection", Toast.LENGTH_SHORT).show();
                            break;
                    }
                }

                @Override
                public void onInstallReferrerServiceDisconnected() {
                    // Try to restart the connection on the next request to
                    // Google Play by calling the startConnection() method.
                    //Toast.makeText(MainActivity.this, "Service disconnected..", Toast.LENGTH_SHORT).show();
                }
            });
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
                    Context context = getApplicationContext();
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
