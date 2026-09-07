#!/bin/bash
# TODO: check if the foreground Python process should block the watchdog
# TODO:   

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)" || exit 1
command_to_spawn1="python3 $script_dir/xarm-remote.py"
function start_xarm() {
      python3 "$script_dir/xarm-remote.py"
}
function check_xarm_ps_running(){
  nc_pid=$(pgrep -f "$command_to_spawn1")
  echo "$nc_pid"
}

while true; do
  arm_ps=$(check_xarm_ps_running)
  if [ -e "/dev/input/js0" ] && [ -z "$arm_ps" ]; then

    start_xarm
  fi

  sleep 1
done

# test
