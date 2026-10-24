// The action mappers as plain C++: see action_mappers.h.

#include "action_mappers/action_mappers.h"

#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"
#include "input/input_manager.h"
#include "std_list/ptr_list.h"

#include <immintrin.h>

#include <bit>
#include <cmath>
#include <cstdlib>
#include <cstdint>

namespace action_mappers {

namespace {

constexpr uint32_t kOne = 0x82000A30;       // the game's float 1.0
constexpr uint32_t kZero = 0x82000A34;      // the game's float 0.0
constexpr uint32_t kMinusOne = 0x82001BE0;  // probably -1.0

inline double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
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
inline double FromU16(uint8_t* base, uint32_t address) {
  return double(float(double(int64_t(guest::LoadU16(base, address)))));
}
inline uint32_t Bit(uint32_t word, uint32_t i) {
  return (word >> i) & 1;
}

}  // namespace

double CurveApply(uint8_t* base, uint32_t curve, double x) {
  ITC_NATIVE_RUNNING("sub_8210B5B8 ActionCurve::Apply");
  const double maximum = Lfs(base, curve + 12);
  const double k = Lfs(base, kMinusOne);
  const double low = Fmuls(maximum, k);
  if (x < low) {
    return k;
  }
  if (x > maximum) {
    return Lfs(base, kOne);
  }
  double center = Lfs(base, curve + 4);
  const double dead = Lfs(base, curve + 8);
  if (x < Fsubs(center, dead)) {
    center = Fsubs(center, dead);
    return Fmuls(Fdivs(Fsubs(x, center), Fsubs(low, center)), k);
  }
  if (x > Fadds(dead, center)) {
    const double edge = Fadds(Lfs(base, curve + 4), Lfs(base, curve + 8));
    return Fdivs(Fsubs(x, edge), Fsubs(maximum, edge));
  }
  return Lfs(base, kZero);
}

uint32_t RepeatUpdate(uint8_t* base, uint32_t repeat, uint32_t held) {
  ITC_NATIVE_RUNNING("sub_8210B568 ActionRepeat::Update");
  if ((held & 0xFF) == 0) {
    guest::StoreU32(base, repeat + 4, 0);
    return 0;
  }
  uint32_t fires = 0;
  const uint32_t delay = guest::LoadU32(base, repeat + 8);
  const uint32_t count = guest::LoadU32(base, repeat + 4);
  if (count >= delay && count == guest::LoadU32(base, repeat + 12) + delay) {
    fires = 1;
    guest::StoreU32(base, repeat + 4, delay);
  }
  guest::StoreU32(base, repeat + 4, guest::LoadU32(base, repeat + 4) + 1);
  return fires;
}

uint32_t IndexOf(uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210B140 ActionMapper::IndexOf");
  const uint32_t index = id - 151;
  return index <= 29 ? index : 30;
}

uint32_t ListenerHeld(uint8_t* base, uint32_t listener, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_825924E0 InputProcessor::AllHeld");
  return (guest::LoadU32(base, listener + 272) & mask) == mask ? 1 : 0;
}

uint32_t ListenerTestValue(uint8_t* base, uint32_t listener, uint32_t mask) {
  ITC_NATIVE_RUNNING("sub_8258CF10 InputProcessor::TestHeld");
  return ListenerHeld(base, listener, mask);
}

uint32_t ListenerTest(uint8_t* base, uint32_t listener, uint32_t mask_address) {
  return ListenerTestValue(base, listener, guest::LoadU32(base, mask_address));
}

void NoteListenerTest(uint32_t caller, uint32_t listener, uint32_t mask, uint32_t result) {
  if (!frame_pacing::GameMessages_LogInputReads()) {
    return;
  }
  frame_pacing::GameMessages_NoteInputRead(0x825924E0, 0x8258CF30, listener, mask, result);
  frame_pacing::GameMessages_NoteInputRead(0x8258CF10, caller, listener, mask, result);
}

void ClientNodeConstruct(uint8_t* base, uint32_t node, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_8259C6D0 ClientNode::ClientNode");
  guest::StoreU32(base, node + 4, type);
  guest::StoreU32(base, node + 8, 0);
  guest::StoreU32(base, node + 0, 0x82071380);
}

void ClientListConstruct(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_8259C7B8 ClientList::ClientList");
  guest::StoreU32(base, list + 4, 0);
  guest::StoreU32(base, list + 0, 0x820713C0);
  guest::StoreU32(base, list + 8, 0);
  for (uint32_t i = 0; i < 3; i++) {
    guest::StoreU32(base, list + 12 + 4 * i, 0);
    guest::StoreU32(base, list + 24 + 4 * i, 0);
  }
  guest::StoreU32(base, kClientList, list);
}

void ClientListAdd(uint8_t* base, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8259C960 ClientList::Add");
  const uint32_t list = guest::LoadU32(base, kClientList);
  if (list == 0) {
    return;
  }
  if (guest::LoadU32(base, list + 4) == 0) {
    guest::StoreU32(base, list + 4, node);
  } else {
    guest::StoreU32(base, guest::LoadU32(base, list + 8) + 8, node);
  }
  guest::StoreU32(base, list + 8, node);
  const uint32_t count = list + 4 * (guest::LoadU32(base, node + 4) + 3);
  guest::StoreU32(base, count, guest::LoadU32(base, count) + 1);
}

namespace {

uint32_t Counter(uint8_t* base, uint32_t address, uint32_t bump) {
  if ((bump & 0xFF) != 0) {
    guest::StoreU32(base, address, guest::LoadU32(base, address) + 1);
  }
  return guest::LoadU32(base, address);
}

}  // namespace

uint32_t ClientListCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump) {
  ITC_NATIVE_RUNNING("sub_8259C868 ClientList::Count");
  return Counter(base, list + 4 * (type + 3), bump);
}

