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

}  // namespace

void XAudioObjects(Context& h) {
  Block block;
  Leaves(h, block.address);
  Lists(h, block.address);
  Construction(h, block.address);
}

}  // namespace harness
