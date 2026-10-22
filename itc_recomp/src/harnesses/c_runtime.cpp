// The C runtime's helpers (src/hooks/c_runtime.cpp) against the recompiled
// ones.

#include "harness.h"

#include "base/log.h"

#include <algorithm>

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

const uint32_t kLengths[] = {0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 17, 31, 64, 200, 450};

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
                         __imp__sub_825A9DD8, sub_825A9DD8, what);
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
              StringCase(h, ascii, setup, args, __imp__sub_825AC6B0, sub_825AC6B0, with);
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
                       __imp__sub_825A82C8, sub_825A82C8, what);
          }
        }
      }
    }
    unsigned_results.Report();
    signed_results.Report();
  }
}

}  // namespace harness
