// The type 6 and 7 modes' handlers and helpers that make no guest calls
// (docs/functions/type6-type7-modes.md): the screens' and dialogs' leaves,
// their blocks' allocation and freeing over the allocation layer, the
// garage camera's presets and the controller pages' blocks. The originals'
// loads and stores in their order; float words go through float registers
// (lfs, stfs) as the originals move them.

#include "type67_modes/type67_modes.h"

#include "base/log.h"
#include "guest.h"
#include "memory/heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <initializer_list>

namespace type67_modes {

namespace {

using guest::LoadU32;
using guest::StoreU32;

// A float word through a float register and back (lfs, stfs).
uint32_t ThroughFloat(uint8_t* base, uint32_t address) {
  return std::bit_cast<uint32_t>(float(double(guest::LoadFloat(base, address))));
}

// VirtualAlloc(0, size, MEM_COMMIT, read-write) over the allocation layer.
uint32_t Pages(uint32_t size) { return memory::AllocPages(size, 4096); }

// VirtualFree(block, 0, MEM_RELEASE)'s r3 for the layer's pages.
uint64_t Free(uint32_t block) { return memory::FreePages(block) ? 1 : 0; }

bool AllPages(std::initializer_list<uint32_t> blocks) {
  for (const uint32_t block : blocks) {
    if (!memory::IsPages(block)) {
      return false;
    }
  }
  return true;
}

// The constants the poses and presets are built from.
constexpr uint32_t kZero = 0x82000A34;  // 0.0
constexpr uint32_t kOne = 0x82000A30;   // 1.0
constexpr uint32_t kPoseY = 0x82009EAC;

// The vectors a pose or preset is built from, below `sp` (-48 to -1), as
// the originals' leaves store them: (z, z, z, w) twice, then (z, `y`, z, w).
void ScratchVectors(uint8_t* base, uint32_t sp, uint32_t z, uint32_t w, uint32_t y) {
  const uint32_t words[12] = {z, z, z, w, z, z, z, w, z, y, z, w};
  for (uint32_t i = 0; i < 12; i++) {
    StoreU32(base, sp - 48 + 4 * i, words[i]);
  }
}

void CopyWords(uint8_t* base, uint32_t to, uint32_t from, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    StoreU32(base, to + 4 * i, LoadU32(base, from + 4 * i));
  }
}

// A message (three words: `message`, 0, 0) queued in a dispatcher's ring
// (+204: 50 slots of 12 bytes, the count at +600, the start at +604), with
// no overflow check, as the handlers do it inline.
uint32_t RingSlot(uint8_t* base, uint32_t dispatcher) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  return ring + (end - (end / 50) * 50) * 12;
}

constexpr uint32_t kMinusOne = 0xFFFFFFFF;

}  // namespace

// --- The garage stage and camera ---

uint64_t GarageStageSetPlates(uint8_t* base, uint64_t r3, uint32_t value) {
  ITC_NATIVE_RUNNING("sub_82178DE8 GarageStage::SetPlates");
  StoreU32(base, LoadU32(base, uint32_t(r3)) + 6092, value);
  return r3;
}

uint64_t GarageStageResetPose(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217A2D8 GarageStage::ResetPose");
  const uint32_t pose = uint32_t(r3);
  const uint32_t zero = ThroughFloat(base, kZero);
  const uint32_t one = ThroughFloat(base, kOne);
  const uint32_t y = ThroughFloat(base, kPoseY);
  ScratchVectors(base, sp, zero, one, y);
  StoreU32(base, pose + 0, zero);
  CopyWords(base, pose + 16, sp - 48, 12);
  StoreU32(base, pose + 64, 0);
  return r3;
}

uint64_t CameraPresetConstruct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217BE08 CameraPreset::CameraPreset");
  const uint32_t preset = uint32_t(r3);
  const uint32_t zero = ThroughFloat(base, kZero);
  const uint32_t one = ThroughFloat(base, kOne);
  ScratchVectors(base, sp, zero, one, zero);
  for (uint32_t offset = 4; offset <= 12; offset += 4) {
    StoreU32(base, preset + offset, zero);
  }
  CopyWords(base, preset + 16, sp - 48, 12);
  return r3;
}

