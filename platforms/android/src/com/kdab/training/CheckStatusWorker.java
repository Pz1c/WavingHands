package com.kdab.training;

import android.content.Context;
import android.util.Log;

import androidx.annotation.NonNull;
import androidx.work.Worker;
import androidx.work.WorkerParameters;

/**
 * Runs one profile check and queues the next one. Replaces the old
 * AlarmReceiver -> CheckStatus service hop; doWork() is already called on a background
 * thread, so the check runs inline rather than spawning one.
 */
public class CheckStatusWorker extends Worker {
    private static final String TAG = "WarlocksDuel.CheckStatusWorker";

    public CheckStatusWorker(@NonNull Context context, @NonNull WorkerParameters params) {
        super(context, params);
    }

    @NonNull
    @Override
    public Result doWork() {
        Context context = getApplicationContext();
        Log.d(TAG, "doWork");
        try {
            CheckStatus.performCheck(context);
        } catch (Exception e) {
            e.printStackTrace();
        }
        // Always chain the next check. A failed poll just waits for the normal schedule
        // instead of burning WorkManager's retry backoff, which would drift the ladder.
        CheckScheduler.schedule(context, false);
        return Result.success();
    }
}
