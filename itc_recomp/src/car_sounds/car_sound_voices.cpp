// The car sounds' voice holders natively (car_sounds.h,
// docs/functions/car-sounds.md): XAudio's entry points through their plain
// C++ (src/xaudio/voices.h), each in a frame below the caller's, with the
// registers set as the original sets them before the call. Strict floating
// point (CMakeLists.txt), as the original's single-precision steps.

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "memory/host_copy.h"
#include "schema/car_sounds.h"

#include <array>
#include <initializer_list>

namespace car_sounds {

namespace {

namespace xa = xaudio;

using Holder = schema::CarSoundVoiceHolder;
using Sound = schema::CarSound;
using G = schema::CarSoundsGlobals;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}
double LoadF(uint32_t address) { return double(guest::LoadFloat(address)); }
// A single-precision step's result, as the register holds it.
double Single(double value) { return double(float(value)); }

xa::Regs Args(std::initializer_list<uint64_t> r) {
  xa::Regs regs;
  for (const uint64_t value : r) {
    regs.r[regs.count++] = value;
  }
  return regs;
}
// The registers the original sets before calling a function whose plain C++
// runs here instead.
void SetRegs(const GuestCall& call, xa::Regs regs) { call(0, 0, regs); }

// The API entry points' frames (their originals' sizes).
constexpr uint32_t kApiFrame = 112;

// The voice word in `slot` (the original's (slot + 1) x 4 from the holder)
// and the voice in it, by its offset in the holder (in host memory since
// E5 step 4: the voice words are read and written through the holder's
// view; the offset wraps as the original's address does for slot -1).
uint32_t VoiceOffset(uint32_t slot) { return 4 + (slot << 2); }
uint32_t VoiceWord(uint32_t holder, uint32_t slot) {
  return Holder(holder).voices_address() + (slot << 2);
}
uint32_t VoiceAt(uint32_t holder, uint32_t slot) {
  return uint32_t(Holder(holder).load_at(VoiceOffset(slot), 4));
}
void SetVoiceAt(uint32_t holder, uint32_t slot, uint32_t voice) {
  Holder(holder).store_at(VoiceOffset(slot), 4, voice);
}
// The paused word of slot `i` (+24 + 4 i).
uint32_t PausedAt(uint32_t holder, uint32_t i) {
  return uint32_t(Holder(holder).load_at(24 + 4 * i, 4));
}
void SetPausedAt(uint32_t holder, uint32_t i, uint32_t value) {
  Holder(holder).store_at(24 + 4 * i, 4, value);
}
// A float constant's value, for a setter (lfs then stfs: the same bits).
float Constant(uint32_t constant) { return guest::LoadFloat(constant); }

// The constants.
constexpr uint32_t kZero = 0x82000A34;           // 0.0
constexpr uint32_t kPitchBias = 0x8200801C;
constexpr uint32_t kPitchScale = 0x82008020;
constexpr uint32_t kPitchScaleR9 = 0x82008024;
constexpr uint32_t kPitchFloorR7 = 0x82002A24;
constexpr uint32_t kPitchFloor = 0x82001BE0;
constexpr uint32_t kOne = 0x82007FC8;            // 1.0
constexpr uint32_t kEngineRest = 0x82003384;

// A car sound's 12 voice holders: the engine's two layers', then the
// parts'.
struct Holders {
  uint32_t layer, pitched_layer, hits, tires, gear, roll, events, fade, turbo, wind, horn,
      pass_by;
};
Holders HoldersOf(uint32_t sound) {
  const Sound s(sound);
  const schema::CarSoundEngine e(s.engine_address());
  using Layer = schema::CarSoundEngineLayer;
  return {Layer(e.layer_address()).holder_address(),
          Layer(e.pitched_layer_address()).holder_address(),
          schema::CarSoundHits(s.hits_address()).holder_address(),
          schema::CarSoundTires(s.tires_address()).holder_address(),
          schema::CarSoundGear(s.gear_address()).holder_address(),
          schema::CarSoundRoll(s.roll_address()).holder_address(),
          schema::CarSoundEvents(s.events_address()).holder_address(),
          schema::CarSoundFade(s.fade_address()).holder_address(),
          schema::CarSoundTurbo(s.turbo_address()).holder_address(),
          schema::CarSoundWind(s.wind_address()).holder_address(),
          schema::CarSoundHorn(s.horn_address()).holder_address(),
          schema::CarSoundPassBy(s.pass_by_address()).holder_address()};
}

// The 12 holders Stop and Release walk, in their order.
std::array<uint32_t, 12> ClearOrder(uint32_t sound) {
  const Holders h = HoldersOf(sound);
  return {h.layer, h.pitched_layer, h.hits, h.tires, h.gear, h.events,
          h.roll,  h.fade,          h.turbo, h.wind, h.horn, h.pass_by};
}

void StoreF(uint32_t address, uint32_t constant) {
  guest::StoreFloat(address, guest::LoadFloat(constant));
}
void StoreB(uint32_t address, uint8_t value) { guest::StoreU8(address, value); }

// The frames of the plain C++ callees (their originals' sizes).
constexpr uint32_t kAddVoiceFrame = 496;
constexpr uint32_t kCreateFrame = 128;
constexpr uint32_t kSubmitFrame = 128;

// The 12 holders PauseAll and ResumeAll walk, in their order.
std::array<uint32_t, 12> PauseOrder(uint32_t sound) {
  const Holders h = HoldersOf(sound);
  return {h.layer, h.pitched_layer, h.turbo, h.hits, h.tires, h.gear,
          h.events, h.roll,         h.fade,  h.wind, h.horn, h.pass_by};
}

}  // namespace

