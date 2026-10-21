// The audio library's XMA layer (B2 in docs/rexglue-removal.md; the functions
// in docs/functions/audio.md) over our own decoding (audio/xma_stream.h):
// no XMA contexts, registers or hardware. A source voice's XMA object (voice
// +184) is an opaque handle (a small system heap block) for an Object here,
// and the voice reaches it only through these helpers. Each stream decodes
// its sound into one flat buffer of big-endian 16-bit samples in guest
// memory (the voice's recompiled update reads the samples there), and
// answers "how many ready, where, how many taken" from a position. "Ready"
// is capped at the size of the output ring the object would have had, so
// the voice never sees more than it could on the 360. Lock, wait and unlock
// have nothing to wait for; unlock decodes ahead.
//
// --log_xma logs each sound's start, loop and end, and the buffers made.

#include "guest.h"

#include "audio/xma_packets.h"
#include "audio/xma_stream.h"

#include <rex/cvar.h>
#include <rex/logging.h>
#include <rex/system/kernel_state.h>
#include <rex/system/xmemory.h>

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

REXCVAR_DEFINE_BOOL(log_xma, false, "ITC",
                    "Log each XMA sound's start, loop and end (B2's native XMA layer)");

namespace {

constexpr uint32_t kDecodeAheadSeconds = 1;
constexpr uint32_t kBothInUse = 0x80070005;

struct StreamState {
  uint32_t sample_rate = 0;
  uint32_t ring_samples = 0;  // the output ring's size the voice asked for: the "ready" cap
  uint32_t channels = 0;
  uint32_t input = 0;    // the sound's data (guest), 0 when none
  uint32_t pending = 0;  // a second sound given while one plays (not seen in play)
  uint32_t pending_size = 0;
  uint32_t buffer = 0;  // the decoded samples (guest, system heap)
  uint32_t consumed = 0;
  audio::xma::Stream stream;
};

struct Object {
  std::mutex mutex;
  bool contexts_made = false;
  std::vector<std::unique_ptr<StreamState>> streams;
};

std::mutex g_objects_mutex;
std::unordered_map<uint32_t, std::shared_ptr<Object>> g_objects;

rex::memory::Memory* Memory() {
  return rex::system::kernel_state()->memory();
}

std::shared_ptr<Object> Find(uint32_t handle) {
  std::lock_guard lock(g_objects_mutex);
  auto it = g_objects.find(handle);
  return it == g_objects.end() ? nullptr : it->second;
}

void FreeBuffer(StreamState& s) {
  s.stream.End();
  if (s.buffer) {
    Memory()->SystemHeapFree(s.buffer);
    s.buffer = 0;
  }
  s.input = 0;
}

void Reset(StreamState& s) {
  FreeBuffer(s);
  s.pending = s.pending_size = 0;
  s.consumed = 0;
}

bool Start(uint8_t* base, StreamState& s, uint32_t data, uint32_t size) {
  FreeBuffer(s);
  const uint8_t* host = base + data;
  const uint32_t samples = audio::xma::Stream::BufferSamples(host, size);
  const uint32_t bytes = std::max(samples, 1u) * s.channels * 2;
  s.buffer = Memory()->SystemHeapAlloc(bytes);
  if (!s.buffer) {
    REXLOG_ERROR("xma: no system heap for {} bytes ({} samples)", bytes, samples);
    return false;
  }
  if (!s.stream.Begin(host, size, s.channels, s.sample_rate, base + s.buffer)) {
    REXLOG_ERROR("xma: couldn't start the decoder ({} channels, {} Hz)", s.channels,
                 s.sample_rate);
    FreeBuffer(s);
    return false;
  }
  s.input = data;
  s.stream.DecodeAhead(s.sample_rate * kDecodeAheadSeconds);
  if (REXCVAR_GET(log_xma)) {
    REXLOG_INFO("xma: start: data {:08X}, {} bytes, {} channels, {} Hz, {} samples, buffer {:08X}",
                data, size, s.channels, s.sample_rate, samples, s.buffer);
  }
  return true;
}

// The stream's sound is still playing (its input "valid" to the voice).
bool Valid(const StreamState& s) {
  return s.input && s.stream.playing();
}

// Moves to a second sound once the first has played out.
void Advance(uint8_t* base, StreamState& s) {
  if (s.input && !s.stream.playing()) {
    if (REXCVAR_GET(log_xma)) {
      REXLOG_INFO("xma: end: data {:08X} after {} samples", s.input, s.consumed);
    }
    FreeBuffer(s);
    if (s.pending) {
      const uint32_t data = s.pending, size = s.pending_size;
      s.pending = s.pending_size = 0;
      Start(base, s, data, size);
    }
  }
}

// Runs `f(state)` on stream r4 of object r3 under its lock; r3 = `none` for
// a handle or stream that isn't ours.
template <typename F>
void OnStream(PPCContext& ctx, uint32_t none, F f) {
  auto object = Find(ctx.r3.u32);
  const uint32_t index = ctx.r4.u32;
  if (!object) {
    REXLOG_ERROR("xma: unknown object {:08X} (from {:08X})", ctx.r3.u32, uint32_t(ctx.lr));
    ctx.r3.u64 = none;
    return;
  }
  std::lock_guard lock(object->mutex);
  if (index >= object->streams.size()) {
    REXLOG_ERROR("xma: no stream {} in {:08X}", index, ctx.r3.u32);
    ctx.r3.u64 = none;
    return;
  }
  ctx.r3.u64 = f(*object->streams[index]);
}

}  // namespace