uint64_t CameraPresetCopy(uint8_t* base, uint32_t source, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8217D7D0 CameraPreset::Copy");
  const uint32_t copy = Pages(64);
  CameraPresetConstruct(base, copy, sp - 96);
  for (uint32_t offset = 4; offset <= 60; offset += 4) {
    StoreU32(base, copy + offset, ThroughFloat(base, source + offset));
  }
  return copy;
}

namespace {

constexpr uint32_t kPresets = 296;
constexpr uint32_t kPresetCount = 296 + 4096;

bool PresetsArePages(uint8_t* base, uint32_t camera) {
  const uint32_t count = LoadU32(base, camera + kPresetCount);
  for (uint32_t i = 0; i < count; i++) {
    if (!memory::IsPages(LoadU32(base, camera + kPresets + 4 * i))) {
      return false;
    }
  }
  return true;
}

uint64_t FreePresets(uint8_t* base, uint64_t r3) {
  const uint32_t camera = uint32_t(r3);
  while (LoadU32(base, camera + kPresetCount) != 0) {
    const uint32_t count = LoadU32(base, camera + kPresetCount) - 1;
    StoreU32(base, camera + kPresetCount, count);
    r3 = Free(LoadU32(base, camera + kPresets + 4 * count));
  }
  return r3;
}

}  // namespace

std::optional<uint64_t> GarageCameraFreePresets(uint8_t* base, uint64_t r3) {
  if (!PresetsArePages(base, uint32_t(r3))) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8217D760 GarageCamera::FreePresets");
  return FreePresets(base, r3);
}

std::optional<uint64_t> GarageCameraOnPresetsLoaded(uint8_t* base, uint32_t data, uint32_t camera,
                                                    uint32_t sp) {
  if (!memory::IsPages(data)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_8217FBE8 GarageCamera::OnPresetsLoaded");
  uint32_t count = LoadU32(base, data + 4);
  uint32_t record = LoadU32(base, data + 8) + data;
  while (count != 0) {
    const uint32_t copy = uint32_t(CameraPresetCopy(base, record, sp - 128));
    const uint32_t presets = LoadU32(base, camera + kPresetCount);
    count--;
    record += 64;
    StoreU32(base, camera + kPresetCount, presets + 1);
    StoreU32(base, camera + kPresets + 4 * presets, copy);
  }
  StoreU32(base, camera + 0, 1);
  return Free(data);
}

std::optional<uint64_t> StandSlotFree(uint8_t* base, uint64_t r3) {
  const uint32_t slot = uint32_t(r3);
  const uint32_t block = LoadU32(base, slot);
  if (block == 0) {
    ITC_NATIVE_RUNNING("sub_821800B0 StandSlot::Free");
    return r3;
  }
  if (!memory::IsPages(block) || !PresetsArePages(base, block + 112)) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821800B0 StandSlot::Free");
  FreePresets(base, block + 112);
  const uint64_t freed = Free(LoadU32(base, slot));
  StoreU32(base, slot, 0);
  return freed;
}

// --- The controller pages ---

uint64_t ControllerOptionsOnEnter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_821BB120 ControllerOptions::OnEnter");
  StoreU32(base, machine + 9608, 1);
  const uint32_t data = Pages(68);
  StoreU32(base, kControllerData, data);
  StoreU32(base, data + 12, 0);
  StoreU32(base, data + 16, 6);
  for (const uint32_t offset : {20, 0, 4, 8, 40, 48, 52, 36, 64, 24, 28, 32}) {
    StoreU32(base, data + offset, 0);
  }
  StoreU32(base, LoadU32(base, kControllerData) + 64, 1);
  const uint32_t page = Pages(28);
  StoreU32(base, kControllerPage, page);
  for (const uint32_t offset : {4, 0, 8, 24}) {
    StoreU32(base, page + offset, 0);
  }
  StoreU32(base, page + 20, kMinusOne);
  StoreU32(base, page + 12, kMinusOne);
  return page;
}