uint32_t SetPitch(uint32_t holder, uint32_t slot, double f1, uint32_t r7,
                  uint32_t sp, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D158 CarSound::SetPitch");
  const double floor = LoadF(r7 != 0 ? kPitchFloorR7 : kPitchFloor);
  if (f1 == LoadF(kZero)) {
    f1 = floor;
  } else {
    f1 = Single(Single(f1 - LoadF(kPitchBias)) * LoadF(kPitchScale));
    if (f1 < floor) {
      f1 = floor;
    }
  }
  const uint32_t voice = VoiceAt(holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = f1;
  SetRegs(call, regs);
  // The tail call's frame where the original's callee makes it.
  xa::FrameBelow(sp, kApiFrame);
  return xa::ApiSetFrequencyRatio(voice, f1, call);
}

uint32_t SetPitchScaled(uint32_t holder, uint32_t slot, double f1, double f3,
                        uint32_t r7, uint32_t r9, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D080 CarSound::SetPitchScaled");
  double f0;
  if (f1 == LoadF(kZero)) {
    f0 = LoadF(r7 != 0 ? kPitchFloorR7 : kPitchFloor);
  } else {
    double f12 = Single(f1 - LoadF(kPitchBias));
    double f13 = Single(double(int64_t(int32_t(r9))));
    f13 = Single(f13 * LoadF(kPitchScaleR9));
    f12 = Single(f12 * f13);
    f0 = Single(f12 * LoadF(kPitchScale));
    f13 = r7 != 0 ? LoadF(kPitchFloorR7) : Single(f13 * LoadF(kPitchFloor));
    if (f0 < f13) {
      f0 = f13;
    }
  }
  const double ratio = Single(Single(f0 + f3) - LoadF(kOne));
  const uint32_t voice = VoiceAt(holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = ratio;
  SetRegs(call, regs);
  xa::FrameBelow(frame, kApiFrame);
  return xa::ApiSetFrequencyRatio(voice, ratio, call);
}

uint32_t IsPlaying(uint32_t holder, uint32_t slot, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D1D8 CarSound::IsPlaying");
  if (int32_t(slot) == -1) {
    return 0;
  }
  const uint32_t voice = VoiceAt(holder, slot);
  if (voice == 0) {
    return 0;
  }
  SetRegs(call, Args({voice, frame + 80}));
  xa::FrameBelow(frame, kApiFrame);
  xa::ApiGetState(voice, frame + 80, 0, 0, 0, call);
  return (guest::LoadU8(frame + 80) & 1) != 0 ? 1 : 0;
}

uint32_t UpdateVoices(uint32_t holder, uint64_t output, uint32_t frame,
                      const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D378 CarSound::UpdateVoices");
  Store(frame + 136, uint32_t(output >> 32));
  Store(frame + 140, uint32_t(output));
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t voice = VoiceAt(holder, i);
    r3 = voice;
    if (voice == 0) {
      continue;
    }
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(frame, kApiFrame);
    r3 = xa::ApiGetState(voice, frame + 80, 0, 0, 0, call);
    const uint8_t state = guest::LoadU8(frame + 80);
    if ((state & 4) != 0) {
      voice = VoiceAt(holder, i);
      SetRegs(call, Args({voice, 0}));
      xa::FrameBelow(frame, kApiFrame);
      xa::ApiStop(voice, 0, 0, 0, 0, call);
      voice = VoiceAt(holder, i);
      SetRegs(call, Args({voice}));
      xa::FrameBelow(frame, kApiFrame);
      r3 = xa::ApiRelease(voice, 0, 0, 0, 0, call);
      SetVoiceAt(holder, i, 0);
    } else if ((state & 1) != 0) {
      voice = VoiceAt(holder, i);
      SetRegs(call, Args({voice, frame + 136}));
      xa::FrameBelow(frame, kApiFrame);
      r3 = xa::ApiSetOutputMatrix(voice, frame + 136, 0, 0, 0, call);
    }
  }
  return r3;
}

uint32_t Clear(uint32_t holder, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D320 CarSound::Clear");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t voice = VoiceAt(holder, i);
    if (voice == 0) {
      // The original's r3: the slot's word, 0.
      r3 = 0;
      continue;
    }
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(frame, kApiFrame);
    xa::ApiStop(voice, 0, 0, 0, 0, call);
    voice = VoiceAt(holder, i);
    SetRegs(call, Args({voice}));
    xa::FrameBelow(frame, kApiFrame);
    r3 = xa::ApiRelease(voice, 0, 0, 0, 0, call);
    SetVoiceAt(holder, i, 0);
  }
  Holder(holder).set_voice_count(0);
  return r3;
}

