// Re-recording the scene for extra frames.
//
// The game records each tick into the command list and the game mode
// object's recording pass list. The draw step (sub_8215B4F8) records the
// scene; afterwards the tick records a little more (the HUD and text), which
// may stay at 30 Hz. For an extra frame:
//
//   1. Rewind the command list and pass list to before the draw step and run
//      it again, recording the scene from current (interpolated) memory.
//   2. Put back what the tick recorded after the draw step: its command
//      bytes, which follow the draw step's in the buffer and stay at the same
//      addresses, and its pass list nodes, re-appended to their buckets.
//
// When the extra frames are done, the tick's original recording is restored
// exactly, so the game's own frame and later ticks see what they expect.
//
// Pass list layout (see sub_8258EB88, which appends a node): node capacity
// +8, bucket count +12, nodes used +16, node pool +20 (20-byte nodes: next,
// item, two sort keys), buckets +24 (16 bytes: head, tail, ?, count).
//
// --debug_redraw_twice runs the draw step twice per tick to recheck that a
// rewound re-run is safe (it should look identical).

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
constexpr uint32_t kCommandListWritePointerOffset = 44;

// Game mode object (scene+64) and its state.
constexpr uint32_t kSceneCommandListOffset = 8;
constexpr uint32_t kSceneModeOffset = 64;
constexpr uint32_t kModeVtable = 0x8206CB8C;
constexpr uint32_t kModeStateOffset = 4;
constexpr uint32_t kModePassListsOffset = 8;  // two pass list pointers
constexpr uint32_t kStateReplayIndexOffset = 0;
constexpr uint32_t kStateSequenceOffset = 84;  // float

// Pass list.
constexpr uint32_t kPassListHeaderBytes = 40;
constexpr uint32_t kPassListNodeCapacityOffset = 8;
constexpr uint32_t kPassListBucketCountOffset = 12;
constexpr uint32_t kPassListNodesUsedOffset = 16;
constexpr uint32_t kPassListNodePoolOffset = 20;
constexpr uint32_t kPassListBucketsOffset = 24;
constexpr uint32_t kNodeBytes = 20;
constexpr uint32_t kBucketBytes = 16;
constexpr uint32_t kBucketHeadOffset = 0;
constexpr uint32_t kBucketTailOffset = 4;
constexpr uint32_t kBucketCountOffset = 12;

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

// A pass list node to re-append: its bucket and contents.
struct Node {
  uint32_t bucket;
  uint32_t item;
  uint32_t key1;
  uint32_t key2;
};

struct State {
  // Taken just before the draw step.
  bool valid = false;
  uint32_t argument = 0;  // the draw step's r3
  uint32_t list = 0;
  uint32_t state = 0;
  uint32_t pass_list = 0;  // the one being recorded
  uint32_t buckets = 0;
  uint32_t bucket_count = 0;
  std::vector<uint32_t> list_header;
  std::vector<uint32_t> pass_header;
  std::vector<uint32_t> bucket_words;
  uint32_t sequence = 0;

  // Taken just after it.
  uint32_t write_pointer_after_draw = 0;
  uint32_t nodes_after_draw = 0;

  // The whole recording when the extra frames start: restored at the end.
  bool official_saved = false;
  std::vector<uint32_t> official_list_header;
  std::vector<uint32_t> official_pass_header;
  std::vector<uint32_t> official_buckets;
  std::vector<uint32_t> official_nodes;
  std::vector<uint32_t> official_bytes;  // from before the draw step to the end
  std::vector<Node> after_draw_nodes;    // recorded after the draw step
};

State& Current() {
  static State state;
  return state;
}

uint32_t StartOfDrawStep(const State& s) {
  return s.list_header[kCommandListWritePointerOffset / 4];
}

