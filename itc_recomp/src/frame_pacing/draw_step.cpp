// Re-running the draw step is safe once everything it registers recordings
// in is put back as it was: the command list header, the recording pass
// list's buckets and counters, and the submission sequence counter (a race
// looked identical with the draw step run twice per tick).
//
// --debug_redraw_twice runs it twice per tick to recheck that.

#include "draw_step.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <vector>

REXCVAR_DEFINE_BOOL(debug_redraw_twice, false, "ITC",
                    "Debug: run the draw step twice per tick (should look identical)");

namespace frame_pacing {
namespace {

// The command list recorders write to (scene+8).
constexpr uint32_t kCurrentCommandList = 0x8296B918;
constexpr uint32_t kCommandListHeaderBytes = 64;

// Game mode object (scene+64) and its state.
constexpr uint32_t kSceneCommandListOffset = 8;
constexpr uint32_t kSceneModeOffset = 64;
constexpr uint32_t kModeVtable = 0x8206CB8C;
constexpr uint32_t kModeStateOffset = 4;
constexpr uint32_t kModePassListsOffset = 8;  // two pass list pointers
constexpr uint32_t kStateReplayIndexOffset = 0;
constexpr uint32_t kStateSequenceOffset = 84;  // float

// Pass list: buckets of submitted items.
constexpr uint32_t kPassListHeaderBytes = 40;
constexpr uint32_t kPassListBucketCountOffset = 12;
constexpr uint32_t kPassListBucketsOffset = 24;
constexpr uint32_t kBucketBytes = 16;

std::vector<uint32_t> Save(uint8_t* base, uint32_t address, uint32_t bytes) {
  std::vector<uint32_t> words(bytes / 4);
  for (uint32_t i = 0; i < words.size(); i++) {
    words[i] = guest::LoadU32(base, address + 4 * i);
  }
  return words;
}

void Restore(uint8_t* base, uint32_t address, const std::vector<uint32_t>& words) {
  for (uint32_t i = 0; i < words.size(); i++) {
    guest::StoreU32(base, address + 4 * i, words[i]);
  }
}

// The recording structures as they were before this tick's draw step.
struct Snapshot {
  bool valid = false;
  uint32_t argument = 0;  // the draw step's r3
  uint32_t list = 0;
  uint32_t state = 0;
  uint32_t recording = 0;  // pass list
  uint32_t buckets = 0;
  std::vector<uint32_t> list_header;
  std::vector<uint32_t> pass_header;
  std::vector<uint32_t> bucket_words;
  uint32_t sequence = 0;
};

Snapshot& CurrentSnapshot() {
  static Snapshot snapshot;
  return snapshot;
}

bool TakeSnapshot(uint8_t* base, uint32_t argument, Snapshot& out) {
  uint32_t list = guest::LoadU32(base, kCurrentCommandList);
  uint32_t mode = list - kSceneCommandListOffset + kSceneModeOffset;
  if (guest::LoadU32(base, mode) != kModeVtable) {
    return false;
  }
  out.argument = argument;
  out.list = list;
  out.state = guest::LoadU32(base, mode + kModeStateOffset);
  uint32_t replay_index = guest::LoadU32(base, out.state + kStateReplayIndexOffset);
  out.recording = guest::LoadU32(base, mode + kModePassListsOffset + 4 * (replay_index ? 0 : 1));
  out.buckets = guest::LoadU32(base, out.recording + kPassListBucketsOffset);
  uint32_t bucket_count = guest::LoadU32(base, out.recording + kPassListBucketCountOffset);
  out.list_header = Save(base, list, kCommandListHeaderBytes);
  out.pass_header = Save(base, out.recording, kPassListHeaderBytes);
  out.bucket_words = Save(base, out.buckets, bucket_count * kBucketBytes);
  out.sequence = guest::LoadU32(base, out.state + kStateSequenceOffset);
  out.valid = true;
  return true;
}

void RestoreSnapshot(uint8_t* base, const Snapshot& snapshot) {
  Restore(base, snapshot.list, snapshot.list_header);
  Restore(base, snapshot.recording, snapshot.pass_header);
  Restore(base, snapshot.buckets, snapshot.bucket_words);
  guest::StoreU32(base, snapshot.state + kStateSequenceOffset, snapshot.sequence);
}

void ToggleReplayIndex(uint8_t* base, uint32_t state) {
  uint32_t index = guest::LoadU32(base, state + kStateReplayIndexOffset);
  guest::StoreU32(base, state + kStateReplayIndexOffset, index ? 0 : 1);
}

}  // namespace

bool DrawStep_CanRerecord() {
  return CurrentSnapshot().valid;
}

void DrawStep_Rerecord(PPCContext& ctx, uint8_t* base) {
  const Snapshot& snapshot = CurrentSnapshot();
  RestoreSnapshot(base, snapshot);
  ctx.r3.u64 = snapshot.argument;
  __imp__sub_8215B4F8(ctx, base);
}

void DrawStep_ShowRecording(uint8_t* base) {
  ToggleReplayIndex(base, CurrentSnapshot().state);
}

void DrawStep_HideRecording(uint8_t* base) {
  ToggleReplayIndex(base, CurrentSnapshot().state);
}

void DrawStep_Invalidate() {
  CurrentSnapshot().valid = false;
}

}  // namespace frame_pacing

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Draw step: r3 = its argument (a game object). Snapshots the recording
// structures first so the step can be re-run this tick.
extern "C" REX_FUNC(sub_8215B4F8) {
  REX_FUNC_PROLOGUE();
  using namespace frame_pacing;
  Snapshot& snapshot = CurrentSnapshot();
  uint32_t argument = ctx.r3.u32;
  if (!TakeSnapshot(base, argument, snapshot)) {
    snapshot.valid = false;
    __imp__sub_8215B4F8(ctx, base);
    return;
  }
  __imp__sub_8215B4F8(ctx, base);
  if (REXCVAR_GET(debug_redraw_twice)) {
    DrawStep_Rerecord(ctx, base);
  }
}
