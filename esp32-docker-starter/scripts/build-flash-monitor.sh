#!/usr/bin/env bash
# Linux equivalent of build-flash-monitor.ps1.
# Usage: bash scripts/build-flash-monitor.sh [--port /dev/ttyUSB0] [--flash-baud 115200] [--monitor-baud 115200]

set -euo pipefail

PORT="/dev/ttyUSB0"
FLASH_BAUD=115200
MONITOR_BAUD=115200

while [[ $# -gt 0 ]]; do
    case "$1" in
        --port)         PORT="$2"; shift 2 ;;
        --flash-baud)   FLASH_BAUD="$2"; shift 2 ;;
        --monitor-baud) MONITOR_BAUD="$2"; shift 2 ;;
        -h|--help)
            echo "Usage: $0 [--port PORT] [--flash-baud BAUD] [--monitor-baud BAUD]"
            exit 0
            ;;
        *) echo "Unknown argument: $1" >&2; exit 2 ;;
    esac
done

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
VENV_BIN="/home/nybo/.venvs/esp32/bin"
IDF_EXPORT="/home/nybo/esp-idf/export.sh"
ELF="$PROJECT_ROOT/build/esp32_docker_starter.elf"
TRACER="$PROJECT_ROOT/scripts/uart_tracer.py"

if [[ ! -f "$IDF_EXPORT" ]]; then
    echo "Missing ESP-IDF export script: $IDF_EXPORT" >&2
    exit 1
fi

export PATH="$VENV_BIN:$PATH"
# shellcheck disable=SC1091
. "$IDF_EXPORT" >/dev/null

cd "$PROJECT_ROOT"

idf.py build

bash "$PROJECT_ROOT/scripts/flash.sh" --port "$PORT" --baud "$FLASH_BAUD"

python "$TRACER" --port "$PORT" --baud "$MONITOR_BAUD" --log-file "$PROJECT_ROOT/serial.log"
