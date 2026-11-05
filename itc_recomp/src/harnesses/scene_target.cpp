// The scene target's passes (src/scene_target/,
// docs/functions/scene-target.md) against the recompiled functions. A
// world of their own in a 40 KiB block: the scene target, the scene's
// command list and the rectangles' list, a rectangle, the tiles object, the
// render manager and its getters (the game's: sub_8248CA78 returns
// [r3 + 284], sub_82379320 [r3 + 124]) and a render queue with the game's
// vtable (RenderQueue::Add records each chunk in its pass list). The
// globals they reach ([0x826BABA0], [0x826D83D0] and +4, [0x8260034C],
// [0x8266D08C], the open chunk count) point into it for the case and are
// put back after. Both versions run on the same bytes; the block, the
// frames' words and r3 are compared, exactly but for the texture
// coordinates the rectangle copies as floats. The originals run with
// --native_scene_target off. A full list isn't made: the original then
// writes its slots at guest address 0.

#include "harness.h"

#include "base/flags.h"
#include "command_list/command_list.h"
#include "graphics/render_queue.h"
#include "hooks/heap.h"
#include "hooks/scene_target.h"
#include "memory/heap.h"
#include "scene_target/scene_target.h"

#include <fmt/format.h>

#include <bit>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_scene_target);

