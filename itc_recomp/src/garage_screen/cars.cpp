// The garage screen's car objects never seen in play (the stand's car, the
// spec texts, the questions, the plate editor's and the spec sheet's
// set-ups, the view's camera buttons; docs/functions/garage-screen.md): see
// garage_screen.h.

#include "garage_screen/garage_screen.h"

#include "base/log.h"
#include "garage_screen/internal.h"
#include "type67_modes/type67_modes.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace garage_screen {

namespace {

constexpr uint32_t kCarInit = 0x820CD5D8;         // Car::Init
constexpr uint32_t kCarRefresh = 0x820C4998;      // Car::Refresh
constexpr uint32_t kCarDataConstruct = 0x820AEF80;  // CarData::CarData
constexpr uint32_t kSetupFromCar = 0x820C27E8;    // Vehicle::SetupFromCar
constexpr uint32_t kCurveFromEngine = 0x820CE7D8; // PowerCurve::FromEngine
constexpr uint32_t kShiftPeak = 0x820CE1D8;       // PowerCurve::ShiftPeak
constexpr uint32_t kSetCar = 0x8217FD58;          // GarageStage::SetCar
constexpr uint32_t kSetNodeNumberText = 0x8211CA18;
constexpr uint32_t kGarageBytes = 98612;          // the save's garage list (+6120)
constexpr uint32_t kCarBytes = 1884;              // a garage car's words a car object takes
constexpr uint32_t kSheet = 0x826D846C;           // the spec sheet's data
constexpr uint32_t kPlate = 0x826D8438;           // the plate editor's data
constexpr uint32_t kCursor = 40960;               // the dialog movies' cursor word
constexpr uint32_t kScale = 0x82009EC0;           // the curve's peak scale
constexpr uint32_t kPowerScale = 0x820021E8;

// A car object at `car` from the garage car `from`: Car::Init, its first
// 1,884 bytes copied over, Car::Refresh (with the flags' bit 0 set between
// when `flag`), through the guest from `frame` with the three return
// addresses. Refresh's r3.
uint64_t MakeCar(uint8_t* base, uint32_t car, uint32_t from, uint32_t frame, const uint32_t lrs[3],
                 bool flag, const Calls& calls) {
  calls.Call(kCarInit, frame, lrs[0], car);
  Memcpy(base, calls, car, from, kCarBytes, frame, lrs[1]);
  if (flag) {
    Store(base, car + 3888, Load(base, car + 3888) | 1);
  }
  return calls.Call(kCarRefresh, frame, lrs[2], car);
}

// The engine's figures at `out` (+0, +4, +8 to +16 ...) the spec sheet and
// the spec texts share: the power (`data` +400 + +672, by the constant, plus
// +0) truncated.
uint32_t Power(uint8_t* base, uint32_t data) {
  const double sum = double(float(Lfs(base, data + 672) + Lfs(base, data + 400)));
  return Fctiwz(double(float(std::fma(sum, Lfs(base, kPowerScale), Lfs(base, data + 0)))));
}

}  // namespace

uint64_t ReloadStandCar(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193B10 GarageScreen::ReloadStandCar");
  // __chkstk's probe dropped (sub_825A9934 is Dropped).
  const uint32_t frame = Frame(base, sp, 100688);
  const uint32_t data = Data(base);
  Memcpy(base, calls, frame + 2048, Load(base, data + 36) + 6120, kGarageBytes, frame,
         0x82193B50);
  CopyGarageCar(base, frame + 80, frame + 2048, Load(base, data + 48) + Load(base, data + 44),
                frame, calls);
  const uint64_t block = calls.Call(kBlockOf, frame, 0x82193B70, Load(base, frame + 92));
  const uint64_t r3 = calls.Call(kReloadModel, frame, 0x82193B84, Holder(base), block);
  Store(base, Data(base) + 76, uint32_t(r3));
  return r3;
}

uint64_t PutCarOnStand(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82193BA8 GarageScreen::PutCarOnStand");
  const uint32_t frame = Frame(base, sp, 104592);
  const uint32_t data = Data(base);
  Memcpy(base, calls, frame + 5952, Load(base, data + 36) + 6120, kGarageBytes, frame,
         0x82193BE4);
  CopyGarageCar(base, frame + 3984, frame + 5952, Load(base, data + 48) + Load(base, data + 44),
                frame, calls);
  static constexpr uint32_t kLrs[3] = {0x82193C04, 0x82193C14, 0x82193C28};
  MakeCar(base, frame + 80, frame + 3984, frame, kLrs, true, calls);
  return calls.Call(kSetCar, frame, 0x82193C40, Holder(base), frame + 80, ~0ull);
}

