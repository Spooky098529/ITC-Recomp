// The C runtime's narrow printf (src/c_runtime/narrow_format.cpp:
// sprintf sub_825A93D0 and _vsnprintf sub_825AB010, with _vsnprintf_l and
// _output_l) against the recompiled ones (--native_c_runtime off, so their
// callees are the recompiled code too). Formats made of literal bytes (some
// above 127) and conversions: flags, a width (digits or '*'), a precision
// (digits or '*'), a size (h, l, ll, w, I64, I32, I) and a type (d i u o x X
// p c C s S Z f, e and g (the original), '%' and a letter that isn't a
// type); the game's own formats; the arguments to match, in r5 to r10 and
// on the stack (sprintf) or in a va_list (_vsnprintf): ints (small, edges,
// random), strings (narrow and wide, or null), counted strings, doubles
// (odd and ordinary values). Also outputs over the format or a string
// (the native version declines: the original runs), and _vsnprintf's sizes:
// none with no buffer (only counted), too small (the original's _flsbuf),
// exact, and above INT_MAX. Not %n or null buffers and formats (the
// original's _invalid_parameter). Compared: the output block, the scratch
// area, the caller's parameter area and stack arguments, r3.

#include "harness.h"

#include "base/flags.h"
#include "c_runtime/format.h"
#include "guest.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <iterator>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

ITC_FLAG_DECLARE(bool, native_c_runtime);