uint32_t Pause(uint32_t holder, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D240 CarSound::Pause");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    uint32_t voice = VoiceAt(holder, i);
    SetPausedAt(holder, i, 0);
    r3 = voice;
    if (voice == 0) {
      continue;
    }
    SetRegs(call, Args({voice, frame + 80}));
    xa::FrameBelow(frame, kApiFrame);
    r3 = xa::ApiGetState(voice, frame + 80, 0, 0, 0, call);
    if ((guest::LoadU8(frame + 80) & 1) == 0) {
      continue;
    }
    voice = VoiceAt(holder, i);
    SetPausedAt(holder, i, 1);
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(frame, kApiFrame);
    r3 = xa::ApiStop(voice, 0, 0, 0, 0, call);
  }
  return r3;
}

uint32_t Resume(uint32_t holder, const Thread& thread, uint32_t frame,
                const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D2C0 CarSound::Resume");
  uint32_t r3 = holder;
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t voice = VoiceAt(holder, i);
    r3 = voice;
    if (voice == 0 || PausedAt(holder, i) == 0) {
      continue;
    }
    SetRegs(call, Args({voice, 0}));
    xa::FrameBelow(frame, kApiFrame);
    r3 = xa::ApiStart(voice, 0, thread, call);
  }
  return r3;
}

uint32_t PauseAll(uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E2B8 CarSound::PauseAll");
  uint32_t r3 = 0;
  for (const uint32_t holder : PauseOrder(sound)) {
    SetRegs(call, Args({holder}));
    r3 = Pause(holder, xa::FrameBelow(frame, 112), call);
  }
  return r3;
}