bool SaveBeforeDrawStep(uint8_t* base, uint32_t argument, State& out) {
  uint32_t list = guest::LoadU32(base, kCurrentCommandList);
  uint32_t mode = list - kSceneCommandListOffset + kSceneModeOffset;
  if (guest::LoadU32(base, mode) != kModeVtable) {
    return false;
  }
  out.argument = argument;
  out.list = list;
  out.state = guest::LoadU32(base, mode + kModeStateOffset);
  uint32_t replay_index = guest::LoadU32(base, out.state + kStateReplayIndexOffset);
  out.pass_list = guest::LoadU32(base, mode + kModePassListsOffset + 4 * (replay_index ? 0 : 1));
  out.buckets = guest::LoadU32(base, out.pass_list + kPassListBucketsOffset);
  out.bucket_count = guest::LoadU32(base, out.pass_list + kPassListBucketCountOffset);
  out.list_header = Save(base, list, kCommandListHeaderBytes);
  out.pass_header = Save(base, out.pass_list, kPassListHeaderBytes);
  out.bucket_words = Save(base, out.buckets, out.bucket_count * kBucketBytes);
  out.sequence = guest::LoadU32(base, out.state + kStateSequenceOffset);
  out.official_saved = false;
  out.valid = true;
  return true;
}

void RestoreBeforeDrawStep(uint8_t* base, const State& s) {
  Restore(base, s.list, s.list_header);
  Restore(base, s.pass_list, s.pass_header);
  Restore(base, s.buckets, s.bucket_words);
  guest::StoreU32(base, s.state + kStateSequenceOffset, s.sequence);
}

uint32_t WritePointer(uint8_t* base, const State& s) {
  return guest::LoadU32(base, s.list + kCommandListWritePointerOffset);
}

uint32_t NodesUsed(uint8_t* base, const State& s) {
  return guest::LoadU32(base, s.pass_list + kPassListNodesUsedOffset);
}

uint32_t NodePool(uint8_t* base, const State& s) {
  return guest::LoadU32(base, s.pass_list + kPassListNodePoolOffset);
}

// Finds the nodes appended after the draw step and which bucket each is in.
std::vector<Node> FindNodesAfterDrawStep(uint8_t* base, const State& s) {
  std::vector<Node> nodes;
  uint32_t first = NodePool(base, s) + s.nodes_after_draw * kNodeBytes;
  for (uint32_t b = 0; b < s.bucket_count; b++) {
    uint32_t node = guest::LoadU32(base, s.buckets + b * kBucketBytes + kBucketHeadOffset);
    while (node != 0) {
      if (node >= first) {
        nodes.push_back({b, guest::LoadU32(base, node + 4), guest::LoadU32(base, node + 8),
                         guest::LoadU32(base, node + 12)});
      }
      node = guest::LoadU32(base, node);
    }
  }
  return nodes;
}

// Appends a node to a bucket, as sub_8258EB88 does.
bool AppendNode(uint8_t* base, const State& s, const Node& n) {
  uint32_t used = NodesUsed(base, s);
  if (used >= guest::LoadU32(base, s.pass_list + kPassListNodeCapacityOffset)) {
    return false;
  }
  uint32_t node = NodePool(base, s) + used * kNodeBytes;
  guest::StoreU32(base, node, 0);
  guest::StoreU32(base, node + 4, n.item);
  guest::StoreU32(base, node + 8, n.key1);
  guest::StoreU32(base, node + 12, n.key2);
  uint32_t bucket = s.buckets + n.bucket * kBucketBytes;
  if (guest::LoadU32(base, bucket + kBucketHeadOffset) == 0) {
    guest::StoreU32(base, bucket + kBucketHeadOffset, node);
  } else {
    guest::StoreU32(base, guest::LoadU32(base, bucket + kBucketTailOffset), node);
  }
  guest::StoreU32(base, bucket + kBucketTailOffset, node);
  guest::StoreU32(base, bucket + kBucketCountOffset,
                  guest::LoadU32(base, bucket + kBucketCountOffset) + 1);
  guest::StoreU32(base, s.pass_list + kPassListNodesUsedOffset, used + 1);
  return true;
}

