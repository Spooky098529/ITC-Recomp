// The action mappers as plain C++: see action_mappers.h.

#include "action_mappers/action_mappers.h"

#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "input/input_manager.h"
#include "memory/c_runtime_heap.h"
#include "schema/action_mappers.h"
#include "schema/app_states.h"
#include "schema/input.h"
#include "std_list/ptr_list.h"

#include <immintrin.h>

#include <bit>
#include <cmath>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <optional>

namespace action_mappers {

namespace {

using G = schema::ActionMappersGlobals;
using Mapper = schema::ActionMapper;
using State = schema::ActionMapperState;
using AnalogStage = schema::ActionMapperAnalog;
using Threshold = schema::ActionThreshold;
using Config = schema::ActionMapperConfig;
using Repeat = schema::ActionRepeat;
using Curve = schema::ActionCurve;
using ClientList = schema::InputClientList;
using ClientNode = schema::InputClientNode;
// The mapper's array elements by address (the mapper is in host memory, E5
// step 4: its arrays embedded objects, so an element's address is an
// interior handle; the views are dual, so a guest address reads as before).
using Word = schema::ActionWord;
using Half = schema::ActionHalf;
using Float = schema::ActionFloat;
inline double Fmuls(double a, double b) {
  return double(float(a * b));
}
inline double Fadds(double a, double b) {
  return double(float(a + b));
}
inline double Fsubs(double a, double b) {
  return double(float(a - b));
}
inline double Fdivs(double a, double b) {
  return double(float(a / b));
}
inline double Fmadds(double a, double b, double c) {
  return double(float(std::fma(a, b, c)));
}
inline double Fnmsubs(double a, double b, double c) {
  return double(float(-std::fma(a, b, -c)));
}
inline double Fabs(double a) {
  return std::bit_cast<double>(std::bit_cast<uint64_t>(a) & ~0x8000000000000000ull);
}
// fctidz as the translated code does it (NaN the smallest integer, too
// large the largest), then the low halfword the original's lhz reads back.
inline uint16_t FctidzLow16(double a) {
  const int64_t value = std::isnan(a)                ? INT64_MIN
                        : a > double(INT64_MAX)      ? INT64_MAX
                                                     : _mm_cvttsd_si64(_mm_set_sd(a));
  return uint16_t(value);
}
// lhz, extsw, std, lfd, fcfid, frsp: an unsigned halfword as a float.
inline double FromU16(uint32_t address) {
  return double(float(double(int64_t(Half(address).value()))));
}
inline uint32_t Bit(uint32_t word, uint32_t i) {
  return (word >> i) & 1;
}

}  // namespace

double CurveApply(uint32_t curve, double x) {
  ITC_NATIVE_RUNNING("sub_8210B5B8 ActionCurve::Apply");
  const Curve c(curve);
  const double maximum = double(c.maximum());
  const double k = double(G::minus_one());
  const double low = Fmuls(maximum, k);
  if (x < low) {
    return k;
  }
  if (x > maximum) {
    return double(G::one());
  }
  double center = double(c.center());
  const double dead = double(c.dead_zone());
  if (x < Fsubs(center, dead)) {
    center = Fsubs(center, dead);
    return Fmuls(Fdivs(Fsubs(x, center), Fsubs(low, center)), k);
  }
  if (x > Fadds(dead, center)) {
    const double edge = Fadds(double(c.center()), double(c.dead_zone()));
    return Fdivs(Fsubs(x, edge), Fsubs(maximum, edge));
  }
  return double(G::zero());
}

uint32_t RepeatUpdate(uint32_t repeat, uint32_t held) {
  ITC_NATIVE_RUNNING("sub_8210B568 ActionRepeat::Update");
  const Repeat r(repeat);
  if ((held & 0xFF) == 0) {
    r.set_count(0);
    return 0;
  }
  uint32_t fires = 0;
  const uint32_t delay = r.delay();
  const uint32_t count = r.count();
  if (count >= delay && count == r.period() + delay) {
    fires = 1;
    r.set_count(delay);
  }
  r.set_count(r.count() + 1);
  return fires;
}

uint32_t IndexOf(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210B140 ActionMapper::IndexOf");
  const uint32_t index = id - 151;
  return index <= 29 ? index : 30;
}

uint32_t ListenerHeld(uint32_t listener, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_825924E0 InputProcessor::AllHeld");
  return (schema::InputProcessor(listener).held() & mask) == mask ? 1 : 0;
}

uint32_t ListenerTestValue(uint32_t listener, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_8258CF10 InputProcessor::TestHeld");
  return ListenerHeld(listener, mask);
}

uint32_t ListenerTest(uint32_t listener, uint32_t mask_address) {
  return ListenerTestValue(listener, schema::ActionButtonMask(mask_address).mask());
}

void NoteListenerTest(uint32_t caller, uint32_t listener, uint32_t mask, uint32_t result) {
  if (!frame_pacing::GameMessages_LogInputReads()) {
    return;
  }
  frame_pacing::GameMessages_NoteInputRead(0x825924E0, 0x8258CF30, listener, mask, result);
  frame_pacing::GameMessages_NoteInputRead(0x8258CF10, caller, listener, mask, result);
}

void ClientNodeConstruct(uint32_t node, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_8259C6D0 ClientNode::ClientNode");
  const ClientNode n(node);
  n.set_type(type);
  n.set_next(0u);
  n.set_vtable(0x82071380);
}

namespace {

// The client list's count words: +12 + 4 x type the nodes added, +24 + 4 x
// type those bound. The list is in host memory (E5 step 4), its members at
// the guest offsets, so a word is the host struct's at that offset (host
// byte order). The original reads past the six words for a type above 2;
// nothing passes one.
constexpr uint32_t kAddedCounts = 12;
constexpr uint32_t kBoundCounts = 24;

uint8_t* CountWord(const ClientList& list, uint32_t offset, uint32_t type) {
  const uint64_t at = uint64_t(offset) + 4 * uint64_t(type);
  if (at + 4 > ClientList::kSize) {
    ITC_LOG_ERROR("ClientList: count word +{} outside the list (type {})", at, type);
    std::abort();
  }
  return reinterpret_cast<uint8_t*>(list.data()) + at;
}

uint32_t LoadCount(const ClientList& list, uint32_t offset, uint32_t type) {
  uint32_t v;
  std::memcpy(&v, CountWord(list, offset, type), sizeof v);
  return v;
}

void StoreCount(const ClientList& list, uint32_t offset, uint32_t type, uint32_t v) {
  std::memcpy(CountWord(list, offset, type), &v, sizeof v);
}

}  // namespace

void ClientListConstruct(uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8259C7B8 ClientList::ClientList");
  const ClientList l(list);
  l.set_first(0u);
  l.set_vtable(0x820713C0);
  l.set_last(0u);
  for (uint32_t i = 0; i < 3; i++) {
    StoreCount(l, kAddedCounts, i, 0);
    StoreCount(l, kBoundCounts, i, 0);
  }
  G::set_the_client_list(list);
}

void ClientListAdd(uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8259C960 ClientList::Add");
  const ClientList list = G::the_client_list();
  if (!list) {
    return;
  }
  if (!list.first()) {
    list.set_first(node);
  } else {
    list.last().set_next(node);
  }
  list.set_last(node);
  const uint32_t type = ClientNode(node).type();
  StoreCount(list, kAddedCounts, type, LoadCount(list, kAddedCounts, type) + 1);
}

namespace {

uint32_t Counter(const ClientList& list, uint32_t offset, uint32_t type, uint32_t bump) {
  if ((bump & 0xFF) != 0) {
    StoreCount(list, offset, type, LoadCount(list, offset, type) + 1);
  }
  return LoadCount(list, offset, type);
}

}  // namespace

uint32_t ClientListCount(uint32_t list, uint32_t type, uint32_t bump) {
  ITC_NATIVE_RUNNING("sub_8259C868 ClientList::Count");
  return Counter(ClientList(list), kAddedCounts, type, bump);
}

uint32_t ClientListBoundCount(uint32_t list, uint32_t type, uint32_t bump) {
  ITC_NATIVE_RUNNING("sub_8259C898 ClientList::BoundCount");
  return Counter(ClientList(list), kBoundCounts, type, bump);
}

uint32_t ClientListFind(uint32_t type, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8259C818 ClientList::Find");
  const ClientList list = G::the_client_list();
  if (!list) {
    return 0;
  }
  for (ClientNode node = list.first(); node; node = node.next()) {
    if (int32_t(type) == int32_t(node.type())) {
      if (n == 0) {
        return node.address;
      }
      n--;
    }
  }
  return 0;
}

// --- The update ---

bool RepeatsKnown(uint32_t mapper) {
  const Mapper m(mapper);
  for (uint32_t i = 0; i < kActions; i++) {
    const Repeat object(m.repeat_pointer(i).value());
    if (object.vtable().update() != kRepeatUpdateFunction) {
      return false;
    }
  }
  return true;
}

bool CurvesKnown(uint32_t mapper) {
  const Mapper m(mapper);
  for (uint32_t i = 0; i < kActions; i++) {
    const Curve object(m.curve_pointer(i).value());
    if (object.vtable().update() != kCurveApplyFunction) {
      return false;
    }
  }
  return true;
}

bool MapperCallsKnown(uint32_t mapper) {
  return mapper != 0 && RepeatsKnown(mapper) && CurvesKnown(mapper);
}

double CurveUnsigned(uint32_t analog, uint32_t index, uint32_t raw) {
  ITC_NATIVE_RUNNING("sub_8210B4A8 ActionMapper::CurveUnsigned");
  const uint32_t curve =
      Word(AnalogStage(analog).curve_pointers_address() + 4 * index).value();
  const double x =
      Fmuls(double(float(double(int64_t(raw & 0xFFFF)))), double(G::unsigned_scale()));
  return CurveApply(curve, x);
}

double CurveCentered(uint32_t analog, uint32_t index, uint32_t raw) {
  ITC_NATIVE_RUNNING("sub_8210B500 ActionMapper::CurveCentered");
  const uint32_t curve =
      Word(AnalogStage(analog).curve_pointers_address() + 4 * index).value();
  const double value = double(float(double(int64_t(raw & 0xFFFF))));
  const double x = Fmuls(Fsubs(value, double(G::centered_offset())),
                         double(G::centered_scale()));
  return CurveApply(curve, x);
}

uint32_t Analog(uint32_t analog, uint32_t config, uint32_t raw, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210B7D0 ActionMapper::Analog");
  const AnalogStage a(analog);
  const Config c(config);
  uint32_t r3 = analog;
  const double zero = double(G::zero());
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t link = Word(c.links() + 4 * i).value();
    uint32_t kind = Word(c.kinds() + 4 * i).value();
    const uint16_t value = Half(raw + 2 * i).value();
    if (link < i && Half(raw + 2 * link).value() != 0) {
      kind = 0;
    }
    if (kind == 0) {
      Half(a.raw_address() + 2 * i).set_value(0);
      Float(a.values_address() + 4 * i).set_value(float(zero));
    } else if (kind == 1 || kind == 2) {
      Half(a.raw_address() + 2 * i).set_value(value);
      const double result = kind == 1 ? CurveUnsigned(analog, i, value)
                                      : CurveCentered(analog, i, value);
      Float(a.values_address() + 4 * i).set_value(float(result));
      r3 = Word(a.curve_pointers_address() + 4 * i).value();
      *f1 = result;
    }
  }
  return r3;
}

