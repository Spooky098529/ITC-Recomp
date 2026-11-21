// XAudio's voices as plain C++ (voices.h; docs/functions/xaudio.md).

#include "xaudio/voices.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "memory/c_runtime_heap.h"
#include "guest.h"
#include "schema/xaudio.h"
#include "schema/xaudio_engine.h"
#include "schema/xaudio_objects.h"

#include <atomic>
#include <cstring>
#include <iterator>

namespace xaudio {

namespace {

using G = schema::XaudioGlobals;
using Voice = schema::XAudioVoice;
using Source = schema::XAudioSourceVoice;
using Submix = schema::XAudioSubmixVoice;
using Buffer = schema::XAudioBuffer;
using Engine = schema::XAudioEngine;
using Format = schema::XAudioVoiceFormat;
using Description = schema::XAudioVoiceDescription;
using EffectEntry = schema::XAudioEffectEntry;
using Slot = schema::XAudioOutputSlot;

// The voice's output slot `i` (12 bytes each from its +72, read at the call).
Slot OutputSlot(const Voice& v, uint32_t i) { return Slot(v.base, v.outputs().address + 12 * i); }

// The voice's effect entry `i` (8 bytes each from its +36, read at the call).
EffectEntry Effect(const Voice& v, uint32_t i) { return EffectEntry(v.base, v.effects() + 8 * i); }

// Frames a pass holds at `rate` (the original's 256 / (48,000 / rate), the
// quotient's low 16 bits); false where the original traps (a rate of 0 or
// above 48,000).
bool PassFrames(uint32_t rate, uint32_t* frames) {
  if (rate == 0 || kSampleRate / rate == 0) {
    return false;
  }
  *frames = (256u / (kSampleRate / rate)) & 0xFFFF;
  return true;
}

}  // namespace

void SourceOutputFormat(uint8_t* base, uint32_t format, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245DC88 XAudio::SourceOutputFormat");
  const schema::XAudioSourceDescription source(base, format);
  const Format o(base, out);
  uint8_t channels;
  if (source.format_type() == 4) {
    const uint32_t streams = source.format_channels();
    uint32_t sum = 0;
    for (uint32_t i = 0; i < streams; i++) {
      sum += guest::LoadU8(base, source.format_address() + 12 + 8 * i);
    }
    channels = uint8_t(sum);
  } else {
    channels = source.format_channels();
  }
  o.set_channels(channels);
  o.set_type(0);
  o.set_rate(kSampleRate);
}

void SubmixDescribe(uint8_t* base, uint32_t description, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F010 XAudioSubmixVoice::Describe");
  const schema::XAudioSubmixDescription d(base, description);
  const Description o(base, out);
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(base, out + i, 0);
  }
  o.set_format_type(0);
  o.set_channels(d.channels());
  o.set_rate(kSampleRate);
  o.set_effect_chain(d.effect_chain());
  o.set_callback(d.callback());
  o.set_callback_context(d.callback_context());
  const uint8_t count = d.output_count();
  o.set_output_count(count != 0 ? count : 1);
  const uint8_t channels = d.output_channels();
  o.set_output_channels(channels != 0 ? channels : 6);
  o.set_output_voices(d.output_voices());
}

uint32_t DefaultMatrix(uint8_t* base, uint32_t inputs, uint32_t outputs, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245EC40 XAudio::DefaultMatrix");
  const uint32_t in = inputs & 0xFF;
  const uint32_t to = outputs & 0xFF;
  if (in == 0 || to == 0) {
    return 0;
  }
  uint8_t count;
  uint32_t table;
  if (in == to) {
    if (in > 6) {
      return 0;
    }
    count = uint8_t(in);
    table = G::default_matrices_address() + 48;
  } else if (to == 1) {
    if (in > 6) {
      return 0;
    }
    count = uint8_t(in);
    table = G::default_matrices_address() + 96;
  } else if (in == 1) {
    if (to != 2 && to != 4 && to != 6) {
      return 0;
    }
    count = 2;
    table = G::default_matrices_address();
  } else if (in == 2) {
    if (to != 4 && to != 6) {
      return 0;
    }
    count = 2;
    table = G::default_matrices_address() + 48;
  } else if (in == 4 && to == 6) {
    count = 4;
    table = G::default_matrices_address() + 144;
  } else {
    return 0;
  }
  const schema::XAudioMatrixLevels levels(base, out);
  levels.set_count(count);
  levels.set_table(table);
  return out;
}

std::optional<uint32_t> BufferSize(uint8_t* base, uint32_t format, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245FA28 XAudioBuffer::Size");
  const schema::XAudioBufferDescription d(base, format);
  uint32_t size = 0;
  if (d.kind() == 0) {
    uint32_t frames;
    if (!PassFrames(d.rate(), &frames)) {
      return std::nullopt;
    }
    size = (frames * uint32_t(d.channels()) + 32) * 4;
  }
  schema::XAudioSizeOut(base, out).set_size(size + 36);
  return 0;
}

std::optional<uint32_t> BufferClear(uint8_t* base, uint32_t buffer, uint32_t format,
                                    uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8245F8E8 XAudioBuffer::Clear");
  const Buffer b(base, buffer);
  if (format == 0) {
    format = b.capacity_format_address();
  }
  const Format f(base, format);
  uint32_t format_frames, capacity_frames, current_frames;
  if (!PassFrames(f.rate(), &format_frames) ||
      !PassFrames(b.capacity_rate(), &capacity_frames) ||
      !PassFrames(b.current_rate(), &current_frames)) {
    return std::nullopt;
  }
  const uint32_t wanted = format_frames * f.channels();
  const uint32_t capacity = capacity_frames * b.capacity_channels();
  uint32_t current = current_frames * b.current_channels();
  if (wanted > capacity) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  const uint32_t mode = flags & 0xFF;
  if ((mode & 1) == 0 && current > wanted) {
    return 0x8000FFFFu;  // E_UNEXPECTED
  }
  if ((mode & 2) != 0) {
    if ((mode & 1) != 0) {
      current = 0;
    }
    // memset (sub_825A71D0) with 0: the same bytes.
    const uint32_t from = b.samples() + current * 4;
    const uint32_t bytes = (wanted - current) * 4;
    guest::ForEachHostPart(base, from, bytes, [](uint8_t* host, uint32_t, uint32_t part) {
      std::memset(host, 0, part);
    });
  }
  b.set_current_channels(f.channels());
  return 0;
}

uint32_t EngineBuffer(uint8_t* base, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8245DEB0 XAudioVoice::EngineBuffer");
  const Engine engine = G::engine(base);
  return guest::LoadU32(base, engine.mix_buffers_address() + ((processor & 0xFF) << 3));
}

uint32_t GetType(uint8_t* base, uint32_t voice, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245E318 XAudioVoice::GetType");
  schema::XAudioByteOut(base, out).set_value(Voice(base, voice).type());
  return 0;
}

uint32_t GetFormat(uint8_t* base, uint32_t voice, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245E328 XAudioVoice::GetFormat");
  const Voice v(base, voice);
  const Format o(base, out);
  o.set_head(v.format());
  o.set_rate(v.rate());
  return 0;
}

void SetFormatWords(uint8_t* base, uint32_t voice, uint32_t format) {
  ITC_NATIVE_RUNNING("sub_82461140 XAudioVoice::SetFormatWords");
  const Voice v(base, voice);
  const Format f(base, format);
  v.set_format(f.head());
  v.set_rate(f.rate());
}

uint32_t AddRef(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8245F860 XAudioVoice::AddRef");
  const Voice v(base, object);
  const uint32_t count = v.ref_count() + 1;
  v.set_ref_count(count);
  return count;
}

void SetRootVtable(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8245FB00 XAudioVoice::SetRootVtable");
  Voice(base, object).set_vtable(kRootVtable);
}


uint32_t AddStat(uint8_t* base, uint32_t index, uint32_t amount) {
  ITC_NATIVE_RUNNING("sub_8245F640 XAudio::AddStat");
  // The original's lwarx/stwcx. loop: a compare-and-swap on the big-endian
  // word, as the recompiled users of the same counters do.
  auto* word = reinterpret_cast<uint32_t*>(guest::Pointer(base, uint32_t(kStats + (index << 2))));
  uint32_t seen = __atomic_load_n(word, __ATOMIC_SEQ_CST);
  while (!__atomic_compare_exchange_n(word, &seen,
                                      __builtin_bswap32(__builtin_bswap32(seen) + amount), false,
                                      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
  }
  return 0;
}

namespace {

Regs Args(std::initializer_list<uint64_t> r) {
  Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}

// The call made at `sp` (a plain C++ function's own frame).
Regs At(uint32_t sp, Regs regs) {
  regs.sp = sp;
  return regs;
}

// The registers the original sets before calling a function whose plain
// C++ runs here instead.
void SetRegs(const GuestCall& call, Regs regs) { call(0, 0, regs); }

// A frame of `bytes` below `frame` with its back chain, for a plain C++
// callee working in its own frame as the original.
uint32_t Below(uint8_t* base, uint32_t frame, uint32_t bytes) {
  const uint32_t below = frame - bytes;
  guest::StoreU32(base, below, frame);
  uint32_t& floor = StackFloor();
  if (floor == 0 || below < floor) {
    floor = below;
  }
  return below;
}

}  // namespace

uint32_t& StackFloor() {
  thread_local uint32_t floor = 0;
  return floor;
}

uint64_t& LastCallR3() {
  thread_local uint64_t r3 = 0;
  return r3;
}

uint32_t FrameBelow(uint8_t* base, uint32_t frame, uint32_t bytes) {
  return Below(base, frame, bytes);
}

uint32_t Method(uint8_t* base, uint32_t object, uint32_t slot) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + slot);
}

