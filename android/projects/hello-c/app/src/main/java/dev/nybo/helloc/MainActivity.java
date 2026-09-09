package dev.nybo.helloc;

import android.Manifest;
import android.app.NativeActivity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;

public class MainActivity extends NativeActivity {
    private static final int REQ_BODY = 100;
    private static final int REQ_BACKGROUND_BODY = 101;
    private static final int REQ_NOTIFICATIONS = 102;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestForegroundPermissions();
        SamsungHealthDbProbe.run(this);
        startPulseService();
    }

    private void startPulseService() {
        Intent intent = new Intent(this, PulseService.class);
        if (Build.VERSION.SDK_INT >= 26) {
            startForegroundService(intent);
        } else {
            startService(intent);
        }
    }

    private void requestForegroundPermissions() {
        if (Build.VERSION.SDK_INT >= 20
                && checkSelfPermission(Manifest.permission.BODY_SENSORS) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] { Manifest.permission.BODY_SENSORS }, REQ_BODY);
            return;
        }
        requestBackgroundSensorsIfPossible();
        requestNotificationsIfPossible();
    }

    private void requestBackgroundSensorsIfPossible() {
        if (Build.VERSION.SDK_INT >= 33
                && checkSelfPermission(Manifest.permission.BODY_SENSORS_BACKGROUND) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] { Manifest.permission.BODY_SENSORS_BACKGROUND }, REQ_BACKGROUND_BODY);
        }
    }

    private void requestNotificationsIfPossible() {
        if (Build.VERSION.SDK_INT >= 33
                && checkSelfPermission(Manifest.permission.POST_NOTIFICATIONS) != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(new String[] { Manifest.permission.POST_NOTIFICATIONS }, REQ_NOTIFICATIONS);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQ_BODY) {
            requestBackgroundSensorsIfPossible();
            requestNotificationsIfPossible();
        }
    }
}
