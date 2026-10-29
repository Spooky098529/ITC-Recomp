// XAudio's voices as plain C++ (voices.h; docs/functions/xaudio.md).

#include "xaudio/voices.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "memory/c_runtime_heap.h"
#include "guest.h"

#include <cstring>

namespace xaudio {

namespace {

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
  uint8_t channels;
  if (guest::LoadU8(base, format) == 4) {
    const uint32_t streams = guest::LoadU8(base, format + 4);
    uint32_t sum = 0;
    for (uint32_t i = 0; i < streams; i++) {
      sum += guest::LoadU8(base, format + 12 + 8 * i);
    }
    channels = uint8_t(sum);
  } else {
    channels = guest::LoadU8(base, format + 4);
  }
  guest::StoreU8(base, out + 1, channels);
  guest::StoreU8(base, out, 0);
  guest::StoreU32(base, out + 4, kSampleRate);
}

void SubmixDescribe(uint8_t* base, uint32_t description, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245F010 XAudioSubmixVoice::Describe");
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(base, out + i, 0);
  }
  guest::StoreU8(base, out, 0);
  guest::StoreU8(base, out + 1, guest::LoadU8(base, description + 1));
  guest::StoreU32(base, out + 4, kSampleRate);
  guest::StoreU32(base, out + 8, guest::LoadU32(base, description + 12));
  guest::StoreU32(base, out + 16, guest::LoadU32(base, description + 20));
  guest::StoreU32(base, out + 20, guest::LoadU32(base, description + 24));
  const uint8_t count = guest::LoadU8(base, description + 8);
  guest::StoreU8(base, out + 24, count != 0 ? count : 1);
  const uint8_t channels = guest::LoadU8(base, description + 9);
  guest::StoreU8(base, out + 25, channels != 0 ? channels : 6);
  guest::StoreU32(base, out + 28, guest::LoadU32(base, description + 16));
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
    table = kDefaultMatrices + 48;
  } else if (to == 1) {
    if (in > 6) {
      return 0;
    }
    count = uint8_t(in);
    table = kDefaultMatrices + 96;
  } else if (in == 1) {
    if (to != 2 && to != 4 && to != 6) {
      return 0;
    }
    count = 2;
    table = kDefaultMatrices;
  } else if (in == 2) {
    if (to != 4 && to != 6) {
      return 0;
    }
    count = 2;
    table = kDefaultMatrices + 48;
  } else if (in == 4 && to == 6) {
    count = 4;
    table = kDefaultMatrices + 144;
  } else {
    return 0;
  }
  guest::StoreU8(base, out, count);
  guest::StoreU32(base, out + 4, table);
  return out;
}

std::optional<uint32_t> BufferSize(uint8_t* base, uint32_t format, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245FA28 XAudioBuffer::Size");
  uint32_t size = 0;
  if (guest::LoadU8(base, format) == 0) {
    uint32_t frames;
    if (!PassFrames(guest::LoadU32(base, format + 8), &frames)) {
      return std::nullopt;
    }
    size = (frames * uint32_t(guest::LoadU8(base, format + 5)) + 32) * 4;
  }
  guest::StoreU32(base, out, size + 36);
  return 0;
}

std::optional<uint32_t> BufferClear(uint8_t* base, uint32_t buffer, uint32_t format,
                                    uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8245F8E8 XAudioBuffer::Clear");
  if (format == 0) {
    format = buffer + 12;
  }
  uint32_t format_frames, capacity_frames, current_frames;
  if (!PassFrames(guest::LoadU32(base, format + 4), &format_frames) ||
      !PassFrames(guest::LoadU32(base, buffer + 16), &capacity_frames) ||
      !PassFrames(guest::LoadU32(base, buffer + 24), &current_frames)) {
    return std::nullopt;
  }
  const uint32_t wanted = format_frames * guest::LoadU8(base, format + 1);
  const uint32_t capacity = capacity_frames * guest::LoadU8(base, buffer + 13);
  uint32_t current = current_frames * guest::LoadU8(base, buffer + 21);
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
    const uint32_t from = guest::LoadU32(base, buffer + 28) + current * 4;
    const uint32_t bytes = (wanted - current) * 4;
    guest::ForEachHostPart(base, from, bytes, [](uint8_t* host, uint32_t, uint32_t part) {
      std::memset(host, 0, part);
    });
  }
  guest::StoreU8(base, buffer + 21, guest::LoadU8(base, format + 1));
  return 0;
}