// XmaObject::Create (r3 the stream count, r4 their descriptions, 12 bytes
// each: sample rate +0, output ring samples +4, channels +8; r5 flags, bit 0
// = contexts made later; r6 out): the object and its handle.
extern "C" REX_FUNC(sub_8245C650) {
  REX_FUNC_PROLOGUE();
  const uint32_t count = ctx.r3.u32, descriptions = ctx.r4.u32, flags = ctx.r5.u32,
                 out = ctx.r6.u32;
  auto object = std::make_shared<Object>();
  object->contexts_made = !(flags & 1);
  for (uint32_t i = 0; i < count; i++) {
    auto s = std::make_unique<StreamState>();
    const uint32_t d = descriptions + 12 * i;
    s->sample_rate = guest::LoadU32(base, d);
    s->ring_samples = guest::LoadU32(base, d + 4);
    s->channels = guest::LoadU8(base, d + 8);
    object->streams.push_back(std::move(s));
  }
  const uint32_t handle = Memory()->SystemHeapAlloc(16);
  if (!handle) {
    ctx.r3.u64 = 0x8007000E;
    return;
  }
  {
    std::lock_guard lock(g_objects_mutex);
    g_objects[handle] = std::move(object);
  }
  guest::StoreU32(base, out, handle);
  ctx.r3.u64 = 0;
}

// XmaObject::Destroy (r3 the object).
extern "C" REX_FUNC(sub_8245C880) {
  REX_FUNC_PROLOGUE();
  const uint32_t handle = ctx.r3.u32;
  std::shared_ptr<Object> object;
  {
    std::lock_guard lock(g_objects_mutex);
    auto it = g_objects.find(handle);
    if (it != g_objects.end()) {
      object = std::move(it->second);
      g_objects.erase(it);
    }
  }
  if (object) {
    std::lock_guard lock(object->mutex);
    for (auto& s : object->streams) {
      Reset(*s);
    }
    Memory()->SystemHeapFree(handle);
  }
  ctx.r3.u64 = 0;
}

// XmaObject::ReleaseContexts and MakeContexts: only the "made" state is
// kept (sub_8245C948 reads it).
extern "C" REX_FUNC(sub_8245C8D0) {
  REX_FUNC_PROLOGUE();
  if (auto object = Find(ctx.r3.u32)) {
    std::lock_guard lock(object->mutex);
    object->contexts_made = false;
  }
  ctx.r3.u64 = 0;
}