uint32_t ClientListBoundCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump) {
  ITC_NATIVE_RUNNING("sub_8259C898 ClientList::BoundCount");
  return Counter(base, list + 4 * (type + 6), bump);
}

uint32_t ClientListFind(uint8_t* base, uint32_t type, uint32_t n) {
  ITC_NATIVE_RUNNING("sub_8259C818 ClientList::Find");
  const uint32_t list = guest::LoadU32(base, kClientList);
  if (list == 0) {
    return 0;
  }
  for (uint32_t node = guest::LoadU32(base, list + 4); node != 0;
       node = guest::LoadU32(base, node + 8)) {
    if (int32_t(type) == int32_t(guest::LoadU32(base, node + 4))) {
      if (n == 0) {
        return node;
      }
      n--;
    }
  }
  return 0;
}

// --- The update ---

bool RepeatsKnown(uint8_t* base, uint32_t mapper) {
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t object = guest::LoadU32(base, mapper + kRepeatPointers + 4 * i);
    if (guest::LoadU32(base, guest::LoadU32(base, object) + 4) != kRepeatUpdateFunction) {
      return false;
    }
  }
  return true;
}

bool CurvesKnown(uint8_t* base, uint32_t mapper) {
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t object = guest::LoadU32(base, mapper + kCurvePointers + 4 * i);
    if (guest::LoadU32(base, guest::LoadU32(base, object) + 4) != kCurveApplyFunction) {
      return false;
    }
  }
  return true;
}

bool MapperCallsKnown(uint8_t* base, uint32_t mapper) {
  return mapper != 0 && RepeatsKnown(base, mapper) && CurvesKnown(base, mapper);
}

double CurveUnsigned(uint8_t* base, uint32_t analog, uint32_t index, uint32_t raw) {
  ITC_NATIVE_RUNNING("sub_8210B4A8 ActionMapper::CurveUnsigned");
  const uint32_t curve = guest::LoadU32(base, analog + 4 * (index + 45));
  const double x = Fmuls(double(float(double(int64_t(raw & 0xFFFF)))), Lfs(base, 0x82002BD4));
  return CurveApply(base, curve, x);
}

double CurveCentered(uint8_t* base, uint32_t analog, uint32_t index, uint32_t raw) {
  ITC_NATIVE_RUNNING("sub_8210B500 ActionMapper::CurveCentered");
  const uint32_t curve = guest::LoadU32(base, analog + 4 * (index + 45));
  const double value = double(float(double(int64_t(raw & 0xFFFF))));
  const double x = Fmuls(Fsubs(value, Lfs(base, 0x82007F64)), Lfs(base, 0x82007F60));
  return CurveApply(base, curve, x);
}

