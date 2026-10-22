#include "race_overlays/race_overlays.h"

#include "guest.h"

#include "base/log.h"

namespace race_overlays {

namespace {

constexpr uint32_t kZero = 0x82000A34;        // the game's float 0.0
constexpr uint32_t kRaceData = 0x826C5B2C;    // the race's data (racer records, mode)
constexpr uint32_t kViewportTable = 0x82008558;
constexpr uint32_t kAxleScale = 0x82001FBC;
constexpr uint32_t kWheelBytes = 368;

uint32_t Vehicle(uint8_t* base, uint32_t view) {
  return guest::LoadU32(base, view + 4);
}

}  // namespace

uint32_t View(uint8_t* base, uint32_t overlays, uint32_t slot) {
  ITC_NATIVE_RUNNING("sub_82126550 CarOverlays::View");
  if (slot == 25) {
    return 0;
  }
  return guest::LoadU32(base, overlays + 4 * (slot + 1));
}

uint32_t ViewportEntry(uint32_t index) {
  ITC_NATIVE_RUNNING("sub_821264A8 CarOverlays::ViewportEntry");
  return kViewportTable + 12 * index;
}

float WheelSpin(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_821268F0 OverlayView::WheelSpin");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 664);
}

float WheelValue512(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126908");
  return guest::LoadFloat(base, Vehicle(base, view) + kWheelBytes * wheel + 512);
}

float AxleValue3764(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126920");
  if (wheel < 2) {
    return guest::LoadFloat(base, Vehicle(base, view) + 3764);
  }
  if (wheel < 4) {
    return guest::LoadFloat(base, Vehicle(base, view) + 4036);
  }
  return guest::LoadFloat(base, kZero);
}

float AxleValue3384(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126958");
  float value;
  if (wheel < 2) {
    value = guest::LoadFloat(base, Vehicle(base, view) + 3384);
  } else if (wheel < 4) {
    value = guest::LoadFloat(base, Vehicle(base, view) + 3388);
  } else {
    value = guest::LoadFloat(base, kZero);
  }
  // fmuls: the product of the two singles (as doubles), rounded to single.
  return float(double(value) * double(guest::LoadFloat(base, kAxleScale)));
}

uint32_t WheelOnGround(uint8_t* base, uint32_t view, uint32_t wheel) {
  ITC_NATIVE_RUNNING("sub_82126998 OverlayView::WheelOnGround");
  return guest::LoadU8(base, Vehicle(base, view) + kWheelBytes * wheel + 433);
}

uint32_t VehicleByte2504(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126A80");
  return guest::LoadU8(base, Vehicle(base, view) + 2504);
}

uint32_t VehicleWord2252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AA0");
  return guest::LoadU32(base, Vehicle(base, view) + 2252);
}

uint32_t VehicleWord340(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AB0");
  return guest::LoadU32(base, Vehicle(base, view) + 340);
}

float VehicleFloat6256(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AC0");
  return guest::LoadFloat(base, Vehicle(base, view) + 6256);
}

float VehicleFloat6252(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AD0");
  return guest::LoadFloat(base, Vehicle(base, view) + 6252);
}

uint32_t RaceMode(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82126D18");
  return guest::LoadU32(base, guest::LoadU32(base, kRaceData) + 0xA3AF4);
}

uint32_t RacerFlags(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82127178 OverlayView::RacerFlags");
  const uint32_t racer = guest::LoadU32(base, Vehicle(base, view) + 5712);
  return guest::LoadU32(base, racer + 3888);
}

int32_t RacerId(uint8_t* base, uint32_t view) {
  ITC_NATIVE_RUNNING("sub_82126AE0 OverlayView::RacerId");
  const uint32_t records = guest::LoadU32(base, kRaceData);
  const uint32_t racer = guest::LoadU32(base, Vehicle(base, view) + 5712);
  for (uint32_t i = 0; i < 93; i++) {
    if (records + 48 + 3936 * i == racer) {
      return int32_t(guest::LoadU32(base, records + 3936 * i + 3960));
    }
  }
  return -1;
}

}  // namespace race_overlays
