// The action mappers as plain C++: see action_mappers.h.

#include "action_mappers/action_mappers.h"

#include "base/log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

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

uint32_t ListenerTest(uint8_t* base, uint32_t listener, uint32_t mask_address) {
  ITC_NATIVE_RUNNING("sub_8258CF10 InputProcessor::TestHeld");
  return ListenerHeld(base, listener, guest::LoadU32(base, mask_address));
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

}  // namespace action_mappers
