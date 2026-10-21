#!/bin/bash
# Generates the files FFmpeg's configure makes, for the build of upstream
# FFmpeg (itc_recomp/thirdparty/FFmpeg, a submodule) with only the xma2
# decoder (B2 in docs/rexglue-removal.md), and keeps them in
# itc_recomp/thirdparty/ffmpeg-config/<platform>/: config.h,
# config_components.h, libavutil/avconfig.h and ffversion.h, libavcodec's
# codec, parser and bsf lists, and sources.txt (the .c files the build
# compiles, from a real build). itc_recomp/cmake/ffmpeg.cmake builds from
# them, so configure never runs in the game's build (it can't for the Windows
# cross-compile). Run again after moving the submodule to another release.
#
#   itc_recomp/tools/ffmpeg_config.sh
#
# Run from the repo root. Needs clang, make, and for Windows the xwin SDK
# (itc_recomp/out/xwin-sdk, docs/windows.md). Local paths in config.h's
# FFMPEG_CONFIGURATION (the configure line) are replaced, as the repo is
# public.
set -eu
R=$PWD
SRC=$R/itc_recomp/thirdparty/FFmpeg
OUT=$R/itc_recomp/thirdparty/ffmpeg-config
XWIN=$R/itc_recomp/out/xwin-sdk
[ -f "$SRC/configure" ] || { echo "no $SRC/configure: git submodule update --init" >&2; exit 1; }
WORK=$(mktemp -d)
trap 'rm -rf "$WORK"' EXIT

# Only libavutil and libavcodec, only the xma2 decoder, plain C (no assembly),
# threads on (FFmpeg's one-time set-up isn't thread-safe without them).
COMMON=(--disable-everything --enable-decoder=xma2 --disable-programs --disable-doc
        --disable-avdevice --disable-avformat --disable-swscale --disable-swresample
        --disable-avfilter --disable-network --disable-autodetect --disable-x86asm
        --disable-inline-asm --disable-debug --enable-static --disable-shared)

generate() {  # <platform> <configure flags...>
  local platform=$1; shift
  local build=$WORK/$platform
  mkdir -p "$build"
  (cd "$build" && "$SRC/configure" "$@" "${COMMON[@]}" >configure.log 2>&1) ||
    { tail -20 "$build/ffbuild/config.log" >&2; exit 1; }
  make -C "$build" -j"$(nproc)" V=1 >"$build/make.log" 2>&1 ||
    { tail -20 "$build/make.log" >&2; exit 1; }
  local dest=$OUT/$platform
  rm -rf "$dest"
  mkdir -p "$dest/libavutil" "$dest/libavcodec"
  for f in config.h config_components.h libavutil/avconfig.h libavutil/ffversion.h \
           libavcodec/codec_list.c libavcodec/parser_list.c libavcodec/bsf_list.c; do
    sed -e "s|$XWIN|<xwin-sdk>|g" -e "s|$R/||g" -e "s|$HOME|~|g" "$build/$f" >"$dest/$f"
  done
  grep -oE '\-o (libavcodec|libavutil)/[^ ]+\.o' "$build/make.log" | sed -e 's/^-o //' -e 's/\.o$/.c/' |
    sort -u >"$dest/sources.txt"
  echo "$platform: $(wc -l <"$dest/sources.txt") sources"
}

generate linux-x86_64 --cc=clang --enable-pthreads

WINCC="clang --target=x86_64-pc-windows-msvc -march=x86-64-v2 -Xmicrosoft-windows-sys-root $XWIN"
generate windows-x86_64 --enable-cross-compile --target-os=win64 --arch=x86_64 \
  --cc="$WINCC" --ld="$WINCC -fuse-ld=lld" --ar=llvm-ar --nm=llvm-nm --ranlib=llvm-ranlib \
  --disable-stripping --enable-w32threads

if grep -rn "/home/\|$WORK" "$OUT"; then
  echo "local paths left in $OUT" >&2
  exit 1
fi
