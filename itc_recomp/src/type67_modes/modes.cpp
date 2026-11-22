// The type 6 and 7 modes' delegate functions NextMode and Leaving
// (docs/functions/type6-type7-modes.md) as plain C++: the messages queued
// in the mode's ring as the originals do it inline (no overflow check),
// GameMessages::Dispatch through its override (a guest call), then the
// screens' machines checked in the originals' order and the delegate's
// member functor rewritten, its two words built in the original's frame
// (+80, +84) and copied from there as the original's ld does.

#include "type67_modes/type67_modes.h"
#include "type67_modes/views.h"

#include "base/log.h"
#include "front_end_car_draw/front_end_car_draw.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "memory/heap.h"
#include "menu_sounds/menu_sounds.h"
#include "mode_tick/mode_tick.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "schema/menu_sounds.h"
#include "schema/strings_and_font.h"

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

using G = schema::Type6Type7ModesGlobals;

constexpr uint32_t kDispatch = 0x821062E8;

constexpr MachineAt kType6Next[] = {{4, 4},         {19432, 19436}, {9672, 9672},
                                    {30396, 30856}, {40948, 40952}, {62336, 62336},
                                    {71976, 71976}, {81616, 81616}};
constexpr MachineAt kType6Leave[] = {{4, 4},         {9672, 9672},   {30396, 30856},
                                     {40948, 40952}, {52692, 52696}, {62336, 62336},
                                     {71976, 71976}, {81616, 81616}};
constexpr MachineAt kType7Next[] = {{4, 8},         {19428, 19432}, {30608, 30612}, {9664, 9664},
                                    {42352, 42356}, {51996, 51996}, {61636, 61636}};
constexpr MachineAt kType7Leave[] = {{4, 8},         {30608, 30612}, {9664, 9664},
                                     {42352, 42356}, {51996, 51996}, {61636, 61636}};

// A machine's state, -1 when its cast gives none.
int32_t State(uint8_t* base, uint32_t block, const MachineAt& at) {
  if (int32_t(block + at.test) == 0) {
    return -1;
  }
  const uint32_t machine = block + at.machine;
  if (machine == 0) {
    return -1;
  }
  return int32_t(schema::ScreenMachine(base, machine).state());
}

// A message (`message`, 0, 0) queued in the mode's ring (+240: the count at
// +840, the start at +844), as the originals do it inline.
void Queue(uint8_t* base, const ModeView& m, uint32_t message) {
  const uint32_t count = m.ring_count() + 1;
  const uint32_t end = m.ring_start() + count - 1;
  m.set_ring_count(count);
  const game_modes::RingSlot queued{m.ring() + (end - (end / 50) * 50) * 12};
  queued.set_id(message);
  queued.set_arg0(0);
  queued.set_arg1(0);
}

void ClearAnswer(uint8_t* base, uint32_t answer) {
  const schema::ModeAnswer a(base, answer);
  a.set_next_type(0);
  a.set_changed(0);
}

// The delegate (mode +4 = mode +8 when that isn't 0) given a member functor:
// vtable 0x82010FA0, the mode, the function and adjustment from the frame.
void SetDelegate(uint8_t* base, const ModeView& m, uint32_t frame) {
  uint32_t delegate = m.delegate();
  if (int32_t(delegate) != 0) {
    m.set_delegate_object(m.t6.address);
    m.set_delegate_function(LoadU32(base, frame + 80));
    m.set_delegate_adjustment(LoadU32(base, frame + 84));
    m.set_delegate_vtable(kModeDelegateVtable);
  } else {
    delegate = 0;
  }
  m.set_delegate_ptr(delegate);
}

uint64_t SoundThunk(void* user, uint32_t function, const uint64_t args[4], int set, uint32_t r1,
                    uint32_t lr) {
  return static_cast<const Calls*>(user)->call_set(function, args, set, r1, lr);
}

