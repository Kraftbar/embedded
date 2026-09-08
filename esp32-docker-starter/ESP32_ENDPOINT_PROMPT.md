# ESP32 Endpoint Prompt

Use this prompt for the server-side endpoint at `https://gautenybo.no/esp32.php`.

## Prompt

Create a small PHP endpoint at `esp32.php` that accepts `POST` requests with
`Content-Type: application/json` from an ESP32 device.

Requirements:

1. Accept only `POST`.
2. Parse the JSON body.
3. Expect fields like:

```json
{
  "device": "esp32_docker_starter",
  "mac": "02:00:00:00:00:01",
  "ip": "192.168.1.75",
  "ssid": "example-network",
  "rssi": -57,
  "channel": 6,
  "uptime_ms": 150050,
  "free_heap": 232984,
  "min_free_heap": 232964,
  "largest_free_block": 113664,
  "internal_free_heap": 232984,
  "main_stack_hwm_words": 1804,
  "main_stack_hwm_bytes": 7216,
  "wifi_up": true
}
```

4. Validate that `device` and `mac` are present.
5. Store each payload as one line of JSON in a log file such as
   `esp32-telemetry.log`.
6. Also store the latest payload in a file such as `esp32-latest.json`.
7. Return HTTP `200` with JSON:

```json
{"ok":true}
```

8. On bad input, return HTTP `400` with JSON:

```json
{"ok":false,"error":"..."}
```

9. Do not require a database yet. Flat files are enough.
10. Use file locking when writing logs.
11. Keep the implementation minimal and compatible with common shared-hosting PHP.

## Notes

- The ESP32 currently posts to:

```text
https://gautenybo.no/esp32.php
```

- The endpoint should be tolerant of extra future fields.
- If wanted later, add a shared secret header or token query parameter for basic
  authentication.