uint32_t ResumeAll(uint32_t sound, const Thread& thread, uint32_t frame,
                   const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E350 CarSound::ResumeAll");
  uint32_t r3 = 0;
  for (const uint32_t holder : PauseOrder(sound)) {
    SetRegs(call, Args({holder}));
    r3 = Resume(holder, thread, xa::FrameBelow(frame, 112), call);
  }
  return r3;
}

uint32_t AddVoice(uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210CE38 CarSound::AddVoice");
  const uint32_t f = frame;
  // The six-channel map at +80 (its entries at +96: input 0 to output i,
  // gain 0.0) and the outputs at +144 (two at +160: the null voice and the
  // submix, each with the map).
  for (const uint32_t at : {100u, 108u, 116u, 124u, 132u, 140u}) {
    StoreF(f + at, kZero);
  }
  StoreB(f + 121, 3);
  StoreB(f + 96, 0);
  StoreB(f + 97, 0);
  StoreB(f + 104, 0);
  StoreB(f + 105, 1);
  StoreB(f + 129, 4);
  StoreB(f + 112, 0);
  StoreB(f + 113, 2);
  StoreB(f + 120, 0);
  StoreB(f + 128, 0);
  StoreB(f + 137, 5);
  StoreB(f + 136, 0);
  StoreB(f + 80, 6);
  StoreB(f + 144, 2);
  Store(f + 160, 0);
  Store(f + 84, f + 96);
  Store(f + 148, f + 160);
  Store(f + 164, f + 80);
  Store(f + 168, G::submix());
  Store(f + 172, f + 80);
  uint32_t slot = 0;
  while (VoiceAt(holder, slot) != 0) {
    if (++slot >= 4) {
      return 0xFFFFFFFFu;
    }
  }
  // The voice's description at +304 (92 bytes cleared), wave 0's format.
  for (uint32_t i = 0; i < 92; i += 4) {
    Store(f + 304 + i, 0);
  }
  SetRegs(call, Args({view, 0, f + 304}));
  WaveBankFormat(view, 0, f + 304);
  StoreB(f + 361, 2);
  StoreB(f + 362, 6);
  StoreB(f + 363, 1);
  // The voice word, where XAudio's CreateSourceVoice writes the voice (an
  // interior handle: its store resolves it).
  const uint32_t out = VoiceWord(holder, slot);
  SetVoiceAt(holder, slot, 0);
  Store(f + 376, f + 144);
  SetRegs(call, Args({f + 304, out}));
  xa::ApiCreateSourceVoice(f + 304, out, voices.native_heap, voices.thread,
                           xa::FrameBelow(f, kCreateFrame), call);
  // The packet at +208: the data, the length (+212), the loop region.
  Store(f + 208, 0);
  for (uint32_t i = 0; i < 84; i += 4) {
    Store(f + 212 + i, 0);
  }
  SetRegs(call, Args({view, wave, f + 88}));
  WaveBankLength(view, wave, f + 88);
  SetRegs(call, Args({view, wave, f + 208}));
  WaveBankData(view, wave, f + 208);
  Store(f + 176, 0);
  for (uint32_t i = 0; i < 20; i += 4) {
    Store(f + 180 + i, 0);
  }
  Store(f + 212, Load(f + 88));
  SetRegs(call, Args({view, wave, f + 176}));
  if (!WaveBankEntry(view, wave, f + 176)) {
    call(0x8210C998, 0x8210CFEC, Args({view, wave, f + 176}));
  }
  if (loop != 0) {
    // The entry's format (+4): bit 31 16-bit samples, bits 2-4 the channels.
    const uint32_t format = Load(f + 180);
    const uint32_t bits = (format >> 31) == 1 ? 16 : 8;
    const uint32_t frame_bytes = (bits * ((format >> 2) & 7)) >> 3;
    Store(f + 220, frame_bytes * Load(f + 192));
    Store(f + 216, 0xFFFFFFFFu);
    Store(f + 224, frame_bytes * Load(f + 196));
  }
  const uint32_t voice = VoiceAt(holder, slot);
  SetRegs(call, Args({voice, f + 208, 1}));
  xa::FrameBelow(f, kSubmitFrame);
  xa::ApiSubmitPacket(voice, f + 208, 1, 0, 0, call);
  const Holder h(holder);
  h.set_voice_count(h.voice_count() + 1);
  return slot;
}

