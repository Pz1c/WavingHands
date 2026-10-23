package com.kdab.training;

import android.content.Intent;
import android.util.Log;
import android.widget.Toast;
import android.content.SharedPreferences;
import android.app.Activity;
import android.app.Notification;
import android.app.NotificationManager;
import net.is.games.WarlocksDuel.R;
import android.os.Bundle;
import android.view.View;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import javax.net.ssl.HttpsURLConnection;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.app.NotificationChannel;
import android.app.Notification.Action;
import android.app.PendingIntent;
import com.kdab.training.MainActivity;
import android.net.Uri;
import java.util.Arrays;
import android.os.Build;

class MyJavaNatives
{
    // declare the native method
    public static native void checkWarlockProfile(String url);
}

public class CheckStatus {
    private static final String TAG = "WarlocksDuel.CheckStatus";

    // One profile check, run synchronously on whatever background thread the caller
    // provides. Used to be the body of a Service started by an exact alarm.
    public static void performCheck(Context context) {
        Log.d(TAG, "performCheck");
        try {
                SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
                SharedPreferences.Editor editor = sharedPreferences.edit();
                String arr_finished_old_str = sharedPreferences.getString("finished_games", "");
                String[] arr_finished_old = arr_finished_old_str.split(",");
                String check_url = sharedPreferences.getString("check_url", "");
                if (check_url.isEmpty()) {
                    return;
                }
                int last_sent_notif = sharedPreferences.getInt("last_notification", 0);
                int app_last_activity = sharedPreferences.getInt("app_last_activity", 0);
                int last_notification = Math.round(System.currentTimeMillis()/1000L);

                String data;
                try {
                    data = getContent(check_url);
                } catch (IOException ex){
                    ex.printStackTrace();
                    return;
                }
                Log.d(TAG, "getContent: " + check_url + " datalength " + data.length());
                String arr_ready_str = parseBattlesFromData(data, "Ready in battles:");
                String[] arr_ready = arr_ready_str.split(",");
                String arr_challenge_str = parseBattlesFromData(data, "Challenged to battles:");
                String[] arr_challenge = arr_challenge_str.split(",");
                String finished_new = parseBattlesFromData(data, "Finished battles:");
                editor.putString("finished_games", finished_new);
                editor.commit();
                String[] arr_finished = finished_new.split(",");
                String[] arr_finished_clean;
                Log.d(TAG, "r=" + arr_ready_str + ", c=" + arr_challenge_str + ", fn=" + finished_new + ", fo=" + arr_finished_old_str);
                Log.d(TAG, "last_sent_notif=" + last_sent_notif + ", app_last_activity=" + app_last_activity + ", last_notification=" + last_notification);
                boolean need_send = (last_sent_notif == 0) || (last_sent_notif < app_last_activity) ||
                                    ((last_sent_notif > 0) && ((last_notification - last_sent_notif) > 60 * 60 * 8));
                boolean app_inactive = ((last_notification - app_last_activity) >= 60);
                boolean ready = need_send && (arr_ready.length > 0) && (!(arr_ready[0]).isEmpty()) && app_inactive;
                boolean challenge = need_send && (arr_challenge.length > 0) && (!arr_challenge[0].isEmpty());
                boolean finished = false;
                int finished_id = 0;
                if (app_inactive && need_send) {
                    for (String battleId : arr_finished) {
                       if(java.util.Arrays.asList(arr_finished_old).indexOf(battleId) == -1) {
                           finished = true;
                           if (finished_id == 0) {
                               finished_id = Integer.parseInt(battleId);
                           } else {
                               finished_id = -2;
                           }
                       }
                    }
                }
                boolean reminder = !(ready || challenge || finished) && (last_sent_notif > 0)
                                && (app_last_activity > 0) && (app_last_activity < last_sent_notif)
                                && ((last_notification - last_sent_notif) > 3 * 24 * 60 * 60);

                // https://github.com/Pz1c/WavingHands/issues/336: with a fresh push registration
                // the caster server tells the player about these two itself. Finished and the
                // reminder stay with the poll.
                if ((ready || challenge) && serverPushActive(sharedPreferences)) {
                    Log.d(TAG, "ready/challenge left to the server push");
                    ready = false;
                    challenge = false;
                }

                if (ready || challenge || finished || reminder) {
                    Log.d(TAG, "need notif");
                    int action_type, battle_id = 0;
                    if (challenge) {
                        action_type = 1;
                        if ((arr_challenge.length == 1) && (!arr_challenge[0].isEmpty())) {
                            battle_id = Integer.parseInt(arr_challenge[0]);
                        }
                    } else if (ready) {
                        action_type = 2;
                        if ((arr_ready.length == 1) && (!arr_ready[0].isEmpty())) {
                            battle_id = Integer.parseInt(arr_ready[0]);
                        }
                    } else if (finished) {
                        action_type = 3;
                        if ((finished_id != 0) && (finished_id != -2)) {
                            battle_id = finished_id;
                        }
                    } else {
                        action_type = 4;
                    }
                    notifyType(context, action_type, battle_id);
                    editor.putInt("notification_action_type", action_type);
                    editor.putInt("notification_battle_id", battle_id);
                    editor.putInt("last_notification", last_notification);
                }
                editor.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static String getContent(String path) throws IOException {
            BufferedReader reader=null;
            InputStream stream = null;
            HttpsURLConnection connection = null;
            try {
                URL url=new URL(path);
                connection =(HttpsURLConnection)url.openConnection();
                connection.setRequestMethod("GET");
                connection.setReadTimeout(10000);
                connection.connect();
                stream = connection.getInputStream();
                reader= new BufferedReader(new InputStreamReader(stream));
                StringBuilder buf=new StringBuilder();
                String line;
                while ((line=reader.readLine()) != null) {
                    buf.append(line).append("\n");
                }
                return(buf.toString());
            }
            finally {
                if (reader != null) {
                    reader.close();
                }
                if (stream != null) {
                    stream.close();
                }
                if (connection != null) {
                    connection.disconnect();
                }
            }
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

    public static String parseBattlesFromData(String data, String search) {
        String result = "";
        int idx1 = data.indexOf(search);
        if (idx1 == -1) {
            return "";
        }
        int idx2 = idx1, idx3 = data.indexOf("</TR>", idx1), idx4, idx5;
        while((idx2 = data.indexOf("?num=", idx2)) != -1) {
            if (idx2 >= idx3) {
                break;
            }
            idx4 = data.indexOf(">", idx2);
            if (idx4 == -1) {
                break;
            }
            idx5 = data.indexOf("<", ++idx4);
            if (idx5 == -1) {
                break;
            }
            idx2 = idx5;
            String res = data.substring(idx4, idx5);
            if (!result.isEmpty()) {
                result += ",";
            }
            result += res;
        }

        return result;
    }

    // The caster server confirmed a push registration less than 7 days ago (issue #336): it
    // covers ready and challenge, the poll leaves them to it. Without a fresh confirmation
    // (server down, app not opened, permission lost) the poll takes over again by itself.
    public static boolean serverPushActive(SharedPreferences sharedPreferences) {
        try {
            if (sharedPreferences.getInt("server_push_active", 0) != 1) {
                return false;
            }
            long ts = sharedPreferences.getLong("server_push_active_ts", 0L);
            long now = System.currentTimeMillis() / 1000L;
            return (ts > 0) && (now - ts < 7L * 24 * 60 * 60);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    // Notification ids, one per action type (1 invite, 2 ready, 3 finished, 4 reminder) so an
    // invite and a ready notification can stand side by side. 0 is what older builds posted;
    // hideNotification still cancels it.
    public static final int[] NOTIFICATION_IDS = {0, 1, 2, 3, 4};

    // The notification of one action type: message, button and intent extras in one place,
    // shared by the poll above and by the push (PushService). battle_id 0 = none or several.
    public static void notifyType(Context context, int action_type, int battle_id) {
        String msg, btn_title = "Play";
        Bundle msg_data = new Bundle();
        switch (action_type) {
            case 1:
                msg = "You got an invite, see invitation";
                break;
            case 2:
                msg = "Good news, you've got a new turn to play!";
                break;
            case 3:
                msg = "Your battle is over!";
                btn_title = "Show me";
                break;
            default:
                action_type = 4;
                msg = "Great news! You got a new match";
                btn_title = "Show me";
                break;
        }
        msg_data.putInt("action_type", action_type);
        if (battle_id > 0) {
            msg_data.putInt("battle_id", battle_id);
        }
        Log.d(TAG, msg);
        notify(context, msg, btn_title, msg_data, action_type);
    }

    public static void notify(Context context, String message, String btn_title, Bundle data, int notification_id) {
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

            Intent resultIntent = new Intent(context, MainActivity.class);
            //if (data != null) {
                resultIntent.putExtras(data);
            //}
            // The request code is the notification id: two notifications up at once must not
            // share one PendingIntent, or FLAG_UPDATE_CURRENT gives both the extras of the
            // one posted last.
            PendingIntent resultPendingIntent;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                resultPendingIntent = PendingIntent.getActivity(context, notification_id, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
            }else {
                resultPendingIntent = PendingIntent.getActivity(context, notification_id, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT);
            }
            Bitmap icon = BitmapFactory.decodeResource(context.getResources(), R.drawable.icon);
            m_builder//.setSmallIcon(icon)
                    .setSmallIcon(R.drawable.notification_icon)
                    .setLargeIcon(icon)
                    .setContentTitle(message)
                    //.setContentText("Warlock duel need your attention")
                    .setDefaults(Notification.DEFAULT_SOUND)
                    //.setColor(Color.GREEN)
                    .setAutoCancel(true)
                    .setContentIntent(resultPendingIntent);

            if (!btn_title.isEmpty()) {
                Notification.Action action =
                       new Notification.Action.Builder(android.R.drawable.star_on,
                               btn_title, resultPendingIntent)
                               //.addRemoteInput(remoteInput)
                               .build();
                m_builder.addAction(action);
            }

            m_notificationManager.notify(notification_id, m_builder.build());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void hideNotification(Context context) {
        try {
            NotificationManager m_notificationManager = (NotificationManager)
                    context.getSystemService(Context.NOTIFICATION_SERVICE);

            for (int id : NOTIFICATION_IDS) {
                m_notificationManager.cancel(id);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
