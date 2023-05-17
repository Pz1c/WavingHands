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

public class NotificationClient
{
    public static void notify(Context context, String message) {
        try {
            NotificationManager m_notificationManager = (NotificationManager)
                    context.getSystemService(Context.NOTIFICATION_SERVICE);

            Notification.Builder m_builder;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel notificationChannel;
                notificationChannel = new NotificationChannel("WarlockDuel", "Warlock Duel", importance);
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

    public static void showToast(Context context, String msg) {
        Toast.makeText(context, msg, Toast.LENGTH_SHORT).show();
    }
}