uint64_t ShowCarSpecs(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82194D90 GarageScreen::ShowCarSpecs");
  const uint32_t frame = Frame(base, sp, 6976);
  const uint32_t object = frame + 3040, vehicle = frame + 1184, curve = frame + 112;
  static constexpr uint32_t kLrs[3] = {0x82194DB4, 0x82194DC4, 0x82194DCC};
  MakeCar(base, object, car, frame, kLrs, false, calls);
  const uint32_t p = vehicle_accessors::ObjectRecord1888(object);
  calls.Call(kCarDataConstruct, frame, 0x82194DE0, vehicle);
  calls.Call(kSetupFromCar, frame, 0x82194DEC, vehicle, object);
  calls.Call(kCurveFromEngine, frame, 0x82194DF8, curve, frame + 2452);
  calls.Call(kShiftPeak, frame, 0x82194E04, curve, 0);
  // The figures' record at the frame's +80 (ShowSpecTexts').
  Store(base, frame + 80, guest::LoadU8(base, p + 130));
  Store(base, frame + 84, guest::LoadU8(base, p + 69) != 0 ? 1 : 0);
  Store(base, frame + 88, Power(base, vehicle));
  Store(base, frame + 92, Load(base, p + 8));
  Store(base, frame + 96, Fctiwz(Lfs(base, curve + 1056)));
  Stfs(base, frame + 100, double(float(Lfs(base, curve + 1048) * Lfs(base, kScale))));
  ShowSpecTexts(base, frame + 80, frame, calls);
  return ShowCarNames(base, Load(base, car + 12), guest::LoadU8(base, p + 129), frame, calls);
}

uint64_t SetUseQuestion(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821933D0 GarageScreen::SetUseQuestion");
  const uint32_t frame = Frame(base, sp, 4064);
  const uint32_t movie = FindMovie(base, 16, frame + 80);
  const uint32_t id = Load(base, car + 12);
  static constexpr uint32_t kLrs[3] = {0x8219340C, 0x8219341C, 0x82193424};
  MakeCar(base, frame + 96, car, frame, kLrs, false, calls);
  const uint32_t p = vehicle_accessors::ObjectRecord1888(frame + 96);
  Store(base, movie + kCursor, 0);
  const uint64_t maker = uint64_t(guest::LoadU8(base, p + 129)) + 1934;
  const uint32_t question = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 0);
  const uint32_t yes = ui_movies::FindNode(base, movie, 1, 3);
  const uint32_t no = ui_movies::FindNode(base, movie, 1, 4);
  // String 97 with the maker's and the car's names (r6 to r9 set; r10 left
  // unset: 0).
  calls.CallArgs(kSetNodeText, frame, 0x821934A4,
                 {movie, question, 97, 24, maker, uint64_t(id) + 1646, uint64_t(id) + 1718, 0});
  calls.Call(kSetNodeText, frame, 0x821934B4, movie, yes, 2);
  return calls.Call(kSetNodeText, frame, 0x821934C4, movie, no, 3);
}

uint64_t SetSaleQuestion(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821934D0 GarageScreen::SetSaleQuestion");
  const uint32_t frame = Frame(base, sp, 4112);
  const uint32_t movie = FindMovie(base, 18, frame + 80);
  const uint32_t id = Load(base, car + 12);
  Store(base, Data(base) + 100, uint32_t(SalePrice(base, car, frame)));
  static constexpr uint32_t kLrs[3] = {0x82193528, 0x82193538, 0x82193540};
  MakeCar(base, frame + 96, car, frame, kLrs, false, calls);
  const uint32_t p = vehicle_accessors::ObjectRecord1888(frame + 96);
  Store(base, movie + kCursor, 1);
  auto node = [&](uint32_t sub_scene, uint32_t child) {
    return ui_movies::FindNode(base, movie, sub_scene, child);
  };
  const uint32_t title = node(0xFFFFFFFFu, 4), maker = node(0xFFFFFFFFu, 5);
  const uint32_t name1 = node(0xFFFFFFFFu, 6), name2 = node(0xFFFFFFFFu, 7);
  const uint32_t line = node(1, 4), price = node(1, 2), label = node(1, 3);
  const uint32_t yes = node(2, 3), no = node(2, 4);
  calls.Call(kSetNodeText, frame, 0x82193620, movie, title, 25);
  calls.Call(kSetNodeText, frame, 0x82193634, movie, maker,
             uint64_t(guest::LoadU8(base, p + 129)) + 1934);
  calls.Call(kSetNodeText, frame, 0x82193644, movie, name1, uint64_t(id) + 1646);
  calls.Call(kSetNodeText, frame, 0x82193654, movie, name2, uint64_t(id) + 1718);
  calls.Call(kSetNodeNumberText, frame, 0x82193668, movie, price,
             Load(base, Data(base) + 100));
  calls.Call(kSetNodeText, frame, 0x82193678, movie, line, 527);
  calls.Call(kSetNodeText, frame, 0x82193688, movie, label, 435);
  calls.Call(kSetNodeText, frame, 0x82193698, movie, yes, 2);
  return calls.Call(kSetNodeText, frame, 0x821936A8, movie, no, 3);
}

