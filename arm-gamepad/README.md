# arm-gamepad

This project now lives in `embedded/arm-gamepad`. The former standalone
`Kraftbar/arm-gamepad` repository has been removed.

## Setup

```sh
mkdir -p ~/github
git clone https://github.com/Kraftbar/embedded.git ~/github/embedded
cd ~/github/embedded/arm-gamepad
go build -o gotest/getstate gotest/getstate.go
go build -o gotest/setstate gotest/setstate.go
```

If you already have `~/github/embedded`, skip the clone. Build the Go helpers on
the machine that talks to the robot. The local Python script uses NumPy and the
xArm SDK; the gamepad sender uses `inputs`. Review the robot address in
`xarm-local.py` and both Go helpers, and the receiver address in `xarm-remote.py`,
before running them.

On the robot-side machine:

```sh
bash ~/github/embedded/arm-gamepad/watchdog-xarm-local.sh
```

On the gamepad-side machine:

```sh
bash ~/github/embedded/arm-gamepad/watchdog-xarm-remote.sh
```

The watchdogs find their Python scripts and Go helpers relative to their own
folder, so the repository can be cloned elsewhere too. Existing shell startup
entries that launch `~/arm-gamepad/watchdog-xarm-*.sh` must be updated to the
new location. Replace the old entry rather than adding a second watchdog.

Update with `git -C ~/github/embedded pull --ff-only`; rebuild the Go helpers
when their source changes.

## Development notes

```sh

go build -o gotest/getstate gotest/getstate.go && ./gotest/getstate









nohup sh -c "nc -l -u -k 12345 | python3 ~/github/embedded/arm-gamepad/xarm-local.py" &

nybo@nybo-Latitude-7480:~$ pstree -p  | grep -C10 "sh"

nybo@nybo-Latitude-7480:~$ lsof -p 5397 | grep -E '([0-9]+[wu])|([0-9]+[wu].+[2,1]u)'
python3 5397 nybo    1w   REG    8,2        0 10356710 /home/nybo/nohup.out
python3 5397 nybo    2w   REG    8,2        0 10356710 /home/nybo/nohup.out
python3 5397 nybo    3u  IPv4  71662      0t0      TCP nybo-Latitude-7480.lan:56226->192.168.1.158:502 (ESTABLISHED)
python3 5397 nybo    4u  IPv4  71663      0t0      TCP nybo-Latitude-7480.lan:46624->192.168.1.158:30002 (ESTABLISHED)
nybo@nybo-Latitude-7480:~$ lsof -p 5396 | grep -E '([0-9]+[wu])|([0-9]+[wu].+[2,1]u)'
nc      5396 nybo    0w   CHR    1,3      0t0        6 /dev/null
nc      5396 nybo    1w  FIFO   0,13      0t0    73154 pipe
nc      5396 nybo    2w   REG    8,2        0 10356710 /home/nybo/nohup.out
nc      5396 nybo    3u  IPv4  72605      0t0      UDP *:12345 
nybo@nybo-Latitude-7480:~$ lsof -p 5395 | grep -E '([0-9]+[wu])|([0-9]+[wu].+[2,1]u)'
sh      5395 nybo    0w   CHR    1,3      0t0        6 /dev/null
sh      5395 nybo    1w   REG    8,2        0 10356710 /home/nybo/nohup.out
sh      5395 nybo    2w   REG    8,2        0 10356710 /home/nybo/nohup.out






# test 
nohup sh -c "nc -l -u -k 12345 | python3 ~/github/embedded/arm-gamepad/xarm-local.py &>/dev/null" &
nohup sh -c "nc -l -u -k 12345 | python3 ~/github/embedded/arm-gamepad/xarm-local.py " &>/dev/null &

pstree -p  | grep -C10 "sh"


```

### Testing, sending drawing data over http
```sh

import asyncio
import websockets

async def echo(websocket, path):
    async for message in websocket:
        print(f"Received message: {message}")
        await websocket.send(f"Echo: {message}")

start_server = websockets.serve(echo, "localhost", 8765)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()




<!DOCTYPE html>
<html>
<head>
    <title>WebSocket Drawing</title>
    <style>
        /* Simple styling for the drawing canvas */
        #drawingCanvas {
            border: 1px solid black;
        }
    </style>
</head>
<body>
    <canvas id="drawingCanvas" width="500" height="500"></canvas>

    <script>
        var canvas = document.getElementById("drawingCanvas");
        var context = canvas.getContext("2d");
        var ws = new WebSocket("ws://localhost:8765");
        var drawing = false;

        function startDrawing(event) {
            drawing = true;
            draw(event); // This ensures a dot is drawn when the mouse is clicked but not moved
        }

        function draw(event) {
            if (!drawing) return;

            var x = event.offsetX;
            var y = event.offsetY;

            context.fillRect(x, y, 2, 2); // Draw a small square

            var message = JSON.stringify({x: x, y: y});
            ws.send(message); // Send coordinates as a JSON string
        }

        function stopDrawing() {
            drawing = false;
        }

        canvas.addEventListener("mousedown", startDrawing);
        canvas.addEventListener("mousemove", draw);
        canvas.addEventListener("mouseup", stopDrawing);
        canvas.addEventListener("mouseout", stopDrawing);
    </script>
</body>
</html>
```

