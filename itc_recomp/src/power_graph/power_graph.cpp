// The front end's power and torque graph as plain C++
// (docs/functions/power-graph.md); see power_graph.h. Built with strict
// floating point (CMakeLists.txt): each step is the translation's, in its
// order, so results match it bit for bit.

#include "base/float_mode.h"
#include "power_graph/power_graph.h"

#include "base/log.h"
#include "c_runtime/arrays.h"
#include "car_makers/car_makers.h"
#include "car_physics/car_physics.h"
#include "career/career.h"
#include "command_list/command_list.h"
#include "guest.h"
#include "minimap/minimap.h"
#include "race_overlays/race_overlays.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "race_setup/race_setup.h"
#include "schema/car_list.h"
#include "schema/car_makers.h"
#include "schema/car_physics.h"
#include "schema/command_list.h"
#include "schema/power_graph.h"
#include "schema/power_graph_screen.h"
#include "schema/race_event.h"
#include "schema/race_overlays.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"
#include "schema/vehicle.h"
#include "strings_and_font/steps.h"
#include "ui_movies/game.h"
#include "vehicle/vehicle.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "xaudio/ppc_float.h"

#include <bit>
#include <cmath>

namespace power_graph {

namespace {

// The image's constants, loaded as the originals' lfs do.
constexpr uint32_t kZero = 0x82000A34;         // 0.0
constexpr uint32_t kOne = 0x82000A30;          // 1.0
constexpr uint32_t kStep = 0x82009ECC;         // 6.4, a point's width
constexpr uint32_t kToKgfM = 0x82009EC0;       // 1 / 9.80665
constexpr uint32_t kRpmStep = 0x82009EC4;      // 400.0
constexpr uint32_t kToPs = 0x82009EC8;         // 1 / 716.2
constexpr uint32_t kTorqueScale = 0x8200822C;  // 3.2
constexpr uint32_t kPowerScale = 0x82009EBC;   // 0.32
constexpr uint32_t kCountScale = 0x82002CE8;   // 0.0025
// The empty destructor the sprites' arrays are destroyed with.
constexpr uint32_t kEmptyDestructor = 0x82106540;
// A line's slots: the header, the transform's five, [13], the vertices'
// header and three, [15].
constexpr uint32_t kLineSlots = 12;

double Lfs(uint8_t* base, uint32_t address) {
  return double(guest::LoadFloat(base, address));
}
void Stfs(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}
// std of a sign-extended word.
void StoreS64(uint8_t* base, uint32_t address, int32_t value) {
  guest::StoreU32(base, address, value < 0 ? 0xFFFFFFFFu : 0);
  guest::StoreU32(base, address + 4, uint32_t(value));
}
// fcfid then frsp of a word (the originals' int to float through the frame).
double ToSingle(int32_t value) {
  return double(float(double(int64_t(value))));
}

using strings_and_font::steps::Fctiwz;
using G = schema::PowerGraphGlobals;
using Sprite = schema::OverlaySprite;

}  // namespace

bool LineRecordFlat(uint8_t* base, uint32_t sprite, uint32_t pass, uint32_t key, uint32_t frame,
                    const LineCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217B508 Line::RecordFlat");
  const auto [list, queue] = calls.list_and_queue(frame);
  G::set_line_queue(base, queue);
  G::set_line_list(base, list);
  const schema::CommandList l(base, list);
  const uint32_t used = l.used();
  if (G::transform_list(base) != list || l.chunk_open() != 0 ||
      schema::CommandListGlobals::open_chunks(base) != 0 || used + kLineSlots < used ||
      used + kLineSlots >= l.capacity()) {
    return false;
  }
  command_list::OpenChunk(base, list);
  const uint32_t header = command_list::AllocateSlots(base, list, 1);
  minimap::RecordTransform(base, pass, sprite, frame - minimap::kTransformProduct);
  const uint32_t slot = command_list::AllocateSlots(base, list, 1);
  guest::StoreU32(base, slot + 0, 13);
  guest::StoreU32(base, slot + 8, 1);
  guest::StoreU32(base, slot + 12, 0);
  guest::StoreU32(base, slot + 4, 2);
  uint32_t vertex = command_list::AllocateVertices(base, list, 2, true);
  const Sprite s(base, sprite);
  Stfs(base, vertex + 0, double(s.end0_x()));
  Stfs(base, vertex + 4, double(s.end0_y()));
  Stfs(base, vertex + 8, double(s.end0_z()));
  const uint32_t color0 = (s.end0_alpha() << 24) | (uint32_t(s.end0_red_byte()) << 16) |
                          (uint32_t(s.end0_green_byte()) << 8) | s.end0_blue_byte();
  guest::StoreU32(base, vertex + 12, color0);
  vertex += 24;
  Stfs(base, vertex + 0, double(s.end1_x()));
  Stfs(base, vertex + 4, double(s.end1_y()));
  Stfs(base, vertex + 8, double(s.end1_z()));
  const uint32_t color1 = (s.end1_alpha() << 24) | (uint32_t(s.end1_red_byte()) << 16) |
                          (uint32_t(s.end1_green_byte()) << 8) | s.end1_blue_byte();
  guest::StoreU32(base, vertex + 12, color1);
  guest::StoreU32(base, command_list::AllocateSlots(base, list, 1), 15);
  command_list::CloseChunk(base, list);
  const int32_t slots = command_list::ChunkSlots(base, list);
  guest::StoreU16(base, header + 0, 0);
  guest::StoreU16(base, header + 2, 0);
  guest::StoreU32(base, header + 4, 0);
  guest::StoreU32(base, header + 12, 0);
  guest::StoreU32(base, header + 8, uint32_t(slots - 1));
  calls.add(frame, G::line_queue(base), header, key);
  return true;
}

int32_t SegmentCount(uint8_t* base, uint32_t car) {
  const double rev = double(schema::CarData(base, car).rev_limit());
  return Fctiwz(double(float(std::fma(rev, Lfs(base, kCountScale), Lfs(base, kOne)))));
}

bool DrawReady(uint8_t* base, uint32_t car) {
  return SegmentCount(base, car) <= 32;
}

void Draw(uint8_t* base, uint32_t frame, double x, double y, uint32_t car,
          const LineCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8217C708 PowerGraph::Draw");
  const uint32_t torque_set = frame + kTorqueSprites;
  const uint32_t power_set = frame + kPowerSprites;
  // Sprite i of a set.
  auto at = [&](uint32_t set, uint32_t i) { return Sprite(base, set + Sprite::kSize * i); };
  for (uint32_t i = 0; i < 32; i++) {
    race_overlays::SpriteResetStyle2(base, at(torque_set, i).address);
  }
  for (uint32_t i = 0; i < 32; i++) {
    race_overlays::SpriteResetStyle2(base, at(power_set, i).address);
  }

  // The segments' x, y and z (sprites 0 to 30).
  const double zero = Lfs(base, kZero);
  const double step = Lfs(base, kStep);
  for (int32_t i = 0; i < 31; i++) {
    const Sprite a = at(torque_set, uint32_t(i));
    const Sprite b = at(power_set, uint32_t(i));
    StoreS64(base, frame + 80, i);
    StoreS64(base, frame + 88, i + 1);
    double left = ToSingle(i);
    left = double(float(left * step));
    a.set_end0_x(float(double(float(left + x))));
    b.set_end0_x(float(double(float(left + x))));
    double right = ToSingle(i + 1);
    right = double(float(right * step));
    a.set_end1_x(float(double(float(right + x))));
    b.set_end1_x(float(double(float(right + x))));
    a.set_end0_y(float(y));
    b.set_end0_y(float(y));
    a.set_end0_z(float(zero));
    b.set_end0_z(float(zero));
    a.set_end1_y(float(y));
    b.set_end1_y(float(y));
    a.set_end1_z(float(zero));
    b.set_end1_z(float(zero));
  }

  // The 32 points: torque (kgf·m) at +112, power (PS) at +240.
  const schema::CarData car_data(base, car);
  const double scale = double(car_data.torque());
  const double to_kgfm = Lfs(base, kToKgfM);
  const double rpm_step = Lfs(base, kRpmStep);
  const double to_ps = Lfs(base, kToPs);
  for (int32_t i = 0; i < 32; i++) {
    double torque = Lfs(base, car_data.torque_curve_address() + 4 * uint32_t(i));
    torque = double(float(torque * scale));
    torque = double(float(torque * to_kgfm));
    Stfs(base, frame + 112 + 4 * uint32_t(i), torque);
    StoreS64(base, frame + 88, i + 1);
    double rpm = ToSingle(i + 1);
    rpm = double(float(rpm * rpm_step));
    guest::StoreU32(base, frame + 80, uint32_t(Fctiwz(rpm)));
    const int32_t whole = int32_t(guest::LoadU32(base, frame + 80));
    StoreS64(base, frame + 96, whole);
    double power = ToSingle(whole);
    power = double(float(power * torque));
    power = double(float(power * to_ps));
    Stfs(base, frame + 240 + 4 * uint32_t(i), power);
    if (power < zero) {
      Stfs(base, frame + 240 + 4 * uint32_t(i), zero);
    }
  }

  // Points 0 to 25: the heights and colors.
  const double torque_scale = Lfs(base, kTorqueScale);
  const double power_scale = Lfs(base, kPowerScale);
  for (uint32_t i = 0; i < 26; i++) {
    const Sprite a = at(torque_set, i);
    const Sprite b = at(power_set, i);
    const double torque = double(float(Lfs(base, frame + 112 + 4 * i) * torque_scale));
    const double power = double(float(Lfs(base, frame + 240 + 4 * i) * power_scale));
    a.set_end1_y(float(double(float(y - torque))));
    at(torque_set, i + 1).set_end0_y(float(double(float(y - torque))));
    b.set_end1_y(float(double(float(y - power))));
    at(power_set, i + 1).set_end0_y(float(double(float(y - power))));
    // Power cyan, torque orange: end 0's colors, then end 1's.
    b.set_end0_red(44);
    b.set_end0_green(242);
    b.set_end0_blue(242);
    b.set_end0_alpha(255);
    a.set_end0_alpha(255);
    a.set_end0_red(198);
    a.set_end0_green(66);
    a.set_end0_blue(14);
    b.set_end1_red(44);
    b.set_end1_green(242);
    b.set_end1_blue(242);
    b.set_end1_alpha(255);
    a.set_end1_alpha(255);
    a.set_end1_red(198);
    a.set_end1_green(66);
    a.set_end1_blue(14);
  }

  // The count, and segments 1 to n - 1.
  const int32_t count = SegmentCount(base, car);
  guest::StoreU32(base, frame + 80, uint32_t(count));
  for (int32_t i = 1; i < count; i++) {
    const uint32_t a = at(torque_set, uint32_t(i)).address;
    const uint32_t b = at(power_set, uint32_t(i)).address;
    if (!LineRecordFlat(base, a, kPass, kKey, frame - kLineFrame, calls)) {
      calls.original(a, kTorqueLineReturn);
    }
    if (!LineRecordFlat(base, b, kPass, kKey, frame - kLineFrame, calls)) {
      calls.original(b, kPowerLineReturn);
    }
  }

  // The sprites' destructors: the power set, then the torque set, 31 to 0.
  for (const uint32_t set : {power_set, torque_set}) {
    for (int32_t i = 31; i >= 0; i--) {
      const Sprite sprite = at(set, uint32_t(i));
      c_runtime::VectorDestruct(base, sprite.end0_red_address(), 16, 2, kEmptyDestructor, nullptr,
                                nullptr);
      c_runtime::VectorDestruct(base, sprite.end0_x_address(), 16, 2, kEmptyDestructor, nullptr,
                                nullptr);
    }
  }
}

namespace {
constexpr uint32_t kMovieTable = schema::UiMoviesGameGlobals::movie_table_pairs_address();
constexpr uint32_t kShownAfter = 0x8200829C;  // 22.0
constexpr uint32_t kNoTime = 0x82001BE0;      // AnimationTime's answer for none
constexpr uint32_t kTuningX = 0x8200AD98;     // 1042.0
constexpr uint32_t kTuningY = 0x8200AD9C;     // 520.0
}  // namespace

bool TuningScreenShowsGraph(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* r3,
                            uint32_t* movie_out) {
  ITC_NATIVE_RUNNING("sub_821DF350 TuningScreen::DrawPowerGraph");
  guest::StoreU32(base, frame + 80, 2);
  const uint32_t entry = ui_movies::PairsFind(base, kMovieTable, frame + 80);
  *r3 = entry;
  const uint32_t movie = guest::LoadU32(base, entry);
  *movie_out = movie;
  const schema::TuningScreen s(base, screen);
  const int32_t state = int32_t(s.state());
  const bool idle = s.category() == 0;
  const uint8_t step = s.item();
  if (state == 5) {
    *r3 = movie;
    auto time = [&] {
      return double(std::bit_cast<float>(ui_movies::AnimationTime(base, movie, 0xFFFFFFFFu, 5)));
    };
    if (!(time() > Lfs(base, kShownAfter)) && !(time() == Lfs(base, kNoTime))) {
      return false;
    }
    return idle && step <= 2;
  }
  if (state == 6) {
    return idle && step == 2;
  }
  return false;
}

TuningDraw TuningScreenDraw(uint8_t* base, uint32_t screen) {
  TuningDraw draw;
  draw.r3 = schema::StandHolder(base, G::stand_holder(base)).stage_block_0();
  draw.x = Lfs(base, kTuningX);
  draw.y = Lfs(base, kTuningY);
  draw.car = schema::TuningScreen(base, screen).car_data_address();
  return draw;
}

namespace {
constexpr uint32_t kEntryX = 0x8200AD98;  // 1042.0
constexpr uint32_t kEntryY = 0x82001FC0;  // 500.0
// CarData::CarData's frame (below the caller's r1).
constexpr uint32_t kCarDataFrame = 96;
}  // namespace

bool CarListEntrySetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t entry,
                       uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82250670 CarList::DrawEntryPowerGraph");
  if (G::graph_shown(base) == 0) {
    return false;
  }
  const uint32_t car = frame + kEntryCar;
  const uint32_t data = frame + kEntryCarData;
  car_makers::GarageCarReset(base, calls, car, frame);
  career::CarSetModelAndClass(base, car, schema::CarListEntry(base, entry).car_id(), frame);
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  car_makers::FromGarageCar(base, calls, car, data, frame, &f1);
  return true;
}