uint32_t Hysteresis(uint32_t words, uint32_t index, double value,
                    uint32_t config) {
  ITC_NATIVE_RUNNING("sub_8210B718 ActionMapper::Hysteresis");
  const State s(words);
  double x = value;
  if (x < double(G::zero())) {
    x = Fmuls(x, double(G::minus_one()));
  }
  const Threshold entry(s.thresholds_address() + Threshold::kSize * index);
  const double on = double(entry.on());
  const double off = double(entry.off());
  if (on == off) {
    entry.set_state(off < x ? 1 : 0);
  } else if (!(on > x)) {
    entry.set_state(1);
  } else if (!(off < x)) {
    entry.set_state(0);
  }
  const uint32_t counter = entry.counter();
  if (counter != 0) {
    entry.set_state(0);
    entry.set_counter(counter - 1);
  }
  if (entry.state() != 0) {
    const uint32_t link = Word(Config(config).links() + 4 * index).value();
    if (link != 30) {
      const Threshold linked(s.thresholds_address() + Threshold::kSize * link);
      linked.set_counter(linked.suppress_ticks());
    }
  }
  return entry.state();
}

namespace {

uint32_t WordsFromState(uint32_t words, uint32_t config, uint32_t state_word,
                        uint32_t stack_word) {
  ITC_NATIVE_RUNNING("sub_8210BBA0 ActionMapper::Words");
  const State s(words);
  const Config c(config);
  uint32_t held = stack_word;
  for (uint32_t i = 0; i < kActions; i++) {
    bool set = false;
    if (Bit(state_word, i) && Word(c.kinds() + 4 * i).value() == 1) {
      const uint32_t link = Word(c.links() + 4 * i).value();
      // A linked action before this one already held suppresses it.
      set = !(link < i && int32_t(link) >= 0 && Bit(held, link & 31));
    }
    held = set ? held | (1u << i) : held & ~(1u << i);
  }
  const uint32_t before = s.held() | s.pending();
  s.set_held(before);
  s.set_held_before(before);
  s.set_held(held);
  const uint32_t changed = s.held_before() ^ held;
  s.set_pressed(s.held() & changed);
  s.set_pressed(s.pressed() | s.pending());
  s.set_released(changed & ~s.held());
  s.set_fired(s.pressed());
  s.set_pending(0);
  uint32_t result = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t repeat = Word(s.repeat_pointers_address() + 4 * i).value();
    result = RepeatUpdate(repeat, Bit(s.held(), i));
    if ((result & 0xFF) != 0) {
      s.set_fired(s.fired() | (1u << i));
    }
  }
  return result;
}

}  // namespace

