// XAudio's voices as plain C++ (voices.h; docs/functions/xaudio.md).

#include "xaudio/voices.h"

#include "base/log.h"
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
    if (!PassFrames(guest::LoadU32(base, format + 4), &frames)) {
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

}  // namespace xaudio