uint32_t Analog(uint8_t* base, uint32_t analog, uint32_t config, uint32_t raw, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210B7D0 ActionMapper::Analog");
  uint32_t r3 = analog;
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t link = guest::LoadU32(base, guest::LoadU32(base, config + 4) + 4 * i);
    uint32_t kind = guest::LoadU32(base, guest::LoadU32(base, config) + 4 * i);
    const uint16_t value = guest::LoadU16(base, raw + 2 * i);
    if (link < i && guest::LoadU16(base, raw + 2 * link) != 0) {
      kind = 0;
    }
    if (kind == 0) {
      guest::StoreU16(base, analog + 2 * i, 0);
      guest::StoreFloat(base, analog + 60 + 4 * i, float(zero));
    } else if (kind == 1 || kind == 2) {
      guest::StoreU16(base, analog + 2 * i, value);
      const double result = kind == 1 ? CurveUnsigned(base, analog, i, value)
                                      : CurveCentered(base, analog, i, value);
      guest::StoreFloat(base, analog + 60 + 4 * i, float(result));
      r3 = guest::LoadU32(base, analog + 4 * (i + 45));
      *f1 = result;
    }
  }
  return r3;
}

uint32_t Hysteresis(uint8_t* base, uint32_t words, uint32_t index, double value,
                    uint32_t config) {
  ITC_NATIVE_RUNNING("sub_8210B718 ActionMapper::Hysteresis");
  double x = value;
  if (x < Lfs(base, kZero)) {
    x = Fmuls(x, Lfs(base, kMinusOne));
  }
  const uint32_t entry = words + 1404 + 20 * index;
  const double on = Lfs(base, entry);
  const double off = Lfs(base, entry + 4);
  if (on == off) {
    guest::StoreU8(base, entry + 16, off < x ? 1 : 0);
  } else if (!(on > x)) {
    guest::StoreU8(base, entry + 16, 1);
  } else if (!(off < x)) {
    guest::StoreU8(base, entry + 16, 0);
  }
  const uint32_t counter = guest::LoadU32(base, entry + 12);
  if (counter != 0) {
    guest::StoreU8(base, entry + 16, 0);
    guest::StoreU32(base, entry + 12, counter - 1);
  }
  if (guest::LoadU8(base, entry + 16) != 0) {
    const uint32_t link = guest::LoadU32(base, guest::LoadU32(base, config + 4) + 4 * index);
    if (link != 30) {
      const uint32_t linked = words + 20 * link;
      guest::StoreU32(base, linked + 1416, guest::LoadU32(base, linked + 1412));
    }
  }
  return guest::LoadU8(base, entry + 16);
}

namespace {

uint32_t WordsFromState(uint8_t* base, uint32_t words, uint32_t config, uint32_t state_word,
                        uint32_t stack_word) {
  ITC_NATIVE_RUNNING("sub_8210BBA0 ActionMapper::Words");
  uint32_t held = stack_word;
  for (uint32_t i = 0; i < kActions; i++) {
    bool set = false;
    if (Bit(state_word, i) &&
        guest::LoadU32(base, guest::LoadU32(base, config) + 4 * i) == 1) {
      const uint32_t link = guest::LoadU32(base, guest::LoadU32(base, config + 4) + 4 * i);
      // A linked action before this one already held suppresses it.
      set = !(link < i && int32_t(link) >= 0 && Bit(held, link & 31));
    }
    held = set ? held | (1u << i) : held & ~(1u << i);
  }
  const uint32_t before = guest::LoadU32(base, words) | guest::LoadU32(base, words + 20);
  guest::StoreU32(base, words, before);
  guest::StoreU32(base, words + 4, before);
  guest::StoreU32(base, words, held);
  const uint32_t changed = guest::LoadU32(base, words + 4) ^ held;
  guest::StoreU32(base, words + 8, guest::LoadU32(base, words) & changed);
  guest::StoreU32(base, words + 8, guest::LoadU32(base, words + 8) | guest::LoadU32(base, words + 20));
  guest::StoreU32(base, words + 12, changed & ~guest::LoadU32(base, words));
  guest::StoreU32(base, words + 16, guest::LoadU32(base, words + 8));
  guest::StoreU32(base, words + 20, 0);
  uint32_t result = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t repeat = guest::LoadU32(base, words + 24 + 4 * i);
    result = RepeatUpdate(base, repeat, Bit(guest::LoadU32(base, words), i));
    if ((result & 0xFF) != 0) {
      guest::StoreU32(base, words + 16, guest::LoadU32(base, words + 16) | (1u << i));
    }
  }
  return result;
}

}  // namespace

