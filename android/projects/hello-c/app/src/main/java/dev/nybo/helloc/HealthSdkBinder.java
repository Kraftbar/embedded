package dev.nybo.helloc;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

public final class HealthSdkBinder implements SensorEventListener {
    private static final String TAG = "HealthSdkBinder";

    static {
        System.loadLibrary("helloc");
    }

    private static native void nativeOnPulse(double bpm, long timestampMs);
    private static native void nativeOnHealthStatus(String status);

    private final SensorManager sensorManager;
    private final Sensor heartRateSensor;
    private final WatchLogReporter reporter;
    private boolean running;

    public HealthSdkBinder(Context context) {
        sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        heartRateSensor = sensorManager != null ? sensorManager.getDefaultSensor(Sensor.TYPE_HEART_RATE) : null;
        reporter = new WatchLogReporter(context, "java");
    }

    public void start() {
        if (running) {
            return;
        }
        if (sensorManager == null || heartRateSensor == null) {
            nativeOnHealthStatus("heart sensor missing");
            Log.i(TAG, "No standard heart-rate sensor exposed");
            return;
        }
        running = sensorManager.registerListener(this, heartRateSensor, SensorManager.SENSOR_DELAY_NORMAL);
        if (running) {
            reporter.start();
        }
        nativeOnHealthStatus(running ? "heart sensor started" : "heart sensor start failed");
        Log.i(TAG, "Heart-rate binder start: " + running);
    }

    public void stop() {
        if (!running || sensorManager == null) {
            return;
        }
        sensorManager.unregisterListener(this);
        running = false;
        reporter.stop();
        nativeOnHealthStatus("heart sensor stopped");
        Log.i(TAG, "Heart-rate binder stopped");
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() != Sensor.TYPE_HEART_RATE || event.values.length == 0) {
            return;
        }
        double bpm = event.values[0];
        reporter.updatePulse(bpm);
        nativeOnPulse(bpm, System.currentTimeMillis());
        Log.i(TAG, "Pulse " + bpm + " bpm");
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        nativeOnHealthStatus("heart accuracy " + accuracy);
    }
}