extern "C" REX_FUNC(sub_8245D138) {
  REX_FUNC_PROLOGUE();
  if (auto object = Find(ctx.r3.u32)) {
    std::lock_guard lock(object->mutex);
    object->contexts_made = true;
  }
  ctx.r3.u64 = 0;
}

extern "C" REX_FUNC(sub_8245C948) {
  REX_FUNC_PROLOGUE();
  auto object = Find(ctx.r3.u32);
  ctx.r3.u64 = object && object->contexts_made ? 1 : 0;
}

// XmaObject::SetInputBuffer (object, stream, data, size in bytes).
extern "C" REX_FUNC(sub_8245C958) {
  REX_FUNC_PROLOGUE();
  const uint32_t data = ctx.r5.u32, size = ctx.r6.u32;
  OnStream(ctx, kBothInUse, [&](StreamState& s) -> uint32_t {
    if (!s.input) {
      return Start(base, s, data, size) ? 0 : kBothInUse;
    }
    if (s.pending) {
      return kBothInUse;
    }
    REXLOG_WARN("xma: a second sound ({:08X}) given while {:08X} plays", data, s.input);
    s.pending = data;
    s.pending_size = size;
    return 0;
  });
}

// (object, stream, address): 1 if a sound at that address is still playing.
extern "C" REX_FUNC(sub_8245CA30) {
  REX_FUNC_PROLOGUE();
  const uint32_t address = ctx.r5.u32;
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    return (Valid(s) && s.input == address) || s.pending == address;
  });
}

// (object, stream): 1 unless both input slots are taken.
extern "C" REX_FUNC(sub_8245CA90) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t { return !(Valid(s) && s.pending); });
}

// (object, stream): 1 if no sound is playing.
extern "C" REX_FUNC(sub_8245CAE0) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 1, [&](StreamState& s) -> uint32_t { return !Valid(s) && !s.pending; });
}

// (object, stream): samples ready.
extern "C" REX_FUNC(sub_8245CB38) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    return s.input ? s.stream.Ready(s.ring_samples) : 0;
  });
}

// (object, stream, out address): where to read (to r5) and the samples
// readable there in one go.
extern "C" REX_FUNC(sub_8245CD68) {
  REX_FUNC_PROLOGUE();
  const uint32_t out = ctx.r5.u32;
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    guest::StoreU32(base, out, s.buffer + s.stream.position() * s.channels * 2);
    return s.input ? s.stream.Contiguous(s.ring_samples) : 0;
  });
}

// (object, stream, samples, out address): takes up to that many samples;
// where they were read from to r6; returns how many.
extern "C" REX_FUNC(sub_8245CBB0) {
  REX_FUNC_PROLOGUE();
  const uint32_t count = ctx.r5.u32, out = ctx.r6.u32;
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    guest::StoreU32(base, out, s.buffer + s.stream.position() * s.channels * 2);
    if (!s.input) {
      return 0;
    }
    const uint32_t loop_end = s.stream.loop_end();
    const uint32_t before = s.stream.position();
    const uint32_t taken = s.stream.Consume(std::min(count, s.ring_samples));
    if (REXCVAR_GET(log_xma) && s.stream.position() < before + taken && loop_end) {
      REXLOG_INFO("xma: loop: data {:08X} at {} back to {}", s.input, before + taken,
                  s.stream.position());
    }
    s.consumed += taken;
    return taken;
  });
}

// (object, stream): samples taken so far.
extern "C" REX_FUNC(sub_8245D050) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t { return s.consumed; });
}

// XmaObject::ResetStream (object, stream): the sound stopped.
extern "C" REX_FUNC(sub_8245CE98) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    if (REXCVAR_GET(log_xma) && s.input) {
      REXLOG_INFO("xma: reset: data {:08X} after {} samples", s.input, s.consumed);
    }
    Reset(s);
    return 0;
  });
}

// (object, stream): the error status; none.
extern "C" REX_FUNC(sub_8245CF60) {
  REX_FUNC_PROLOGUE();
  OnStream(ctx, 0, [&](StreamState&) -> uint32_t { return 0; });
}

