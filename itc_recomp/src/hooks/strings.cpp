// The game's character set conversions natively: adapters in front of
// src/helpers/strings.cpp. UTF-8 stays with the game's own converters
// (sub_82297340, sub_82297540, guest calls); errors are set with
// last_error::Set.

#include "guest.h"
#include "guest_call.h"
#include "last_error.h"

#include "helpers/strings.h"

#include <rex/logging.h>

#include <vector>

using guest_call::InFrame;

namespace {

constexpr uint32_t kCodePageUtf8 = 65001;
constexpr uint32_t kErrorInsufficientBuffer = 122;

}  // namespace

// MultiByteToWideChar: r3 = code page, r4 = flags (unused), r5 = source,
// r6 = its length (-1: terminated), r7 = destination, r8 = its room in
// characters; returns the characters converted (or needed), 0 on error.
extern "C" REX_FUNC(sub_82296648) {
  REX_FUNC_PROLOGUE();
  const uint32_t code_page = ctx.r3.u32;
  const uint32_t source = ctx.r5.u32;
  const int32_t source_length = ctx.r6.s32;
  const uint32_t destination = ctx.r7.u32;
  const int32_t destination_length = ctx.r8.s32;
  if (code_page == kCodePageUtf8) {
    InFrame(ctx, base, 96, 0x82296680, [&](uint32_t) {
      ctx.r3.u64 = source;
      ctx.r4.s64 = source_length;
      ctx.r5.u64 = destination;
      ctx.r6.s64 = destination_length;
      sub_82297340(ctx, base);
    });
    return;
  }
  int32_t length = source_length;
  if (length == -1) {
    uint32_t n = 0;
    while (guest::LoadU8(base, source + n)) {
      n++;
    }
    length = int32_t(n + 1);
  }
  switch (helpers::PlanConversion(length, destination_length)) {
    case helpers::Conversion::kMeasure:
      ctx.r3.s64 = length;
      return;
    case helpers::Conversion::kTooSmall:
      last_error::Set(base, ctx.r13.u32, kErrorInsufficientBuffer);
      ctx.r3.u64 = 0;
      return;
    case helpers::Conversion::kConvert:
      break;
  }
  std::vector<uint8_t> in(static_cast<size_t>(length));
  for (int32_t i = 0; i < length; i++) {
    in[i] = guest::LoadU8(base, source + uint32_t(i));
  }
  std::vector<uint16_t> out(static_cast<size_t>(length));
  helpers::Widen(in.data(), out.data(), in.size());
  for (int32_t i = 0; i < length; i++) {
    guest::StoreU16(base, destination + 2 * uint32_t(i), out[i]);
  }
  ctx.r3.s64 = length;
}

// WideCharToMultiByte: r3 = code page, r4 = flags (unused), r5 = source,
// r6 = its length (-1: terminated), r7 = destination, r8 = its room in bytes,
// r9 = the default character (unused), r10 = "default used" (out, always 0;
// may be 0); returns the bytes converted (or needed), 0 on error.
extern "C" REX_FUNC(sub_82296720) {
  REX_FUNC_PROLOGUE();
  const uint32_t code_page = ctx.r3.u32;
  const uint32_t source = ctx.r5.u32;
  const int32_t source_length = ctx.r6.s32;
  const uint32_t destination = ctx.r7.u32;
  const int32_t destination_length = ctx.r8.s32;
  if (ctx.r10.u32) {
    guest::StoreU32(base, ctx.r10.u32, 0);
  }
  if (code_page == kCodePageUtf8) {
    InFrame(ctx, base, 128, 0x82296764, [&](uint32_t) {
      ctx.r3.u64 = source;
      ctx.r4.s64 = source_length;
      ctx.r5.u64 = destination;
      ctx.r6.s64 = destination_length;
      sub_82297540(ctx, base);
    });
    return;
  }
  int32_t length = source_length;
  if (length == -1) {
    uint32_t n = 0;
    while (guest::LoadU16(base, source + 2 * n)) {
      n++;
    }
    length = int32_t(n + 1);
  }
  switch (helpers::PlanConversion(length, destination_length)) {
    case helpers::Conversion::kMeasure:
      ctx.r3.s64 = length;
      return;
    case helpers::Conversion::kTooSmall:
      last_error::Set(base, ctx.r13.u32, kErrorInsufficientBuffer);
      ctx.r3.u64 = 0;
      return;
    case helpers::Conversion::kConvert:
      break;
  }
  std::vector<uint16_t> in(static_cast<size_t>(length));
  for (int32_t i = 0; i < length; i++) {
    in[i] = guest::LoadU16(base, source + 2 * uint32_t(i));
  }
  std::vector<uint8_t> out(static_cast<size_t>(length));
  helpers::Narrow(in.data(), out.data(), in.size());
  for (int32_t i = 0; i < length; i++) {
    guest::StoreU8(base, destination + uint32_t(i), out[i]);
  }
  ctx.r3.s64 = length;
}

// swprintf: r3 = the buffer, r4 = the format, then its arguments; returns the
// characters written (without the terminator), -1 for no buffer or format.
// Only one call site, with a format of no conversions (see
// helpers::FormatWithoutArguments).
extern "C" REX_FUNC(sub_82296870) {
  REX_FUNC_PROLOGUE();
  const uint32_t buffer = ctx.r3.u32;
  const uint32_t format_address = ctx.r4.u32;
  if (!buffer || !format_address) {
    ctx.r3.s64 = -1;
    return;
  }
  std::u16string format;
  for (uint32_t a = format_address;; a += 2) {
    char16_t c = char16_t(guest::LoadU16(base, a));
    if (!c) {
      break;
    }
    format += c;
  }
  std::u16string text;
  if (!helpers::FormatWithoutArguments(format, &text)) {
    REXLOG_ERROR("swprintf: format at {:08X} (from {:08X}) has conversions; not written",
                 format_address, uint32_t(ctx.lr));
  }
  for (size_t i = 0; i < text.size(); i++) {
    guest::StoreU16(base, buffer + 2 * uint32_t(i), uint16_t(text[i]));
  }
  guest::StoreU16(base, buffer + 2 * uint32_t(text.size()), 0);
  ctx.r3.s64 = int64_t(text.size());
}
