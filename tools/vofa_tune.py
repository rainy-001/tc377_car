#!/usr/bin/env python3
"""VOFA+ JustFloat console for the TC377 line-follower.

The firmware sends 4 little-endian float32 values followed by the 4-byte
JustFloat tail 00 00 80 7F at 50 Hz.  This tool prints them and lets you
send the same ASCII tuning commands VOFA+ would send.

Commands are defined in cmd.json (same folder), so both this tool and the
human reading the table use one source of truth.

Examples
--------
  python vofa_tune.py --list
  python vofa_tune.py --list-cmds
  python vofa_tune.py --port COM5 --baud 460800
  python vofa_tune.py --port COM5 --csv run1.csv
  python vofa_tune.py --send pid_all 2.2 0.6 0 --port COM5
  python vofa_tune.py --send curve 0.255 40 --dry-run

Interactive host commands (inside the console):
  :list                 show cmd.json commands
  :send <id> [values]   build and send one of them
  :q                    quit

Requires: pyserial  (pip install pyserial)
"""

import argparse
import json
import os
import struct
import sys
import threading
import time

TAIL = b"\x00\x00\x80\x7f"
CH_NAMES = ["A1", "A2", "ERR", "YAW"]
HERE = os.path.dirname(os.path.abspath(__file__))
CMD_FILE = os.path.join(HERE, "cmd.json")


def list_ports():
    from serial.tools import list_ports as lp
    ports = list(lp.comports())
    if not ports:
        print("No serial ports found.")
        return
    for p in ports:
        print(f"{p.device:10s} {p.description}")


def load_commands(path=CMD_FILE):
    try:
        with open(path, "r", encoding="utf-8") as f:
            data = json.load(f)
    except FileNotFoundError:
        print(f"[warn] {path} not found, command shortcuts disabled")
        return []
    return data.get("commands", [])


def find_command(commands, cid):
    for c in commands:
        if c.get("id") == cid:
            return c
    return None


def format_command(spec, values):
    """Fill a cmd.json template, clamping numbers to the declared range."""
    args = spec.get("args", [])
    if len(values) > len(args):
        raise ValueError(f"{spec['id']} takes at most {len(args)} values")

    filled = {}
    for i, a in enumerate(args):
        raw = values[i] if i < len(values) else a.get("default")
        if raw is None:
            raise ValueError(f"missing value for {a['name']}")
        try:
            v = float(raw)
        except (TypeError, ValueError):
            raise ValueError(f"{a['name']} must be a number")

        lo, hi = a.get("min"), a.get("max")
        if lo is not None and v < lo:
            print(f"[clamp] {a['name']} {v:g} -> {lo:g}")
            v = lo
        if hi is not None and v > hi:
            print(f"[clamp] {a['name']} {v:g} -> {hi:g}")
            v = hi
        filled[a["name"]] = f"{v:g}"

    return spec["template"].format(**filled)


def print_commands(commands):
    if not commands:
        print("no commands loaded")
        return
    for c in commands:
        args = " ".join(f"<{a['name']}>" for a in c.get("args", []))
        print(f"{c['id']:9s} {c['template']:26s} {c.get('desc', '')}")
        if args:
            print(f"{'':9s} args: {args}")


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
    ap.add_argument("--ch", type=int, default=4, help="channel count")
    ap.add_argument("--csv", help="also log frames to this CSV file")
    ap.add_argument("--list", action="store_true", help="list serial ports")
    ap.add_argument("--list-cmds", action="store_true", help="print cmd.json commands")
    ap.add_argument("--send", nargs="+", metavar="CMD",
                    help="send one command and exit: --send pid_all 2.2 0.6 0")
    ap.add_argument("--dry-run", action="store_true",
                    help="print the assembled command instead of sending it")
    args = ap.parse_args()

    commands = load_commands()

    if args.list:
        list_ports()
        return 0

    if args.list_cmds:
        print_commands(commands)
        return 0

    if args.send:
        cid, values = args.send[0], args.send[1:]
        spec = find_command(commands, cid)
        if spec is None:
            ap.error(f"unknown command id: {cid} (use --list-cmds)")
        try:
            text = format_command(spec, values)
        except ValueError as exc:
            ap.error(str(exc))

        if args.dry_run or not args.port:
            print(text)
            return 0

        import serial
        with serial.Serial(args.port, args.baud, timeout=0.5) as ser:
            ser.write((text + "\n").encode("ascii", "ignore"))
            ser.flush()
        print(f"sent: {text}")
        return 0

    if not args.port:
        ap.error("--port is required (use --list to see the ports)")

    import serial

    ser = serial.Serial(args.port, args.baud, timeout=0.05)
    print(f"opened {args.port} @ {args.baud}")
    print("channels: " + " ".join(CH_NAMES[:args.ch]))
    print("raw line = send as-is | :list | :send <id> [v...] | :q")

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

            if line.startswith(":"):
                parts = line[1:].split()
                if not parts:
                    continue
                op = parts[0].lower()
                if op in ("q", "quit", "exit"):
                    break
                if op == "list":
                    print_commands(commands)
                    continue
                if op == "send":
                    if len(parts) < 2:
                        print("usage: :send <id> [values...]")
                        continue
                    spec = find_command(commands, parts[1])
                    if spec is None:
                        print(f"unknown id: {parts[1]} (try :list)")
                        continue
                    try:
                        text = format_command(spec, parts[2:])
                    except ValueError as exc:
                        print(f"[error] {exc}")
                        continue
                    ser.write((text + "\n").encode("ascii", "ignore"))
                    print(f"sent: {text}")
                    continue
                print(f"unknown host command: {op}")
                continue

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

    return 0


if __name__ == "__main__":
    sys.exit(main())
