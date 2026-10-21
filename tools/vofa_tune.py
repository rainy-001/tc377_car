#!/usr/bin/env python3
"""VOFA+ JustFloat console for the TC377 line-follower.

The firmware sends 12 little-endian float32 values followed by the 4-byte
JustFloat tail 00 00 80 7F at 50 Hz.  This tool prints them and lets you
send the same ASCII tuning commands VOFA+ would send.

Examples
--------
  python vofa_tune.py --list
  python vofa_tune.py --port COM5 --baud 460800
  python vofa_tune.py --port COM5 --csv run1.csv

Commands (one per line):
  p 2.0 0.5 0            both motors PID
  p1 2.2 0.6 0           motor 1 only (p2 for motor 2)
  spd 600                straight base speed
  dir 1.6 0.035 0.45     steering Kp Kp2 Kd
  curve 0.255 40         min speed ratio / curve error reference
  diff 1.35              differential gain
  thr 20                 binarization threshold offset (-60..60)
  start / stop           start / stop the car

Requires: pyserial  (pip install pyserial)
"""

import argparse
import struct
import sys
import threading
import time

TAIL = b"\x00\x00\x80\x7f"
CH_NAMES = ["T1", "A1", "T2", "A2", "D1%", "D2%",
            "ERR", "SDR", "RAT", "SPD", "YAW", "VLD"]


def list_ports():
    from serial.tools import list_ports as lp
    ports = list(lp.comports())
    if not ports:
        print("No serial ports found.")
        return
    for p in ports:
        print(f"{p.device:10s} {p.description}")


def serial_reader(ser, nch, stop, csv_file):
    """Parse JustFloat frames until stop is set."""
    buf = bytearray()
    last_print = 0.0
    last_flush = time.time()

    while not stop.is_set():
        try:
            chunk = ser.read(64)
        except Exception as exc:               # port unplugged
            print(f"\n[serial error] {exc}")
            stop.set()
            return

        if chunk:
            buf.extend(chunk)

        while True:
            idx = buf.find(TAIL)
            if idx < 0:
                if len(buf) > 8192:
                    del buf[:-1024]
                break

            frame = bytes(buf[:idx])
            del buf[:idx + 4]

            if len(frame) < nch * 4:
                continue

            values = struct.unpack("<%df" % nch, frame[-nch * 4:])

            now = time.time()
            if now - last_print >= 0.2:
                last_print = now
                print("  ".join(f"{v:8.2f}" for v in values))

            if csv_file is not None:
                csv_file.write(",".join(f"{v:.4f}" for v in values) + "\n")
                if now - last_flush >= 1.0:
                    last_flush = now
                    csv_file.flush()


def main():
    ap = argparse.ArgumentParser(description="VOFA+ JustFloat console")
    ap.add_argument("--port", help="COM port, e.g. COM5")
    ap.add_argument("--baud", type=int, default=460800)
    ap.add_argument("--ch", type=int, default=12, help="channel count")
    ap.add_argument("--csv", help="also log frames to this CSV file")
    ap.add_argument("--list", action="store_true", help="list serial ports")
    args = ap.parse_args()

    if args.list:
        list_ports()
        return

    if not args.port:
        ap.error("--port is required (use --list to see the ports)")

    import serial

    ser = serial.Serial(args.port, args.baud, timeout=0.05)
    print(f"opened {args.port} @ {args.baud}")
    print("channels: " + " ".join(CH_NAMES[:args.ch]))
    print("type a command + Enter (empty line quits)")

    csv_file = open(args.csv, "w", encoding="utf-8", newline="") if args.csv else None
    if csv_file:
        csv_file.write(",".join(CH_NAMES[:args.ch]) + "\n")

    stop = threading.Event()
    reader = threading.Thread(target=serial_reader,
                              args=(ser, args.ch, stop, csv_file),
                              daemon=True)
    reader.start()

    try:
        for line in sys.stdin:
            line = line.strip()
            if not line:
                break
            ser.write((line + "\n").encode("ascii", "ignore"))
        time.sleep(0.2)
    except KeyboardInterrupt:
        pass
    finally:
        stop.set()
        reader.join(timeout=1.0)
        if csv_file:
            csv_file.close()
        ser.close()
        print("closed")


if __name__ == "__main__":
    main()
