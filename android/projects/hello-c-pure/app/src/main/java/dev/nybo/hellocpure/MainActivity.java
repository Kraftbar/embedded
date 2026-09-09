package dev.nybo.hellocpure;

import android.app.NativeActivity;
import android.os.Bundle;

public class MainActivity extends NativeActivity {
    private static WatchLogReporter reporter;

    static {
        System.loadLibrary("hellocpure");
    }

    public static void nativeOnPulse(double bpm) {
        WatchLogReporter current = reporter;
        if (current != null) {
            current.updatePulse(bpm);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        reporter = new WatchLogReporter(this, "pure");
        reporter.start();
    }

    @Override
    protected void onDestroy() {
        WatchLogReporter current = reporter;
        reporter = null;
        if (current != null) {
            current.stop();
        }
        super.onDestroy();
    }
}