uint32_t Words(uint32_t words, uint32_t config, uint32_t states,
               uint32_t stack_word) {
  return WordsFromState(words, config, guest::LoadU32(states), stack_word);
}

uint32_t Rebuild(uint32_t words, uint32_t config, uint32_t raw,
                 uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210C0E0 ActionMapper::Rebuild");
  const State s(words);
  // The analog stage's config follows the digital one's.
  Analog(s.analog_address(), config + Config::kSize, raw, f1);
  // The hysteresis states, a local word of the original's (bits 30 and 31
  // never read).
  uint32_t states = 0;
  double value = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    value = double(Float(s.analog_values_address() + 4 * i).value());
    if ((Hysteresis(words, i, value, config) & 0xFF) != 0) {
      states |= 1u << i;
    } else {
      states &= ~(1u << i);
    }
  }
  *f1 = value;
  return WordsFromState(words, config, states, stack_word);
}

uint32_t MapperUpdate(uint32_t mapper, uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210C370 ActionMapper::Update");
  const Mapper m(mapper);
  const uint32_t raw = m.raw_address();
  for (uint32_t i = 0; i < kActions; i++) {
    Half(raw + 2 * i).set_value(0);
  }
  const double zero = double(G::zero());
  const double scale = double(G::pad_scale());
  using Pad = schema::ActionPadValues;
  // A stick's direction: its magnitude scaled, into one raw value or the
  // other by its sign (NaN: neither).
  auto direction = [&](float (Pad::*value)() const, uint32_t positive, uint32_t negative) {
    const double v = double((m.pad_values().*value)());
    if (v > zero) {
      Half(positive).set_value(FctidzLow16(Fabs(Fmuls(v, scale))));
    } else if (v < zero) {
      Half(negative).set_value(FctidzLow16(Fabs(Fmuls(v, scale))));
    }
  };
  direction(&Pad::left_y, m.raw_left_y_plus_address(), m.raw_left_y_minus_address());
  direction(&Pad::left_x, m.raw_left_x_plus_address(), m.raw_left_x_minus_address());
  direction(&Pad::right_y, m.raw_right_y_plus_address(), m.raw_right_y_minus_address());
  direction(&Pad::right_x, m.raw_right_x_plus_address(), m.raw_right_x_minus_address());
  // The triggers.
  const Pad pad = m.pad_values();
  m.set_raw_left_trigger(FctidzLow16(Fabs(Fmuls(double(pad.left_trigger()), scale))));
  m.set_raw_right_trigger(
      FctidzLow16(Fabs(Fmuls(double(m.pad_values().right_trigger()), scale))));
  // The buttons: each mask held on the listener sets its action's raw value.
  static constexpr uint32_t kMasks[14] = {1,    2,     4,     8,      16,      32,      64,
                                          128,  4096,  65536, 16384,  262144,  1048576, 2097152};
  static constexpr uint32_t kIds[14] = {153, 154, 155, 156, 159, 158, 160,
                                        157, 161, 162, 165, 166, 152, 151};
  const uint32_t listener = m.listener_processor().address;
  for (int b = 0; b < 14; b++) {
    const uint32_t held = ListenerTestValue(listener, kMasks[b]);
    NoteListenerTest(0x8210C624, listener, kMasks[b], held);
    if ((held & 0xFF) != 0) {
      Half(raw + 2 * IndexOf(kIds[b])).set_value(32767);
    }
  }
  // The axes, from the directions' raw values.
  const double k1 = double(G::axis_scale());
  const double k2 = double(G::half());
  auto axis = [&](uint32_t minus, uint32_t plus) {
    const double from = Fnmsubs(FromU16(raw + 2 * minus), k1, k2);
    return Fmadds(FromU16(raw + 2 * plus), k1, from);
  };
  const double a25 = Fmuls(axis(2, 3), scale);
  const double a24 = Fmuls(axis(4, 5), scale);
  const double a27 = Fmuls(axis(16, 17), scale);
  const double a26 = axis(18, 19);
  m.set_raw_dpad_y(FctidzLow16(a25));
  m.set_raw_dpad_x(FctidzLow16(a24));
  m.set_raw_left_y(FctidzLow16(a27));
  m.set_raw_left_x(FctidzLow16(Fmuls(a26, scale)));
  const double a29 = Fmuls(axis(20, 21), scale);
  const double a28 = Fmuls(axis(22, 23), scale);
  m.set_raw_right_y(FctidzLow16(a29));
  m.set_raw_right_x(FctidzLow16(a28));
  return Rebuild(m.held_address(), m.config_address(), raw, stack_word, f1);
}

