// XAudio's factory objects (src/xaudio/objects.h,
// docs/functions/xaudio-objects.md) against the recompiled functions. The
// game's live objects aren't touched: each case builds its objects in a
// block from the allocation layer (and, for the functions that allocate, a
// real default allocator for each version), filled with the same random
// words for both versions, and runs the original or the plain C++ through
// the register steps (hooks/xaudio_objects.h). The block and r3 (all 64
// bits) are compared with harness::Outputs, exactly except where a field is
// shown to be a float.

#include "harness.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "hooks/xaudio.h"
#include "hooks/xaudio_objects.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "xaudio/objects.h"
#include "xaudio/voices.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

namespace harness {

namespace {

namespace xa = xaudio;

constexpr uint32_t kBlockBytes = 0x2000;
constexpr uint32_t kAllocatorBytes = 0x10000;
constexpr uint32_t kAddRef = 0x8245F860;  // +4 up by one, the count to r3

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

void R3(Outputs& out, uint64_t r3) {
  out.Word(uint32_t(r3));
  out.Word(uint32_t(r3 >> 32));
}

// The block and the stack below the call filled with the same random
// words for both versions, then `setup(random, t)`; the block and r3
// compared.
template <typename Setup>
void Case(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
          GuestFunction native, Setup setup, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    setup(random, t);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    R3(out[v], t.r3.u64);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// The words of [address, address + bytes) with those pointing into the
// allocator's block as offsets in it (marked), the rest as they are.
void Normalized(Outputs& out, uint8_t* base, uint32_t address, uint32_t bytes,
                uint32_t allocator, uint32_t allocator_bytes) {
  std::vector<uint8_t> words = Read(base, address, bytes);
  for (uint32_t i = 0; i + 4 <= bytes; i += 4) {
    const uint32_t word = guest::LoadU32(base, address + i);
    if (word >= allocator && word < allocator + allocator_bytes) {
      const uint32_t offset = 0xA110C000u ^ (word - allocator);
      words[i] = uint8_t(offset >> 24);
      words[i + 1] = uint8_t(offset >> 16);
      words[i + 2] = uint8_t(offset >> 8);
      words[i + 3] = uint8_t(offset);
    }
  }
  out.Bytes(words);
}

// A case over a real default allocator for each version (its data filled
// with the same pattern): `setup(random, t, allocator)`, the call, then
// `after(out, allocator)`; the block and the allocator's block compared
// with pointers into the allocator as offsets, then the allocator freed.
template <typename Setup, typename After>
void RealCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
              GuestFunction native, Setup setup, After after, const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  const uint32_t bytes = kAllocatorBytes + 28;
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    xa::DefaultAllocator(h.base, 0x61820006, kAllocatorBytes, block + 0x1FFC);
    const uint32_t allocator = guest::LoadU32(h.base, block + 0x1FFC);
    Store(h.base, block + 0x1FFC, 0);
    out[v].Word(allocator != 0);
    if (allocator == 0) {
      continue;
    }
    for (uint32_t i = 28; i < bytes; i += 4) {
      Store(h.base, allocator + i, 0x5EED0000u + i);
    }
    setup(random, t, allocator);
    (v == 0 ? recompiled : native)(t, h.base);
    R3(out[v], t.r3.u64 - ((t.r3.u64 >= allocator && t.r3.u64 < allocator + bytes) ? allocator : 0));
    after(out[v], allocator);
    Normalized(out[v], h.base, block, kBlockBytes, allocator, bytes);
    Normalized(out[v], h.base, allocator, bytes, allocator, bytes);
    memory::crt::XMemFree(allocator, 0x61820006);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A float worth testing: ordinary values most of the time, and zeros,
// denormals, infinities, NaNs and the extremes.
uint32_t AnyFloat(Random& random) {
  static constexpr uint32_t kEdges[] = {0x00000000, 0x80000000, 0x00000001, 0x807FFFFF,
                                        0x7F800000, 0xFF800000, 0x7FC00000, 0x7F800001,
                                        0xFFC00001, 0x7F7FFFFF, 0xFF7FFFFF, 0x00800000,
                                        0x42F00000, 0xC2F00000, 0x4B000000, 0x3F800000};
  switch (random.Below(4)) {
    case 0:
      return kEdges[random.Below(16)];
    case 1:
      return uint32_t(random.Next());
    default:
      return std::bit_cast<uint32_t>(float(int32_t(random.Below(64001)) - 32000) / 1000.0f);
  }
}

// --- Leaves ---

void Leaves(Context& h, uint32_t block) {
  {
    Results results("XAudioCodec::Size (sub_82466B78), with PcmCodec::Size (sub_8246D720) and "
                    "XmaCodec::Size (sub_8246B458)");
    for (int i = 0; i < 64; i++) {
      Case(h, results, block, __imp__sub_82466B78, hooks::XAudioCodecSize,
           [&](Random& random, PPCContext& t) {
             guest::StoreU8(h.base, block + 8, uint8_t(random.Below(3) == 0 ? random.Next() : random.Below(6)));
             t.r3.u64 = block;
             t.r4.u64 = block + 0x100;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioMatrix::Size (sub_82466840)");
    for (int i = 0; i < 48; i++) {
      Case(h, results, block, __imp__sub_82466840, hooks::XAudioMatrixSize,
           [&](Random& random, PPCContext& t) {
             Store(h.base, block + 12, random.Below(4) == 0 ? 0 : block + 0x200);
             t.r3.u64 = block;
             t.r4.u64 = block + 0x100;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("Reverb::Size (sub_82463B78)");
    for (int i = 0; i < 8; i++) {
      Case(h, results, block, __imp__sub_82463B78, hooks::XAudioReverbSize,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = random.Next();
             t.r4.u64 = block + 4 * random.Below(16);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("VolumeMeter::Size (sub_825A60E0)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_825A60E0, hooks::XAudioMeterSize,
           [&](Random& random, PPCContext& t) {
             if (i < 16) {
               Store(h.base, block + 8, random.Below(9));
               Store(h.base, block + 12, random.Below(64));
             }
             t.r3.u64 = block;
             t.r4.u64 = block + 0x100;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  struct Leaf {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
  };
  const Leaf leaves[] = {
      {"XAudioCodec::GetInfo (sub_82466E80)", __imp__sub_82466E80, hooks::XAudioCodecGetInfo},
      {"XAudioMatrix::GetInfo (sub_82465BE0)", __imp__sub_82465BE0, hooks::XAudioMatrixGetInfo},
      {"VolumeMeter::GetInfo (sub_825A5778)", __imp__sub_825A5778, hooks::XAudioMeterGetInfo},
      {"XAudioCodec::GetState (sub_8246A920)", __imp__sub_8246A920, hooks::XAudioCodecGetState},
      {"XAudioMatrix::SetOverwrite (sub_82465C08)", __imp__sub_82465C08,
       hooks::XAudioMatrixSetOverwrite},
      {"ReverbCore::SetStereo (sub_824679F8)", __imp__sub_824679F8, hooks::XAudioReverbSetStereo},
  };
  for (const Leaf& leaf : leaves) {
    Results results(leaf.name);
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, leaf.recompiled, leaf.native,
           [&](Random& random, PPCContext& t) {
             const bool stores_r4 = leaf.native == hooks::XAudioMatrixSetOverwrite ||
                                    leaf.native == hooks::XAudioReverbSetStereo;
             t.r3.u64 = block + 4 * random.Below(64);
             t.r4.u64 = stores_r4 ? random.Next() : block + 0x400 + random.Below(64);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The inner's slot +12 is AddRef (on the inner: +8 of the object), so
    // the call shows; counts of 0, 1 and others.
    Results results("XAudioObject::Release (sub_8246CE70)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_8246CE70, hooks::XAudioObjectRelease,
           [&](Random& random, PPCContext& t) {
             Store(h.base, block + 4, block + 0x1C00);
             Store(h.base, block + 0x1C00 + 12, kAddRef);
             Store(h.base, block + 8, i % 4 == 0 ? 1 : i % 4 == 1 ? 0 : random.Below(5));
             t.r3.u64 = block;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The packet lists ---

// A list head at `head` (offset `offset`) with `count` entries at
// `entries` (120 bytes apart), linked in order.
void MakeList(uint8_t* base, uint32_t head, uint32_t offset, uint32_t entries, uint32_t count) {
  Store(base, head + 8, offset);
  uint32_t prev = head;
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t node = entries + 120 * k + offset;
    Store(base, prev, node);
    Store(base, node + 4, prev);
    prev = node;
  }
  Store(base, prev, head);
  Store(base, head + 4, prev);
}

void Lists(Context& h, uint32_t block) {
  {
    Results results("PacketList::Next (sub_82466EA0)");
    for (int i = 0; i < 64; i++) {
      Case(h, results, block, __imp__sub_82466EA0, hooks::XAudioPacketListNext,
           [&](Random& random, PPCContext& t) {
             const uint32_t head = block + 0x100;
             Store(h.base, head + 8, 8 * random.Below(8));
             if (random.Below(3) == 0) {
               Store(h.base, head + (random.Below(2) == 0 ? 0 : 4), head);
             }
             t.r3.u64 = head;
             t.r4.u64 = random.Below(4) == 0 ? 0 : block + 0x200 + 8 * random.Below(64);
             t.r5.u64 = random.Below(3) == 0 ? 0 : random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("PacketList::Clear (sub_8246E060)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_8246E060, hooks::XAudioPacketListClear,
           [&](Random& random, PPCContext& t) {
             const uint32_t head = block + 0x100;
             MakeList(h.base, head, 8 * random.Below(4), block + 0x400, random.Below(12));
             t.r3.u64 = head;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A PCM codec's inner at +4 (its base at +16, the lists at +20 and +32),
    // through the inner destructor (sub_8246CE68 -> PcmCodec::~PcmCodec ->
    // XAudioCodec::Destruct -> CodecBase::~CodecBase -> PacketLists::Clear).
    Results results("sub_8246CE68 (PcmCodec::~PcmCodec sub_8246CF50, XAudioCodec::Destruct "
                    "sub_82466EE8, CodecBase::~CodecBase sub_82469960, PacketLists::Clear "
                    "sub_8246E0E8)");
    for (int i = 0; i < 32; i++) {
      Case(h, results, block, __imp__sub_8246CE68, hooks::XAudioPcmCodecInnerDestroy,
           [&](Random& random, PPCContext& t) {
             const uint32_t codec = block;
             MakeList(h.base, codec + 20, 0, block + 0x400, random.Below(6));
             MakeList(h.base, codec + 32, 0, block + 0x400 + 120 * 6, random.Below(6));
             t.r3.u64 = codec + 4;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioCodec::Destruct (sub_82466EE8)");
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, __imp__sub_82466EE8, hooks::XAudioCodecDestruct,
           [&](Random& random, PPCContext& t) {
             const uint32_t inner = block + 4;
             MakeList(h.base, inner + 16, 0, block + 0x400, random.Below(6));
             MakeList(h.base, inner + 28, 0, block + 0x400 + 120 * 6, random.Below(6));
             t.r3.u64 = inner;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_82465BD8 (XAudioMatrix::Destruct sub_82466418)");
    for (int i = 0; i < 8; i++) {
      Case(h, results, block, __imp__sub_82465BD8, hooks::XAudioMatrixInnerDestroy,
           [&](Random& random, PPCContext& t) { t.r3.u64 = block + 4 + 4 * random.Below(64); },
           fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- Construction (a real allocator) ---

// A source codec's descriptor at `at`: +0 the class (0), +4 the owner, the
// format at +8 (+8 the type, +12 the channels or streams, +16 the rate, the
// streams' rates and channels from +16), +64 a float, +68 the packets, +69
// a byte, +72 and +76 the callbacks.
void CodecDescriptor(uint8_t* base, uint32_t at, Random& random, uint8_t type, bool small) {
  guest::StoreU8(base, at, 0);
  guest::StoreU8(base, at + 8, type);
  if (type == 4) {
    const uint8_t streams = uint8_t(1 + random.Below(small ? 2 : 4));
    guest::StoreU8(base, at + 12, streams);
    // Each stream's rate and channels (the codec's +60 + 8i and +64 + 8i).
    static constexpr uint32_t kRates[] = {48000, 44100, 32000, 24000};
    for (uint32_t s = 0; s < 4; s++) {
      Store(base, at + 16 + 8 * s, kRates[random.Below(4)]);
      guest::StoreU8(base, at + 20 + 8 * s, uint8_t(1 + random.Below(2)));
      guest::StoreU8(base, at + 21 + 8 * s, uint8_t(random.Below(3) == 0 ? 255 : random.Below(8)));
    }
    Store(base, at + 64, AnyFloat(random));
  } else {
    guest::StoreU8(base, at + 12, uint8_t(1 + random.Below(6)));
    static constexpr uint32_t kRates[] = {48000, 44100, 22050, 24000, 8000, 96000};
    Store(base, at + 16, random.Below(4) != 0 ? kRates[random.Below(6)] : uint32_t(random.Next()));
    Store(base, at + 64, AnyFloat(random));
  }
  guest::StoreU8(base, at + 68, uint8_t(random.Below(small ? 16 : 64)));
}

void Construction(Context& h, uint32_t block) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const auto none = [](Outputs&, uint32_t) {};
  {
    Results results("PacketLists::Alloc (sub_8246E120)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_8246E120, hooks::XAudioPacketListsAlloc,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 PPCContext init = t;
                 init.r3.u64 = block + 0x100;
                 __imp__sub_8246E1E0(init, h.base);
                 Store(h.base, block + 0x10C + 8, 8 * random.Below(8));  // the free list's offset
                 t.r3.u64 = block + 0x100;
                 t.r4.u64 = (random.Next() & ~0xFFull) | (i < 4 ? 0 : random.Below(256));
                 t.r5.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("CodecBase::Init (sub_82469978)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82469978, hooks::XAudioCodecBaseInit,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 PPCContext init = t;
                 init.r3.u64 = block + 0x104;
                 __imp__sub_8246E1E0(init, h.base);
                 t.r3.u64 = block + 0x100;
                 t.r4.u64 = block + 0x400 + 4 * random.Below(16);
                 t.r5.u64 = random.Below(32);
                 t.r6.u64 = random.Next();
                 t.r7.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("PcmCodec::PcmCodec (sub_8246D750), with XAudioCodec::Construct "
                    "(sub_82466BC8), CodecBase::CodecBase (sub_82469920), PacketLists::Init "
                    "(sub_8246E1E0) and XAudioCodec::Init (sub_82466960)");
    for (int i = 0; i < 64; i++) {
      RealCase(h, results, block, __imp__sub_8246D750, hooks::XAudioPcmCodecConstruct,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 CodecDescriptor(h.base, block + 0x1800, random, uint8_t(random.Below(4)), false);
                 t.r3.u64 = block + 0x100;
                 t.r4.u64 = block + 0x1800;
                 t.r5.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The block size (+192) from vexptefp of +64: every edge float.
    Results results("XmaCodec::XmaCodec (sub_8246C1D0)");
    for (int i = 0; i < 96; i++) {
      RealCase(h, results, block, __imp__sub_8246C1D0, hooks::XAudioXmaCodecConstruct,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 CodecDescriptor(h.base, block + 0x1800, random, 4, false);
                 Store(h.base, block + 0x1800 + 64, AnyFloat(random));
                 t.r3.u64 = block + 0x100;
                 t.r4.u64 = block + 0x1800;
                 t.r5.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // PCM codecs (made and destroyed by their inner destructor afterwards
    // in both versions), the XMA codec (its XMA object's handle, a system
    // heap block, compared as made or not, then released by the XMA
    // side's release), and other types (E_NOTIMPL).
    Results results("XAudioCodec::Create (sub_82466BA0), with PcmCodec::Create (sub_8246DF78) "
                    "and XmaCodec::Create (sub_8246CDB0)");
    for (int i = 0; i < 48; i++) {
      const uint8_t type = uint8_t(i % 6 == 5 ? 5 + i : i % 6 == 4 ? 4 : i % 4);
      RealCase(h, results, block, __imp__sub_82466BA0, hooks::XAudioCodecCreate,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 CodecDescriptor(h.base, block + 0x1800, random, type, true);
                 Store(h.base, block + 0x1F00, 0);
                 t.r3.u64 = block + 0x1800;
                 t.r4.u64 = allocator;
                 t.r5.u64 = block + 0x1F00;
               },
               [&](Outputs& out, uint32_t allocator) {
                 const uint32_t codec = guest::LoadU32(h.base, block + 0x1F00);
                 if (type != 4 || codec == 0) {
                   return;
                 }
                 const uint32_t handle = guest::LoadU32(h.base, codec + 184);
                 out.Word(handle != 0);
                 Store(h.base, codec + 184, 0);
                 Normalized(out, h.base, allocator, kAllocatorBytes + 28, allocator,
                            kAllocatorBytes + 28);
                 Store(h.base, codec + 184, handle);
                 PPCContext release = CallContext(h);
                 release.r3.u64 = codec;
                 __imp__sub_8246A758(release, h.base);
               },
               fmt::format("type {} case {}", type, i));
    }
    results.Report();
  }
  {
    // A default matrix (count byte, entries of channel bytes and a level)
    // at +0x200; the matrix's slot +52 is the game's Init (under the voice
    // lists' spin lock, which this thread takes as the game does).
    Results results("XAudioMatrix::Create (sub_82466888)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82466888, hooks::XAudioMatrixCreate,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t descriptor = block + 0x1800;
                 Store(h.base, descriptor + 4, block);
                 const uint8_t entries = uint8_t(1 + random.Below(8));
                 guest::StoreU8(h.base, descriptor + 8, entries);
                 guest::StoreU8(h.base, descriptor + 9, uint8_t(random.Below(2)));
                 const uint32_t matrix = block + 0x200;
                 Store(h.base, descriptor + 12, matrix);
                 guest::StoreU8(h.base, matrix, uint8_t(random.Below(entries + 2)));
                 Store(h.base, matrix + 4, matrix + 16);
                 for (uint32_t k = 0; k < 10; k++) {
                   guest::StoreU8(h.base, matrix + 16 + 8 * k, uint8_t(random.Below(6)));
                   guest::StoreU8(h.base, matrix + 17 + 8 * k, uint8_t(random.Below(6)));
                   Store(h.base, matrix + 20 + 8 * k, AnyFloat(random));
                 }
                 Store(h.base, block + 0x1F00, 0);
                 t.r3.u64 = descriptor;
                 t.r4.u64 = allocator;
                 t.r5.u64 = block + 0x1F00;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("VolumeMeter::Create (sub_825A6460)");
    for (int i = 0; i < 24; i++) {
      RealCase(h, results, block, __imp__sub_825A6460, hooks::XAudioMeterCreate,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t descriptor = block + 0x1800;
                 Store(h.base, descriptor + 8, 1 + random.Below(8));
                 Store(h.base, descriptor + 12, 1 + random.Below(16));
                 t.r3.u64 = descriptor;
                 t.r4.u64 = allocator;
                 t.r5.u64 = block + 0x1F00;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("VolumeMeter::Init (sub_825A6138)");
    for (int i = 0; i < 24; i++) {
      RealCase(h, results, block, __imp__sub_825A6138, hooks::XAudioMeterInit,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t descriptor = block + 0x1800;
                 Store(h.base, descriptor + 8, i < 2 ? 0 : 1 + random.Below(8));
                 Store(h.base, descriptor + 12, 1 + random.Below(16));
                 t.r3.u64 = block + 0x100;
                 t.r4.u64 = descriptor;
                 t.r5.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
}

// --- The packet queue and the codecs' slots (under the spin lock) ---

constexpr uint32_t kSetFlags = 0x82467078;
constexpr uint32_t kRetirePacket = 0x8246A430;
constexpr uint32_t kCompletePacket = 0x824674D8;
constexpr uint32_t kSetFloat112 = 0x820B6950;  // RigidBody::SetFloat112: f1 to +112 of r3
constexpr uint32_t kPcmBaseRatio = 0x8246D320;

// The live spin lock (0x82954F10) is taken as the game takes it, by this
// thread. With `hold`, the case holds it around the call (so the function
// takes it again) and its depth and owner after the call are compared.
// `after(out)` adds outputs before the block's.
template <typename Setup, typename After>
void LockedCase(Context& h, Results& results, uint32_t block, GuestFunction recompiled,
                GuestFunction native, bool hold, Setup setup, After after,
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
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    setup(random, t);
    const uint32_t sp = t.r1.u32;
    PPCContext lock = t;
    if (hold) {
      hooks::XAudioSpinTake(lock, h.base);
    }
    (v == 0 ? recompiled : native)(t, h.base);
    after(out[v], sp);
    out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 4));
    out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 8) == t.r13.u32);
    if (hold) {
      hooks::XAudioSpinRelease(lock, h.base);
    }
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    R3(out[v], t.r3.u64);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A codec at `codec` in the block (the PCM layout: its base at +16, the
// queued list at +20 and the free list at +32, their entries from +0x400
// and +0x800), the base's vtable at +0x1C00 with the game's SetFlags
// (+84), CompletePacket or RetirePacket (+92) and the PCM base's
// SetFrequencyRatio or a recorder (+80); the callback (+176) 0 or AddRef;
// the state byte, the block size (1-8) and the converter's words random.
void Codec(uint8_t* base, uint32_t block, uint32_t codec, Random& random) {
  const uint32_t codec_base = codec + 16;
  Store(base, codec_base, block + 0x1C00);
  Store(base, block + 0x1C00 + 84, kSetFlags);
  Store(base, block + 0x1C00 + 92, random.Below(2) == 0 ? kCompletePacket : kRetirePacket);
  Store(base, block + 0x1C00 + 80, random.Below(2) == 0 ? kPcmBaseRatio : kSetFloat112);
  MakeList(base, codec_base + 4, 8 * random.Below(3), block + 0x400, random.Below(6));
  MakeList(base, codec_base + 16, 0, block + 0x800, random.Below(6));
  Store(base, codec_base + 160, random.Below(3) == 0 ? 0 : kAddRef);
  guest::StoreU8(base, codec_base + 156, uint8_t(random.Next()));
  guest::StoreU16(base, codec_base + 92, uint16_t(1 + random.Below(8)));
}

uint32_t QueuedEntry(uint8_t* base, uint32_t codec, Random& random) {
  const uint32_t head = codec + 20;
  const uint32_t next = guest::LoadU32(base, head);
  if (next == head || random.Below(4) == 0) {
    return 0;
  }
  return next - guest::LoadU32(base, head + 8);
}

void Queue(Context& h, uint32_t block) {
  const auto none = [](Outputs&, uint32_t) {};
  const uint32_t codec = block;
  {
    Results results("CodecBase::SetFlags (sub_82467078)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_82467078, hooks::XAudioCodecSetFlags, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   t.r3.u64 = codec + 16;
                   t.r4.u64 = random.Next();
                   t.r5.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("CodecBase::RetirePacket (sub_8246A430)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_8246A430, hooks::XAudioCodecRetirePacket,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   t.r3.u64 = codec + 16;
                   t.r4.u64 = QueuedEntry(h.base, codec, random);
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The callback's struct in CompletePacket's frame (r1 - 128 + 80)
    // compared when the callback ran.
    Results results("CodecBase::CompletePacket (sub_824674D8)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_824674D8, hooks::XAudioCodecCompletePacket,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   t.r3.u64 = codec + 16;
                   t.r4.u64 = QueuedEntry(h.base, codec, random);
                   t.r5.u64 = random.Next();
                 },
                 [&](Outputs& out, uint32_t sp) { out.Bytes(Read(h.base, sp - 48, 12)); },
                 fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("CodecBase::FlushPackets (sub_824699E0)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_824699E0, hooks::XAudioCodecFlushPackets,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   Store(h.base, codec + 16 + 160, 0);
                   if (random.Below(2) == 0) {
                     guest::StoreU8(h.base, codec + 16 + 156,
                                    guest::LoadU8(h.base, codec + 16 + 156) & 0xFE);
                   }
                   t.r3.u64 = codec + 16;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("sub_8246CEC8 (the codecs' flush)");
    for (int i = 0; i < 16; i++) {
      LockedCase(h, results, block, __imp__sub_8246CEC8, hooks::XAudioCodecFlush, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   Store(h.base, codec + 16 + 160, 0);
                   guest::StoreU8(h.base, codec + 16 + 156,
                                  guest::LoadU8(h.base, codec + 16 + 156) & 0xFE);
                   t.r3.u64 = codec;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("CodecBase::SetFormat (sub_82469E58)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_82469E58, hooks::XAudioCodecBaseSetFormat,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   if (random.Below(2) == 0) {
                     guest::StoreU8(h.base, codec + 16 + 156,
                                    guest::LoadU8(h.base, codec + 16 + 156) & 0xFE);
                   }
                   if (random.Below(2) == 0) {
                     MakeList(h.base, codec + 20, 0, block + 0x400, 0);
                   }
                   t.r3.u64 = codec + 16;
                   t.r4.u64 = block + 0x1800;
                   t.r5.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("PcmCodec::SetFormat (sub_8246CFA8)");
    for (int i = 0; i < 32; i++) {
      LockedCase(h, results, block, __imp__sub_8246CFA8, hooks::XAudioPcmCodecSetFormat,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   guest::StoreU8(h.base, codec + 16 + 156,
                                  guest::LoadU8(h.base, codec + 16 + 156) & 0xFE);
                   if (random.Below(3) != 0) {
                     MakeList(h.base, codec + 20, 0, block + 0x400, 0);
                   }
                   guest::StoreU8(h.base, block + 0x1800, uint8_t(random.Below(4)));
                   guest::StoreU8(h.base, block + 0x1804, uint8_t(1 + random.Below(6)));
                   t.r3.u64 = codec;
                   t.r4.u64 = block + 0x1800;
                   t.r5.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  struct Slot {
    const char* name;
    GuestFunction recompiled;
    GuestFunction native;
    bool f1;
  };
  const Slot slots[] = {
      {"PcmCodec::Start (sub_8246D410)", __imp__sub_8246D410, hooks::XAudioPcmCodecStart, false},
      {"PcmCodec::Stop (sub_8246D500)", __imp__sub_8246D500, hooks::XAudioPcmCodecStop, false},
      {"XAudioCodec::EndOfStream (sub_8246D630)", __imp__sub_8246D630,
       hooks::XAudioCodecEndOfStream, false},
      {"PcmCodec::SetVolume (sub_8246D008)", __imp__sub_8246D008, hooks::XAudioPcmCodecSetVolume,
       true},
      {"PcmCodec::SetPitch (sub_8246D0C8)", __imp__sub_8246D0C8, hooks::XAudioPcmCodecSetPitch,
       true},
      {"PcmCodec::SetFrequencyRatio (sub_8246D1E0)", __imp__sub_8246D1E0,
       hooks::XAudioPcmCodecSetFrequencyRatio, true},
      {"sub_8246D320 (the PCM base's SetFrequencyRatio)", __imp__sub_8246D320,
       hooks::XAudioPcmCodecBaseSetFrequencyRatio, true},
      {"XmaCodec::SetVolume (sub_8246AB48)", __imp__sub_8246AB48, hooks::XAudioXmaCodecSetVolume,
       true},
      {"XmaCodec::SetPitch (sub_8246AC40)", __imp__sub_8246AC40, hooks::XAudioXmaCodecSetPitch,
       true},
  };
  for (const Slot& slot : slots) {
    Results results(slot.name);
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, slot.recompiled, slot.native, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   // The XMA codec's streams (up to 4) at +0x1000.
                   guest::StoreU8(h.base, codec + 56, uint8_t(random.Below(5)));
                   Store(h.base, codec + 188, block + 0x1000);
                   static constexpr uint32_t kRates[] = {48000, 44100, 22050, 8000};
                   Store(h.base, codec + 60, random.Below(4) != 0 ? kRates[random.Below(4)]
                                                                  : uint32_t(random.Next()));
                   for (uint32_t k = 1; k < 4; k++) {
                     Store(h.base, codec + 60 + 8 * k, kRates[random.Below(4)]);
                   }
                   t.r3.u64 = slot.native == hooks::XAudioPcmCodecBaseSetFrequencyRatio
                                  ? codec + 16
                                  : codec;
                   t.r4.u64 = random.Next();
                   if (slot.f1) {
                     t.f1.f64 = random.Below(4) == 0
                                    ? double(std::bit_cast<float>(AnyFloat(random)))
                                    : random.Below(3) == 0
                                          ? std::bit_cast<double>(random.Next())
                                          : double(int32_t(random.Below(8001)) - 4000) / 1000.0;
                   }
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("CodecBase::OnSubmit (sub_82469AF8)");
    for (int i = 0; i < 64; i++) {
      LockedCase(h, results, block, __imp__sub_82469AF8, hooks::XAudioCodecOnSubmit, i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   Codec(h.base, block, codec, random);
                   const uint32_t entry = block + 0x1400;
                   for (uint32_t k = 12; k <= 32; k += 4) {
                     Store(h.base, entry + k, random.Below(3) == 0 ? 0 : random.Below(16));
                   }
                   t.r3.u64 = codec + 16;
                   t.r4.u64 = entry;
                   t.r5.u64 = random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // A packet at +0x1800 (+0 the data, +4 its size, +8 the loops, +12 and
    // +16 the loop's start and length, +84 the context), its sizes small so
    // the bounds checks pass and fail.
    const auto packet = [&](Random& random) {
      const uint32_t at = block + 0x1800;
      for (uint32_t k = 4; k <= 16; k += 4) {
        Store(h.base, at + k, random.Below(3) == 0 ? 0 : random.Below(64));
      }
      return at;
    };
    struct Submit {
      const char* name;
      GuestFunction recompiled;
      GuestFunction native;
      uint32_t r3;  // the codec's offset in r3: 16 for the base's
    };
    const Submit submits[] = {
        {"CodecBase::SubmitPacket (sub_82469EF8)", __imp__sub_82469EF8,
         hooks::XAudioCodecSubmitPacket, 16},
        {"CodecBase::SubmitRegion (sub_8246A1C0)", __imp__sub_8246A1C0,
         hooks::XAudioCodecSubmitRegion, 16},
        {"sub_8246CEB8 (the PCM codec's SubmitPacket)", __imp__sub_8246CEB8,
         hooks::XAudioPcmCodecSubmitPacket, 0},
        {"XmaCodec::SubmitRegion (sub_8246A860)", __imp__sub_8246A860,
         hooks::XAudioXmaCodecSubmitRegion, 0},
    };
    for (const Submit& submit : submits) {
      Results results(submit.name);
      for (int i = 0; i < 64; i++) {
        LockedCase(h, results, block, submit.recompiled, submit.native, i % 2 == 0,
                   [&](Random& random, PPCContext& t) {
                     Codec(h.base, block, codec, random);
                     guest::StoreU8(h.base, codec + 266, uint8_t(random.Below(3)));
                     const uint32_t at = packet(random);
                     if (random.Below(4) == 0) {
                       Store(h.base, at + 8, random.Below(2) == 0 ? 0xFFFFFFFF : random.Below(1000));
                     }
                     t.r3.u64 = codec + submit.r3;
                     t.r4.u64 = at;
                     t.r5.u64 = random.Next();
                   },
                   none, fmt::format("case {}", i));
      }
      results.Report();
    }
  }
}

// --- The output matrix and the volume meter ---

constexpr uint32_t kSamplesBytes = 0x3000;

// A sample worth mixing: mostly in [-1, 1], with the edge floats now and
// then.
uint32_t Sample(Random& random) {
  if (random.Below(32) == 0) {
    return AnyFloat(random);
  }
  return std::bit_cast<uint32_t>(float(int32_t(random.Below(20001)) - 10000) / 10000.0f);
}

// The block and a samples region (16-aligned, from the allocation layer)
// filled with the same random words and samples for both versions, the
// stack below the call too; the block, the samples (their floats compared
// as floats), the spin lock's depth and owner and r3 compared.
template <typename Setup>
void SamplesCase(Context& h, Results& results, uint32_t block, uint32_t samples,
                 GuestFunction recompiled, GuestFunction native, Setup setup,
                 const std::string& what) {
  const Random start = h.random;
  Random random = start;
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    random = start;
    for (uint32_t i = 0; i < kBlockBytes; i += 4) {
      Store(h.base, block + i, uint32_t(random.Next()));
    }
    for (uint32_t i = 0; i < kSamplesBytes; i += 4) {
      Store(h.base, samples + i, Sample(random));
    }
    PPCContext t = CallContext(h);
    for (uint32_t i = 4; i <= 0x400; i += 4) {
      Store(h.base, t.r1.u32 - i, uint32_t(random.Next()));
    }
    setup(random, t);
    (v == 0 ? recompiled : native)(t, h.base);
    out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 4));
    out[v].Word(guest::LoadU32(h.base, xa::kSpinLock + 8) == t.r13.u32);
    out[v].Bytes(Read(h.base, block, kBlockBytes));
    out[v].Floats(h.base, samples, kSamplesBytes / 4);
    R3(out[v], t.r3.u64);
  }
  h.random = random;
  results.Check(out[0], out[1], what);
}

// A matrix object at `matrix` (+16 its count of entries at +20, +24 the
// entries allowed, +25 the highest output channel + 1, +26 overwrite).
void Matrix(uint8_t* base, uint32_t matrix, uint32_t entries, Random& random,
            uint32_t in_channels, uint32_t out_channels) {
  guest::StoreU8(base, matrix + 16, uint8_t(random.Below(7)));
  Store(base, matrix + 20, entries);
  guest::StoreU8(base, matrix + 24, 8);
  guest::StoreU8(base, matrix + 25, uint8_t(random.Below(6) == 0 ? 4 + random.Below(4) : random.Below(4)));
  guest::StoreU8(base, matrix + 26, uint8_t(random.Below(8) == 0 ? 2 : random.Below(2)));
  for (uint32_t k = 0; k < 8; k++) {
    guest::StoreU8(base, entries + 12 * k, uint8_t(random.Below(in_channels)));
    guest::StoreU8(base, entries + 12 * k + 1, uint8_t(random.Below(out_channels)));
    Store(base, entries + 12 * k + 4, random.Below(4) == 0 ? AnyFloat(random) : Sample(random));
    Store(base, entries + 12 * k + 8, random.Below(4) == 0 ? AnyFloat(random) : Sample(random));
  }
}

// A buffer object at `buffer` (its interface at +8) over `samples`: +12
// its own format (3 channels of capacity at 48 kHz), +20 the current one
// (`channels` at 48 kHz), +28 the samples.
void Buffer(uint8_t* base, uint32_t buffer, uint32_t samples, uint32_t channels) {
  guest::StoreU8(base, buffer + 12, 0);
  guest::StoreU8(base, buffer + 13, 3);
  Store(base, buffer + 16, 48000);
  guest::StoreU8(base, buffer + 20, 0);
  guest::StoreU8(base, buffer + 21, uint8_t(channels));
  Store(base, buffer + 24, 48000);
  Store(base, buffer + 28, samples);
}

void MatrixAndMeter(Context& h, uint32_t block) {
  const auto none = [](Outputs&, uint32_t) {};
  {
    Results results("XAudioMatrix::SetMatrix (sub_82465E50)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_82465E50, hooks::XAudioMatrixSetMatrix,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t matrix = block + 0x100, levels = block + 0x200;
                   Store(h.base, matrix + 20, block + 0x140);
                   guest::StoreU8(h.base, matrix + 24, uint8_t(random.Below(9)));
                   guest::StoreU8(h.base, levels, uint8_t(random.Below(10)));
                   Store(h.base, levels + 4, levels + 0x10);
                   for (uint32_t k = 0; k < 10; k++) {
                     Store(h.base, levels + 0x14 + 8 * k, AnyFloat(random));
                   }
                   t.r3.u64 = matrix;
                   t.r4.u64 = i % 8 == 7 ? 0 : levels;
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("XAudioMatrix::SetLevels (sub_82466188)");
    for (int i = 0; i < 48; i++) {
      LockedCase(h, results, block, __imp__sub_82466188, hooks::XAudioMatrixSetLevels,
                 i % 2 == 0,
                 [&](Random& random, PPCContext& t) {
                   const uint32_t matrix = block + 0x100, levels = block + 0x200;
                   Store(h.base, matrix + 20, block + 0x140);
                   guest::StoreU8(h.base, matrix + 16, uint8_t(random.Below(9)));
                   guest::StoreU8(h.base, levels, uint8_t(random.Below(8)));
                   Store(h.base, levels + 4, levels + 0x10);
                   for (uint32_t k = 0; k < 8; k++) {
                     guest::StoreU8(h.base, levels + 0x10 + 8 * k, uint8_t(random.Below(9)));
                     Store(h.base, levels + 0x14 + 8 * k, AnyFloat(random));
                   }
                   t.r3.u64 = matrix;
                   t.r4.u64 = levels;
                   t.r5.u64 = random.Below(2) == 0 ? 0 : random.Next();
                 },
                 none, fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The matrix's slots +40 and +32 the game's SetMatrix and SetOverwrite.
    Results results("XAudioMatrix::Init (sub_82465C30)");
    for (int i = 0; i < 32; i++) {
      RealCase(h, results, block, __imp__sub_82465C30, hooks::XAudioMatrixInit,
               [&](Random& random, PPCContext& t, uint32_t allocator) {
                 const uint32_t matrix = block + 0x100, descriptor = block + 0x1800;
                 Store(h.base, matrix, block + 0x1C00);
                 Store(h.base, block + 0x1C00 + 40, 0x82465E50);
                 Store(h.base, block + 0x1C00 + 32, 0x82465C08);
                 guest::StoreU8(h.base, descriptor + 8, uint8_t(random.Below(8)));
                 guest::StoreU8(h.base, descriptor + 9, uint8_t(random.Below(3)));
                 const uint32_t levels = block + 0x200;
                 Store(h.base, descriptor + 12, i % 8 == 7 ? 0 : levels);
                 guest::StoreU8(h.base, levels, uint8_t(random.Below(9)));
                 Store(h.base, levels + 4, levels + 0x10);
                 t.r3.u64 = matrix;
                 t.r4.u64 = descriptor;
                 t.r5.u64 = allocator;
               },
               none, fmt::format("case {}", i));
    }
    results.Report();
  }
  const uint32_t samples_block = memory::Alloc(kSamplesBytes + 16, 0);
  const uint32_t samples = (samples_block + 15) & ~15u;
  {
    Results results("XAudioMatrix::MixRamp (sub_82466310)");
    for (int i = 0; i < 48; i++) {
      SamplesCase(h, results, block, samples, __imp__sub_82466310, hooks::XAudioMatrixMixRamp,
                  [&](Random& random, PPCContext& t) {
                    for (uint32_t k = 0; k < 8; k++) {
                      Store(h.base, block + 0x100 + 4 * k,
                            random.Below(4) == 0 ? AnyFloat(random) : Sample(random));
                    }
                    t.r3.u64 = samples + 0x1000 + 16 * random.Below(4);
                    t.r4.u64 = samples + 4 * random.Below(8);
                    t.r5.u64 = block + 0x100 + 4 * random.Below(2);
                    t.r6.u64 = block + 0x110;
                    t.r7.u64 = i < 8 ? 256 : random.Below(257);
                  },
                  fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // Sources of 1-2 channels at +0x400, targets of 1-3 at +0x500 (their
    // interfaces at +8), the matrix at +0x100 with up to 6 entries over
    // channels 0-2 in and 0-3 out; the same buffer as both now and then (no
    // entries: nothing).
    Results results("XAudioMatrix::Mix (sub_82466440)");
    for (int i = 0; i < 96; i++) {
      SamplesCase(h, results, block, samples, __imp__sub_82466440, hooks::XAudioMatrixMix,
                  [&](Random& random, PPCContext& t) {
                    const uint32_t matrix = block + 0x100;
                    Matrix(h.base, matrix, block + 0x140, random, 3, 4);
                    if (i % 4 == 0) {
                      guest::StoreU8(h.base, matrix + 16, 0);
                    }
                    Buffer(h.base, block + 0x400, samples, 1 + random.Below(2));
                    Buffer(h.base, block + 0x500, samples + 0x1000, 1 + random.Below(3));
                    t.r3.u64 = matrix;
                    t.r4.u64 = block + 0x408;
                    t.r5.u64 = i % 16 == 15 ? block + 0x408 : block + 0x508;
                  },
                  fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // 1-8 meter channels over buffers of 1-8 (8 over 6 for the
    // fold-downs), a history of 0-4 slots.
    Results results("VolumeMeter::Process (sub_825A6210)");
    for (int i = 0; i < 48; i++) {
      SamplesCase(h, results, block, samples, __imp__sub_825A6210, hooks::XAudioMeterProcess,
                  [&](Random& random, PPCContext& t) {
                    const uint32_t meter = block + 0x100;
                    const bool fold = i % 3 == 0;
                    Store(h.base, meter + 16, fold ? 8 : 1 + random.Below(8));
                    Store(h.base, meter + 20, random.Below(5));
                    Store(h.base, meter + 24, random.Below(11));
                    Store(h.base, meter + 28, block + 0x800);
                    Store(h.base, meter + 32, block + 0xC00);
                    Store(h.base, meter + 36, block + 0x1000);
                    Buffer(h.base, block + 0x400, samples, fold ? 6 : 1 + random.Below(8));
                    t.r3.u64 = meter;
                    t.r4.u64 = block + 0x408;
                  },
                  fmt::format("case {}", i));
    }
    results.Report();
  }
  memory::Free(samples_block);
}

// --- The render driver's set-up and the reverb's small parts ---

void ReverbParts(Context& h, uint32_t block) {
  {
    // The driver's slot +28 a recompiled leaf that leaves a trace
    // (sub_820CE9F8: r4 and r5 to +444 and +448).
    Results results("sub_824657C8 (the render driver's set-up)");
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, __imp__sub_824657C8, hooks::XAudioRenderDriverSetUp,
           [&](Random& random, PPCContext& t) {
             Store(h.base, block, block + 0x1C00);
             Store(h.base, block + 0x1C00 + 28, 0x820CE9F8);
             t.r3.u64 = block;
             t.r4.u64 = block + 0x400 + 4 * random.Below(16);
             t.r5.u64 = random.Next();
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    // The core's vtable (+16) at +0x1C00, its +4 word the second vtable's
    // offset (small).
    Results results("sub_82465410 (Reverb::Destruct sub_824656B8)");
    for (int i = 0; i < 16; i++) {
      Case(h, results, block, __imp__sub_82465410, hooks::XAudioReverbInnerDestroy,
           [&](Random& random, PPCContext& t) {
             Store(h.base, block + 16, block + 0x1C00);
             Store(h.base, block + 0x1C04, 4 * random.Below(64));
             t.r3.u64 = block + 4;
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("ReverbCore::Defaults (sub_82467538)");
    for (int i = 0; i < 8; i++) {
      Case(h, results, block, __imp__sub_82467538, hooks::XAudioReverbDefaults,
           [&](Random& random, PPCContext& t) { t.r3.u64 = block + 4 * random.Below(64); },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    Results results("LargestPrimeBelow (sub_8246DFE8)");
    for (int i = 0; i < 96; i++) {
      Case(h, results, block, __imp__sub_8246DFE8, hooks::XAudioLargestPrimeBelow,
           [&](Random& random, PPCContext& t) {
             t.r3.u64 = i < 8 ? uint64_t(i) : i < 16 ? 9970 + i - 8
                                             : i < 24 ? random.Next() : random.Below(10000);
           },
           fmt::format("case {}", i));
    }
    results.Report();
  }
  {
    struct Fill {
      const char* name;
      GuestFunction recompiled;
      GuestFunction native;
      uint32_t groups;
    };
    const Fill fills[] = {
        {"sub_82467720 (a delay line's fill, 2,048)", __imp__sub_82467720,
         hooks::XAudioFillDelayLine2048, 2048},
        {"sub_82467788 (64)", __imp__sub_82467788, hooks::XAudioFillDelayLine64, 64},
        {"sub_824677F0 (16)", __imp__sub_824677F0, hooks::XAudioFillDelayLine16, 16},
        {"sub_82467858 (256)", __imp__sub_82467858, hooks::XAudioFillDelayLine256, 256},
        {"sub_824678C0 (32)", __imp__sub_824678C0, hooks::XAudioFillDelayLine32, 32},
        {"sub_82467928 (128)", __imp__sub_82467928, hooks::XAudioFillDelayLine128, 128},
        {"sub_82467990 (512)", __imp__sub_82467990, hooks::XAudioFillDelayLine512, 512},
    };
    // The line in a region of its size from the allocation layer (2,048 x 32
    // bytes + 12), its +4 an edge float now and then.
    const uint32_t region = memory::Alloc(2048 * 32 + 64, 0);
    for (const Fill& fill : fills) {
      Results results(fill.name);
      for (int i = 0; i < 8; i++) {
        const Random start = h.random;
        Random random = start;
        Outputs out[2];
        for (int v = 0; v < 2; v++) {
          random = start;
          harness::Fill(h.base, region, 2048 * 32 + 64);
          Store(h.base, region + 4, i < 4 ? AnyFloat(random) : Sample(random));
          PPCContext t = CallContext(h);
          t.r3.u64 = region;
          (v == 0 ? fill.recompiled : fill.native)(t, h.base);
          out[v].Bytes(Read(h.base, region, fill.groups * 32 + 40));
          R3(out[v], t.r3.u64);
        }
        h.random = random;
        results.Check(out[0], out[1], fmt::format("case {}", i));
      }
      results.Report();
    }
    memory::Free(region);
  }
  {
    // The filter's four inputs: times and frequencies as the reverb's
    // parameters give them, and edge floats now and then; +24 and +28
    // compared as floats.
    Results results("ReverbCore::FilterCoefficient (sub_824675B8)");
    for (int i = 0; i < 96; i++) {
      const Random start = h.random;
      Random random = start;
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        random = start;
        for (uint32_t k = 0; k < 64; k += 4) {
          Store(h.base, block + k, uint32_t(random.Next()));
        }
        for (uint32_t k = 0; k < 16; k += 4) {
          const float value = k == 0 ? float(8000 + random.Below(40001))
                              : float(random.Below(20001)) / 1000.0f;
          Store(h.base, block + k,
                random.Below(8) == 0 ? AnyFloat(random) : std::bit_cast<uint32_t>(value));
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = block;
        (v == 0 ? __imp__sub_824675B8 : hooks::XAudioFilterCoefficient)(t, h.base);
        out[v].Bytes(Read(h.base, block, 24));
        out[v].Floats(h.base, block + 24, 2);
        out[v].Bytes(Read(h.base, block + 32, 32));
      }
      h.random = random;
      results.Check(out[0], out[1], fmt::format("case {}", i));
    }
    results.Report();
  }
}

}  // namespace

void XAudioObjects(Context& h) {
  Block block;
  Leaves(h, block.address);
  Lists(h, block.address);
  Construction(h, block.address);
  Queue(h, block.address);
  MatrixAndMeter(h, block.address);
  ReverbParts(h, block.address);
}

}  // namespace harness