namespace harness {

namespace {

// The register steps: the adapters' work without the flag.
void NativeSprintf(PPCContext& ctx, uint8_t* base) {
  const uint64_t registers[6] = {ctx.r5.u64, ctx.r6.u64, ctx.r7.u64,
                                 ctx.r8.u64, ctx.r9.u64, ctx.r10.u64};
  for (uint32_t i = 0; i < 6; i++) {
    guest::StoreU32(base, ctx.r1.u32 + 32 + 8 * i, uint32_t(registers[i] >> 32));
    guest::StoreU32(base, ctx.r1.u32 + 36 + 8 * i, uint32_t(registers[i]));
  }
  int32_t count;
  if (!c_runtime::Sprintf(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 + 32,
                          ctx.r1.u32 - 144 - 1328, &count)) {
    __imp__sub_825A93D0(ctx, base);
    return;
  }
  ctx.r3.u64 = uint32_t(count);
}

void NativeVsnprintf(PPCContext& ctx, uint8_t* base) {
  int32_t count;
  if (!c_runtime::Vsnprintf(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                            ctx.r1.u32 - 144 - 1328, &count)) {
    __imp__sub_825AB010(ctx, base);
    return;
  }
  ctx.r3.u64 = uint32_t(count);
}

void Run(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t, uint8_t* base) {
  if (v == 0) {
    ITC_FLAG(native_c_runtime) = false;
    recompiled(t, base);
    ITC_FLAG(native_c_runtime) = true;
  } else {
    native(t, base);
  }
}

struct Block {
  explicit Block(uint32_t bytes) : bytes(bytes), address(memory::Alloc(bytes, 0)) {}
  ~Block() { memory::Free(address); }
  uint32_t bytes;
  uint32_t address;
};

// The scratch area's layout: eight strings of 64 bytes at +0 (narrow text,
// or wide text in the odd ones), the format at +512, eight counted strings
// (+0 the length, +2 the capacity, +4 the text) at +1024, a va_list at
// +1536.
constexpr uint32_t kStrings = 0, kFormat = 512, kCounted = 1024, kVaList = 1536;

struct FormatCase {
  std::string format;
  std::vector<uint64_t> slots;
};

void AddInt(Random& r, std::vector<uint64_t>& slots, bool wide64) {
  uint64_t v;
  switch (r.Below(5)) {
    case 0: v = r.Next(); break;
    case 1: {
      constexpr uint64_t kEdges[] = {0, 1, 0xFFFFFFFF, 0x80000000, 0x7FFFFFFF, 0xFFFF,
                                     0x8000, 0xFFFFFFFFFFFFFFFFull, 0x8000000000000000ull, 0xFF,
                                     0x41};
      v = kEdges[r.Below(std::size(kEdges))];
      break;
    }
    default: v = uint64_t(int64_t(int32_t(r.Below(200000)) - 100000)); break;
  }
  if (!wide64 && r.Below(2) == 0) v = uint64_t(int64_t(int32_t(uint32_t(v))));
  slots.push_back(v);
}

uint64_t DoubleBits(Random& r) {
  constexpr double kValues[] = {0.0, -0.0, 0.5, -3.25, 123.456, 1e10, 1e-5, 99.995, 2.5, 1e300,
                                -1e-300, 4294967296.5, 0.1, 7.0, 0.05, 1.0 / 3.0};
  switch (r.Below(6)) {
    case 0: return r.Next();
    case 1: {
      constexpr uint64_t kOdd[] = {0x7FF8000000000000ull, 0xFFF0000000000000ull,
                                   0x7FF0000000000000ull, 0x0000000000000001ull,
                                   0x7FF4000000000001ull, 0x8000000000000000ull};
      return kOdd[r.Below(std::size(kOdd))];
    }
    case 2: return std::bit_cast<uint64_t>(kValues[r.Below(std::size(kValues))]);
    default:
      return std::bit_cast<uint64_t>(double(int32_t(r.Below(2000000)) - 1000000) / 100.0);
  }
}

// A string argument: one of the eight strings (at an offset into it, so
// lengths vary), or null now and then.
uint64_t StringArg(Random& r, uint32_t data, bool wide) {
  if (r.Below(8) == 0) return 0;
  const uint32_t k = 2 * r.Below(4) + (wide ? 1 : 0);
  return data + kStrings + 64 * k + (wide ? 2 * r.Below(8) : r.Below(16));
}

FormatCase MakeFormat(Random& r, uint32_t data) {
  FormatCase c;
  const uint32_t pieces = 1 + r.Below(6);
  bool bare_i = false;  // the last piece ended in a bare "I" (below)
  for (uint32_t k = 0; k < pieces; k++) {
    // After a bare "I", a literal letter could be an integer type ("Ix"):
    // the next piece is a conversion.
    if (!std::exchange(bare_i, false) && r.Below(3) == 0) {
      const uint32_t pick = r.Below(30);
      c.format += pick == 0 ? char(0xE9) : pick == 1 ? char(0x80) : pick == 2 ? '\n'
                : pick == 3 ? '\\' : char('a' + r.Below(26));
      continue;
    }
    c.format += '%';
    const char* kFlags = "-+ #0";
    for (uint32_t f = 0; f < 5; f++) {
      if (r.Below(5) == 0) c.format += kFlags[f];
    }
    if (r.Below(3) == 0) {
      if (r.Below(4) == 0) {
        c.format += '*';  // a small width (a huge one would write gigabytes)
        c.slots.push_back(uint64_t(int64_t(int32_t(r.Below(80)) - 40)));
      } else {
        c.format += std::to_string(r.Below(30));
      }
    }
    bool big_precision = false;
    if (r.Below(3) == 0) {
      c.format += '.';
      if (r.Below(4) == 0) {
        c.format += '*';
        c.slots.push_back(uint64_t(int64_t(int32_t(r.Below(60)) - 10)));
      } else if (r.Below(3) != 0) {
        const uint32_t n = r.Below(12) == 0 ? 160 + r.Below(40) : r.Below(25);
        big_precision = n > 100;
        c.format += std::to_string(n);
      }
    }
    const char* kSizes[] = {"", "", "", "h", "l", "ll", "w", "I64", "I32", "I"};
    const std::string size = kSizes[r.Below(std::size(kSizes))];
    c.format += size;
    // ('k' isn't a type: the last conversion's text again. Not 'F' or
    // 'N': the table takes them as sizes, and the arguments after would be
    // read as the wrong kinds. Not 'n': the original's _invalid_parameter.)
    const char* kTypes = "diuoxXpcCsSsZfffdegE%kk";
    const char type = kTypes[r.Below(23)];
    // A bare "I" ends the conversion, and a '%' after it starts a new one,
    // so "%I%" isn't a conversion of its own: the next piece's '%' (or the
    // format's end) goes after the "I".
    if (size == "I" && type == '%') {
      bare_i = true;
      continue;
    }
    c.format += type;
    const bool wide64 = size == "ll" || size == "I64";
    // A bare "I" before anything but an integer type is written out, and
    // the type after it with it: no argument.
    if (size == "I" && std::string_view("diouxX").find(type) == std::string_view::npos) {
      continue;
    }
    const bool wide_size = size == "l" || size == "w";
    switch (type) {
      case 'd': case 'i': case 'u': case 'o': case 'x': case 'X': case 'p':
        AddInt(r, c.slots, wide64);
        break;
      case 'c': case 'C':
        // Wide characters mostly within a byte (wctomb_s, the original's).
        c.slots.push_back(r.Below(4) == 0 ? r.Next() : 0x20 + r.Below(0xE0));
        break;
      case 's':
        c.slots.push_back(StringArg(r, data, wide_size));
        break;
      case 'S':
        c.slots.push_back(StringArg(r, data, size != "h"));
        break;
      case 'Z':
        c.slots.push_back(r.Below(8) == 0 ? 0 : data + kCounted + 8 * r.Below(8));
        break;
      case 'f': case 'e': case 'g': case 'E':
        c.slots.push_back(big_precision && r.Below(2) ? 0x4000000000000000ull : DoubleBits(r));
        break;
      default:
        break;
    }
  }
  return c;
}

// The scratch area's strings and counted strings (the same every case).
void WriteInputs(uint8_t* base, uint32_t data) {
  Random text_random(0x5EED);
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t at = data + kStrings + 64 * k;
    for (uint32_t b = 0; b < 56; b++) {
      uint8_t value;
      if (k % 2 == 1) {
        // Wide text: 20 units, some above a byte.
        value = b >= 40 ? 0 : b % 2 == 0 ? uint8_t(text_random.Below(5) == 0 ? 1 : 0)
                                         : uint8_t('A' + text_random.Below(26));
      } else {
        value = b < 20 + 4 * k ? uint8_t(text_random.Below(10) == 0 ? 0xC0 + text_random.Below(60)
                                                                   : 'a' + text_random.Below(26))
                               : 0;
      }
      guest::StoreU8(base, at + b, value);
    }
    for (uint32_t b = 56; b < 64; b++) guest::StoreU8(base, at + b, 0);
  }
  for (uint32_t k = 0; k < 8; k++) {
    const uint32_t at = data + kCounted + 8 * k;
    // Lengths: small, odd, 0, negative (as int16), a null text for one.
    static const uint16_t kLengths[] = {5, 12, 0, 7, 0xFFF0, 30, 1, 9};
    guest::StoreU16(base, at, kLengths[k]);
    guest::StoreU16(base, at + 2, 64);
    guest::StoreU32(base, at + 4, k == 6 ? 0 : data + kStrings + 64 * (k % 8));
  }
}

enum class Where { kBlock, kOverFormat, kOverString };

void SprintfCase(Context& h, Results& results, const Block& block, const FormatCase& c,
                 uint32_t format_address, Where where, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block.bytes);
    WriteInputs(h.base, h.data);
    if (format_address == 0) {
      Write(h.base, h.data + kFormat, c.format.data(), uint32_t(c.format.size()));
      guest::StoreU8(h.base, h.data + kFormat + uint32_t(c.format.size()), 0);
    }
    PPCContext t = CallContext(h);
    // The caller's parameter area and stack arguments (r1 + 32 on).
    Fill(h.base, t.r1.u32 + 24, 200);
    uint64_t* registers[6] = {&t.r5.u64, &t.r6.u64, &t.r7.u64, &t.r8.u64, &t.r9.u64, &t.r10.u64};
    for (uint32_t k = 0; k < 6; k++) {
      *registers[k] = k < c.slots.size() ? c.slots[k] : 0x1234567800000000ull + k;
    }
    for (uint32_t k = 6; k < c.slots.size() && k < 20; k++) {
      StoreU64(h.base, t.r1.u32 + 32 + 8 * k, c.slots[k]);
    }
    t.r4.u64 = format_address != 0 ? format_address : h.data + kFormat;
    switch (where) {
      case Where::kBlock: t.r3.u64 = block.address; break;
      case Where::kOverFormat: t.r3.u64 = t.r4.u64 - 2; break;
      case Where::kOverString: t.r3.u64 = h.data + kStrings + 64 * 2 + 3; break;
    }
    Run(v, __imp__sub_825A93D0, NativeSprintf, t, h.base);
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Bytes(Read(h.base, t.r1.u32 + 24, 200));
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

void VsnprintfCase(Context& h, Results& results, const Block& block, const FormatCase& c,
                   uint32_t size, bool null_buffer, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    Fill(h.base, block.address, block.bytes);
    WriteInputs(h.base, h.data);
    Write(h.base, h.data + kFormat, c.format.data(), uint32_t(c.format.size()));
    guest::StoreU8(h.base, h.data + kFormat + uint32_t(c.format.size()), 0);
    for (uint32_t k = 0; k < c.slots.size() && k < 40; k++) {
      StoreU64(h.base, h.data + kVaList + 8 * k, c.slots[k]);
    }
    PPCContext t = CallContext(h);
    t.r3.u64 = null_buffer ? 0 : block.address;
    t.r4.u64 = size;
    t.r5.u64 = h.data + kFormat;
    t.r6.u64 = h.data + kVaList;
    Run(v, __imp__sub_825AB010, NativeVsnprintf, t, h.base);
    out[v].Bytes(Read(h.base, block.address, block.bytes));
    out[v].Bytes(Read(h.base, h.data, kScratchBytes));
    out[v].Word(t.r3.u32);
  }
  results.Check(out[0], out[1], what);
}

