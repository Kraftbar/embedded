import contextlib
import importlib.util
import io
from pathlib import Path
import signal
import sys
import tempfile
import types
import unittest
from unittest.mock import patch


class SerialError(Exception):
    pass


class TracerTests(unittest.TestCase):
    def setUp(self):
        self.serial = types.SimpleNamespace(SerialException=SerialError)
        path = Path(__file__).resolve().parents[1] / "scripts" / "uart_tracer.py"
        spec = importlib.util.spec_from_file_location("uart_tracer_under_test", path)
        self.tracer = importlib.util.module_from_spec(spec)
        with patch.dict(sys.modules, {"serial": self.serial}):
            spec.loader.exec_module(self.tracer)

    def test_color_is_console_only(self):
        line = "(12:34:56.789) E (123) main: failure"
        console, log = io.StringIO(), io.StringIO()
        with contextlib.redirect_stdout(console):
            self.tracer.emit([line], log)
        self.assertIn(self.tracer.ANSI_RED, console.getvalue())
        self.assertEqual(log.getvalue(), line + "\n")

    def test_sigint_finishes_read_and_flushes_partial_line(self):
        class Port:
            def __init__(self, **kwargs):
                self.is_open = False
                self.data = b"last partial line"

            def __enter__(self):
                return self

            def __exit__(self, *args):
                self.is_open = False

            def open(self):
                self.is_open = True

            @property
            def in_waiting(self):
                if self.data:
                    return len(self.data)
                signal.raise_signal(signal.SIGINT)
                if not self.is_open:
                    raise AssertionError("signal handler closed an active descriptor")
                return 0

            def read(self, size):
                data, self.data = self.data, b""
                return data

            def close(self):
                self.is_open = False

        self.serial.Serial = Port
        old_handler = signal.getsignal(signal.SIGINT)
        with tempfile.TemporaryDirectory() as tmp:
            logfile = Path(tmp) / "serial.log"
            args = ["uart_tracer.py", "--port", "mock", "--log-file", str(logfile)]
            with patch.object(sys, "argv", args), contextlib.redirect_stdout(io.StringIO()):
                self.assertEqual(self.tracer.main(), 0)
            self.assertIn("last partial line\n", logfile.read_text())
        self.assertEqual(signal.getsignal(signal.SIGINT), old_handler)


if __name__ == "__main__":
    unittest.main()