// What differs between the two WaitForLoads.
struct LoadsLayout {
  uint32_t phase;
  uint32_t loaded[8];  // the members' machines, whose loaded words are read (0: none)
  uint32_t message;    // the loaded message
  uint32_t run_lr, slot_lr, finish_lr, draw_lr, dispatch_lr;
};
constexpr LoadsLayout kType6Loads = {
    91264, {19436, 52696, 81616, 9672, 40952, 4, 30856, 62336}, 34,
    0x82235F64, 0x82235FF4, 0x82236004, 0x8223602C, 0x822360F0};
constexpr LoadsLayout kType7Loads = {
    71284, {51996, 42356, 19432, 9664, 30612, 8, 0, 0}, 28,
    0x82246414, 0x82246484, 0x82246494, 0x822464BC, 0x82246580};

}  // namespace

const ModeLayout kType6Layout = {91264,       91256,       4,  0x82236100, 0x82235C18,
                                 0x82636F9C,  0x82236468,  0x82236174, kType6Next, 8,
                                 kType6Leave, 8,           {19432, 19436}};
const ModeLayout kType7Layout = {71284,       71276,       3,  0x82246590, 0x82246098,
                                 0x82637984,  0x822468B0,  0x82246604, kType7Next, 7,
                                 kType7Leave, 6,           {19428, 19432}};

void ModeNextMode(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                  uint32_t frame, const Calls& calls) {
  if (&layout == &kType6Layout) {
    ITC_NATIVE_RUNNING("sub_822363B8 Type6Mode::NextMode");
  } else {
    ITC_NATIVE_RUNNING("sub_82246800 Type7Mode::NextMode");
  }
  const ModeView m(base, mode, &layout == &kType7Layout);
  Queue(base, m, 1);
  Queue(base, m, 0);
  calls.call(kDispatch, frame, layout.next_lr, m.dispatcher(), 0);
  const uint32_t block = m.block();
  ClearAnswer(base, answer);
  bool leave = false;
  for (uint32_t i = 0; i < layout.next_count && !leave; i++) {
    leave = State(base, block, layout.next_checks[i]) == 10;
  }
  if (!leave) {
    return;
  }
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, layout.leaving);
  SetDelegate(base, m, frame);
  G::set_leaving_flag(base, 0);
  BlockView(base, m.block(), m.type7).set_phase(4);
}

uint64_t ModeWaitForLoads(uint8_t* base, bool type7, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_822463D8 Type7Mode::WaitForLoads");
  } else {
    ITC_NATIVE_RUNNING("sub_82235F28 Type6Mode::WaitForLoads");
  }
  const LoadsLayout& layout = type7 ? kType7Loads : kType6Loads;
  constexpr uint32_t kLoadingRun = 0x82247898, kLoadingFinish = 0x82247A60;
  constexpr uint32_t kGarageStageDraw = 0x8217FE60, kNextModeThunk = 0x82235C08;
  const uint32_t frame = sp - 160;
  const uint32_t answer = uint32_t(r3), mode = uint32_t(r4);
  const ModeView m(base, mode, type7);
  ClearAnswer(base, answer);
  BlockView(base, m.block(), type7).set_phase(1);
  calls.call(kLoadingRun, frame, layout.run_lr, ~uint64_t(0), r4);
  const uint32_t block = m.block();
  uint32_t loaded = 0xFFFFFFFF;
  for (const uint32_t at : layout.loaded) {
    if (at != 0) loaded &= schema::ScreenMachine(base, block + at).loaded();
  }
  uint32_t message = 1;
  if ((loaded & 1) != 0) {
    // The music's bank loading (the sound manager's slot 6 on bank 1).
    StoreU32(base, frame + 80, 1);
    const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
    const uint64_t args[4] = {manager.address, frame + 80, 0, 0};
    const uint64_t loading = calls.call_set(manager.vtable().is_bank_loading(), args, 2, frame,
                                            layout.slot_lr);
    if ((loading & 0xFF) == 0) {
      calls.call(kLoadingFinish, frame, layout.finish_lr, ~uint64_t(0), frame + 80);
      front_end_car_draw::GarageStageSetDrawWord(
          base, G::stand_holder(base).stage_block_0(), 0, frame);
      const uint64_t draw[4] = {G::stand_holder(base).stage_block_0(), 0, 0, 0};
      calls.call_set(kGarageStageDraw, draw, 3, frame, layout.draw_lr);
      StoreU32(base, frame + 88, 1);
      StoreU32(base, frame + 84, 2);
      menu_sounds::PlaySound(base, frame + 88, frame + 84, frame,
                             {const_cast<Calls*>(&calls), &SoundThunk});
      const uint32_t now = m.block();
      StoreU32(base, frame + 92, 0);
      BlockView(base, now, type7).set_phase(3);
      StoreU32(base, frame + 88, kNextModeThunk);
      uint32_t delegate = m.delegate();
      if (int32_t(delegate) != 0) {
        m.set_delegate_object(mode);
        m.set_delegate_function(LoadU32(base, frame + 88));
        m.set_delegate_adjustment(LoadU32(base, frame + 92));
        m.set_delegate_vtable(kModeDelegateVtable);
      } else {
        delegate = 0;
      }
      m.set_delegate_ptr(delegate);
      message = layout.message;
    }
  }
  const uint32_t count = m.ring_count() + 1;
  const uint32_t end = m.ring_start() + count - 1;
  m.set_ring_count(count);
  const game_modes::RingSlot queued{m.ring() + (end - (end / 50) * 50) * 12};
  queued.set_arg1(0);
  queued.set_arg0(0);
  queued.set_id(message);
  calls.call(kDispatch, frame, layout.dispatch_lr, m.dispatcher(), 0);
  return r3;
}

