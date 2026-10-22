// The C runtime's helpers (src/hooks/c_runtime.cpp) against the recompiled
// ones.

#include "harness.h"

#include "base/log.h"
#include "c_runtime/format.h"
#include "c_runtime/parse.h"
#include "c_runtime/strings.h"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <tuple>

namespace harness {

namespace {

// Appends a register's 64 bits to a result.
void Append(std::vector<uint8_t>& out, uint64_t value) {
  for (int i = 0; i < 8; i++) {
    out.push_back(uint8_t(value >> (56 - 8 * i)));
  }
}

// Runs one case of a string helper through both versions: the scratch area
// filled with 0xCD, `setup()` writing the strings, `args(t)` the registers;
// the whole scratch area and r3's 64 bits compared.
template <typename Setup, typename Args>
void StringCase(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
                GuestFunction native, const std::string& what) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    result[v] = Read(h.base, h.data, kScratchBytes);
    Append(result[v], t.r3.u64);
  }
  results.Check(result[0], result[1], what);
}

// Where strings go in the scratch area: offsets from its start, some odd,
// and two that put a string across a page end (the 4 KB boundary inside
// the scratch area, or its middle when the boundary is too near an end).
struct Places {
  uint32_t page_end;  // an offset into the scratch area
  std::vector<uint32_t> offsets;
};

Places MakePlaces(const Context& h) {
  Places p;
  p.page_end = ((h.data + 4095) & ~4095u) - h.data;
  if (p.page_end < 1024 || p.page_end > kScratchBytes - 1024) {
    p.page_end = kScratchBytes / 2;
  }
  p.offsets = {64, 65, 66, 67, p.page_end - 3, p.page_end - 8};
  return p;
}

// Random string contents: bytes or UTF-16 units that aren't 0, many with
// the high bit set.
std::vector<uint8_t> RandomBytes(Context& h, uint32_t length) {
  std::vector<uint8_t> v(length);
  for (uint8_t& b : v) {
    b = uint8_t(1 + h.random.Below(255));
  }
  return v;
}

std::vector<uint16_t> RandomUnits(Context& h, uint32_t length) {
  static const uint16_t kEdges[] = {0xFFFF, 0x8000, 0x00FF, 0xFF00, 0x0001, 0x7FFF, 0xD800};
  std::vector<uint16_t> v(length);
  for (uint16_t& u : v) {
    const uint32_t r = h.random.Below(4);
    u = r == 0 ? kEdges[h.random.Below(7)] : uint16_t(1 + h.random.Below(0xFFFF));
  }
  return v;
}

// Writes a terminated string at an address (big-endian units).
void WriteUnits(Context& h, uint32_t address, const std::vector<uint16_t>& units) {
  for (size_t i = 0; i <= units.size(); i++) {
    guest::StoreU16(h.base, address + 2 * uint32_t(i), i < units.size() ? units[i] : 0);
  }
}

void WriteBytes(Context& h, uint32_t address, const std::vector<uint8_t>& bytes, bool terminate = true) {
  Write(h.base, address, bytes.data(), uint32_t(bytes.size()));
  if (terminate) {
    guest::StoreU8(h.base, address + uint32_t(bytes.size()), 0);
  }
}

// Bytes from a small alphabet (so strings match and nearly match): letters
// of both cases, the characters beside A-Z and a-z, high bytes.
std::vector<uint8_t> AlphabetBytes(Context& h, uint32_t length) {
  static const uint8_t kAlphabet[] = {'a', 'A', 'b', 'B', 'z', 'Z', '@', '[', '`', '{',
                                      '0', 0x80, 0xC1, 0xE1, 0xFF};
  std::vector<uint8_t> v(length);
  for (uint8_t& b : v) {
    b = kAlphabet[h.random.Below(sizeof(kAlphabet))];
  }
  return v;
}

// The same string with letters' case flipped at random.
std::vector<uint8_t> FlipCase(Context& h, std::vector<uint8_t> v) {
  for (uint8_t& b : v) {
    if (((b >= 'a' && b <= 'z') || (b >= 'A' && b <= 'Z')) && h.random.Below(2)) {
      b ^= 0x20;
    }
  }
  return v;
}

// Runs one case of a start-up piece through both versions: the guest ranges
// it writes (live globals) saved, `setup()` run, the function called, the
// ranges and r3 kept (unless `r3` is false), and the ranges put back; then
// compared.
template <typename Setup, typename Args>
void GlobalsCase(Context& h, Results& results,
                 const std::vector<std::pair<uint32_t, uint32_t>>& ranges, Setup setup, Args args,
                 GuestFunction recompiled, GuestFunction native, const std::string& what,
                 bool r3 = true) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    std::vector<std::vector<uint8_t>> saved;
    for (const auto& [address, bytes] : ranges) {
      saved.push_back(Read(h.base, address, bytes));
    }
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    for (size_t i = 0; i < ranges.size(); i++) {
      const auto bytes = Read(h.base, ranges[i].first, ranges[i].second);
      result[v].insert(result[v].end(), bytes.begin(), bytes.end());
      Write(h.base, ranges[i].first, saved[i].data(), ranges[i].second);
    }
    const auto scratch = Read(h.base, h.data, kScratchBytes);
    result[v].insert(result[v].end(), scratch.begin(), scratch.end());
    if (r3) {
      Append(result[v], t.r3.u64);
    }
  }
  results.Check(result[0], result[1], what);
}

const uint32_t kLengths[] = {0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 17, 31, 64, 200, 450};

// The register steps of the functions whose adapters went once their
// callers were native (docs/loops/b7-loop.md, "Removing an adapter"): the
// cases run the plain C++ through these, against the recompiled versions;
// where the plain C++ refuses (the error paths the adapters left to the
// original), the original runs, as the adapters did.