TuningDraw CarListEntryDraw(uint8_t* base, uint32_t frame) {
  TuningDraw draw;
  draw.r3 = schema::StandHolder(base, G::stand_holder(base)).stage_block_0();
  draw.x = Lfs(base, kEntryX);
  draw.y = Lfs(base, kEntryY);
  draw.car = frame + kEntryCarData;
  return draw;
}

bool CarListCarSetUp(uint8_t* base, const front_end_car_draw::Calls& calls, uint32_t car,
                     uint32_t frame) {
  ITC_NATIVE_RUNNING("sub_82250700 CarList::DrawCarPowerGraph");
  if (G::graph_shown(base) == 0) {
    return false;
  }
  const uint32_t data = frame + kEntryCarData;
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  car_makers::FromGarageCar(base, calls, car, data, frame, &f1);
  return true;
}

namespace {

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kRefresh = 0x820C4998;       // Car::Refresh
constexpr uint32_t kSetupFromCar = 0x820C27E8;  // Vehicle::SetupFromCar
constexpr uint32_t kSumScale = 0x820021E8;
// Flush-to-zero off, as the car's plain C++ expects at entry.
void FlushOff() {
  using Fpscr = ::base::FloatMode;
  Fpscr::setcsr(Fpscr::getcsr() & ~uint32_t(Fpscr::FlushMask));
}
uint32_t Fctiwz32(double value) {
  return uint32_t(Fctiwz(value));
}

}  // namespace