uint32_t Words(uint8_t* base, uint32_t words, uint32_t config, uint32_t states,
               uint32_t stack_word) {
  return WordsFromState(base, words, config, guest::LoadU32(base, states), stack_word);
}

uint32_t Rebuild(uint8_t* base, uint32_t words, uint32_t config, uint32_t raw,
                 uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210C0E0 ActionMapper::Rebuild");
  Analog(base, words + 624, config + 8, raw, f1);
  // The hysteresis states, a local word of the original's (bits 30 and 31
  // never read).
  uint32_t states = 0;
  double value = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    value = Lfs(base, words + 684 + 4 * i);
    if ((Hysteresis(base, words, i, value, config) & 0xFF) != 0) {
      states |= 1u << i;
    } else {
      states &= ~(1u << i);
    }
  }
  *f1 = value;
  return WordsFromState(base, words, config, states, stack_word);
}

uint32_t MapperUpdate(uint8_t* base, uint32_t mapper, uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210C370 ActionMapper::Update");
  const uint32_t raw = mapper + kRaw;
  for (uint32_t i = 0; i < kActions; i++) {
    guest::StoreU16(base, raw + 2 * i, 0);
  }
  const double zero = Lfs(base, kZero);
  const double scale = Lfs(base, 0x82007F58);
  // A stick's direction: its magnitude scaled, into one raw value or the
  // other by its sign (NaN: neither).
  auto direction = [&](uint32_t value, uint32_t positive, uint32_t negative) {
    const double v = Lfs(base, guest::LoadU32(base, mapper + kPadValues) + value);
    if (v > zero) {
      guest::StoreU16(base, mapper + positive, FctidzLow16(Fabs(Fmuls(v, scale))));
    } else if (v < zero) {
      guest::StoreU16(base, mapper + negative, FctidzLow16(Fabs(Fmuls(v, scale))));
    }
  };
  direction(4, 420, 422);
  direction(0, 426, 424);
  direction(20, 428, 430);
  direction(16, 434, 432);
  // The triggers.
  const uint32_t pad = guest::LoadU32(base, mapper + kPadValues);
  guest::StoreU16(base, mapper + 412, FctidzLow16(Fabs(Fmuls(Lfs(base, pad + 8), scale))));
  guest::StoreU16(base, mapper + 414,
                  FctidzLow16(Fabs(Fmuls(Lfs(base, guest::LoadU32(base, mapper + kPadValues) + 24),
                                         scale))));
  // The buttons: each mask held on the listener sets its action's raw value.
  static constexpr uint32_t kMasks[14] = {1,    2,     4,     8,      16,      32,      64,
                                          128,  4096,  65536, 16384,  262144,  1048576, 2097152};
  static constexpr uint32_t kIds[14] = {153, 154, 155, 156, 159, 158, 160,
                                        157, 161, 162, 165, 166, 152, 151};
  const uint32_t listener = mapper + kListener;
  for (int b = 0; b < 14; b++) {
    const uint32_t held = ListenerTestValue(base, listener, kMasks[b]);
    NoteListenerTest(0x8210C624, listener, kMasks[b], held);
    if ((held & 0xFF) != 0) {
      guest::StoreU16(base, mapper + 2 * (IndexOf(kIds[b]) + 194), 32767);
    }
  }
  // The axes, from the directions' raw values.
  const double k1 = Lfs(base, 0x82007F5C);
  const double k2 = Lfs(base, 0x82001BE4);
  auto axis = [&](uint32_t minus, uint32_t plus) {
    const double from = Fnmsubs(FromU16(base, raw + 2 * minus), k1, k2);
    return Fmadds(FromU16(base, raw + 2 * plus), k1, from);
  };
  const double a25 = Fmuls(axis(2, 3), scale);
  const double a24 = Fmuls(axis(4, 5), scale);
  const double a27 = Fmuls(axis(16, 17), scale);
  const double a26 = axis(18, 19);
  guest::StoreU16(base, mapper + 438, FctidzLow16(a25));
  guest::StoreU16(base, mapper + 436, FctidzLow16(a24));
  guest::StoreU16(base, mapper + 442, FctidzLow16(a27));
  guest::StoreU16(base, mapper + 440, FctidzLow16(Fmuls(a26, scale)));
  const double a29 = Fmuls(axis(20, 21), scale);
  const double a28 = Fmuls(axis(22, 23), scale);
  guest::StoreU16(base, mapper + 446, FctidzLow16(a29));
  guest::StoreU16(base, mapper + 444, FctidzLow16(a28));
  return Rebuild(base, mapper + kWords, mapper + kConfig, raw, stack_word, f1);
}