bool UpdateAllKnown(uint32_t list) {
  const uint32_t count = ClientList(list).bound_count_2();
  for (uint32_t i = 0; i < count; i++) {
    if (!MapperCallsKnown(ClientListFind(2, i))) {
      return false;
    }
  }
  return true;
}

uint32_t UpdateAll(uint32_t list, uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210B018 ActionMappers::Update");
  uint32_t count = ClientListBoundCount(list, 2, 0);
  for (uint32_t i = 0; i < count;) {
    const uint32_t mapper = ClientListFind(2, i);
    const uint32_t fired_before = debug::PauseMenuLog_BeforeMapperUpdate(mapper);
    MapperUpdate(mapper, stack_word, f1);
    debug::PauseMenuLog_AfterMapperUpdate(mapper, 0x8210B058, fired_before);
    i++;
    count = ClientListBoundCount(list, 2, 0);
  }
  return count;
}

namespace {

// Bit `IndexOf(id)` of the word at `word`, 0 for an id out of range.
uint32_t WordBit(uint32_t id, uint32_t word) {
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return 0;
  }
  return Bit(Word(word).value(), index);
}

}  // namespace

uint32_t Held(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BE70 ActionMapper::Held");
  return WordBit(id, Mapper(mapper).held_address());
}

uint32_t Pressed(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BF40 ActionMapper::Pressed");
  return WordBit(id, Mapper(mapper).pressed_address());
}

