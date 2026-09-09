# Watch Battery Test

The watch apps can self-report to the dev server without ADB polling.

Endpoint:

```text
http://192.168.1.237/watch-log.php
```

Server-side JSONL files:

```text
/home/nybo/android-docker-dev/measurements/watch-ingest/YYYYMMDD-java.jsonl
/home/nybo/android-docker-dev/measurements/watch-ingest/YYYYMMDD-pure.jsonl
```

Each record includes app name, package, watch model, Android SDK, battery percent,
charging state, battery temperature, voltage, latest BPM, and sample count. The
server adds `_server_ts` and `_remote`.

Current apps:

```text
dev.nybo.helloc      Java/provider app
dev.nybo.hellocpure  Pure-C sensor app with a small Java HTTP logger
dev.nybo.purewatchface  C-rendered watchface with a small Java shell/logger
dev.nybo.wffwatchface   Resource-only WFF watchface that the current picker/runtime accepts
```

The logger sends once when the app starts and then every 5 minutes. The first
record can have `samples: 0` because it may post before the HR sensor returns a
sample.

Daily A/B idea:

```text
Day 1: baseline, no test app running
Day 2: pure-C app running
Day 3: Java/provider app running
```

Use the existing scripts for formal snapshots:

```bash
/home/nybo/android-docker-dev/scripts/watch-battery-day-start 192.168.1.118:33597 baseline
/home/nybo/android-docker-dev/scripts/watch-battery-day-start 192.168.1.118:33597 pure
/home/nybo/android-docker-dev/scripts/watch-battery-day-start 192.168.1.118:33597 java
/home/nybo/android-docker-dev/scripts/watch-battery-day-end
```

The app-side JSONL logs are useful for the less strict day-by-day test, while
the ADB scripts are still useful for start/end snapshots.

Watchface build/install:

```bash
/home/nybo/android-docker-dev/scripts/build-apk /home/nybo/android-projects/pure-c-watchface assembleDebug
```

The watchface installs as an APK, but it is selected through the watch face
picker. Its service component is:

```text
dev.nybo.purewatchface/.PureCWatchFaceService
```

On the updated watch, third-party watchfaces need Watch Face Format to be picked
up properly. The current WFF test face is:

```text
/home/nybo/android-projects/wff-watchface
dev.nybo.wffwatchface
```

Build/install it with the normal Docker Android tooling, then set it directly:

```bash
/home/nybo/android-docker-dev/scripts/build-apk /home/nybo/android-projects/wff-watchface assembleDebug
adb -s 192.168.1.118:33597 install -r /home/nybo/android-projects/wff-watchface/app/build/outputs/apk/debug/app-debug.apk
adb -s 192.168.1.118:33597 shell am broadcast -a com.google.android.wearable.app.DEBUG_SURFACE --es operation set-watchface --es watchFaceId dev.nybo.wffwatchface
```