void ModeLeaving(uint8_t* base, const ModeLayout& layout, uint32_t answer, uint32_t mode,
                 uint32_t frame, const Calls& calls) {
  if (&layout == &kType6Layout) {
    ITC_NATIVE_RUNNING("sub_82236100 Type6Mode::Leaving");
  } else {
    ITC_NATIVE_RUNNING("sub_82246590 Type7Mode::Leaving");
  }
  const ModeView m(base, mode, &layout == &kType7Layout);
  ClearAnswer(base, answer);
  Queue(base, m, layout.leave_message);
  calls.call(kDispatch, frame, layout.leaving_lr, m.dispatcher(), mode);
  const uint32_t block = m.block();
  for (uint32_t i = 0; i < layout.leave_count; i++) {
    if (State(base, block, layout.leave_checks[i]) != 6) {
      return;
    }
  }
  if (State(base, block, layout.list) == 6) {
    BlockView(base, block, m.type7).set_phase(5);
    StoreU32(base, frame + 80, layout.tear_down);
    schema::Type67Car(base, m.type7 ? kType7Car : kType6Car).set_source(0xFFFFFFFF);
  } else if (State(base, block, layout.list) == 11) {
    BlockView(base, m.block(), m.type7).set_phase(5);
    guest::StoreU8(base, frame + 80, 1);
    StoreU32(base, frame + 84, 1);
    const uint32_t high = LoadU32(base, frame + 80), low = LoadU32(base, frame + 84);
    const BlockView to(base, m.block(), m.type7);
    to.set_answer_word0(high);
    to.set_answer_word(low);
    StoreU32(base, frame + 80, layout.tear_down);
  } else {
    return;
  }
  StoreU32(base, frame + 84, 0);
  SetDelegate(base, m, frame);
}

