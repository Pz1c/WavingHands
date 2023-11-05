import android.util.Log;
import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;
import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;
import net.is.games.WarlocksDuel.R;

public class CustomBR extends BroadcastReceiver {

    private static final String D_TAG = "WarlocksDuelBR";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(D_TAG, "CustomReceiver onReceive (context, intent)");
        try {

            String referrer = intent.getStringExtra("referrer");
            Log.d(D_TAG, referrer);
            
            SharedPreferences sharedPreferences = context.getSharedPreferences("activity", 0);
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("referrer", referrer);
            editor.commit();
            
            Toast.makeText(context, "referrer = " + referrer, Toast.LENGTH_LONG).show();
            //Toast.makeText(getApplicationContext(), "referrer = " + referrer, Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
