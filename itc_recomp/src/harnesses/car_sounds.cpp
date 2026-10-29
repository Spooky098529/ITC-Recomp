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
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "xaudio/voices.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "models/memory_blocks.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <initializer_list>
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

// --- The voice holders ---
//
// A car sound (or one holder) at +0 of the block; ten fake voices at
// +0x800 + 0x100 k, each with its vtable at +0x1C00 (64 slots, each one of
// three leaf recorders: XAudioVoice::AddRef sub_8245F860, the count at +4
// up and in r3; sub_820CE9F8, r4 and r5 to +444 and +448; sub_820B6950, f1
// to +112 as a float), its codec (+76) another fake voice and its flags'
// pending bit (+61 bit 6) clear, so XAudio's Start and Stop stay on the
// fake objects. The holders' slots are fake voices or 0. The states
// GetState gives are the stack's random bytes (no recorder writes there),
// the same for both versions. Compared: the block (the fake voices' +112
// as floats), r3 and the 64 bytes above the stack pointer (UpdateVoices'
// output word in the caller's frame).
constexpr uint32_t kVoices = 0x800;
constexpr uint32_t kVoiceBytes = 0x100;
constexpr uint32_t kVoiceCount = 10;
constexpr uint32_t kVoiceVtable = 0x1C00;
constexpr uint32_t kVoiceRecorders[] = {0x8245F860, 0x820CE9F8, 0x820B6950};
constexpr uint32_t kSoundHolders[] = {408 + 44, 408 + 128, 952, 584, 676, 736,
                                      840,      792,       888, 1016, 1072, 1136};

uint32_t FakeVoice(uint32_t block, uint32_t k) { return block + kVoices + kVoiceBytes * k; }

void FakeVoices(uint8_t* base, uint32_t block, Random& random) {
  for (uint32_t slot = 0; slot < 0x100; slot += 4) {
    Store(base, block + kVoiceVtable + slot, kVoiceRecorders[random.Below(3)]);
  }
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    const uint32_t voice = FakeVoice(block, k);
    Store(base, voice, block + kVoiceVtable);
    Store(base, voice + 4, random.Below(3));
    Store(base, voice + 76, FakeVoice(block, random.Below(kVoiceCount)));
    guest::StoreU8(base, voice + 61, guest::LoadU8(base, voice + 61) & 0xBF);
  }
}

// A holder at `holder`: each slot a fake voice, or 0 one time in `empty`.
void FakeHolder(uint8_t* base, uint32_t block, uint32_t holder, Random& random, uint32_t empty) {
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, holder + 4 + 4 * i,
          random.Below(empty) == 0 ? 0 : FakeVoice(block, random.Below(kVoiceCount)));
    Store(base, holder + 24 + 4 * i, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
  }
}

template <typename Setup, typename After>
void HolderCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, Setup setup, After after, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
  }
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < 0x400; i += 4) {
      Store(h.base, t.r1.u32 - 0x400 + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < 64; i += 4) {
      Store(h.base, t.r1.u32 + i, uint32_t(random.Next()));
    }
    FakeVoices(h.base, block, random);
    setup(random, t);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    after(out[v], t);
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Word(t.r3.u32);
    out[v].Bytes(Read(h.base, t.r1.u32, 64));
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

template <typename Setup>
void HolderCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, Setup setup, const std::string& what) {
  HolderCase(h, results, block, recompiled, native, setup, [](Outputs&, PPCContext&) {}, what);
}

// A pitch: the 0.0 the code tests for, specials, the game's range and wide.
double Pitch(Random& random) {
  switch (random.Below(6)) {
    case 0:
      return 0.0;
    case 1:
      return double(std::bit_cast<float>(Special(random)));
    case 2:
      return double(Uniform(random, -4.0f, 4.0f));
    case 3:
      return double(Uniform(random, 0.0f, 2.0f));
    default:
      return double(int32_t(random.Next())) / 65536.0;
  }
}