uint32_t Fired(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210C010 ActionMapper::Fired");
  return WordBit(id, Mapper(mapper).fired_address());
}

uint32_t HeldBefore(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BED8 ActionMapper::HeldBefore");
  return WordBit(id, Mapper(mapper).held_before_address());
}

uint32_t Released(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BFA8 ActionMapper::Released");
  return WordBit(id, Mapper(mapper).released_address());
}

uint32_t AnalogValue(uint32_t mapper, uint32_t id, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210BA68 ActionMapper::AnalogValue");
  const uint32_t index = IndexOf(id);
  // An id out of range reads the float at 0x82000A34.
  *f1 = double(index == 30 ? G::zero() : Mapper(mapper).analog_value(index).value());
  return index;
}

uint32_t AnalogRaw(uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BA28 ActionMapper::AnalogRaw");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return 0;
  }
  return Mapper(mapper).analog_raw_value(index).value();
}

uint32_t SetThreshold(uint32_t mapper, uint32_t id, double on, double off,
                      uint32_t ticks) {
  ITC_NATIVE_RUNNING("sub_8210B698 ActionMapper::SetThreshold");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const Threshold entry = Mapper(mapper).threshold(index);
  entry.set_on(float(on));
  entry.set_suppress_ticks(ticks);
  entry.set_off(float(off));
  entry.set_counter(0);
  entry.set_state(0);
  return index;
}