bool UpdateAllKnown(uint8_t* base, uint32_t list) {
  const uint32_t count = guest::LoadU32(base, list + 4 * (2 + 6));
  for (uint32_t i = 0; i < count; i++) {
    if (!MapperCallsKnown(base, ClientListFind(base, 2, i))) {
      return false;
    }
  }
  return true;
}

uint32_t UpdateAll(uint8_t* base, uint32_t list, uint32_t stack_word, double* f1) {
  ITC_NATIVE_RUNNING("sub_8210B018 ActionMappers::Update");
  uint32_t count = ClientListBoundCount(base, list, 2, 0);
  for (uint32_t i = 0; i < count;) {
    const uint32_t mapper = ClientListFind(base, 2, i);
    const uint32_t fired_before = debug::PauseMenuLog_BeforeMapperUpdate(base, mapper);
    MapperUpdate(base, mapper, stack_word, f1);
    debug::PauseMenuLog_AfterMapperUpdate(base, mapper, 0x8210B058, fired_before);
    i++;
    count = ClientListBoundCount(base, list, 2, 0);
  }
  return count;
}

namespace {

uint32_t WordBit(uint8_t* base, uint32_t mapper, uint32_t id, uint32_t word) {
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return 0;
  }
  return Bit(guest::LoadU32(base, mapper + word), index);
}

}  // namespace

uint32_t Held(uint8_t* base, uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BE70 ActionMapper::Held");
  return WordBit(base, mapper, id, kWords);
}

uint32_t Pressed(uint8_t* base, uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BF40 ActionMapper::Pressed");
  return WordBit(base, mapper, id, kWords + 8);
}

uint32_t Fired(uint8_t* base, uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210C010 ActionMapper::Fired");
  return WordBit(base, mapper, id, kWords + 16);
}

uint32_t AnalogRaw(uint8_t* base, uint32_t mapper, uint32_t id) {
  ITC_NATIVE_RUNNING("sub_8210BA28 ActionMapper::AnalogRaw");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return 0;
  }
  return guest::LoadU16(base, mapper + kAnalog + 2 * index);
}

uint32_t SetThreshold(uint8_t* base, uint32_t mapper, uint32_t id, double on, double off,
                      uint32_t ticks) {
  ITC_NATIVE_RUNNING("sub_8210B698 ActionMapper::SetThreshold");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const uint32_t entry = mapper + kThresholds + 20 * index;
  guest::StoreFloat(base, entry, float(on));
  guest::StoreU32(base, entry + 8, ticks);
  guest::StoreFloat(base, entry + 4, float(off));
  guest::StoreU32(base, entry + 12, 0);
  guest::StoreU8(base, entry + 16, 0);
  return index;
}

uint32_t SetRepeat(uint8_t* base, uint32_t mapper, uint32_t id, uint32_t delay,
                   uint32_t period) {
  ITC_NATIVE_RUNNING("sub_8210BDF0 ActionMapper::SetRepeat");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const uint32_t repeat = mapper + 592 + 16 * index;
  guest::StoreU32(base, repeat + 8, delay);
  guest::StoreU32(base, repeat + 12, period);
  return index;
}

uint32_t SetCurve(uint8_t* base, uint32_t mapper, uint32_t id, double center, double dead,
                  double maximum) {
  ITC_NATIVE_RUNNING("sub_8210BE30 ActionMapper::SetCurve");
  const uint32_t index = IndexOf(id);
  if (index == 30) {
    return index;
  }
  const uint32_t curve = mapper + 1372 + 16 * index;
  guest::StoreFloat(base, curve + 4, float(center));
  guest::StoreFloat(base, curve + 8, float(dead));
  guest::StoreFloat(base, curve + 12, float(maximum));
  return index;
}