// strcpy_s: r3 = destination, r4 = its size, r5 = source; returns 0.
void NativeStrcpyS(PPCContext& ctx, uint8_t* base) {
  if (!c_runtime::StrcpyS(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_825A9DD8(ctx, base);
    return;
  }
  ctx.r3.u64 = 0;
}

// __ascii_strnicmp: r3, r4 = strings, r5 = count.
void NativeAsciiStrnicmp(PPCContext& ctx, uint8_t* base) {
  ctx.r3.s64 = c_runtime::AsciiStrnicmp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// strtoxl: r4 = string, r5 = end pointer out, r6 = base, r7 = flags.
void NativeStrToXl(PPCContext& ctx, uint8_t* base) {
  uint32_t end = 0;
  uint64_t result = 0;
  if (!c_runtime::StrToXl(base, ctx.r4.u32, ctx.r6.s32, ctx.r7.u32, ctx.r5.u32 != 0, &end,
                          &result)) {
    __imp__sub_825A82C8(ctx, base);
    return;
  }
  if (ctx.r5.u32 != 0) {
    guest::StoreU32(base, ctx.r5.u32, end);
  }
  ctx.r3.u64 = result;
}

// ___dtold: r3 = the 12-byte long double out, r4 = the double's address.
void NativeDtold(PPCContext& ctx, uint8_t* base) {
  const uint64_t bits = (uint64_t(guest::LoadU32(base, ctx.r4.u32)) << 32) |
                        guest::LoadU32(base, ctx.r4.u32 + 4);
  const c_runtime::LongDouble12 value = c_runtime::DoubleToLd12(bits);
  const uint32_t out = ctx.r3.u32;
  guest::StoreU16(base, out, value.sign_exponent);
  guest::StoreU32(base, out + 2, value.mantissa_high);
  guest::StoreU32(base, out + 6, value.mantissa_low);
}

// _ld12tod: r3 = the 12-byte long double, r4 = the double out; returns the
// status.
void NativeLd12tod(PPCContext& ctx, uint8_t* base) {
  const uint32_t in = ctx.r3.u32;
  const c_runtime::LongDouble12 value = {guest::LoadU16(base, in), guest::LoadU32(base, in + 2),
                                         guest::LoadU32(base, in + 6)};
  int32_t status;
  const uint64_t bits = c_runtime::Ld12ToDouble(value, guest::LoadU16(base, in + 10), &status);
  guest::StoreU32(base, ctx.r4.u32 + 4, uint32_t(bits));
  guest::StoreU32(base, ctx.r4.u32, uint32_t(bits >> 32));
  ctx.r3.s64 = status;
}

// __mtold12: r3 = the digits, r4 = their count, r5 = the 12-byte long
// double out.
void NativeMtold12(PPCContext& ctx, uint8_t* base) {
  const uint32_t count = ctx.r4.u32;
  c_runtime::LongDouble12Full value;
  uint8_t digits[4096];
  bool native = count <= sizeof(digits);
  if (native) {
    for (uint32_t i = 0; i < count; i++) {
      digits[i] = guest::LoadU8(base, ctx.r3.u32 + i);
    }
    native = c_runtime::Mtold12(digits, count, &value);
  }
  if (!native) {
    __imp__sub_825B7F10(ctx, base);
    return;
  }
  const uint32_t out = ctx.r5.u32;
  guest::StoreU16(base, out, value.value.sign_exponent);
  guest::StoreU32(base, out + 2, value.value.mantissa_high);
  guest::StoreU32(base, out + 6, value.value.mantissa_low);
  guest::StoreU16(base, out + 10, value.extra);
}

// __strgtold12_l: r3 = the 12-byte long double out, r4 = the end pointer
// out, r5 = the string, r6 to r9 = mult12, scale, decpt, implicit_E, r10 =
// the locale; returns the flags.
void NativeStrgtold12(PPCContext& ctx, uint8_t* base) {
  c_runtime::Strgtold12Result result;
  if (ctx.r10.u32 == 0 || !c_runtime::Strgtold12(base, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                                                 ctx.r8.u32, ctx.r9.u32, &result)) {
    __imp__sub_825B36B8(ctx, base);
    return;
  }
  guest::StoreU32(base, ctx.r4.u32, result.end);
  const uint32_t out = ctx.r3.u32;
  guest::StoreU16(base, out + 10, result.value.extra);
  guest::StoreU32(base, out + 6, result.value.value.mantissa_low);
  guest::StoreU32(base, out + 2, result.value.value.mantissa_high);
  guest::StoreU16(base, out, result.value.value.sign_exponent);
  ctx.r3.u64 = result.flags;
}

// _fltin2: r3 = the result, r4 = the string, r8 = the locale; returns r3.
void NativeFltin2(PPCContext& ctx, uint8_t* base) {
  c_runtime::FltinResult result;
  if (ctx.r8.u32 == 0 || !c_runtime::Fltin2(base, ctx.r4.u32, &result)) {
    __imp__sub_825AFDC8(ctx, base);
    return;
  }
  const uint32_t out = ctx.r3.u32;
  guest::StoreU32(base, out, result.flags);
  guest::StoreU32(base, out + 16, uint32_t(result.bits >> 32));
  guest::StoreU32(base, out + 20, uint32_t(result.bits));
  guest::StoreU32(base, out + 4, result.used);
}

// _atof_l: r3 = the string; the double in f1.
void NativeAtofL(PPCContext& ctx, uint8_t* base) {
  uint64_t bits;
  if (!c_runtime::AtofL(base, ctx.r3.u32, &bits)) {
    __imp__sub_825AB0C0(ctx, base);
    return;
  }
  ctx.fpscr.disableFlushMode();
  ctx.f1.u64 = bits;
}

}  // namespace

void CRuntime(Context& h) {
  const Places places = MakePlaces(h);
  // The stack probe: frame sizes below, at and above whole pages (the
  // callers' are 4 KB to 64 KB). It only reads, so the scratch area and the
  // registers the callers use after it (r1, r12) and r3 must be the same.
  {
    Results results("__chkstk (sub_825A9934)");
    std::vector<uint32_t> sizes = {0,     1,     16,    4095,  4096,  4097,  8191,
                                   8192,  8193,  12288, 16384, 20000, 32768, 65536};
    for (int i = 0; i < 20; i++) {
      sizes.push_back(h.random.Below(65536));
    }
    for (uint32_t size : sizes) {
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        t.r12.s64 = -int64_t(size);
        t.r3.u64 = 0x12345678;
        (v == 0 ? __imp__sub_825A9934 : sub_825A9934)(t, h.base);
        result[v] = Read(h.base, h.data, kScratchBytes);
        Append(result[v], t.r1.u64);
        Append(result[v], t.r12.u64);
        Append(result[v], t.r3.u64);
      }
      results.Check(result[0], result[1], fmt::format("size {}", size));
    }
    results.Report();
  }

  // memset: every alignment of the destination, counts from none to a few
  // hundred bytes (the word loop's 16-byte steps and the rest), values with
  // bits above the low byte; the scratch area around it and r3 compared.
  {
    Results results("memset (sub_825A71D0)");
    std::vector<uint32_t> counts;
    for (uint32_t c = 0; c <= 40; c++) {
      counts.push_back(c);
    }
    for (uint32_t c : {63u, 64u, 65u, 255u, 256u, 257u, 1000u, 2048u, 3000u}) {
      counts.push_back(c);
    }
    for (int i = 0; i < 40; i++) {
      counts.push_back(h.random.Below(3500));
    }
    const uint64_t values[] = {0, 0xFF, 0x5A, 0x12345678ABull, 0xFFFFFFFFFFFFFF00ull};
    for (uint32_t count : counts) {
      for (uint32_t offset = 0; offset < 8; offset++) {
        const uint64_t value = values[(count + offset) % 5];
        Both(h, results, [] {},
             [&](PPCContext& t) {
               t.r3.u64 = h.data + 64 + offset;
               t.r4.u64 = value;
               t.r5.u64 = count;
             },
             __imp__sub_825A71D0, sub_825A71D0, h.data, kScratchBytes,
             fmt::format("count {} offset {} value {:X}", count, offset, value));
      }
    }
    results.Report();
  }

  // The wide string helpers: every length in kLengths at each place (odd
  // addresses and across a page end included), units with high bits; the
  // destination apart from the source, or overlapping below it.
  {
    Results lengths("wcslen (sub_825A9958)");
    Results copies("wcscpy (sub_825A7F30)");
    Results joins("wcscat (sub_825A7EF8)");
    for (uint32_t length : kLengths) {
      for (uint32_t offset : places.offsets) {
        const std::vector<uint16_t> units = RandomUnits(h, length);
        const uint32_t at = h.data + offset;
        const std::string what = fmt::format("length {} offset {}", length, offset);
        StringCase(h, lengths, [&] { WriteUnits(h, at, units); },
                   [&](PPCContext& t) { t.r3.u64 = at; }, __imp__sub_825A9958, sub_825A9958,
                   what);
        // The destination elsewhere (the source near the page end goes to the
        // start, and the other way), then overlapping, one unit below.
        const uint32_t destination = h.data + (offset < 1024 ? 2601 : 1);
        StringCase(h, copies, [&] { WriteUnits(h, at, units); },
                   [&](PPCContext& t) {
                     t.r3.u64 = destination;
                     t.r4.u64 = at;
                   },
                   __imp__sub_825A7F30, sub_825A7F30, what);
        StringCase(h, copies, [&] { WriteUnits(h, at, units); },
                   [&](PPCContext& t) {
                     t.r3.u64 = at - 2;
                     t.r4.u64 = at;
                   },
                   __imp__sub_825A7F30, sub_825A7F30, what + " overlapping");
        for (uint32_t prefix : {0u, 1u, 9u, 120u}) {
          const std::vector<uint16_t> start = RandomUnits(h, prefix);
          StringCase(h, joins,
                     [&] {
                       WriteUnits(h, at, units);
                       WriteUnits(h, destination, start);
                     },
                     [&](PPCContext& t) {
                       t.r3.u64 = destination;
                       t.r4.u64 = at;
                     },
                     __imp__sub_825A7EF8, sub_825A7EF8, what + fmt::format(" prefix {}", prefix));
        }
      }
    }
    lengths.Report();
    copies.Report();
    joins.Report();

    // wcscmp: equal strings, one a prefix of the other, and a difference at
    // each position either way (high units, where signed and unsigned
    // differ).
    Results compares("wcscmp (sub_825A7EB0)");
    for (uint32_t length : kLengths) {
      for (uint32_t offset : {65u, places.page_end - 3}) {
        const std::vector<uint16_t> a = RandomUnits(h, length);
        std::vector<std::vector<uint16_t>> others = {a};
        if (length > 0) {
          others.push_back(std::vector<uint16_t>(a.begin(), a.end() - 1));
          for (int k = 0; k < 4; k++) {
            std::vector<uint16_t> b = a;
            const uint32_t at = h.random.Below(length);
            b[at] = RandomUnits(h, 1)[0];
            others.push_back(b);
          }
        }
        std::vector<uint16_t> longer = a;
        longer.push_back(0x8001);
        others.push_back(longer);
        for (size_t o = 0; o < others.size(); o++) {
          for (int order = 0; order < 2; order++) {
            const uint32_t first = h.data + offset;
            const uint32_t second = h.data + (offset < 1024 ? 2600 : 1);
            StringCase(h, compares,
                       [&] {
                         WriteUnits(h, first, order == 0 ? a : others[o]);
                         WriteUnits(h, second, order == 0 ? others[o] : a);
                       },
                       [&](PPCContext& t) {
                         t.r3.u64 = first;
                         t.r4.u64 = second;
                       },
                       __imp__sub_825A7EB0, sub_825A7EB0,
                       fmt::format("length {} offset {} other {} order {}", length, offset, o,
                                   order));
          }
        }
      }
    }
    compares.Report();

    // mbtowc: no destination or bytes, counts 0, 1 and 5, an empty string,
    // ASCII and high bytes, odd addresses.
    Results wides("mbtowc (sub_825B7EC8)");
    for (uint32_t wide : {0u, h.data + 64, h.data + 65}) {
      for (uint32_t bytes : {0u, h.data + 200, h.data + 201}) {
        for (uint32_t count : {0u, 1u, 5u, 0xFFFFFFFFu}) {
          for (uint8_t byte : {uint8_t(0), uint8_t('A'), uint8_t(0x80), uint8_t(0xFF)}) {
            StringCase(h, wides, [&] { guest::StoreU8(h.base, h.data + 200, byte); guest::StoreU8(h.base, h.data + 201, byte); },
                       [&](PPCContext& t) {
                         t.r3.u64 = wide;
                         t.r4.u64 = bytes;
                         t.r5.u64 = count;
                       },
                       __imp__sub_825B7EC8, sub_825B7EC8,
                       fmt::format("wide {:X} bytes {:X} count {} byte {:X}", wide, bytes, count,
                                   byte));
          }
        }
      }
    }
    wides.Report();
  }

  // strncpy and strcpy_s: every length at each place, counts and sizes
  // around the length; the destination apart, or overlapping below the
  // source. (strcpy_s's error paths, which call _errno and
  // _invalid_parameter, aren't run: both versions take the recompiled path
  // there.)
  {
    Results copies("strncpy (sub_825A7F50)");
    Results safe("strcpy_s (sub_825A9DD8)");
    for (uint32_t length : kLengths) {
      for (uint32_t offset : places.offsets) {
        const std::vector<uint8_t> bytes = RandomBytes(h, length);
        const uint32_t at = h.data + offset;
        const uint32_t destination = h.data + (offset < 1024 ? 2601 : 1);
        for (uint32_t to : {destination, at - 1, at - 5}) {
          for (uint32_t count : {0u, 1u, length, length + 1, length + 2, length + 37,
                                 length > 0 ? length - 1 : 3}) {
            const std::string what =
                fmt::format("length {} offset {} to {:X} count {}", length, offset, to - h.data, count);
            StringCase(h, copies, [&] { WriteBytes(h, at, bytes); },
                       [&](PPCContext& t) {
                         t.r3.u64 = to;
                         t.r4.u64 = at;
                         t.r5.u64 = count;
                       },
                       __imp__sub_825A7F50, sub_825A7F50, what);
            if (count > length) {
              StringCase(h, safe, [&] { WriteBytes(h, at, bytes); },
                         [&](PPCContext& t) {
                           t.r3.u64 = to;
                           t.r4.u64 = count;
                           t.r5.u64 = at;
                         },
                         __imp__sub_825A9DD8, NativeStrcpyS, what);
            }
          }
        }
      }
    }
    copies.Report();
    safe.Report();
  }

  // The compares: pairs of strings from a small alphabet (equal, the case
  // flipped, a prefix, a byte changed at random positions), each way round,
  // counts around where they differ; strncmp, _stricmp, _strnicmp and its
  // compare.
  {
    Results ncmp("strncmp (sub_825AACD0)");
    Results icmp("_stricmp (sub_825AA100)");
    Results nicmp("_strnicmp (sub_825AC710)");
    Results ascii("__ascii_strnicmp (sub_825AC6B0)");
    for (uint32_t length : kLengths) {
      for (uint32_t offset : {65u, places.page_end - 3}) {
        const std::vector<uint8_t> a = AlphabetBytes(h, length);
        std::vector<std::vector<uint8_t>> others = {a, FlipCase(h, a), FlipCase(h, a)};
        if (length > 0) {
          others.push_back(std::vector<uint8_t>(a.begin(), a.end() - 1));
          for (int k = 0; k < 4; k++) {
            std::vector<uint8_t> b = FlipCase(h, a);
            b[h.random.Below(length)] = AlphabetBytes(h, 1)[0];
            others.push_back(b);
          }
        }
        std::vector<uint8_t> longer = a;
        longer.push_back('Q');
        others.push_back(longer);
        for (size_t o = 0; o < others.size(); o++) {
          for (int order = 0; order < 2; order++) {
            const uint32_t first = h.data + offset;
            const uint32_t second = h.data + (offset < 1024 ? 2600 : 1);
            auto setup = [&] {
              WriteBytes(h, first, order == 0 ? a : others[o]);
              WriteBytes(h, second, order == 0 ? others[o] : a);
            };
            const std::string what =
                fmt::format("length {} offset {} other {} order {}", length, offset, o, order);
            StringCase(h, icmp, setup,
                       [&](PPCContext& t) {
                         t.r3.u64 = first;
                         t.r4.u64 = second;
                       },
                       __imp__sub_825AA100, sub_825AA100, what);
            for (uint64_t count : {uint64_t(0), uint64_t(1), uint64_t(length), uint64_t(length) + 1,
                                   uint64_t(length / 2 + 1), uint64_t(0x7FFFFFFF), uint64_t(0xFFFFFFFF),
                                   uint64_t(0x100000003)}) {
              auto args = [&](PPCContext& t) {
                t.r3.u64 = first;
                t.r4.u64 = second;
                t.r5.u64 = count;
              };
              const std::string with = what + fmt::format(" count {:X}", count);
              StringCase(h, ncmp, setup, args, __imp__sub_825AACD0, sub_825AACD0, with);
              StringCase(h, ascii, setup, args, __imp__sub_825AC6B0, NativeAsciiStrnicmp, with);
              if (count <= 0x7FFFFFFF) {  // above, EINVAL through _errno
                StringCase(h, nicmp, setup, args, __imp__sub_825AC710, sub_825AC710, with);
              }
            }
          }
        }
      }
    }
    ncmp.Report();
    icmp.Report();
    nicmp.Report();
    ascii.Report();
  }

  // strstr: needles taken from the string (at its start, middle and end),
  // changed in their last byte (a partial match), longer than the string,
  // empty; an empty string.
  {
    Results finds("strstr (sub_825A98A8)");
    for (uint32_t length : kLengths) {
      for (uint32_t offset : {65u, places.page_end - 3}) {
        const std::vector<uint8_t> text = AlphabetBytes(h, length);
        std::vector<std::vector<uint8_t>> needles = {{}, AlphabetBytes(h, 1), AlphabetBytes(h, 2),
                                                     AlphabetBytes(h, length + 1)};
        for (int k = 0; k < 6 && length > 0; k++) {
          const uint32_t from = h.random.Below(length);
          const uint32_t n = 1 + h.random.Below(std::min(length - from, 40u));
          std::vector<uint8_t> needle(text.begin() + from, text.begin() + from + n);
          needles.push_back(needle);
          needle.back() ^= 0x20;
          needles.push_back(needle);
        }
        if (length > 0) {
          needles.push_back(text);
          needles.push_back(std::vector<uint8_t>(text.end() - std::min(length, 5u), text.end()));
        }
        for (size_t n = 0; n < needles.size(); n++) {
          const uint32_t at = h.data + offset;
          const uint32_t needle_at = h.data + (offset < 1024 ? 2600 : 1);
          StringCase(h, finds,
                     [&] {
                       WriteBytes(h, at, text);
                       WriteBytes(h, needle_at, needles[n]);
                     },
                     [&](PPCContext& t) {
                       t.r3.u64 = at;
                       t.r4.u64 = needle_at;
                     },
                     __imp__sub_825A98A8, sub_825A98A8,
                     fmt::format("length {} offset {} needle {}", length, offset, n));
          // r3 with its high word set (as a sign-extended pointer has).
          StringCase(h, finds,
                     [&] {
                       WriteBytes(h, at, text);
                       WriteBytes(h, needle_at, needles[n]);
                     },
                     [&](PPCContext& t) {
                       t.r3.u64 = 0xFFFFFFFF00000000ull | at;
                       t.r4.u64 = needle_at;
                     },
                     __imp__sub_825A98A8, sub_825A98A8,
                     fmt::format("length {} offset {} needle {} high r3", length, offset, n));
        }
      }
    }
    finds.Report();
  }

  // The second memcpy: every alignment of destination and source, counts
  // from none to a few hundred bytes, overlapping either way by a few bytes,
  // and across a page end.
  {
    Results copies("the second memcpy (sub_825A7830)");
    std::vector<uint32_t> counts;
    for (uint32_t c = 0; c <= 24; c++) {
      counts.push_back(c);
    }
    for (uint32_t c : {63u, 64u, 65u, 255u, 700u}) {
      counts.push_back(c);
    }
    for (uint32_t count : counts) {
      for (uint32_t d = 0; d < 4; d++) {
        for (uint32_t sa = 0; sa < 4; sa++) {
          for (int32_t shift : {2000, -1, -3, -4, -6, 1, 3, 4, 7}) {
            const uint32_t source = h.data + 1000 + sa;
            const uint32_t destination = uint32_t(int32_t(h.data + 1000 + d) + shift);
            const std::vector<uint8_t> bytes = RandomBytes(h, count + 8);
            StringCase(h, copies, [&] { WriteBytes(h, source - 4, bytes, false); },
                       [&](PPCContext& t) {
                         t.r3.u64 = destination;
                         t.r4.u64 = source;
                         t.r5.u64 = count;
                       },
                       __imp__sub_825A7830, sub_825A7830,
                       fmt::format("count {} destination {} source {} shift {}", count, d, sa,
                                   shift));
          }
        }
      }
      const uint32_t across = h.data + places.page_end - 2 - (count & 7);
      const std::vector<uint8_t> bytes = RandomBytes(h, count);
      StringCase(h, copies, [&] { WriteBytes(h, h.data + 64, bytes, false); },
                 [&](PPCContext& t) {
                   t.r3.u64 = across;
                   t.r4.u64 = h.data + 65;
                   t.r5.u64 = count;
                 },
                 __imp__sub_825A7830, sub_825A7830, fmt::format("count {} across a page end", count));
    }
    copies.Report();
  }

  // The character helpers: every character from -2 to 300 (EOF and the
  // halfwords before the table included), registers with high words that
  // aren't the low word's sign extension (their low words within the table).
  {
    std::vector<uint64_t> characters;
    for (int64_t c = -2; c <= 300; c++) {
      characters.push_back(uint64_t(c));
    }
    for (uint64_t c : {0x0000000100000061ull, 0xFFFFFFFF00000041ull, 0x00000000FFFFFFFFull,
                       0x7FFFFFFF0000007Aull, 0x8000000000000020ull}) {
      characters.push_back(c);
    }
    struct Function {
      const char* name;
      GuestFunction recompiled, native;
    };
    const Function functions[] = {
        {"toupper (sub_825A8600)", __imp__sub_825A8600, sub_825A8600},
        {"tolower (sub_825A8618)", __imp__sub_825A8618, sub_825A8618},
        {"isalpha (sub_825AB020)", __imp__sub_825AB020, sub_825AB020},
        {"isdigit (sub_825AB040)", __imp__sub_825AB040, sub_825AB040},
        {"isalnum (sub_825AB0A0)", __imp__sub_825AB0A0, sub_825AB0A0},
        {"isleadbyte (sub_825B6898)", __imp__sub_825B6898, sub_825B6898},
    };
    for (const Function& f : functions) {
      Results results(f.name);
      for (uint64_t c : characters) {
        StringCase(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = c; }, f.recompiled,
                   f.native, fmt::format("c {:X}", c));
      }
      results.Report();
    }
  }

  // strtoul and strtoxl: spaces, a sign, a 0x prefix, digits of each base
  // (both cases) and something after them; bases 0, 2, 8, 10, 16 and 36;
  // with and without an end pointer; numbers at the edges of the range.
  // Numbers out of range (ERANGE through _errno, which sets the thread's
  // errno) aren't run.
  {
    Results unsigned_results("strtoul (sub_825A85E0)");
    Results signed_results("strtoxl, signed (sub_825A82C8)");
    struct Text {
      std::string text;
      int32_t radix;
      uint64_t value;  // the digits' value, for leaving out what's out of range
      bool negative;
    };
    std::vector<Text> texts = {
        {"", 10, 0, false},          {"   ", 0, 0, false},       {"-", 10, 0, false},
        {"+", 0, 0, false},          {"0x", 16, 0, false},       {"0x", 0, 0, false},
        {"0xg", 16, 0, false},       {"0", 0, 0, false},         {"08", 0, 0, false},
        {"077", 0, 63, false},       {"z", 36, 35, false},       {"Zz", 36, 1295, false},
        {"z", 10, 0, false},         {"4294967295", 10, 4294967295, false},
        {"-4294967295", 10, 4294967295, true}, {"2147483647", 10, 2147483647, false},
        {"-2147483648", 10, 2147483648, true},  {"0xFFFFFFFF", 0, 4294967295, false},
        {"\t\n\v\f\r 12", 10, 12, false},  {"1z", 36, 71, false}, {"12a", 10, 12, false},
        {"\x80" "5", 10, 0, false},
    };
    const int32_t radices[] = {0, 2, 8, 10, 16, 36};
    const char* kDigits = "0123456789abcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < 300; i++) {
      Text t;
      t.radix = radices[h.random.Below(6)];
      const uint32_t digit_base = t.radix == 0 ? (i % 3 == 0 ? 16 : i % 3 == 1 ? 8 : 10) : uint32_t(t.radix);
      for (uint32_t n = h.random.Below(3); n > 0; n--) {
        t.text += " \t\n"[h.random.Below(3)];
      }
      t.negative = h.random.Below(3) == 0;
      if (t.negative) {
        t.text += '-';
      } else if (h.random.Below(3) == 0) {
        t.text += '+';
      }
      if (digit_base == 16 && (t.radix == 0 || h.random.Below(2))) {
        t.text += h.random.Below(2) ? "0x" : "0X";
      } else if (t.radix == 0 && digit_base == 8) {
        t.text += '0';
      }
      t.value = 0;
      const uint32_t count = 1 + h.random.Below(12);
      for (uint32_t n = 0; n < count; n++) {
        uint32_t d = h.random.Below(digit_base);
        if (n == 0 && t.radix == 0 && digit_base == 10 && d == 0) {
          d = 1;  // a leading 0 would make it octal
        }
        char c = kDigits[d];
        if (c >= 'a' && h.random.Below(2)) {
          c = char(c - 32);
        }
        t.text += c;
        t.value = std::min<uint64_t>(t.value * digit_base + d, 1ull << 40);
      }
      t.text += "@ ?!/:"[h.random.Below(6)];  // never a digit
      texts.push_back(t);
    }
    for (const Text& text : texts) {
      for (uint32_t offset : {65u, places.page_end - 3}) {
        for (bool want_end : {true, false}) {
          const uint32_t at = h.data + offset;
          const uint32_t end_out = want_end ? h.data + 8 : 0;
          auto setup = [&] {
            WriteBytes(h, at, std::vector<uint8_t>(text.text.begin(), text.text.end()));
          };
          const std::string what = fmt::format("\"{}\" base {} offset {} end {}", text.text,
                                                text.radix, offset, want_end);
          if (text.value <= 0xFFFFFFFFull) {
            StringCase(h, unsigned_results, setup,
                       [&](PPCContext& t) {
                         t.r3.u64 = at;
                         t.r4.u64 = end_out;
                         t.r5.s64 = text.radix;
                       },
                       __imp__sub_825A85E0, sub_825A85E0, what);
          }
          if (text.value <= (text.negative ? 0x80000000ull : 0x7FFFFFFFull)) {
            StringCase(h, signed_results, setup,
                       [&](PPCContext& t) {
                         t.r3.u64 = 0x8266BC28;
                         t.r4.u64 = at;
                         t.r5.u64 = end_out;
                         t.r6.s64 = text.radix;
                         t.r7.u64 = 0;
                       },
                       __imp__sub_825A82C8, NativeStrToXl, what);
          }
        }
      }
    }
    unsigned_results.Report();
    signed_results.Report();
  }

  // ___dtold: zeros, infinities, NaNs, denormals of every length, normals
  // and random bits, out at an even and an odd address; the scratch area
  // and r3 compared.
  {
    Results results("___dtold (sub_825B6378)");
    std::vector<uint64_t> doubles = {0, 0x8000000000000000, 0x7FF0000000000000,
                                     0xFFF0000000000000, 0x7FF8000000000000, 0xFFF8000000000001,
                                     0x7FF0000000000001, 0x000FFFFFFFFFFFFF, 0x800FFFFFFFFFFFFF,
                                     0x0010000000000000, 0x7FEFFFFFFFFFFFFF, 0x3FF0000000000000,
                                     0xBFF8000000000000, 0x0000000080000000, 0x0000000100000000};
    for (int b = 0; b < 52; b++) {
      doubles.push_back(1ull << b);
      doubles.push_back((1ull << b) | 0x8000000000000000ull);
      doubles.push_back((1ull << b) | 0x00000000FFFFFFFFull);
    }
    for (int i = 0; i < 1500; i++) {
      const uint64_t r = h.random.Next();
      doubles.push_back(r);
      doubles.push_back(r & 0x800FFFFFFFFFFFFFull);
      doubles.push_back(r >> h.random.Below(64));
    }
    for (uint64_t bits : doubles) {
      for (uint32_t offset : {64u, 67u}) {
        StringCase(h, results, [&] { StoreU64(h.base, h.data + 512, bits); },
                   [&](PPCContext& t) {
                     t.r3.u64 = h.data + offset;
                     t.r4.u64 = h.data + 512;
                   },
                   __imp__sub_825B6378, NativeDtold,
                   fmt::format("{:016X} out +{}", bits, offset));
      }
    }
    results.Report();
  }

  // _ld12tod: exponents at and around a double's range (normal, denormal,
  // zero, overflow) and far outside it, 0 and 32767; mantissas with their
  // leading bit set or not, rounding ties (the bit after the 53rd set and
  // nothing after it, or something only in the extra 16 bits), carries out
  // of all ones, and random bits; both signs. The scratch area (the double
  // among 0xCD) and r3 compared.
  {
    Results results("_ld12tod (sub_825B69B8)");
    struct Ld12 {
      uint16_t sign_exponent;
      uint32_t high, low;
      uint16_t extra;
    };
    std::vector<Ld12> values;
    std::vector<int32_t> exponents = {0, 1, 2, 32767, 32766, 16383, 16382, 16384};
    for (int32_t e = -1090; e <= -1010; e++) {
      exponents.push_back(16383 + e);
    }
    for (int32_t e = 1015; e <= 1030; e++) {
      exponents.push_back(16383 + e);
    }
    for (int i = 0; i < 40; i++) {
      exponents.push_back(int32_t(h.random.Below(32768)));
    }
    auto mantissas = [&](std::vector<std::array<uint32_t, 3>>& m) {
      m = {{0, 0, 0},
           {0x80000000u, 0, 0},
           {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFF},
           {0xFFFFFFFFu, 0xFFFFF800u, 0},
           {0xFFFFFFFFu, 0xFFFFFC00u, 0},
           {0x80000000u, 0x00000400u, 0},
           {0x80000000u, 0x00000C00u, 0},
           {0x80000000u, 0x00000400u, 1},
           {0x80000000u, 0x00000401u, 0},
           {0x80000000u, 0x00000800u, 0},
           {0x40000000u, 0, 0},
           {0, 1, 0},
           {0, 0, 1},
           {0x12345678u, 0x9ABCDEF0u, 0x1234}};
      for (int i = 0; i < 6; i++) {
        const uint64_t r = h.random.Next();
        const uint32_t high = uint32_t(r >> 32) | (i < 4 ? 0x80000000u : 0);
        m.push_back({high, uint32_t(r), uint32_t(h.random.Below(0x10000))});
        m.push_back({high, (uint32_t(r) & ~0x7FFu) | 0x400u, 0});
        m.push_back({high, (uint32_t(r) & ~0x7FFu) | 0x400u, 0x8000});
      }
    };
    std::vector<std::array<uint32_t, 3>> m;
    mantissas(m);
    for (int32_t e : exponents) {
      for (const auto& man : m) {
        for (uint16_t sign : {uint16_t(0), uint16_t(0x8000)}) {
          values.push_back({uint16_t(sign | e), man[0], man[1], uint16_t(man[2])});
        }
      }
    }
    for (int i = 0; i < 3000; i++) {
      const uint64_t r = h.random.Next();
      values.push_back({uint16_t(r >> 48), uint32_t(h.random.Next()), uint32_t(r),
                        uint16_t(r >> 32)});
    }
    for (const Ld12& v : values) {
      StringCase(h, results,
                 [&] {
                   const uint32_t in = h.data + 512;
                   guest::StoreU16(h.base, in, v.sign_exponent);
                   guest::StoreU32(h.base, in + 2, v.high);
                   guest::StoreU32(h.base, in + 6, v.low);
                   guest::StoreU16(h.base, in + 10, v.extra);
                 },
                 [&](PPCContext& t) {
                   t.r3.u64 = h.data + 512;
                   t.r4.u64 = h.data + 64;
                 },
                 __imp__sub_825B69B8, NativeLd12tod,
                 fmt::format("{:04X} {:08X} {:08X} {:04X}", v.sign_exponent, v.high, v.low,
                             v.extra));
    }
    results.Report();
  }

  // write_char: string streams with and without flag 0x40 and a buffer,
  // room from 1 up (no room calls _flsbuf, which the native version leaves
  // to the original: those cases are left out), characters as whole
  // registers, the pointer at the stream's own words or at the count; the
  // scratch area and r3 compared.
  {
    Results results("write_char (sub_825AF000)");
    const uint32_t stream = h.data + 256;
    const uint32_t count = h.data + 300;
    const uint32_t buffer = h.data + 1024;
    const uint32_t flags[] = {0x40, 0x42, 0x49, 0, 2, 0xFFFFFFBF, 0xFFFFFFFF};
    const uint32_t bases[] = {0, buffer, 1};
    const uint32_t rooms[] = {0, 1, 2, 100, 0x7FFFFFFF, 0x80000000, 0xFFFFFFFF};
    const uint32_t pointers[] = {buffer, buffer + 3, stream, stream + 3, stream + 4, count + 3};
    const uint32_t counts[] = {0, 5, 0xFFFFFFFF, 0x7FFFFFFF};
    for (uint32_t flag : flags) {
      for (uint32_t base_word : bases) {
        for (uint32_t room : rooms) {
          const bool counts_only = (flag & 0x40) != 0 && base_word == 0;
          if (!counts_only && int32_t(room - 1) < 0) {
            continue;
          }
          for (uint32_t pointer : pointers) {
            for (uint32_t c : counts) {
              const uint64_t character = h.random.Next();
              StringCase(h, results,
                         [&] {
                           guest::StoreU32(h.base, stream, pointer);
                           guest::StoreU32(h.base, stream + 4, room);
                           guest::StoreU32(h.base, stream + 8, base_word);
                           guest::StoreU32(h.base, stream + 12, flag);
                           guest::StoreU32(h.base, count, c);
                         },
                         [&](PPCContext& t) {
                           t.r3.u64 = character;
                           t.r4.u64 = stream;
                           t.r5.u64 = count;
                         },
                         __imp__sub_825AF000, sub_825AF000,
                         fmt::format("flags {:X} base {:X} room {:X} pointer {:X} count {:X}",
                                     flag, base_word, room, pointer, c));
            }
          }
        }
      }
    }
    results.Report();
  }

  // write_multi_char (sub_825AF098): a stream (string or file flags, with
  // and without a buffer) with room for every character (no room calls
  // _flsbuf: left out), lengths from -3 to 40, the buffer apart, unaligned,
  // over the count (the original runs then) or over the string ahead of
  // what's read; counts near -1 (the _errno path, also the original's); the
  // scratch area and r3 compared.
  {
    Results results("write_multi_char (sub_825AF098)");
    const uint32_t stream = h.data + 256;
    const uint32_t count = h.data + 300;
    const uint32_t buffer = h.data + 1024;
    const uint32_t string = h.data + 2048;
    const uint32_t flags[] = {0x40, 0x49, 0};
    const uint32_t bases[] = {0, buffer};
    const uint32_t rooms[] = {1, 5, 30, 40, 100, 0x7FFFFFFF, 0x80000000};
    const int32_t lengths[] = {-3, 0, 1, 2, 7, 30, 40};
    const uint32_t pointers[] = {buffer, buffer + 3, count - 2, string + 5};
    const uint32_t counts[] = {0, 5, 0xFFFFFFF0, 0x7FFFFFFF};
    for (uint32_t flag : flags) {
      for (uint32_t base_word : bases) {
        const bool counts_only = (flag & 0x40) != 0 && base_word == 0;
        for (uint32_t room : rooms) {
          for (int32_t length : lengths) {
            bool fits = true;
            for (int32_t k = 0; k < length; k++) {
              fits = fits && int32_t(room - 1 - uint32_t(k)) >= 0;
            }
            if (!counts_only && !fits) {
              continue;
            }
            for (uint32_t pointer : pointers) {
              for (uint32_t c : counts) {
                const auto bytes = RandomBytes(h, 48);
                StringCase(h, results,
                           [&] {
                             WriteBytes(h, string, bytes);
                             guest::StoreU32(h.base, stream, pointer);
                             guest::StoreU32(h.base, stream + 4, room);
                             guest::StoreU32(h.base, stream + 8, base_word);
                             guest::StoreU32(h.base, stream + 12, flag);
                             guest::StoreU32(h.base, count, c);
                           },
                           [&](PPCContext& t) {
                             t.r3.u64 = string;
                             t.r4.u64 = uint64_t(int64_t(length));
                             t.r5.u64 = stream;
                             t.r6.u64 = count;
                           },
                           __imp__sub_825AF098, sub_825AF098,
                           fmt::format("flags {:X} base {:X} room {:X} length {} pointer +{} "
                                       "count {:X}",
                                       flag, base_word, room, length, pointer - h.data, c));
              }
            }
          }
        }
      }
    }
    results.Report();
  }

  // _putwc_nolock (sub_825B7A80) and the wide write_char (sub_825B1368):
  // string streams (flag 0x40; file streams and the console go through
  // _fileno and the console's output, left out) with room for two bytes or
  // more (no room calls _flswbuf: left out; with no buffer the wide
  // write_char only counts), the pointer apart, unaligned or on the
  // stream's own words or the count, units as whole registers (0xFFFF
  // included), counts; the scratch area and r3 compared.
  for (int wide_char = 0; wide_char < 2; wide_char++) {
    Results results(wide_char ? "the wide write_char (sub_825B1368)"
                              : "_putwc_nolock (sub_825B7A80)");
    const uint32_t stream = h.data + 256;
    const uint32_t count = h.data + 300;
    const uint32_t buffer = h.data + 1024;
    const uint32_t flags[] = {0x40, 0x49, 0xFFFFFFFF};
    const uint32_t bases[] = {0, buffer};
    const uint32_t rooms[] = {0, 1, 2, 3, 100, 0x7FFFFFFF, 0x80000000, 0x80000001};
    const uint32_t pointers[] = {buffer, buffer + 1, stream, stream + 2, stream + 4, count};
    const uint32_t counts[] = {0, 7, 0xFFFFFFFE};
    for (uint32_t flag : flags) {
      for (uint32_t base_word : bases) {
        const bool counts_only = wide_char && base_word == 0;
        for (uint32_t room : rooms) {
          if (!counts_only && int32_t(room - 2) < 0) {
            continue;
          }
          for (uint32_t pointer : pointers) {
            for (uint32_t c : counts) {
              for (int u = 0; u < 3; u++) {
                const uint64_t unit = u == 0 ? 0xFFFF : u == 1 ? 0x12345678FFFFull : h.random.Next();
                StringCase(h, results,
                           [&] {
                             guest::StoreU32(h.base, stream, pointer);
                             guest::StoreU32(h.base, stream + 4, room);
                             guest::StoreU32(h.base, stream + 8, base_word);
                             guest::StoreU32(h.base, stream + 12, flag);
                             guest::StoreU32(h.base, count, c);
                           },
                           [&](PPCContext& t) {
                             t.r3.u64 = unit;
                             t.r4.u64 = stream;
                             t.r5.u64 = count;
                           },
                           wide_char ? __imp__sub_825B1368 : __imp__sub_825B7A80,
                           wide_char ? sub_825B1368 : sub_825B7A80,
                           fmt::format("flags {:X} base {:X} room {:X} pointer +{} count {:X} "
                                       "unit {:X}",
                                       flag, base_word, room, pointer - h.data, c, unit));
              }
            }
          }
        }
      }
    }
    results.Report();
  }

  // The start-up's pieces, on the live globals (saved and put back around
  // each call): the float conversion table filled (from 0xCD, through
  // sub_825A6E48 too; r3 not compared: _cfltcvt_init leaves 0x825B0000 there,
  // which its caller, _cinit through sub_825A6E48, sets again before reading);
  // each setter with zero, a pointer and random words;
  // __initmbctable with its done flag clear (the info, from 0xCD, filled)
  // and set (nothing done); setSBCS on the scratch area at even and odd
  // addresses.
  {
    Results results("the C runtime's start-up (sub_825A6DB0 and the rest)");
    const std::vector<std::pair<uint32_t, uint32_t>> table = {{0x8266BA5C, 40}};
    for (auto [recompiled, native, name] :
         {std::tuple{__imp__sub_825A6DB0, sub_825A6DB0, "_cfltcvt_init"},
          std::tuple{__imp__sub_825A6E48, sub_825A6E48, "sub_825A6E48"}}) {
      GlobalsCase(h, results, table, [&] { Fill(h.base, 0x8266BA5C, 40); },
                  [](PPCContext&) {}, recompiled, native, name, false);
    }
    struct Setter {
      GuestFunction recompiled, native;
      uint32_t global;
      const char* name;
    };
    const Setter setters[] = {
        {__imp__sub_825AD140, sub_825AD140, 0x82971170, "sub_825AD140"},
        {__imp__sub_825AEE18, sub_825AEE18, 0x82971178, "sub_825AEE18"},
        {__imp__sub_825AFCB8, sub_825AFCB8, 0x82978320, "sub_825AFCB8"},
        {__imp__sub_825B3560, sub_825B3560, 0x8297132C, "sub_825B3560"},
        {__imp__sub_825B3548, sub_825B3548, 0x82971328, "sub_825B3548"},
    };
    for (const Setter& setter : setters) {
      for (uint64_t value : {uint64_t(0), uint64_t(0x825B3498), uint64_t(0xFFFFFFFF), h.random.Next(), h.random.Next()}) {
        GlobalsCase(h, results, {{setter.global - 8, 24}}, [] {},
                    [&](PPCContext& t) { t.r3.u64 = value; }, setter.recompiled, setter.native,
                    fmt::format("{} r3 {:X}", setter.name, value));
      }
    }
    const uint32_t info = guest::LoadU32(h.base, 0x8266C450);
    const std::vector<std::pair<uint32_t, uint32_t>> mbc = {
        {0x82978338, 4}, {0x8297135C, 4}, {0x8297136C, 4}, {info, 544}};
    for (uint32_t done : {0u, 1u}) {
      GlobalsCase(h, results, mbc,
                  [&] {
                    guest::StoreU32(h.base, 0x82978338, done);
                    guest::StoreU32(h.base, 0x8297135C, 0x12345678);
                    guest::StoreU32(h.base, 0x8297136C, 0x12345678);
                    Fill(h.base, info, 544);
                  },
                  [](PPCContext& t) { t.r3.u64 = 0x5555; }, __imp__sub_825B6958, sub_825B6958,
                  fmt::format("__initmbctable done {}", done));
    }
    for (uint32_t offset : {64u, 67u, 1024u}) {
      GlobalsCase(h, results, {}, [] {}, [&](PPCContext& t) { t.r3.u64 = h.data + offset; },
                  __imp__sub_825B68C8, sub_825B68C8, fmt::format("setSBCS +{}", offset));
    }
    results.Report();
  }

  // memcpy (sub_825A7410, its override with the native copy against the
  // original): every alignment of both ends (mod 8) for sizes around the
  // original's paths (bytes, words, doublewords, 128-byte blocks), random
  // alignments for the sizes between, a source and destination across a
  // page end, and overlaps either way by 1 to 17 bytes (left to the
  // original by the native copy); the scratch area and r3 compared.
  {
    Results results("memcpy (sub_825A7410)");
    auto copy_case = [&](uint32_t to, uint32_t from, uint32_t size) {
      const auto bytes = RandomBytes(h, size + 16);
      StringCase(h, results, [&] { Write(h.base, from - 8, bytes.data(), size + 16); },
                 [&](PPCContext& t) {
                   t.r3.u64 = to;
                   t.r4.u64 = from;
                   t.r5.u64 = size;
                 },
                 __imp__sub_825A7410, sub_825A7410,
                 fmt::format("to +{} from +{} size {}", to - h.data, from - h.data, size));
    };
    std::vector<uint32_t> sizes;
    for (uint32_t size = 0; size <= 300; size++) {
      sizes.push_back(size);
    }
    for (uint32_t size : {383u, 384u, 385u, 511u, 512u, 513u, 1000u, 1023u, 1024u, 1025u, 1500u}) {
      sizes.push_back(size);
    }
    for (uint32_t size : sizes) {
      const bool all = size <= 40 || (size >= 120 && size <= 140) || size >= 256;
      for (uint32_t a = 0; a < 8; a++) {
        for (uint32_t b = 0; b < 8; b++) {
          if (!all && h.random.Below(16) != 0) {
            continue;
          }
          copy_case(h.data + 2048 + a, h.data + 64 + b, size);
        }
      }
    }
    for (uint32_t size : {5u, 64u, 200u, 700u}) {
      copy_case(h.data + places.page_end - 3, h.data + 64 + 1, size);
      copy_case(h.data + 2048 + 4, h.data + places.page_end - 9, size);
    }
    for (uint32_t size : {1u, 7u, 8u, 9u, 16u, 64u, 127u, 128u, 129u, 300u}) {
      for (uint32_t k = 1; k <= 17; k++) {
        copy_case(h.data + 1024 + k, h.data + 1024, size);
        copy_case(h.data + 1024, h.data + 1024 + k, size);
      }
    }
    results.Report();
  }

  // memmove (sub_825AA840): equal, a destination below the source (memcpy's
  // override) and above it, apart and overlapping by 1 to 17 bytes, at every
  // alignment of both ends for small sizes, and across a page end; the
  // scratch area and r3 compared.
  {
    Results results("memmove (sub_825AA840)");
    auto move_case = [&](uint32_t to, uint32_t from, uint32_t size) {
      const auto bytes = RandomBytes(h, size + 16);
      StringCase(h, results, [&] { Write(h.base, from - 8, bytes.data(), size + 16); },
                 [&](PPCContext& t) {
                   t.r3.u64 = to;
                   t.r4.u64 = from;
                   t.r5.u64 = size;
                 },
                 __imp__sub_825AA840, sub_825AA840,
                 fmt::format("to +{} from +{} size {}", to - h.data, from - h.data, size));
    };
    for (uint32_t size : {0u, 1u, 2u, 3u, 4u, 5u, 7u, 8u, 9u, 13u, 16u, 31u, 64u, 100u, 257u}) {
      for (uint32_t a = 0; a < 4; a++) {
        for (uint32_t b = 0; b < 4; b++) {
          move_case(h.data + 2048 + a, h.data + 64 + b, size);
          move_case(h.data + 64 + a, h.data + 2048 + b, size);
        }
      }
      move_case(h.data + 1024, h.data + 1024, size);
      for (uint32_t k = 1; k <= 17; k++) {
        move_case(h.data + 1024 + k, h.data + 1024, size);
        move_case(h.data + 1024, h.data + 1024 + k, size);
      }
    }
    for (uint32_t size : {700u, 1500u}) {
      for (uint32_t k : {1u, 4u, 9u, 300u}) {
        move_case(h.data + 512 + k, h.data + 512, size);
        move_case(h.data + 512, h.data + 512 + k, size);
      }
    }
    for (uint32_t size : {5u, 64u, 200u}) {
      move_case(h.data + places.page_end - 3, h.data + 64 + 1, size);
      move_case(h.data + places.page_end - 3, h.data + places.page_end - 9, size);
    }
    results.Report();
  }

  // memcpy_s (sub_825A7270): counts of 0, below, at and (for count 0 only)
  // above the size, at several alignments; the error paths (_errno) left
  // out. The scratch area and r3 compared.
  {
    Results results("memcpy_s (sub_825A7270)");
    for (uint32_t count : {0u, 1u, 3u, 4u, 8u, 17u, 64u, 200u, 1000u}) {
      for (uint32_t extra : {0u, 1u, 50u}) {
        for (uint32_t a = 0; a < 4; a++) {
          const uint32_t to = h.data + 2048 + a, from = h.data + 64 + 3 - a;
          const uint32_t size = count + extra;
          const auto bytes = RandomBytes(h, count + 16);
          StringCase(h, results, [&] { Write(h.base, from - 8, bytes.data(), count + 16); },
                     [&](PPCContext& t) {
                       t.r3.u64 = to;
                       t.r4.u64 = size;
                       t.r5.u64 = from;
                       t.r6.u64 = count;
                     },
                     __imp__sub_825A7270, sub_825A7270,
                     fmt::format("to +{} size {} from +{} count {}", to - h.data, size,
                                 from - h.data, count));
        }
      }
    }
    // A count of 0 returns 0 before anything is checked, even with no buffers.
    StringCase(h, results, [] {}, [&](PPCContext& t) {
      t.r3.u64 = 0;
      t.r4.u64 = 0;
      t.r5.u64 = 0;
      t.r6.u64 = 0;
    }, __imp__sub_825A7270, sub_825A7270, "all 0");
    results.Report();
  }

  // The atof chain, bottom up: __mtold12 on digit buffers, then
  // __strgtold12_l, _fltin2, _atof_l and atof on the same strings (signs,
  // points, exponents with d, D, e and E, too many digits, the limits of
  // the exponent and of a double, spaces, junk; random numbers and random
  // strings from the characters the parser knows), at odd addresses and
  // across a page end. Each recompiled version calls the native ones below
  // it, so each step is checked on its own.
  {
    Results results("__mtold12 (sub_825B7F10)");
    for (int i = 0; i < 600; i++) {
      const uint32_t count = 1 + h.random.Below(i < 500 ? 25 : 28);
      std::vector<uint8_t> digits(count);
      for (uint32_t k = 0; k < count; k++) {
        digits[k] = uint8_t(k == 0 ? 1 + h.random.Below(9) : h.random.Below(i % 7 == 0 ? 11 : 10));
      }
      // The scratch area only: the original leaves r3 pointing into its
      // finished frame, which its one caller doesn't read.
      const uint32_t from = h.data + 64 + (i & 3), out = h.data + 2048 + (i & 1);
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        WriteBytes(h, from, digits, false);
        PPCContext t = CallContext(h);
        t.r3.u64 = from;
        t.r4.u64 = count;
        t.r5.u64 = out;
        (v == 0 ? __imp__sub_825B7F10 : NativeMtold12)(t, h.base);
        result[v] = Read(h.base, h.data, kScratchBytes);
      }
      results.Check(result[0], result[1], fmt::format("{} digits", count));
    }
    results.Report();
  }
  std::vector<std::string> numbers = {
      "", "0", "1", "-1", "+1", "1.5", "-0.0", ".5", "5.", ".", "-.", "+.e5", "1e5", "1E-5",
      "1d10", "1D+10", "2e", "2e+", "2e-x", "2e0", "2e-0", "2e00005", "-2E+0010", "3.14159265358979323846264338327950288",
      "1e308", "1.7976931348623157e308", "1.8e308", "2.2250738585072014e-308", "4.9e-324",
      "2.4e-324", "2.5e-324", "1e-400", "1e4931", "1e4933", "1e5200", "1e5201", "1e99999",
      "1e-5200", "1e-5201", "1e-99999", "0.000000000000000000000000000001",
      "123456789012345678901234567890", "999999999999999999999999999", "99999999999999999999999.5",
      "1234567890123456789012345e-10", "0.1234567890123456789012345678", "0000000000123",
      "00.00", "0e5", "000", "0.", "-0", "  \t\n\r 42", "\v42", "\f42", " - 1", "1+2", "1-2",
      "1.5+3", "1.5-3e", "abc", "-abc", "1.2.3", "1,5", "0x1A", "inf", "nan", "1e0010",
      "12e3456", "\x80" "1", "1\xB0", "9.9999999999999999999999999", "0.5e1e2", "6e-1d", "8.e",
      "1e+-5", "1ee5", ".e", "7.25E", "0.1", "0.2", "0.3", "100", "1048576", "33.333",
      "-273.15", "6.02214076e23", "1.602176634e-19", "4294967296", "9007199254740993",
      "2.5", "3.5", "0.125", "1.00000000000000011102230246251565404236316680908203125",
      "1.0000000000000001110223024625156540423631668090820312500001"};
  {
    const char kChars[] = "0123456789.+-eEdD x0";
    for (int i = 0; i < 1500; i++) {
      std::string text;
      if (i % 3 == 0) {
        const uint32_t length = 1 + h.random.Below(30);
        for (uint32_t k = 0; k < length; k++) {
          text += kChars[h.random.Below(sizeof(kChars) - 1)];
        }
      } else {
        if (h.random.Below(3) == 0) {
          text += h.random.Below(2) ? '-' : '+';
        }
        const uint32_t whole = h.random.Below(i % 5 == 0 ? 30 : 12);
        for (uint32_t k = 0; k < whole; k++) {
          text += char('0' + h.random.Below(10));
        }
        if (h.random.Below(2)) {
          text += '.';
          const uint32_t part = h.random.Below(i % 7 == 0 ? 30 : 12);
          for (uint32_t k = 0; k < part; k++) {
            text += char('0' + h.random.Below(10));
          }
        }
        if (h.random.Below(2)) {
          text += "eEdD"[h.random.Below(4)];
          if (h.random.Below(2)) {
            text += h.random.Below(2) ? '-' : '+';
          }
          text += std::to_string(h.random.Below(i % 4 == 0 ? 6000 : 400));
        }
      }
      numbers.push_back(text);
    }
  }
  auto string_place = [&](size_t i) {
    return i % 5 == 4 ? h.data + places.page_end - 6 : h.data + 64 + uint32_t(i % 4);
  };
  auto write_string = [&](uint32_t at, const std::string& text) {
    WriteBytes(h, at, std::vector<uint8_t>(text.begin(), text.end()));
  };
  {
    Results results("__strgtold12_l (sub_825B36B8)");
    for (size_t i = 0; i < numbers.size(); i++) {
      const uint32_t at = string_place(i);
      for (int v = 0; v < (i < 120 ? 6 : 1); v++) {
        // mult12, scale, decpt and implicit_E: _fltin2's zeros, then others.
        static const uint32_t kArgs[6][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 5, 0, 0},
                                             {0, uint32_t(-3), 2, 0}, {0, 0, 0, 1},
                                             {1, 7, uint32_t(-4), 1}};
        StringCase(h, results, [&] { write_string(at, numbers[i]); },
                   [&](PPCContext& t) {
                     t.r3.u64 = h.data + 2048;
                     t.r4.u64 = h.data + 2064;
                     t.r5.u64 = at;
                     t.r6.u64 = kArgs[v][0];
                     t.r7.u64 = kArgs[v][1];
                     t.r8.u64 = kArgs[v][2];
                     t.r9.u64 = kArgs[v][3];
                     t.r10.u64 = 0x8266BC28;
                   },
                   __imp__sub_825B36B8, NativeStrgtold12, fmt::format("\"{}\" args {}", numbers[i], v));
      }
    }
    results.Report();
  }
  {
    Results results("_fltin2 (sub_825AFDC8)");
    for (size_t i = 0; i < numbers.size(); i++) {
      const uint32_t at = string_place(i);
      StringCase(h, results, [&] { write_string(at, numbers[i]); },
                 [&](PPCContext& t) {
                   t.r3.u64 = h.data + 2048 + (i & 3) * 4;
                   t.r4.u64 = at;
                   t.r5.u64 = uint32_t(numbers[i].size());
                   t.r6.u64 = 0;
                   t.r7.u64 = 0;
                   t.r8.u64 = 0x8266BC28;
                 },
                 __imp__sub_825AFDC8, NativeFltin2, fmt::format("\"{}\"", numbers[i]));
    }
    results.Report();
  }
  // _atof_l and atof: f1 and the scratch area compared.
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "_atof_l (sub_825AB0C0)" : "atof (sub_825AB1C0)");
    for (size_t i = 0; i < numbers.size(); i++) {
      const uint32_t at = string_place(i);
      const uint64_t locale = h.random.Next();  // unused by both
      std::vector<uint8_t> result[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        write_string(at, numbers[i]);
        PPCContext t = CallContext(h);
        t.r3.u64 = at;
        t.r4.u64 = locale;
        t.f1.u64 = 0x7FF4000000000001ull;
        if (which == 0) {
          (v == 0 ? __imp__sub_825AB0C0 : NativeAtofL)(t, h.base);
        } else {
          (v == 0 ? __imp__sub_825AB1C0 : sub_825AB1C0)(t, h.base);
        }
        result[v] = Read(h.base, h.data, kScratchBytes);
        Append(result[v], t.f1.u64);
      }
      results.Check(result[0], result[1], fmt::format("\"{}\"", numbers[i]));
    }
    results.Report();
  }

  // $I10_OUTPUT (sub_825B8B48): long doubles (the special values, zeros,
  // unnormalized and exponent-0 mantissas, every size of exponent, ___dtold's
  // forms of doubles) with 0 to 25 digits asked for (-1 and 0 too: the
  // original then reads a byte it never wrote, both runs take it), flags 0
  // and 1, the FOS at even and odd addresses; the scratch area and r3
  // compared.
  {
    Results results("$I10_OUTPUT (sub_825B8B48)");
    std::vector<std::tuple<uint16_t, uint32_t, uint32_t>> values = {
        {0, 0, 0},           {0x8000, 0, 0},           {0x7FFF, 0x80000000, 0},
        {0xFFFF, 0x80000000, 0}, {0xFFFF, 0xC0000000, 0}, {0xFFFF, 0xC0000000, 1},
        {0x7FFF, 0xC0000000, 0}, {0x7FFF, 0xC0000001, 5}, {0x7FFF, 0x80000000, 1},
        {0x7FFF, 0x00000001, 0}, {0xFFFF, 0x3FFFFFFF, 7}, {0x3FFF, 0x80000000, 0},
        {0x3FFE, 0xFFFFFFFF, 0xFFFFFFFF}, {0x4002, 0xA0000000, 0}, {0x0000, 0x00000001, 0},
        {0x0001, 0x80000000, 0}, {0x7FFE, 0xFFFFFFFF, 0xFFFFFFFF}, {0x3FFF, 0x00000000, 1}};
    for (int i = 0; i < 400; i++) {
      const uint64_t bits = h.random.Next();
      const c_runtime::LongDouble12 d =
          c_runtime::DoubleToLd12(i % 4 == 0 ? bits : (bits & 0x800FFFFFFFFFFFFFull) |
                                                         (uint64_t(1000 + h.random.Below(48)) << 52));
      values.push_back({d.sign_exponent, d.mantissa_high, d.mantissa_low});
      values.push_back({uint16_t(h.random.Next()), uint32_t(h.random.Next()) | 0x80000000u,
                        uint32_t(h.random.Next())});
    }
    for (int i = 0; i < 40; i++) {  // powers of ten and their neighbors
      double v = 1;
      for (int k = 0; k < i - 20; k++) v *= 10;
      for (int k = 0; k < 20 - i; k++) v /= 10;
      const uint64_t b = std::bit_cast<uint64_t>(v);
      for (uint64_t near : {b - 1, b, b + 1}) {
        const c_runtime::LongDouble12 d = c_runtime::DoubleToLd12(near);
        values.push_back({d.sign_exponent, d.mantissa_high, d.mantissa_low});
      }
    }
    size_t n = 0;
    for (const auto& [sign_exponent, high, low] : values) {
      const uint64_t r3 = uint64_t(sign_exponent) << 48 | uint64_t(high) << 16 | (low >> 16);
      const uint64_t r4 = uint64_t(low & 0xFFFF) << 48;
      for (int32_t digits : {17, int32_t(n % 27) - 1}) {
        const uint32_t flags = n % 3 == 2 ? 1 : 0;
        const uint32_t fos = h.data + 2048 + uint32_t(n & 1);
        StringCase(h, results, [] {},
                   [&](PPCContext& t) {
                     t.r3.u64 = r3;
                     t.r4.u64 = r4;
                     t.r5.u64 = uint64_t(int64_t(digits));
                     t.r6.u64 = flags;
                     t.r7.u64 = fos;
                   },
                   __imp__sub_825B8B48, sub_825B8B48,
                   fmt::format("{:04X} {:08X} {:08X} digits {} flags {}", sign_exponent, high,
                               low, digits, flags));
      }
      n++;
    }
    results.Report();
  }

  // _fltout2 (sub_825B6458): doubles (the special values, zeros, denormals,
  // powers of ten and their neighbors, random bits and random values), the
  // buffer big enough (strcpy_s's error path, never seen, left out); the
  // scratch area and r3 compared.
  {
    Results results("_fltout2 (sub_825B6458)");
    std::vector<uint64_t> doubles = {0, 0x8000000000000000ull, 0x7FF0000000000000ull,
                                     0xFFF0000000000000ull, 0x7FF8000000000000ull,
                                     0xFFF8000000000000ull, 0x7FF0000000000001ull,
                                     0x7FF4000000000000ull, 0x0000000000000001ull,
                                     0x000FFFFFFFFFFFFFull, 0x0010000000000000ull,
                                     0x7FEFFFFFFFFFFFFFull, 0x3FF0000000000000ull,
                                     std::bit_cast<uint64_t>(0.1), std::bit_cast<uint64_t>(123.456),
                                     std::bit_cast<uint64_t>(-2.5), std::bit_cast<uint64_t>(1e22),
                                     std::bit_cast<uint64_t>(9.999999999999999e22),
                                     std::bit_cast<uint64_t>(0.5), std::bit_cast<uint64_t>(1e-5)};
    for (int i = -30; i <= 30; i++) {
      const uint64_t b = std::bit_cast<uint64_t>(std::pow(10.0, i));
      doubles.push_back(b - 1);
      doubles.push_back(b);
      doubles.push_back(b + 1);
    }
    for (int i = 0; i < 600; i++) {
      const uint64_t bits = h.random.Next();
      doubles.push_back(i % 2 == 0 ? bits : (bits & 0x800FFFFFFFFFFFFFull) |
                                                (uint64_t(1000 + h.random.Below(48)) << 52));
    }
    size_t n = 0;
    for (uint64_t d : doubles) {
      const uint32_t result = h.data + 2048 + 4 * uint32_t(n & 1);
      const uint32_t buffer = h.data + 1024 + uint32_t(n % 3);
      StringCase(h, results, [] {},
                 [&](PPCContext& t) {
                   t.r3.u64 = d;
                   t.r4.u64 = result;
                   t.r5.u64 = buffer;
                   t.r6.u64 = 22 + n % 40;
                 },
                 __imp__sub_825B6458, sub_825B6458, fmt::format("{:016X}", d));
      n++;
    }
    results.Report();
  }

  // %f (_cfltcvt_l sub_825AEB20, _cftof_l sub_825AE8E0): doubles (the
  // special values, zeros of both signs, denormals, rounding carries like
  // 9.995 and 0.95, powers of ten and their neighbors, random values and
  // bits) with precisions 0 to 6, 17, 30, 100, 163 and 300, the buffer at
  // three alignments and big enough (the error paths through _errno, never
  // seen, left out); 'e', 'E', 'g' and 'G' too, through _cfltcvt_l's
  // dispatch (recompiled, calling the _fptostr and _cftof2_l overrides).
  // The scratch area and r3 compared.
  {
    std::vector<uint64_t> doubles = {0, 0x8000000000000000ull, 0x7FF0000000000000ull,
                                     0xFFF0000000000000ull, 0x7FF8000000000000ull,
                                     0xFFF8000000000000ull, 0x7FF0000000000001ull,
                                     0x7FF4000000000000ull, 0x0000000000000001ull,
                                     0x800FFFFFFFFFFFFFull, 0x0010000000000000ull,
                                     0x7FEFFFFFFFFFFFFFull, 0xFFEFFFFFFFFFFFFFull};
    for (double v : {9.995, -9.995, 0.995, 0.95, 0.5, 1.5, 2.5, -0.5, 0.05, 0.005, 99.95,
                     999.5, 9.5, 9.9999999, 0.0005, 0.00049, 1e22, 1e-5, 123.456, -2.5,
                     0.1, 0.3, 1.0, 10.0, 99.999, 9999999.9999, 0.999999999, 1e15 - 0.5,
                     4503599627370495.5, 1e300, 1e-300}) {
      doubles.push_back(std::bit_cast<uint64_t>(v));
    }
    for (int i = -12; i <= 12; i++) {
      const uint64_t b = std::bit_cast<uint64_t>(std::pow(10.0, i));
      doubles.push_back(b - 1);
      doubles.push_back(b);
      doubles.push_back(b + 1);
    }
    for (int i = 0; i < 160; i++) {
      const uint64_t bits = h.random.Next();
      doubles.push_back(i % 4 == 0 ? bits : (bits & 0x800FFFFFFFFFFFFFull) |
                                                (uint64_t(960 + h.random.Below(120)) << 52));
    }
    const int32_t kPrecisions[] = {0, 1, 2, 3, 4, 5, 6, 17, 30, 100, 163, 300};
    Results f_results("%f: _cfltcvt_l and _cftof_l (sub_825AEB20, sub_825AE8E0)");
    Results other_results("_cfltcvt_l's 'e' and 'g' (sub_825AEB20, recompiled below it)");
    size_t n = 0;
    for (uint64_t d : doubles) {
      for (size_t k = 0; k < std::size(kPrecisions); k++) {
        const int32_t precision = kPrecisions[k];
        const bool other = (n + k) % 7 == 3 && precision <= 30;
        static const int32_t kOthers[] = {'e', 'E', 'g', 'G'};
        const int32_t format = other ? kOthers[(n + k) % 4] : 'f';
        const uint32_t buffer = h.data + 64 + uint32_t((n + k) % 3);
        const uint32_t size = 2400 + uint32_t(h.random.Below(1000));
        for (int direct = 0; direct < (format == 'f' ? 2 : 1); direct++) {
          StringCase(h, other ? other_results : f_results,
                     [&] {
                       guest::StoreU32(h.base, h.data + 8, uint32_t(d >> 32));
                       guest::StoreU32(h.base, h.data + 12, uint32_t(d));
                       guest::StoreU8(h.base, h.data + kScratchBytes - 1, 0);
                     },
                     [&](PPCContext& t) {
                       t.r3.u64 = h.data + 8;
                       t.r4.u64 = buffer;
                       t.r5.u64 = size;
                       if (direct) {
                         t.r6.u64 = uint64_t(int64_t(precision));
                         t.r7.u64 = 0;
                       } else {
                         t.r6.u64 = uint64_t(format);
                         t.r7.u64 = uint64_t(int64_t(precision));
                         t.r8.u64 = n & 1;
                         t.r9.u64 = 0;
                       }
                     },
                     direct ? __imp__sub_825AE8E0 : __imp__sub_825AEB20,
                     direct ? sub_825AE8E0 : sub_825AEB20,
                     fmt::format("{:016X} %{}.{}{}", d, char(format), precision,
                                 direct ? " (_cftof_l)" : ""));
        }
      }
      n++;
    }
    f_results.Report();
    other_results.Report();
  }

  // _fptostr (sub_825B61E8): digit strings (random digits, runs of nines to
  // carry through, '5' and above at the rounding place, high bytes, shorter
  // than asked) with -3 to 40 digits wanted and 100 to 300, the result's
  // exponent random, the buffer at three alignments with room to spare and
  // at the least room (size = digits + 2). The scratch area and r3 compared.
  {
    Results results("_fptostr (sub_825B61E8)");
    for (int i = 0; i < 900; i++) {
      const uint32_t length = h.random.Below(i % 5 == 0 ? 60 : 24);
      std::vector<uint8_t> mantissa(length);
      for (uint8_t& c : mantissa) {
        const uint32_t r = h.random.Below(10);
        c = r < 4 ? '9' : r < 9 ? uint8_t('0' + h.random.Below(10)) : uint8_t(0x80 + h.random.Below(0x7F));
      }
      const int32_t digits = i % 10 == 9 ? int32_t(100 + h.random.Below(200))
                                         : int32_t(h.random.Below(44)) - 3;
      const uint32_t whole = digits > 0 ? uint32_t(digits) : 0;
      const uint32_t size = i % 3 == 0 ? whole + 2 : whole + 2 + h.random.Below(400);
      const uint32_t buffer = h.data + 64 + uint32_t(i % 3);
      const uint32_t result = h.data + 2048;
      const uint32_t digits_at = h.data + 2200 + uint32_t(i % 2);
      const int32_t exponent = int32_t(h.random.Below(80)) - 40;
      StringCase(h, results,
                 [&] {
                   guest::StoreU32(h.base, result, i % 2 ? '-' : ' ');
                   guest::StoreU32(h.base, result + 4, uint32_t(exponent));
                   guest::StoreU32(h.base, result + 8, 1);
                   guest::StoreU32(h.base, result + 12, digits_at);
                   WriteBytes(h, digits_at, mantissa);
                 },
                 [&](PPCContext& t) {
                   t.r3.u64 = buffer;
                   t.r4.u64 = size;
                   t.r5.u64 = uint64_t(int64_t(digits));
                   t.r6.u64 = result;
                 },
                 __imp__sub_825B61E8, sub_825B61E8,
                 fmt::format("\"{}\" digits {} size {}",
                             std::string(mantissa.begin(), mantissa.end()), digits, size));
    }
    results.Report();
  }

  // _cftof2_l (sub_825AE708): digit strings as _fptostr leaves them, signs
  // '-', ' ' and others, exponents from -45 to the string's length,
  // precisions 0 to 40 and 100 to 300 (and -1), %f and %g (g_format, with
  // the exponent one above the precision often). The scratch area and r3
  // compared.
  {
    Results results("_cftof2_l (sub_825AE708)");
    for (int i = 0; i < 900; i++) {
      const uint32_t length = 1 + h.random.Below(i % 6 == 0 ? 120 : 30);
      std::vector<uint8_t> text(length);
      for (uint8_t& c : text) {
        c = uint8_t('0' + h.random.Below(10));
      }
      static const uint32_t kSigns[] = {'-', ' ', '+', 0x2D00};
      const uint32_t sign = kSigns[i % 4];
      const int32_t precision = i % 8 == 7 ? int32_t(100 + h.random.Below(200))
                                : i % 50 == 1 ? -1
                                              : int32_t(h.random.Below(41));
      const bool g_format = i % 3 == 1;
      int32_t exponent = int32_t(h.random.Below(length + 46)) - 45;
      if (g_format && i % 2 == 0 && precision + 1 <= int32_t(length)) {
        exponent = precision + 1;
      }
      const uint32_t buffer = h.data + 64 + uint32_t(i % 3);
      const uint32_t result = h.data + 2048;
      const uint32_t size = 1 + h.random.Below(2000);
      StringCase(h, results,
                 [&] {
                   guest::StoreU32(h.base, result, sign);
                   guest::StoreU32(h.base, result + 4, uint32_t(exponent));
                   WriteBytes(h, buffer + (sign == '-' ? 1 : 0), text);
                   guest::StoreU8(h.base, h.data + kScratchBytes - 1, 0);
                 },
                 [&](PPCContext& t) {
                   t.r3.u64 = buffer;
                   t.r4.u64 = size;
                   t.r5.u64 = uint64_t(int64_t(precision));
                   t.r6.u64 = result;
                   t.r7.u64 = g_format ? 0x100 | (1 + i % 200) : 0x100;
                   t.r8.u64 = 0;
                 },
                 __imp__sub_825AE708, sub_825AE708,
                 fmt::format("\"{}\" sign {:X} exponent {} precision {} g {}",
                             std::string(text.begin(), text.end()), sign, exponent, precision,
                             g_format));
    }
    results.Report();
  }

  // __iob_func: r3 from random registers.
  {
    Results results("__iob_func (sub_825AD4C0)");
    for (int i = 0; i < 16; i++) {
      const uint64_t r3 = h.random.Next();
      StringCase(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = r3; }, __imp__sub_825AD4C0,
                 sub_825AD4C0, fmt::format("r3 {:X}", r3));
    }
    results.Report();
  }
}

}  // namespace harness