std::string Printable(const std::string& s) {
  std::string n;
  for (char ch : s) n += uint8_t(ch) < 128 && uint8_t(ch) >= 32 ? ch : '?';
  return n;
}

void SprintfCases(Context& h) {
  Results results("sub_825A93D0 sprintf");
  const Block block(16384);
  const uint32_t d = h.data;
  // The game's formats: "%s%s", "%s%s%d" (the car models' names),
  // "data\audio\%s.xgs" (TXR_XACT.xgs's open), Path::Build's three kinds.
  struct Game {
    uint32_t format;
    const char* args;  // s a string, d an int
  };
  const Game games[] = {{0x82004FE8, "ss"}, {0x82004FA4, "ssd"}, {0x82008350, "s"},
                        {0x8206CC48, "s"}, {0x8206CC54, "s"}, {0x8206CC60, "sss"}};
  for (const Game& g : games) {
    for (int i = 0; i < 40; i++) {
      FormatCase c;
      for (const char* a = g.args; *a; a++) {
        if (*a == 's') {
          c.slots.push_back(StringArg(h.random, d, false));
        } else {
          AddInt(h.random, c.slots, false);
          if (i < 10) c.slots.back() = h.random.Below(12);
        }
      }
      SprintfCase(h, results, block, c, g.format, Where::kBlock,
                  fmt::format("format {:08X} case {}", g.format, i));
    }
  }
  const char* kBuilt[] = {"%d", "%05d", "%-8s|", "%.3s", "%x", "%#x", "%08X", "%p", "%c", "%%",
                          "%+d", "% d", "%lld", "%I64x", "%hd", "%hu", "%u", "%o", "%#o",
                          "%.0d", "%*d", "%-*d", "%.*s", "%f", "%.2f", "%10.3f", "%-12.1f|",
                          "%e", "%g", "%Z", "%wZ", "%S", "%ls", "%hs", "%k", "%s%k", "%5k",
                          "%I", "%Id", "%I32d", "%I6", "%#.0o", "%+.0d", "abc", "", "%s\\%s",
                          "%d%%%d", "%-+05d", "%0-5d", "%#08x", "%.10x", "%3c|", "%-3c|"};
  for (const char* f : kBuilt) {
    for (int i = 0; i < 30; i++) {
      FormatCase c;
      c.format = f;
      for (const char* p = f; *p; p++) {
        if (*p != '%') continue;
        p++;
        while (*p && std::string_view("-+ #0123456789.").find(*p) != std::string_view::npos) p++;
        if (*p == '*') {
          c.slots.push_back(uint64_t(int64_t(int32_t(h.random.Below(40)) - 20)));
          p++;
          while (*p && std::string_view("0123456789.").find(*p) != std::string_view::npos) p++;
        }
        std::string_view rest(p);
        const bool wide64 = rest.starts_with("ll") || rest.starts_with("I64");
        const bool wide = rest.starts_with("l") || rest.starts_with("w") || rest.starts_with("S");
        while (*p && std::string_view("hlwI0123456").find(*p) != std::string_view::npos) p++;
        switch (*p) {
          case 'd': case 'i': case 'u': case 'o': case 'x': case 'X': case 'p': case 'c':
            AddInt(h.random, c.slots, wide64);
            break;
          case 's': case 'S': c.slots.push_back(StringArg(h.random, d, wide)); break;
          case 'Z': c.slots.push_back(d + kCounted + 8 * h.random.Below(8)); break;
          case 'f': case 'e': case 'g': c.slots.push_back(DoubleBits(h.random)); break;
          default: break;
        }
        if (*p == 0) break;
      }
      // A spare argument or two (a bare "I" and the like take none).
      AddInt(h.random, c.slots, false);
      SprintfCase(h, results, block, c, 0, Where::kBlock,
                  fmt::format("\"{}\" case {}", Printable(c.format), i));
    }
  }
  // The output over the format's start (behind what's still to be read, so
  // the original ends: one digit written for every two characters read) and
  // over a string being copied (its length taken first, so the copy ends).
  for (int i = 0; i < 20; i++) {
    FormatCase c;
    c.format = "%d%d%d%d";
    for (int k = 0; k < 4; k++) c.slots.push_back(h.random.Below(10));
    SprintfCase(h, results, block, c, 0, Where::kOverFormat,
                fmt::format("over its format case {}", i));
  }
  for (int i = 0; i < 20; i++) {
    FormatCase c;
    c.format = i % 2 ? "ab%s%dcd" : "%s-%s";
    c.slots = {d + kStrings + 64 * 2, uint64_t(i), d + kStrings + 64 * 4};
    if (i % 2 == 0) c.slots[1] = d + kStrings + 64 * 4;
    SprintfCase(h, results, block, c, 0, Where::kOverString,
                fmt::format("over a string case {}", i));
  }
  for (int i = 0; i < 3000; i++) {
    const FormatCase c = MakeFormat(h.random, d);
    SprintfCase(h, results, block, c, 0, Where::kBlock,
                fmt::format("\"{}\" case {}", Printable(c.format), i));
  }
  results.Report();
}

