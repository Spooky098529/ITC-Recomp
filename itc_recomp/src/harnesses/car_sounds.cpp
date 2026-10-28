// The car sounds (src/car_sounds/, docs/functions/car-sounds.md) against the
// recompiled functions. The game's live car sounds aren't touched: each case
// builds its objects in a block from the allocation layer, filled with the
// same random words for both versions, and runs the original (with
// --native_car_sounds off, so its callees are the originals too) or the
// plain C++ through the register steps (hooks/car_sounds.h). The block and
// r3 are compared with harness::Outputs: exactly, except the fields the code
// loads and stores as floats.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_sounds/car_sounds.h"
#include "hooks/car_sounds.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <bit>
#include <cmath>
#include <limits>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_car_sounds);

namespace harness {

namespace {

namespace cs = car_sounds;

constexpr uint32_t kBlockBytes = 0x2000;

struct Block {
  Block() : address(memory::Alloc(kBlockBytes, 0)) {}
  ~Block() { memory::Free(address); }
  Block(const Block&) = delete;
  Block& operator=(const Block&) = delete;
  uint32_t address;
};

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t FloatBits(float value) { return std::bit_cast<uint32_t>(value); }

// The block's bytes with the float fields (offsets in the block) left out
// as bytes and added as floats.
void BlockOutputs(Outputs& out, uint8_t* base, uint32_t block,
                  const std::vector<uint32_t>& floats) {
  std::vector<uint8_t> bytes = Read(base, block, kBlockBytes);
  for (const uint32_t offset : floats) {
    for (uint32_t i = 0; i < 4; i++) {
      bytes[offset + i] = 0;
    }
  }
  out.Bytes(bytes);
  for (const uint32_t offset : floats) {
    out.Float(guest::LoadU32(base, block + offset));
  }
}

// One case: the block filled with random words, then `setup(random, t)`
// (the same random state for both versions) writes the inputs and sets the
// registers; the block and r3 are compared.
template <typename Setup>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::vector<uint32_t>& floats,
          const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    setup(random, t);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Word(t.r3.u32);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// --- The wave bank views ---

// The view at +0, the bank data at +0x100 (+0 flags, +4 count, +84 the
// compact format), the entries at +0x400, the wave data's start any word,
// the out block at +0x1F00.
constexpr uint32_t kView = 0x0;
constexpr uint32_t kBankData = 0x100;
constexpr uint32_t kEntries = 0x400;
constexpr uint32_t kOut = 0x1F00;

// A format word: PCM (low bits 0) most of the time, bit 31 random.
uint32_t FormatWord(Random& random) {
  uint32_t word = uint32_t(random.Next());
  if (random.Below(4) != 0) {
    word &= ~3u;
  }
  return word;
}

void LinkView(uint8_t* base, uint32_t block, Random& random) {
  Store(base, block + kView + 12, block + kBankData);
  Store(base, block + kView + 16, block + kEntries);
  uint32_t flags = uint32_t(random.Next());
  if (random.Below(2) == 0) {
    flags &= ~0x20000u;
  } else {
    flags |= 0x20000u;
  }
  Store(base, block + kBankData + 0, flags);
  Store(base, block + kBankData + 4, random.Below(10));
  Store(base, block + kBankData + 84, FormatWord(random));
  for (uint32_t i = 0; i < 12; i++) {
    Store(base, block + kEntries + 24 * i + 4, FormatWord(random));
  }
}

void WaveBankViews(Context& h, uint32_t block) {
  {
    Results results("WaveBankView::Init (sub_8210C9D8)");
    for (int i = 0; i < 40; i++) {
      Case(
          h, results, block, __imp__sub_8210C9D8, hooks::CarSoundsWaveBankViewInit,
          [&](Random& random, PPCContext& t) {
            const uint32_t bank = block + 0x200;
            // The offsets: small ones, or any word (the sums wrap).
            for (const uint32_t at : {8u, 16u, 40u}) {
              Store(h.base, bank + at,
                    random.Below(2) == 0 ? random.Below(0x1000) : uint32_t(random.Next()));
            }
            t.r3.u64 = block + kView + 4 * random.Below(8);
            t.r4.u64 = bank;
          },
          {}, fmt::format("case {}", i));
    }
    results.Report();
  }
  const struct {
    const char* name;
    GuestFunction recompiled, native;
  } getters[] = {
      {"WaveBankView::Format (sub_8210C850)", __imp__sub_8210C850, hooks::CarSoundsWaveBankFormat},
      {"WaveBankView::Length (sub_8210C920)", __imp__sub_8210C920, hooks::CarSoundsWaveBankLength},
      {"WaveBankView::Data (sub_8210C958)", __imp__sub_8210C958, hooks::CarSoundsWaveBankData},
      {"WaveBankView::Entry (sub_8210C998)", __imp__sub_8210C998, hooks::CarSoundsWaveBankEntry},
  };
  for (const auto& g : getters) {
    Results results(g.name);
    for (int i = 0; i < 80; i++) {
      Case(
          h, results, block, g.recompiled, g.native,
          [&](Random& random, PPCContext& t) {
            LinkView(h.base, block, random);
            t.r3.u64 = block + kView;
            // Past the count now and then; Entry doesn't check it (kept
            // inside the entries' area).
            t.r4.u64 = random.Below(12);
            t.r5.u64 = block + kOut + 4 * random.Below(8);
          },
          {}, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The car sound objects ---

void Objects(Context& h, uint32_t block) {
  {
    // The table at 0x82616318 points at the case's lists while it runs (put
    // back after): entry i (16 bytes apart) a list of pointers at +0x100,
    // each to parameters in the block.
    Results results("CarSounds::SetBanks (sub_8210CB48)");
    uint32_t saved[4];
    for (uint32_t i = 0; i < 4; i++) {
      saved[i] = guest::LoadU32(h.base, cs::kSoundParameters + 16 * i);
    }
    for (int c = 0; c < 40; c++) {
      Case(
          h, results, block, __imp__sub_8210CB48, hooks::CarSoundsSetBanks,
          [&](Random& random, PPCContext& t) {
            const uint32_t i = random.Below(4);
            const uint32_t j = random.Below(4);
            Store(h.base, cs::kSoundParameters + 16 * i, block + 0x100 + 0x10 * i);
            Store(h.base, block + 0x100 + 0x10 * i + 4 * j, block + 0x200 + 4 * random.Below(64));
            t.r3.u64 = block + 0x800 + 4 * random.Below(16);
            t.r4.u64 = i;
            t.r5.u64 = j;
          },
          {}, fmt::format("case {}", c));
    }
    for (uint32_t i = 0; i < 4; i++) {
      Store(h.base, cs::kSoundParameters + 16 * i, saved[i]);
    }
    results.Report();
  }
  {
    Results results("TireSound::Amount (sub_8210CA28)");
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    // The thresholds the code compares with, read from the game's data.
    const auto constant = [&](uint32_t address) {
      return double(guest::LoadFloat(h.base, address));
    };
    const double one = constant(0x82000A34);
    const double upper = constant(0x82007FEC);
    const double low = constant(0x820067B0);
    const double f1s[] = {one, upper, 0.0, -0.0, 0.5, -0.5, 1.0, 2.0, -3.0, 100.0, -100.0,
                          1e12, -1e12, nan, -nan, inf, -inf, (one + upper) / 2,
                          std::nextafter(upper, inf), std::nextafter(one, -inf)};
    const uint32_t part = block + 0x100;
    int c = 0;
    for (const double f1 : f1s) {
      for (int k = 0; k < 6; k++, c++) {
        Case(
            h, results, block, __imp__sub_8210CA28, hooks::CarSoundsTireSoundAmount,
            [&](Random& random, PPCContext& t) {
              // +72 below or above the threshold, now and then a NaN.
              const float amounts[] = {float(low) * 0.5f, float(low) * 2.0f, float(low), 0.0f,
                                       -1.0f, std::numeric_limits<float>::quiet_NaN(),
                                       float(random.Below(1000)) * 0.01f};
              guest::StoreFloat(h.base, part + 72, amounts[random.Below(7)]);
              guest::StoreU16(h.base, part + 70,
                              uint16_t(random.Below(2) == 0 ? random.Below(10) : random.Next()));
              t.r3.u64 = part;
              t.f1.f64 = f1;
            },
            {0x100 + 80}, fmt::format("f1 {}, case {}", f1, c));
      }
    }
    results.Report();
  }
  {
    Results results("EngineSound::EngineSound (sub_8210CBA0)");
    for (int c = 0; c < 4; c++) {
      Case(
          h, results, block, __imp__sub_8210CBA0, hooks::CarSoundsEngineSoundConstruct,
          [&](Random& random, PPCContext& t) { t.r3.u64 = block + 4 * random.Below(64); }, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::CarSound (sub_8210CC18)");
    for (int c = 0; c < 4; c++) {
      Case(
          h, results, block, __imp__sub_8210CC18, hooks::CarSoundsCarSoundConstruct,
          [&](Random& random, PPCContext& t) { t.r3.u64 = block + 4 * random.Below(64); }, {},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // Without freeing (flags bit 0 clear): the array of 0 to 4 car sounds
    // (its count at r3 - 16) or one car sound.
    Results results("CarSounds::Destroy (sub_8210DCA8)");
    for (int c = 0; c < 24; c++) {
      Case(
          h, results, block, __imp__sub_8210DCA8, hooks::CarSoundsDestroy,
          [&](Random& random, PPCContext& t) {
            const uint32_t sound = block + 16 + 4 * random.Below(4);
            Store(h.base, sound - 16, random.Below(5));
            t.r3.u64 = sound;
            t.r4.u64 = (uint32_t(random.Next()) & ~1u);
          },
          {}, fmt::format("case {}", c));
    }
    // Freeing: a block from operator new (new[] for the array) per version;
    // the r3 left as an offset from it.
    for (int c = 0; c < 8; c++) {
      const bool array = c % 2 == 0;
      const uint32_t count = uint32_t(c / 2);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        const uint32_t bytes = array ? 16 + count * cs::kCarSoundBytes : cs::kCarSoundBytes;
        const auto made = array ? memory::crt::OperatorNewArray(h.base, bytes)
                                : memory::crt::OperatorNew(h.base, bytes);
        if (!made) {
          ITC_LOG_ERROR("harness: car_sounds: operator new refused {} bytes", bytes);
          return;
        }
        Fill(h.base, *made, bytes, 0xCD);
        const uint32_t sound = array ? *made + 16 : *made;
        if (array) {
          Store(h.base, *made, count);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = sound;
        t.r4.u64 = array ? 3 : 1;
        ITC_FLAG(native_car_sounds) = v == 1;
        (v == 0 ? __imp__sub_8210DCA8 : hooks::CarSoundsDestroy)(t, h.base);
        ITC_FLAG(native_car_sounds) = true;
        out[v].Word(t.r3.u32 - *made);
      }
      results.Check(out[0], out[1],
                    fmt::format("freed, {}, count {}", array ? "array" : "one", count));
    }
    results.Report();
  }
}

}  // namespace

void CarSounds(Context& h) {
  Block block;
  if (!block.address) {
    ITC_LOG_ERROR("harness: car_sounds: no block from the allocation layer");
    return;
  }
  WaveBankViews(h, block.address);
  Objects(h, block.address);
}

}  // namespace harness