void Holders(Context& h, uint32_t block) {
  const uint32_t holder = block + 0x100;
  {
    Results results("CarSound::SetPitch (sub_8210D158)");
    for (int c = 0; c < 200; c++) {
      HolderCase(h, results, block, __imp__sub_8210D158, hooks::CarSoundsSetPitch,
                 [&](Random& random, PPCContext& t) {
                   FakeHolder(h.base, block, holder, random, 1000000);
                   t.r3.u64 = holder;
                   t.r4.u64 = random.Below(4);
                   t.r7.u64 = random.Below(2) == 0 ? 0 : random.Next();
                   t.f1.f64 = Pitch(random);
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::SetPitchScaled (sub_8210D080)");
    for (int c = 0; c < 300; c++) {
      HolderCase(h, results, block, __imp__sub_8210D080, hooks::CarSoundsSetPitchScaled,
                 [&](Random& random, PPCContext& t) {
                   FakeHolder(h.base, block, holder, random, 1000000);
                   t.r3.u64 = holder;
                   t.r4.u64 = random.Below(4);
                   t.r7.u64 = random.Below(2) == 0 ? 0 : random.Next();
                   t.r9.u64 = random.Below(3) == 0 ? random.Next()
                                                   : uint64_t(int64_t(random.Below(201)) - 100);
                   t.f1.f64 = Pitch(random);
                   t.f3.f64 = Pitch(random);
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::IsPlaying (sub_8210D1D8)");
    for (int c = 0; c < 120; c++) {
      HolderCase(h, results, block, __imp__sub_8210D1D8, hooks::CarSoundsIsPlaying,
                 [&](Random& random, PPCContext& t) {
                   FakeHolder(h.base, block, holder, random, 3);
                   t.r3.u64 = holder;
                   t.r4.u64 = random.Below(5) == 0 ? 0xFFFFFFFFu : random.Below(4);
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::UpdateVoices (sub_8210D378)");
    for (int c = 0; c < 120; c++) {
      HolderCase(h, results, block, __imp__sub_8210D378, hooks::CarSoundsUpdateVoices,
                 [&](Random& random, PPCContext& t) {
                   FakeHolder(h.base, block, holder, random, 3);
                   t.r3.u64 = holder;
                   t.r4.u64 = random.Next();
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  struct Walk {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool sound;  // the car sound's 12 holders, else one
  };
  const Walk walks[] = {
      {"CarSound::Clear (sub_8210D320)", __imp__sub_8210D320, hooks::CarSoundsClear, false},
      {"CarSound::Pause (sub_8210D240)", __imp__sub_8210D240, hooks::CarSoundsPause, false},
      {"CarSound::Resume (sub_8210D2C0)", __imp__sub_8210D2C0, hooks::CarSoundsResume, false},
      {"CarSound::PauseAll (sub_8210E2B8)", __imp__sub_8210E2B8, hooks::CarSoundsPauseAll, true},
      {"CarSound::ResumeAll (sub_8210E350)", __imp__sub_8210E350, hooks::CarSoundsResumeAll,
       true},
      {"CarSound::Stop (sub_8210E218)", __imp__sub_8210E218, hooks::CarSoundsStop, true},
  };
  for (const Walk& w : walks) {
    Results results(w.name);
    for (int c = 0; c < (w.sound ? 40 : 120); c++) {
      HolderCase(h, results, block, w.recompiled, w.native,
                 [&](Random& random, PPCContext& t) {
                   if (w.sound) {
                     for (const uint32_t offset : kSoundHolders) {
                       FakeHolder(h.base, block, block + offset, random, 3);
                     }
                     t.r3.u64 = block;
                   } else {
                     FakeHolder(h.base, block, holder, random, 3);
                     t.r3.u64 = holder;
                   }
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
}

// CarSound::Release on fake voices, its matrix (+176) a block from operator
// new[] (freed by the case); the matrix's word and r3 (the block) compared
// as whether they're the block.
void Releases(Context& h, uint32_t block) {
  Results results("CarSound::Release (sub_8210E170)");
  for (int c = 0; c < 40; c++) {
    uint32_t matrix = 0;
    HolderCase(
        h, results, block, __imp__sub_8210E170, hooks::CarSoundsRelease,
        [&](Random& random, PPCContext& t) {
          for (const uint32_t offset : kSoundHolders) {
            FakeHolder(h.base, block, block + offset, random, 3);
          }
          matrix = memory::crt::OperatorNewArray(h.base, 24).value_or(0);
          Store(h.base, block + 176, matrix);
          t.r3.u64 = block;
        },
        [&](Outputs& out, PPCContext& t) {
          out.Word(matrix != 0);
          out.Word(guest::LoadU32(h.base, block + 176) == matrix);
          out.Word(t.r3.u32 == matrix);
          Store(h.base, block + 176, 0);
          t.r3.u64 = 0;
        },
        fmt::format("case {}", c));
  }
  results.Report();
}

// StopAll and CarSounds::Release over seven car sounds in a block from
// operator new[] (the count 7 at +0, the sounds from +16) at [0x826C5C3C]
// (put back after), their holders fake voices in the block; for Release
// each sound's matrix a block from operator new[] and the submix [0x826C5C44]
// a fake voice. Compared: the block, r3 and for StopAll the sounds' bytes
// (Release frees them: their matrices compared as whether they were the
// blocks given).
void AllSounds(Context& h, uint32_t block) {
  const uint32_t saved_sounds = guest::LoadU32(h.base, cs::kSounds);
  const uint32_t saved_submix = guest::LoadU32(h.base, cs::kSubmix);
  constexpr uint32_t kArrayBytes = 16 + 7 * cs::kCarSoundBytes;
  for (const bool release : {false, true}) {
    Results results(release ? "CarSounds::Release (sub_8210F750)"
                            : "CarSounds::StopAll (sub_8210F7C8)");
    for (int c = 0; c < 12; c++) {
      uint32_t array = 0;
      uint32_t matrices[7] = {};
      HolderCase(
          h, results, block, release ? __imp__sub_8210F750 : __imp__sub_8210F7C8,
          release ? hooks::CarSoundsReleaseAll : hooks::CarSoundsStopAll,
          [&](Random& random, PPCContext&) {
            array = memory::crt::OperatorNewArray(h.base, kArrayBytes).value_or(0);
            if (array == 0) {
              return;
            }
            for (uint32_t i = 0; i < kArrayBytes; i += 4) {
              Store(h.base, array + i, uint32_t(random.Next()));
            }
            Store(h.base, array, 7);
            for (uint32_t k = 0; k < 7; k++) {
              const uint32_t sound = array + 16 + cs::kCarSoundBytes * k;
              for (const uint32_t offset : kSoundHolders) {
                FakeHolder(h.base, block, sound + offset, random, 3);
              }
              if (release) {
                matrices[k] = memory::crt::OperatorNewArray(h.base, 24).value_or(0);
                Store(h.base, sound + 176, matrices[k]);
              }
            }
            Store(h.base, cs::kSounds, array + 16);
            Store(h.base, cs::kSubmix, FakeVoice(block, random.Below(kVoiceCount)));
          },
          [&](Outputs& out, PPCContext&) {
            out.Word(array != 0);
            if (array != 0 && !release) {
              out.Bytes(Read(h.base, array + 16, 7 * cs::kCarSoundBytes));
              memory::crt::OperatorDeleteArray(array);
            }
            for (uint32_t k = 0; k < 7; k++) {
              out.Word(matrices[k] != 0);
            }
            Store(h.base, cs::kSounds, saved_sounds);
            Store(h.base, cs::kSubmix, saved_submix);
          },
          fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- Making and starting real voices ---
//
// XAudio's own voices, made by the engine for each version: the submix
// word [0x826C5C44] 0 while a case runs (both outputs then the mastering
// voice), the mixer held out by its lock (0x8264D4F8, as the mixer's
// callback takes it) so nothing plays them, and every voice made stopped
// (XAudio's Stop at once), taken back out of the engine's source list and
// freed with its allocator's block before the next version. A wave bank in
// the block (the view at +0x1800, the bank data at +0x1840, the entries at
// +0x1900, the wave data at +0x1A00) with PCM formats XAudio takes (1 or 2
// channels, 22,050 to 48,000 Hz, 8 or 16 bits). Compared: the block and
// each made voice's allocator block with pointers into those blocks as
// offsets (the voices' order), the frame's descriptions for AddVoice, r3;
// the mastering voice's count and +40 put back.
constexpr uint32_t kMixLock = 0x8264D4F8;
constexpr uint32_t kRealView = 0x1800;
constexpr uint32_t kWaveBankData = 0x1840;
constexpr uint32_t kWaveEntries = 0x1900;
constexpr uint32_t kWaveData = 0x1A00;
constexpr uint32_t kTaken = 0x0BAD0000;  // a slot's word for a voice already there

uint32_t PcmFormat(Random& random) {
  static constexpr uint32_t kRates[] = {22050, 24000, 32000, 44100, 48000};
  const uint32_t channels = 1 + random.Below(2);
  const uint32_t rate = kRates[random.Below(5)];
  return (random.Below(2) == 0 ? 0x80000000u : 0) | (rate << 5) | (channels << 2);
}

void WaveBank(uint8_t* base, uint32_t block, Random& random) {
  const uint32_t view = block + kRealView, data = block + kWaveBankData;
  const uint32_t waves = 2 + random.Below(5);
  Store(base, view + 0, block);
  Store(base, view + 12, data);
  Store(base, view + 16, block + kWaveEntries);
  Store(base, view + 20, block + kWaveData);
  uint32_t flags = uint32_t(random.Next());
  flags = random.Below(2) == 0 ? (flags & ~0x20000u) : (flags | 0x20000u);
  Store(base, data + 0, flags);
  Store(base, data + 4, waves);
  Store(base, data + 84, PcmFormat(random));
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t entry = block + kWaveEntries + 24 * i;
    Store(base, entry + 4, PcmFormat(random));
    Store(base, entry + 8, 4 * random.Below(64));
    Store(base, entry + 12, 4 * (1 + random.Below(64)));
    Store(base, entry + 16, random.Below(32));
    Store(base, entry + 20, random.Below(32));
  }
}

struct RealVoice {
  uint32_t voice, allocator, bytes;
};
// Voices the native versions made, and how many of them played.
uint32_t real_made = 0;
uint32_t real_playing = 0;

// The voices made in the holders' slots (not 0, not taken), in order.
std::vector<RealVoice> MadeVoices(uint8_t* base, const std::vector<uint32_t>& holders) {
  std::vector<RealVoice> made;
  for (const uint32_t holder : holders) {
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t voice = guest::LoadU32(base, holder + 4 + 4 * i);
      if (voice == 0 || (voice & 0xFFFF0000u) == kTaken) {
        continue;
      }
      const uint32_t allocator = guest::LoadU32(base, voice + 8);
      made.push_back({voice, allocator, guest::LoadU32(base, allocator + 16) + 28});
    }
  }
  return made;
}

// The words of [address, address + bytes) with those pointing into a made
// voice's allocator block as (its index, the offset), the rest as they are.
void Relative(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
              const std::vector<RealVoice>& made) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    for (uint32_t k = 0; k < made.size(); k++) {
      if (word >= made[k].allocator && word < made[k].allocator + made[k].bytes) {
        const uint32_t value = 0xA0000000u | (k << 20) | (word - made[k].allocator);
        for (uint32_t b = 0; b < 4; b++) {
          words[i + b] = uint8_t(value >> (24 - 8 * b));
        }
        break;
      }
    }
  }
  out.Bytes(words);
}

uint32_t MasteringVoice(uint8_t* base) {
  return guest::LoadU32(base, guest::LoadU32(base, xaudio::kEngine) + 64);
}

void CriticalSection(const Context& h, uint32_t section, bool enter) {
  PPCContext c = CallContext(h);
  c.r3.u64 = section;
  if (enter) {
    __imp__RtlEnterCriticalSection(c, h.base);
  } else {
    __imp__RtlLeaveCriticalSection(c, h.base);
  }
}

// One real case: `setup(random, t)` writes the inputs (the holders listed
// in `holders`); `frame_bytes` > 0 compares the frame's +80 to +400.
template <typename Setup>
void RealCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
              GuestFunction native, const std::vector<uint32_t>& holders, uint32_t frame_bytes,
              Setup setup, const std::string& what) {
  const uint32_t engine = guest::LoadU32(h.base, xaudio::kEngine);
  const uint32_t source_offset = guest::LoadU32(h.base, engine + 88);
  const uint32_t saved_submix = guest::LoadU32(h.base, cs::kSubmix);
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < 0x1000; i += 4) {
      Store(h.base, t.r1.u32 - 0x1000 + i, uint32_t(random.Next()));
    }
    WaveBank(h.base, block, random);
    setup(random, t);
    Store(h.base, cs::kSubmix, 0);
    CriticalSection(h, kMixLock, true);
    const uint32_t mastering = MasteringVoice(h.base);
    const uint32_t count = guest::LoadU32(h.base, mastering + 4);
    const uint32_t active = guest::LoadU32(h.base, mastering + 40);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    const std::vector<RealVoice> made = MadeVoices(h.base, holders);
    if (v == 1) {
      real_made += uint32_t(made.size());
      for (const RealVoice& m : made) {
        real_playing += guest::LoadU8(h.base, m.voice + 61) & 1;
      }
    }
    out[v].Word(uint32_t(made.size()));
    out[v].Word(t.r3.u32);
    Relative(out[v], h.base, block, kBlockBytes, made);
    for (const RealVoice& m : made) {
      Relative(out[v], h.base, m.allocator, m.bytes, made);
    }
    if (frame_bytes != 0) {
      Relative(out[v], h.base, t.r1.u32 - frame_bytes + 80, 320, made);
    }
    for (const RealVoice& m : made) {
      PPCContext stop = CallContext(h);
      stop.r3.u64 = m.voice;
      stop.r4.u64 = 1;
      hooks::XAudioApiStop(stop, h.base);
      PPCContext lock = CallContext(h);
      hooks::XAudioSpinTake(lock, h.base);
      const uint32_t node = m.voice + source_offset;
      const uint32_t prev = guest::LoadU32(h.base, node + 4);
      const uint32_t next = guest::LoadU32(h.base, node);
      out[v].Word(guest::LoadU32(h.base, prev) == node && guest::LoadU32(h.base, next + 4) == node);
      guest::StoreU32(h.base, prev, next);
      guest::StoreU32(h.base, next + 4, prev);
      hooks::XAudioSpinRelease(lock, h.base);
      memory::crt::XMemFree(m.allocator, 0x61820006);
    }
    out[v].Word(guest::LoadU32(h.base, mastering + 4) - count);
    out[v].Word(guest::LoadU32(h.base, mastering + 40) - active);
    Store(h.base, mastering + 4, count);
    Store(h.base, mastering + 40, active);
    CriticalSection(h, kMixLock, false);
    Store(h.base, cs::kSubmix, saved_submix);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A holder's slots: each free (0) or taken, at least one free `free` times
// in four.
void RealHolder(uint8_t* base, uint32_t holder, Random& random) {
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, holder + 4 + 4 * i, random.Below(3) == 0 ? kTaken + i : 0);
  }
  Store(base, holder + 20, random.Below(4));
}

void RealVoices(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xaudio::kEngine);
  if (engine == 0 || MasteringVoice(h.base) == 0 || !hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: car_sounds: no engine or native heap: the real voices' cases skipped");
    return;
  }
  const uint32_t holder = block + 0x100;
  {
    Results results("CarSound::AddVoice (sub_8210CE38)");
    for (int c = 0; c < 48; c++) {
      RealCase(h, results, block, __imp__sub_8210CE38, hooks::CarSoundsAddVoice, {holder}, 496,
               [&](Random& random, PPCContext& t) {
                 RealHolder(h.base, holder, random);
                 if (random.Below(8) == 0) {
                   for (uint32_t i = 0; i < 4; i++) {
                     Store(h.base, holder + 4 + 4 * i, kTaken + i);
                   }
                 }
                 t.r3.u64 = holder;
                 t.r4.u64 = block + kRealView;
                 t.r5.u64 = random.Below(guest::LoadU32(h.base, block + kWaveBankData + 4));
                 t.r6.u64 = random.Below(2) == 0 ? 0 : random.Next();
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::PlayOnce (sub_8210E3E8)");
    for (int c = 0; c < 48; c++) {
      RealCase(h, results, block, __imp__sub_8210E3E8, hooks::CarSoundsPlayOnce, {holder}, 0,
               [&](Random& random, PPCContext& t) {
                 RealHolder(h.base, holder, random);
                 t.r3.u64 = holder;
                 t.r4.u64 = block + kRealView;
                 t.r5.u64 = random.Below(guest::LoadU32(h.base, block + kWaveBankData + 4));
                 t.r8.u64 = random.Below(2);
                 t.f1.f64 = double(Uniform(random, 0.0f, 1.5f));
                 t.f2.f64 = random.Below(4) == 0 ? 0.0 : double(Uniform(random, 0.5f, 2.0f));
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("EngineLayer::Start (sub_8210D410)");
    for (int c = 0; c < 16; c++) {
      RealCase(h, results, block, __imp__sub_8210D410, hooks::CarSoundsEngineLayerStart,
               {holder + 44}, 0,
               [&](Random& random, PPCContext& t) {
                 RealHolder(h.base, holder + 44, random);
                 t.r3.u64 = holder;
                 t.r4.u64 = random.Next();
                 t.r5.u64 = block + kRealView;
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The views' pair at +0x17F0 (both the bank).
    Results results("EngineSound::Start (sub_8210D6C0)");
    for (int c = 0; c < 12; c++) {
      RealCase(h, results, block, __imp__sub_8210D6C0, hooks::CarSoundsEngineSoundStart,
               {holder + 44, holder + 84 + 44}, 0,
               [&](Random& random, PPCContext& t) {
                 RealHolder(h.base, holder + 44, random);
                 RealHolder(h.base, holder + 84 + 44, random);
                 Store(h.base, block + 0x17F0, block + kRealView);
                 Store(h.base, block + 0x17F4, block + kRealView);
                 t.r3.u64 = holder;
                 t.r4.u64 = random.Below(7);
                 t.r5.u64 = block + 0x17F0;
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("TireSound::Start (sub_8210D7F8)");
    for (int c = 0; c < 16; c++) {
      RealCase(h, results, block, __imp__sub_8210D7F8, hooks::CarSoundsTireSoundStart,
               {holder + 24}, 0,
               [&](Random& random, PPCContext& t) {
                 RealHolder(h.base, holder + 24, random);
                 t.r3.u64 = holder;
                 t.r4.u64 = random.Next();
                 t.r5.u64 = block + kRealView;
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // The car sound at +0 (its holders up to +1180), the views' pair at
    // +0x17F0.
    Results results("CarSound::Start (sub_8210E030)");
    const std::vector<uint32_t> holders = {block + 408 + 44, block + 408 + 128, block + 652 + 24,
                                           block + 792};
    for (int c = 0; c < 8; c++) {
      RealCase(h, results, block, __imp__sub_8210E030, hooks::CarSoundsCarSoundStart, holders, 0,
               [&](Random& random, PPCContext& t) {
                 for (const uint32_t at : holders) {
                   RealHolder(h.base, at, random);
                 }
                 Store(h.base, block + 0x17F0, block + kRealView);
                 Store(h.base, block + 0x17F4, block + kRealView);
                 t.r3.u64 = block;
                 t.r4.u64 = block + 0x17F0;
                 t.r5.u64 = block + kRealView;
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  ITC_LOG_INFO("harness: car_sounds: the real voices' cases made {} voices ({} playing)", real_made,
               real_playing);
  {
    // CarSounds::Init with its globals put back after: the array, its
    // matrices and the submix voice it makes compared (the matrices' words
    // as whether they're set, the submix's allocator block with pointers
    // into it as offsets), with its frame's descriptions; then the submix
    // released (XAudio's Release, as CarSounds::Release does) and the
    // blocks freed.
    Results results("CarSounds::Init (sub_8210FE00)");
    const uint32_t saved[3] = {guest::LoadU32(h.base, cs::kSounds),
                               guest::LoadU32(h.base, 0x826C5C40),
                               guest::LoadU32(h.base, cs::kSubmix)};
    for (int c = 0; c < 4; c++) {
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        PPCContext t = CallContext(h);
        for (uint32_t i = 0; i < 0x1000; i += 4) {
          Store(h.base, t.r1.u32 - 0x1000 + i, uint32_t(random.Next()));
        }
        Store(h.base, 0x826C5C40, uint32_t(random.Next()));
        Store(h.base, cs::kSubmix, 0);
        CriticalSection(h, kMixLock, true);
        const uint32_t mastering = MasteringVoice(h.base);
        const uint32_t count = guest::LoadU32(h.base, mastering + 4);
        const uint32_t active = guest::LoadU32(h.base, mastering + 40);
        ITC_FLAG(native_car_sounds) = v == 1;
        (v == 0 ? __imp__sub_8210FE00 : hooks::CarSoundsInit)(t, h.base);
        ITC_FLAG(native_car_sounds) = true;
        const uint32_t array = guest::LoadU32(h.base, cs::kSounds);
        const uint32_t submix = guest::LoadU32(h.base, cs::kSubmix);
        std::vector<RealVoice> made;
        if (submix != 0) {
          const uint32_t allocator = guest::LoadU32(h.base, submix + 8);
          made.push_back({submix, allocator, guest::LoadU32(h.base, allocator + 16) + 28});
        }
        out[v].Word(array != 0);
        out[v].Word(uint32_t(made.size()));
        out[v].Word(t.r3.u32);
        out[v].Word(guest::LoadU32(h.base, 0x826C5C40));
        uint32_t matrices[7] = {};
        if (array != 0) {
          out[v].Word(guest::LoadU32(h.base, array - 16));
          for (uint32_t k = 0; k < 7; k++) {
            const uint32_t sound = array + cs::kCarSoundBytes * k;
            matrices[k] = guest::LoadU32(h.base, sound + 176);
            out[v].Word(matrices[k] != 0);
            Store(h.base, sound + 176, 0);
            if (matrices[k] != 0) {
              out[v].Bytes(Read(h.base, matrices[k], 24));
            }
          }
          Relative(out[v], h.base, array, 7 * cs::kCarSoundBytes, made);
        }
        for (const RealVoice& m : made) {
          Relative(out[v], h.base, m.allocator, m.bytes, made);
        }
        Relative(out[v], h.base, t.r1.u32 - 368 + 80, 184, made);
        if (submix != 0) {
          PPCContext release = CallContext(h);
          release.r3.u64 = submix;
          hooks::XAudioApiRelease(release, h.base);
          out[v].Word(release.r3.u32);
        }
        out[v].Word(guest::LoadU32(h.base, mastering + 4) - count);
        out[v].Word(guest::LoadU32(h.base, mastering + 40) - active);
        Store(h.base, mastering + 4, count);
        Store(h.base, mastering + 40, active);
        CriticalSection(h, kMixLock, false);
        for (const uint32_t matrix : matrices) {
          if (matrix != 0) {
            memory::crt::OperatorDeleteArray(matrix);
          }
        }
        if (array != 0) {
          memory::crt::OperatorDeleteArray(array - 16);
        }
        Store(h.base, cs::kSounds, saved[0]);
        Store(h.base, 0x826C5C40, saved[1]);
        Store(h.base, cs::kSubmix, saved[2]);
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
}

// --- The parts' updates ---
//
// HitSounds::Update and its three parts, the parts at +736, +840 and +888,
// WindSound::Update and PassBySound::Update on a part at +0x100 of the block
// whose slots hold the fake voices of "The voice holders" (their vtable at
// +0x1C00, its GetState kFakeGetState) or 0, a wave bank in the block with enough waves for every wave
// they play (the view at +0x1800, the bank data at +0x1840, the entries at
// +0x1900, the wave data at +0x1E00) and a vehicle in a block of its own
// (random words, the fields they read set to cover their branches: the
// collision object +340 at its +0x3000 with up to three contacts from
// +0x3100, the car's specs +5712 at +0x2000, the camera pass
// [[0x826C2CE8]] at +0x3400, put back after). The waves they play are real
// voices made by the engine as in "Making and starting real voices" (the
// mixer held out, the submix 0, each made voice stopped, taken out and
// freed before the next version). The output (r5) a valid one in the block
// (+0x300: two outputs, each six channels), as the car sound's +352.
// Compared: the block (the fake voices' +112 and +140 and the part's +56
// and +60 as floats) and the vehicle with pointers into the made voices'
// blocks as offsets, those blocks, the frame up to the original's register
// saves (`saves` bytes below the stack pointer, which the plain C++ doesn't
// write) and the 64 bytes above the stack pointer, r3 (as an offset when it points into a made voice), the
// mastering voice's count and +40.
constexpr uint32_t kPart = 0x100;
constexpr uint32_t kOutput = 0x300;
constexpr uint32_t kPartWaveData = 0x1E00;
constexpr uint32_t kVehicleBytes = 0x4000;
constexpr uint32_t kCameraPass = 0x826C2CE8;
// The fake voices' GetState (slot +48) here: XAudioSubmixVoice::GetState, a
// leaf writing the voice's flags byte (+61) as the state, as a real voice's
// GetState always writes one (the leaf recorders write none, and the state
// would be a stack word the originals' register saves land on).
constexpr uint32_t kFakeGetState = 0x8245F218;

struct VehicleBlock {
  VehicleBlock() : address(memory::Alloc(kVehicleBytes, 0)) {}
  ~VehicleBlock() { memory::Free(address); }
  VehicleBlock(const VehicleBlock&) = delete;
  VehicleBlock& operator=(const VehicleBlock&) = delete;
  uint32_t address;
};

// A bank of `waves` waves (at most 32) with PCM formats XAudio takes.
void PartWaveBank(uint8_t* base, uint32_t block, uint32_t waves, Random& random) {
  const uint32_t view = block + kRealView, data = block + kWaveBankData;
  Store(base, view + 0, block);
  Store(base, view + 12, data);
  Store(base, view + 16, block + kWaveEntries);
  Store(base, view + 20, block + kPartWaveData);
  uint32_t flags = uint32_t(random.Next());
  flags = random.Below(2) == 0 ? (flags & ~0x20000u) : (flags | 0x20000u);
  Store(base, data + 0, flags);
  Store(base, data + 4, waves);
  Store(base, data + 84, PcmFormat(random));
  for (uint32_t i = 0; i < waves; i++) {
    const uint32_t entry = block + kWaveEntries + 24 * i;
    Store(base, entry + 4, PcmFormat(random));
    Store(base, entry + 8, 4 * random.Below(16));
    Store(base, entry + 12, 4 * (1 + random.Below(16)));
    Store(base, entry + 16, random.Below(16));
    Store(base, entry + 20, random.Below(16));
  }
}

// The output at +0x300 (the car sound's +352: the count 2 and the entries;
// each entry an output's index and a six-channel map) as the 64-bit word.
uint64_t PartOutput(uint8_t* base, uint32_t block, Random& random) {
  const uint32_t at = block + kOutput;
  for (uint32_t e = 0; e < 2; e++) {
    const uint32_t entry = at + 16 + 8 * e, map = at + 32 + 8 * e, pairs = at + 48 + 48 * e;
    guest::StoreU8(base, entry, uint8_t(e));
    Store(base, entry + 4, map);
    guest::StoreU8(base, map, 6);
    Store(base, map + 4, pairs);
    for (uint32_t i = 0; i < 6; i++) {
      guest::StoreU8(base, pairs + 8 * i, uint8_t(i));
      Store(base, pairs + 8 * i + 4, FloatBits(Uniform(random, 0.0f, 1.0f)));
    }
  }
  const uint32_t high = 0x02000000u | (uint32_t(random.Next()) & 0xFFFFFFu);
  return (uint64_t(high) << 32) | (at + 16);
}

// A float near one of `around` (or one of them exactly, 0, a special, or
// spread over six orders of magnitude).
float Near(Random& random, std::initializer_list<float> around) {
  const float pick = around.begin()[random.Below(uint32_t(around.size()))];
  switch (random.Below(8)) {
    case 0:
      return pick;
    case 1:
      return 0.0f;
    case 2:
      return std::bit_cast<float>(Special(random));
    case 3:
      return std::pow(10.0f, Uniform(random, -3.0f, 3.0f));
    case 4:
      return -Uniform(random, 0.0f, 2.0f) * pick;
    default:
      return pick * Uniform(random, 0.5f, 2.0f);
  }
}

float Constant(uint8_t* base, uint32_t address) { return guest::LoadFloat(base, address); }

// The vehicle's fields the parts read.
void PartVehicle(uint8_t* base, uint32_t vehicle, Random& random) {
  const float high = Constant(base, 0x82002A0C), low = Constant(base, 0x82001FC0);
  const float scrape = Constant(base, 0x82001FD8);
  const float speed = random.Below(10) == 0 ? std::bit_cast<float>(Special(random))
                                            : Uniform(random, -5.0f, 90.0f);
  Store(base, vehicle + 48, FloatBits(speed));
  Store(base, vehicle + 6152, (uint32_t(random.Next()) & ~7u) | random.Below(8));
  // The collision object and its contacts (flags of either mask or both).
  const uint32_t object = vehicle + 0x3000;
  Store(base, vehicle + 340, random.Below(6) == 0 ? 0 : object);
  const uint32_t contacts = random.Below(4);
  Store(base, object + 240, contacts == 0 ? 0 : vehicle + 0x3100);
  // The scrape's amount is speed (km/h) x strength x [0x8200805C]: a
  // strength for an amount near its thresholds, now and then.
  const float kmh = speed * 3.6f;
  const float per = kmh * Constant(base, 0x8200805C);
  for (uint32_t k = 0; k < contacts; k++) {
    const uint32_t contact = vehicle + 0x3100 + 80 * k;
    static constexpr uint32_t kFlags[] = {0x00000001, 0x00010000, 0x01000000,
                                          0x80000000, 0x01000001, 0};
    Store(base, contact + 4, kFlags[random.Below(6)]);
    Store(base, contact + 68, k + 1 < contacts ? contact + 80 : 0);
    float strength = Near(random, {high, low, high * 2.0f});
    if (random.Below(2) == 0 && per != 0.0f && std::isfinite(per)) {
      strength = Near(random, {scrape, low, high}) / per;
    }
    Store(base, contact + 64, FloatBits(strength));
  }
  Store(base, vehicle + 2252, random.Below(3) == 0 ? 0 : uint32_t(random.Next()));
  guest::StoreU8(base, vehicle + 2948, random.Below(2) == 0 ? 0 : uint8_t(1 + random.Below(255)));
  Store(base, vehicle + 2952, FloatBits(random.Below(8) == 0 ? std::bit_cast<float>(Special(random))
                                                              : Uniform(random, 0.0f, 2.0f)));
  const uint32_t specs = vehicle + 0x2000;
  Store(base, vehicle + 5712, specs);
  guest::StoreU8(base, specs + 2019, uint8_t(random.Below(3)));
  Store(base, specs + 1656, random.Below(8) == 0 ? uint32_t(random.Next()) : random.Below(7));
  guest::StoreU8(base, vehicle + 2504, uint8_t(random.Below(2)));
  guest::StoreU8(base, vehicle + 2960, uint8_t(random.Below(2)));
  guest::StoreU8(base, vehicle + 2424, uint8_t(random.Below(2)));
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t at = vehicle + 5410 + 96 * i;
    const uint16_t half = uint16_t(random.Next());
    guest::StoreU16(base, at, random.Below(5) == 0 ? (half | 1) : (half & ~1));
  }
  Store(base, vehicle + 6148,
        random.Below(2) == 0 ? (uint32_t(random.Next()) & ~0x81u) : uint32_t(random.Next()));
  // The camera pass: [0x826C2CE8] at +0x3300, its +0 the object at +0x3400.
  Store(base, vehicle + 0x3300, vehicle + 0x3400);
  Store(base, vehicle + 0x3400 + 1860, random.Below(3) == 0 ? uint32_t(random.Next())
                                                            : random.Below(3));
}

// The part's slots: each a fake voice or 0 (all four fake voices one time
// in eight), +20 a small count, +40 the view.
void PartSlots(uint8_t* base, uint32_t block, Random& random) {
  const uint32_t part = block + kPart;
  const bool full = random.Below(8) == 0;
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, part + 4 + 4 * i,
          full || random.Below(2) == 0 ? FakeVoice(block, random.Below(kVoiceCount)) : 0);
    Store(base, part + 24 + 4 * i, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
  }
  Store(base, part + 20, random.Below(4));
  Store(base, part + 40, block + kRealView);
}

// A slot (-1 one time in `none`, else 0-3).
uint32_t Slot(Random& random, uint32_t none) {
  return none != 0 && random.Below(none) == 0 ? 0xFFFFFFFFu : random.Below(4);
}

// A counter: 0 most often, small values, -1 and random words.
uint32_t Counter(Random& random) {
  switch (random.Below(6)) {
    case 0:
    case 1:
      return 0;
    case 2:
      return 0xFFFFFFFFu;
    case 3:
      return uint32_t(random.Next());
    default:
      return random.Below(36);
  }
}

// The word r3 with a pointer into a made voice's block as (its index, the
// offset).
uint32_t RelativeWord(uint32_t word, const std::vector<RealVoice>& made) {
  for (uint32_t k = 0; k < made.size(); k++) {
    if (word >= made[k].allocator && word < made[k].allocator + made[k].bytes) {
      return 0xA0000000u | (k << 20) | (word - made[k].allocator);
    }
  }
  return word;
}

template <typename Setup>
void PartCase(Context& h, Results& results, uint32_t block, uint32_t vehicle,
              GuestFunction recompiled, GuestFunction native, uint32_t frame_bytes, uint32_t saves,
              Setup setup, const std::string& what) {
  const uint32_t engine = guest::LoadU32(h.base, xaudio::kEngine);
  const uint32_t source_offset = guest::LoadU32(h.base, engine + 88);
  const uint32_t saved_submix = guest::LoadU32(h.base, cs::kSubmix);
  const uint32_t saved_camera = guest::LoadU32(h.base, kCameraPass);
  const uint32_t part = block + kPart;
  // Every wave they play: up to 18, and 7 + the hits' level.
  const float level_max = Constant(h.base, 0x820021E8);
  const uint32_t waves = std::clamp(uint32_t(std::isfinite(level_max) && level_max > 0.0f
                                                 ? 9.0f + level_max
                                                 : 9.0f),
                                    19u, 32u);
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  floats.push_back(kPart + 56);
  floats.push_back(kPart + 60);
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < kVehicleBytes; i += 4) {
      Store(h.base, vehicle + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < 0x1000; i += 4) {
      Store(h.base, t.r1.u32 - 0x1000 + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < 64; i += 4) {
      Store(h.base, t.r1.u32 + i, uint32_t(random.Next()));
    }
    PartWaveBank(h.base, block, waves, random);
    FakeVoices(h.base, block, random);
    Store(h.base, block + kVoiceVtable + 48, kFakeGetState);
    PartVehicle(h.base, vehicle, random);
    PartSlots(h.base, block, random);
    t.r3.u64 = part;
    t.r4.u64 = vehicle;
    t.r5.u64 = PartOutput(h.base, block, random);
    setup(random, t);
    Store(h.base, kCameraPass, vehicle + 0x3300);
    Store(h.base, cs::kSubmix, 0);
    CriticalSection(h, kMixLock, true);
    const uint32_t mastering = MasteringVoice(h.base);
    const uint32_t count = guest::LoadU32(h.base, mastering + 4);
    const uint32_t active = guest::LoadU32(h.base, mastering + 40);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    // The voices made (not the fake ones in the block).
    std::vector<RealVoice> made;
    for (uint32_t i = 0; i < 4; i++) {
      const uint32_t voice = guest::LoadU32(h.base, part + 4 + 4 * i);
      if (voice == 0 || (voice >= block && voice < block + kBlockBytes)) {
        continue;
      }
      const uint32_t allocator = guest::LoadU32(h.base, voice + 8);
      made.push_back({voice, allocator, guest::LoadU32(h.base, allocator + 16) + 28});
    }
    if (v == 1) {
      real_made += uint32_t(made.size());
    }
    out[v].Word(uint32_t(made.size()));
    out[v].Word(RelativeWord(t.r3.u32, made));
    {
      Outputs words;
      std::vector<uint8_t> bytes = Read(h.base, block, kBlockBytes);
      for (uint32_t i = 0; i + 4 <= kBlockBytes; i += 4) {
        const uint32_t word = RelativeWord(guest::LoadU32(h.base, block + i), made);
        for (uint32_t b = 0; b < 4; b++) {
          bytes[i + b] = uint8_t(word >> (24 - 8 * b));
        }
      }
      for (const uint32_t offset : floats) {
        for (uint32_t b = 0; b < 4; b++) {
          bytes[offset + b] = 0;
        }
      }
      out[v].Bytes(bytes);
      for (const uint32_t offset : floats) {
        out[v].Float(guest::LoadU32(h.base, block + offset));
      }
    }
    Relative(out[v], h.base, vehicle, kVehicleBytes, made);
    for (const RealVoice& m : made) {
      Relative(out[v], h.base, m.allocator, m.bytes, made);
    }
    Relative(out[v], h.base, t.r1.u32 - frame_bytes, frame_bytes - saves, made);
    Relative(out[v], h.base, t.r1.u32, 64, made);
    for (const RealVoice& m : made) {
      PPCContext stop = CallContext(h);
      stop.r3.u64 = m.voice;
      stop.r4.u64 = 1;
      hooks::XAudioApiStop(stop, h.base);
      PPCContext lock = CallContext(h);
      hooks::XAudioSpinTake(lock, h.base);
      const uint32_t node = m.voice + source_offset;
      const uint32_t prev = guest::LoadU32(h.base, node + 4);
      const uint32_t next = guest::LoadU32(h.base, node);
      out[v].Word(guest::LoadU32(h.base, prev) == node && guest::LoadU32(h.base, next + 4) == node);
      guest::StoreU32(h.base, prev, next);
      guest::StoreU32(h.base, next + 4, prev);
      hooks::XAudioSpinRelease(lock, h.base);
      memory::crt::XMemFree(m.allocator, 0x61820006);
    }
    out[v].Word(guest::LoadU32(h.base, mastering + 4) - count);
    out[v].Word(guest::LoadU32(h.base, mastering + 40) - active);
    Store(h.base, mastering + 4, count);
    Store(h.base, mastering + 40, active);
    CriticalSection(h, kMixLock, false);
    Store(h.base, cs::kSubmix, saved_submix);
    Store(h.base, kCameraPass, saved_camera);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

void Parts(Context& h, uint32_t block) {
  const uint32_t engine = guest::LoadU32(h.base, xaudio::kEngine);
  if (engine == 0 || MasteringVoice(h.base) == 0 || !hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: car_sounds: no engine or native heap: the parts' cases skipped");
    return;
  }
  VehicleBlock vehicle_block;
  if (!vehicle_block.address) {
    ITC_LOG_ERROR("harness: car_sounds: no vehicle block from the allocation layer");
    return;
  }
  const uint32_t vehicle = vehicle_block.address;
  const uint32_t part = block + kPart;
  const uint32_t made_before = real_made;
  const float rest = Constant(h.base, 0x82001BE0);
  ITC_LOG_INFO("harness: car_sounds: the parts' constants: level max {}, hit {} and {}, scrape from "
               "{}, its scale {}, wind from {} km/h, fade rest {}",
               Constant(h.base, 0x820021E8), Constant(h.base, 0x82002A0C),
               Constant(h.base, 0x82001FC0), Constant(h.base, 0x82001FD8),
               Constant(h.base, 0x8200805C), Constant(h.base, 0x8200257C), rest);
  // The hits' fields: +48 the ticks, +52, +56 the scrape's slot, +60, +64.
  const auto hits = [&](Random& random, PPCContext&) {
    Store(h.base, part + 48, random.Below(3) == 0 ? Counter(random) : 0);
    Store(h.base, part + 52, Counter(random));
    Store(h.base, part + 56, Slot(random, 3));
    Store(h.base, part + 64, Counter(random));
  };
  struct Part {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    uint32_t frame_bytes;
    uint32_t saves;
    int cases;
  };
  const Part hit_parts[] = {
      {"HitSounds::Impact (sub_8210E778)", __imp__sub_8210E778, hooks::CarSoundsHitImpact, 144,
       56, 200},
      {"HitSounds::Scrape (sub_8210E938)", __imp__sub_8210E938, hooks::CarSoundsHitScrape, 192,
       104, 200},
      {"HitSounds::Wave9 (sub_8210EB90)", __imp__sub_8210EB90, hooks::CarSoundsHitWave9, 96, 16,
       40},
      {"HitSounds::Update (sub_8210FD78)", __imp__sub_8210FD78, hooks::CarSoundsHitSoundsUpdate,
       112, 32, 120},
  };
  // A third of the cases an impact (+6152 bit 0, a contact of the mask
  // 0x00FFFFFF, its strength around the thresholds [0x82002A0C] and
  // [0x82001FC0], +64 often -1), a third a scrape (bit 1 alone, a contact of
  // the mask 0xFF000000, the amount speed x strength x [0x8200805C] from
  // below its window, [0x82001FD8] to [0x82001FC0], to above it).
  const auto scraping = [&](Random& random, PPCContext& t) {
    hits(random, t);
    const uint32_t kind = random.Below(3);
    if (kind == 2) {
      return;
    }
    const float speed = Uniform(random, 10.0f, 90.0f);
    Store(h.base, vehicle + 48, FloatBits(speed));
    Store(h.base, vehicle + 340, vehicle + 0x3000);
    Store(h.base, vehicle + 0x3000 + 240, vehicle + 0x3100);
    Store(h.base, vehicle + 0x3100 + 68, 0);
    const uint32_t flags = guest::LoadU32(h.base, vehicle + 6152) & ~7u;
    const float hit = Constant(h.base, 0x82002A0C), hit_low = Constant(h.base, 0x82001FC0);
    if (kind == 0) {
      Store(h.base, vehicle + 6152, flags | 1 | (random.Below(8) & 6));
      Store(h.base, vehicle + 0x3100 + 4, 0x00000001);
      Store(h.base, vehicle + 0x3100 + 64,
            FloatBits(Uniform(random, 0.5f * hit, 2.0f * hit_low)));
      if (random.Below(2) == 0) {
        Store(h.base, part + 64, 0xFFFFFFFFu);
      }
      return;
    }
    Store(h.base, vehicle + 6152, flags | 2);
    Store(h.base, vehicle + 0x3100 + 4, 0x01000000);
    const float per = speed * 3.6f * Constant(h.base, 0x8200805C);
    const float amount = Uniform(random, 0.5f * Constant(h.base, 0x82001FD8), 2.0f * hit_low);
    Store(h.base, vehicle + 0x3100 + 64, FloatBits(amount / per));
  };
  for (const Part& p : hit_parts) {
    Results results(p.name);
    for (int c = 0; c < p.cases; c++) {
      PartCase(h, results, block, vehicle, p.recompiled, p.native, p.frame_bytes, p.saves,
               scraping, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::Part736Update (sub_8210EBF0)");
    for (int c = 0; c < 80; c++) {
      PartCase(h, results, block, vehicle, __imp__sub_8210EBF0, hooks::CarSoundsPart736Update, 112, 24,
               [&](Random& random, PPCContext&) {
                 Store(h.base, part + 48, random.Below(2) == 0 ? 0 : Counter(random));
                 Store(h.base, part + 52, Counter(random));
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CarSound::Part840Update (sub_8210ECF0)");
    static constexpr uint8_t kLeft[] = {0, 0, 1, 2, 3, 0x7F, 0x80, 0x81, 0xFF};
    for (int c = 0; c < 80; c++) {
      PartCase(h, results, block, vehicle, __imp__sub_8210ECF0, hooks::CarSoundsPart840Update, 128, 48,
               [&](Random& random, PPCContext&) {
                 guest::StoreU8(h.base, part + 45, kLeft[random.Below(9)]);
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // +52's slot always a fake voice (its volume is set without a check).
    Results results("CarSound::Part888Update (sub_8210EEE0)");
    for (int c = 0; c < 120; c++) {
      PartCase(h, results, block, vehicle, __imp__sub_8210EEE0, hooks::CarSoundsPart888Update, 144, 64,
               [&](Random& random, PPCContext&) {
                 Store(h.base, part + 48, random.Below(2) == 0 ? 0 : Counter(random));
                 const uint32_t slot = Slot(random, 0);
                 Store(h.base, part + 52, slot);
                 Store(h.base, part + 4 + 4 * slot, FakeVoice(block, random.Below(kVoiceCount)));
                 const auto fade = [&] {
                   switch (random.Below(7)) {
                     case 0:
                     case 1:
                       return rest;
                     case 2:
                       return 1.0f;
                     case 3:
                       return 0.0f;
                     case 4:
                       return std::bit_cast<float>(Special(random));
                     default:
                       return Uniform(random, 0.0f, 3.0f);
                   }
                 };
                 Store(h.base, part + 56, FloatBits(fade()));
                 Store(h.base, part + 60, FloatBits(fade()));
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("WindSound::Update (sub_8210EDB0)");
    const float fast = Constant(h.base, 0x8200257C) / 3.6f;
    for (int c = 0; c < 80; c++) {
      PartCase(h, results, block, vehicle, __imp__sub_8210EDB0, hooks::CarSoundsWindSoundUpdate, 128, 40,
               [&](Random& random, PPCContext&) {
                 Store(h.base, part + 48, random.Below(4) == 0 ? Counter(random)
                                                               : random.Below(2));
                 if (random.Below(2) == 0) {
                   Store(h.base, vehicle + 48, FloatBits(Near(random, {fast})));
                 }
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PassBySound::Update (sub_8210F088)");
    for (int c = 0; c < 60; c++) {
      PartCase(h, results, block, vehicle, __imp__sub_8210F088, hooks::CarSoundsPassBySoundUpdate,
               112, 24,
               [&](Random& random, PPCContext& t) {
                 if (random.Below(6) == 0) {
                   t.r4.u64 = 0;
                 }
                 t.r6.u64 = random.Below(3) == 0 ? 0 : (random.Below(2) == 0 ? 1 : random.Next());
               },
               fmt::format("case {}", c));
    }
    results.Report();
  }
  ITC_LOG_INFO("harness: car_sounds: the parts' cases made {} voices", real_made - made_before);
}

// --- The engine ---
//
// EngineSound::Update and its two layers' updates on an engine part at
// +0x100 of the block (layer 0 at +0x100, layer 1 at +0x154), each layer's
// slots (+48 to +60) fake voices of "The voice holders" (their GetState
// kFakeGetState, so the state is the voice's flags byte, playing or not),
// each layer's table (+4) at +0x1500 and +0x1780 (+0 the volume word, then
// up to four 136-byte entries from +4: the word above -1, +4 SetPitchScaled's
// r9, two curves of eight x and y pairs at +8 and +72, ended by an x at most
// [0x82001BE0] at a random point, the x's rising by random steps, now and
// then a repeat or a special; the entries after the last a word -1 or
// below), the output at +0x300 as the parts', and a vehicle in a block of
// its own (its specs +272 at +0x2000). Compared: the block (the fake
// voices' +112 and +140, both layers' +20 to +36 and the engine's +168 and
// +172 as floats), the vehicle, r3, the last layer update's frame's locals
// (+80 to +104: the state, the fcfid word, the output word; for
// EngineSound::Update the second call's, as the original's register saves
// for it land on the first's) and the 64 bytes above the stack pointer.
constexpr uint32_t kEnginePart = 0x100;
constexpr uint32_t kLayerTables[] = {0x1500, 0x1780};
constexpr uint32_t kLayerOffsets[] = {0, 84};
constexpr uint32_t kCurveEnd = 0x82001BE0;
constexpr uint32_t kCodecVtable = 0x1A00;
constexpr uint32_t kSlotVoices = 5;

// One curve of eight points at `at`: x's rising from above the end, ended at
// a random point (the end itself, or below it).
void EngineCurve(uint8_t* base, uint32_t at, Random& random, std::vector<float>& xs) {
  const float end = Constant(base, kCurveEnd);
  const uint32_t stop = random.Below(5) == 0 ? random.Below(8) : 3 + random.Below(5);
  float x = std::isfinite(end) ? end : 0.0f;
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t point = at + 8 * i;
    float y = random.Below(10) == 0 ? std::bit_cast<float>(Special(random))
                                    : Uniform(random, -0.5f, 2.0f);
    if (i >= stop) {
      Store(base, point, FloatBits(random.Below(2) == 0 ? end : end - Uniform(random, 0.0f, 2.0f)));
      Store(base, point + 4, FloatBits(y));
      continue;
    }
    switch (random.Below(12)) {
      case 0:
        break;  // a repeat (a zero span)
      case 1:
        x = std::bit_cast<float>(Special(random));
        break;
      default:
        x = (std::isfinite(x) ? x : end) + Uniform(random, 0.01f, 0.5f);
    }
    xs.push_back(x);
    Store(base, point, FloatBits(x));
    Store(base, point + 4, FloatBits(y));
  }
}

// A layer: its table and four slots of fake voices.
void EngineLayer(uint8_t* base, uint32_t block, uint32_t layer, uint32_t table, Random& random,
                 std::vector<float>& xs) {
  Store(base, layer + 4, random.Below(30) == 0 ? 0 : table);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, layer + 48 + 4 * i, FakeVoice(block, random.Below(kSlotVoices)));
  }
  Store(base, table, random.Below(4) == 0 ? uint32_t(random.Next()) : random.Below(5));
  const uint32_t entries = random.Below(6) == 0 ? random.Below(4) : 4;
  for (uint32_t e = 0; e < 4; e++) {
    const uint32_t entry = table + 4 + 136 * e;
    Store(base, entry, random.Below(3) == 0 ? uint32_t(random.Next()) & 0x7FFFFFFFu
                                            : random.Below(3));
    Store(base, entry + 4, random.Below(3) == 0 ? uint32_t(random.Next())
                                                : uint32_t(int32_t(random.Below(201)) - 100));
    EngineCurve(base, entry + 8, random, xs);
    EngineCurve(base, entry + 72, random, xs);
  }
  const uint32_t after = table + 4 + 136 * entries;
  Store(base, after, random.Below(2) == 0 ? 0xFFFFFFFFu : (uint32_t(random.Next()) | 0x80000000u));
}

// A speed near the curves' x's (or one exactly, a special, or wide).
double EngineSpeed(uint8_t* base, Random& random, const std::vector<float>& xs) {
  const uint32_t pick = random.Below(8);
  if (pick == 0 || xs.empty()) {
    return double(std::bit_cast<float>(Special(random)));
  }
  if (pick == 1) {
    return double(Uniform(random, -1.0f, 3.0f));
  }
  if (pick == 2) {
    return double(Constant(base, 0x82007FC4)) * double(Uniform(random, 0.5f, 1.5f));
  }
  const float x = xs[random.Below(uint32_t(xs.size()))];
  return pick == 3 ? double(x) : double(x + Uniform(random, -0.3f, 0.3f));
}

// A float input: specials now and then, else in [low, high].
double EngineInput(Random& random, float low, float high) {
  return random.Below(10) == 0 ? double(std::bit_cast<float>(Special(random)))
                               : double(Uniform(random, low, high));
}

// The vehicle's fields EngineSound::Update reads.
void EngineVehicle(uint8_t* base, uint32_t vehicle, Random& random, const std::vector<float>& xs) {
  const uint32_t specs = vehicle + 0x2000;
  Store(base, vehicle + 272, specs);
  const float top = random.Below(10) == 0 ? std::bit_cast<float>(Special(random))
                                          : Uniform(random, 3000.0f, 9000.0f);
  Store(base, specs + 1280, FloatBits(top));
  const float cap = Constant(base, 0x82007FC4);
  float rpm = Uniform(random, 0.0f, 1.2f) * top;
  if (random.Below(2) == 0 && !xs.empty() && cap != 0.0f) {
    rpm = xs[random.Below(uint32_t(xs.size()))] / cap * top;
  } else if (random.Below(10) == 0) {
    rpm = std::bit_cast<float>(Special(random));
  }
  Store(base, vehicle + 2224, FloatBits(rpm));
  guest::StoreU8(base, vehicle + 2248, random.Below(3) == 0 ? uint8_t(1 + random.Below(255)) : 0);
  Store(base, vehicle + 2268, FloatBits(float(EngineInput(random, -1.0f, 1.0f))));
  Store(base, vehicle + 2256, random.Below(2) == 0 ? 0 : uint32_t(random.Next()));
  Store(base, vehicle + 2252, random.Below(2) == 0 ? 0xFFFFFFFFu : random.Below(7));
  Store(base, vehicle + 284, FloatBits(float(EngineInput(random, 0.0f, 1.0f))));
  Store(base, vehicle + 288, FloatBits(float(EngineInput(random, 0.0f, 1.0f))));
}

template <typename Setup>
void EngineCase(Context& h, Results& results, uint32_t block, uint32_t vehicle,
                GuestFunction recompiled, GuestFunction native,
                std::initializer_list<uint32_t> layer_frames, Setup setup,
                const std::string& what) {
  std::vector<uint32_t> floats;
  for (uint32_t k = 0; k < kVoiceCount; k++) {
    floats.push_back(kVoices + kVoiceBytes * k + 112);
    floats.push_back(kVoices + kVoiceBytes * k + 140);
  }
  for (const uint32_t layer : kLayerOffsets) {
    for (uint32_t i = 0; i < 5; i++) {
      floats.push_back(kEnginePart + layer + 20 + 4 * i);
    }
  }
  floats.push_back(kEnginePart + 168);
  floats.push_back(kEnginePart + 172);
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < kVehicleBytes; i += 4) {
      Store(h.base, vehicle + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 0; i < 0x1000; i += 4) {
      Store(h.base, t.r1.u32 - 0x1000 + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < 64; i += 4) {
      Store(h.base, t.r1.u32 + i, uint32_t(random.Next()));
    }
    FakeVoices(h.base, block, random);
    // The codecs (fake voices 5-9) on a copy of the recorders' vtable, so
    // Start's codec call (+48) stays a recorder; the slots' voices (0-4)
    // with kFakeGetState and one of them as their codec.
    for (uint32_t slot = 0; slot < 0x100; slot += 4) {
      Store(h.base, block + kCodecVtable + slot,
            guest::LoadU32(h.base, block + kVoiceVtable + slot));
    }
    Store(h.base, block + kVoiceVtable + 48, kFakeGetState);
    for (uint32_t k = 0; k < kVoiceCount; k++) {
      if (k >= kSlotVoices) {
        Store(h.base, FakeVoice(block, k), block + kCodecVtable);
      } else {
        Store(h.base, FakeVoice(block, k) + 76,
              FakeVoice(block, kSlotVoices + random.Below(kVoiceCount - kSlotVoices)));
      }
    }
    std::vector<float> xs;
    for (uint32_t l = 0; l < 2; l++) {
      EngineLayer(h.base, block, block + kEnginePart + kLayerOffsets[l], block + kLayerTables[l],
                  random, xs);
    }
    Store(h.base, block + kEnginePart + 168, FloatBits(float(EngineInput(random, -0.5f, 1.5f))));
    Store(h.base, block + kEnginePart + 172, FloatBits(float(EngineSpeed(h.base, random, xs))));
    EngineVehicle(h.base, vehicle, random, xs);
    const uint64_t output = PartOutput(h.base, block, random);
    setup(random, t, xs, output);
    ITC_FLAG(native_car_sounds) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_car_sounds) = true;
    BlockOutputs(out[v], h.base, block, floats);
    out[v].Bytes(Read(h.base, vehicle, kVehicleBytes));
    out[v].Word(t.r3.u32);
    for (const uint32_t below : layer_frames) {
      out[v].Bytes(Read(h.base, t.r1.u32 - below + 80, 24));
    }
    out[v].Bytes(Read(h.base, t.r1.u32, 64));
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

void Engine(Context& h, uint32_t block) {
  VehicleBlock vehicle_block;
  if (!vehicle_block.address) {
    ITC_LOG_ERROR("harness: car_sounds: no vehicle block from the allocation layer");
    return;
  }
  const uint32_t vehicle = vehicle_block.address;
  const uint32_t engine = block + kEnginePart;
  ITC_LOG_INFO("harness: car_sounds: the engine's constants: speed cap {}, curve end {}, pitched "
               "from {}, volume scale {}, gain {}, ease {}, idle step {} up to {}, rest {}",
               Constant(h.base, 0x82007FC4), Constant(h.base, kCurveEnd),
               Constant(h.base, 0x82007FE0), Constant(h.base, 0x82008028),
               Constant(h.base, 0x82007FD4), Constant(h.base, 0x82002968),
               Constant(h.base, 0x82001FD8), Constant(h.base, 0x82008048),
               Constant(h.base, 0x82003384));
  {
    Results results("EngineLayer::Update (sub_8210D478)");
    for (int c = 0; c < 300; c++) {
      EngineCase(h, results, block, vehicle, __imp__sub_8210D478, hooks::CarSoundsEngineLayerUpdate,
                 {272},
                 [&](Random& random, PPCContext& t, const std::vector<float>& xs, uint64_t output) {
                   t.r3.u64 = engine + kLayerOffsets[random.Below(2)];
                   t.r7.u64 = random.Below(8) == 0 ? random.Next() : output;
                   t.f1.f64 = EngineSpeed(h.base, random, xs);
                   t.f2.f64 = EngineInput(random, 0.0f, 2.0f);
                   t.f4.f64 = EngineInput(random, -1.0f, 2.0f);
                   t.f5.f64 = EngineInput(random, 0.0f, 2.0f);
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("EngineLayer::UpdatePitched (sub_8210D718)");
    const float threshold = Constant(h.base, 0x82007FE0), cap = Constant(h.base, 0x82007FC4);
    for (int c = 0; c < 200; c++) {
      EngineCase(h, results, block, vehicle, __imp__sub_8210D718,
                 hooks::CarSoundsEngineLayerUpdatePitched, {144 + 272},
                 [&](Random& random, PPCContext& t, const std::vector<float>& xs, uint64_t output) {
                   t.r3.u64 = engine;
                   t.r6.u64 = output;
                   t.f1.f64 = random.Below(2) == 0
                                  ? double(Near(random, {threshold, cap, (threshold + cap) / 2}))
                                  : EngineSpeed(h.base, random, xs);
                   t.f2.f64 = EngineInput(random, -0.5f, 1.5f);
                   t.f3.f64 = EngineInput(random, -1.0f, 2.0f);
                   t.f4.f64 = EngineInput(random, 0.0f, 2.0f);
                 },
                 fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("EngineSound::Update (sub_8210E470)");
    for (int c = 0; c < 200; c++) {
      EngineCase(h, results, block, vehicle, __imp__sub_8210E470, hooks::CarSoundsEngineSoundUpdate,
                 {160 + 144 + 272},
                 [&](Random& random, PPCContext& t, const std::vector<float>&, uint64_t output) {
                   t.r3.u64 = engine;
                   t.r4.u64 = random.Below(5) == 0 ? 0 : vehicle;
                   t.r5.u64 = output;
                   t.f1.f64 = EngineInput(random, -1.0f, 2.0f);
                   t.f2.f64 = EngineInput(random, 0.0f, 2.0f);
                 },
                 fmt::format("case {}", c));
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
  SetUps(h, block.address);
  Update3Ds(h, block.address);
  Holders(h, block.address);
  Releases(h, block.address);
  AllSounds(h, block.address);
  RealVoices(h, block.address);
  Parts(h, block.address);
  Engine(h, block.address);
}

}  // namespace harness