uint32_t PlayOnce(uint32_t holder, uint32_t view, uint32_t wave, uint32_t loop,
                  double volume, double pitch, const Voices& voices, uint32_t frame,
                  const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E3E8 CarSound::PlayOnce");
  SetRegs(call, Args({holder, view, wave, loop}));
  const uint32_t slot = AddVoice(holder, view, wave, loop, voices,
                                 xa::FrameBelow(frame, kAddVoiceFrame), call);
  if (int32_t(slot) == -1) {
    return slot;
  }
  uint32_t voice = VoiceAt(holder, slot);
  SetRegs(call, Args({voice, 0}));
  xa::FrameBelow(frame, kApiFrame);
  xa::ApiStart(voice, 0, voices.thread, call);
  voice = VoiceAt(holder, slot);
  xa::Regs regs = Args({voice});
  regs.f1_set = true;
  regs.f1 = volume;
  SetRegs(call, regs);
  xa::FrameBelow(frame, kApiFrame);
  xa::ApiSetVolume(voice, volume, call);
  regs = Args({holder, slot});
  regs.f1_set = true;
  regs.f1 = pitch;
  SetRegs(call, regs);
  SetPitch(holder, slot, pitch, 0, frame, call);
  return slot;
}

uint32_t EngineLayerStart(uint32_t layer, uint32_t view, const Voices& voices,
                          uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D410 EngineLayer::Start");
  const schema::CarSoundEngineLayer l(layer);
  l.set_table(0);
  l.set_table_copy(0);
  l.set_table_volume(0);
  l.set_table_volume_copy(0);
  // memset (sub_825A71D0) with 0: the same bytes.
  for (uint32_t i = 0; i < 24; i += 4) {
    l.store_at(20 + i, 4, 0);  // amounts
  }
  uint32_t r3 = 0;
  for (uint32_t wave = 0; wave < 4; wave++) {
    SetRegs(call, Args({l.holder_address(), view, wave, 1}));
    r3 = AddVoice(l.holder_address(), view, wave, 1, voices,
                  xa::FrameBelow(frame, kAddVoiceFrame), call);
  }
  return r3;
}

uint32_t EngineSoundStart(uint32_t engine, uint64_t index, uint32_t views,
                          const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D6C0 EngineSound::Start");
  const schema::CarSoundEngine e(engine);
  const schema::CarSoundEngineViews vs(views);
  SetRegs(call, Args({e.pitched_layer_address(), index, vs.pitched_layer_view().address}));
  EngineLayerStart(e.pitched_layer_address(), vs.pitched_layer_view().address, voices,
                   xa::FrameBelow(frame, 112), call);
  SetRegs(call, Args({e.layer_address(), index, vs.layer_view().address}));
  const uint32_t r3 = EngineLayerStart(e.layer_address(), vs.layer_view().address, voices,
                                       xa::FrameBelow(frame, 112), call);
  e.set_load(Constant(kZero));
  e.set_idle_speed(Constant(kEngineRest));
  return r3;
}

uint32_t TireSoundStart(uint32_t part, uint32_t index, uint32_t view,
                        const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210D7F8 TireSound::Start");
  const schema::CarSoundTires t(part);
  t.set_index(uint8_t(index));
  t.set_volume(Constant(kZero));
  t.set_slip_time(Constant(kZero));
  t.set_view(view);
  t.set_slip_steps(0);
  t.set_pitch(0);
  uint32_t r3 = 0;
  for (int i = 0; i < 2; i++) {
    SetRegs(call, Args({t.holder_address(), view, 0, 1}));
    r3 = AddVoice(t.holder_address(), view, 0, 1, voices,
                  xa::FrameBelow(frame, kAddVoiceFrame), call);
  }
  t.set_slot(0);
  t.set_was_active(0);
  t.set_slip_ticks(0);
  t.set_ramp_step(0);
  t.set_ramp_length(0);
  t.set_slot_2(1);
  t.set_was_active_2(0);
  t.set_slip_ticks_2(0);
  t.set_ramp_step_2(0);
  t.set_ramp_length_2(0);
  return r3;
}