std::optional<uint64_t> ControllerOptionsOnLeft(uint8_t* base) {
  if (!AllPages({LoadU32(base, kControllerData), LoadU32(base, kControllerPage)})) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_821BB1F8 ControllerOptions::OnLeft");
  Free(LoadU32(base, kControllerData));
  const uint64_t r3 = Free(LoadU32(base, kControllerPage));
  StoreU32(base, kControllerData, 0);
  StoreU32(base, kControllerPage, 0);
  return r3;
}

uint64_t ControllerOptionsShow(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BB260 ControllerOptions::Show");
  const uint32_t page = LoadU32(base, kControllerPage);
  const uint32_t current = LoadU32(base, page + 0);
  if (current != kMinusOne) {
    StoreU32(base, page + 8, current);
  }
  StoreU32(base, page + 0, kMinusOne);
  StoreU32(base, page + 12, kMinusOne);
  StoreU32(base, page + 16, kMinusOne);
  return r3;
}

uint64_t ControllerOptionsOpen(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_821BC660 ControllerOptions::Open");
  StoreU32(base, uint32_t(r3) + 9604, 4);
  return r3;
}

uint64_t ControllerOptionsLeave(uint8_t* base, uint64_t r3, uint32_t dispatcher) {
  ITC_NATIVE_RUNNING("sub_821BFB48 ControllerOptions::Leave");
  StoreU32(base, uint32_t(r3) + 9604, 7);
  uint32_t message;
  switch (int32_t(LoadU32(base, kModeType))) {
    case 2:
    case 4:
    case 6:
    case 8:
      message = 4;
      break;
    case 7:
      message = 3;
      break;
    default:
      return r3;
  }
  const uint32_t slot = RingSlot(base, dispatcher);
  StoreU32(base, slot + 8, 0);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 0, message);
  return r3;
}

uint64_t ControllerOptionsStepSliders(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_821BB290 ControllerOptions::StepSliders");
  const uint32_t menu = LoadU32(base, kControllerData);
  const int32_t side = int32_t(LoadU32(base, menu + 36));
  const uint32_t cursor = LoadU32(base, menu + 0);
  if ((side != -1 && side != 1) || cursor >= 3) {
    return 0;
  }
  // The sliders (-10 to 10) at +272 to +280: no change at a bound.
  const uint32_t settings = LoadU32(base, kControllerSettings);
  const uint32_t field = settings + 272 + 4 * cursor;
  const int32_t value = int32_t(LoadU32(base, field));
  if (side == -1 ? value <= -10 : value >= 10) {
    return 0;
  }
  StoreU32(base, field, uint32_t(value + side));
  return 1;
}

uint64_t ControllerOptionsStepSide(uint8_t* base, uint32_t offset, uint32_t last) {
  if (offset == 672) {
    ITC_NATIVE_RUNNING("sub_821BB3B8 ControllerOptions::StepButtons");
  } else {
    ITC_NATIVE_RUNNING("sub_821BB590 ControllerOptions::StepAnalog");
  }
  const uint32_t menu = LoadU32(base, kControllerData);
  const uint32_t side = LoadU32(base, menu + 36);
  const uint32_t cursor = LoadU32(base, menu + 0);
  StoreU32(base, LoadU32(base, kControllerSettings) + offset, 0);
  if (side != kMinusOne && side != 1) {
    return 0;
  }
  StoreU32(base, LoadU32(base, kControllerSettings) + offset, side);
  return cursor == last ? 0 : 1;
}

uint64_t ControllerOptionsSetMarkY(uint8_t* base, uint64_t y, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821C0238 ControllerOptions::SetMarkY");
  const uint32_t frame = sp - 112;
  StoreU32(base, frame, sp);
  StoreU32(base, frame + 80, 7);
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const uint32_t node = ui_movies::FindNode(base, movie, 8, 5);
  const uint32_t child = ui_movies::Child(base, ui_movies::Child(base, node, 0), 0);
  // The int's doubleword stored over the key for the conversion (std, lfd,
  // fcfid, frsp).
  const int64_t value = int32_t(uint32_t(y));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(uint64_t(value)));
  guest::StoreFloat(base, child + 16, float(double(value)));
  return child;
}