uint64_t SetUpPlateEditor(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191810 GarageScreen::SetUpPlateEditor");
  const uint32_t frame = Frame(base, sp, 4000);
  static constexpr uint32_t kLrs[3] = {0x8219182C, 0x8219183C, 0x82191844};
  MakeCar(base, frame + 80, car, frame, kLrs, false, calls);
  const uint32_t p = vehicle_accessors::ObjectRecord1888(frame + 80);
  const uint32_t plate = Load(base, kPlate) + 24;
  Store(base, plate + 24, 0);
  // The plate's kind by the car's sizes (each compared unsigned).
  const bool small = Load(base, p + 8) <= 2000 && Load(base, p + 76) <= 4700 &&
                     Load(base, p + 80) <= 2000 && Load(base, p + 84) <= 1700;
  Store(base, plate + 20, small ? 1 : 0);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, plate + 4 * i, Load(base, car + 1568 + 4 * i));
  }
  return p;
}

uint64_t SetUpSpecSheet(uint8_t* base, uint32_t car, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191628 GarageScreen::SetUpSpecSheet");
  const uint32_t frame = Frame(base, sp, 5104);
  const uint32_t sheet = Load(base, kSheet) + 16;
  const uint32_t object = frame + 1152, curve = frame + 80;
  static constexpr uint32_t kLrs[3] = {0x82191650, 0x82191660, 0x82191668};
  MakeCar(base, object, car, frame, kLrs, false, calls);
  const uint32_t p = vehicle_accessors::ObjectRecord1888(object);
  const uint32_t vehicle = sheet + 96;
  calls.Call(kSetupFromCar, frame, 0x82191684, vehicle, object);
  calls.Call(kCurveFromEngine, frame, 0x82191690, curve, vehicle + 1268);
  const uint64_t r3 = calls.Call(kShiftPeak, frame, 0x8219169C, curve, 0);
  Store(base, sheet + 80, 2);
  for (uint32_t i = 0; i < 5; i++) {
    Store(base, sheet + 60 + 4 * i, Load(base, car + 1568 + 4 * i));
  }
  Store(base, sheet + 0, Fctiwz(Lfs(base, car + 1892)));
  Store(base, sheet + 4, Power(base, vehicle));
  Store(base, sheet + 8, Load(base, p + 76));
  Store(base, sheet + 12, Load(base, p + 84));
  Store(base, sheet + 16, Load(base, p + 80));
  Store(base, sheet + 20, Load(base, car + 12));
  Store(base, sheet + 24, guest::LoadU8(base, p + 129));
  Store(base, sheet + 28, guest::LoadU8(base, p + 130));
  // The drive by the spec's byte +68 (a jump table; above 7 +32 unwritten).
  // Its cases 0 and 4 are the recompiler's piece sub_82191768, merged here.
  static constexpr uint32_t kDrives[8] = {6, 2, 3, 4, 6, 0, 1, 5};
  const uint32_t kind = guest::LoadU8(base, p + 68);
  if (kind <= 7) {
    Store(base, sheet + 32, kDrives[kind]);
  }
  Store(base, sheet + 36, guest::LoadU8(base, p + 69) != 0 ? 1 : 0);
  Store(base, sheet + 40, Load(base, p + 8));
  Store(base, sheet + 44, Fctiwz(Lfs(base, curve + 1056)));
  Store(base, sheet + 48, Fctiwz(Lfs(base, curve + 1060)));
  Stfs(base, sheet + 52, double(float(Lfs(base, curve + 1048) * Lfs(base, kScale))));
  Store(base, sheet + 56, Fctiwz(Lfs(base, curve + 1052)));
  return r3;
}

uint64_t CameraA(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191A70 GarageScreen::CameraA");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, Data(base) + 88, 0);
  StageCommand(base, 1, frame, 0x82191AA4, calls);
  return type67_modes::GarageStageCameraCommand(base, Holder(base), 0, frame);
}

uint64_t CameraB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82191AC8 GarageScreen::CameraB");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, Data(base) + 88, 0);
  StageCommand(base, 1, frame, 0x82191AFC, calls);
  return type67_modes::GarageStageCameraCommand(base, Holder(base), 1, frame);
}

}  // namespace garage_screen