uint32_t EngineBuffer(uint8_t* base, uint32_t processor) {
  ITC_NATIVE_RUNNING("sub_8245DEB0 XAudioVoice::EngineBuffer");
  const uint32_t engine = guest::LoadU32(base, kEngine);
  return guest::LoadU32(base, engine + ((processor & 0xFF) << 3) + 12);
}

uint32_t GetType(uint8_t* base, uint32_t voice, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245E318 XAudioVoice::GetType");
  guest::StoreU8(base, out, guest::LoadU8(base, voice + 12));
  return 0;
}

uint32_t GetFormat(uint8_t* base, uint32_t voice, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8245E328 XAudioVoice::GetFormat");
  guest::StoreU32(base, out, guest::LoadU32(base, voice + 52));
  guest::StoreU32(base, out + 4, guest::LoadU32(base, voice + 56));
  return 0;
}

void SetFormatWords(uint8_t* base, uint32_t voice, uint32_t format) {
  ITC_NATIVE_RUNNING("sub_82461140 XAudioVoice::SetFormatWords");
  guest::StoreU32(base, voice + 52, guest::LoadU32(base, format));
  guest::StoreU32(base, voice + 56, guest::LoadU32(base, format + 4));
}

uint32_t AddRef(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8245F860 XAudioVoice::AddRef");
  const uint32_t count = guest::LoadU32(base, object + 4) + 1;
  guest::StoreU32(base, object + 4, count);
  return count;
}

void SetRootVtable(uint8_t* base, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8245FB00 XAudioVoice::SetRootVtable");
  guest::StoreU32(base, object, kRootVtable);
}

uint32_t AddStat(uint8_t* base, uint32_t index, uint32_t amount) {
  ITC_NATIVE_RUNNING("sub_8245F640 XAudio::AddStat");
  // The original's lwarx/stwcx. loop: a compare-and-swap on the big-endian
  // word, as the recompiled users of the same counters do.
  auto* word = reinterpret_cast<uint32_t*>(REX_RAW_ADDR(uint32_t(kStats + (index << 2))));
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

// A frame of `bytes` below `frame` with its back chain, for a plain C++
// callee working in its own frame as the original.
uint32_t Below(uint8_t* base, uint32_t frame, uint32_t bytes) {
  const uint32_t below = frame - bytes;
  guest::StoreU32(base, below, frame);
  return below;
}

constexpr uint32_t kOne = 0x82000A30;  // 1.0f

}  // namespace

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
  const uint32_t count = guest::LoadU32(base, object + 4) - 1;
  guest::StoreU32(base, object + 4, count);
  if (count != 0) {
    return count;
  }
  call(Method(base, object, 12), 0x8245F8A4, Args({object}));
  return 0;
}

uint32_t ObjectFree(uint8_t* base, uint32_t object, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245AAD8 XAudioObject::Free");
  const uint32_t allocator = guest::LoadU32(base, object + 8);
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
  const uint32_t category = guest::LoadU8(base, voice + 144);
  const float volume =
      category < 2
          ? guest::LoadFloat(base, guest::LoadU32(base, kEngine) + ((category + 33) << 2))
          : guest::LoadFloat(base, kOne);
  const uint32_t codec = guest::LoadU32(base, voice + 76);
  Regs regs = Args({codec});
  regs.f1_set = true;
  regs.f1 = double(float(double(guest::LoadFloat(base, voice + 140)) * double(volume)));
  return call(Method(base, codec, 88), 0, regs);
}

uint32_t StopAndReset(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DD60 XAudioSourceVoice::StopAndReset");
  const uint32_t r3 = call(Method(base, voice, 60), 0x8245DD88, Args({voice, 1}));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t codec = guest::LoadU32(base, voice + 76);
  if (codec == 0) {
    return r3;
  }
  return call(Method(base, codec, 32), 0x8245DDB0, Args({codec}));
}

uint32_t SetEffectParameters(uint8_t* base, uint32_t voice, uint32_t index, uint64_t r5,
                             uint64_t r6, uint64_t r7, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461418 XAudioVoice::SetEffectParameters");
  const uint32_t i = index & 0xFF;
  if (i >= guest::LoadU8(base, voice + 60)) {
    return 0x80070057u;  // E_INVALIDARG
  }
  const uint32_t effect = guest::LoadU32(base, guest::LoadU32(base, voice + 36) + (i << 3));
  return call(Method(base, effect, 16), 0x82461480, Args({effect, r5, r6, r7}));
}