namespace {

constexpr uint32_t kMappers = 0x826BAB80;
constexpr uint32_t kSettingsIndex = 0x82600B84;
constexpr uint32_t kSettings = 0x82600B88;

}  // namespace

bool ConfigureKnown(uint8_t* base) {
  for (uint32_t j = 0; j < 4; j++) {
    const uint32_t mapper = guest::LoadU32(base, kMappers + 8 * j);
    if (mapper == 0) {
      return false;
    }
    const uint32_t vtable = guest::LoadU32(base, mapper);
    if (guest::LoadU32(base, vtable + 8) != 0x8210B698 ||
        guest::LoadU32(base, vtable + 16) != 0x8210BDF0 ||
        guest::LoadU32(base, vtable + 28) != 0x8210BE30) {
      return false;
    }
  }
  return true;
}

uint32_t Configure(uint8_t* base, uint32_t index, double f[3]) {
  ITC_NATIVE_RUNNING("sub_8209F228 ActionMappers::Configure");
  guest::StoreU32(base, kSettingsIndex, index);
  uint32_t r3 = 0;
  for (uint32_t j = 0; j < 4; j++) {
    for (uint32_t id = 151; id < 181; id++) {
      const uint32_t mapper = guest::LoadU32(base, kMappers + 8 * j);
      uint32_t entry = kSettings + 32 * guest::LoadU32(base, kSettingsIndex);
      f[1] = Lfs(base, entry + 4);
      f[0] = Lfs(base, entry);
      SetThreshold(base, mapper, id, f[0], f[1], guest::LoadU32(base, entry + 8));
      entry = kSettings + 32 * guest::LoadU32(base, kSettingsIndex);
      SetRepeat(base, guest::LoadU32(base, kMappers + 8 * j), id, guest::LoadU32(base, entry + 12),
                guest::LoadU32(base, entry + 16));
      entry = kSettings + 32 * guest::LoadU32(base, kSettingsIndex);
      f[2] = Lfs(base, entry + 28);
      f[1] = Lfs(base, entry + 24);
      f[0] = Lfs(base, entry + 20);
      r3 = SetCurve(base, guest::LoadU32(base, kMappers + 8 * j), id, f[0], f[1], f[2]);
    }
  }
  return r3;
}

// --- The set-up ---

namespace {

uint32_t Slot0(uint8_t* base, uint32_t object) {
  return guest::LoadU32(base, guest::LoadU32(base, object));
}

bool PointersKnown(uint8_t* base, uint32_t pointers, uint32_t function) {
  for (uint32_t i = 0; i < kActions; i++) {
    if (Slot0(base, guest::LoadU32(base, pointers + 4 * i)) != function) {
      return false;
    }
  }
  return true;
}

bool ObjectsKnown(uint8_t* base, uint32_t objects, uint32_t function) {
  for (uint32_t i = 0; i < kActions; i++) {
    if (Slot0(base, objects + 16 * i) != function) {
      return false;
    }
  }
  return true;
}

}  // namespace

bool RepeatResetsKnown(uint8_t* base, uint32_t pointers) {
  return PointersKnown(base, pointers, kRepeatResetFunction);
}
bool CurveResetsKnown(uint8_t* base, uint32_t pointers) {
  return PointersKnown(base, pointers, kCurveResetFunction);
}
bool RepeatObjectsResetKnown(uint8_t* base, uint32_t objects) {
  return ObjectsKnown(base, objects, kRepeatResetFunction);
}
bool CurveObjectsResetKnown(uint8_t* base, uint32_t objects) {
  return ObjectsKnown(base, objects, kCurveResetFunction);
}

uint32_t ResetWords(uint8_t* base, uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210B8A8 ActionMapper::ResetWords");
  uint32_t object = 0;
  for (uint32_t i = 0; i < kActions; i++) {
    object = guest::LoadU32(base, words + 24 + 4 * i);
    guest::StoreU32(base, object + 4, 0);  // sub_8243DC88
  }
  for (uint32_t k = 0; k < 6; k++) {
    guest::StoreU32(base, words + 4 * k, 0);
  }
  return object;
}