uint32_t SetFromCar(uint8_t* base, uint32_t car, uint32_t tab, uint32_t sp,
                    const SetFromCarCall& call) {
  ITC_NATIVE_RUNNING("sub_82250488 PowerGraph::SetFromCar");
  const uint32_t frame = sp - kSetFromCarFrame;
  guest::StoreU32(base, frame, sp);  // the back chain (stwu)
  const auto record = schema::PowerGraphScreenGlobals::data(base);
  const uint32_t object = frame + 3008, data = frame + 1152, curve = frame + 80;
  FlushOff();
  race_setup::CarInit(base, object);
  if (!c_runtime::Memcpy(base, object, car, 1884)) {
    call(kMemcpy, object, car, 1884, frame, 0x822504C4);
  }
  FlushOff();
  uint32_t refreshed = 0;
  if (!car_specs::CarRefresh(base, object, &refreshed)) {
    call(kRefresh, object, 0, 0, frame, 0x822504CC);
  }
  FlushOff();
  const uint32_t spec = vehicle_accessors::ObjectRecord1888(object);
  car_physics::CarDataConstruct(base, data, frame - kCarDataFrame);
  double f1 = 0.0;
  if (!car_physics::VehicleSetupFromCar(base, data, object, frame - 368, &f1)) {
    call(kSetupFromCar, data, object, 0, frame, 0x822504EC);
  }
  FlushOff();
  vehicle::PowerCurveFromEngine(base, curve, data + 1268, frame - 112);
  vehicle::PowerCurveShiftPeak(base, curve, 0, frame);
  FlushOff();
  const schema::CarMakersCar head(base, car);
  const schema::GarageCar garage_car(base, car);
  const schema::CarData d(base, data);
  const schema::CarSpecCopies s(base, spec);
  const schema::PowerCurve c(base, curve);
  record.set_tab(tab);
  for (uint32_t i = 0; i < 5; i++) {
    guest::StoreU32(base, record.parts_address() + 4 * i,
                    guest::LoadU32(base, head.setting_bytes_address() + 4 * i));
  }
  record.set_car_value(Fctiwz32(double(garage_car.racer_float())));
  const double sum = double(float(double(schema::CarDataAxle(base, d.front_axle_address()).wheel_mass()) +
                                  double(schema::CarDataAxle(base, d.rear_axle_address()).wheel_mass())));
  record.set_rating(Fctiwz32(double(float(std::fma(sum, Lfs(base, kSumScale), double(d.mass()))))));
  record.set_spec_word_0(s.unknown_76());
  record.set_spec_word_1(s.unknown_84());
  record.set_spec_word_2(s.unknown_80());
  record.set_car_id(head.car_id());
  record.set_spec_byte_0(s.spec_byte_129());
  record.set_spec_byte_1(s.unknown_130());
  // The switch on s byte +68 (cases 0 and 4 the split-off sub_822505CC).
  static constexpr uint32_t kKinds[8] = {6, 2, 3, 4, 6, 0, 1, 5};
  const uint8_t kind = s.engine_type();
  if (kind <= 7) {
    record.set_engine_class(kKinds[kind]);
  }
  record.set_spec_flag(s.turbo() != 0 ? 1 : 0);
  record.set_spec_word_3(s.displacement());
  record.set_curve_value_0(Fctiwz32(double(c.most_power())));
  record.set_curve_value_1(Fctiwz32(double(c.most_power_rpm())));
  record.set_peak_torque(float(double(float(double(c.torque_scale()) * Lfs(base, kToKgfM)))));
  record.set_peak_index(Fctiwz32(double(c.peak_rpm())));
  return curve;
}

}  // namespace power_graph