uint32_t ReleaseOutputs(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460A58 XAudioVoice::ReleaseOutputs");
  uint32_t r3 = voice;
  for (uint32_t i = 0; i < guest::LoadU8(base, voice + 69); i++) {
    const uint32_t target = guest::LoadU32(base, guest::LoadU32(base, voice + 72) + 12 * i);
    r3 = call(Method(base, target, 8), 0x82460A98, Args({target}));
  }
  guest::StoreU8(base, voice + 69, 0);
  return r3;
}

uint32_t SetOutputMatrices(uint8_t* base, uint32_t voice, uint32_t matrices,
                           const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460AB8 XAudioVoice::SetOutputMatrices");
  const uint32_t not_playing = ~uint32_t(guest::LoadU8(base, voice + 61)) & 1;
  uint32_t r3 = 0;
  if (guest::LoadU8(base, matrices) == 0) {
    return r3;
  }
  for (uint32_t i = 0; int32_t(r3) >= 0; ) {
    const uint32_t entry = guest::LoadU32(base, matrices + 4) + 8 * i;
    const uint32_t output = guest::LoadU8(base, entry);
    const uint32_t matrix = guest::LoadU32(base, entry + 4);
    const uint32_t object = guest::LoadU32(base, guest::LoadU32(base, voice + 72) + 12 * output + 4);
    r3 = call(Method(base, object, 56), 0x82460B34, Args({object, matrix, not_playing}));
    if (++i >= guest::LoadU8(base, matrices)) {
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
  for (uint32_t i = 0; i < 32; i += 4) {
    guest::StoreU32(base, out + i, 0);
  }
  SourceOutputFormat(base, description, out);
  guest::StoreU32(base, out + 8, guest::LoadU32(base, description + 68));
  const uint32_t engine = guest::LoadU32(base, kEngine);
  uint32_t buffer = 0;
  for (uint32_t i = 0; i < 6; i++) {
    if (guest::LoadU32(base, engine + 12 + 8 * i) != 0) {
      buffer = guest::LoadU32(base, engine + 12 + 8 * i);
      break;
    }
  }
  guest::StoreU32(base, out + 12, buffer);
  guest::StoreU32(base, out + 16, guest::LoadU32(base, description + 76));
  guest::StoreU32(base, out + 20, guest::LoadU32(base, description + 88));
  const uint8_t count = guest::LoadU8(base, description + 57);
  guest::StoreU8(base, out + 24, count != 0 ? count : 1);
  const uint8_t channels = guest::LoadU8(base, description + 58);
  guest::StoreU8(base, out + 25, channels != 0 ? channels : 6);
  guest::StoreU32(base, out + 28, guest::LoadU32(base, description + 72));
}

std::optional<uint32_t> Reformat(uint8_t* base, uint32_t voice, uint32_t format,
                                 uint32_t outputs, uint64_t r6, uint32_t frame,
                                 const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245DCF0 XAudioSourceVoice::Reformat");
  if (!c_runtime::Memcpy(base, voice + 84, format, 56)) {
    return std::nullopt;
  }
  const uint32_t codec = guest::LoadU32(base, voice + 76);
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
  uint32_t r3 = 0;
  uint32_t size = 0;
  if (guest::LoadU32(base, description + 12) == 0) {
    // A format of the description's channels at 48 kHz, at the frame's +96.
    const uint32_t local = frame + 96;
    for (uint32_t i = 0; i < 20; i += 4) {
      guest::StoreU32(base, local + i, 0);
    }
    guest::StoreU8(base, local + 5, guest::LoadU8(base, description + 1));
    guest::StoreU32(base, local + 8, kSampleRate);
    // BufferSize can't refuse a rate of 48,000.
    r3 = *BufferSize(base, local, frame + 80);
    size = guest::LoadU32(base, frame + 80);
  }
  uint32_t effects = 0;
  const uint32_t chain = guest::LoadU32(base, description + 8);
  if (chain != 0 && guest::LoadU8(base, chain) != 0) {
    for (uint32_t offset = 0; effects < guest::LoadU8(base, guest::LoadU32(base, description + 8));
         offset += 4) {
      const uint32_t id =
          guest::LoadU32(base, guest::LoadU32(base, guest::LoadU32(base, description + 8) + 4) +
                                   offset);
      const uint32_t factory = guest::LoadU32(base, guest::LoadU32(base, kEngine) + 60);
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
  guest::StoreU32(base, out, (effects << 3) + size);
  return r3;
}

std::optional<uint32_t> BufferConstruct(uint8_t* base, uint32_t buffer, uint32_t format,
                                        uint32_t allocator, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245F750 XAudioBuffer::XAudioBuffer");
  const uint8_t type = guest::LoadU8(base, format);
  uint32_t frames = 0;
  if (type == 0 && !PassFrames(guest::LoadU32(base, format + 8), &frames)) {
    return std::nullopt;
  }
  guest::StoreU32(base, buffer, kBufferVtable);
  guest::StoreU32(base, buffer + 4, 1);
  guest::StoreU8(base, buffer + 8, type);
  guest::StoreU8(base, buffer + 20, 0);
  guest::StoreU32(base, buffer + 24, kSampleRate);
  guest::StoreU32(base, buffer + 32, guest::LoadU32(base, format + 16));
  if (guest::LoadU8(base, buffer + 8) != 0) {
    guest::StoreU32(base, buffer + 12, 0);
    guest::StoreU32(base, buffer + 16, 0);
    guest::StoreU8(base, buffer + 21, 0);
    guest::StoreU32(base, buffer + 28, guest::LoadU32(base, format + 12));
    return buffer;
  }
  const uint32_t bytes = ((frames * uint32_t(guest::LoadU8(base, format + 5))) << 2) + 128;
  const uint32_t samples =
      (call(Method(base, allocator, 20), 0x8245F7E8, Args({allocator, bytes, allocator})) +
       127) &
      0xFFFFFF80u;
  if (format + 4 == 0) {
    guest::StoreU32(base, buffer + 12, 0);
    guest::StoreU32(base, buffer + 16, 0);
  } else {
    guest::StoreU32(base, buffer + 12, guest::LoadU32(base, format + 4));
    guest::StoreU32(base, buffer + 16, guest::LoadU32(base, format + 8));
  }
  guest::StoreU32(base, buffer + 28, samples);
  guest::StoreU8(base, buffer + 21, 0);
  return buffer;
}

uint32_t BufferCreate(uint8_t* base, uint32_t format, uint32_t allocator, uint32_t out,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8245FA80 XAudioBuffer::Create");
  const uint32_t memory = call(Method(base, allocator, 20), 0x8245FAB0, Args({allocator, 36}));
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
  guest::StoreU32(base, voice + 8, allocator);
  guest::StoreU32(base, voice, kVoiceVtable0);
  guest::StoreU32(base, voice + 4, 1);
  if (allocator != 0) {
    call(Method(base, allocator, 0), 0x82461550, Args({allocator}));
  }
  guest::StoreU8(base, voice + 12, uint8_t(type));
  guest::StoreU32(base, voice, kVoiceVtable);
  guest::StoreU32(base, voice + 16, voice + 16);
  guest::StoreU32(base, voice + 20, voice + 16);
  guest::StoreU32(base, voice + 24, voice + 24);
  guest::StoreU32(base, voice + 28, voice + 24);
  guest::StoreU32(base, voice + 40, 0);
  return voice;
}

uint32_t DefaultAllocator(uint8_t* base, uint32_t attributes, uint32_t size, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_824605E8 XAudio::DefaultAllocator");
  const uint32_t block = Alloc(base, size + 28, attributes);
  if (block == 0) {
    return 0x8007000Eu;  // E_OUTOFMEMORY
  }
  guest::StoreU32(base, block + 4, kRootVtable);
  guest::StoreU32(base, block + 8, 1);
  guest::StoreU32(base, block, kAllocatorVtable);
  guest::StoreU32(base, block + 4, kAllocatorVtable2);
  guest::StoreU32(base, block + 12, attributes);
  guest::StoreU32(base, block + 20, 0);
  guest::StoreU32(base, block + 16, size);
  guest::StoreU32(base, block + 24, block + 28);
  guest::StoreU32(base, out, block);
  return 0;
}

std::optional<uint32_t> GetAllocator(uint8_t* base, uint32_t attributes, uint32_t size,
                                     uint32_t inout, uint32_t frame, bool native_heap,
                                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460688 XAudio::GetAllocator");
  const uint32_t allocator = guest::LoadU32(base, inout);
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
    guest::StoreU32(base, inout, guest::LoadU32(base, frame + 80));
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
  guest::StoreU32(base, voice, kOutputsVtable);
  return voice;
}

uint32_t OutputsSize(uint8_t* base, uint32_t description, uint32_t out, uint32_t frame,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_824607C8 XAudioVoice::OutputsSize");
  uint32_t r3 = BaseSize(base, description, frame + 84, Below(base, frame, 176), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  // The matrix object's description: type 1, the outputs' channels at +8.
  const uint32_t local = frame + 96;
  for (uint32_t i = 0; i < 16; i += 4) {
    guest::StoreU32(base, local + i, 0);
  }
  guest::StoreU8(base, local + 8, guest::LoadU8(base, description + 25));
  guest::StoreU8(base, local, 1);
  const uint32_t factory = guest::LoadU32(base, guest::LoadU32(base, kEngine) + 60);
  uint32_t function = 0;
  const uint32_t object = FactorySizeThunk(base, factory, &function);
  r3 = call(function, 0x82460834, At(frame, Args({object, local, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t count = guest::LoadU8(base, description + 24);
  guest::StoreU32(base, out,
                  count * guest::LoadU32(base, frame + 80) + guest::LoadU32(base, frame + 84) +
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
    entry = kDefaultOutput;
  }
  uint32_t target = guest::LoadU32(base, entry);
  if (target == 0) {
    target = guest::LoadU32(base, guest::LoadU32(base, kEngine) + 64);
  }
  guest::StoreU32(base, slot, target);
  call(Method(base, target, 4), 0x824609F0, At(frame, Args({target, entry_arg, slot})));
  const uint32_t matrix = guest::LoadU32(base, entry + 4);
  const uint32_t object = guest::LoadU32(base, slot + 4);
  if (matrix != 0) {
    return call(Method(base, object, 40), 0x82460A50, At(frame, Args({object, matrix})));
  }
  const uint32_t output = guest::LoadU32(base, slot);
  const uint32_t r3 = call(Method(base, output, 64), 0x82460A1C, At(frame, Args({output, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t defaults = DefaultMatrix(base, guest::LoadU8(base, voice + 53),
                                          guest::LoadU8(base, frame + 81), frame + 88);
  const uint32_t matrix_object = guest::LoadU32(base, slot + 4);
  return call(Method(base, matrix_object, 40), 0x82460A50,
              At(frame, Args({matrix_object, defaults, frame + 88})));
}

uint32_t SetOutputVoices(uint8_t* base, uint32_t voice, uint32_t outputs, uint32_t frame,
                         const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82460BF0 XAudioVoice::SetOutputVoices");
  ReleaseOutputs(base, voice, call);
  const uint32_t below = Below(base, frame, 128);
  if (outputs == 0) {
    if (guest::LoadU8(base, voice + 68) == 0) {
      return 0;
    }
    const uint32_t r3 = SetOutput(base, voice, 0, guest::LoadU32(base, voice + 72), below, call);
    if (int32_t(r3) < 0) {
      return r3;
    }
    guest::StoreU8(base, voice + 69, 1);
    return r3;
  }
  uint32_t r26 = 0;
  for (uint32_t i = 0; i < guest::LoadU8(base, outputs); i++) {
    if (int32_t(r26) < 0) {
      return r26;
    }
    r26 = SetOutput(base, voice, guest::LoadU32(base, outputs + 4) + 8 * i,
                    guest::LoadU32(base, voice + 72) + 12 * i, below, call);
  }
  if (int32_t(r26) < 0) {
    return r26;
  }
  guest::StoreU8(base, voice + 69, guest::LoadU8(base, outputs));
  return r26;
}

uint32_t Pass(uint8_t* base, uint32_t voice, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461070 XAudioVoice::Pass");
  guest::StoreU32(base, frame + 84,
                  call(Method(base, voice, 72), 0x82461094, At(frame, Args({voice}))));
  vehicle_accessors::DebugSwitchOff();
  const uint32_t callback = guest::LoadU32(base, voice + 44);
  if (callback != 0) {
    guest::StoreU32(base, frame + 80, guest::LoadU32(base, voice + 48));
    call(callback, 0x824610C0, At(frame, Args({frame + 80})));
  }
  uint32_t r3;
  if ((guest::LoadU8(base, voice + 61) & 4) != 0 && guest::LoadU16(base, voice + 64) == 0) {
    r3 = call(Method(base, voice, 60), 0x8246113C, At(frame, Args({voice, 1})));
  } else {
    if ((guest::LoadU8(base, voice + 61) & 4) != 0) {
      guest::StoreU16(base, voice + 64, uint16_t(guest::LoadU16(base, voice + 64) - 1));
    }
    r3 = call(Method(base, voice, 84), 0x82461100, At(frame, Args({voice, frame + 84})));
  }
  vehicle_accessors::DebugSwitchOff();
  return r3;
}

uint32_t Destroy(uint8_t* base, uint32_t voice, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461240 XAudioVoice::Destroy");
  guest::StoreU32(base, voice, kVoiceVtable);
  for (uint32_t i = 0; i < guest::LoadU8(base, voice + 60); i = (i + 1) & 0xFF) {
    const uint32_t effect = guest::LoadU32(base, guest::LoadU32(base, voice + 36) + 8 * i);
    if (effect == 0) {
      continue;
    }
    call(Method(base, effect, 4), 0x82461298, Args({effect}));
    guest::StoreU32(base, guest::LoadU32(base, voice + 36) + 8 * i, 0);
  }
  const uint32_t buffer = guest::LoadU32(base, voice + 32);
  if (buffer != 0) {
    uint32_t function = 0;
    const uint32_t object = BufferReleaseThunk(base, buffer, &function);
    call(function, 0x824612C4, Args({object}));
    guest::StoreU32(base, voice + 32, 0);
  }
  const uint32_t allocator = guest::LoadU32(base, voice + 8);
  guest::StoreU32(base, voice, kVoiceVtable0);
  uint32_t r3 = 0;
  if (allocator != 0) {
    r3 = call(Method(base, allocator, 4), 0x824612F0, Args({allocator}));
    guest::StoreU32(base, voice + 8, 0);
  }
  guest::StoreU32(base, voice, kRootVtable);
  return r3;
}

uint32_t CreateObject(uint8_t* base, uint32_t voice, uint32_t entry, uint32_t description,
                      uint32_t flag, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461308 XAudioVoice::CreateObject");
  const uint32_t factory = guest::LoadU32(base, guest::LoadU32(base, kEngine) + 60);
  guest::StoreU32(base, entry, 0);
  uint32_t function = 0;
  const uint32_t object = FactoryCreateThunk(base, factory, &function);
  uint32_t r3 = call(function, 0x82461344,
                     At(frame, Args({object, description, guest::LoadU32(base, voice + 8), entry})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint32_t made = guest::LoadU32(base, entry);
  r3 = call(Method(base, made, 8), 0x82461364, At(frame, Args({made, frame + 80})));
  if (int32_t(r3) < 0) {
    return r3;
  }
  const uint16_t size = guest::LoadU16(base, frame + 82);
  guest::StoreU8(base, entry + 4, uint8_t(flag));
  guest::StoreU8(base, entry + 5, 1);
  guest::StoreU8(base, entry + 6, guest::LoadU8(base, frame + 80));
  if (size > guest::LoadU16(base, voice + 62)) {
    guest::StoreU16(base, voice + 62, size);
  }
  return r3;
}

uint32_t ProcessEffect(uint8_t* base, uint32_t entry, uint32_t inout, uint32_t processor,
                       uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_82461678 XAudioVoice::ProcessEffect");
  if ((guest::LoadU8(base, entry + 5) & 1) == 0) {
    return 0;
  }
  const uint8_t flags = guest::LoadU8(base, entry + 6);
  uint32_t r4;
  uint32_t r5;
  if ((flags & 6) == 6) {
    // In place to the processor's other buffer of its pair.
    const uint32_t engine = guest::LoadU32(base, kEngine);
    const uint32_t buffers = engine + 12 + 8 * processor;
    r4 = guest::LoadU32(base, inout);
    r5 = guest::LoadU32(base, buffers);
    if (r4 != 0) {
      for (uint32_t k = 0; k < 2; k++) {
        if (r4 == guest::LoadU32(base, buffers + 4 * k)) {
          r5 = guest::LoadU32(base, buffers + 4 * ((k - 1) & 1));
          break;
        }
      }
    }
    guest::StoreU32(base, inout, r5);
  } else if ((flags & 3) != 0) {
    r4 = guest::LoadU32(base, inout);
    r5 = 0;
  } else if ((flags & 4) != 0) {
    r5 = guest::LoadU32(base, inout);
    r4 = 0;
  } else {
    r4 = guest::LoadU32(base, frame + 80);
    r5 = r4;
  }
  const uint32_t effect = guest::LoadU32(base, entry);
  return call(Method(base, effect, 24), 0x82461788, At(frame, Args({effect, r4, r5})));
}

}  // namespace xaudio