uint32_t CarSoundStart(uint32_t sound, uint32_t views, uint32_t view,
                       const Voices& voices, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E030 CarSound::Start");
  const Sound s(sound);
  const auto index = [&] { return s.index(); };
  uint32_t i = index();
  SetRegs(call, Args({s.engine_address(), i, views}));
  EngineSoundStart(s.engine_address(), i, views, voices, xa::FrameBelow(frame, 112),
                   call);
  i = index();
  SetRegs(call, Args({s.tires_address(), i, view}));
  TireSoundStart(s.tires_address(), i, view, voices, xa::FrameBelow(frame, 128), call);
  constexpr uint32_t kNone = 0xFFFFFFFFu;
  i = index();
  const schema::CarSoundHits hits(s.hits_address());
  hits.set_view(view);
  hits.set_index(uint8_t(i));
  hits.set_wait_ticks(0);
  hits.set_scrape_ticks(0);
  hits.set_scrape_slot(kNone);
  hits.set_strength(0);
  hits.set_since_impact(kNone);
  i = index();
  const schema::CarSoundGear gear(s.gear_address());
  gear.set_view(view);
  gear.set_wait_ticks(0);
  gear.set_slot(kNone);
  gear.set_index(uint8_t(i));
  i = index();
  const schema::CarSoundEvents events(s.events_address());
  events.set_view(view);
  events.set_wave18_wait(0);
  events.set_index(uint8_t(i));
  i = index();
  const schema::CarSoundRoll roll(s.roll_address());
  roll.set_view(view);
  roll.set_index(uint8_t(i));
  SetRegs(call, Args({roll.address, view, 1, 1}));
  const uint32_t r3 = AddVoice(roll.address, view, 1, 1, voices,
                               xa::FrameBelow(frame, kAddVoiceFrame), call);
  i = index();
  const schema::CarSoundFade fade(s.fade_address());
  fade.set_view(view);
  fade.set_playing(0);
  fade.set_slot(kNone);
  fade.set_fade_out(Constant(kPitchFloor));
  fade.set_index(uint8_t(i));
  fade.set_fade_in(Constant(kPitchFloor));
  i = index();
  const schema::CarSoundTurbo turbo(s.turbo_address());
  turbo.set_last_boost(Constant(kZero));
  turbo.set_view(view);
  turbo.set_volume(Constant(kZero));
  turbo.set_slot(kNone);
  turbo.set_pitch(0);
  turbo.set_wait_ticks(0);
  turbo.set_index(uint8_t(i));
  turbo.set_volume_steps(0);
  i = index();
  const schema::CarSoundWind wind(s.wind_address());
  wind.set_view(view);
  wind.set_played(0);
  wind.set_unknown_52(kNone);
  wind.set_index(uint8_t(i));
  i = index();
  const schema::CarSoundHorn horn(s.horn_address());
  horn.set_view(view);
  horn.set_slot(kNone);
  horn.set_slot_2(kNone);
  horn.set_playing(0);
  horn.set_index(uint8_t(i));
  horn.set_playing_2(0);
  i = index();
  const schema::CarSoundPassBy pass_by(s.pass_by_address());
  pass_by.set_view(view);
  pass_by.set_unknown_48(kNone);
  pass_by.set_unknown_52(0);
  pass_by.set_index(uint8_t(i));
  s.set_last_doppler(Constant(kZero));
  return r3;
}

