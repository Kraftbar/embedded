# embedded

Microcontroller and hardware projects, consolidated from five repositories.

| Folder | Was | What |
|---|---|---|
| `esp32-docker-starter` | `esp32-docker-starter` | Minimal ESP-IDF project for a Windows + Docker build and flash workflow |
| `sds011` | `SDS011-driver` | Driver for the SDS011 particulate-matter sensor (C, CMake) |
| `uart-tracer` | `uart-tracer` | Small UART log tracer for local serial debugging |
| `label-printer` | `auto-print-labels` | Arduino label printer with a serial host tool |
| `night-alarm` | `esp32-night-alarm` | ESP32 night alarm with waveform playback |
| `rsp-balancing-robot` | `RSP` | Raspberry Pi self-balancing robot: parts lists, ROS/MPU-6050 references, accelerometer and motor data |

History from each source is preserved via `git subtree`. Pre-merge commits are
not reachable through `git log -- <folder>`; walk the merge's second parent:

```sh
sha=$(git log --full-history --format=%H -- sds011 | tail -1)
git log $sha^2
```
