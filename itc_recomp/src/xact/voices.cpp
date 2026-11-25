// XACT's voice class natively (waves.h, docs/functions/xact-waves.md): each
// method one or two of XAudio's entry points through their plain C++
// (src/xaudio/voices.h), in a frame below the caller's, with the registers
// set as the original sets them before the call. XAudio's locks are taken
// there, as the original takes them.

#include "xact/waves.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "schema/xact_waves.h"
#include "schema/xaudio.h"
#include "schema/xaudio_objects.h"

#include <initializer_list>

namespace xact {

namespace {

namespace xa = xaudio;
using Voice = schema::XACTVoice;
using G = schema::XactWavesGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
// A float loaded as the original's lfs does (through a double).
double LoadF(uint32_t from) { return double(guest::LoadFloat(from)); }

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

constexpr uint32_t kOutOfMemory = 0x8007000Eu;

// (r4 != 0) as the original's cntlzw, rlwinm and xori give it.
uint32_t NotZero(uint32_t value) { return value != 0 ? 1 : 0; }

}  // namespace

uint32_t VoiceInit(uint32_t voice, uint32_t description, uint32_t holder,
                   uint32_t level, uint32_t reverb, const Thread& thread, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9B60 XACTVoice::Init");
  const Voice v(voice);
  SetRegs(call, Args({description, v.source_voice_address()}));
  uint32_t r3 =
      xa::ApiCreateSourceVoice(description, v.source_voice_address(), true, thread,
                               xa::FrameBelow(frame, kCreateSourceFrame), call);
  if (int32_t(r3) < 0) {
    return r3;
  }
  v.set_channels(schema::XAudioSourceDescription(description).format_channels());
  if (reverb == 0) {
    return r3;
  }
  const uint32_t submix_holder = Load(holder);
  xa::Regs regs = Args({submix_holder});
  regs.sp = frame;
  r3 = call(xa::Method(submix_holder, 4), 0x825C9BB8, regs);
  v.set_reverb_voice(r3);
  v.set_reverb_level(float(LoadF(level)));
  regs = Args({voice, 0});
  regs.sp = frame;
  return call(xa::Method(voice, 32), 0x825C9BE0, regs);
}

uint32_t VoiceCreate(uint32_t description, uint32_t holder, uint32_t level,
                     uint32_t reverb, uint32_t out, const Thread& thread, uint32_t frame,
                     const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C40 XACTVoice::Create");
  SetRegs(call, Args({24, kVoiceAttributes}));
  const uint32_t voice = memory::crt::XMemAlloc(24, kVoiceAttributes);
  if (voice == 0) {
    Store(out, 0);
    return kOutOfMemory;
  }
  const Voice v(voice);
  v.set_source_voice(0);
  v.set_reverb_voice(0);
  v.set_channels(0);
  v.set_reverb_level(float(LoadF(G::one_address())));
  v.set_vtable(kVoiceVtable);
  v.set_outputs_unset(1);
  SetRegs(call, Args({voice, description, holder, level, reverb}));
  const uint32_t r3 = VoiceInit(voice, description, holder, level, reverb, thread,
                                xa::FrameBelow(frame, kInitFrame), call);
  if (int32_t(r3) >= 0) {
    Store(out, voice);
    return r3;
  }
  const uint32_t destructor = xa::Method(voice, 56);
  if (destructor == kVoiceDeletingDestructor) {
    SetRegs(call, Args({voice, 1}));
    VoiceDeletingDestruct(voice, 1, xa::FrameBelow(frame, kDestructFrame), call);
  } else {
    xa::Regs regs = Args({voice, 1});
    regs.sp = frame;
    call(destructor, 0x825C9D08, regs);
  }
  Store(out, 0);
  return r3;
}

uint32_t VoiceGetState(uint32_t voice, uint32_t out, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9DB8 XACTVoice::GetState");
  const uint32_t source = Voice(voice).source_voice();
  const schema::XAudioByteOut state(frame + 80);  // XAudio's state byte
  SetRegs(call, Args({source, state.address}));
  xa::FrameBelow(frame, kApiFrame);
  const uint32_t r3 = xa::ApiGetState(source, state.address, 0, 0, 0, call);
  if (int32_t(r3) >= 0) {
    Store(out, ~uint32_t(state.value()) & 1);
  }
  return r3;
}

uint32_t VoiceSetVolume(uint32_t voice, double volume, uint32_t sp,
                        const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BE8 XACTVoice::SetVolume");
  const uint32_t source = Voice(voice).source_voice();
  SetRegs(call, ArgsF1({source}, volume));
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiSetVolume(source, volume, call);
}

uint32_t VoiceSetPitch(uint32_t voice, double ratio, uint32_t sp,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BF0 XACTVoice::SetPitch");
  const uint32_t source = Voice(voice).source_voice();
  SetRegs(call, ArgsF1({source}, ratio));
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiSetFrequencyRatio(source, ratio, call);
}

uint32_t VoiceStart(uint32_t voice, uint32_t deferred, const Thread& thread,
                    uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9BF8 XACTVoice::Start");
  const uint32_t source = Voice(voice).source_voice();
  const uint32_t flags = NotZero(deferred);
  SetRegs(call, Args({source, flags}));
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiStart(source, flags, thread, call);
}

uint32_t VoiceStop(uint32_t voice, uint32_t now, uint32_t sp,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C10 XACTVoice::Stop");
  const uint32_t source = Voice(voice).source_voice();
  const uint32_t flags = NotZero(now);
  SetRegs(call, Args({source, flags}));
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiStop(source, flags, 0, 0, 0, call);
}

uint32_t VoiceFlushPackets(uint32_t voice, uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9C28 XACTVoice::FlushPackets");
  const uint32_t source = Voice(voice).source_voice();
  SetRegs(call, Args({source}));
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiFlushPackets(source, 0, 0, 0, 0, call);
}

uint32_t VoiceSubmitPacket(uint32_t voice, uint32_t packet, uint32_t flag,
                           uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825C9E08 XACTVoice::SubmitPacket");
  // XAudio's 32-byte region packet (the codec's slot +36) on the stack.
  const schema::XAudioRegionPacket to(frame + 80);
  to.set_data(Load(packet + 0));
  to.set_size(Load(packet + 4));
  const uint32_t loops = Load(packet + 8);
  to.set_loop_count(loops == 255 ? 0xFFFFFFFFu : loops);
  to.set_loop_start(Load(packet + 20));
  to.set_loop_end(Load(packet + 24));
  to.set_play_begin(Load(packet + 28));
  to.set_play_end(Load(packet + 32));
  to.set_context(Load(packet + 36));
  const uint32_t source = Voice(voice).source_voice();
  const uint32_t r5 = NotZero(flag);
  SetRegs(call, Args({source, to.address, r5}));
  xa::FrameBelow(frame, kCodec36Frame);
  return xa::ApiCodec36(source, to.address, r5, 0, 0, call);
}

uint32_t VoiceDeletingDestruct(uint32_t voice, uint32_t flags, uint32_t frame,
                               const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_825CA448 XACTVoice::~XACTVoice (deleting)");
  const Voice v(voice);
  v.set_vtable(kVoiceVtable);
  const uint32_t source = v.source_voice();
  if (source != 0) {
    SetRegs(call, Args({source}));
    xa::FrameBelow(frame, kApiFrame);
    xa::ApiRelease(source, 0, 0, 0, 0, call);
  }
  if ((flags & 1) != 0) {
    SetRegs(call, Args({voice, kVoiceAttributes}));
    memory::crt::XMemFree(voice, kVoiceAttributes);
  }
  return voice;
}

}  // namespace xact