// --- The screens' and dialogs' leaves ---

uint64_t PanelScreenInitWords(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82230740 PanelScreen::InitWords");
  StoreU32(base, uint32_t(r3) + 11672, 4);
  StoreU32(base, uint32_t(r3) + 11680, 9);
  return r3;
}

uint64_t ScreenMachineWait(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82277068 ScreenMachine::Wait");
  StoreU32(base, uint32_t(r3) + 9604, 9);
  return r3;
}

uint64_t ScreenReenter(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223B740 Screen::Reenter");
  const uint32_t screen = uint32_t(r3);
  StoreU32(base, screen + 9608, int32_t(LoadU32(base, screen + 9612)) != 0 ? 4 : 1);
  return r3;
}

uint64_t DialogShown(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82231F48 Type6Dialog::Shown");
  } else {
    ITC_NATIVE_RUNNING("sub_822419C0 Type7Dialog::Shown");
  }
  StoreU32(base, uint32_t(r3) + shown, 0);
  return r3;
}

uint64_t DialogOnEnter(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82231F98 Type6Dialog::OnEnter");
  } else {
    ITC_NATIVE_RUNNING("sub_82241A08 Type7Dialog::OnEnter");
  }
  const uint32_t dialog = uint32_t(r3);
  for (const uint32_t offset : {9608, 9644, 9640, 9648}) {
    StoreU32(base, dialog + offset, 0);
  }
  StoreU32(base, dialog + 9660, kMinusOne);
  StoreU32(base, dialog + 9652, kMinusOne);
  StoreU32(base, dialog + 9664, 0);
  StoreU32(base, dialog + 9752, 0);
  StoreU32(base, dialog + 9680, 0);
  StoreU32(base, dialog + 9684, 1);
  StoreU32(base, dialog + 9688, 1);
  for (const uint32_t offset :
       {9668, 9672, 9676, 9708, 9716, 9720, 9704, 9732, 9692, 9696, 9700, 9736}) {
    StoreU32(base, dialog + offset, 0);
  }
  StoreU32(base, dialog + 9740, 21);
  StoreU32(base, dialog + 9744, 0);
  const uint32_t open = LoadU32(base, dialog + 9612);
  StoreU32(base, dialog + 9748, 0);
  if (int32_t(open) != 0) {
    StoreU32(base, dialog + 9604, 1);
    StoreU32(base, dialog + shown, kMinusOne);
  } else {
    StoreU32(base, dialog + 9604, 9);
    StoreU32(base, dialog + shown, 3);
  }
  return r3;
}

uint64_t DialogWaitForMovies(uint8_t* base, uint64_t r3, uint32_t shown) {
  if (shown == 9756) {
    ITC_NATIVE_RUNNING("sub_82232910 Type6Dialog::WaitForMovies");
  } else {
    ITC_NATIVE_RUNNING("sub_82242388 Type7Dialog::WaitForMovies");
  }
  const uint32_t dialog = uint32_t(r3);
  if (int32_t(LoadU32(base, dialog + 9608)) != 0) {
    StoreU32(base, dialog + 9736, 1);
    StoreU32(base, dialog + 9604, 9);
    StoreU32(base, dialog + shown, 3);
    return r3;
  }
  for (uint32_t id = 16; id <= 19; id++) {
    r3 = ui_movies::IsLoaded(base, id);
    if (int32_t(r3) == 0) {
      return r3;
    }
  }
  StoreU32(base, dialog + 9608, 1);
  return r3;
}

