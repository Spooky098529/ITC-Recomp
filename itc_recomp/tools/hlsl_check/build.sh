#!/bin/sh
# Builds hlsl_check.exe into itc_recomp/out/tools/ with the cross toolchain
# (clang and the xwin SDK, docs/windows.md). Run from the repo root.
set -e
XWIN=itc_recomp/out/xwin-sdk
mkdir -p itc_recomp/out/tools
clang++ --target=x86_64-pc-windows-msvc -fuse-ld=lld -O1 -std=c++17 \
  -fms-runtime-lib=static -Xmicrosoft-windows-sys-root "$XWIN" \
  itc_recomp/tools/hlsl_check/hlsl_check.cpp -o itc_recomp/out/tools/hlsl_check.exe
echo itc_recomp/out/tools/hlsl_check.exe