void SaveOfficial(uint8_t* base, State& s) {
  s.official_list_header = Save(base, s.list, kCommandListHeaderBytes);
  s.official_pass_header = Save(base, s.pass_list, kPassListHeaderBytes);
  s.official_buckets = Save(base, s.buckets, s.bucket_count * kBucketBytes);
  s.official_nodes = Save(base, NodePool(base, s), NodesUsed(base, s) * kNodeBytes);
  s.official_bytes = Save(base, StartOfDrawStep(s), WritePointer(base, s) - StartOfDrawStep(s));
  s.after_draw_nodes = FindNodesAfterDrawStep(base, s);
  s.official_saved = true;
}

void RestoreOfficial(uint8_t* base, const State& s) {
  Restore(base, s.list, s.official_list_header);
  Restore(base, s.pass_list, s.official_pass_header);
  Restore(base, s.buckets, s.official_buckets);
  Restore(base, NodePool(base, s), s.official_nodes);
  Restore(base, StartOfDrawStep(s), s.official_bytes);
}

void ToggleReplayIndex(uint8_t* base, uint32_t state) {
  uint32_t index = guest::LoadU32(base, state + kStateReplayIndexOffset);
  guest::StoreU32(base, state + kStateReplayIndexOffset, index ? 0 : 1);
}

}  // namespace

bool DrawStep_CanRerecord() {
  return Current().valid;
}

bool DrawStep_Rerecord(PPCContext& ctx, uint8_t* base) {
  State& s = Current();
  if (!s.official_saved) {
    SaveOfficial(base, s);
  }
  RestoreBeforeDrawStep(base, s);
  ctx.r3.u64 = s.argument;
  __imp__sub_8215B4F8(ctx, base);

  // Put back what the tick recorded after the draw step. Its bytes must not
  // have been overwritten by a longer re-run.
  if (WritePointer(base, s) > s.write_pointer_after_draw) {
    RestoreOfficial(base, s);
    return false;
  }
  uint32_t start = StartOfDrawStep(s);
  for (uint32_t i = (s.write_pointer_after_draw - start) / 4; i < s.official_bytes.size(); i++) {
    guest::StoreU32(base, start + 4 * i, s.official_bytes[i]);
  }
  guest::StoreU32(base, s.list + kCommandListWritePointerOffset,
                  s.official_list_header[kCommandListWritePointerOffset / 4]);
  for (const Node& node : s.after_draw_nodes) {
    if (!AppendNode(base, s, node)) {
      RestoreOfficial(base, s);
      return false;
    }
  }
  return true;
}

void DrawStep_FinishExtraFrames(uint8_t* base) {
  State& s = Current();
  if (s.valid && s.official_saved) {
    RestoreOfficial(base, s);
  }
  s.valid = false;
  s.official_saved = false;
}

void DrawStep_ShowRecording(uint8_t* base) {
  ToggleReplayIndex(base, Current().state);
}

void DrawStep_HideRecording(uint8_t* base) {
  ToggleReplayIndex(base, Current().state);
}

}  // namespace frame_pacing

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Draw step: r3 = its argument (a game object). Saves the recording state
// around it so it can be re-run this tick.
extern "C" REX_FUNC(sub_8215B4F8) {
  REX_FUNC_PROLOGUE();
  using namespace frame_pacing;
  State& s = Current();
  uint32_t argument = ctx.r3.u32;
  if (!SaveBeforeDrawStep(base, argument, s)) {
    s.valid = false;
    __imp__sub_8215B4F8(ctx, base);
    return;
  }
  __imp__sub_8215B4F8(ctx, base);
  if (REXCVAR_GET(debug_redraw_twice)) {
    RestoreBeforeDrawStep(base, s);
    ctx.r3.u64 = argument;
    __imp__sub_8215B4F8(ctx, base);
  }
  s.write_pointer_after_draw = WritePointer(base, s);
  s.nodes_after_draw = NodesUsed(base, s);
}
