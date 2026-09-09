package dev.nybo.helloc;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Intent;
import android.os.Build;
import android.os.IBinder;

public class PulseService extends Service {
    private static final String CHANNEL_ID = "pulse";
    private static final int NOTIFICATION_ID = 42;
    private HealthSdkBinder healthSdkBinder;

    @Override
    public void onCreate() {
        super.onCreate();
        createChannel();
        startForeground(NOTIFICATION_ID, buildNotification());
        healthSdkBinder = new HealthSdkBinder(this);
        healthSdkBinder.start();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (healthSdkBinder == null) {
            healthSdkBinder = new HealthSdkBinder(this);
        }
        healthSdkBinder.start();
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (healthSdkBinder != null) {
            healthSdkBinder.stop();
            healthSdkBinder = null;
        }
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private void createChannel() {
        if (Build.VERSION.SDK_INT < 26) return;
        NotificationChannel channel = new NotificationChannel(
                CHANNEL_ID,
                "Pulse",
                NotificationManager.IMPORTANCE_LOW);
        channel.setDescription("Reads pulse while Hello C is running.");
        NotificationManager manager = getSystemService(NotificationManager.class);
        if (manager != null) manager.createNotificationChannel(channel);
    }

    private Notification buildNotification() {
        Notification.Builder builder = Build.VERSION.SDK_INT >= 26
                ? new Notification.Builder(this, CHANNEL_ID)
                : new Notification.Builder(this);
        return builder
                .setSmallIcon(android.R.drawable.stat_notify_sync)
                .setContentTitle("Hello C pulse")
                .setContentText("Reading heart rate in the background")
                .setOngoing(true)
                .build();
    }
}