uint32_t SetRepeat(uint32_t mapper, uint32_t id, uint32_t delay,
                   uint32_t period) {
  ITC_NATIVE_RUNNING("sub_8210BDF0 ActionMapper::SetRepeat");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const Repeat repeat = Mapper(mapper).repeat(index);
  repeat.set_delay(delay);
  repeat.set_period(period);
  return index;
}

uint32_t SetCurve(uint32_t mapper, uint32_t id, double center, double dead,
                  double maximum) {
  ITC_NATIVE_RUNNING("sub_8210BE30 ActionMapper::SetCurve");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const Curve curve = Mapper(mapper).curve(index);
  curve.set_center(float(center));
  curve.set_dead_zone(float(dead));
  curve.set_maximum(float(maximum));
  return index;
}

namespace {

// The mapper table's entries (8 bytes: +0 the mapper, +4 a word the menus
// test).
uint32_t MapperEntry(uint32_t j) {
  return G::mappers_address() + 8 * j;
}

// The settings' entry the index picks.
schema::ActionSettings SettingsEntry() {
  return schema::ActionSettings(G::settings_address() + schema::ActionSettings::kSize * G::settings_index());
}

}  // namespace

bool ConfigureKnown() {
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t mapper = guest::LoadU32(MapperEntry(j));
    if (mapper == 0) {
      return false;
    }
    const uint32_t vtable = Mapper(mapper).vtable();
    if (guest::LoadU32(vtable + 8) != 0x8210B698 ||
        guest::LoadU32(vtable + 16) != 0x8210BDF0 ||
        guest::LoadU32(vtable + 28) != 0x8210BE30) {
      return false;
    }
  }
  return true;
}

uint32_t Configure(uint32_t index, double f[3]) {
  ITC_NATIVE_RUNNING("sub_8209F228 ActionMappers::Configure");
  G::set_settings_index(index);
  uint32_t r3 = 0;
  for (uint32_t j = 0; j < 4; j++) {
    for (uint32_t id = 151; id < 181; id++) {
      const uint32_t mapper = guest::LoadU32(MapperEntry(j));
      schema::ActionSettings entry = SettingsEntry();
      f[1] = double(entry.off());
      f[0] = double(entry.on());
      SetThreshold(mapper, id, f[0], f[1], entry.suppress_ticks());
      entry = SettingsEntry();
      SetRepeat(guest::LoadU32(MapperEntry(j)), id, entry.repeat_delay(),
                entry.repeat_period());
      entry = SettingsEntry();
      f[2] = double(entry.curve_maximum());
      f[1] = double(entry.curve_dead_zone());
      f[0] = double(entry.curve_center());
      r3 = SetCurve(guest::LoadU32(MapperEntry(j)), id, f[0], f[1], f[2]);
    }
  }
  return r3;
}

// --- The set-up ---

namespace {

// An object's slot 0 (the reset), through its vtable.
uint32_t Slot0(uint32_t object) {
  return Repeat(object).vtable().reset();
}

bool PointersKnown(uint32_t pointers, uint32_t function) {
  for (uint32_t i = 0; i < kActions; i++) {
    if (Slot0(Word(pointers + 4 * i).value()) != function) {
      return false;
    }
  }
  return true;
}

bool ObjectsKnown(uint32_t objects, uint32_t function) {
  for (uint32_t i = 0; i < kActions; i++) {
    if (Slot0(objects + 16 * i) != function) {
      return false;
    }
  }
  return true;
}

}  // namespace

bool RepeatResetsKnown(uint32_t pointers) {
  return PointersKnown(pointers, kRepeatResetFunction);
}
bool CurveResetsKnown(uint32_t pointers) {
  return PointersKnown(pointers, kCurveResetFunction);
}
bool RepeatObjectsResetKnown(uint32_t objects) {
  return ObjectsKnown(objects, kRepeatResetFunction);
}
bool CurveObjectsResetKnown(uint32_t objects) {
  return ObjectsKnown(objects, kCurveResetFunction);
}

