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
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.app.NotificationChannel;
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
import android.app.PendingIntent;
import com.kdab.training.MainActivity;
import android.net.Uri;

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
        String check_url = getCheckUrl(getApplicationContext());
        Log.d(TAG, "check_url = " + check_url);
        //Toast.makeText(this, "Try check url:\n"+check_url, Toast.LENGTH_SHORT).show();
        new Thread(new Runnable() {
            public void run() {
                try{
                    String data = getContent(check_url);
                    Log.d(TAG, "getContent: " + check_url + " datalength " + data.length());
                    boolean ready = (data.indexOf("Ready in battles:") != -1);
                    if (ready || (data.indexOf("Challenged to battles:") != -1)) {
                        Log.d(TAG, "need notif");
                        Context context = getApplicationContext();
                        String msg = "You got an invite, see invitation";
                        Bundle msg_data = null;
                        if (ready) {
                            msg = "Your turn, open the game list";
                            //notify(context, "Your turn, open the game list");
                        } else {
                            //notify(context, "You got an invite, see invitation");
                        }
                        Log.d(TAG, msg);
                        CheckStatus.notify(context, msg, msg_data);

                        try {
                          SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
                          SharedPreferences.Editor editor = sharedPreferences.edit();
                          int last_notification = Math.round(System.currentTimeMillis()/1000L);
                          editor.putInt("last_notification", last_notification);
                          editor.commit();
                          Log.d(TAG, "last_notification = " + last_notification);
                        } catch (Exception e) {
                            e.printStackTrace();
                            //return "";
                        }
                    }
                }
                catch (IOException ex){
                    Toast.makeText(getApplicationContext(), "Error: " + ex.getMessage(), Toast.LENGTH_SHORT).show();
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

    public static void notify(Context context, String message, Bundle data) {
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
            if (data != null) {
                resultIntent.putExtras(data);
            }
            PendingIntent resultPendingIntent = PendingIntent.getActivity(context, 0, resultIntent, PendingIntent.FLAG_UPDATE_CURRENT);

            Bitmap icon = BitmapFactory.decodeResource(context.getResources(), R.drawable.icon);
            m_builder//.setSmallIcon(icon)
                    .setSmallIcon(R.drawable.icon)
                    .setLargeIcon(icon)
                    .setContentTitle(message)
                    .setContentText("Warlock duel need your attention")
                    .setDefaults(Notification.DEFAULT_SOUND)
                    //.setColor(Color.GREEN)
                    .setAutoCancel(true)
                    .setContentIntent(resultPendingIntent);

            m_notificationManager.notify(0, m_builder.build());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
