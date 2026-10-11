#include "draw_tags.h"

#include "blend_state.h"

#include "command_list.h"
#include "guest.h"

#include <initializer_list>
#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace interpolation {
namespace {

// Command list object: see "Command list format" in docs/functions.md.
constexpr uint32_t kCommandListIndexOffset = 4;
constexpr uint32_t kCommandListCapacityOffset = 8;  // slots per buffer
constexpr uint32_t kCommandListBuffersOffset = 24;

class Hasher {
 public:
  void Add(uint64_t value) {
    for (int byte = 0; byte < 8; byte++) {
      hash_ ^= (value >> (8 * byte)) & 0xFF;
      hash_ *= 0x100000001B3ull;  // FNV-1a
    }
  }
  uint64_t value() const { return hash_; }

 private:
  uint64_t hash_ = 0xCBF29CE484222325ull;
};

class DrawTagger {
 public:
  // Main thread: the game is about to record into `buffer`, overwriting it.
  void OnRecordingStarted(uint32_t buffer, uint32_t size) {
    std::lock_guard lock(mutex_);
    tags_.erase(tags_.lower_bound(buffer), tags_.lower_bound(buffer + size));
    occurrences_.clear();
  }

  // Main thread, after the recorder `recorder` recorded commands between
  // `start` and `end` (guest addresses) for a call identified by `call`.
  void OnRecorded(uint8_t* base, uint32_t recorder, uint64_t call, uint32_t start,
                  uint32_t end) {
    if (end <= start) {
      return;
    }
    std::vector<std::pair<uint32_t, ShaderConstantUpload>> uploads;
    for (uint32_t header = start; header < end; header += kCommandSlotSize) {
      std::optional<ShaderConstantUpload> upload = ReadShaderConstantHeader(base, header);
      if (upload) {
        uploads.push_back({header + kCommandSlotSize, *upload});
        header += upload->register_count * kCommandSlotSize;  // skip the payload
      }
    }

    std::lock_guard lock(mutex_);
    uint32_t occurrence = occurrences_[call]++;
    for (size_t position = 0; position < uploads.size(); position++) {
      Hasher hasher;
      hasher.Add(call);
      hasher.Add(occurrence);
      hasher.Add(position);
      hasher.Add(uploads.size());
      hasher.Add(uploads[position].second.Signature());
      tags_[uploads[position].first] = {hasher.value(), recorder};
    }
  }

  std::optional<DrawTagInfo> Find(uint32_t payload) {
    std::lock_guard lock(mutex_);
    auto it = tags_.find(payload);
    if (it == tags_.end()) {
      return std::nullopt;
    }
    return it->second;
  }

 private:
  std::mutex mutex_;
  std::map<uint32_t, DrawTagInfo> tags_;  // by payload address
  std::unordered_map<uint64_t, uint32_t> occurrences_;
};

DrawTagger& GetDrawTagger() {
  static DrawTagger tagger;
  return tagger;
}

// Runs a recorder, tagging what it records. `recorder` is its guest address
// and `args` are the register values that say what it draws.
template <typename Record>
void RecordTagged(uint8_t* base, uint32_t recorder, std::initializer_list<uint32_t> args,
                  Record&& record) {
  Hasher call;
  call.Add(recorder);
  for (uint32_t arg : args) {
    call.Add(arg);
  }
  uint32_t start = CurrentRecordingWritePointer(base);
  record();
  uint32_t end = CurrentRecordingWritePointer(base);
  GetDrawTagger().OnRecorded(base, recorder, call.value(), start, end);
}

}  // namespace

std::optional<DrawTagInfo> DrawTags_Find(uint32_t payload) {
  return GetDrawTagger().Find(payload);
}

}  // namespace interpolation

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Starts a tick's recording into buffers[index] of the command list in r3.
// Clears the tags of whatever that buffer held.
extern "C" REX_FUNC(sub_8258C998) {
  REX_FUNC_PROLOGUE();
  uint32_t list = ctx.r3.u32;
  __imp__sub_8258C998(ctx, base);
  uint32_t index = guest::LoadU32(base, list + interpolation::kCommandListIndexOffset);
  uint32_t buffer =
      guest::LoadU32(base, list + interpolation::kCommandListBuffersOffset + 4 * index);
  uint32_t capacity = guest::LoadU32(base, list + interpolation::kCommandListCapacityOffset);
  interpolation::GetDrawTagger().OnRecordingStarted(buffer,
                                                    capacity * interpolation::kCommandSlotSize);
}

// Draw recorder: r3 = model, r4 = part index, r5 = flag, r6-r9 = per-draw
// values. Its uploads hold sprite and texture parameters, not transforms.
extern "C" REX_FUNC(sub_82146BD0) {
  REX_FUNC_PROLOGUE();
  interpolation::RecordTagged(base, 0x82146BD0,
                              {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                               ctx.r8.u32, ctx.r9.u32},
                              [&] { __imp__sub_82146BD0(ctx, base); });
}

// Draw recorder: r3, r4 = command list, r5 = descriptor.
extern "C" REX_FUNC(sub_8209C1A0) {
  REX_FUNC_PROLOGUE();
  interpolation::RecordTagged(base, 0x8209C1A0, {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32},
                              [&] { __imp__sub_8209C1A0(ctx, base); });
}

// Per-object transform constants (~135 calls a tick in a race): uploads the
// object's world-view-projection matrix, world matrix and a vector. r5 is
// passed to both matrix routines (sub_8209BD50, sub_820A0AC0), so it's the
// object's own transform and identifies the object; r4 goes only into the
// combined matrix (the camera), and tagging with it paired only about half
// the objects between ticks.
extern "C" REX_FUNC(sub_825953D0) {
  REX_FUNC_PROLOGUE();
  interpolation::BlendState_BeforeRecordObject(base, ctx.r4.u32, ctx.r5.u32);
  interpolation::RecordTagged(base, 0x825953D0, {ctx.r5.u32},
                              [&] { __imp__sub_825953D0(ctx, base); });
  interpolation::BlendState_AfterRecordObject(base);
}

// Draw recorder: r3 = model, r4 = part index, r5 = flag, r6 = per-draw value.
extern "C" REX_FUNC(sub_820CECA0) {
  REX_FUNC_PROLOGUE();
  interpolation::RecordTagged(base, 0x820CECA0,
                              {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32},
                              [&] { __imp__sub_820CECA0(ctx, base); });
}

// Batch recorder (up to 500 items per call, likely sprites): r3 = owner,
// r4, r5. Items pair only while the batch size is unchanged.
extern "C" REX_FUNC(sub_82128750) {
  REX_FUNC_PROLOGUE();
  interpolation::RecordTagged(base, 0x82128750, {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32},
                              [&] { __imp__sub_82128750(ctx, base); });
}

// Draw recorder: r3 = model, r4 = part index, r5 = flag, r6-r8 = per-draw
// values.
extern "C" REX_FUNC(sub_82595550) {
  REX_FUNC_PROLOGUE();
  interpolation::RecordTagged(base, 0x82595550,
                              {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                               ctx.r8.u32},
                              [&] { __imp__sub_82595550(ctx, base); });
}
