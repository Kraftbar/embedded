# Android Docker Dev

Build Android APKs on this Linux server with Docker, then install them to a phone over Wi-Fi with `adb`.

## Phone Setup

On the phone:

1. Enable Developer options.
2. Enable USB debugging.
3. Enable Wireless debugging.
4. Pair/connect the phone on the same Wi-Fi as the server.

Android usually shows an IP and port like:

```text
192.168.1.50:39555
```

## Build An APK

From any Android project that has `gradlew`:

```bash
/home/nybo/android-docker-dev/scripts/build-apk
```

Or point it at a project:

```bash
/home/nybo/android-docker-dev/scripts/build-apk /path/to/android/project
```

## Project Directory

The web GUI scans this directory:

```bash
/home/nybo/android-projects
```

Put Android projects there directly, or symlink existing repos:

```bash
ln -s /path/to/android/project /home/nybo/android-projects/project-name
```

Default task is `assembleDebug`. You can pass another Gradle task:

```bash
/home/nybo/android-docker-dev/scripts/build-apk /path/to/project :app:assembleDebug
```

## Connect To Phone

Pair first if Android shows a pairing code:

```bash
/home/nybo/android-docker-dev/scripts/adb-wifi pair 192.168.1.50:37123 123456
```

```bash
/home/nybo/android-docker-dev/scripts/adb-wifi connect 192.168.1.50:39555
```

Check devices:

```bash
/home/nybo/android-docker-dev/scripts/adb-wifi devices
```

Watch logs:

```bash
/home/nybo/android-docker-dev/scripts/adb-wifi logcat
```

## Install APK

```bash
/home/nybo/android-docker-dev/scripts/adb-wifi install /path/to/project/app/build/outputs/apk/debug/app-debug.apk
```

## Build And Flash In One Command

```bash
/home/nybo/android-docker-dev/scripts/flash-apk 192.168.1.50:39555 /path/to/android/project
```

This runs `assembleDebug`, connects to the phone, then installs the debug APK.

## Open A Dev Shell

```bash
/home/nybo/android-docker-dev/scripts/android-shell /path/to/android/project
```

Inside the shell you can run:

```bash
./gradlew assembleDebug
adb devices
```

## Notes

- The container uses `network_mode: host` so `adb connect PHONE_IP:PORT` works on the LAN.
- Gradle and Android SDK user caches live in Docker volumes, so builds get faster after the first run.
- Project files stay on the host. The container is just the build/adb toolchain.