uint64_t Screen0OnEnter(uint8_t* base, uint32_t screen, uint32_t d, uint32_t items) {
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_822339F0 Type6Screen0::OnEnter");
  } else {
    ITC_NATIVE_RUNNING("sub_82243F58 Type7Screen0::OnEnter");
  }
  const uint32_t s = screen + d;
  StoreU32(base, s + 9640, Pages(76));
  StoreU32(base, s + 9644, Pages(28));
  const uint32_t list = Pages(68);
  const uint32_t page = LoadU32(base, s + 9644);
  StoreU32(base, s + 9648, list);
  if (page != 0) {
    for (const uint32_t offset : {4, 0, 8}) {
      StoreU32(base, page + offset, 0);
    }
    StoreU32(base, page + 20, kMinusOne);
    StoreU32(base, page + 12, kMinusOne);
    StoreU32(base, page + 24, 0);
  }
  const uint32_t menu = LoadU32(base, s + 9640);
  StoreU32(base, menu + 72, kMinusOne);
  StoreU32(base, menu + 68, 0);
  StoreU32(base, menu + 0, 0);
  const uint32_t l = LoadU32(base, s + 9648);
  StoreU32(base, s + 9652, 0);
  StoreU32(base, l + 12, 0);
  StoreU32(base, l + 16, items);
  StoreU32(base, l + 20, 1);
  for (const uint32_t offset : {0, 4, 8, 40, 44, 48, 52, 56}) {
    StoreU32(base, l + offset, 0);
  }
  StoreU32(base, l + 60, items + 1);
  for (const uint32_t offset : {36, 64, 24, 28, 32}) {
    StoreU32(base, l + offset, 0);
  }
  StoreU32(base, s + 9604, int32_t(LoadU32(base, s + 9612)) != 0 ? 1 : 9);
  return list;
}

std::optional<uint64_t> Screen0OnLeft(uint8_t* base, uint32_t screen, uint32_t d) {
  const uint32_t s = screen + d;
  if (!AllPages({LoadU32(base, s + 9640), LoadU32(base, s + 9644), LoadU32(base, s + 9648)})) {
    return std::nullopt;
  }
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_82233870 Type6Screen0::OnLeft");
  } else {
    ITC_NATIVE_RUNNING("sub_82243E08 Type7Screen0::OnLeft");
  }
  Free(LoadU32(base, s + 9640));
  Free(LoadU32(base, s + 9644));
  const uint64_t r3 = Free(LoadU32(base, s + 9648));
  StoreU32(base, s + 9640, 0);
  StoreU32(base, s + 9644, 0);
  StoreU32(base, s + 9648, 0);
  return r3;
}

uint64_t Screen0WaitForMovie(uint8_t* base, uint64_t r3, uint32_t d) {
  if (d == 0) {
    ITC_NATIVE_RUNNING("sub_82233B18 Type6Screen0::WaitForMovie");
  } else {
    ITC_NATIVE_RUNNING("sub_82244080 Type7Screen0::WaitForMovie");
  }
  const uint32_t s = uint32_t(r3) + d;
  if (int32_t(LoadU32(base, s + 9608)) != 0) {
    StoreU32(base, s + 9604, 9);
    return r3;
  }
  const uint32_t loaded = ui_movies::IsLoaded(base, 22);
  if (int32_t(loaded) != 0) {
    StoreU32(base, s + 9608, 1);
  }
  return loaded;
}

uint64_t Screen0Pick(uint8_t* base, uint64_t r3, uint32_t menu, uint32_t pick) {
  if (menu == 9644) {
    ITC_NATIVE_RUNNING("sub_82243F28 Type7Screen0::PickYes");
  } else if (pick == 1) {
    ITC_NATIVE_RUNNING("sub_82233990 Type6Screen0::PickYes");
  } else {
    ITC_NATIVE_RUNNING("sub_822339A0 Type6Screen0::PickNo");
  }
  StoreU32(base, LoadU32(base, uint32_t(r3) + menu) + 72, pick);
  return r3;
}

uint64_t Type6Screen0GoOn(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_822339C0 Type6Screen0::GoOn");
  const uint32_t page = LoadU32(base, uint32_t(r3) + 9644);
  const uint32_t current = LoadU32(base, page + 0);
  if (current != 3) {
    StoreU32(base, page + 8, current);
  }
  StoreU32(base, page + 12, 3);
  StoreU32(base, page + 16, kMinusOne);
  StoreU32(base, page + 4, 3);
  StoreU32(base, page + 0, kMinusOne);
  return r3;
}

}  // namespace type67_modes