uint64_t ModeTearDown(uint8_t* base, bool type7, uint64_t r3, uint32_t mode, uint32_t sp,
                      const Calls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82246098 Type7Mode::TearDown");
  } else {
    ITC_NATIVE_RUNNING("sub_82235C18 Type6Mode::TearDown");
  }
  const ModeLayout& layout = type7 ? kType7Layout : kType6Layout;
  const uint32_t frame = sp - kTearDownFrame;
  const uint32_t answer = uint32_t(r3);
  const ModeView m(base, mode, type7);
  ClearAnswer(base, answer);
  if (int32_t(BlockView(base, m.block(), type7).phase()) == 5 &&
      StandScenePollTeardown(base, frame, calls) != 0) {
    BlockView(base, m.block(), type7).set_phase(6);
    // Bank 1's cues 0 and 2: when playing (the sound manager's slot 14),
    // stopped (slot 13); the indexes built in the frame.
    const uint32_t lr = type7 ? 0x82246114 : 0x82235C94;  // the first slot call's return
    struct Cue {
      uint32_t cue, playing, stop, playing_lr, stop_lr;
    };
    const Cue cues[2] = {{0, 80, 88, lr, lr + 0x2C}, {2, 96, 104, lr + 0x54, lr + 0x80}};
    for (const Cue& c : cues) {
      const schema::AudioManager manager = schema::MenuSoundsGlobals::sound_manager(base);
      StoreU32(base, frame + c.playing, c.cue);
      StoreU32(base, frame + c.playing + 4, 1);
      const uint64_t ask[4] = {manager.address, frame + c.playing + 4, frame + c.playing, 0};
      const uint64_t playing =
          calls.call_set(manager.vtable().is_cue_playing(), ask, 3, frame, c.playing_lr);
      if ((playing & 0xFF) == 0) {
        if (c.cue == 0) continue;
        break;
      }
      const schema::AudioManager now = schema::MenuSoundsGlobals::sound_manager(base);
      StoreU32(base, frame + c.stop, 0 + c.cue);
      StoreU32(base, frame + c.stop + 4, 1);
      const uint64_t stop[4] = {now.address, frame + c.stop + 4, frame + c.stop, 0};
      calls.call_set(now.vtable().stop_cue(), stop, 3, frame, c.stop_lr);
    }
  }
  const BlockView block(base, m.block(), type7);
  // The word after the phase (+91268, +71288) isn't in type67-screens.toml.
  if (int32_t(block.phase()) == 6 && LoadU32(base, block.phase_address() + 4) == 0) {
    const uint32_t high = block.answer_word0(), low = block.answer_word();
    const schema::ModeAnswer to(base, answer);
    to.set_word0(high);
    to.set_next_type(low);
  }
  if (type7) {
    Queue(base, m, 1);  // queued without a dispatch
  }
  return r3;
}