namespace {

uint32_t ClearHolders(uint32_t sound, uint32_t frame, const GuestCall& call) {
  uint32_t r3 = 0;
  for (const uint32_t holder : ClearOrder(sound)) {
    SetRegs(call, Args({holder}));
    r3 = Clear(holder, xa::FrameBelow(frame, 128), call);
  }
  return r3;
}

uint32_t SoundAt(uint32_t offset) {
  const uint32_t array = G::sounds().address;
  return array != 0 ? array + offset : 0;
}

}  // namespace

uint32_t Stop(uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E218 CarSound::Stop");
  const uint32_t r3 = ClearHolders(sound, frame, call);
  Sound(sound).set_live(0);
  return r3;
}

uint32_t StopAll(uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F7C8 CarSounds::StopAll");
  uint32_t r3 = 0;
  for (uint32_t offset = 0; offset < 7 * Sound::kSize; offset += Sound::kSize) {
    const uint32_t sound = SoundAt(offset);
    SetRegs(call, Args({sound}));
    r3 = Stop(sound, xa::FrameBelow(frame, 112), call);
  }
  return r3;
}

uint32_t Release(uint32_t sound, uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210E170 CarSound::Release");
  ClearHolders(sound, frame, call);
  const uint32_t matrix =
      schema::X3DAudioDspSettings(Sound(sound).dsp_address()).matrix().address;
  SetRegs(call, Args({matrix}));
  uint32_t r3 = matrix;  // the native free leaves r3 the block
  if (!memory::crt::OperatorDeleteArray(matrix)) {
    r3 = call(0x825A71C0, 0x8210E1F4, Args({matrix}));
  }
  Sound(sound).set_live(0);
  return r3;
}

uint32_t CarSoundsRelease(uint32_t frame, const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210F750 CarSounds::Release");
  for (uint32_t offset = 0; offset < 7 * Sound::kSize; offset += Sound::kSize) {
    const uint32_t sound = SoundAt(offset);
    SetRegs(call, Args({sound}));
    Release(sound, xa::FrameBelow(frame, 112), call);
  }
  const uint32_t array = G::sounds().address;
  if (array != 0) {
    SetRegs(call, Args({array, 3}));
    const Destroyed done = CarSoundsDestroy(array, 3);
    if (done.to_free != 0) {
      // operator delete[] of the array's block: in host memory since E5
      // step 4 (CarSoundArray, made by CarSounds::Init).
      if (done.array) {
        schema::CarSoundArray(done.to_free).Delete();
      } else if (!memory::crt::OperatorDelete(done.to_free)) {
        call(0x825A70E8, 0x8210F7A4, Args({done.to_free}));
      }
    }
    SetRegs(call, Args({done.r3}));
  }
  const uint32_t submix = G::submix();
  SetRegs(call, Args({submix}));
  xa::FrameBelow(frame, kApiFrame);
  return xa::ApiRelease(submix, 0, 0, 0, 0, call);
}

namespace {

// operator new[] (sub_825A6940): its plain C++, the guest's where that
// refuses.
uint32_t NewArray(uint32_t size, uint32_t return_address, const GuestCall& call) {
  if (const auto block = memory::crt::OperatorNewArray(size)) {
    return *block;
  }
  return call(0x825A6940, return_address, Args({size}));
}

}  // namespace