void VsnprintfCases(Context& h) {
  Results results("sub_825AB010 _vsnprintf");
  const Block block(16384);
  for (int i = 0; i < 2000; i++) {
    const FormatCase c = MakeFormat(h.random, h.data);
    uint32_t size;
    bool null_buffer = false;
    switch (h.random.Below(6)) {
      case 0: size = 0; null_buffer = h.random.Below(2) == 0; break;  // only counted
      case 1: size = 1 + h.random.Below(8); break;                    // likely too small
      case 2: size = 0x80000000u + h.random.Below(1000); break;       // above INT_MAX
      case 3: size = 0xFFFFFFFFu; break;
      default: size = 64 + h.random.Below(400); break;
    }
    VsnprintfCase(h, results, block, c, size, null_buffer,
                  fmt::format("\"{}\" size {} {}", Printable(c.format), size,
                              null_buffer ? "no buffer" : "buffer"));
  }
  // Sizes around the output's length: exact, one short, one over.
  for (int i = 0; i < 200; i++) {
    FormatCase c;
    c.format = "%s:%05d";
    c.slots = {StringArg(h.random, h.data, false), uint64_t(h.random.Below(100000))};
    for (uint32_t size = 4; size < 40; size += 1 + h.random.Below(6)) {
      VsnprintfCase(h, results, block, c, size, false,
                    fmt::format("\"%s:%05d\" case {} size {}", i, size));
    }
  }
  results.Report();
}

}  // namespace

void CRuntimeFormat(Context& h) {
  SprintfCases(h);
  VsnprintfCases(h);
}

}  // namespace harness
