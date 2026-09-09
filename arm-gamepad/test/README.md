# Drawing experiments recovered from LAT

These files preserve the existing LAT work from September 9, 2026:

- `index.html`: canvas input with coordinate adjustment and an HTTP sender.
- `remote.py`: Flask receiver forwarding coordinates as a UDP vector.
- `local.py`: xArm movement driven by vectors read from standard input.

The Python files pass syntax checks. Browser, networking and robot motion have
not been tested during recovery. Addresses are the original fixed LAN values;
the files are experiments, and do not provide a complete launch pipeline.
The main gamepad scripts in the parent directory are unchanged.
