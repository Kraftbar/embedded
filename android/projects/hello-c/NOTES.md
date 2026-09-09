# Hello C Notes

## Battery

This app currently reads the watch heart-rate sensor through a foreground health service.

Battery impact can be meaningful because:

- The heart-rate sensor is sampled continuously while the service is running.
- The service keeps a foreground notification active.
- The app logs every pulse sample in debug builds.
- Network upload, if added later, will add more battery cost.

Current mitigation:

- `PulseService` owns the background lifetime.
- `HealthSdkBinder` owns Android health/sensor API calls.
- C receives pulse values through JNI and renders/stores state.
- The foreground screen only draws the latest shared BPM and does not start its own sensor reader.

Next mitigations:

- Sample less often when the screen is off.
- Batch samples and upload every N seconds/minutes instead of every reading.
- Stop service explicitly when not needed.
- Avoid verbose logcat in release builds.

## Data Source

We are collecting live pulse ourselves from the watch sensor.

We are not reading Samsung Health or the watch health database.