namespace {

// A member's Init in a mode's slot 0: its address (0 for the vtable slot 0
// call) and the member's offset in the block.
struct InitAt {
  uint32_t address;
  uint32_t offset;
  uint32_t lr;  // the call's return address
};
// The machines added to the dispatcher: the member's offset (tested, the
// cast's null check, when `test`) and the machine's.
struct AddAt {
  bool test;
  uint32_t member;
  uint32_t machine;
};
// The +9612 and +9616 words' stores: the machine (tested like an add), in
// the original's order (`second_first`: +9616 written before +9612).
struct WordsAt {
  bool test;
  uint32_t member;
  uint32_t machine;
  bool second_first = false;
};

constexpr InitAt kType6Inits[] = {
    {0x82235138, 4, 0x822368EC},     {0x822329A8, 9672, 0x822368F8},
    {0x822515A0, 19432, 0x82236904}, {0x82248E10, 40948, 0x82236918},
    {0x82259128, 30396, 0x82236924}, {0, 52692, 0x82236944},
    {0x821CB6E8, 62336, 0x82236958}, {0x821C9918, 71976, 0x8223696C},
    {0x821A8210, 81616, 0x82236980}};
constexpr AddAt kType6Adds[] = {{false, 4, 4},          {true, 19432, 19436},
                                {false, 9672, 9672},    {true, 30396, 30856},
                                {true, 40948, 40952},   {true, 52692, 52696},
                                {false, 62336, 62336},  {false, 71976, 71976},
                                {false, 81616, 81616}};
constexpr InitAt kType7Inits[] = {
    {0x822454B8, 4, 0x82246CFC},     {0x82242420, 9664, 0x82246D08},
    {0x822515A0, 19428, 0x82246D14}, {0x82248E10, 30608, 0x82246D20},
    {0, 42352, 0x82246D40},          {0x821CB090, 51996, 0x82246D54},
    {0x821C9430, 61636, 0x82246D68}};
constexpr AddAt kType7Adds[] = {{true, 4, 8},           {true, 19428, 19432},
                                {false, 9664, 9664},    {true, 30608, 30612},
                                {true, 42352, 42356},   {false, 51996, 51996},
                                {false, 61636, 61636}};

struct ModeInitLayout {
  uint32_t frame;
  uint32_t block_bytes;
  const InitAt* inits;
  size_t init_count;
  const AddAt* adds;
  size_t add_count;
  // The words' stores before the frame's answer is built, and after it.
  const WordsAt* words;
  size_t word_count;
  const WordsAt* words_after;
  size_t word_after_count;
  uint32_t answer;  // the block's doubleword, then two words 0
  uint32_t wait_for_loads;
  uint32_t lr_start, lr_strings, lr_stand, lr_show;
};

constexpr WordsAt kType6Words[] = {{false, 0, 4},          {true, 19432, 19436},
                                   {true, 30396, 30856},   {true, 40948, 40952},
                                   {false, 0, 9672},       {true, 52692, 52696}};
constexpr WordsAt kType6WordsAfter[] = {
    {false, 0, 62336}, {false, 0, 71976}, {false, 0, 81616}};
constexpr WordsAt kType7Words[] = {{true, 4, 8, true},    {true, 19428, 19432},
                                   {true, 30608, 30612},  {false, 0, 9664},
                                   {true, 42352, 42356}};
constexpr WordsAt kType7WordsAfter[] = {{false, 0, 51996}, {false, 0, 61636}};

constexpr ModeInitLayout kType6Init{
    kType6ModeInitFrame, 91272,
    kType6Inits, std::size(kType6Inits), kType6Adds, std::size(kType6Adds),
    kType6Words, std::size(kType6Words), kType6WordsAfter, std::size(kType6WordsAfter),
    91256, 0x82235F28,
    0x82236894, 0x822368A4, 0x822368AC, 0x82236BCC};
constexpr ModeInitLayout kType7Init{
    kType7ModeInitFrame, 71292,
    kType7Inits, std::size(kType7Inits), kType7Adds, std::size(kType7Adds),
    kType7Words, std::size(kType7Words), kType7WordsAfter, std::size(kType7WordsAfter),
    71276, 0x822463D8,
    0x82246CA4, 0x82246CB4, 0x82246CBC, 0x82246F58};

// The machine a WordsAt or AddAt names (0 for a member at address 0).
uint32_t Machine(uint32_t block, bool test, uint32_t member, uint32_t machine) {
  if (test && int32_t(block + member) == 0) {
    return 0;
  }
  return block + machine;
}

void StoreWords(uint8_t* base, uint32_t block, const WordsAt& w) {
  const schema::ScreenMachine machine(base, Machine(block, w.test, w.member, w.machine));
  if (w.second_first) {
    machine.set_delete_movie(0);
    machine.set_reenter(1);
  } else {
    machine.set_reenter(1);
    machine.set_delete_movie(0);
  }
}

}  // namespace

