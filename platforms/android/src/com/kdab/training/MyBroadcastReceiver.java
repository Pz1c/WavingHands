import android.content.Context;
import android.content.Intent;
import android.content.BroadcastReceiver;
import com.kdab.training.service;

public class MyBroadcastReceiver extends BroadcastReceiver {
    @Override
    public void onReceive(Context context, Intent intent) {
        Intent startServiceIntent = new Intent(context, MyService.class);
        context.startService(startServiceIntent);
    }
}