uint32_t CarSoundsInit(const Voices& voices, uint32_t frame,
                       const GuestCall& call) {
  ITC_NATIVE_RUNNING("sub_8210FE00 CarSounds::Init");
  const uint32_t f = frame;
  SetRegs(call, Args({8864}));
  // operator new[] of 8,864 bytes (0x8210FE18): the array's block in host
  // memory since E5 step 4 (freed by CarSounds::Release).
  const uint32_t made = schema::CarSoundArray::New().address;
  uint32_t array = 0;
  if (made != 0) {
    const schema::CarSoundArray block(made);
    array = block.sound(0).address;
    block.set_count(7);
    for (uint32_t k = 0; k < 7; k++) {
      SetRegs(call, Args({array + Sound::kSize * k}));
      CarSoundConstruct(array + Sound::kSize * k);
    }
  }
  G::set_sounds(array);
  // memset (sub_825A71D0) with 0: the same bytes (the game's memset over a
  // handle, memory/host_copy.h).
  if (!memory::FillWithHandles(array, 0, 7 * Sound::kSize)) {
    for (uint32_t i = 0; i < 7 * Sound::kSize; i += 4) {
      Store(array + i, 0);
    }
  }
  for (uint32_t k = 0; k < 7; k++) {
    const uint32_t at = G::sounds().address;
    const uint32_t sound = at != 0 ? at + Sound::kSize * k : 0;
    if (sound != 0) {
      SetRegs(call, Args({sound}));
      CarSoundConstruct(sound);
    }
    const Sound s(sound);
    s.set_engine_gain(Constant(kOne));
    s.set_index(uint8_t(k));
    s.set_vehicle(0);
    s.set_tunnel_reverb(0);
    s.set_live(0);
    SetRegs(call, Args({sound}));
    const uint32_t matrix = NewArray(24, 0x8210DF88, call);
    SetUp(sound, matrix);
  }
  // The submix's map at +208 (six entries at +96: input i to output i,
  // gain [0x82002208], 0.0 for 2), its outputs at +144 (one, at +256: the
  // null voice with the map at +160: input 0 to output i, gain 1.0).
  for (const uint32_t at : {164u, 172u, 180u, 188u, 196u, 204u}) {
    StoreF(f + at, kOne);
  }
  G::set_unknown_826C5C40(0);
  StoreB(f + 160, 0);
  StoreB(f + 161, 0);
  StoreB(f + 168, 0);
  StoreB(f + 169, 1);
  Store(f + 148, f + 256);
  StoreB(f + 176, 0);
  StoreB(f + 177, 2);
  StoreB(f + 184, 0);
  StoreB(f + 185, 3);
  StoreB(f + 192, 0);
  StoreB(f + 193, 4);
  StoreB(f + 200, 0);
  StoreB(f + 201, 5);
  StoreB(f + 144, 1);
  Store(f + 256, 0);
  Store(f + 260, f + 208);
  // memset (sub_825A71D0) of the description's 27 bytes from +225.
  for (uint32_t i = 0; i < 27; i++) {
    StoreB(f + 225 + i, 0);
  }
  constexpr uint32_t kSubmixGain = 0x82002208;
  StoreB(f + 96, 0);
  StoreB(f + 97, 0);
  StoreB(f + 104, 1);
  StoreB(f + 105, 1);
  StoreB(f + 112, 2);
  StoreB(f + 113, 2);
  StoreB(f + 120, 3);
  StoreB(f + 121, 3);
  StoreF(f + 100, kSubmixGain);
  StoreF(f + 108, kSubmixGain);
  StoreB(f + 128, 4);
  StoreF(f + 124, kSubmixGain);
  StoreB(f + 129, 4);
  StoreF(f + 132, kSubmixGain);
  StoreB(f + 136, 5);
  StoreB(f + 137, 5);
  StoreF(f + 116, kZero);
  StoreB(f + 208, 6);
  StoreF(f + 140, kSubmixGain);
  Store(f + 236, kOne + 72);
  StoreB(f + 233, 6);
  StoreB(f + 224, 0);
  Store(f + 212, f + 96);
  StoreB(f + 225, 1);
  Store(f + 228, 48000);
  Store(f + 240, f + 144);
  SetRegs(call, Args({f + 224, G::submix_address()}));
  xa::ApiCreateSubmixVoice(f + 224, G::submix_address(), voices.native_heap, voices.thread,
                           xa::FrameBelow(f, 128), call);
  // The effect's parameters: the block 0x82616638, 48 bytes.
  Store(f + 84, 0);
  Store(f + 80, G::reverb_off_address());
  Store(f + 84, 48);
  const uint32_t submix = G::submix();
  SetRegs(call, Args({submix, 0, 1, 3, f + 80}));
  xa::FrameBelow(f, 144);
  return xa::ApiSetEffectParameters(submix, 0, 1, 3, f + 80, call);
}

}  // namespace car_sounds
