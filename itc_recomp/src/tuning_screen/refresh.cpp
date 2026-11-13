// TuningScreen::Refresh (sub_821E3AD0, docs/functions/tuning-screen.md's
// "The refresh"): see tuning_screen.h and refresh.h. Movie 2's nodes through
// the UI movies' plain C++ (FindNode, Child, AnimationTime, ShowNode's and
// HideNode's stores, SetNodeTexture, SetNodeColor's color); the texts
// (SetText, SetNumberText, SetChildNameText) and the help line's scroll
// (sub_8211C5F8, ScrollText) through their overrides from the original's
// frame with its return addresses, the varargs SetText leaves unset passed
// as 0 (the user's call, 2026-10-30); the callees seen in play by their
// plain C++ (EquipChoice, SetCarStats, HideNode19, GetPrice, ShowTireMarks,
// PickCameraPreset, PreviewItem, GarageStage::SetLightsOn, SetStateTexts,
// RefreshMovie13, and since 2026-11-13 RefreshPaint sub_821F2110,
// RefreshLightKits sub_821F2378 and RefreshMovie14 sub_821F7A98, never seen
// in play), the one still recompiled (RefreshInterior sub_821F2AD8) through
// the guest with r3 the screen (all 64 bits) and the original's return
// addresses.

#include "tuning_screen/refresh.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "guest.h"
#include "stand_models/stand_models.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>

