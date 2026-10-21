#!/usr/bin/env python3
"""Import vofa+.cmds.json into the VOFA+ command panel.

VOFA+ keeps its command tree in
    %LOCALAPPDATA%\\vofa+\\<vnumber>\\context\\vofa+.cmds.json
This script finds that file, backs it up, and merges the group defined in
vofa+.cmds.json into it.  Other groups are left untouched.

Close VOFA+ before running (otherwise it may overwrite the file on exit),
then reopen it.

Examples
--------
  python vofa_import_cmds.py --dry-run
  python vofa_import_cmds.py
  python vofa_import_cmds.py --file "C:\\path\\to\\vofa+.cmds.json"
"""

import argparse
import glob
import json
import os
import shutil
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_SRC = os.path.join(os.path.dirname(HERE), "vofa+.cmds.json")


def find_live_file():
    base = os.path.join(os.environ.get("LOCALAPPDATA", ""), "vofa+")
    found = glob.glob(os.path.join(base, "*", "context", "vofa+.cmds.json"))
    if not found:
        return None
    return max(found, key=os.path.getmtime)


def load_json(path):
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def empty_tree(vnumber):
    return {
        "type": "cmds",
        "vnumber": vnumber,
        "ctx": {
            "name": "group", "intro_on": True, "intro": "", "hex_on": False,
            "loop_on": True, "loop_ms": 100, "loop_count": 1, "cmd_hex": "",
            "is_group": True, "down": True, "subCmds": [],
        },
    }


def main():
    ap = argparse.ArgumentParser(description="import commands into VOFA+")
    ap.add_argument("--src", default=DEFAULT_SRC, help="command group JSON")
    ap.add_argument("--file", help="target vofa+.cmds.json (auto-detect by default)")
    ap.add_argument("--name", default="TC377 调参", help="group name")
    ap.add_argument("--dry-run", action="store_true", help="do not write")
    args = ap.parse_args()

    if not os.path.exists(args.src):
        print(f"source not found: {args.src}")
        return 1

    group = load_json(args.src)["ctx"]
    if not group.get("is_group"):
        print("source ctx is not a group")
        return 1

    target = args.file or find_live_file()
    if not target:
        print("VOFA+ command file not found.")
        print("Start VOFA+ once (so it creates the file), or pass --file.")
        return 1

    if os.path.exists(target):
        live = load_json(target)
    else:
        live = empty_tree(group.get("vnumber", 100))

    root = live.setdefault("ctx", {})
    subs = root.setdefault("subCmds", [])

    # replace an older copy of the same group, keep every other group
    subs = [c for c in subs if c.get("name") != args.name]
    subs.append(group)
    root["subCmds"] = subs
    live.setdefault("type", "cmds")
    live.setdefault("vnumber", group.get("vnumber", 100))

    text = json.dumps(live, ensure_ascii=False, separators=(",", ":"))

    print(f"source : {args.src}")
    print(f"target : {target}")
    print(f"group  : {args.name}  ({len(group.get('subCmds', []))} commands)")

    if args.dry_run:
        print("dry-run: nothing written")
        return 0

    if os.path.exists(target):
        backup = target + ".bak-" + time.strftime("%Y%m%d-%H%M%S")
        shutil.copy2(target, backup)
        print(f"backup : {backup}")

    with open(target, "w", encoding="utf-8") as f:
        f.write(text)

    print("done. restart VOFA+ and open the command panel.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
