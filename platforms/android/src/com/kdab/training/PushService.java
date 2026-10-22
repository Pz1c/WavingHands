package com.kdab.training;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;

import androidx.annotation.NonNull;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

import org.qtproject.example.androidnotifier.NotificationClient;

import java.util.Map;

/**
 * Firebase Cloud Messaging entry point (https://github.com/Pz1c/WavingHands/issues/336).
 *
 * Pure Java on purpose: FCM starts this service in a process where the Qt native library is
 * not loaded, so nothing on this path may depend on C++. The token lands in the "activity"
 * prefs, where the core picks it up at its next start (NotificationClient.getPushToken), and
 * is handed to the core at once when it is running (NotificationClient.deliverPushToken), to
 * be registered with the caster server over the roster link (PUSH). A data message is
 * rendered through the same CheckStatus.notifyType the WorkManager poll uses, so both roads
 * give the same notification and the same tap behaviour (MainActivity.onResume extras ->
 * NotificationClient.get_refferer -> show_battle).
 *
 * Wire contract, shared with the server. Data-only messages (never a "notification" block),
 * every value a string:
 *   v          always "1"; anything else is ignored
 *   type       "ready": battles await the player's orders; "invite": private challenges await
 *              the player's answer. Never mixed in one message.
 *   battles    ascending battle numbers joined with ';', at least one
 *   battle_id  the number when count is "1", "" otherwise
 *   count      number of ids in battles
 *   sent       UTC unix seconds when the server built the message; older than 12 h is dropped
 */
public class PushService extends FirebaseMessagingService {
    private static final String TAG = "WarlocksDuel.PushService";
    // The logcat tag of the token registration, shared with NotificationClient and the core.
    public static final String PUSH_TAG = "Push";
    private static final long MAX_AGE_SEC = 12L * 60 * 60;

    @Override
    public void onNewToken(@NonNull String token) {
        super.onNewToken(token);
        Log.d(PUSH_TAG, "onNewToken " + redact(token));
        // Stored, and handed to the core when it is running (it then sends PUSH at once).
        storeToken(getApplicationContext(), token);
    }

    @Override
    public void onMessageReceived(@NonNull RemoteMessage message) {
        super.onMessageReceived(message);
        try {
            Context context = getApplicationContext();
            Map<String, String> data = message.getData();
            if (!"1".equals(data.get("v"))) {
                Log.d(TAG, "onMessageReceived: unknown version " + data.get("v") + ", dropped");
                return;
            }
            String type = data.get("type");
            int action_type;
            if ("invite".equals(type)) {
                action_type = 1;
            } else if ("ready".equals(type)) {
                action_type = 2;
            } else {
                Log.d(TAG, "onMessageReceived: unknown type " + type + ", dropped");
                return;
            }
            long now = System.currentTimeMillis() / 1000L;
            long sent = parseLong(data.get("sent"), 0L);
            long age = (sent > 0) ? (now - sent) : 0;
            int battle_id = (int) parseLong(data.get("battle_id"), 0L);
            Log.d(TAG, "onMessageReceived: type=" + type + " battles=" + data.get("battles")
                    + " count=" + data.get("count") + " battle_id=" + battle_id + " age=" + age + "s");
            if (age > MAX_AGE_SEC) {
                Log.d(TAG, "onMessageReceived: stale, dropped");
                return;
            }
            if (MainActivity.isForeground) {
                // The live link and the battle list already cover it.
                Log.d(TAG, "onMessageReceived: app in foreground, nothing to show");
                return;
            }
            if (!CheckScheduler.canPostNotifications(context)) {
                Log.d(TAG, "onMessageReceived: notifications not allowed, nothing to show");
                return;
            }
            SharedPreferences prefs = context.getSharedPreferences("activity", 0);
            if (prefs.getInt("server_push_active", 0) != 1) {
                // The app never registered this device, or has logged out or switched the
                // setting off since: a binding the server still holds is not the player's wish.
                Log.d(TAG, "onMessageReceived: push not active on this device, dropped");
                return;
            }
            CheckStatus.notifyType(context, action_type, battle_id);
            // The bookkeeping of CheckStatus.performCheck, so the poll's reminder logic keeps
            // working from the last notification whichever road posted it.
            SharedPreferences.Editor editor = prefs.edit();
            editor.putInt("last_notification", (int) now);
            editor.putInt("notification_action_type", action_type);
            editor.putInt("notification_battle_id", battle_id);
            editor.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // The token FCM gave this install, for the core to register: kept in the prefs for the
    // next start of the app, and handed to the core right away when it is running. Also the
    // completion of every fetch (NotificationClient.fetchPushToken).
    public static void storeToken(Context context, String token) {
        if (token == null || token.isEmpty()) {
            return;
        }
        try {
            SharedPreferences prefs = context.getSharedPreferences("activity", 0);
            String previous = prefs.getString("fcm_token", "");
            SharedPreferences.Editor editor = prefs.edit();
            editor.putString("fcm_token", token);
            editor.putLong("fcm_token_ts", System.currentTimeMillis() / 1000L);
            if (!previous.isEmpty() && !previous.equals(token)) {
                // Rotated: whatever the server has bound is dead until the core registers the
                // new one, so the poll covers ready and challenge in the meantime.
                Log.d(PUSH_TAG, "storeToken: token rotated, the poll takes over until re-registered");
                editor.putInt("server_push_active", 0);
            }
            editor.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
        NotificationClient.deliverPushToken(token, "");
    }

    // Never a whole token in a log: the first 8 characters and the length.
    public static String redact(String token) {
        if (token == null || token.isEmpty()) {
            return "<empty>";
        }
        return "<" + token.length() + "B:" + token.substring(0, Math.min(8, token.length())) + "...>";
    }

    private static long parseLong(String s, long fallback) {
        if (s == null || s.isEmpty()) {
            return fallback;
        }
        try {
            return Long.parseLong(s.trim());
        } catch (NumberFormatException e) {
            return fallback;
        }
    }
}