// XmaObject::SetLoopData (object, stream, r5 the loop data: start bit +0,
// end bit +4 (26 bits each), subframes played in the last frame - 1 +8,
// subframes skipped at the start +9, count +10).
extern "C" REX_FUNC(sub_8245CF98) {
  REX_FUNC_PROLOGUE();
  const uint32_t loop = ctx.r5.u32;
  const uint32_t start_bit = guest::LoadU32(base, loop) & 0x3FFFFFF;
  const uint32_t end_bit = guest::LoadU32(base, loop + 4) & 0x3FFFFFF;
  const uint32_t end_subframe = guest::LoadU8(base, loop + 8) & 3;
  const uint32_t skip = guest::LoadU8(base, loop + 9) & 7;
  const uint32_t count = guest::LoadU8(base, loop + 10);
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    s.stream.SetLoop(start_bit, end_bit, skip, end_subframe, count);
    if (REXCVAR_GET(log_xma)) {
      REXLOG_INFO("xma: loop data: data {:08X}: bits {} to {}, skip {}, end subframe {}, "
                  "count {}: samples {} to {}",
                  s.input, start_bit, end_bit, skip, end_subframe, count, s.stream.loop_start(),
                  s.stream.loop_end());
    }
    return 0;
  });
}

// (object, stream, r5 the start's bit offset, r6 subframes to skip).
extern "C" REX_FUNC(sub_8245D0E0) {
  REX_FUNC_PROLOGUE();
  const uint32_t bit = ctx.r5.u32 & 0x3FFFFFF, skip = ctx.r6.u32 & 7;
  OnStream(ctx, 0, [&](StreamState& s) -> uint32_t {
    s.stream.SetStart(bit, skip);
    return 0;
  });
}

// XmaObject::Lock, IsLocked, Unlock: nothing to wait for. Unlock decodes
// ahead and moves finished streams on.
extern "C" REX_FUNC(sub_8245D208) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 0;
}

extern "C" REX_FUNC(sub_8245D350) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 = 1;
}

extern "C" REX_FUNC(sub_8245D4D0) {
  REX_FUNC_PROLOGUE();
  if (auto object = Find(ctx.r3.u32)) {
    std::lock_guard lock(object->mutex);
    for (auto& s : object->streams) {
      Advance(base, *s);
      s->stream.DecodeAhead(s->sample_rate * kDecodeAheadSeconds);
    }
  }
  ctx.r3.u64 = 0;
}

// Reads the XMA registers' context array address at boot: no hardware.
extern "C" REX_FUNC(sub_8245C638) {
  REX_FUNC_PROLOGUE();
}

// Never called in play (docs/functions/audio.md): logged if one ever is.
extern "C" REX_FUNC(sub_8245CE10) {
  REX_FUNC_PROLOGUE();
  REXLOG_ERROR("xma: sub_8245CE10 called (from {:08X}), not handled", uint32_t(ctx.lr));
  ctx.r3.u64 = 1;
}

extern "C" REX_FUNC(sub_8245D010) {
  REX_FUNC_PROLOGUE();
  REXLOG_ERROR("xma: sub_8245D010 called (from {:08X}), not handled", uint32_t(ctx.lr));
  ctx.r3.u64 = 0;
}

extern "C" REX_FUNC(sub_8245D070) {
  REX_FUNC_PROLOGUE();
  REXLOG_ERROR("xma: sub_8245D070 called (from {:08X}), not handled", uint32_t(ctx.lr));
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, ctx.r5.u32 + 4 * i, 0);
  }
  ctx.r3.u64 = 0;
}

// The frame walk, sample to bit offset (r3 the data, r4 its size, r5 the
// stream, r6 the sample).
extern "C" REX_FUNC(sub_8246A5F8) {
  REX_FUNC_PROLOGUE();
  ctx.r3.u64 =
      audio::xma::BitOffsetOfSample(base + ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}
