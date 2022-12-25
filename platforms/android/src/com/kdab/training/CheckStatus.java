package com.kdab.training;

import android.app.Service;
import android.content.Intent;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Process;
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

public class CheckStatus extends Service {
    private static final String TAG = "WarlockDuel.CheckStatus";

    // This method run only one time. At the first time of service created and running
    @Override
    public void onCreate() {
        HandlerThread thread = new HandlerThread("ServiceStartArguments", Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();
        Log.d(TAG, "After service created");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "onStartCommand");
        //Here is the source of the TOASTS :D
        //String check_url = getCheckUrl(getApplicationContext());
        //Log.d(TAG, "check_url = " + check_url);
        //Toast.makeText(this, "Try check url:\n"+check_url, Toast.LENGTH_SHORT).show();
        new Thread(new Runnable() {
            public void run() {
                try{
                    //Log.d(TAG, "check_url = " + check_url);
                    Context context = getApplicationContext();
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


                    if (ready || challenge || finished || reminder) {
                        Log.d(TAG, "need notif");
                        String msg, btn_title = "Play";//
                        Bundle msg_data = new Bundle();
                        if (challenge) {
                            msg = "You got an invite, see invitation";
                            msg_data.putInt("action_type", 1);
                            editor.putInt("notification_action_type", 1);
                            if ((arr_challenge.length == 1) && (!arr_challenge[0].isEmpty())) {
                                msg_data.putInt("battle_id", Integer.parseInt(arr_challenge[0]));
                                editor.putInt("notification_battle_id", Integer.parseInt(arr_challenge[0]));
                            }
                        } else if (ready) {
                            msg = "Good news, you've got a new turn to play!";
                            msg_data.putInt("action_type", 2);
                            editor.putInt("notification_action_type", 2);
                            if ((arr_ready.length == 1) && (!arr_ready[0].isEmpty())) {
                                msg_data.putInt("battle_id", Integer.parseInt(arr_ready[0]));
                                editor.putInt("notification_battle_id", Integer.parseInt(arr_ready[0]));
                            }
                        } else if (finished) {
                            msg = "Your battle is over!";
                            msg_data.putInt("action_type", 3);
                            editor.putInt("notification_action_type", 3);
                            btn_title = "Show me";
                            if ((finished_id != 0) && (finished_id != -2)) {
                                msg_data.putInt("battle_id", finished_id);
                                editor.putInt("notification_battle_id", finished_id);
                            }
                        } else {
                            msg = "Great news! You got a new match";
                            msg_data.putInt("action_type", 4);
                            editor.putInt("notification_action_type", 4);
                            btn_title = "Show me";
                            if ((finished_id != 0) && (finished_id != -2)) {
                                msg_data.putInt("battle_id", 0);
                                editor.putInt("notification_battle_id", 0);
                            }
                        }
                        Log.d(TAG, msg);
                        CheckStatus.notify(context, msg, btn_title, msg_data);
                        editor.putInt("last_notification", last_notification);
                    }
                    editor.commit();
                }
                 catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }).start();
        //MyJavaNatives.checkWarlockProfile(check_url);

        return START_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        // We don't provide binding
        return null;
    }

    private String getContent(String path) throws IOException {
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

    public static void notify(Context context, String message, String btn_title, Bundle data) {
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
            PendingIntent resultPendingIntent;// = PendingIntent.getActivity(context, 0, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                resultPendingIntent = PendingIntent.getActivity(context, 0, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);
            }else {
                resultPendingIntent = PendingIntent.getActivity(context, 0, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT);
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
}