namespace {
uint32_t InterfaceThunk(uint8_t* base, uint32_t iface, uint32_t slot, uint32_t* function) {
  const uint32_t object = iface != 0 ? iface - 8 : 0;
  *function = Method(base, object, slot);
  return object;
}
}  // namespace

uint32_t FactorySizeThunk(uint8_t* base, uint32_t iface, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245D610 XAudioFactory::Size");
  return InterfaceThunk(base, iface, 20, function);
}

uint32_t FactoryCreateThunk(uint8_t* base, uint32_t iface, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245D630 XAudioFactory::Create");
  return InterfaceThunk(base, iface, 24, function);
}

uint32_t BufferAddRefThunk(uint8_t* base, uint32_t iface, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245F6B0 XAudioBuffer::AddRefThunk");
  return InterfaceThunk(base, iface, 4, function);
}

uint32_t BufferReleaseThunk(uint8_t* base, uint32_t iface, uint32_t* function) {
  ITC_NATIVE_RUNNING("sub_8245F6D0 XAudioBuffer::ReleaseThunk");
  return InterfaceThunk(base, iface, 8, function);
}

uint32_t Release(uint8_t* base, uint32_t object, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F878 XAudioVoice::Release");
  const Voice v(base, object);
  const uint32_t count = v.ref_count() - 1;
  v.set_ref_count(count);
  if (count != 0) {
    return count;
  }
  call(Method(base, object, 12), 0x8245F8A4, Args({object}));
  return 0;
}

uint32_t ObjectFree(uint8_t* base, uint32_t object, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245AAD8 XAudioObject::Free");
  const uint32_t allocator = Voice(base, object).allocator().address;
  if (allocator != 0) {
    call(Method(base, allocator, 0), 0x8245AB10, Args({allocator}));
  }
  uint32_t r3 = call(Method(base, object, 0), 0x8245AB28, Args({object, 1}));
  if (allocator != 0) {
    r3 = call(Method(base, allocator, 4), 0x8245AB44, Args({allocator}));
  }
  return r3;
}

uint32_t ApplyVolume(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DFC0 XAudioSourceVoice::ApplyVolume");
  const Source s(base, voice);
  const uint32_t category = s.category();
  const float volume =
      category < 2
          ? guest::LoadFloat(base, G::engine(base).category_volumes_address() + (category << 2))
          : G::one(base);
  const uint32_t codec = s.codec().address;
  Regs regs = Args({codec});
  regs.f1_set = true;
  regs.f1 = double(float(double(s.volume()) * double(volume)));
  return call(Method(base, codec, 88), 0, regs);
}

uint32_t StopAndReset(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DD60 XAudioSourceVoice::StopAndReset");
  const uint32_t r3 = call(Method(base, voice, 60), 0x8245DD88, Args({voice, 1}));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t codec = Source(base, voice).codec().address;
  if (codec == 0) {
    return r3;
  }
  return call(Method(base, codec, 32), 0x8245DDB0, Args({codec}));
}

uint32_t SetEffectParameters(uint8_t* base, uint32_t voice, uint32_t index, uint64_t r5,
                             uint64_t r6, uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461418 XAudioVoice::SetEffectParameters");
  const Voice v(base, voice);
  const uint32_t i = index & 0xFF;
  if (i >= v.effect_count()) {
    return 0x80070057u;  // E_INVALIDARG
  }
  const uint32_t effect = Effect(v, i).object().address;
  return call(Method(base, effect, 16), 0x82461480, Args({effect, r5, r6, r7}));
}

uint32_t ReleaseOutputs(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460A58 XAudioVoice::ReleaseOutputs");
  const Voice v(base, voice);
  uint32_t r3 = voice;
  for (uint32_t i = 0; i < v.output_count(); i++) {
    const uint32_t target = OutputSlot(v, i).target().address;
    r3 = call(Method(base, target, 8), 0x82460A98, Args({target}));
  }
  v.set_output_count(0);
  return r3;
}

uint32_t SetOutputMatrices(uint8_t* base, uint32_t voice, uint32_t matrices,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460AB8 XAudioVoice::SetOutputMatrices");
  const Voice v(base, voice);
  const schema::XAudioOutputMatrices list(base, matrices);
  const uint32_t not_playing = ~uint32_t(v.flags()) & 1;
  uint32_t r3 = 0;
  if (list.count() == 0) {
    return r3;
  }
  for (uint32_t i = 0; int32_t(r3) >= 0; ) {
    const schema::XAudioOutputMatrixEntry entry(base, list.entries() + 8 * i);
    const uint32_t output = entry.output();
    const uint32_t matrix = entry.levels();
    const uint32_t object = OutputSlot(v, output).matrix().address;
    r3 = call(Method(base, object, 56), 0x82460B34, Args({object, matrix, not_playing}));
    if (++i >= list.count()) {
      break;
    }
  }
  return r3;
}

uint32_t SetFormat(uint8_t* base, uint32_t voice, uint32_t format, uint32_t outputs,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460778 XAudioVoice::SetFormat");
  SetFormatWords(base, voice, format);
  // r5 still `outputs` (the original's own r5, kept across SetFormatWords).
  return call(Method(base, voice, 36), 0x824607B0, Args({voice, outputs, outputs}));
}

uint32_t Alloc(uint8_t* base, uint32_t size, uint32_t attributes) {
  ITC_NATIVE_RUNNING("sub_8245EF80 XAudio::Alloc");
  return memory::crt::XMemAlloc(base, size, attributes);
}

std::optional<uint32_t> BufferClearThunk(uint8_t* base, uint32_t iface, uint32_t format,
                                         uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8245FA10 XAudioBuffer::ClearThunk");
  return BufferClear(base, iface != 0 ? iface - 8 : 0, format, flags);
}

void SourceDescribe(uint8_t* base, uint32_t description, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245E010 XAudioSourceVoice::Describe");
  const schema::XAudioSourceDescription d(base, description);
  const Description o(base, out);
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(base, out + i, 0);
  }
  SourceOutputFormat(base, description, out);
  o.set_effect_chain(d.effect_chain());
  const Engine engine = G::engine(base);
  uint32_t buffer = 0;
  for (uint32_t i = 0; i < 6; i++) {
    if (guest::LoadU32(base, engine.mix_buffers_address() + 8 * i) != 0) {
      buffer = guest::LoadU32(base, engine.mix_buffers_address() + 8 * i);
      break;
    }
  }
  o.set_buffer(buffer);
  o.set_callback(d.callback());
  o.set_callback_context(d.callback_context());
  const uint8_t count = d.output_count();
  o.set_output_count(count != 0 ? count : 1);
  const uint8_t channels = d.output_channels();
  o.set_output_channels(channels != 0 ? channels : 6);
  o.set_output_voices(d.output_voices());
}

std::optional<uint32_t> Reformat(uint8_t* base, uint32_t voice, uint32_t format,
                                 uint32_t outputs, uint64_t r6, uint32_t frame,
                                 const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DCF0 XAudioSourceVoice::Reformat");
  const Source s(base, voice);
  if (!c_runtime::Memcpy(base, s.format_address(), format, 56)) {
    return std::nullopt;
  }
  const uint32_t codec = s.codec().address;
  const uint32_t r3 = call(Method(base, codec, 112), 0x8245DD34, Args({codec, format, r6}));
  if (int32_t(r3) < 0) {
    return r3;
  }
  SourceOutputFormat(base, format, frame + 80);
  return SetFormat(base, voice, frame + 80, outputs, call);
}

uint32_t BaseSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461158 XAudioVoice::BaseSize");
  const Description d(base, description);
  uint32_t r3 = 0;
  uint32_t size = 0;
  if (d.buffer() == 0) {
    // A format of the description's channels at 48 kHz, at the frame's +96.
    const uint32_t local = frame + 96;
    for (uint32_t i = 0; i < 20; i += 4) {
      guest::StoreU32(base, local + i, 0);
    }
    guest::StoreU8(base, local + 5, d.channels());
    guest::StoreU32(base, local + 8, kSampleRate);
    // BufferSize can't refuse a rate of 48,000.
    r3 = *BufferSize(base, local, frame + 80);
    size = guest::LoadU32(base, frame + 80);
  }
  uint32_t effects = 0;
  const schema::XAudioEffectChain chain = d.effect_chain();
  if (chain && chain.count() != 0) {
    for (uint32_t offset = 0; effects < d.effect_chain().count(); offset += 4) {
      const uint32_t id = guest::LoadU32(base, d.effect_chain().ids() + offset);
      const uint32_t factory = G::engine(base).factory();
      uint32_t function = 0;
      const uint32_t object = FactorySizeThunk(base, factory, &function);
      r3 = call(function, 0x82461200, At(frame, Args({object, id, frame + 80})));
      if (int32_t(r3) < 0) {
        return r3;
      }
      effects++;
      size += guest::LoadU32(base, frame + 80);
    }
  }
  schema::XAudioSizeOut(base, out).set_size((effects << 3) + size);
  return r3;

}

