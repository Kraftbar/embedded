# ESP32 Docker Starter

Minimal ESP-IDF project for a Windows + Docker build workflow.

## Known issue first

On Linux, a successful USB enumeration is **not** enough to flash.
If `scripts/flash.sh` fails with `Permission denied: '/dev/ttyUSB0'`, the board
is usually fine and your user just does not have serial-port access yet.

Typical fix:

```bash
sudo usermod -aG dialout $USER
sudo usermod -aG uucp $USER
```

Then log out and back in, or run `newgrp dialout` in a fresh shell before
retrying.

## Current status

The Linux path is currently the known-good workflow for this project:

- build on Linux works
- flash on Linux works
- monitor on Linux works
- Wi-Fi connect on device works

The Windows path still exists in this repo, but it is not the most reliable
workflow right now and should be treated as secondary until it is revalidated.

## What this gives you

- Build in Docker with a pinned `ESP-IDF` image.
- Flash from Windows PowerShell with `esptool`.
- Edit from WSL, Vim, or your Windows editor.

## Layout

- `main/main.c`: program entry and main loop.
- `main/board.c`, `main/board.h`: board-specific pin setup.
- `main/status.c`, `main/status.h`: status LED patterns.
- `main/wifi.c`, `main/wifi.h`: Wi-Fi station connect and link logging.
- `main/wifi_secrets.h`: local untracked Wi-Fi credentials.
- `main/wifi_secrets.example.h`: sample credentials header.
- `scripts/build.ps1`: builds with Docker.
- `scripts/flash.ps1`: flashes the built binaries to your board.

Wi-Fi credentials live in `main/wifi_secrets.h`.
If you set this project up on another machine, copy `main/wifi_secrets.example.h` to `main/wifi_secrets.h` and edit it.

## Requirements

- Docker Desktop
- PowerShell
- Python on Windows
- `esptool` installed on Windows: `py -m pip install esptool`

## Linux quick path

Linux flashing works too, but you need local `ESP-IDF`, `esptool`, and serial
permissions for `/dev/ttyUSB*` or `/dev/ttyACM*`.

After building, flash with:

```bash
bash scripts/flash.sh --port /dev/ttyUSB0 --baud 115200
```

If you get `Permission denied`, fix group membership first as described above.

## Telemetry Endpoint

The device currently posts telemetry every 30 seconds to:

```text
https://gautenybo.no/esp32.php
```

Current payload fields:

- `device`
- `mac`
- `ip`
- `ssid`
- `bssid`
- `rssi`
- `channel`
- `uptime_ms`
- `free_heap`
- `min_free_heap`
- `largest_free_block`
- `internal_free_heap`
- `main_stack_hwm_words`
- `main_stack_hwm_bytes`
- `wifi_up`

Important server requirement:

- the endpoint must return `200 {"ok":true}` immediately
- any file logging or other slow work should happen only after the response is flushed
- no redirects
- no slow setup before response

If the server is slow, the ESP32 logs HTTP timeout failures such as
`ESP_ERR_HTTP_EAGAIN` even when Wi-Fi and TLS are working correctly.

See [ESP32_ENDPOINT_PROMPT.md](/home/nybo/tmp/ESP32_ENDPOINT_PROMPT.md) for a
copy-paste endpoint prompt/spec.

### Known issue: empty POST body (len=0) on server

Diagnosed via `/var/www/twitterclone/esp32-debug.log` on the server.
Symptoms: TLS handshake succeeds, server receives the POST, but body is empty (`len=0`).
Root cause: ESP32 HTTP client was reusing stale keep-alive connections. The connection
appeared open but the body never got transmitted.

Fix applied in `main/telemetry.c`:

```c
.keep_alive_enable = false,
```

This forces a fresh TCP+TLS connection on every POST. Slightly slower per request
(new handshake each time) but reliable. Do not re-enable keep-alive without testing.

The server-side PHP (`esp32.php`) is correct — `set_post_field` is called before
`perform`, the issue was purely the stale connection reuse.

## Build

From PowerShell in the project root:

```powershell
.\scripts\build.ps1
```

## Flash

```powershell
.\scripts\flash.ps1 -Port COM6
```

`flash.ps1` now defaults to `115200` baud because it is slower but more reliable on flaky boards.

If flashing fails, close any open serial monitor first and retry.

This project is set for `4MB` flash and the larger single-app partition layout.

If your board still needs you to hold `BOOT` while flashing starts, that is usually a board auto-reset quirk, not a firmware bug.

## Monitor

```powershell
.\scripts\monitor.ps1 -Port COM6
```

Windows monitor note: this path currently needs revalidation and may not stop
cleanly with `Ctrl+C`. If you need a reliable monitor today, use the Linux
serial monitor flow instead.

This opens the serial log at `115200` baud using a local Python tracer adapted from `Kraftbar/uart-tracer` and writes `serial.log` in the project root.

## Build, flash, and monitor

```powershell
.\scripts\build-flash-monitor.ps1 -Port COM6
```

This builds first, flashes only on success, then opens a serial monitor at `115200` baud.

If you already have a monitor open from an earlier run, close it before flashing again.

Use `Ctrl+C` to stop the monitor.

## WSL workflow

You can stay in WSL to inspect files, edit code, and ask me to make changes.

If Docker is available from WSL, you can also try:

```bash
docker run --rm -v "$(pwd):/project" -w /project espressif/idf:v5.2.2 idf.py set-target esp32 build
```

Flashing is still simplest from Windows PowerShell because the serial port is already there as `COM6`.