namespace harness {

namespace {

namespace st = scene_target;

constexpr uint32_t kBlockBytes = 0xA000;
// The block's layout.
constexpr uint32_t kScene = 0x0000;      // the scene target, 32 bytes
constexpr uint32_t kList = 0x0040;       // the scene's command list, 56 bytes
constexpr uint32_t kRectList = 0x0080;   // the rectangles' list
constexpr uint32_t kRect = 0x00C0;       // a rectangle, 120 bytes
constexpr uint32_t kTiles = 0x0140;      // the tiles object
constexpr uint32_t kManager = 0x0400;    // the render manager (+284 the scene)
constexpr uint32_t kManagerVtable = 0x0600;
constexpr uint32_t kRenderScene = 0x0700;  // (+124 the queue)
constexpr uint32_t kRenderSceneVtable = 0x0900;
constexpr uint32_t kQueue = 0x0A00;
constexpr uint32_t kState = 0x0A40;      // 88 bytes
constexpr uint32_t kPassListA = 0x0AA0;  // 36 bytes each
constexpr uint32_t kPassListB = 0x0AD0;
constexpr uint32_t kItems = 0x0B00;      // 32 items of 20 bytes
constexpr uint32_t kBuckets = 0x1000;    // 700 buckets of 16 bytes
constexpr uint32_t kBucketCount = 700;
constexpr uint32_t kSlots = 0x4000;      // the scene list's slots
constexpr uint32_t kRectSlots = 0x8000;  // the rectangles' list's slots

// The globals the world replaces.
constexpr uint32_t kRenderManager = 0x826BABA0;
constexpr uint32_t kTilesPointer = 0x8260034C;
struct Global {
  uint32_t address;
  uint32_t bytes;
};
constexpr Global kGlobals[] = {
    {kRenderManager, 4},     {st::kRectLists, 8}, {kTilesPointer, 4},
    {st::kSceneTarget, 4},   {command_list::kOpenChunks, 4},
};

// The removed adapters' register steps, without the flag (the original
// where the plain C++ declines).
void NativeRecordTargets(PPCContext& ctx, uint8_t* base) {
  const auto r3 = st::RecordTargets(base, ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    __imp__sub_8209AE48(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}
void NativeRecordTileTargets(PPCContext& ctx, uint8_t* base) {
  const auto r3 = st::RecordTileTargets(base, ctx.r3.u32, ctx.r4.u32);
  if (!r3) {
    __imp__sub_8209AF40(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}
void NativeRecordBoundTargets(PPCContext& ctx, uint8_t* base) {
  const auto r3 = st::RecordBoundTargets(base, ctx.r3.u32);
  if (!r3) {
    __imp__sub_8209B058(ctx, base);
    return;
  }
  ctx.r3.u64 = *r3;
}

uint32_t RandomFloat(Random& r) {
  switch (r.Below(12)) {
    case 0: return 0x7FC00000u | uint32_t(r.Next() & 0x3FFFFF);  // a quiet NaN
    case 1: return 0x7F800001u | uint32_t(r.Next() & 0x3FFFFF);  // a signaling NaN
    case 2: return r.Below(2) ? 0x7F800000u : 0xFF800000u;       // infinities
    case 3: return 0x80000000u;                                    // -0.0
    case 4: return uint32_t(r.Next());
    case 5: return std::bit_cast<uint32_t>(1.0f);
    case 6: return 0;
    default: return std::bit_cast<uint32_t>(float(int32_t(r.Below(20001)) - 10000) / 10000.0f);
  }
}

// A command list at `list` with its slots at `slots`: `used` slots used and
// room for `room` more (the capacity used + room + 1, so `room` just fits);
// `kind` 0 none open, 1 a chunk open on it, 2 another open (the count).
void MakeList(uint8_t* base, uint32_t list, uint32_t slots, uint32_t used, uint32_t room,
              uint32_t kind) {
  guest::StoreU32(base, list + 0, command_list::kVtable);
  guest::StoreU32(base, list + 8, used + room + 1);
  guest::StoreU32(base, list + 12, used);
  guest::StoreU32(base, list + 32, slots);
  guest::StoreU32(base, list + 44, slots + 16 * (kind == 1 ? 1 : 0));
  guest::StoreU8(base, list + 52, kind == 1 ? 1 : 0);
  // An open chunk counts in the global, as in the game (else a list's
  // CloseChunk would take the count below 0).
  if (kind != 0) {
    guest::StoreU32(base, command_list::kOpenChunks, 1);
  }
}

// The block filled with `r`'s words, then the world: the getters, the
// queue (its replay index random, a few items used, the buckets empty),
// the globals pointing into it.
void MakeWorld(Context& h, uint32_t block, Random& r) {
  for (uint32_t at = 0; at < kBlockBytes; at += 4) {
    guest::StoreU32(h.base, block + at, uint32_t(r.Next()));
  }
  auto store = [&](uint32_t offset, uint32_t value) {
    guest::StoreU32(h.base, block + offset, value);
  };
  store(kManager, block + kManagerVtable);
  store(kManagerVtable + 32, 0x8248CA78);  // slot 8: [r3 + 284]
  store(kManager + 284, block + kRenderScene);
  store(kRenderScene, block + kRenderSceneVtable);
  store(kRenderSceneVtable + 8, 0x82379320);  // slot 2: [r3 + 124]
  store(kRenderScene + 124, block + kQueue);
  store(kQueue + 0, graphics::render_queue::kVtable);
  store(kQueue + 4, block + kState);
  store(kQueue + 8, block + kPassListA);
  store(kQueue + 12, block + kPassListB);
  store(kState + 0, r.Below(2));
  store(kState + 84, std::bit_cast<uint32_t>(float(r.Below(1000)) / 10.0f));
  for (const uint32_t pass_list : {kPassListA, kPassListB}) {
    store(pass_list + 8, 32);
    store(pass_list + 12, kBucketCount);
    store(pass_list + 16, r.Below(4));
    store(pass_list + 20, block + kItems);
    store(pass_list + 24, block + kBuckets);
  }
  for (uint32_t at = 0; at < 16 * kBucketCount; at += 4) {
    store(kBuckets + at, 0);
  }
  guest::StoreU32(h.base, kRenderManager, block + kManager);
  guest::StoreU32(h.base, st::kRectLists, block + kQueue);
  guest::StoreU32(h.base, st::kRectLists + 4, block + kRectList);
  guest::StoreU32(h.base, kTilesPointer, block + kTiles);
  guest::StoreU32(h.base, st::kSceneTarget, block + kScene);
  guest::StoreU32(h.base, command_list::kOpenChunks, 0);
}

// One case through both versions: `setup(r)` builds the inputs (handed a
// copy of the same generator state both times), `args(t)` sets the
// registers; the stack below the call (2 KiB) and the caller's frame's
// first 64 bytes are filled with one pattern; `extra(t, out)` adds the
// frames' words. The block is compared but for the ranges `floats(out)`
// names (word offsets in the block, compared as floats).
template <typename Setup, typename Args, typename Extra>
void Case(Context& h, Results& results, uint32_t block, Setup setup, Args args,
          GuestFunction recompiled, GuestFunction native, const std::string& what, Extra extra,
          const std::vector<uint32_t>& (*floats)(uint8_t*, uint32_t) = nullptr) {
  std::vector<std::vector<uint8_t>> saved;
  for (const Global& g : kGlobals) {
    saved.push_back(Read(h.base, g.address, g.bytes));
  }
  Outputs out[2];
  const Random start = h.random;
  Random after;
  for (int v = 0; v < 2; v++) {
    Random r = start;
    Fill(h.base, h.data, kScratchBytes);
    MakeWorld(h, block, r);
    setup(r);
    after = r;
    PPCContext t = CallContext(h);
    Fill(h.base, t.r1.u32 - 2048, 2048 + 64, 0xA5);
    args(t);
    ITC_FLAG(native_scene_target) = v == 1;
    (v == 0 ? recompiled : native)(t, h.base);
    ITC_FLAG(native_scene_target) = true;
    std::vector<uint32_t> float_words;
    if (floats) {
      float_words = floats(h.base, block);
    }
    uint32_t at = 0;
    for (const uint32_t word : float_words) {
      out[v].Bytes(Read(h.base, block + at, word - at));
      out[v].Floats(h.base, block + word, 1);
      at = word + 4;
    }
    out[v].Bytes(Read(h.base, block + at, kBlockBytes - at));
    for (const Global& g : kGlobals) {
      out[v].Bytes(Read(h.base, g.address, g.bytes));
    }
    extra(t, out[v]);
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  for (size_t i = 0; i < saved.size(); i++) {
    Write(h.base, kGlobals[i].address, saved[i].data(), kGlobals[i].bytes);
  }
  h.random = after;
  results.Check(out[0], out[1], what);
}

void NoFrame(const PPCContext&, Outputs&) {}

void TargetCases(Context& h, uint32_t block) {
  struct Function {
    const char* name;
    GuestFunction recompiled, native;
    bool bound;  // r3 = the list, the scene target from [0x8266D08C]
  };
  const Function functions[] = {
      {"SceneTarget::RecordTargets (sub_8209AE48)", __imp__sub_8209AE48, NativeRecordTargets,
       false},
      {"SceneTarget::RecordTileTargets (sub_8209AF40)", __imp__sub_8209AF40,
       NativeRecordTileTargets, false},
      {"SceneTarget::RecordBoundTargets (sub_8209B058)", __imp__sub_8209B058,
       NativeRecordBoundTargets, true},
  };
  for (const Function& f : functions) {
    Results results(f.name);
    for (int c = 0; c < 100; c++) {
      Case(
          h, results, block,
          [&](Random& r) {
            // Room for the 7 slots (just, now and then).
            const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Below(200)) : r.Below(20);
            const uint32_t room = r.Below(4) == 0 ? 7 : 7 + r.Below(100);
            MakeList(h.base, block + kList, block + kSlots, used, room, 0);
          },
          [&](PPCContext& t) {
            if (f.bound) {
              t.r3.u64 = block + kList;
              t.r4.u64 = 143;
            } else {
              t.r3.u64 = block + kScene;
              t.r4.u64 = block + kList;
              t.r5.u64 = c % 2 ? 143 : 0;
            }
          },
          f.recompiled, f.native, fmt::format("case {}", c), NoFrame);
    }
    results.Report();
  }
}

// The rectangle's vertices' texture coordinates (floats): the list's
// vertices start 15 slots after the chunk's first (its +44 before); its
// offset in the block, or 0 when the original runs both times.
uint32_t g_rect_first = 0;
const std::vector<uint32_t>& RectFloats(uint8_t*, uint32_t) {
  static std::vector<uint32_t> words;
  words.clear();
  if (g_rect_first != 0) {
    for (uint32_t corner = 0; corner < 4; corner++) {
      const uint32_t v = g_rect_first + 15 * 16 + 24 * corner;
      words.push_back(v + 16);
      words.push_back(v + 20);
    }
  }
  return words;
}

// A rectangle at `rect`: the game's position and size (0, 0 and 1280 x 720
// or 1024 x 576) or random integers, random texture coordinates (NaNs,
// infinities), a random texture, blending off or on (any word).
void MakeRect(uint8_t* base, uint32_t rect, Random& r) {
  const bool game = r.Below(3) != 0;
  const bool small = r.Below(2) == 0;
  auto any = [&](uint32_t typical) {
    return game ? typical : r.Below(4) == 0 ? uint32_t(r.Next()) : r.Below(2048);
  };
  guest::StoreU32(base, rect + 80, game ? 0 : any(0));
  guest::StoreU32(base, rect + 84, game ? 0 : any(0));
  guest::StoreU32(base, rect + 88, any(small ? 1024 : 1280));
  guest::StoreU32(base, rect + 92, any(small ? 576 : 720));
  for (uint32_t i = 0; i < 4; i++) {
    guest::StoreU32(base, rect + 96 + 4 * i, RandomFloat(r));
  }
  guest::StoreU32(base, rect + 112, uint32_t(r.Next()));
  guest::StoreU32(base, rect + 116, r.Below(2) == 0 ? 0 : r.Below(2) ? 1 : uint32_t(r.Next()));
}

void TexturedRectCases(Context& h, uint32_t block) {
  Results results("TexturedRect::Record (sub_8217C368) with RecordFlat (sub_8217AA98)");
  for (int c = 0; c < 300; c++) {
    Case(
        h, results, block,
        [&](Random& r) {
          MakeRect(h.base, block + kRect, r);
          // Room (just, now and then); now and then a chunk open on the list
          // or another (the original then).
          const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Below(300)) : r.Below(20);
          const uint32_t room = r.Below(4) == 0 ? 24 : 24 + r.Below(100);
          const uint32_t kind = r.Below(8) == 0 ? 1 + r.Below(2) : 0;
          MakeList(h.base, block + kRectList, block + kRectSlots, used, room, kind);
          g_rect_first = kind == 0 ? kRectSlots : 0;
        },
        [&](PPCContext& t) {
          t.r3.u64 = block + kRect;
          t.r4.u64 = 0;
          t.r5.u64 = c % 3 == 0 ? 594 : c % 3 == 1 ? 150 : 2000;
        },
        __imp__sub_8217C368, hooks::SceneTargetTexturedRect, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& out) {
          // RecordFlat's frame: the integers at +80 to +111, the 1.0s at
          // +124 to +175.
          out.Bytes(Read(h.base, t.r1.u32 - st::kRecordFlatFrame + 80, 96));
        },
        RectFloats);
  }
  results.Report();
}

// RecordPasses's slots on the scene's list for a mode, and its rectangles.
void PassSlots(uint32_t mode, uint32_t* slots, uint32_t* rects) {
  const uint32_t bits = mode == 0 ? 7 : mode == 1 ? 4 : mode == 3 ? 1 : mode == 4 ? 8 : 0;
  *slots = ((bits & 8) ? 36 : 0) + ((bits & 1) ? 23 : 0) +
           ((bits & 6) ? ((bits & 2) ? 36 : 30) : 0) + ((bits & 4) ? 12 : 0);
  *rects = ((bits & 8) ? 1 : 0) + ((bits & 2) ? 1 : 0) + ((bits & 4) ? 1 : 0);
}

void RecordPassesCases(Context& h, uint32_t block) {
  Results results("SceneTarget::RecordPasses (sub_8209B3D0)");
  for (int c = 0; c < 400; c++) {
    uint32_t buckets[3] = {150, 602, 121};
    Case(
        h, results, block,
        [&](Random& r) {
          // The mode: each of 0 to 5 (the passes' kinds), or any word.
          const uint32_t mode = c % 8 < 6 ? uint32_t(c % 8) : c % 8 == 6 ? 5 + r.Below(4)
                                                                        : uint32_t(r.Next());
          guest::StoreU32(h.base, block + kScene + 28, mode);
          guest::StoreU32(h.base, block + kTiles + 12, r.Below(2) ? r.Below(8) : uint32_t(r.Next()));
          uint32_t slots = 0, rects = 0;
          PassSlots(mode, &slots, &rects);
          const uint32_t used = r.Below(4) == 0 ? uint32_t(r.Below(300)) : r.Below(20);
          // The rectangles on the scene's list now and then (then the room
          // for both), else on their own; room just enough now and then; a
          // chunk open on either list or another (the originals then).
          const bool same = r.Below(4) == 0;
          const uint32_t need = slots + (same ? 24 * rects : 0);
          const uint32_t room = r.Below(4) == 0 ? need : need + r.Below(100);
          const uint32_t kind = r.Below(10) == 0 ? 1 + r.Below(2) : 0;
          MakeList(h.base, block + kList, block + kSlots, used, room, kind);
          if (same) {
            guest::StoreU32(h.base, st::kRectLists + 4, block + kList);
          } else {
            const uint32_t rect_kind = r.Below(10) == 0 ? 1 : 0;
            MakeList(h.base, block + kRectList, block + kRectSlots, r.Below(50),
                     24 * rects + r.Below(4) * 24, rect_kind);
          }
          for (uint32_t& b : buckets) {
            const uint32_t pick = r.Below(6);
            b = pick < 3 ? b : pick < 5 ? r.Below(700) : 700 + r.Below(2000);
          }
        },
        [&](PPCContext& t) {
          t.r3.u64 = block + kScene;
          t.r4.u64 = block + kList;
          t.r5.u64 = buckets[0];
          t.r6.u64 = buckets[1];
          t.r7.u64 = buckets[2];
          buckets[0] = 150;
          buckets[1] = 602;
          buckets[2] = 121;
        },
        __imp__sub_8209B3D0, hooks::SceneTargetRecordPasses, fmt::format("case {}", c),
        [&](const PPCContext& t, Outputs& out) {
          // Its frame's words (the passes' bits at +80, the rectangles at
          // +96, +224 and +352), the last rectangle's RecordFlat words below
          // it, and r5 and r6 in the caller's frame.
          const uint32_t frame = t.r1.u32 - st::kRecordPassesFrame;
          out.Bytes(Read(h.base, frame + 80, 400));
          // RecordFlat's words only: the rest of its frame holds the
          // original RecordTargets' frame (its back chain and saved
          // registers), which the native one doesn't make.
          const uint32_t flat = frame - st::kRecordFlatFrame;
          out.Bytes(Read(h.base, flat + 80, 32));
          for (uint32_t i = 0; i < 4; i++) {
            out.Word(guest::LoadU32(h.base, flat + 124 + 16 * i));
          }
          out.Bytes(Read(h.base, t.r1.u32 + 32, 16));
        });
  }
  results.Report();
}

}  // namespace

void SceneTarget(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  const uint32_t block = memory::Alloc(kBlockBytes, memory::kHeapZeroMemory);
  if (block == 0) {
    return;
  }
  TargetCases(h, block);
  TexturedRectCases(h, block);
  RecordPassesCases(h, block);
  memory::Free(block);
}

}  // namespace harness