uint64_t ModeInit(uint8_t* base, bool type7, uint32_t mode, uint32_t sp,
                  const ModeInitCalls& calls) {
  if (type7) {
    ITC_NATIVE_RUNNING("sub_82246C90 Type7Mode::Init");
  } else {
    ITC_NATIVE_RUNNING("sub_82236880 Type6Mode::Init");
  }
  const ModeInitLayout& l = type7 ? kType7Init : kType6Init;
  const ModeView m(base, mode, type7);
  const uint32_t frame = sp - l.frame;
  StoreU32(base, frame, sp);
  auto call = [&](uint32_t target, uint32_t lr, std::initializer_list<uint64_t> args) {
    uint64_t regs[6] = {};
    int set = 0;
    for (const uint64_t a : args) regs[set++] = a;
    return calls.call(target, regs, set, frame, lr);
  };
  call(0x8211AD00, l.lr_start, {mode});
  call(0x8210A8D0, l.lr_strings,
       {schema::StringsAndFontGlobals::strings<schema::View>(base).address, 3});
  call(0x82180B00, l.lr_stand, {1});
  const uint32_t pages = memory::AllocPages(l.block_bytes, 4096);
  m.set_block(pages);
  if (pages != 0) {
    calls.construct(pages, frame);
  }
  if (m.block() != 0) {
    for (size_t i = 0; i < l.init_count; i++) {
      const InitAt& at = l.inits[i];
      const uint32_t object = m.block() + at.offset;
      uint32_t address = at.address;
      if (address == 0) {  // the power graph screen's vtable slot 0
        address = LoadU32(base, schema::Type67PowerGraphHead(base, object).vtable());
      }
      switch (address) {
        case 0x822515A0:
          ListScreenInit(base, object, frame, *calls.screens);
          continue;
        case 0x82248E10:
          PanelScreenInit(base, object, frame, *calls.screens);
          continue;
        case 0x82259128:
          NodeScreenInit(base, object, frame, *calls.screens);
          continue;
        default:
          break;
      }
      const InitInfo* info = nullptr;
      for (const InitInfo& k : kInits) {
        if (k.address == address) info = &k;
      }
      if (info != nullptr) {
        RunInit(base, address, object, frame - info->frame);
      } else {
        call(address, at.lr, {object});
      }
    }
    for (size_t i = 0; i < l.add_count; i++) {
      const AddAt& a = l.adds[i];
      const uint32_t machine = Machine(m.block(), a.test, a.member, a.machine);
      const uint32_t count = m.handler_count();
      m.set_handler_count(count + 1);
      schema::host::StoreU32(m.handlers() + (count << 2), machine);  // guest order (E5 step 4)
    }
    for (size_t i = 0; i < l.word_count; i++) {
      StoreWords(base, m.block(), l.words[i]);
    }
    guest::StoreU8(base, frame + 80, 1);
    StoreU32(base, frame + 84, 4);
    const uint32_t w0 = LoadU32(base, frame + 80), w1 = LoadU32(base, frame + 84);
    for (size_t i = 0; i < l.word_after_count; i++) {
      StoreWords(base, m.block(), l.words_after[i]);
    }
    // The answer's doubleword, then the phase and the word after it (not in
    // type67-screens.toml).
    BlockView(base, m.block(), type7).set_answer_word0(w0);
    BlockView(base, m.block(), type7).set_answer_word(w1);
    BlockView(base, m.block(), type7).set_phase(0);
    StoreU32(base, BlockView(base, m.block(), type7).phase_address() + 4, 0);
  }
  {
    const uint64_t args[6] = {1, 0, 0xFFFFFFFFFFFFFFFFull};
    calls.call(0x822478D0, args, 3, frame, l.lr_show);
  }
  const uint32_t passes = schema::Type67ScreensGlobals::fade_in_passes(base);
  mode_tick::ScreenFadesFadeIn(base, passes, 0, 0, 0, 0);
  // The delegate's member functor, built at the frame's +80.
  StoreU32(base, frame + 84, 0);
  StoreU32(base, frame + 80, l.wait_for_loads);
  const uint32_t delegate = m.delegate();
  uint32_t holder = 0;
  if (int32_t(delegate) != 0) {
    holder = delegate;
    const uint32_t function = LoadU32(base, frame + 80), adjustment = LoadU32(base, frame + 84);
    m.set_delegate_object(mode);
    m.set_delegate_function(function);
    m.set_delegate_adjustment(adjustment);
    m.set_delegate_vtable(kModeDelegateVtable);
  }
  m.set_delegate_ptr(holder);
  return passes;
}

std::optional<uint64_t> StandModeRelease(uint8_t* base, uint32_t mode, uint32_t sp,
                                         const Calls& calls) {
  const schema::StandMode m(base, mode);
  if (!memory::IsPages(m.block())) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82246038 StandMode::Release");
  constexpr uint32_t kStandSceneRelease = 0x82180738, kStringsRelease = 0x8210A538;
  constexpr uint32_t kDeleteAllJump = 0x8211E370;
  const uint32_t frame = sp - kStandModeReleaseFrame;
  StoreU32(base, frame, sp);
  if (!StandSceneRelease(base)) {
    calls.call(kStandSceneRelease, frame, 0x82246050, mode, 0);
  }
  memory::FreePages(m.block());
  m.set_block(0);
  const uint64_t r3 = calls.call(kStringsRelease, frame, 0x8224607C,
                                 schema::StringsAndFontGlobals::strings<schema::View>(base).address,
                                 3);
  return calls.call(kDeleteAllJump, frame, 0x82246080, r3, 0);
}

}  // namespace type67_modes
