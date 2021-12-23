import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class CustomBR extends BroadcastReceiver {

    private static final String D_TAG = "BR";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(D_TAG, "CustomReceiver onReceive (context, intent)");
        try {

            String referrer = intent.getStringExtra("referrer");
            Log.d(D_TAG, referrer);
            
            SharedPreferences sharedPreferences = getPreferences(MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPreferences.edit();
            editor.putString("referrer", referrer);
            editor.commit();
            
            Toast.makeText(getApplicationContext(), "referrer = " + referrer, Toast.LENGTH_LONG).show();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}