uint32_t ResetAnalog(uint8_t* base, uint32_t analog) {
  ITC_NATIVE_RUNNING("sub_8210B438 ActionMapper::ResetAnalog");
  // Each curve's slot 0 (sub_82106540) does nothing.
  const uint32_t object = guest::LoadU32(base, analog + 180 + 4 * (kActions - 1));
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < kActions; i++) {
    guest::StoreU16(base, analog + 2 * i, 0);
    guest::StoreFloat(base, analog + 60 + 4 * i, float(zero));
  }
  return object;
}

uint32_t InitRepeats(uint8_t* base, uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210BB38 ActionMapper::InitRepeats");
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t object = words + 144 + 16 * i;
    guest::StoreU32(base, words + 24 + 4 * i, object);
    guest::StoreU32(base, object + 4, 0);  // sub_8243DC88
    guest::StoreU32(base, object + 8, 20);
    guest::StoreU32(base, object + 12, 10);
  }
  return ResetWords(base, words);
}

uint32_t InitCurves(uint8_t* base, uint32_t analog) {
  ITC_NATIVE_RUNNING("sub_8210BAB0 ActionMapper::InitCurves");
  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t object = analog + 300 + 16 * i;
    guest::StoreU32(base, analog + 180 + 4 * i, object);
    guest::StoreFloat(base, object + 4, float(zero));
    guest::StoreFloat(base, object + 8, float(zero));
    guest::StoreFloat(base, object + 12, float(one));
  }
  return ResetAnalog(base, analog);
}

uint32_t ResetStages(uint8_t* base, uint32_t words) {
  ITC_NATIVE_RUNNING("sub_8210C078 ActionMapper::ResetStages");
  InitRepeats(base, words);
  const uint32_t r3 = InitCurves(base, words + 624);
  const double zero = Lfs(base, kZero);
  for (uint32_t i = 0; i < kActions; i++) {
    const uint32_t entry = words + 1404 + 20 * i;
    guest::StoreFloat(base, entry, float(zero));
    guest::StoreU32(base, entry + 8, 0);
    guest::StoreFloat(base, entry + 4, float(zero));
    guest::StoreU32(base, entry + 12, 0);
    guest::StoreU8(base, entry + 16, 0);
  }
  return r3;
}

void InputClientListConstruct(uint8_t* base, uint32_t list, uint32_t manager) {
  ITC_NATIVE_RUNNING("sub_8210AEB0 InputClientList::InputClientList");
  ClientListConstruct(base, list);
  guest::StoreU32(base, list + 36, manager);
  guest::StoreU32(base, list, 0x82007F18);
}

void InputClientConstruct(uint8_t* base, uint32_t node, uint32_t type) {
  ITC_NATIVE_RUNNING("sub_8210AE60 InputClient::InputClient");
  ClientNodeConstruct(base, node, type);
  guest::StoreU32(base, node, 0x82007ED8);
}

void InputClientListAdd(uint8_t* base, uint32_t node) {
  ITC_NATIVE_RUNNING("sub_8210AF00 InputClientList::Add");
  ClientListAdd(base, node);
}

namespace {

constexpr uint32_t kMapperVtable = 0x82007F78;
constexpr uint32_t kRepeatVtable = 0x82007F68;
constexpr uint32_t kCurveVtable = 0x82007F70;
constexpr uint32_t kThresholdOn = 0x82002A10;
constexpr uint32_t kThresholdOff = 0x8200296C;
// The links of actions 2-23 (the byte table at 0x82007F40 picks the case):
// each direction's opposite, or 30 (none).
constexpr uint32_t kLinks[22] = {3,  2,  5,  4,  30, 30, 30, 30, 30, 30, 30,
                                 30, 30, 30, 17, 16, 19, 18, 21, 20, 23, 22};

}  // namespace

bool MapperConstructKnown(uint8_t* base) {
  return guest::LoadU32(base, kRepeatVtable) == kRepeatResetFunction &&
         guest::LoadU32(base, kCurveVtable) == kCurveResetFunction;
}