uint32_t ResetWords(uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210B8A8 ActionMapper::ResetWords");
  const State s(words);
  uint32_t object = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    object = Word(s.repeat_pointers_address() + 4 * i).value();
    Repeat(object).set_count(0);  // sub_8243DC88
  }
  s.set_held(0);
  s.set_held_before(0);
  s.set_pressed(0);
  s.set_released(0);
  s.set_fired(0);
  s.set_pending(0);
  return object;
}

uint32_t ResetAnalog(uint32_t analog) {
  ITC_NATIVE_RUNNING("sub_8210B438 ActionMapper::ResetAnalog");
  const AnalogStage a(analog);
  // Each curve's slot 0 (sub_82106540) does nothing.
  const uint32_t object = Word(a.curve_pointers_address() + 4 * (kActions - 1)).value();
  const double zero = double(G::zero());
  for (uint32_t i = 0; i < kActions; i++) {
    Half(a.raw_address() + 2 * i).set_value(0);
    Float(a.values_address() + 4 * i).set_value(float(zero));
  }
  return object;
}

uint32_t InitRepeats(uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210BB38 ActionMapper::InitRepeats");
  const State s(words);
  for (uint32_t i = 0; i < kActions; i++) {
    const Repeat object(s.repeats_address() + Repeat::kSize * i);
    Word(s.repeat_pointers_address() + 4 * i).set_value(object.address);
    object.set_count(0);  // sub_8243DC88
    object.set_delay(20);
    object.set_period(10);
  }
  return ResetWords(words);
}

uint32_t InitCurves(uint32_t analog) {
  ITC_NATIVE_RUNNING("sub_8210BAB0 ActionMapper::InitCurves");
  const AnalogStage a(analog);
  const double one = double(G::one());
  const double zero = double(G::zero());
  for (uint32_t i = 0; i < kActions; i++) {
    const Curve object(a.curves_address() + Curve::kSize * i);
    Word(a.curve_pointers_address() + 4 * i).set_value(object.address);
    object.set_center(float(zero));
    object.set_dead_zone(float(zero));
    object.set_maximum(float(one));
  }
  return ResetAnalog(analog);
}

uint32_t ResetStages(uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210C078 ActionMapper::ResetStages");
  const State s(words);
  InitRepeats(words);
  const uint32_t r3 = InitCurves(s.analog_address());
  const double zero = double(G::zero());
  for (uint32_t i = 0; i < kActions; i++) {
    const Threshold entry(s.thresholds_address() + Threshold::kSize * i);
    entry.set_on(float(zero));
    entry.set_suppress_ticks(0);
    entry.set_off(float(zero));
    entry.set_counter(0);
    entry.set_state(0);
  }
  return r3;
}

void InputClientListConstruct(uint32_t list, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_8210AEB0 InputClientList::InputClientList");
  ClientListConstruct(list);
  const ClientList l(list);
  l.set_manager(manager);
  l.set_vtable(ClientList::kVtable);
}

void InputClientConstruct(uint32_t node, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_8210AE60 InputClient::InputClient");
  ClientNodeConstruct(node, type);
  ClientNode(node).set_vtable(ClientNode::kVtable);
}

void InputClientListAdd(uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8210AF00 InputClientList::Add");
  ClientListAdd(node);
}

namespace {

// The links of actions 2-23 (the byte table at 0x82007F40 picks the case):
// each direction's opposite, or 30 (none).
constexpr uint32_t kLinks[22] = {3,  2,  5,  4,  30, 30, 30, 30, 30, 30, 30,
                                 30, 30, 30, 17, 16, 19, 18, 21, 20, 23, 22};

}  // namespace

bool MapperConstructKnown() {
  return schema::ActionObjectVtable(Repeat::kVtable).reset() == kRepeatResetFunction &&
         schema::ActionObjectVtable(Curve::kVtable).reset() == kCurveResetFunction;
}