std::optional<uint32_t> BufferConstruct(uint8_t* base, uint32_t buffer, uint32_t format,
                                        uint32_t allocator, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F750 XAudioBuffer::XAudioBuffer");
  const schema::XAudioBufferDescription d(base, format);
  const Buffer b(base, buffer);
  const uint8_t type = d.kind();
  uint32_t frames = 0;
  if (type == 0 && !PassFrames(d.rate(), &frames)) {
    return std::nullopt;
  }
  b.set_vtable(kBufferVtable);
  b.set_ref_count(1);
  b.set_kind(type);
  b.set_current_type(0);
  b.set_current_rate(kSampleRate);
  b.set_extra(d.extra());
  if (b.kind() != 0) {
    b.set_capacity_format(0);
    b.set_capacity_rate(0);
    b.set_current_channels(0);
    b.set_samples(d.samples());
    return buffer;
  }
  const uint32_t bytes = ((frames * uint32_t(d.channels())) << 2) + 128;
  const uint32_t samples =
      (call(Method(base, allocator, 20), 0x8245F7E8, Args({allocator, bytes, allocator})) +
       127) &
      0xFFFFFF80u;
  if (d.format_address() == 0) {
    b.set_capacity_format(0);
    b.set_capacity_rate(0);
  } else {
    b.set_capacity_format(d.format());
    b.set_capacity_rate(d.rate());
  }
  b.set_samples(samples);
  b.set_current_channels(0);
  return buffer;
}

