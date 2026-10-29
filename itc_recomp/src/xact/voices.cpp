// XACT's voice class natively (waves.h, docs/functions/xact-waves.md): each
// method one or two of XAudio's entry points through their plain C++
// (src/xaudio/voices.h), in a frame below the caller's, with the registers
// set as the original sets them before the call. XAudio's locks are taken
// there, as the original takes them.

#include "xact/waves.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"

#include <initializer_list>

namespace xact {

namespace {

namespace xa = xaudio;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
// A float moved as the original's lfs and stfs do (through a double).
void MoveFloat(uint8_t* base, uint32_t to, uint32_t from) {
  const double value = double(guest::LoadFloat(base, from));
  guest::StoreFloat(base, to, float(value));
}

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
xa::Regs ArgsF1(std::initializer_list<uint64_t> r, double f1) {
  xa::Regs regs = Args(r);
  regs.f1_set = true;
  regs.f1 = f1;
  return regs;
}
// The registers the original sets before calling a function whose plain C++
// runs here instead.
void SetRegs(const GuestCall& call, const xa::Regs& regs) { call(0, 0, regs); }

// The frames of the callees (their originals' sizes).
constexpr uint32_t kApiFrame = 112;      // most of XAudio's entry points
constexpr uint32_t kCodec36Frame = 128;  // sub_8245A108
constexpr uint32_t kCreateSourceFrame = 128;  // XAudioCreateSourceVoice
constexpr uint32_t kInitFrame = 128;     // XACTVoice::Init
constexpr uint32_t kDestructFrame = 112;  // XACTVoice::~XACTVoice

constexpr uint32_t kOne = 0x82000A30;  // 1.0f
constexpr uint32_t kOutOfMemory = 0x8007000Eu;

// (r4 != 0) as the original's cntlzw, rlwinm and xori give it.
uint32_t NotZero(uint32_t value) { return value != 0 ? 1 : 0; }

}  // namespace

uint32_t VoiceInit(uint8_t* base, uint32_t voice, uint32_t description, uint32_t holder,
                   uint32_t level, uint32_t reverb, const Thread& thread, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9B60 XACTVoice::Init");
  SetRegs(call, Args({description, voice + 4}));
  uint32_t r3 = xa::ApiCreateSourceVoice(base, description, voice + 4, true, thread,
                                         xa::FrameBelow(base, frame, kCreateSourceFrame), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  guest::StoreU8(base, voice + 12, guest::LoadU8(base, description + 4));
  if (reverb == 0) {
    return r3;
  }
  const uint32_t submix_holder = Load(base, holder);
  xa::Regs regs = Args({submix_holder});
  regs.sp = frame;
  r3 = call(xa::Method(base, submix_holder, 4), 0x825C9BB8, regs);
  Store(base, voice + 8, r3);
  MoveFloat(base, voice + 16, level);
  regs = Args({voice, 0});
  regs.sp = frame;
  return call(xa::Method(base, voice, 32), 0x825C9BE0, regs);
}

uint32_t VoiceCreate(uint8_t* base, uint32_t description, uint32_t holder, uint32_t level,
                     uint32_t reverb, uint32_t out, const Thread& thread, uint32_t frame,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C40 XACTVoice::Create");
  SetRegs(call, Args({24, kVoiceAttributes}));
  const uint32_t voice = memory::crt::XMemAlloc(base, 24, kVoiceAttributes);
  if (voice == 0) {
    Store(base, out, 0);
    return kOutOfMemory;
  }
  Store(base, voice + 4, 0);
  Store(base, voice + 8, 0);
  guest::StoreU8(base, voice + 12, 0);
  MoveFloat(base, voice + 16, kOne);
  Store(base, voice + 0, kVoiceVtable);
  Store(base, voice + 20, 1);
  SetRegs(call, Args({voice, description, holder, level, reverb}));
  const uint32_t r3 = VoiceInit(base, voice, description, holder, level, reverb, thread,
                                xa::FrameBelow(base, frame, kInitFrame), call);
  if (int32_t(r3) >= 0) {
    Store(base, out, voice);
    return r3;
  }
  const uint32_t destructor = xa::Method(base, voice, 56);
  if (destructor == kVoiceDeletingDestructor) {
    SetRegs(call, Args({voice, 1}));
    VoiceDeletingDestruct(base, voice, 1, xa::FrameBelow(base, frame, kDestructFrame), call);
  } else {
    xa::Regs regs = Args({voice, 1});
    regs.sp = frame;
    call(destructor, 0x825C9D08, regs);
  }
  Store(base, out, 0);
  return r3;
}

uint32_t VoiceGetState(uint8_t* base, uint32_t voice, uint32_t out, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9DB8 XACTVoice::GetState");
  const uint32_t source = Load(base, voice + 4);
  SetRegs(call, Args({source, frame + 80}));
  xa::FrameBelow(base, frame, kApiFrame);
  const uint32_t r3 = xa::ApiGetState(base, source, frame + 80, 0, 0, 0, call);
  if (int32_t(r3) >= 0) {
    Store(base, out, ~uint32_t(guest::LoadU8(base, frame + 80)) & 1);
  }
  return r3;
}

uint32_t VoiceSetVolume(uint8_t* base, uint32_t voice, double volume, uint32_t sp,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BE8 XACTVoice::SetVolume");
  const uint32_t source = Load(base, voice + 4);
  SetRegs(call, ArgsF1({source}, volume));
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiSetVolume(base, source, volume, call);
}

uint32_t VoiceSetPitch(uint8_t* base, uint32_t voice, double ratio, uint32_t sp,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BF0 XACTVoice::SetPitch");
  const uint32_t source = Load(base, voice + 4);
  SetRegs(call, ArgsF1({source}, ratio));
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiSetFrequencyRatio(base, source, ratio, call);
}

uint32_t VoiceStart(uint8_t* base, uint32_t voice, uint32_t deferred, const Thread& thread,
                    uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BF8 XACTVoice::Start");
  const uint32_t source = Load(base, voice + 4);
  const uint32_t flags = NotZero(deferred);
  SetRegs(call, Args({source, flags}));
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiStart(base, source, flags, thread, call);
}

uint32_t VoiceStop(uint8_t* base, uint32_t voice, uint32_t now, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C10 XACTVoice::Stop");
  const uint32_t source = Load(base, voice + 4);
  const uint32_t flags = NotZero(now);
  SetRegs(call, Args({source, flags}));
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiStop(base, source, flags, 0, 0, 0, call);
}

uint32_t VoiceFlushPackets(uint8_t* base, uint32_t voice, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C28 XACTVoice::FlushPackets");
  const uint32_t source = Load(base, voice + 4);
  SetRegs(call, Args({source}));
  xa::FrameBelow(base, sp, kApiFrame);
  return xa::ApiFlushPackets(base, source, 0, 0, 0, 0, call);
}

uint32_t VoiceSubmitPacket(uint8_t* base, uint32_t voice, uint32_t packet, uint32_t flag,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9E08 XACTVoice::SubmitPacket");
  const uint32_t to = frame + 80;
  Store(base, to + 0, Load(base, packet + 0));
  Store(base, to + 4, Load(base, packet + 4));
  const uint32_t loops = Load(base, packet + 8);
  Store(base, to + 8, loops == 255 ? 0xFFFFFFFFu : loops);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, to + 12 + 4 * i, Load(base, packet + 20 + 4 * i));
  }
  const uint32_t source = Load(base, voice + 4);
  const uint32_t r5 = NotZero(flag);
  SetRegs(call, Args({source, to, r5}));
  xa::FrameBelow(base, frame, kCodec36Frame);
  return xa::ApiCodec36(base, source, to, r5, 0, 0, call);
}

uint32_t VoiceDeletingDestruct(uint8_t* base, uint32_t voice, uint32_t flags, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825CA448 XACTVoice::~XACTVoice (deleting)");
  Store(base, voice + 0, kVoiceVtable);
  const uint32_t source = Load(base, voice + 4);
  if (source != 0) {
    SetRegs(call, Args({source}));
    xa::FrameBelow(base, frame, kApiFrame);
    xa::ApiRelease(base, source, 0, 0, 0, 0, call);
  }
  if ((flags & 1) != 0) {
    SetRegs(call, Args({voice, kVoiceAttributes}));
    memory::crt::XMemFree(voice, kVoiceAttributes);
  }
  return voice;
}

}  // namespace xact
