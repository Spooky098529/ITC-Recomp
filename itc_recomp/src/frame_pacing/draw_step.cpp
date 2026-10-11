// The game's draw step (sub_8215B4F8) and how to run it again.
//
// Each tick, game logic runs and then the draw step records the scene into
// the render command list (see "Separating drawing from the game tick" in
// docs/functions.md). It contains no physics, so it can be re-run to record
// the scene again, for example from interpolated state, provided everything
// it registers recordings in is first put back as it was: the command list
// header, the recording pass list's buckets and counters, and the
// submission sequence counter.
//
// --debug_redraw_twice runs it twice per tick to check that re-running is
// safe (it should look identical).

#include "guest.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <vector>

REXCVAR_DEFINE_BOOL(debug_redraw_twice, false, "ITC",
                    "Debug: run the draw step twice per tick (should look identical)");

namespace {
constexpr uint32_t kCurrentCommandList = 0x8296B918;
constexpr uint32_t kCommandListHeaderBytes = 64;
constexpr uint32_t kSceneCommandListOffset = 8;
constexpr uint32_t kSceneModeOffset = 64;
constexpr uint32_t kModeVtable = 0x8206CB8C;
constexpr uint32_t kModeStateOffset = 4;
constexpr uint32_t kModePassListsOffset = 8;      // two pass list pointers
constexpr uint32_t kStateReplayIndexOffset = 0;
constexpr uint32_t kStateSequenceOffset = 84;     // float
constexpr uint32_t kPassListHeaderBytes = 40;     // count +12, buckets +24, ...
constexpr uint32_t kPassListBucketCountOffset = 12;
constexpr uint32_t kPassListBucketsOffset = 24;
constexpr uint32_t kBucketBytes = 16;

std::vector<uint32_t> Save(uint8_t* base, uint32_t address, uint32_t bytes) {
  std::vector<uint32_t> words(bytes / 4);
  for (uint32_t i = 0; i < words.size(); i++) words[i] = REX_LOAD_U32(address + 4 * i);
  return words;
}
void Restore(uint8_t* base, uint32_t address, const std::vector<uint32_t>& words) {
  for (uint32_t i = 0; i < words.size(); i++) REX_STORE_U32(address + 4 * i, words[i]);
}
}  // namespace

// Draw step: r3 = game mode object.
extern "C" REX_FUNC(sub_8215B4F8) {
  REX_FUNC_PROLOGUE();
  if (!REXCVAR_GET(debug_redraw_twice)) {
    __imp__sub_8215B4F8(ctx, base);
    return;
  }
  uint32_t list = REX_LOAD_U32(kCurrentCommandList);
  uint32_t mode = list - kSceneCommandListOffset + kSceneModeOffset;
  static int runs = 0;
  bool ok = REX_LOAD_U32(mode) == kModeVtable;
  if (!ok) {
    if (runs++ % 300 == 0) REXLOG_INFO("draw step: mode vtable {:08x} unexpected; single run", REX_LOAD_U32(mode));
    __imp__sub_8215B4F8(ctx, base);
    return;
  }
  uint32_t state = REX_LOAD_U32(mode + kModeStateOffset);
  uint32_t replay_index = REX_LOAD_U32(state + kStateReplayIndexOffset);
  uint32_t recording = REX_LOAD_U32(mode + kModePassListsOffset + 4 * (replay_index ? 0 : 1));
  uint32_t buckets = REX_LOAD_U32(recording + kPassListBucketsOffset);
  uint32_t bucket_count = REX_LOAD_U32(recording + kPassListBucketCountOffset);

  auto list_header = Save(base, list, kCommandListHeaderBytes);
  auto pass_header = Save(base, recording, kPassListHeaderBytes);
  auto bucket_words = Save(base, buckets, bucket_count * kBucketBytes);
  uint32_t sequence = REX_LOAD_U32(state + kStateSequenceOffset);

  uint32_t r3 = ctx.r3.u32;
  __imp__sub_8215B4F8(ctx, base);
  if (runs++ % 300 == 0) {
    REXLOG_INFO("draw step: recorded {} slots, {} buckets; re-running", REX_LOAD_U32(list + 12) - list_header[3], bucket_count);
  }
  Restore(base, list, list_header);
  Restore(base, recording, pass_header);
  Restore(base, buckets, bucket_words);
  REX_STORE_U32(state + kStateSequenceOffset, sequence);
  ctx.r3.u64 = r3;
  __imp__sub_8215B4F8(ctx, base);
}
