#!/bin/sh
# Syntax-checks one source of the itc target without touching the build
# folder: check.sh src/hooks/<file>.cpp (from itc_recomp/).
set -e
B="$(cd "$(dirname "$0")/../out/build/linux-amd64-release" && pwd)"
cmd=$(ninja -C "$B" -t commands "CMakeFiles/itc.dir/$1.o" | tail -1)
[ -n "$cmd" ] || { echo "no compile command for $1" >&2; exit 2; }
cmd=$(printf '%s' "$cmd" | sed -E 's/ -MD//; s/ -MT [^ ]+//; s/ -MF [^ ]+//; s/ -o [^ ]+/ -fsyntax-only/')
cd "$B" && eval "$cmd"