namespace tuning_screen {

namespace refresh {

namespace {

namespace steps = camera_modes::steps;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
void Store64(uint8_t* base, uint32_t address, uint64_t value) {
  Store(base, address, uint32_t(value >> 32));
  Store(base, address + 4, uint32_t(value));
}
double Lfd(uint8_t* base, uint32_t address) {
  return std::bit_cast<double>((uint64_t(Load(base, address)) << 32) | Load(base, address + 4));
}

constexpr uint32_t kHolderAt = 0x826D83E0;     // [[it]]: the stand's holder
constexpr uint32_t kPounds = 0x82009F00;       // a double: the weight's unit
constexpr uint32_t kTorqueUnit = 0x82009F08;   // a double: child 12's

// The callees never seen in play, through the guest.
constexpr uint32_t kRefreshInterior = 0x821F2AD8;

}  // namespace

uint32_t Context::Word(uint32_t offset) const { return Load(base, screen + offset); }
uint32_t Context::Byte(uint32_t offset) const { return guest::LoadU8(base, screen + offset); }

uint32_t Context::Node(uint32_t sub_scene, uint32_t child) const {
  return ui_movies::FindNode(base, movie, sub_scene, child);
}
uint32_t Context::ChildOf(uint32_t node, uint32_t i) const {
  return ui_movies::Child(base, node, i);
}
void Context::Shown(uint32_t sub_scene, uint32_t child, uint32_t shown) const {
  Store(base, Node(sub_scene, child) + 80, shown);
}
void Context::Texture(uint32_t sub_scene, uint32_t child, uint64_t texture) const {
  ui_movies::SetNodeTexture(base, Node(sub_scene, child), uint32_t(texture));
}
void Context::Color(uint32_t sub_scene, uint32_t child, uint32_t color) const {
  ui_movies::SetColor(base, Node(sub_scene, child), color);
}
bool Context::TimeIs(uint32_t sub_scene, uint32_t index, uint32_t at) const {
  const double t =
      double(std::bit_cast<float>(ui_movies::AnimationTime(base, movie, sub_scene, index)));
  *f1 = t;
  return t == steps::Lfs(base, at);
}

uint64_t Context::Call(uint32_t function, std::initializer_list<uint64_t> args,
                       uint32_t lr) const {
  return stand.call(function, args, frame, lr);
}
void Context::Text(uint64_t sub_scene, uint32_t child, uint64_t id, uint64_t r7, uint32_t lr,
                   uint64_t r10) const {
  Call(kSetText, {movie, sub_scene, child, id, r7, 0, 0, r10}, lr);
}
void Context::ResetHelp(uint32_t lr) const { Call(kResetTextScroll, {movie, 10, 0}, lr); }
void Context::Help(uint64_t id) const {
  Text(10, 0, id, 0, kHelpTextLr);
  HelpScroll();
}
void Context::HelpScroll() const { Call(kScrollText, {movie, 10, 0, 1}, kHelpScrollLr); }

// Block 1: every state's header, to the switch.
void Context::Header() {
  Call(kSetChildNameText, {movie, 9, 6, self + 20792}, 0x821E3B14);
  Call(kSetNumberText, {movie, 9, 9, Word(25116)}, 0x821E3B28);
  Text(9, 13, 8, Word(25092), 0x821E3B40);
  Texture(9, 11, Word(25096));
  Shown(8, 1, 1);
  Shown(8, 2, 0);
  Shown(8, 10, 0);
  Texture(8, 6, 1);
  Texture(8, 7, 1);
  EquipChoice(base, screen);
  // SetCarStats gets r3 = the screen as EquipChoice (a leaf) leaves it.
  double stats_f1 = 0.0;
  SetCarStats(base, stand.calls, screen, frame, &stats_f1);
  *f1 = stats_f1;
  // Sub-scene 18's stats (SetCarStats' texts again, as the original).
  Text(18, 7, 8, Word(20768), 0x821E3BD0);
  // +20772 made a float through the frame's +80 (std, lfd, fcfid, frsp),
  // scaled by the double at 0x82009F00 (fmul, frsp), back to an int (fctiwz,
  // stfiwx at +80); r10 the frame's +80 (the stfiwx's address, set).
  const int64_t weight = int32_t(Word(20772));
  Store64(base, frame + 80, uint64_t(weight));
  const double single = steps::Single(double(weight));
  const double pounds = steps::Single(single * Lfd(base, kPounds));
  Store(base, frame + 80, uint32_t(steps::Fctiwz(pounds)));
  Text(18, 8, 8, Load(base, frame + 80), 0x821E3C18, frame + 80);
  Text(18, 11, 8, Word(20776), 0x821E3C30);
  // +20780 times the double at 0x82009F08 (frsp into f1), passed as a
  // double in r7 (through the frame's +48) and in f1.
  const double unit = steps::Single(steps::Lfs(base, screen + 20780) * Lfd(base, kTorqueUnit));
  const uint64_t bits = std::bit_cast<uint64_t>(unit);
  Store64(base, frame + 48, bits);
  *f1 = unit;
  Text(18, 12, 436, bits, 0x821E3C60);
  Text(18, 9, 438, 0, 0x821E3C74);
  Text(18, 10, 440, 0, 0x821E3C88);
  Text(18, 13, 437, 0, 0x821E3C9C);
  Text(18, 14, 439, 0, 0x821E3CB0);
  if (State() != 16) {
    Shown(kAll, 19, 1);
  }
  HideNode19(base, self, frame);
  Text(19, 4, 527, 0, 0x821E3CE8);
  // GetPrice's r3 (zero-extended) in r7.
  Text(19, 2, 9, GetPrice(base, screen), 0x821E3D08);
  Text(19, 3, 435, 0, 0x821E3D1C);
  {
    const int32_t state = State();
    const bool bonnet = state >= 7 && state <= 11 && Category() == 4 && Item() == 4;
    if (!bonnet) {
  ShowTireMarks(base, screen, frame);
    }
  }
  Shown(kAll, 20, 0);
  Shown(kAll, 17, 0);
  Shown(kAll, 0, 0);
  PickCameraPreset(base, stand, self, frame);
  PreviewItem(base, stand, self, frame, f1);
  {
    const int32_t state = State();
    const bool headlights = (state == 6 || state == 9 || state == 10 || state == 11) &&
                            Category() == 4 && Item() == 6;
    // f1 the light value on the flag 0 path, left on the other.
    double lights_f1 = 0.0;
    stand_models::SetLightsOn(base, Load(base, Load(base, kHolderAt)), headlights ? 1 : 0,
                              frame, &lights_f1);
    if (!headlights) {
      *f1 = lights_f1;
    }
  }
}

// Block 2: state 3, the categories' page.
void Context::State3() {
  Texture(3, 1, Word(9716) != 0 ? 0 : uint64_t(Word(kCategory)) + 1);
  Texture(3, 2, 0);
  Shown(kAll, 25, 0);
  Shown(kAll, 26, 0);
  Shown(kAll, 19, 0);
  ResetHelp(0x821E3EB8);
  Help(uint64_t(Word(kCategory)) + 596);
}

// Block 3: state 4, the category's items.
void Context::State4() {
  if (Word(9716) != 0) {
    Texture(3, 1, 0);
  } else {
    // By the category (unsigned): none for 7 and above.
    static constexpr int32_t kFirst[7] = {8, 12, -1, 17, 20, 30, 38};
    const uint32_t category = Word(kCategory);
    if (category < 7) {
      Texture(3, 1, category == 2 ? 3 : uint64_t(Item()) + uint64_t(kFirst[category]));
    }
  }
  Texture(3, 2, uint64_t(Word(kCategory)) + 1);
  // The aero's item list (only the aero: the other categories leave at the
  // first time), once the scene's animations 2, 1 and 2 have stopped.
  if (Word(9716) == 0 && TimeIs(kAll, 2, kAtEnd) && Category() == 4 && TimeIs(kAll, 1, kAtEnd) &&
      TimeIs(kAll, 2, kAtEnd)) {
    for (uint32_t n = 5; n < 15; n++) {
      Shown(27, n, 0);
      Color(27, n, 0);
    }
    for (int32_t row = 0; row < int32_t(Byte(9676)); row++) {
      if (row >= 8) {
        continue;
      }
      const uint32_t n = uint32_t(row) + 5;
      Shown(27, n, 1);
      // The rows' item: the item byte - +25100 + the row (64 bits), unchecked.
      Text(27, n, uint64_t(Item()) - uint64_t(Word(25100)) + uint64_t(row) + 203, 0, 0x821E4070);
      uint32_t available = 1;
      if (Category() == 4) {
        const uint64_t k = uint64_t(Item()) - uint64_t(Word(25100)) + uint64_t(row) + 6282;
        available = Load(base, uint32_t(k << 2) + screen);
      }
      if (int32_t(available) == 0) {
        Color(27, n, 2);
      }
    }
  }
  // The scroll arrows (25 and 26) by the aero's first row +25100.
  if (Category() == 4) {
    const uint64_t d = uint64_t(Item()) - uint64_t(Word(25100));
    if (int32_t(d) == 0) {
      Shown(kAll, 25, 1);
      Shown(kAll, 26, 0);
    } else {
      Shown(kAll, 25, int32_t(d + 7) == 9 ? 0 : 1);
      Shown(kAll, 26, 1);
    }
  } else {
    Shown(kAll, 25, 0);
    Shown(kAll, 26, 0);
  }
  Shown(kAll, 19, 0);
  ResetHelp(0x821E415C);
  const uint64_t item = Item();
  switch (Word(kCategory)) {
    case 0: Help(item + 603); break;
    case 1: Help(item + 607); break;
    case 3: Help(item + 612); break;
    case 4:
      if (int32_t(Load(base, uint32_t((item + 6282) << 2) + screen)) != 0) {
        Help(item + 615);
      } else {
        Help(item == 9 ? 857 : 860);
      }
      break;
    case 5: Help(item + 637); break;
    case 6: Help(item + 645); break;
    default: HelpScroll(); break;  // 2, and 7 and above: the scroll alone
  }
}

}  // namespace refresh

uint64_t Refresh(uint8_t* base, const StandCalls& stand, uint64_t r3, uint32_t sp,
                 std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E3AD0 TuningScreen::Refresh");
  const uint32_t frame = sp - refresh::kFrameBytes;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  refresh::Context c{base, stand, r3, uint32_t(r3), frame, guest::LoadU32(base, entry), f1};
  c.Header();
  // The switch on the state - 3 (unsigned, 0 to 55) through the table at
  // 0x8200A908.
  const uint32_t state = c.Word(kState);
  const uint32_t index = state - 3;
  if (index > 55) {
    return 0;
  }
  switch (state) {
    case 3: c.State3(); break;
    case 4: c.State4(); break;
    case 5: c.State5(); break;
    case 6: c.State6(); break;
    case 7: c.State7(); break;
    case 8: c.State8(); break;
    case 9:
    case 11: break;
    case 10:
    case 12:
    case 13:
    case 14: return 0;
    case 15:
    case 16:
    case 17: SetStateTexts(base, stand.call, uint32_t(r3), frame, f1); break;
    case 18: RefreshPaint(base, stand.call, uint32_t(r3), frame); break;
    case 19:
    case 20:
    case 21:
    case 22: RefreshLightKits(base, stand, r3, frame, f1); break;
    default:
      if (state <= 37 && c.Category() == 5 && c.Item() == 6) {
        RefreshMovie13(base, stand.call, uint32_t(r3), frame, f1);
      } else if (state <= 52) {
        RefreshMovie14(base, stand.call, uint32_t(r3), frame);
      } else {
        c.Call(refresh::kRefreshInterior, {r3}, 0x821E6934);
      }
      break;
  }
  return 1;
}

}  // namespace tuning_screen
