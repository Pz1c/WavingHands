// java file android/src/com/kdab/training/MyService.java
package com.kdab.training;

import android.content.Context;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtService;

public class MyService extends QtService
{
    public static void startMyService(Context ctx) {
        ctx.startService(new Intent(ctx, MyService.class));
    }
}
