#include "draw_tags.h"

#include "command_list.h"
#include "guest.h"


#include <mutex>

namespace interpolation {
namespace {

// FNV-1a over the recorder's arguments.
DrawTag HashArguments(const uint32_t* values, size_t count) {
  uint64_t hash = 0xCBF29CE484222325ull;
  for (size_t i = 0; i < count; i++) {
    for (int byte = 0; byte < 4; byte++) {
      hash ^= (values[i] >> (8 * byte)) & 0xFF;
      hash *= 0x100000001B3ull;
    }
  }
  return hash;
}

class DrawTagger {
 public:
  // Main thread, around each call to the per-draw recorder.
  void OnDrawRecorded(uint8_t* base, uint32_t start, uint32_t end, const uint32_t (&args)[6]) {
    if (end <= start) {
      return;
    }
    DrawTag tag = HashArguments(args, 6);
    std::lock_guard lock(mutex_);
    DrawTag occurrence_tag = tag + 0x9E3779B97F4A7C15ull * occurrences_[tag]++;
    for (uint32_t header = start; header < end; header += kCommandSlotSize) {
      std::optional<ShaderConstantUpload> upload = ReadShaderConstantHeader(base, header);
      if (!upload) {
        continue;
      }
      recording_[header + kCommandSlotSize] = occurrence_tag + upload->Signature();
      header += upload->register_count * kCommandSlotSize;  // skip the payload
    }
  }

  const DrawTags& TakeReplayedTags() {
    std::lock_guard lock(mutex_);
    replayed_ = std::move(recorded_last_tick_);
    recorded_last_tick_ = std::move(recording_);
    recording_.clear();
    occurrences_.clear();
    return replayed_;
  }

 private:
  std::mutex mutex_;
  DrawTags recording_;           // this tick, being recorded
  DrawTags recorded_last_tick_;  // last tick's
  DrawTags replayed_;            // handed to the render thread
  std::unordered_map<DrawTag, uint32_t> occurrences_;
};

DrawTagger& GetDrawTagger() {
  static DrawTagger tagger;
  return tagger;
}

}  // namespace

const DrawTags& DrawTags_TakeReplayedTags() {
  return GetDrawTagger().TakeReplayedTags();
}

}  // namespace interpolation

// Guest entry points. These keep the generated names so the game's calls and
// the function table reach them; they only unpack registers and forward.

// Per-draw recorder: r3 = model, r4 = part index, r5 = flag, r6-r8 = per-draw
// values. Tags the uploads it records.
extern "C" REX_FUNC(sub_82595550) {
  REX_FUNC_PROLOGUE();
  uint32_t args[6] = {ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                      ctx.r6.u32, ctx.r7.u32, ctx.r8.u32};
  uint32_t start = interpolation::CurrentRecordingWritePointer(base);
  __imp__sub_82595550(ctx, base);
  uint32_t end = interpolation::CurrentRecordingWritePointer(base);
  interpolation::GetDrawTagger().OnDrawRecorded(base, start, end, args);
}
