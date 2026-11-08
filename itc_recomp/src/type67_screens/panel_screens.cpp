// The panel screens' handlers (docs/functions/type67-screens.md): see
// type67_screens.h. The car list (+9644) and the group pair (+9648) are
// called through their vtables (Slot).

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "guest.h"

namespace type67_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kCarList = 9644, kGroupPair = 9648;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

}  // namespace

uint64_t PanelScreenOnEnter(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249D18 PanelScreen::OnEnter");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  StoreU32(base, screen + 9612, 0);
  Slot(base, LoadU32(base, screen + kCarList), 8, r4, frame, 0x82249D48, calls);
  const uint64_t result =
      Slot(base, LoadU32(base, screen + kGroupPair), 12, std::nullopt, frame, 0x82249D5C, calls);
  StoreU32(base, screen + 9608, LoadU32(base, screen + 9616) != 0 ? 1 : 9);
  return result;
}

uint64_t PanelScreenOpenMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248CD0 PanelScreen::OpenMovie");
  return Slot(base, r3, 52, r4, sp, lr, calls);
}

uint64_t PanelScreenWaitForMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249D88 PanelScreen::WaitForMovie");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  const uint64_t loaded = Slot(base, r3, 8, r4, frame, 0x82249DAC, calls);
  StoreU32(base, screen + 9612, uint32_t(loaded));
  if (uint32_t(loaded) != 0) {
    uint32_t state;
    if (LoadU32(base, screen + 9652) != 0) {
      state = 3;
    } else {
      state = LoadU32(base, screen + 9616) != 0 ? 9 : 4;
    }
    StoreU32(base, screen + 9608, state);
  }
  return loaded;
}

uint64_t PanelScreenOpen(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249DF8 PanelScreen::Open");
  const uint32_t screen = uint32_t(r3);
  if (LoadU32(base, screen + 9612) == 0) {
    StoreU32(base, screen + 9608, 1);
    return r3;
  }
  const uint32_t list = LoadU32(base, screen + kCarList);
  StoreU32(base, screen + 9608, 4);
  return Slot(base, list, 52, 1, sp, lr, calls);
}

uint64_t PanelScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248CE0 PanelScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  if (LoadU32(base, screen + 9652) == 0) {
    const uint64_t list_tab = Slot(base, r3, 24, r4, frame, 0x82248D10, calls);
    Slot(base, LoadU32(base, screen + kCarList), 60, list_tab, frame, 0x82248D2C, calls);
    const uint64_t entry = Slot(base, r3, 28, std::nullopt, frame, 0x82248D40, calls);
    Slot(base, LoadU32(base, screen + kCarList), 64, entry, frame, 0x82248D5C, calls);
    const uint64_t slot = Slot(base, r3, 32, std::nullopt, frame, 0x82248D70, calls);
    Slot(base, LoadU32(base, screen + kCarList), 68, slot, frame, 0x82248D8C, calls);
    Slot(base, LoadU32(base, screen + kCarList), 12, std::nullopt, frame, 0x82248DA0, calls);
  } else {
    Slot(base, LoadU32(base, screen + kCarList), 12, r4, frame, 0x82248DA0, calls);
  }
  Slot(base, LoadU32(base, screen + kCarList), 4, std::nullopt, frame, 0x82248DB4, calls);
  return Slot(base, LoadU32(base, screen + kGroupPair), 20, std::nullopt, frame, 0x82248DC8,
              calls);
}

uint64_t PanelScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248C48 PanelScreen::Update");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  bool first = true;
  // The first call's r4 is the handler's; the later ones' what the last
  // call left.
  auto next_r4 = [&first, r4]() -> std::optional<uint64_t> {
    if (first) {
      first = false;
      return r4;
    }
    return std::nullopt;
  };
  if (const uint32_t pair = LoadU32(base, screen + kGroupPair); pair != 0) {
    r3 = Slot(base, pair, 8, next_r4(), frame, 0x82248C7C, calls);
  }
  if (const uint32_t list = LoadU32(base, screen + kCarList); list != 0) {
    r3 = Slot(base, list, 24, next_r4(), frame, 0x82248C9C, calls);
  }
  if (const uint32_t pair = LoadU32(base, screen + kGroupPair); pair != 0) {
    r3 = Slot(base, pair, 4, next_r4(), frame, 0x82248CBC, calls);
  }
  return r3;
}

uint64_t PanelScreenOnMessage24(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248DF8 PanelScreen::OnMessage24");
  return Slot(base, LoadU32(base, uint32_t(r3) + kCarList), 44, 0, sp, lr, calls);
}

uint64_t PanelScreenLeave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82249E38 PanelScreen::Leave");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  Slot(base, LoadU32(base, screen + kCarList), 28, r4, frame, 0x82249E68, calls);
  Slot(base, LoadU32(base, screen + kGroupPair), 28, std::nullopt, frame, 0x82249E7C, calls);
  const uint64_t result = Slot(base, r3, 72, r4, frame, 0x82249E94, calls);
  StoreU32(base, screen + 9608, 7);
  return result;
}

uint64_t PanelScreenOnLeft(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248B08 PanelScreen::OnLeft");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  Slot(base, LoadU32(base, screen + kCarList), 16, r4, frame, 0x82248B30, calls);
  return Slot(base, LoadU32(base, screen + kGroupPair), 16, std::nullopt, frame, 0x82248B44,
              calls);
}

uint64_t PanelScreenDrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82248B58 PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  if (LoadU32(base, screen + kCarList) != 0) {
    Slot(base, LoadU32(base, screen + kCarList), 20, r4, frame, 0x82248B94, calls);
    uint32_t pick = 0;
    if (uint32_t(Slot(base, LoadU32(base, screen + kCarList), 32, std::nullopt, frame,
                      0x82248BA8, calls)) != 0) {
      pick = 12;
    } else if (uint32_t(Slot(base, LoadU32(base, screen + kCarList), 36, std::nullopt, frame,
                             0x82248BD0, calls)) != 0) {
      pick = 16;
    } else if (uint32_t(Slot(base, LoadU32(base, screen + kCarList), 40, std::nullopt, frame,
                             0x82248BF8, calls)) != 0) {
      pick = 20;
    }
    if (pick != 0) {
      Slot(base, r3, pick, r4, frame, 0x82248C18, calls);
    }
  }
  return Slot(base, r3, 40, r4, frame, 0x82248C30, calls);
}

uint64_t Type7PanelScreenDrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240908 Type7PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  uint64_t result = PanelScreenDrawList(base, r3, r4, frame, calls);
  if (LoadU32(base, screen + 11700) != 0) {
    result = calls.call(kAskQuit, frame, 0x82240940, r3, r4);
  }
  return result;
}

uint64_t Type7PanelScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82240958 Type7PanelScreen::Draw");
  const uint32_t screen = uint32_t(r3);
  const int32_t quit = int32_t(LoadU32(base, screen + 9656));
  if (quit == 0) {
    return Type7PanelScreenDrawList(base, r3, r4, sp, calls);
  }
  if (quit == 2 && (LoadU32(base, kFadeCountA) | LoadU32(base, kFadeCountB)) == 0) {
    StoreU32(base, screen + 9608, 10);
  }
  return r3;
}

}  // namespace type67_screens