void MapperConstruct(uint8_t* base, uint32_t mapper, uint32_t player) {
  ITC_NATIVE_RUNNING("sub_8210C180 ActionMapper::ActionMapper");
  InputClientConstruct(base, mapper, 2);
  const uint32_t words = mapper + kWords;
  guest::StoreU32(base, mapper, kMapperVtable);
  for (uint32_t i = 0; i < kActions; i++) {
    guest::StoreU32(base, words + 144 + 16 * i, kRepeatVtable);
  }
  for (uint32_t i = 0; i < kActions; i++) {
    guest::StoreU32(base, words + 924 + 16 * i, kCurveVtable);
  }
  input::InputProcessorConstruct(base, mapper + kListener);
  guest::StoreU32(base, mapper + 2888, player);
  guest::StoreU8(base, mapper + 2884, 0);
  ResetStages(base, words);

  const double one = Lfs(base, kOne);
  const double zero = Lfs(base, kZero);
  const double off = Lfs(base, kThresholdOff);
  const double on = Lfs(base, kThresholdOn);
  for (uint32_t i = 0; i < kActions; i++) {
    guest::StoreU32(base, mapper + 12 + 4 * i, 1);
    guest::StoreU32(base, mapper + 132 + 4 * i, i >= 24 ? 2 : 1);
    const uint32_t c = i - 2;  // the switch's case, unsigned
    guest::StoreU32(base, mapper + 252 + 4 * i, c <= 21 ? kLinks[c] : 30);
    const uint32_t entry = mapper + kThresholds + 20 * i;
    guest::StoreFloat(base, entry, float(on));
    guest::StoreU32(base, entry + 8, 0);
    guest::StoreFloat(base, entry + 4, float(off));
    guest::StoreU32(base, entry + 12, 0);
    guest::StoreU8(base, entry + 16, 0);
    const uint32_t repeat = mapper + 592 + 16 * i;
    guest::StoreU32(base, repeat + 8, 20);
    guest::StoreU32(base, repeat + 12, 3);
    const uint32_t curve = mapper + 1372 + 16 * i;
    guest::StoreFloat(base, curve + 4, float(zero));
    guest::StoreFloat(base, curve + 8, float(zero));
    guest::StoreFloat(base, curve + 12, float(one));
  }
  guest::StoreU32(base, mapper + 376, mapper + 252);
  guest::StoreU32(base, mapper + 384, mapper + 252);
  guest::StoreU32(base, mapper + 372, mapper + 12);
  guest::StoreU32(base, mapper + 380, mapper + 132);
  ResetWords(base, words);
  ResetAnalog(base, mapper + kAnalog);
}

namespace {

constexpr uint32_t kGetLogicalPad = 0x82098CD0;  // the manager's slot 2
constexpr uint32_t kAddListener = 0x82099430;    // its slot 3
constexpr uint32_t kLogicalPadValues = 0x82591B30;  // the pad's slot 1

uint32_t VtableSlot(uint8_t* base, uint32_t object, uint32_t offset) {
  return guest::LoadU32(base, guest::LoadU32(base, object) + offset);
}

}  // namespace

bool MapperBindKnown(uint8_t* base, uint32_t mapper, uint32_t object) {
  if (mapper == 0) {
    return false;
  }
  const uint32_t player = guest::LoadU32(base, mapper + 2888);
  if (player > 3) {
    return true;
  }
  if (object == 0 || VtableSlot(base, object, 12) != kAddListener ||
      VtableSlot(base, object, 8) != kGetLogicalPad) {
    return false;
  }
  const uint32_t list =
      object + input::kManagerListeners + input::kListenerListBytes * player;
  const uint32_t pad = input::ManagerLogicalPad(object, player);
  return guest::LoadU32(base, list + std_list::kSize) == 0 &&
         std_list::CanAllocate(base, 1) && VtableSlot(base, pad, 4) == kLogicalPadValues;
}

uint32_t MapperBind(uint8_t* base, uint32_t mapper, uint32_t object) {
  ITC_NATIVE_RUNNING("sub_8210B270 ActionMapper::Bind");
  const uint32_t player = guest::LoadU32(base, mapper + 2888);
  guest::StoreU32(base, mapper + 2892, object);
  if (player <= 3) {
    if (!input::ManagerAddListener(base, object, player, mapper + kListener)) {
      ITC_LOG_ERROR("ActionMapper::Bind: AddListener refused after its checks");
      std::abort();
    }
    const uint32_t pad = input::ManagerLogicalPad(guest::LoadU32(base, mapper + 2892), player);
    guest::StoreU32(base, mapper + kPadValues, input::LogicalPadValues(pad));
  }
  guest::StoreU8(base, mapper + 2884, 1);
  return 1;
}

}  // namespace action_mappers