uint32_t BufferCreate(uint8_t* base, uint32_t format, uint32_t allocator, uint32_t out,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FA80 XAudioBuffer::Create");
  const uint32_t memory = call(Method(base, allocator, 20), 0x8245FAB0, Args({allocator, 36, out}));
  if (memory == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  uint32_t buffer;
  if (const auto made = BufferConstruct(base, memory, format, allocator, call)) {
    buffer = *made;
  } else {
    buffer = call(kBufferConstruct, 0x8245FAC4, Args({memory, format, allocator}));
  }
  if (buffer == 0) {
    return 0x8007000Eu;
  }
  Regs regs;
  regs.r[0] = buffer;
  regs.r[1] = out;
  regs.count = 2;
  call(Method(base, buffer, 20), 0x8245FAE4, regs);
  return 0;
}

uint32_t VoiceConstruct(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461500 XAudioVoice::Construct");
  const Voice v(base, voice);
  v.set_allocator(allocator);
  v.set_vtable(kVoiceVtable0);
  v.set_ref_count(1);
  if (allocator != 0) {
    call(Method(base, allocator, 0), 0x82461550, Args({allocator}));
  }
  v.set_type(uint8_t(type));
  v.set_vtable(kVoiceVtable);
  v.set_node_next(v.node_next_address());
  v.set_node_prev(v.node_next_address());
  v.set_active_next(v.active_next_address());
  v.set_active_prev(v.active_next_address());
  v.set_output_uses(0);
  return voice;
}

uint32_t DefaultAllocator(uint8_t* base, uint32_t attributes, uint32_t size, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_824605E8 XAudio::DefaultAllocator");
  const uint32_t block = Alloc(base, size + 28, attributes);
  if (block == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  const schema::XAudioBumpAllocator a(base, block);
  a.set_interface_vtable(kRootVtable);
  a.set_ref_count(1);
  a.set_vtable(kAllocatorVtable);
  a.set_interface_vtable(kAllocatorVtable2);
  a.set_attributes(attributes);
  a.set_used(0);
  a.set_capacity(size);
  a.set_block(block + schema::XAudioBumpAllocator::kSize);
  schema::XAudioAllocatorOut(base, out).set_allocator(block);
  return 0;
}

std::optional<uint32_t> GetAllocator(uint8_t* base, uint32_t attributes, uint32_t size,
                                     uint32_t inout, uint32_t frame, bool native_heap,
                                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460688 XAudio::GetAllocator");
  const schema::XAudioAllocatorOut io(base, inout);
  const uint32_t allocator = io.allocator().address;
  if (allocator == 0) {
    if (size != 0 && !native_heap) {
      return std::nullopt;
    }
    guest::StoreU32(base, frame + 80, 0);
    const uint32_t r3 = size != 0 ? DefaultAllocator(base, attributes, size, frame + 80)
                                  : call(kAllocatorUnseen, 0x824606D0, Args({frame + 80}));
    if (int32_t(r3) < 0) {
      return r3;
    }
    io.set_allocator(guest::LoadU32(base, frame + 80));
    return r3;
  }
  call(Method(base, allocator, 0), 0x824606FC, Args({allocator}));
  call(Method(base, allocator, 8), 0x82460714, Args({allocator, size}));
  return call(Method(base, allocator, 12), 0x8246072C, Args({allocator, attributes}));
}

// --- The voices with outputs (lock-free) ---

uint32_t ConstructOutputs(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                          const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460738 XAudioVoice::ConstructOutputs");
  VoiceConstruct(base, voice, allocator, type, call);
  Voice(base, voice).set_vtable(kOutputsVtable);
  return voice;
}

uint32_t OutputsSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824607C8 XAudioVoice::OutputsSize");
  const Description d(base, description);
  SetRegs(call, Args({description, frame + 84}));
  uint32_t r3 = BaseSize(base, description, frame + 84, Below(base, frame, 176), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  // The matrix object's description: type 1, the outputs' channels at +8.
  const uint32_t local = frame + 96;
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, local + i, 0);
  }
  guest::StoreU8(base, local + 8, d.output_channels());
  guest::StoreU8(base, local, 1);
  const uint32_t factory = G::engine(base).factory();
  uint32_t function = 0;
  const uint32_t object = FactorySizeThunk(base, factory, &function);
  r3 = call(function, 0x82460834, At(frame, Args({object, local, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t count = d.output_count();
  schema::XAudioSizeOut(base, out)
      .set_size(count * guest::LoadU32(base, frame + 80) + guest::LoadU32(base, frame + 84) +
                count * 12);
  return r3;
}

uint32_t SetOutput(uint8_t* base, uint32_t voice, uint32_t entry, uint32_t slot, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460998 XAudioVoice::SetOutput");
  // r4 and r5 still the arguments at the first call (its caller may be
  // plain C++, whose calls left others).
  const uint32_t entry_arg = entry;
  if (entry == 0) {
    entry = G::default_output_address();
  }
  const Slot s(base, slot);
  // The entry (8 bytes of XAudioOutputList's: +0 the target, +4 the levels).
  uint32_t target = guest::LoadU32(base, entry);
  if (target == 0) {
    target = G::engine(base).mastering_voice().address;
  }
  s.set_target(target);
  call(Method(base, target, 4), 0x824609F0, At(frame, Args({target, entry_arg, slot})));
  const uint32_t matrix = guest::LoadU32(base, entry + 4);
  const uint32_t object = s.matrix().address;
  if (matrix != 0) {
    return call(Method(base, object, 40), 0x82460A50, At(frame, Args({object, matrix})));
  }
  const uint32_t output = s.target().address;
  const uint32_t r3 = call(Method(base, output, 64), 0x82460A1C, At(frame, Args({output, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t defaults = DefaultMatrix(base, Voice(base, voice).channels(),
                                          guest::LoadU8(base, frame + 81), frame + 88);
  const uint32_t matrix_object = s.matrix().address;
  return call(Method(base, matrix_object, 40), 0x82460A50,
              At(frame, Args({matrix_object, defaults, frame + 88})));
}

uint32_t SetOutputVoices(uint8_t* base, uint32_t voice, uint32_t outputs, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460BF0 XAudioVoice::SetOutputVoices");
  const Voice v(base, voice);
  const schema::XAudioOutputList list(base, outputs);
  ReleaseOutputs(base, voice, call);
  const uint32_t below = Below(base, frame, 128);
  if (outputs == 0) {
    if (v.output_slots() == 0) {
      return 0;
    }
    SetRegs(call, Args({voice, 0, v.outputs().address}));
    const uint32_t r3 = SetOutput(base, voice, 0, v.outputs().address, below, call);
    if (int32_t(r3) < 0) {
      return r3;
    }
    v.set_output_count(1);
    return r3;
  }
  uint32_t r26 = 0;
  for (uint32_t i = 0; i < list.count(); i++) {
    if (int32_t(r26) < 0) {
      return r26;
    }
    SetRegs(call, Args({voice, list.entries() + 8 * i, OutputSlot(v, i).address}));
    r26 = SetOutput(base, voice, list.entries() + 8 * i, OutputSlot(v, i).address, below, call);
  }
  if (int32_t(r26) < 0) {
    return r26;
  }
  v.set_output_count(list.count());
  return r26;
}

uint32_t Pass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461070 XAudioVoice::Pass");
  guest::StoreU32(base, frame + 84,
                  call(Method(base, voice, 72), 0x82461094, At(frame, Args({voice}))));
  vehicle_accessors::DebugSwitchOff();
  const Voice v(base, voice);
  const uint32_t callback = v.callback();
  if (callback != 0) {
    guest::StoreU32(base, frame + 80, v.callback_context());
    call(callback, 0x824610C0, At(frame, Args({frame + 80})));
  }
  uint32_t r3;
  if ((v.flags() & 4) != 0 && v.tail_left() == 0) {
    r3 = call(Method(base, voice, 60), 0x8246113C, At(frame, Args({voice, 1})));
  } else {
    if ((v.flags() & 4) != 0) {
      v.set_tail_left(uint16_t(v.tail_left() - 1));
    }
    r3 = call(Method(base, voice, 84), 0x82461100, At(frame, Args({voice, frame + 84})));
  }
  vehicle_accessors::DebugSwitchOff();
  return r3;
}

uint32_t Destroy(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461240 XAudioVoice::Destroy");
  const Voice v(base, voice);
  v.set_vtable(kVoiceVtable);
  for (uint32_t i = 0; i < v.effect_count(); i = (i + 1) & 0xFF) {
    const uint32_t effect = Effect(v, i).object().address;
    if (effect == 0) {
      continue;
    }
    call(Method(base, effect, 4), 0x82461298, Args({effect}));
    Effect(v, i).set_object(0u);
  }
  const uint32_t buffer = v.buffer();
  if (buffer != 0) {
    uint32_t function = 0;
    const uint32_t object = BufferReleaseThunk(base, buffer, &function);
    call(function, 0x824612C4, Args({object}));
    v.set_buffer(0);
  }
  const uint32_t allocator = v.allocator().address;
  v.set_vtable(kVoiceVtable0);
  uint32_t r3 = 0;
  if (allocator != 0) {
    r3 = call(Method(base, allocator, 4), 0x824612F0, Args({allocator}));
    v.set_allocator(0u);
  }
  v.set_vtable(kRootVtable);
  return r3;
}

uint32_t CreateObject(uint8_t* base, uint32_t voice, uint32_t entry, uint32_t description,
                      uint32_t flag, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461308 XAudioVoice::CreateObject");
  const Voice v(base, voice);
  const EffectEntry e(base, entry);
  const uint32_t factory = G::engine(base).factory();
  e.set_object(0u);
  uint32_t function = 0;
  const uint32_t object = FactoryCreateThunk(base, factory, &function);
  uint32_t r3 = call(function, 0x82461344,
                     At(frame, Args({object, description, v.allocator().address, entry})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t made = e.object().address;
  r3 = call(Method(base, made, 8), 0x82461364, At(frame, Args({made, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint16_t size = guest::LoadU16(base, frame + 82);
  e.set_index(uint8_t(flag));
  e.set_enabled(1);
  e.set_flags(guest::LoadU8(base, frame + 80));
  if (size > v.tail()) {
    v.set_tail(size);
  }
  return r3;
}

uint32_t ProcessEffect(uint8_t* base, uint32_t entry, uint32_t inout, uint32_t processor,
                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461678 XAudioVoice::ProcessEffect");
  const EffectEntry e(base, entry);
  const schema::XAudioBufferInOut io(base, inout);
  if ((e.enabled() & 1) == 0) {
    return 0;
  }
  const uint8_t flags = e.flags();
  uint32_t r4;
  uint32_t r5;
  if ((flags & 6) == 6) {
    // In place to the processor's other buffer of its pair.
    const Engine engine = G::engine(base);
    const uint32_t buffers = engine.mix_buffers_address() + 8 * processor;
    r4 = io.buffer();
    r5 = guest::LoadU32(base, buffers);
    if (r4 != 0) {
      for (uint32_t k = 0; k < 2; k++) {
        if (r4 == guest::LoadU32(base, buffers + 4 * k)) {
          r5 = guest::LoadU32(base, buffers + 4 * ((k - 1) & 1));
          break;
        }
      }
    }
    io.set_buffer(r5);
  } else if ((flags & 3) != 0) {
    r4 = io.buffer();
    r5 = 0;
  } else if ((flags & 4) != 0) {
    r5 = io.buffer();
    r4 = 0;
  } else {
    r4 = guest::LoadU32(base, frame + 80);
    r5 = r4;
  }
  const uint32_t effect = e.object().address;

  return call(Method(base, effect, 24), 0x82461788, At(frame, Args({effect, r4, r5})));
}

// --- Making a voice and its passes (lock-free) ---

namespace {

// memcpy's plain C++, or its original (through the translated code, at
// `sp`) where that refuses.
void Copy(uint8_t* base, uint32_t destination, uint32_t source, uint32_t size,
          uint32_t return_address, uint32_t sp, const GuestCall& call) {
  SetRegs(call, Args({destination, source, size}));
  if (!c_runtime::Memcpy(base, destination, source, size)) {
    call(kMemcpy, return_address, At(sp, Args({destination, source, size})));
  }
}

// A float word copied through a float register, as lfs and stfs do in the
// translated code (a signaling NaN comes out quiet).
void CopyFloat(uint8_t* base, uint32_t destination, uint32_t source) {
  volatile double value = guest::LoadFloat(base, source);
  guest::StoreFloat(base, destination, float(value));
}

// The codec's description (SourceSize's and SourceInit's) at `local`.
void CodecDescription(uint8_t* base, uint32_t description, uint32_t local,
                      uint32_t return_address, uint32_t sp, const GuestCall& call) {
  const schema::XAudioSourceDescription d(base, description);
  for (uint32_t i = 0; i < 80; i += 4) {
    guest::StoreU32(base, local + i, 0);
  }
  guest::StoreU8(base, local, 0);
  guest::StoreU32(base, local + 4, d.callback_context());
  Copy(base, local + 8, d.format_address(), 56, return_address, sp, call);
  CopyFloat(base, local + 64, d.codec_float_address());
  guest::StoreU8(base, local + 68, d.codec_byte());
  guest::StoreU8(base, local + 69, d.codec_flag());
  guest::StoreU32(base, local + 72, d.codec_word_a());
  guest::StoreU32(base, local + 76, d.codec_word_b());
}

}  // namespace

uint32_t SourceSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E0D8 XAudioSourceVoice::Size");
  const uint32_t factory = G::engine(base).factory();
  SourceDescribe(base, description, frame + 96);
  SetRegs(call, Args({frame + 96, frame + 80}));
  if (int32_t(OutputsSize(base, frame + 96, frame + 80, Below(base, frame, 144), call)) < 0) {
    return 0;
  }
  CodecDescription(base, description, frame + 128, 0x8245E14C, frame, call);
  uint32_t function = 0;
  const uint32_t object = FactorySizeThunk(base, factory, &function);
  const uint32_t r3 = call(function, 0x8245E184, At(frame, Args({object, frame + 128, frame + 84})));
  if (int32_t(r3) >= 0) {
    schema::XAudioSizeOut(base, out)
        .set_size(guest::LoadU32(base, frame + 84) + guest::LoadU32(base, frame + 80));
  }
  return 0;
}

uint32_t CreateEffects(uint8_t* base, uint32_t voice, uint32_t chain, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461600 XAudioVoice::CreateEffects");
  const Voice v(base, voice);
  const schema::XAudioEffectChain c(base, chain);
  uint32_t r3 = 0;
  const uint32_t below = Below(base, frame, 128);
  for (uint32_t i = 0; i < c.count(); i = (i + 1) & 0xFF) {
    if (int32_t(r3) < 0) {
      break;
    }
    SetRegs(call, Args({voice, Effect(v, i).address, guest::LoadU32(base, c.ids() + 4 * i), i}));
    r3 = CreateObject(base, voice, Effect(v, i).address, guest::LoadU32(base, c.ids() + 4 * i), i,
                      below, call);
  }
  return r3;
}

uint32_t InitBase(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461D50 XAudioVoice::InitBase");
  const Voice v(base, voice);
  const Description d(base, description);
  v.set_callback(d.callback());
  v.set_callback_context(d.callback_context());
  v.set_format(d.format());
  v.set_rate(d.rate());
  const schema::XAudioEffectChain chain = d.effect_chain();
  v.set_effect_count(chain ? chain.count() : 0);
  uint32_t r28 = 0;
  const uint32_t buffer = d.buffer();
  if (buffer != 0) {
    v.set_buffer(buffer);
    uint32_t function = 0;
    const uint32_t object = BufferAddRefThunk(base, buffer, &function);
    call(function, 0x82461DCC, At(frame, Args({object})));
  } else {
    // A format of the description's channels at 48 kHz.
    const uint32_t local = frame + 80;
    for (uint32_t i = 0; i < 20; i += 4) {
      guest::StoreU32(base, local + i, 0);
    }
    guest::StoreU8(base, local + 5, d.channels());
    guest::StoreU32(base, local + 8, kSampleRate);
    SetRegs(call, Args({local, v.allocator().address, v.buffer_address()}));
    r28 = BufferCreate(base, local, v.allocator().address, v.buffer_address(), call);
    if (int32_t(r28) < 0) {
      return r28;
    }
  }
  const uint32_t effects = v.effect_count();
  if (effects == 0) {
    return r28;
  }
  const uint32_t allocator = v.allocator().address;
  const uint32_t entries =
      call(Method(base, allocator, 20), 0x82461E3C, At(frame, Args({allocator, effects * 8})));
  v.set_effects(entries);
  if (entries == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  if (v.effect_count() == 0) {
    return 0;
  }
  SetRegs(call, Args({voice, d.effect_chain().address}));
  return CreateEffects(base, voice, d.effect_chain().address, Below(base, frame, 112), call);
}

uint32_t InitOutputs(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460880 XAudioVoice::InitOutputs");
  const Voice v(base, voice);
  const Description d(base, description);
  v.set_output_slots(d.output_count());
  SetRegs(call, Args({voice, description}));
  uint32_t r3 = InitBase(base, voice, description, Below(base, frame, 144), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t outputs = v.output_slots();
  if (outputs != 0) {
    const uint32_t allocator = v.allocator().address;
    const uint32_t slots =
        call(Method(base, allocator, 20), 0x824608D8, At(frame, Args({allocator, outputs * 12})));
    v.set_outputs(slots);
    if (slots == 0) {
      return 0x8007000Eu;  // E_OUTOFMEMORY
    }
    r3 = 0;
  }
  if (v.output_slots() != 0) {
    // The matrix objects' description: type 1, +4 the voice, +8 the
    // outputs' channels.
    const uint32_t local = frame + 80;
    for (uint32_t i = 0; i < 16; i += 4) {
      guest::StoreU32(base, local + i, 0);
    }
    guest::StoreU8(base, local + 8, d.output_channels());
    guest::StoreU32(base, local + 4, voice);
    guest::StoreU8(base, local, 1);
    const uint32_t below = Below(base, frame, 128);
    for (uint32_t i = 0; i < v.output_slots(); i++) {
      if (int32_t(r3) < 0) {
        return r3;
      }
      SetRegs(call, Args({voice, OutputSlot(v, i).matrix_address(), local, 255}));
      r3 = CreateObject(base, voice, OutputSlot(v, i).matrix_address(), local, 255, below, call);
    }
  }
  if (int32_t(r3) < 0) {
    return r3;
  }
  return call(Method(base, voice, 36), 0x8246097C,
              At(frame, Args({voice, d.output_voices().address})));
}

uint32_t SourceInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E348 XAudioSourceVoice::Init");
  const Source s(base, voice);
  const schema::XAudioSourceDescription d(base, description);
  Copy(base, s.format_address(), d.format_address(), 56, 0x8245E370, frame, call);
  s.set_category(d.category());
  SourceDescribe(base, description, frame + 80);
  SetRegs(call, Args({voice, frame + 80}));
  const uint32_t r3 = InitOutputs(base, voice, frame + 80, Below(base, frame, 144), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  CodecDescription(base, description, frame + 112, 0x8245E3D0, frame, call);
  SetRegs(call, Args({voice, s.codec_address(), frame + 112, 255}));
  return CreateObject(base, voice, s.codec_address(), frame + 112, 255, Below(base, frame, 128),
                      call);
}

uint32_t SubmixInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t frame,
                    const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F230 XAudioSubmixVoice::Init");
  SubmixDescribe(base, description, frame + 80);
  SetRegs(call, Args({voice, frame + 80}));
  const uint32_t r3 = InitOutputs(base, voice, frame + 80, Below(base, frame, 144), call);
  if (int32_t(r3) >= 0) {
    const uint32_t buffer = Submix(base, voice).buffer();
    SetRegs(call, Args({buffer, 0, 3}));
    if (!BufferClearThunk(base, buffer, 0, 3)) {
      call(0x8245FA10, 0x8245F27C, At(frame, Args({buffer, 0, 3})));
    }
  }
  return r3;
}

uint32_t ProcessEffects(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461E88 XAudioVoice::ProcessEffects");
  const Voice v(base, voice);
  uint32_t r3 = 0;
  const uint32_t below = Below(base, frame, 96);
  for (uint32_t i = 0; i < v.effect_count(); i = (i + 1) & 0xFF) {
    if (int32_t(r3) < 0) {
      break;
    }
    SetRegs(call, Args({voice, Effect(v, i).address, inout}));
    r3 = ProcessEffect(base, Effect(v, i).address, inout, processor, below, call);
  }
  return r3;
}

uint32_t MixToOutputs(uint8_t* base, uint32_t voice, uint32_t inout, uint32_t processor,
                      uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460CB8 XAudioVoice::MixToOutputs");
  SetRegs(call, Args({voice, inout}));
  uint32_t r3 = ProcessEffects(base, voice, inout, processor, Below(base, frame, 112), call);
  const Voice v(base, voice);
  for (uint32_t i = 0; i < v.output_count(); i++) {
    if (int32_t(r3) < 0) {
      break;
    }
    const uint32_t target = OutputSlot(v, i).target().address;
    const uint32_t buffer = call(Method(base, target, 72), 0x82460D08, At(frame, Args({target})));
    const uint32_t matrix = OutputSlot(v, i).matrix().address;
    r3 = call(Method(base, matrix, 24), 0x82460D30,
              At(frame, Args({matrix, schema::XAudioBufferInOut(base, inout).buffer(), buffer})));
  }
  return r3;
}

uint32_t SubmixPass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245EFA0 XAudioSubmixVoice::Pass");
  const uint32_t r30 = Pass(base, voice, Below(base, frame, 112), call);
  if (int32_t(r30) < 0) {
    return r30;
  }
  const Voice v(base, voice);
  const uint32_t iface = v.buffer();
  const uint32_t buffer = iface != 0 ? iface - 8 : 0;
  SetRegs(call, Args({buffer, v.format_address(), 3}));
  if (!BufferClear(base, buffer, v.format_address(), 3)) {
    call(0x8245F8E8, 0x8245EFE8, At(frame, Args({buffer, v.format_address(), 3})));
  }

  SetRegs(call, Args({1, 1}));
  AddStat(base, 1, 1);
  return r30;
}

// --- The game's locks, taken as the original does ---

void SpinTake(uint8_t* base, uint32_t thread, const GuestCall& call) {
  const uint32_t irql = call(kKeRaiseIrqlToDpcLevel, 0, Regs{});
  uint32_t depth = G::spin_depth(base);
  if (depth == 0 || thread != G::spin_owner(base)) {
    call(kKeAcquireSpinLockAtRaisedIrql, 0, Args({G::spin_lock_address()}));
    G::set_spin_owner(base, thread);
    G::set_spin_irql(base, uint8_t(irql));
    depth = G::spin_depth(base);
  }
  G::set_spin_depth(base, depth + 1);
}

void SpinRelease(uint8_t* base, uint32_t thread, const GuestCall& call) {
  const uint32_t depth = G::spin_depth(base);
  if (depth == 0 || thread != G::spin_owner(base)) {
    return;
  }
  G::set_spin_depth(base, depth - 1);
  if (depth - 1 != 0) {
    return;
  }
  const uint32_t irql = G::spin_irql(base);
  G::set_spin_irql(base, 0);
  G::set_spin_owner(base, 0);
  call(kKeReleaseSpinLockFromRaisedIrql, 0, Args({G::spin_lock_address()}));
  call(kKfLowerIrql, 0, Args({irql}));
}

namespace {

// The node at `node` added at the end of the list at `list` (+0 next, +4
// prev), as the voices' inline code does.
void LinkAtEnd(uint8_t* base, uint32_t list, uint32_t node) {
  guest::StoreU32(base, node, list);
  guest::StoreU32(base, node + 4, guest::LoadU32(base, list + 4));
  guest::StoreU32(base, list + 4, node);
  guest::StoreU32(base, guest::LoadU32(base, node + 4), node);
}

// The node at `node` out of its list when it's in one, left pointing at
// itself.
void Unlink(uint8_t* base, uint32_t node) {
  const uint32_t next = guest::LoadU32(base, node);
  if (next == node) {
    return;
  }
  guest::StoreU32(base, next + 4, guest::LoadU32(base, node + 4));
  guest::StoreU32(base, guest::LoadU32(base, node + 4), guest::LoadU32(base, node));
  guest::StoreU32(base, node + 4, node);
  guest::StoreU32(base, node, node);
}

}  // namespace

// --- Under the voice lists' spin lock ---

uint32_t SourceGetState(uint8_t* base, uint32_t voice, uint32_t out, uint32_t thread,
                        uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DDC8 XAudioSourceVoice::GetState");
  SpinTake(base, thread, call);
  const Source s(base, voice);
  const uint32_t codec = s.codec().address;
  const uint32_t r3 = call(Method(base, codec, 44), 0x8245DE3C, At(frame, Args({codec, frame + 80})));
  if (int32_t(r3) >= 0) {
    schema::XAudioByteOut(base, out).set_value(s.flags() | guest::LoadU8(base, frame + 80));
  }
  SpinRelease(base, thread, call);
  return r3;
}

uint32_t Play(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461798 XAudioVoice::Play");
  vehicle_accessors::DebugSwitchOff();
  const Voice v(base, voice);
  SpinTake(base, thread, call);
  if ((v.flags() & 1) != 0) {
    SpinTake(base, thread, call);
    v.set_flags(v.flags() & 0xFB);
    SpinRelease(base, thread, call);
  } else {
    SpinTake(base, thread, call);
    v.set_flags((v.flags() & 0xBE) | 1);
    SpinRelease(base, thread, call);
    call(Method(base, voice, 76), 0x8246194C, Args({voice}));
  }
  SpinRelease(base, thread, call);
  vehicle_accessors::DebugSwitchOff();
  return 0;
}

uint32_t Activate(uint8_t* base, uint32_t voice, uint32_t thread, uint32_t frame,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460EF0 XAudioVoice::Activate");
  const Voice v(base, voice);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < v.output_count(); i++) {
    if (int32_t(r3) < 0) {
      return r3;
    }
    const uint32_t target = OutputSlot(v, i).target().address;
    const Voice t(base, target);
    t.set_output_uses(t.output_uses() + 1);
    r3 = call(Method(base, target, 56), 0x82460F44, At(frame, Args({target})));
  }
  if (int32_t(r3) < 0) {
    return r3;
  }
  const Engine engine = G::engine(base);
  call(Method(base, voice, 52), 0x82460F80, At(frame, Args({voice, frame + 80})));
  const uint8_t type = guest::LoadU8(base, frame + 80);
  uint32_t lists;
  if (type == 0) {
    lists = engine.source_stage_address();
  } else if (type == 1) {
    lists = engine.submix_stages() + 44 * v.stage();
  } else {
    return 0;
  }
  if (lists == 0) {
    return 0;
  }
  SpinTake(base, thread, call);
  const uint32_t list = guest::LoadU32(base, lists + 40);
  LinkAtEnd(base, list, voice + guest::LoadU32(base, list + 8));
  SpinRelease(base, thread, call);
  return 0;
}

uint32_t Deactivate(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460D50 XAudioVoice::Deactivate");
  const Voice v(base, voice);
  SpinTake(base, thread, call);
  Unlink(base, v.active_next_address());
  SpinRelease(base, thread, call);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < v.output_count(); i++) {
    if (int32_t(r3) < 0) {
      break;
    }
    const uint32_t target = OutputSlot(v, i).target().address;
    const Voice t(base, target);
    const uint32_t uses = t.output_uses() - 1;
    t.set_output_uses(uses);
    r3 = uses != 0 ? 0 : call(Method(base, target, 60), 0x82460E74, Args({target, 0}));
  }
  return r3;
}

uint32_t StartLater(uint8_t* base, uint32_t voice, uint32_t thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E7D0 XAudioSourceVoice::StartLater");
  const Source s(base, voice);
  uint32_t r3 = 0;
  SpinTake(base, thread, call);
  const uint8_t flags = s.flags();
  if ((flags & 1) != 0) {
    r3 = 0x8000FFFFu;  // E_UNEXPECTED
  } else if ((flags & 0x40) == 0) {
    SpinTake(base, thread, call);
    s.set_flags(s.flags() | 0x40);
    SpinRelease(base, thread, call);
    const Engine engine = G::engine(base);
    SpinTake(base, thread, call);
    const uint32_t list = engine.pending_next_address();
    LinkAtEnd(base, list, voice + guest::LoadU32(base, list + 8));
    SpinRelease(base, thread, call);
  }
  SpinRelease(base, thread, call);
  return r3;
}

void WaitForVoice(uint8_t* base, uint32_t engine, uint32_t voice, uint32_t thread,
                  uint32_t mixer_thread, uint32_t processor, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245AE60 XAudio::WaitForVoice");
  const Engine e(base, engine);
  if (mixer_thread == e.mixer_thread() ||
      mixer_thread == guest::LoadU32(base, e.workers_address() + 4 * processor)) {
    return;
  }
  for (;;) {
    bool working = false;
    for (uint32_t k = 0; k < 6; k++) {
      if (voice == guest::LoadU32(base, e.working_voices_address() + 4 * k)) {
        working = true;
        break;
      }
    }
    if (!working) {
      return;
    }
    SpinRelease(base, thread, call);
    const uint32_t count = G::api_lock_recursion(base);
    for (uint32_t i = 0; i < count; i++) {
      call(kRtlLeaveCriticalSection, 0, Args({kApiLock}));
    }
    for (uint32_t i = 0; i < count; i++) {
      call(kRtlEnterCriticalSection, 0, Args({kApiLock}));
    }
    SpinTake(base, thread, call);
  }
}

uint32_t CancelPendingStart(uint8_t* base, uint32_t voice, uint32_t thread,
                            uint32_t mixer_thread, uint32_t processor, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E428 XAudioSourceVoice::CancelPendingStart");
  const Source s(base, voice);
  SpinTake(base, thread, call);
  if ((s.flags() & 0x40) == 0) {
    SpinRelease(base, thread, call);
    return 0x80004004u;
  }
  SetRegs(call, Args({G::engine(base).address, voice}));
  WaitForVoice(base, G::engine(base).address, voice, thread, mixer_thread, processor, call);
  const Engine engine = G::engine(base);
  SpinTake(base, thread, call);
  Unlink(base, voice + engine.pending_node_offset());
  SpinRelease(base, thread, call);
  SpinTake(base, thread, call);
  s.set_flags(s.flags() & 0xBF);
  SpinRelease(base, thread, call);
  SpinRelease(base, thread, call);
  return 0;
}

uint32_t StopOrTail(uint8_t* base, uint32_t voice, uint32_t flags, uint32_t thread,
                    uint32_t mixer_thread, uint32_t processor, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824619B0 XAudioVoice::StopOrTail");
  vehicle_accessors::DebugSwitchOff();
  const Voice v(base, voice);
  SpinTake(base, thread, call);
  if ((flags & 1) != 0) {
    SetRegs(call, Args({G::engine(base).address, voice}));
    WaitForVoice(base, G::engine(base).address, voice, thread, mixer_thread, processor, call);
    if ((v.flags() & 1) != 0) {
      SpinTake(base, thread, call);
      v.set_flags(v.flags() & 0xBA);
      SpinRelease(base, thread, call);
      // r4 still the voice (WaitForVoice's argument; the kernel calls keep it).
      call(Method(base, voice, 80), 0x82461B44, Args({voice, voice}));
    }
    SpinRelease(base, thread, call);
  } else if ((v.flags() & 1) == 0) {
    SpinRelease(base, thread, call);
  } else {
    SpinTake(base, thread, call);
    v.set_flags(v.flags() | 4);
    SpinRelease(base, thread, call);
    SpinRelease(base, thread, call);
    v.set_tail_left(v.tail());
  }
  vehicle_accessors::DebugSwitchOff();
  return 0;
}

// --- Starting, stopping, passes, making and destroying voices ---

uint32_t SourceStart(uint8_t* base, uint32_t voice, uint32_t flags, const Thread& thread,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245EBA0 XAudioSourceVoice::Start");
  if ((flags & 1) != 0) {
    return StartLater(base, voice, thread.r13, call);
  }
  uint32_t r3 = CancelPendingStart(base, voice, thread.r13, thread.mixer, thread.processor, call);
  if (r3 != 0x80004004u && int32_t(r3) < 0) {
    return r3;
  }
  call(Method(base, voice, 92), 0x8245EC08, Args({voice}));
  const uint32_t codec = Source(base, voice).codec().address;
  r3 = call(Method(base, codec, 48), 0x8245EC1C, Args({codec}));
  if (int32_t(r3) < 0) {
    return r3;
  }
  return Play(base, voice, thread.r13, call);
}

uint32_t SourceStop(uint8_t* base, uint32_t voice, uint64_t flags, const Thread& thread,
                    uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E9F0 XAudioSourceVoice::Stop");
  uint32_t r3 = CancelPendingStart(base, voice, thread.r13, thread.mixer, thread.processor, call);
  if (r3 != 0x80004004u) {
    return r3;
  }
  uint64_t stop = flags;
  const Source s(base, voice);
  const uint32_t codec = s.codec().address;
  if (codec != 0) {
    r3 = call(Method(base, codec, 52), 0x8245EA38, At(frame, Args({codec, flags})));
    if (int32_t(r3) < 0) {
      return r3;
    }
    if ((flags & 1) == 0) {
      const uint32_t again = s.codec().address;
      r3 = call(Method(base, again, 44), 0x8245EA68, At(frame, Args({again, frame + 80})));
      if (int32_t(r3) < 0) {
        return r3;
      }
      if ((guest::LoadU8(base, frame + 80) & 1) == 0) {
        stop = (flags & 0xFF) | 1;
      }
    }
  }
  SetRegs(call, Args({voice, stop}));
  return StopOrTail(base, voice, uint32_t(stop), thread.r13, thread.mixer, thread.processor, call);
}

uint32_t SourcePass(uint8_t* base, uint32_t voice, uint32_t inout, const Thread& thread,
                    uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DED0 XAudioSourceVoice::Pass");
  const Source s(base, voice);
  const uint32_t category = s.category();
  const uint32_t bit = category < 32 ? 1u << category : 0;  // slw: 0 from 32 up
  if ((bit & G::engine(base).changed_categories()) != 0) {
    const uint32_t r3 = call(Method(base, voice, 92), 0x8245DF18, At(frame, Args({voice})));
    if (int32_t(r3) < 0) {
      return r3;
    }
  }
  SetRegs(call, Args({voice, s.codec_address(), inout}));
  uint32_t r3 =
      ProcessEffect(base, s.codec_address(), inout, thread.processor, Below(base, frame, 96), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  SetRegs(call, Args({voice, inout}));
  r3 = MixToOutputs(base, voice, inout, thread.processor, Below(base, frame, 128), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t codec = s.codec().address;
  const uint32_t state = call(Method(base, codec, 44), 0x8245DF68, At(frame, Args({codec, frame + 80})));
  if (int32_t(state) < 0) {
    return state;
  }
  SetRegs(call, Args({2, 1}));
  AddStat(base, 2, 1);
  if ((guest::LoadU8(base, frame + 80) & 1) != 0 || (s.flags() & 4) != 0) {
    return state;
  }
  SetRegs(call, Args({voice, 0}));
  return StopOrTail(base, voice, 0, thread.r13, thread.mixer, thread.processor, call);
}

uint32_t SourceConstruct(uint8_t* base, uint32_t voice, uint32_t allocator, uint32_t type,
                         const Thread& thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E6A8 XAudioSourceVoice::XAudioSourceVoice");
  ConstructOutputs(base, voice, allocator, type, call);
  const Source s(base, voice);
  s.set_vtable(kSourceVtable);
  CopyFloat(base, s.volume_address(), G::one_address());
  const uint32_t list = G::engine(base).source_stage_address();
  SpinTake(base, thread.r13, call);
  LinkAtEnd(base, list, voice + guest::LoadU32(base, list + 8));
  SpinRelease(base, thread.r13, call);
  return voice;
}

uint32_t SubmixConstruct(uint8_t* base, uint32_t voice, uint32_t description,
                         uint32_t allocator, const Thread& thread, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F298 XAudioSubmixVoice::XAudioSubmixVoice");
  SetRegs(call, Args({voice, allocator, 1}));
  ConstructOutputs(base, voice, allocator, 1, call);
  const Submix m(base, voice);
  m.set_vtable(kSubmixVtable);
  const uint8_t stage = schema::XAudioSubmixDescription(base, description).stage();
  m.set_stage(stage);
  const uint32_t list = 44 * stage + G::engine(base).submix_stages();
  SpinTake(base, thread.r13, call);
  LinkAtEnd(base, list, voice + guest::LoadU32(base, list + 8));
  SpinRelease(base, thread.r13, call);
  return voice;
}

uint32_t OutputsDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460B50 XAudioVoice::~XAudioVoiceOutputs");
  const Voice v(base, voice);
  v.set_vtable(kOutputsVtable);
  SetRegs(call, Args({voice, 1}));
  StopOrTail(base, voice, 1, thread.r13, thread.mixer, thread.processor, call);
  SetRegs(call, Args({voice}));
  ReleaseOutputs(base, voice, call);
  for (uint32_t i = 0; i < v.output_slots(); i++) {
    const uint32_t matrix = OutputSlot(v, i).matrix().address;
    if (matrix == 0) {
      continue;
    }
    call(Method(base, matrix, 4), 0x82460BBC, Args({matrix}));
    OutputSlot(v, i).set_matrix(0u);
  }
  SetRegs(call, Args({voice}));
  return Destroy(base, voice, call);
}

uint32_t SourceDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E1B8 XAudioSourceVoice::~XAudioSourceVoice");
  const Source s(base, voice);
  s.set_vtable(kSourceVtable);
  const uint32_t r3 = call(Method(base, voice, 60), 0x8245E1E4, Args({voice, 1}));
  if (int32_t(r3) >= 0) {
    const uint32_t codec = s.codec().address;
    if (codec != 0) {
      call(Method(base, codec, 32), 0x8245E208, Args({codec}));
    }
  }
  const Engine engine = G::engine(base);
  SpinTake(base, thread.r13, call);
  Unlink(base, voice + engine.source_node_offset());
  SpinRelease(base, thread.r13, call);
  const uint32_t codec = s.codec().address;
  if (codec != 0) {
    call(Method(base, codec, 4), 0x8245E2F0, Args({codec}));
    s.set_codec(0u);
  }
  SetRegs(call, Args({voice}));
  return OutputsDestroy(base, voice, thread, call);
}

uint32_t SourceDeletingDestructor(uint8_t* base, uint32_t voice, const Thread& thread,
                                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245E7A0 XAudioSourceVoice::DeletingDestructor");
  SourceDestroy(base, voice, thread, call);
  return voice;
}

uint32_t SubmixDeletingDestructor(uint8_t* base, uint32_t voice, const Thread& thread,
                                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F3A0 XAudioSubmixVoice::DeletingDestructor");
  SubmixDestroy(base, voice, thread, call);
  return voice;
}

uint32_t SubmixDestroy(uint8_t* base, uint32_t voice, const Thread& thread,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F108 XAudioSubmixVoice::~XAudioSubmixVoice");
  const Submix m(base, voice);
  m.set_vtable(kSubmixVtable);
  SetRegs(call, Args({voice, 1}));
  StopOrTail(base, voice, 1, thread.r13, thread.mixer, thread.processor, call);
  const uint32_t list = 44 * m.stage() + G::engine(base).submix_stages();
  SpinTake(base, thread.r13, call);
  Unlink(base, voice + guest::LoadU32(base, list + 8));
  SpinRelease(base, thread.r13, call);
  SetRegs(call, Args({voice}));
  return OutputsDestroy(base, voice, thread, call);
}

namespace {

// The allocator for a new voice of `size` bytes at [inout] (GetAllocator's
// plain C++, or its original through the translated code where that
// can't run).
uint32_t VoiceAllocator(uint8_t* base, uint32_t size, uint32_t inout, bool native_heap,
                        uint32_t return_address, uint32_t frame, const GuestCall& call) {
  SetRegs(call, Args({kVoiceAttributes, size, inout}));
  if (const auto r3 = GetAllocator(base, kVoiceAttributes, size, inout, Below(base, frame, 128),
                                   native_heap, call)) {
    return *r3;
  }
  return call(kGetAllocator, return_address, At(frame, Args({kVoiceAttributes, size, inout})));
}

}  // namespace

uint32_t CreateSource(uint8_t* base, uint32_t description, uint32_t allocator, uint32_t out,
                      bool native_heap, const Thread& thread, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245EAA0 XAudioVoice::CreateSource");
  const uint32_t slot = frame + 156;  // the caller's parameter word
  guest::StoreU32(base, slot, allocator);
  SetRegs(call, Args({description, frame + 80}));
  uint32_t r31 = SourceSize(base, description, frame + 80, Below(base, frame, 240), call);
  if (int32_t(r31) >= 0) {
    r31 = VoiceAllocator(base, guest::LoadU32(base, frame + 80) + 148, slot, native_heap,
                         0x8245EAF4, frame, call);
  }
  if (int32_t(r31) >= 0) {
    const uint32_t owner = guest::LoadU32(base, slot);
    const uint32_t memory = call(Method(base, owner, 20), 0x8245EB18, At(frame, Args({owner, 148})));
    uint32_t voice = 0;
    if (memory != 0) {
      SetRegs(call, Args({memory, guest::LoadU32(base, slot), 0}));
      voice = SourceConstruct(base, memory, guest::LoadU32(base, slot), 0, thread, call);
    }
    if (voice == 0) {
      r31 = 0x8007000Eu;  // E_OUTOFMEMORY
    } else {
      SetRegs(call, Args({voice, description}));
      r31 = SourceInit(base, voice, description, Below(base, frame, 224), call);
      if (int32_t(r31) >= 0) {
        schema::XAudioVoiceOut(base, out).set_voice(voice);
      } else {
        call(Method(base, voice, 12), 0x8245EB78, At(frame, Args({voice})));
      }
    }
  }
  const uint32_t owner = guest::LoadU32(base, slot);
  if (owner != 0) {
    call(Method(base, owner, 4), 0x8245EB94, At(frame, Args({owner})));
  }
  return r31;
}

uint32_t CreateSubmix(uint8_t* base, uint32_t description, uint32_t allocator, uint32_t out,
                      bool native_heap, const Thread& thread, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F3D0 XAudioVoice::CreateSubmix");
  const uint32_t slot = frame + 204;  // the caller's parameter word
  guest::StoreU32(base, slot, allocator);
  SubmixDescribe(base, description, frame + 96);
  SetRegs(call, Args({frame + 96, frame + 80}));
  uint32_t r30 = OutputsSize(base, frame + 96, frame + 80, Below(base, frame, 144), call);
  if (int32_t(r30) >= 0) {
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, frame + 80) + 80);
    r30 = VoiceAllocator(base, guest::LoadU32(base, frame + 80), slot, native_heap, 0x8245F430,
                         frame, call);
  }
  if (int32_t(r30) >= 0) {
    const uint32_t owner = guest::LoadU32(base, slot);
    const uint32_t memory = call(Method(base, owner, 20), 0x8245F454, At(frame, Args({owner, 80})));
    uint32_t voice = 0;
    if (memory != 0) {
      SetRegs(call, Args({memory, description, guest::LoadU32(base, slot)}));
      voice = SubmixConstruct(base, memory, description, guest::LoadU32(base, slot), thread, call);
    }
    if (voice == 0) {
      r30 = 0x8007000Eu;  // E_OUTOFMEMORY
    } else {
      SetRegs(call, Args({voice, description}));
      r30 = SubmixInit(base, voice, description, Below(base, frame, 144), call);
      if (int32_t(r30) >= 0) {
        schema::XAudioVoiceOut(base, out).set_voice(voice);
      } else {
        call(Method(base, voice, 12), 0x8245F4B4, At(frame, Args({voice})));
      }
    }
  }
  const uint32_t owner = guest::LoadU32(base, slot);
  if (owner != 0) {
    call(Method(base, owner, 4), 0x8245F4D0, At(frame, Args({owner})));
  }
  return r30;
}

// --- The API entry points (under the API lock) ---

void ApiEnter(uint8_t* base, const GuestCall& call) {
  (void)base;
  call(kRtlEnterCriticalSection, 0, Args({kApiLock}));
}

void ApiLeave(uint8_t* base, const GuestCall& call) {
  (void)base;
  call(kRtlLeaveCriticalSection, 0, Args({kApiLock}));
}

namespace {

// One method of the voice (or its codec, +76) at `slot` under the API lock,
// with `count` registers of `args` (r3 the object).
uint32_t ApiMethodCall(uint8_t* base, uint32_t voice, bool codec, uint32_t slot,
                       uint32_t return_address, uint8_t count, const uint64_t (&args)[4],
                       const GuestCall& call) {
  ApiEnter(base, call);
  const uint32_t object = codec ? Source(base, voice).codec().address : voice;
  Regs regs = Args({object, args[0], args[1], args[2], args[3]});
  regs.count = count;
  const uint32_t r3 = call(Method(base, object, slot), return_address, regs);
  ApiLeave(base, call);
  return r3;
}

}  // namespace

uint32_t ApiAddRef(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459DB0 XAudioVoice::AddRef (the API's)");
  return ApiMethodCall(base, voice, false, 4, 0x82459DEC, 1, {r4, r5, r6, r7}, call);
}

uint32_t ApiRelease(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459E18 XAudioVoice::Release (the API's)");
  return ApiMethodCall(base, voice, false, 8, 0x82459E54, 1, {r4, r5, r6, r7}, call);
}

uint32_t ApiSetEffectParameters(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459E80 XAudioVoice::SetEffectParameters (the API's)");
  return ApiMethodCall(base, voice, false, 32, 0x82459ED4, 5, {r4, r5, r6, r7}, call);
}

uint32_t ApiSetOutputVoices(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459EF0 XAudioVoice::SetOutputVoices (the API's)");
  return ApiMethodCall(base, voice, false, 36, 0x82459F2C, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiSetOutputMatrix(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459F48 XAudioVoice::SetOutputMatrix");
  return ApiMethodCall(base, voice, false, 40, 0x82459F84, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiGetState(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459FA0 XAudioVoice::GetState (the API's)");
  return ApiMethodCall(base, voice, false, 48, 0x82459FDC, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiStop(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A1B0 XAudioVoice::Stop (the API's)");
  return ApiMethodCall(base, voice, false, 60, 0x8245A1EC, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiGetOutputFormat(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A3B8 XAudioVoice::GetOutputFormat");
  return ApiMethodCall(base, voice, false, 64, 0x8245A3F4, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiSubmitPacket(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A050 XAudioSourceVoice::SubmitPacket");
  return ApiMethodCall(base, voice, true, 28, 0x8245A094, 3, {r4, r5, r6, r7}, call);
}

uint32_t ApiCodec36(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A108 XAudioSourceVoice (the codec's slot +36)");
  return ApiMethodCall(base, voice, true, 36, 0x8245A14C, 3, {r4, r5, r6, r7}, call);
}

uint32_t ApiFlushPackets(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A208 XAudioSourceVoice::FlushPackets");
  return ApiMethodCall(base, voice, true, 56, 0x8245A244, 1, {r4, r5, r6, r7}, call);
}

uint32_t ApiCodec72(uint8_t* base, uint32_t voice, uint64_t r4, uint64_t r5, uint64_t r6,
                 uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A270 XAudioSourceVoice (the codec's slot +72)");
  return ApiMethodCall(base, voice, true, 72, 0x8245A2AC, 2, {r4, r5, r6, r7}, call);
}

uint32_t ApiSetFormat(uint8_t* base, uint32_t voice, uint32_t format, uint32_t outputs,
                      uint64_t r6, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82459FF8 XAudioSourceVoice::SetFormat");
  ApiEnter(base, call);
  SetRegs(call, Args({voice, format, outputs, r6}));
  uint32_t r3;
  if (const auto done = Reformat(base, voice, format, outputs, r6, Below(base, frame, 128), call)) {
    r3 = *done;
  } else {
    r3 = call(0x8245DCF0, 0x8245A038, At(frame, Args({voice, format, outputs, r6})));
  }
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiReset(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A0B0 XAudioSourceVoice::Reset");
  ApiEnter(base, call);
  SetRegs(call, Args({voice}));
  const uint32_t r3 = StopAndReset(base, voice, call);
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiStart(uint8_t* base, uint32_t voice, uint32_t flags, const Thread& thread,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A168 XAudioVoice::Start (the API's)");
  ApiEnter(base, call);
  SetRegs(call, Args({voice, flags}));
  const uint32_t r3 = SourceStart(base, voice, flags, thread, call);
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiSetVolume(uint8_t* base, uint32_t voice, double volume, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A2C8 XAudioSourceVoice::SetVolume");
  ApiEnter(base, call);
  Source(base, voice).set_volume(float(volume));
  const uint32_t r3 = call(Method(base, voice, 92), 0x8245A310, Args({voice}));
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiSetFrequencyRatio(uint8_t* base, uint32_t voice, double ratio, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A340 XAudioSourceVoice::SetFrequencyRatio");
  ApiEnter(base, call);
  const uint32_t codec = Source(base, voice).codec().address;
  Regs regs = Args({codec});
  regs.f1_set = true;
  regs.f1 = ratio;
  const uint32_t r3 = call(Method(base, codec, 96), 0x8245A388, regs);
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiGetMasteringVoice(uint8_t* base, uint32_t out, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A568 XAudioGetMasteringVoice");
  ApiEnter(base, call);
  const Engine engine = G::engine(base);
  const uint32_t mastering = engine ? engine.mastering_voice().address : 0;
  if (mastering != 0) {
    schema::XAudioVoiceOut(base, out).set_voice(mastering);
  }
  ApiLeave(base, call);
  return mastering != 0 ? 0 : 0x80004005u;  // E_FAIL
}

uint32_t ApiCreateSourceVoice(uint8_t* base, uint32_t description, uint32_t out,
                              bool native_heap, const Thread& thread, uint32_t frame,
                              const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A768 XAudioCreateSourceVoice");
  ApiEnter(base, call);
  SetRegs(call, Args({description, 0, frame + 80}));
  const uint32_t r3 = CreateSource(base, description, 0, frame + 80, native_heap, thread,
                                   Below(base, frame, 128), call);
  if (int32_t(r3) >= 0) {
    schema::XAudioVoiceOut(base, out).set_voice(guest::LoadU32(base, frame + 80));
  }
  ApiLeave(base, call);
  return r3;
}

uint32_t ApiCreateSubmixVoice(uint8_t* base, uint32_t description, uint32_t out,
                              bool native_heap, const Thread& thread, uint32_t frame,
                              const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245A7C8 XAudioCreateSubmixVoice");
  ApiEnter(base, call);
  SetRegs(call, Args({description, 0, frame + 80}));
  const uint32_t r3 = CreateSubmix(base, description, 0, frame + 80, native_heap, thread,
                                   Below(base, frame, 176), call);
  if (int32_t(r3) >= 0) {
    schema::XAudioVoiceOut(base, out).set_voice(guest::LoadU32(base, frame + 80));
  }

  ApiLeave(base, call);
  return r3;
}

}  // namespace xaudio
