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
#include "models/memory_blocks.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
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

// --- The set-ups ---

void SetUps(Context& h, uint32_t block) {
  {
    // The car sound at any word's alignment (stvx rounds the vectors'
    // addresses down to 16); the matrix from operator new[] differs between
    // the versions: r3 is checked against +176, then both are left out.
    Results results("CarSound::SetUp (sub_8210DDC8)");
    for (int c = 0; c < 8; c++) {
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        for (uint32_t i = 0; i < kBlockBytes; i += 4) {
          Store(h.base, block + i, uint32_t(random.Next()));
        }
        const uint32_t sound = block + 4 * random.Below(64);
        PPCContext t = CallContext(h);
        t.r3.u64 = sound;
        ITC_FLAG(native_car_sounds) = v == 1;
        (v == 0 ? __imp__sub_8210DDC8 : hooks::CarSoundsSetUp)(t, h.base);
        ITC_FLAG(native_car_sounds) = true;
        const uint32_t matrix = t.r3.u32;
        out[v].Word(matrix != 0 && guest::LoadU32(h.base, sound + 176) == matrix);
        Store(h.base, sound + 176, 0);
        BlockOutputs(out[v], h.base, block, {});
        memory::crt::OperatorDeleteArray(matrix);
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // On the game's globals (0x82616BEC, 0x82616C34 to 0x82616C3F,
    // 0x826C5C48 to 0x826C5C9F), saved and put back after each version, with
    // a music player of the case's whose slot 4 is an empty function
    // (sub_82106540). The two physical buffers are compared by their
    // headers and freed.
    Results results("SoundGlobals::Init (sub_82110258)");
    if (!hooks::CarSoundsGlobalsNative()) {
      ITC_LOG_WARN("harness: car_sounds: the memory blocks aren't native; SoundGlobals::Init skipped");
      return;
    }
    const struct {
      uint32_t address, bytes;
    } ranges[] = {{0x82616BEC, 4}, {0x82616C34, 12}, {0x826C5C48, 0x58}};
    for (int c = 0; c < 2; c++) {
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        std::vector<uint8_t> saved[3];
        for (int r = 0; r < 3; r++) {
          saved[r] = Read(h.base, ranges[r].address, ranges[r].bytes);
          Fill(h.base, ranges[r].address, ranges[r].bytes, uint8_t(0xA5 + c));
        }
        const uint32_t player = block + 0x100 + 16 * uint32_t(c);
        Store(h.base, player, block + 0x200);
        Store(h.base, block + 0x200 + 16, 0x82106540);
        PPCContext t = CallContext(h);
        t.r3.u64 = player;
        ITC_FLAG(native_car_sounds) = v == 1;
        (v == 0 ? __imp__sub_82110258 : hooks::CarSoundsGlobalsInit)(t, h.base);
        ITC_FLAG(native_car_sounds) = true;
        out[v].Word(t.r3.u32);
        const uint32_t buffers[] = {guest::LoadU32(h.base, 0x826C5C8C),
                                    guest::LoadU32(h.base, 0x826C5C88)};
        for (const uint32_t buffer : buffers) {
          out[v].Word(buffer != 0);
          if (buffer != 0) {
            const uint32_t header = buffer - models::kBlockHeaderBytes;
            out[v].Word(header % 2048);
            out[v].Bytes(Read(h.base, header, 16));
          }
        }
        Store(h.base, 0x826C5C8C, 0);
        Store(h.base, 0x826C5C88, 0);
        for (int r = 0; r < 3; r++) {
          out[v].Bytes(Read(h.base, ranges[r].address, ranges[r].bytes));
          Write(h.base, ranges[r].address, saved[r].data(), ranges[r].bytes);
        }
        for (const uint32_t buffer : buffers) {
          if (buffer != 0) {
            memory::FreePhysical(buffer - models::kBlockHeaderBytes);
          }
        }
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}


// --- CarSound::Update3D ---

// The layout: the vehicle at +0 (its rigid body pointer +336 at 0x150, the
// half-word +5314 at 0x14C2, the flags +6148 at 0x1804; nothing else of it
// read), the car sound at 0x200 (at a word's alignment now and then: lvx and
// stvx round down to 16), the rigid body at 0x700 (its position +208 at
// 0x7D0), the matrix at 0x900, viewport 0 at 0x1000 (its view mode +1860 at
// 0x1744), the viewports' block at 0x1A00 (+0 viewport 0, +8 the second)
// and the race data's two words (+0xA3AF0, +0xA3AF4) at 0x1B00.
constexpr uint32_t kU3Sound = 0x200, kU3Body = 0x700, kU3Matrix = 0x900, kU3Viewport = 0x1000,
                   kU3Viewports = 0x1A00, kU3Race = 0x1B00;
// The original's 288-byte frame and X3DAudioCalculate's (144) with the
// deepest of its callees' (Matrix's 304 and Pan's 192) below it.
constexpr uint32_t kU3Frame = 288 + 144 + 304 + 192;
constexpr uint32_t kU3Ticks = 0x82001FD8, kU3Close = 0x82002568, kU3Drop = 0x8200806C,
                   kU3One = 0x82007FC8, kU3Volume = 0x82616C38;

// A float in [low, high].
float Uniform(Random& random, float low, float high) {
  return low + (high - low) * float(random.Below(1000001)) / 1000000.0f;
}
// Now and then a special float (zeros, infinities, NaNs quiet and
// signaling, denormals, the extremes).
uint32_t Special(Random& random) {
  static constexpr uint32_t kSpecial[] = {0x00000000, 0x80000000, 0x7F800000, 0xFF800000,
                                          0x7FC00000, 0xFFC00000, 0x7F800001, 0xFFA00001,
                                          0x00000001, 0x807FFFFF, 0x7F7FFFFF, 0xFF7FFFFF};
  return kSpecial[random.Below(sizeof(kSpecial) / sizeof(kSpecial[0]))];
}

void Update3Ds(Context& h, uint32_t block) {
  Results results("CarSound::Update3D (sub_8210F820)");
  const uint32_t handle_speakers = guest::LoadU32(h.base, cs::kX3DAudioHandle);
  if (handle_speakers == 0) {
    ITC_LOG_WARN("harness: car_sounds: the X3DAudio handle at 0x826C5C90 has no speakers");
  }
  // The globals it reads, put back after each version: the race data's,
  // the viewports' and the side word's pointers or words, the volume, the
  // two camera passes' +400 to +475.
  const struct {
    uint32_t address, bytes;
  } globals[] = {{0x826C5B2C, 4},       {0x826C2CE8, 4},          {0x826C7824, 4},
                 {kU3Volume, 4},        {0x826BAC00 + 400, 76},   {0x826BAC00 + 592 + 400, 76}};
  constexpr uint32_t kGlobals = sizeof(globals) / sizeof(globals[0]);
  // The car sound's words the code stores with stfs (offsets from it), the
  // DSP settings' (+176) and the matrix's.
  std::vector<uint32_t> sound_floats = {4,   8,   12,  28,  32,  36,  40,  44,  48,  80,  84,
                                        88,  92,  96,  100, 368, 372, 376, 384, 388, 392, 1256,
                                        176 + 16, 176 + 20, 176 + 24, 176 + 28, 176 + 36,
                                        176 + 40, 176 + 44};
  for (uint32_t i = 0; i < 6; i++) {
    sound_floats.push_back(228 + 8 * i);
    sound_floats.push_back(276 + 8 * i);
  }
  // The frames' words the float instructions store (offsets in the region
  // below the stack pointer): Update3D's +80 and +96 to +108; Calculate's
  // vector, Curves' default curves, Cone's and Doppler's vectors, Matrix's
  // locals (as the x3daudio harness lists them).
  std::vector<uint32_t> frame_floats;
  const uint32_t own = kU3Frame - 288;
  for (const uint32_t o : {80u, 96u, 100u, 104u, 108u}) {
    frame_floats.push_back(own + o);
  }
  const uint32_t calc = own - 144;
  for (const uint32_t o : {80u, 84u, 88u}) {
    frame_floats.push_back(calc + o);
  }
  for (const uint32_t o : {96u, 100u, 104u, 108u}) {
    frame_floats.push_back(calc - 128 + o);
  }
  for (const uint32_t o : {88u, 92u, 96u}) {
    frame_floats.push_back(calc - 160 + o);
  }
  for (const uint32_t o : {80u, 84u, 88u}) {
    const uint32_t at = calc - 144 + o;
    if (std::find(frame_floats.begin(), frame_floats.end(), at) == frame_floats.end()) {
      frame_floats.push_back(at);
    }
  }
  for (const uint32_t o :
       {104u, 108u, 112u, 120u, 124u, 128u, 136u, 140u, 144u, 152u, 156u, 160u}) {
    frame_floats.push_back(calc - 304 + o);
  }
  const float ticks = guest::LoadFloat(h.base, kU3Ticks);
  const float close = guest::LoadFloat(h.base, kU3Close);
  const float drop = guest::LoadFloat(h.base, kU3Drop);
  const float one = guest::LoadFloat(h.base, kU3One);
  ITC_LOG_INFO("harness: car_sounds: Update3D's constants: ticks {}, close {}, drop {}, one {}, "
               "volume {}, handle speakers {}",
               ticks, close, drop, one, guest::LoadFloat(h.base, kU3Volume), handle_speakers);
  uint32_t passed = 0, at_camera_cases = 0, within = 0, fallen = 0;
  for (int c = 0; c < 4000; c++) {
    const bool realistic = c >= 1000;
    const bool passing = c >= 3000;
    float passing_step = 0.0f, passing_dir[3] = {};
    const Random start = h.random;
    Random random = start;
    Outputs out[2];
    uint32_t sound = 0;
    for (int v = 0; v < 2; v++) {
      random = start;
      std::vector<uint8_t> saved[kGlobals];
      for (uint32_t g = 0; g < kGlobals; g++) {
        saved[g] = Read(h.base, globals[g].address, globals[g].bytes);
      }
      for (uint32_t i = 0; i < kBlockBytes; i += 4) {
        Store(h.base, block + i, uint32_t(random.Next()));
      }
      PPCContext t = CallContext(h);
      for (uint32_t i = 0; i < kU3Frame; i += 4) {
        Store(h.base, t.r1.u32 - kU3Frame + i, uint32_t(random.Next()));
      }
      sound = block + kU3Sound + (random.Below(5) == 0 ? 4 * random.Below(4) : 0);
      // A realistic listener, emitter and DSP settings.
      cs::SetUp(h.base, sound, block + kU3Matrix);
      const uint32_t vehicle = block + 0;
      Store(h.base, vehicle + 336, block + kU3Body);
      Store(h.base, sound + 400, vehicle);
      guest::StoreU16(h.base, vehicle + 5314, uint16_t(random.Next()));
      // The flags +6148: bits 0x1 and 0x80 either way.
      uint32_t flags = uint32_t(random.Next()) & ~0x81u;
      flags |= random.Below(2) ? 0x1u : 0;
      flags |= random.Below(2) ? 0x80u : 0;
      Store(h.base, vehicle + 6148, flags);
      // The camera's case: the race data's kind (4 or another) and mode (8
      // or another), the second viewport, the view mode (1 or another), the
      // side word, r4.
      static constexpr uint32_t kKinds[] = {4, 4, 0, 1, 5, 6};
      static constexpr uint32_t kModes[] = {8, 8, 0, 3, 4, 7};
      Store(h.base, 0x826C5B2C, block + kU3Race - 0xA3AF0);
      Store(h.base, block + kU3Race + 0, kKinds[random.Below(6)]);
      Store(h.base, block + kU3Race + 4, kModes[random.Below(6)]);
      Store(h.base, 0x826C2CE8, block + kU3Viewports);
      Store(h.base, block + kU3Viewports + 0, block + kU3Viewport);
      Store(h.base, block + kU3Viewports + 8,
            random.Below(2) == 0 ? 0 : block + 4 * random.Below(16));
      static constexpr uint32_t kViewModes[] = {1, 1, 1, 0, 2, 3};
      Store(h.base, block + kU3Viewport + 1860, kViewModes[random.Below(6)]);
      Store(h.base, 0x826C7824, random.Below(2) == 0 ? 0 : uint32_t(random.Next()) | 1);
      const uint32_t r4_kind = random.Below(8);
      t.r4.u64 = r4_kind < 4 ? 1 : r4_kind < 7 ? 0 : random.Next();
      // The volume: the game's 1.0, or another.
      if (random.Below(4) == 0) {
        guest::StoreFloat(h.base, kU3Volume, Uniform(random, 0.0f, 1.5f));
      } else {
        guest::StoreFloat(h.base, kU3Volume, 1.0f);
      }
      // The camera passes' positions (meters), fronts (+464 to +475) and the
      // car near them; their old positions (+368, +384, w lanes any word) a
      // tick's movement back.
      for (const uint32_t pass : {0x826BAC00u, 0x826BAC00u + 592}) {
        for (uint32_t i = 0; i < 3; i++) {
          Store(h.base, pass + 400 + 4 * i, FloatBits(Uniform(random, -3000.0f, 3000.0f)));
        }
        const float angle = Uniform(random, 0.0f, 6.2831853f);
        Store(h.base, pass + 464, FloatBits(std::sin(angle)));
        Store(h.base, pass + 468, FloatBits(Uniform(random, -0.2f, 0.2f)));
        Store(h.base, pass + 472, FloatBits(std::cos(angle)));
      }
      // The pass the code reads (the second for kind 4 with a second
      // viewport).
      const uint32_t pass =
          0x826BAC00u + (guest::LoadU32(h.base, block + kU3Race) == 4 &&
                                 guest::LoadU32(h.base, block + kU3Viewports + 8) != 0
                             ? 592
                             : 0);
      const float reach = random.Below(3) == 0 ? 5.0f : random.Below(2) == 0 ? 30.0f : 300.0f;
      for (uint32_t i = 0; i < 4; i++) {
        const float at = i < 3 ? guest::LoadFloat(h.base, pass + 400 + 4 * i) : 1.0f;
        Store(h.base, block + kU3Body + 208 + 4 * i, FloatBits(at + Uniform(random, -reach, reach)));
      }
      if (passing) {
        // Another car passing: view mode 1, r4 0, the car at the squared
        // distance's edge or within it, moving away or toward along the
        // line at 0 to 40 m/s, the last factor either side of 1.
        Store(h.base, block + kU3Viewport + 1860, 1);
        t.r4.u64 = 0;
        const float a = Uniform(random, 0.0f, 6.2831853f);
        const float b = Uniform(random, -1.5f, 1.5f);
        const float dir[3] = {std::cos(a) * std::cos(b), std::sin(b), std::sin(a) * std::cos(b)};
        const float edge = std::sqrt(close);
        float d = Uniform(random, 0.0f, edge);
        switch (random.Below(4)) {
          case 0:
            d = edge * (1.0f + Uniform(random, -0.002f, 0.002f));
            break;
          case 1:
            d = Uniform(random, edge, 2.0f * edge);
            break;
          default:
            break;
        }
        const float step = Uniform(random, -4.0f, 4.0f) / (ticks * 3.0f) * 10.0f;
        for (uint32_t i = 0; i < 3; i++) {
          const float camera = guest::LoadFloat(h.base, pass + 400 + 4 * i);
          Store(h.base, block + kU3Body + 208 + 4 * i, FloatBits(camera - d * dir[i]));
        }
        passing_step = step;
        passing_dir[0] = dir[0];
        passing_dir[1] = dir[1];
        passing_dir[2] = dir[2];
      }
      if (random.Below(10) == 0) {
        // The car on the listener.
        for (uint32_t i = 0; i < 3; i++) {
          Store(h.base, block + kU3Body + 208 + 4 * i, guest::LoadU32(h.base, pass + 400 + 4 * i));
        }
      }
      for (uint32_t i = 0; i < 3; i++) {
        const float car = guest::LoadFloat(h.base, block + kU3Body + 208 + 4 * i);
        const float camera = guest::LoadFloat(h.base, pass + 400 + 4 * i);
        // Passing: the camera still, the car a tick back along the line.
        Store(h.base, sound + 368 + 4 * i,
              FloatBits(passing ? car + passing_step * passing_dir[i]
                                : car - Uniform(random, -3.0f, 3.0f)));
        Store(h.base, sound + 384 + 4 * i,
              FloatBits(passing ? camera : camera - Uniform(random, -3.0f, 3.0f)));
      }
      // The last Doppler factor (+1256): either side of 1 and of the drop.
      static constexpr float kLast[] = {1.0f, 1.05f, 1.2f, 1.5f, 0.95f, 0.8f, 1.017f, 1.0001f};
      guest::StoreFloat(h.base, sound + 1256,
                        random.Below(4) == 0 ? Uniform(random, 0.5f, 2.0f)
                                             : kLast[random.Below(8)]);
      if (!realistic) {
        // Degenerate words: NaNs, infinities, denormals, any bits in the
        // positions, the old positions, the fronts, the last factor.
        const uint32_t places[] = {block + kU3Body + 208, block + kU3Body + 212,
                                   block + kU3Body + 216, sound + 368,
                                   sound + 372,           sound + 376,
                                   sound + 384,           sound + 388,
                                   sound + 392,           pass + 400,
                                   pass + 404,            pass + 408,
                                   pass + 464,            pass + 472,
                                   sound + 1256};
        const uint32_t count = 1 + random.Below(4);
        for (uint32_t k = 0; k < count; k++) {
          const uint32_t at = places[random.Below(sizeof(places) / sizeof(places[0]))];
          Store(h.base, at, random.Below(3) == 0 ? uint32_t(random.Next()) : Special(random));
        }
        if (random.Below(4) == 0) {
          // A movement under the denormal range (flushed by the vector unit).
          const uint32_t at = sound + 368 + 16 * random.Below(2) + 4 * random.Below(3);
          Store(h.base, at, random.Below(2) == 0 ? 0x00000003u : 0x80000005u);
          Store(h.base, at == sound + 368 ? block + kU3Body + 208 : at, 0x00000001u);
        }
      }
      const uint32_t saved_last = guest::LoadU32(h.base, sound + 1256);
      t.r3.u64 = sound;
      t.r5.u64 = vehicle;
      t.r22.u64 = random.Next();
      t.r23.u64 = random.Next();
      t.r24.u64 = random.Next();
      t.r25.u64 = random.Next();
      t.r26.u64 = random.Next();
      t.r27.u64 = random.Next();
      t.r28.u64 = random.Next();
      t.r29.u64 = random.Next();
      t.r30.u64 = random.Next();
      t.r31.u64 = random.Next();
      t.f23.u64 = random.Next();
      t.f24.u64 = random.Next();
      t.f25.u64 = random.Next();
      t.f26.u64 = random.Next();
      t.f27.u64 = random.Next();
      t.f28.u64 = random.Next();
      t.f29.u64 = random.Next();
      t.f30.u64 = random.Next();
      t.f31.u64 = random.Next();
      for (uint32_t i = 0; i < 16; i += 8) {
        const uint64_t a = random.Next(), b = random.Next();
        std::memcpy(t.v126.u8 + i, &a, 8);
        std::memcpy(t.v127.u8 + i, &b, 8);
      }
      ITC_FLAG(native_car_sounds) = v == 1;
      (v == 0 ? __imp__sub_8210F820 : hooks::CarSoundsUpdate3D)(t, h.base);
      ITC_FLAG(native_car_sounds) = true;
      if (v == 1 && passing) {
        float d2 = 0.0f;
        for (uint32_t i = 0; i < 3; i++) {
          const float d = guest::LoadFloat(h.base, sound + 384 + 4 * i) -
                          guest::LoadFloat(h.base, sound + 368 + 4 * i);
          d2 += d * d;
        }
        within += d2 < close;
        const float last = std::bit_cast<float>(saved_last);
        fallen += d2 < close && last - guest::LoadFloat(h.base, sound + 204) > drop;
      }
      if (v == 1) {
        passed += guest::LoadU32(h.base, sound + 1260) == 1;
        at_camera_cases += guest::LoadU32(h.base, sound + 368) ==
                           guest::LoadU32(h.base, sound + 384);
      }
      std::vector<uint32_t> floats;
      for (const uint32_t o : sound_floats) {
        floats.push_back(sound - block + o);
      }
      for (uint32_t i = 0; i < 6; i++) {
        floats.push_back(kU3Matrix + 4 * i);
      }
      BlockOutputs(out[v], h.base, block, floats);
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      {
        const uint32_t region = t.r1.u32 - kU3Frame;
        std::vector<uint8_t> bytes = Read(h.base, region, kU3Frame);
        for (const uint32_t o : frame_floats) {
          for (uint32_t i = 0; i < 4; i++) {
            bytes[o + i] = 0;
          }
        }
        out[v].Bytes(bytes);
        for (const uint32_t o : frame_floats) {
          out[v].Float(guest::LoadU32(h.base, region + o));
        }
      }
      for (uint32_t g = 0; g < kGlobals; g++) {
        Write(h.base, globals[g].address, saved[g].data(), globals[g].bytes);
      }
    }
    h.random = random;
    results.Check(out[0], out[1],
                  fmt::format("{} case {} (sound +{})", realistic ? "realistic" : "degenerate", c,
                              sound - block));
  }
  ITC_LOG_INFO("harness: car_sounds: Update3D: {} cases passed close ({} of the passing "
               "cases within the distance, {} of those with the factor's fall over the drop), {} "
               "with the car at the camera's position",
               passed, within, fallen, at_camera_cases);
  results.Report();
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
  SetUps(h, block.address);
  Update3Ds(h, block.address);
}

}  // namespace harness
