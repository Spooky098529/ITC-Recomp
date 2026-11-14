// The type 6 and 7 modes' screen blocks' destructors (Type6Screens and
// Type7Screens; docs/functions/type67-screens.md): the screens they hold
// destroyed in turn. See type67_screens.h. The screens' destructors by
// their plain C++ (the panel, node and list screens'), the empty handler
// sub_82106540 (nothing) and ListNode::~ListNode's plain C++ (the archive's)
// for the first screen's base; operator delete a guest call.

#include "type67_screens/type67_screens.h"

#include "archive/archive.h"
#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "type67_screens/list_pages.h"

namespace type67_screens {

namespace {

using namespace pages;
using guest::StoreU32;

constexpr uint32_t kType6ScreensVtable = 0x82010A78, kType7ScreensVtable = 0x82010FE0;
// The power graph member's base vtable (empty slots).
constexpr uint32_t kPowerGraphBaseVtable = 0x82010A2C;

}  // namespace

uint64_t Type6ScreensDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822367C0 Type6Screens::~Type6Screens");
  const uint32_t block = uint32_t(r3), frame = Frame(base, sp, 96);
  StoreU32(base, block, kType6ScreensVtable);
  StoreU32(base, block + 52692, kPowerGraphBaseVtable);
  PanelScreenDestruct(base, r3 + 40948, false, frame, calls);
  Type6NodeScreenDestruct(base, r3 + 30396, frame, calls);
  Type6ListScreenDestruct(base, r3 + 19432, frame, calls);
  // The first screen's base: the empty handler on +4 (its r3 left).
  return r3 + 4;
}

uint64_t Type7ScreensDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82246BE0 Type7Screens::~Type7Screens");
  const uint32_t block = uint32_t(r3), frame = Frame(base, sp, 96);
  StoreU32(base, block, kType7ScreensVtable);
  StoreU32(base, block + 42352, kPowerGraphBaseVtable);
  PanelScreenDestruct(base, r3 + 30608, true, frame, calls);
  Type7ListScreenDestruct(base, r3 + 19428, frame, calls);
  // The first screen's base: ListNode::~ListNode on +4 (its r3 left).
  archive::NodeDestruct(base, block + 4);
  return r3 + 4;
}

uint64_t Type6ScreensDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82236830 Type6Screens::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Type6ScreensDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82236860);
  }
  return r3;
}

uint64_t Type7ScreensDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                      const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82246C40 Type7Screens::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  Type7ScreensDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82246C70);
  }
  return r3;
}

}  // namespace type67_screens
