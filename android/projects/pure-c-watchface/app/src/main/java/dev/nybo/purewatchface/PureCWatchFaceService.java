package dev.nybo.purewatchface;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.BatteryManager;
import android.os.Handler;
import android.os.Looper;
import android.service.wallpaper.WallpaperService;
import android.view.SurfaceHolder;

import java.util.Calendar;

public final class PureCWatchFaceService extends WallpaperService {
    static {
        System.loadLibrary("purecwatchface");
    }

    private static native void render(
            Bitmap bitmap,
            int width,
            int height,
            int hour,
            int minute,
            int second,
            int batteryPct,
            double bpm,
            boolean ambient);

    @Override
    public Engine onCreateEngine() {
        return new Engine();
    }

    private final class Engine extends WallpaperService.Engine implements SensorEventListener {
        private final Handler handler = new Handler(Looper.getMainLooper());
        private final Runnable drawTick = new Runnable() {
            @Override
            public void run() {
                drawFrame();
                scheduleNextDraw();
            }
        };
        private final WatchLogReporter reporter = new WatchLogReporter(PureCWatchFaceService.this);
        private final SensorManager sensorManager =
                (SensorManager) getSystemService(SENSOR_SERVICE);
        private final Sensor heartSensor = sensorManager == null
                ? null
                : sensorManager.getDefaultSensor(Sensor.TYPE_HEART_RATE);

        private Bitmap bitmap;
        private boolean visible;
        private boolean ambient;
        private double bpm;

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);
            reporter.start();
            reporter.updateState(visible, ambient);
        }

        @Override
        public void onVisibilityChanged(boolean visible) {
            this.visible = visible;
            reporter.updateState(visible, ambient);
            if (visible) {
                registerSensors();
                drawFrame();
                scheduleNextDraw();
            } else {
                handler.removeCallbacks(drawTick);
                unregisterSensors();
            }
        }

        public void onAmbientModeChanged(boolean inAmbientMode, long animationDuration) {
            ambient = inAmbientMode;
            reporter.updateState(visible, ambient);
            drawFrame();
            scheduleNextDraw();
        }

        @Override
        public void onSurfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            super.onSurfaceChanged(holder, format, width, height);
            if (width > 0 && height > 0) {
                bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            }
            drawFrame();
        }

        @Override
        public void onDestroy() {
            handler.removeCallbacks(drawTick);
            unregisterSensors();
            reporter.stop();
            bitmap = null;
            super.onDestroy();
        }

        @Override
        public void onSensorChanged(SensorEvent event) {
            if (event.sensor.getType() == Sensor.TYPE_HEART_RATE && event.values.length > 0) {
                bpm = event.values[0];
                reporter.updatePulse(bpm);
                drawFrame();
            }
        }

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }

        private void registerSensors() {
            if (sensorManager != null && heartSensor != null) {
                sensorManager.registerListener(this, heartSensor, 1_000_000);
            }
        }

        private void unregisterSensors() {
            if (sensorManager != null) {
                sensorManager.unregisterListener(this);
            }
        }

        private void scheduleNextDraw() {
            handler.removeCallbacks(drawTick);
            if (!visible) return;
            long delay = ambient ? 60_000L : 1_000L;
            handler.postDelayed(drawTick, delay);
        }

        private void drawFrame() {
            SurfaceHolder holder = getSurfaceHolder();
            Rect frame = holder.getSurfaceFrame();
            int width = frame.width();
            int height = frame.height();
            if (width <= 0 || height <= 0) return;
            if (bitmap == null || bitmap.getWidth() != width || bitmap.getHeight() != height) {
                bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            }

            Calendar now = Calendar.getInstance();
            int batteryPct = readBatteryPct();
            render(bitmap, width, height,
                    now.get(Calendar.HOUR_OF_DAY),
                    now.get(Calendar.MINUTE),
                    now.get(Calendar.SECOND),
                    batteryPct,
                    bpm,
                    ambient);

            Canvas canvas = holder.lockCanvas();
            if (canvas == null) return;
            try {
                canvas.drawBitmap(bitmap, 0, 0, null);
            } finally {
                holder.unlockCanvasAndPost(canvas);
            }
        }

        private int readBatteryPct() {
            BatteryManager manager = (BatteryManager) getSystemService(BATTERY_SERVICE);
            if (manager == null) return -1;
            return manager.getIntProperty(BatteryManager.BATTERY_PROPERTY_CAPACITY);
        }
    }
}
