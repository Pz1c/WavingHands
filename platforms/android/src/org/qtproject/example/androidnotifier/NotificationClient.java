package org.qtproject.example.androidnotifier;

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
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;
import android.content.res.Resources;
import android.util.Log;

import com.google.firebase.messaging.FirebaseMessaging;

import com.kdab.training.CheckScheduler;
import com.kdab.training.MainActivity;
import com.kdab.training.PushService;

public class NotificationClient
{
    private static final String TAG = "WarlocksDuel.NotificationClient";

    public static void notify(Context context, String message) {
        try {
            NotificationManager m_notificationManager = (NotificationManager)
                    context.getSystemService(Context.NOTIFICATION_SERVICE);

            Notification.Builder m_builder;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel notificationChannel;
                notificationChannel = new NotificationChannel("WarlocksDuel", "Warlock Duel", importance);
                m_notificationManager.createNotificationChannel(notificationChannel);
                m_builder = new Notification.Builder(context, notificationChannel.getId());
            } else {
                m_builder = new Notification.Builder(context);
            }

            Bitmap icon = BitmapFactory.decodeResource(context.getResources(), R.drawable.icon);
            m_builder.setSmallIcon(R.drawable.icon)
                    .setLargeIcon(icon)
                    .setContentTitle("Warlock duel need your attention")
                    .setContentText(message)
                    .setDefaults(Notification.DEFAULT_SOUND)
                    .setColor(Color.GREEN)
                    .setAutoCancel(true);

            m_notificationManager.notify(0, m_builder.build());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void hideNotification(Context context) {
        try {
            NotificationManager m_notificationManager = (NotificationManager)
                    context.getSystemService(Context.NOTIFICATION_SERVICE);

            m_notificationManager.cancel(0);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public static void share(Context context, String message) {
        try {
          Intent sendIntent = new Intent();
          sendIntent.setAction(Intent.ACTION_SEND);
          sendIntent.putExtra(Intent.EXTRA_TEXT, message);
          sendIntent.setType("text/plain");

          Intent shareIntent = Intent.createChooser(sendIntent, null);
          context.startActivity(shareIntent);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    
    public static String get_refferer(Context context, String message) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          //SharedPreferences sharedPreferences = getPreferences(MODE_PRIVATE);
          String res = sharedPreferences.getString("referrer", "");
          SharedPreferences.Editor editor = sharedPreferences.edit();
          // fix for issue #193
          editor.putString("finished_games", message);
          editor.putInt("app_last_activity", Math.round(System.currentTimeMillis()/1000L));
          if (!res.isEmpty()) {
            editor.putString("referrer", "");
            editor.commit();
            return "create_battle,"+res;
          }
          int action_type = sharedPreferences.getInt("action_type", 0);
          int battle_id = sharedPreferences.getInt("battle_id", 0);
          if (battle_id > 0) {
              editor.putInt("action_type", 0);
              editor.putInt("battle_id", 0);
              editor.commit();
              return "show_battle,"+battle_id+","+action_type;
          }
          editor.commit();
          return "";
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    public static String get_screen_size(Context context, String message) {
        try {
            int w = Resources.getSystem().getDisplayMetrics().widthPixels;
            int h = Resources.getSystem().getDisplayMetrics().heightPixels;
            return w + "," + h;
        } catch (Exception e) {
            e.printStackTrace();
            return "0,0";
        }
    }


    public static String isNotificationAllowed(Context context, String message) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          SharedPreferences.Editor editor = sharedPreferences.edit();
          editor.putInt("app_last_activity", Math.round(System.currentTimeMillis()/1000L));
          editor.commit();
          // Asked live rather than read back from a "set_alarm_allowed" pref, so revoking
          // notifications in system settings is reflected straight away.
          return CheckScheduler.canPostNotifications(context) ? "1" : "0";
        } catch (Exception e) {
            e.printStackTrace();
            return "0";
        }
    }

    public static void askPermission(Context context, String message) {
        try {
            // MainActivity owns the whole flow: POST_NOTIFICATIONS at runtime, then the
            // exact alarm settings screen. Requesting SCHEDULE_EXACT_ALARM here did nothing.
            MainActivity.appMainActivity.askPermission();
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
    }


    public static void setLastActivity(Context context, String message) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          //SharedPreferences sharedPreferences = getPreferences(MODE_PRIVATE);
          //String res = sharedPreferences.getString("app_last_activity", "");
          SharedPreferences.Editor editor = sharedPreferences.edit();
          editor.putInt("app_last_activity", Math.round(System.currentTimeMillis()/1000L));
          editor.putString("check_url", message);
          editor.commit();
          //return res;
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
    }

    public static void setCheckUrl(Context context, String url) {
        try {
          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
          SharedPreferences.Editor editor = sharedPreferences.edit();
          editor.putInt("app_last_activity", Math.round(System.currentTimeMillis()/1000L));
          editor.putString("check_url", url);
          editor.commit();
          //return res;
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
    }

    // https://github.com/Pz1c/WavingHands/issues/336
    // The FCM token of this install, "" while unknown. An empty answer starts the fetch; the
    // token then lands in the prefs (PushService.storeToken) and reaches the core through
    // pushTokenChanged.
    public static String getPushToken(Context context, String message) {
        try {
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            String token = sharedPreferences.getString("fcm_token", "");
            if (!token.isEmpty()) {
                return token;
            }
            fetchPushToken(context, message);
            return "";
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    // FirebaseMessaging.getToken(), whatever the prefs hold: the first fetch of an install,
    // and the forced send of the Settings window. The token, or the failure, goes to the
    // core through pushTokenChanged (the token also to the prefs, PushService.storeToken).
    public static void fetchPushToken(Context context, String message) {
        try {
            final Context app = context.getApplicationContext();
            Log.d(PushService.PUSH_TAG, "fetching the token from Firebase");
            FirebaseMessaging.getInstance().getToken().addOnCompleteListener(task -> {
                if (!task.isSuccessful() || task.getResult() == null || task.getResult().isEmpty()) {
                    Exception e = task.getException();
                    String why = (e == null) ? "no token" : e.toString();
                    Log.w(PushService.PUSH_TAG, "getToken failed: " + why);
                    deliverPushToken("", why);
                    return;
                }
                Log.d(PushService.PUSH_TAG, "getToken " + PushService.redact(task.getResult()));
                PushService.storeToken(app, task.getResult());
            });
        } catch (Exception e) {
            e.printStackTrace();
            deliverPushToken("", e.toString());
        }
    }

    // The core binds this (QJniEnvironment::registerNativeMethods, registerPushNatives) as
    // soon as it is up. In the process FCM starts without the Qt library it stays unbound,
    // and the token just waits in the prefs for the next start of the app.
    public static native void pushTokenChanged(String token, String error);

    public static void deliverPushToken(String token, String error) {
        String t = (token == null) ? "" : token;
        String e = (error == null) ? "" : error;
        try {
            pushTokenChanged(t, e);
            Log.d(PushService.PUSH_TAG, "token handed to the core " + PushService.redact(t));
        } catch (UnsatisfiedLinkError err) {
            Log.d(PushService.PUSH_TAG, "core not running, token kept in the prefs " + PushService.redact(t));
        } catch (Throwable err) {
            err.printStackTrace();
        }
    }

    // "1" once the caster server acknowledged the PUSH of this install's token, "0" when the
    // binding is gone (UNPUSH, logout, a server without push). CheckStatus.performCheck
    // leaves ready and challenge to the server while this is fresh (7 days).
    public static void setServerPushActive(Context context, String flag) {
        try {
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            SharedPreferences.Editor editor = sharedPreferences.edit();
            boolean active = "1".equals(flag);
            editor.putInt("server_push_active", active ? 1 : 0);
            editor.putLong("server_push_active_ts", System.currentTimeMillis() / 1000L);
            editor.commit();
            Log.d(TAG, "setServerPushActive " + (active ? 1 : 0));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void showToast(Context context, String msg) {
        try {
            Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
        } catch (Exception e) {
            e.printStackTrace();
            //return "";
        }
    }
}
