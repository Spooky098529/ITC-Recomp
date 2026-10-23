// The C++ library's narrow string natively: adapters in front of
// src/std_string/std_string.cpp (docs/functions/std-string.md). The
// functions that allocate or free run natively only while --native_heap
// routes the game's heap to the allocation layer, and hand the paths left to
// the original (see std_string.h) back to it from the start, nothing having
// been done. allocator<char>::allocate (sub_820912B0), _Copy (sub_82091458)
// and _Grow (sub_82091540) have no adapters any more: only the plain C++ of
// their native callers (assign, append) calls them in play (a recompiled
// caller reaching them runs the original, and the flag switches those
// callers). The wildcard find (sub_82592970) and search (sub_82592740) have
// no adapters: only the wildcard replace's plain C++ calls them in play (a
// recompiled caller reaching them runs the original, and the flag switches
// the replace). The registers the originals leave that a caller could read (r3)
// are left the same.

#include "guest.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "std_string/std_string.h"

ITC_FLAG_BOOL(native_std_string, true, "ITC",
              "Run the C++ library's narrow string (docs/functions/std-string.md: _Tidy "
              "sub_82091328, assign sub_82091720 and sub_82091808 and append sub_820A6F60 with "
              "allocator<char>::allocate sub_820912B0, _Copy sub_82091458 and _Grow sub_82091540 "
              "(no adapters of their own), with --native_heap; the constructors sub_825929E8, sub_825931D0, sub_82593248 and "
              "sub_82593040, the last three with --native_heap; the "
              "wildcard replace sub_82593088 with its find sub_82592970 and search "
              "sub_82592740, no adapters of their own; replace sub_82592E18, for a match, on "
              "the translated code) "
              "natively instead of through the translated code (same results; checked by "
              "--run_harnesses=std_string)");

using namespace std_string;

namespace {

// The functions that free or allocate: only over the allocation layer.
bool NativeLifetime() {
  return ITC_FLAG(native_std_string) && hooks::NativeHeapActive();
}

}  // namespace

// _Tidy: r3 = the string, r4 = built (its low byte), r5 = the new length.
// The original leaves r3 as operator delete leaves it when it frees the
// block (the block: free's plain C++ keeps r3), else the string; a few
// callers return right after it, so r3 is kept the same.
extern "C" REX_FUNC(sub_82091328) {
  REX_FUNC_PROLOGUE();
  const uint32_t string = ctx.r3.u32;
  const bool built = (ctx.r4.u32 & 0xFF) != 0;
  const bool frees = built && guest::LoadU32(base, string + kCapacity) >= kInPlace;
  const uint32_t block = guest::LoadU32(base, string + kText);
  if (!NativeLifetime() || !Tidy(base, string, built, ctx.r5.u32)) {
    __imp__sub_82091328(ctx, base);
    return;
  }
  if (frees) {
    ctx.r3.u64 = block;
  }
}

// assign(const string&, size_t, size_t): r3 = the string, r4 = the source,
// r5 = the offset, r6 = the count; r3 the string (left as it came).
extern "C" REX_FUNC(sub_82091720) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() ||
      !AssignFrom(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    __imp__sub_82091720(ctx, base);
  }
}

// assign(const char*, size_t): r3 = the string, r4 = the text, r5 = the
// count; r3 the string (left as it came).
extern "C" REX_FUNC(sub_82091808) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !AssignText(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_82091808(ctx, base);
  }
}

// append(const char*, size_t): r3 = the string, r4 = the text, r5 = the
// count; r3 the string (left as it came).
extern "C" REX_FUNC(sub_820A6F60) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !Append(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_820A6F60(ctx, base);
  }
}

// string(): r3 = the string, returned (r4, the allocator, not read). It
// neither allocates nor frees.
extern "C" REX_FUNC(sub_825929E8) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_std_string)) {
    __imp__sub_825929E8(ctx, base);
    return;
  }
  Construct(base, ctx.r3.u32);
}

// string(const char*): r3 = the string, r4 = the text; r3 the string (left
// as it came).
extern "C" REX_FUNC(sub_825931D0) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !ConstructFromText(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_825931D0(ctx, base);
  }
}

// string(const char*, const allocator&): r3 = the string, r4 = the text (r5,
// the allocator, not read); r3 the string (left as it came).
extern "C" REX_FUNC(sub_82593248) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() || !ConstructFromTextWithAllocator(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_82593248(ctx, base);
  }
}

// string(const string&, size_t, size_t): r3 = the string, r4 = the source,
// r5 = the offset, r6 = the count (r7, the allocator, not read); r3 the
// string (left as it came).
extern "C" REX_FUNC(sub_82593040) {
  REX_FUNC_PROLOGUE();
  if (!NativeLifetime() ||
      !ConstructFrom(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32)) {
    __imp__sub_82593040(ctx, base);
  }
}

// The wildcard replace: r3 = the string, r4 = the pattern, r5 = the
// replacement, r6 = how many (0 for all); r3 the string's text. Its frame
// where the original's would be (144 bytes down). A match (replace, never in
// play) runs the original.
extern "C" REX_FUNC(sub_82593088) {
  REX_FUNC_PROLOGUE();
  if (ITC_FLAG(native_std_string)) {
    if (const auto text = ReplaceAll(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 144)) {
      ctx.r3.u64 = *text;
      return;
    }
  }
  __imp__sub_82593088(ctx, base);
}
