// The C runtime's helpers (src/hooks/c_runtime.cpp) against the recompiled
// ones.

#include "harness.h"

#include "base/log.h"

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
}

}  // namespace harness