void MapperConstruct(uint32_t mapper, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_8210C180 ActionMapper::ActionMapper");
  const Mapper m(mapper);
  InputClientConstruct(mapper, 2);
  const uint32_t words = m.held_address();
  m.set_vtable(Mapper::kVtable);
  for (uint32_t i = 0; i < kActions; i++) {
    m.repeat(i).set_vtable(Repeat::kVtable);
  }
  for (uint32_t i = 0; i < kActions; i++) {
    m.curve(i).set_vtable(Curve::kVtable);
  }
  input::InputProcessorConstruct(m.listener_processor().address);
  m.set_player(player);
  m.set_bound(0);
  ResetStages(words);

  const double one = double(G::one());
  const double zero = double(G::zero());
  const double off = double(G::threshold_off());
  const double on = double(G::threshold_on());
  for (uint32_t i = 0; i < kActions; i++) {
    m.kind(i).set_value(1);
    m.analog_kind(i).set_value(i >= 24 ? 2 : 1);
    const uint32_t c = i - 2;  // the switch's case, unsigned
    m.link(i).set_value(c <= 21 ? kLinks[c] : 30);
    const Threshold entry = m.threshold(i);
    entry.set_on(float(on));
    entry.set_suppress_ticks(0);
    entry.set_off(float(off));
    entry.set_counter(0);
    entry.set_state(0);
    const Repeat repeat = m.repeat(i);
    repeat.set_delay(20);
    repeat.set_period(3);
    const Curve curve = m.curve(i);
    curve.set_center(float(zero));
    curve.set_dead_zone(float(zero));
    curve.set_maximum(float(one));
  }
  m.set_config_links(m.links_address());
  m.set_analog_config_links(m.links_address());
  m.set_config(m.kinds_address());
  m.set_analog_config(m.analog_kinds_address());
  ResetWords(words);
  ResetAnalog(m.analog_raw_address());
}

namespace {

constexpr uint32_t kGetLogicalPad = 0x82098CD0;  // the manager's slot 2
constexpr uint32_t kAddListener = 0x82099430;    // its slot 3
constexpr uint32_t kLogicalPadValues = 0x82591B30;  // the pad's slot 1

}  // namespace

bool MapperBindKnown(uint32_t mapper, uint32_t object) {
  if (mapper == 0) {
    return false;
  }
  const uint32_t player = Mapper(mapper).player();
  if (player > 3) {
    return true;
  }
  const schema::ActionMapperInputManager manager(object);
  if (object == 0 || manager.vtable().add_listener() != kAddListener ||
      manager.vtable().get_logical_pad() != kGetLogicalPad) {
    return false;
  }
  const schema::PtrList list = schema::InputManager(object).listener_list(player);
  const schema::LogicalPad pad(input::ManagerLogicalPad(object, player));
  return list.size() == 0 && std_list::CanAllocate(1) &&
         pad.vtable().values() == kLogicalPadValues;
}

uint32_t MapperBind(uint32_t mapper, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8210B270 ActionMapper::Bind");
  const Mapper m(mapper);
  const uint32_t player = m.player();
  m.set_input_manager(object);
  if (player <= 3) {
    if (!input::ManagerAddListener(object, player, m.listener_processor().address)) {
      ITC_LOG_ERROR("ActionMapper::Bind: AddListener refused after its checks");
      std::abort();
    }
    const uint32_t pad = input::ManagerLogicalPad(m.input_manager().address, player);
    m.set_pad_values(input::LogicalPadValues(pad));
  }
  m.set_bound(1);
  return 1;
}

bool InitCreate(uint32_t manager, uint32_t* list) {
  ITC_NATIVE_RUNNING("sub_8209F430 ActionMappers::Init");
  // operator new's error paths are the original's: every block made first
  // (in the original's order: the list, then mappers 0-3; the constructors
  // allocate nothing), then built and stored. The list and the mappers are
  // in host memory (E5 step 4), never freed (the process ends with them), so
  // operator new's error paths are gone.
  uint32_t blocks[5];
  blocks[0] = ClientList::New().address;
  for (int i = 1; i < 5; i++) {
    blocks[i] = Mapper::New().address;
  }
  G::set_owner(manager);
  InputClientListConstruct(blocks[0], G::owner().address);
  G::set_client_list(blocks[0]);
  for (uint32_t j = 0; j < 4; j++) {
    MapperConstruct(blocks[1 + j], j);
    guest::StoreU32(MapperEntry(j), blocks[1 + j]);
  }
  for (uint32_t j = 0; j < 4; j++) {
    InputClientListAdd(guest::LoadU32(MapperEntry(j)));
  }
  *list = G::client_list().address;
  return true;
}

}  // namespace action_mappers
