package dev.nybo.helloc;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.provider.Settings;
import android.util.Log;

import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.StandardCharsets;
import java.util.Locale;

public final class WatchLogReporter {
    private static final String TAG = "WatchLogReporter";
    private static final String ENDPOINT = "http://192.168.1.237/watch-log.php";
    private static final long DEFAULT_INTERVAL_MS = 5 * 60 * 1000L;

    private final Context context;
    private final String appName;
    private final HandlerThread thread;
    private final Handler handler;
    private final long intervalMs;
    private final Runnable tick = new Runnable() {
        @Override
        public void run() {
            sendNow();
            handler.postDelayed(this, intervalMs);
        }
    };

    private volatile double bpm;
    private volatile int samples;
    private volatile boolean running;

    public WatchLogReporter(Context context, String appName) {
        this(context, appName, DEFAULT_INTERVAL_MS);
    }

    public WatchLogReporter(Context context, String appName, long intervalMs) {
        this.context = context.getApplicationContext();
        this.appName = appName;
        this.intervalMs = intervalMs;
        this.thread = new HandlerThread("watch-log-reporter");
        this.thread.start();
        this.handler = new Handler(thread.getLooper());
    }

    public void start() {
        if (running) return;
        running = true;
        handler.post(tick);
    }

    public void stop() {
        running = false;
        handler.removeCallbacksAndMessages(null);
        thread.quitSafely();
    }

    public void updatePulse(double bpm) {
        this.bpm = bpm;
        this.samples++;
    }

    private void sendNow() {
        try {
            String payload = buildPayload();
            HttpURLConnection connection = (HttpURLConnection) new URL(ENDPOINT).openConnection();
            connection.setRequestMethod("POST");
            connection.setConnectTimeout(5000);
            connection.setReadTimeout(5000);
            connection.setDoOutput(true);
            connection.setRequestProperty("Content-Type", "application/json");
            byte[] body = payload.getBytes(StandardCharsets.UTF_8);
            connection.setFixedLengthStreamingMode(body.length);
            try (OutputStream output = connection.getOutputStream()) {
                output.write(body);
            }
            int code = connection.getResponseCode();
            connection.disconnect();
            Log.i(TAG, "sent " + appName + " watch log: " + code);
        } catch (Throwable t) {
            Log.i(TAG, "watch log failed: " + t.getClass().getSimpleName() + ": " + t.getMessage());
        }
    }

    private String buildPayload() {
        BatteryState battery = readBattery();
        return String.format(Locale.US,
                "{\"ts\":%d,\"app\":\"%s\",\"package\":\"%s\",\"model\":\"%s\",\"android\":%d,"
                        + "\"battery_pct\":%d,\"charging\":%s,\"temp_c\":%.1f,\"voltage_mv\":%d,"
                        + "\"bpm\":%.1f,\"samples\":%d}",
                System.currentTimeMillis(),
                json(appName),
                json(context.getPackageName()),
                json(Build.MODEL),
                Build.VERSION.SDK_INT,
                battery.level,
                battery.charging ? "true" : "false",
                battery.tempC,
                battery.voltageMv,
                bpm,
                samples);
    }

    private BatteryState readBattery() {
        Intent intent = context.registerReceiver(null, new IntentFilter(Intent.ACTION_BATTERY_CHANGED));
        BatteryState state = new BatteryState();
        if (intent == null) {
            return state;
        }
        int level = intent.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
        int scale = intent.getIntExtra(BatteryManager.EXTRA_SCALE, 100);
        int status = intent.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
        int plugged = intent.getIntExtra(BatteryManager.EXTRA_PLUGGED, 0);
        int temp = intent.getIntExtra(BatteryManager.EXTRA_TEMPERATURE, 0);
        state.level = scale > 0 && level >= 0 ? Math.round(level * 100f / scale) : -1;
        state.charging = status == BatteryManager.BATTERY_STATUS_CHARGING
                || status == BatteryManager.BATTERY_STATUS_FULL
                || plugged != 0;
        state.tempC = temp / 10.0f;
        state.voltageMv = intent.getIntExtra(BatteryManager.EXTRA_VOLTAGE, 0);
        return state;
    }

    private static String json(String value) {
        if (value == null) return "";
        return value.replace("\\", "\\\\").replace("\"", "\\\"");
    }

    private static final class BatteryState {
        int level = -1;
        boolean charging;
        float tempC;
        int voltageMv;
    }
}
