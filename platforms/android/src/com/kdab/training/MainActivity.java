package com.kdab.training;

import org.qtproject.qt.android.bindings.QtActivity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.os.Bundle;
import android.os.Build;
import android.net.Uri;
import android.content.SharedPreferences;

import com.android.installreferrer.api.InstallReferrerClient;
import com.android.installreferrer.api.InstallReferrerStateListener;
import com.android.installreferrer.api.ReferrerDetails;

import android.content.pm.PackageManager;
import android.Manifest;
import androidx.core.app.ActivityCompat;


public class MainActivity extends QtActivity {
    private static final int STORAGE_PERMISSION_CODE = 1;
    private static final int NOTIFICATION_PERMISSION_CODE = 123;
    private static final String TAG = "WarlocksDuelActivity";
    public static MainActivity appMainActivity;

    // variable for install referer client.
    InstallReferrerClient referrerClient;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        appMainActivity = this;
        // prepare service
        try {
            CheckScheduler.schedule(this, true);
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
        int need_check_ref = 0;
        try {
            Context context = getApplicationContext();
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            need_check_ref = sharedPreferences.getInt("referrer_checked", 0);
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            CheckStatus.hideNotification(this);
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }

        if (need_check_ref == 0) {
            try {
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
                                    editor.putInt("referrer_checked", 1);
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
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
        }
    }

    @Override
    public void onResume () {
        super.onResume();
        try {
            // The user may have changed the notification setting while we were away, and
            // the poll cadence depends on when they were last active - re-queue on resume.
            CheckScheduler.schedule(this, true);
        } catch (Exception e) {
            e.printStackTrace();
        }
        try {
        Intent intent = getIntent();
        Uri data = intent.getData();
        Bundle bundle = intent.getExtras();
        Log.d(TAG, "onResume check intent");

        // https://github.com/Pz1c/WavingHands/issues/258
        CheckStatus.hideNotification(this);

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
    } catch (Exception e) {
        e.printStackTrace();
        //return "";
    }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        Log.d(TAG, "onRequestPermissionsResult: " + requestCode);
        if (requestCode == NOTIFICATION_PERMISSION_CODE) {
            boolean granted = (grantResults.length > 0) && (grantResults[0] == PackageManager.PERMISSION_GRANTED);
            Log.d(TAG, "POST_NOTIFICATIONS granted: " + granted);
            CheckScheduler.schedule(this, true);
        }
    }

    public void askPermission() {
        try {
            // POST_NOTIFICATIONS is the only permission the notifications need now; the
            // background poll runs on WorkManager and asks for nothing.
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU
                    && checkSelfPermission(Manifest.permission.POST_NOTIFICATIONS) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this,
                                    new String[]{Manifest.permission.POST_NOTIFICATIONS},
                                    NOTIFICATION_PERMISSION_CODE);
                return;
            }
            CheckScheduler.schedule(this, true);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
