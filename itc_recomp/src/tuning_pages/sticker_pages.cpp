// The tuning screen's sticker editor (states 38 to 46 and 49 of
// TuningScreen::HandleInput), its movie sides and helpers
// (docs/functions/sticker-editor.md): see tuning_pages.h. None of it was
// seen in play; ported to leave as little recompiled as possible (the
// user's call, 2026-11-12), with the originals' loads and stores in their
// order and their quirks kept.

#include "tuning_pages/tuning_pages.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "schema/action_mappers.h"
#include "schema/career.h"
#include "schema/menu_helpers.h"
#include "schema/racer_controllers.h"
#include "schema/sticker_editor.h"
#include "schema/tuning_pages.h"
#include "schema/tuning_screen.h"
#include "schema/ui_movies_game.h"
#include "mode_tick/mode_tick.h"
#include "native/buffers.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

#include <bit>
#include <cmath>
#include <cstdint>

namespace tuning_pages {

namespace {

namespace fs = strings_and_font::steps;

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself
constexpr uint32_t kStickerMovie = 14;
// The image words and tables (T, G, the layouts, the bounds, the constants).
using G = schema::StickerEditorGlobals;

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint32_t s) { return schema::TuningScreen(s); }

// A field's address as the originals work it out from all 64 bits of the
// screen's register: `screen` plus the field's offset.
uint64_t Wide(uint64_t screen, uint32_t field_address) {
  return screen + (field_address - uint32_t(screen));
}

// The players' action mapper entries (racer-controllers.toml's
// PlayerMapperEntry: the mapper, blocked when +4 is 1) and their indices (a
// word a player).
using Controllers = schema::RacerControllersGlobals;
using Entry = schema::PlayerMapperEntry;
constexpr uint32_t kHeldSlot = 0x8210BE70;     // ActionMapper::Held (slot 8, +32)
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed (slot 10, +40)
constexpr uint32_t kFiredSlot = 0x8210C010;    // ActionMapper::Fired (slot 12, +48)

// The player's mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t PlayerEntry() {
  return (Load(Controllers::player_entries_address()) << 3) +
         Controllers::mapper_entries_address();
}

// The mapper's read slot at `offset` on `action`, as the original's bctrl
// from `frame` returning to `lr`: ActionMapper::Held's, Pressed's or Fired's
// plain C++ when the vtable holds it and `native_slots` (logged for
// --log_pause_menu as their adapters do), else the slot through `call`. Its
// r3.
uint64_t ReadSlot(const tuning_screen::GuestCall& call, uint32_t mapper,
                  uint32_t offset, uint64_t action, uint32_t frame, uint32_t lr,
                  bool native_slots) {
  const uint32_t function = Load(Load(mapper) + offset);
  if (native_slots) {
    const uint32_t id = uint32_t(action);
    uint32_t r3;
    int slot;
    if (function == kHeldSlot) {
      r3 = action_mappers::Held(mapper, id);
      slot = 8;
    } else if (function == kPressedSlot) {
      r3 = action_mappers::Pressed(mapper, id);
      slot = 10;
    } else if (function == kFiredSlot) {
      r3 = action_mappers::Fired(mapper, id);
      slot = 12;
    } else {
      return call(function, {mapper, action}, frame, lr);
    }
    debug::PauseMenuLog_NoteSlotRead(slot, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

// Whether the player's mapper reads `action` set on the slot at `offset`
// (skipped, false, while the entry's +4 is 1).
bool Asked(const tuning_screen::GuestCall& call, uint32_t offset,
           uint64_t action, uint32_t frame, uint32_t lr, bool native_slots) {
  const Entry entry(PlayerEntry());
  if (int32_t(entry.unread()) == 1) {
    return false;
  }
  return (ReadSlot(call, entry.mapper().address, offset, action, frame, lr, native_slots) &
          0xFF) != 0;
}

// UiMovie::ShowNode and HideNode (sub_8211AC00, sub_8211AC48): the node's
// +80 set; the node.
uint32_t SetShown(uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const schema::UiMovieNode node(ui_movies::FindNode(movie, sub_scene, child));
  node.set_visible(shown);
  return node.address;
}

// T[id] is 1 (id below 61, signed; the word read at 0x8200B630 + 4 × id).
bool OnSecondNode(uint32_t id) {
  return int32_t(id) < 61 && int32_t(Load(G::second_node_address() + (id << 2))) == 1;
}

// A doubleword at `at` (an int sign-extended through the stack for fcfid).
void StoreDoubleword(uint32_t at, int64_t value) {
  Store(at, uint32_t(uint64_t(value) >> 32));
  Store(at + 4, uint32_t(value));
}

constexpr uint32_t kStageCommand = 0x82180108;  // GarageStage::Command(holder, command)
constexpr uint32_t kUnloopNamed = 0x8211DE30;   // UiMovie::UnloopNamed(movie, named)
constexpr uint32_t kUnloop = 0x8211DE28;        // UiMovie::Unloop(movie, sub, index)
// G's record g (G::groups(): 6 records of 12 bytes, +0 the first id, +4 the last).
schema::StickerGroup Group(uint32_t g) {
  return schema::StickerGroup(g * 12 + G::groups_address());
}

// The sticker menu [+10044].
schema::FrontEndMenu Menu(uint32_t s) { return Screen(s).sticker_menu(); }

// The sticker menu [+10044]'s cursor.
uint32_t Cursor(uint32_t s) { return Menu(s).cursor(); }

// id(j): the placed sticker of slot j in the area: s + 33320 + 20a + 4j.
uint32_t IdAt(uint32_t s, uint32_t area, uint32_t j) {
  return Screen(s).sticker_ids_address() + ((area * 5 + j) << 2);
}
uint32_t PlacedId(uint32_t s, uint32_t j) {
  return Load(IdAt(s, Screen(s).editor_page(), j));
}

// The calls on movie 14 the movie sides make from `frame`.
struct Movie {
  const tuning_screen::GuestCall& call;
  uint32_t movie;
  uint32_t frame;
  uint64_t Finish(uint64_t named, uint32_t lr) {
    return call(tuning_screen::kFinishNamed, {movie, named}, frame, lr);
  }
  uint64_t Unloop(uint64_t named, uint32_t lr) {
    return call(kUnloopNamed, {movie, named}, frame, lr);
  }
  // FinishNamed then UnloopNamed.
  uint64_t Stop(uint64_t named, uint32_t finish_lr, uint32_t unloop_lr) {
    Finish(named, finish_lr);
    return Unloop(named, unloop_lr);
  }
  uint64_t Play(uint64_t named, uint32_t lr, uint64_t r5 = 1, uint64_t r6 = 0) {
    return call(tuning_screen::kPlayNamed, {movie, named, r5, r6, 0, 0}, frame, lr);
  }
  uint64_t Loop(uint64_t named, uint32_t lr) {
    return call(tuning_screen::kLoopNamed, {movie, named}, frame, lr);
  }
  // PlayNamed(named, 1, 0, 0, 0) then LoopNamed.
  uint64_t PlayLooped(uint64_t named, uint32_t play_lr, uint32_t loop_lr) {
    Play(named, play_lr);
    return Loop(named, loop_lr);
  }
  // SetText(sub, child, id, argument) with r8 to r10 0.
  uint64_t Text(uint64_t sub, uint64_t child, uint64_t id, uint64_t argument, uint32_t lr) {
    return call(tuning_screen::kSetText, {movie, sub, child, id, argument, 0, 0, 0}, frame, lr);
  }
  // The help line (sub-scene 2, child 0): its scroll reset, SetText, and
  // ScrollText(.., 1).
  uint64_t Help(uint64_t id, const uint32_t (&lr)[3]) {
    call(tuning_screen::kResetTextScroll, {movie, 2, 0}, frame, lr[0]);
    Text(2, 0, id, 0, lr[1]);
    return call(tuning_screen::kScrollText, {movie, 2, 0, 1}, frame, lr[2]);
  }
};

// The hold toggles: the flag at `at` 0 -> 1 with `action` held, 1 -> 0
// without.
uint32_t UpdateHold(const tuning_screen::GuestCall& call, uint32_t sp,
                    bool native_slots, uint32_t action, uint32_t at, uint32_t lr) {
  const uint32_t frame = Frame(sp, 96);
  if (Asked(call, 32, action, frame, lr, native_slots)) {
    if (int32_t(Load(at)) == 0) {
      Store(at, 1);
      return 1;
    }
  } else if (int32_t(Load(at)) == 1) {
    Store(at, 0);
    return 1;
  }
  return 0;
}

// The pages' calls and constants.
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMenuStep = 0x82186568;       // Menu::Step(menu)
constexpr uint32_t kMenuPressed = 0x821864C8;    // Menu::Pressed(menu, action)
constexpr uint32_t kMenuSoundPlay = 0x821103F0;  // MenuSound::Play(cue)
constexpr uint32_t kCameraCommand = 0x821808F0;  // GarageStage::CameraCommand(holder, command)
constexpr uint32_t kMemcpy = 0x825A7410;
// l is the screen's layer_cursor (the slot), D its edited_x to edited_word.

// The stand holder [[0x826D83E0]] (tuning-screen.toml's StandHolder's +0).
uint32_t Holder() {
  return schema::StandHolder(G::stand_holder().address).stage_block_0();
}

// The page's reads of the player's mapper entry, each skipped while its +4
// is 1; the last read's r3 kept.
struct Reads {
  const tuning_screen::GuestCall& call;
  uint32_t entry;
  uint32_t frame;
  bool native_slots;
  uint64_t r3;
  bool operator()(uint32_t offset, uint64_t action, uint32_t lr) {
    const Entry e(entry);
    if (int32_t(e.unread()) == 1) {
      return false;
    }
    r3 = ReadSlot(call, e.mapper().address, offset, action, frame, lr, native_slots);
    return (r3 & 0xFF) != 0;
  }
};

// Queue(st, c): q+8 = q+0 when q+0 isn't st, then q+4 = st, q+16 = -1,
// q+12 = c, q+0 = -1 (q = [+10036]; the originals' store orders vary,
// nothing reads between them).
void Queue(uint32_t s, uint32_t state, uint32_t command) {
  const schema::TuningCommandQueue queue = Screen(s).queue();
  const uint32_t current = queue.page();
  if (int32_t(state) != int32_t(current)) {
    queue.set_previous_page(current);
  }
  queue.set_resume(state);
  queue.set_next(kNone);
  queue.set_current(command);
  queue.set_page(kNone);
}

// Go(st, c): +9652 = st, then Queue(st, c).
void Go(uint32_t s, uint32_t state, uint32_t command) {
  Screen(s).set_state(state);
  Queue(s, state, command);
}

// memcpy(destination, source, n) from `frame` returning to `lr`: its plain
// C++ where Buffers_Memcpy takes it, else the guest's. Its r3.
uint64_t Memcpy(const tuning_screen::GuestCall& call, uint64_t destination,
                uint64_t source, uint32_t n, uint32_t frame, uint32_t lr) {
  if (native::Buffers_Memcpy(uint32_t(destination), uint32_t(source), n, lr)) {
    return destination;
  }
  return call(kMemcpy, {destination, source, n}, frame, lr);
}

// rec(l) as the originals work it out: (5a + l) × 24 + s + 32360, all 64
// bits of the screen.
uint64_t SlotRecord(uint64_t screen) {
  const schema::TuningScreen sc = Screen(uint32_t(screen));
  return (uint64_t(sc.editor_page()) * 5 + sc.layer_cursor()) * 24 +
         Wide(screen, sc.stickers_address());
}

// A float constant through lfs and stfs (the recompiled conversions).
void CopyFloat(uint32_t from, uint32_t to) {
  const double value = double(std::bit_cast<float>(Load(from)));
  Store(to, std::bit_cast<uint32_t>(float(value)));
}

// D reset: x, y, the flip and +20 0, the angle 0.0 and the scale 1.0.
void ResetDecal(uint32_t s) {
  const schema::TuningScreen sc = Screen(s);
  sc.set_edited_x(0);
  sc.set_edited_y(0);
  sc.set_edited_flip(0);
  sc.set_edited_word(0);
  CopyFloat(G::zero_address(), sc.edited_angle_address());
  CopyFloat(G::one_address(), sc.edited_scale_address());
}

// The pages' start: Menu::Step on the sticker menu, then UpdateSoloHold
// answering 1: ShowSlotNode(s, id(j), j, current) for j 0 to 4 (current
// the cursor or l by `by_slot`).
void StepAndSolo(const tuning_screen::GuestCall& call, uint32_t s, uint32_t frame,
                 uint32_t step_lr, bool by_slot, bool native_slots) {
  call(kMenuStep, {Menu(s).address}, frame, step_lr);
  if (UpdateSoloHold(call, s, frame, native_slots) == 0) {
    return;
  }
  for (uint32_t j = 0; j < 5; j++) {
    const uint32_t current = by_slot ? Screen(s).layer_cursor() : Cursor(s);
    ShowSlotNode(s, PlacedId(s, j), j, current, frame);
  }
}

// The two-button row's side step (m's row 5 with m+36 set) on the word at
// `at` (the screen's layer_cursor or edit_cursor): below 5 -> 5; -1 or +1
// by m+36; below 5 -> 6, above 6 -> 5.
void StepButtons(uint32_t s, uint32_t at) {
  if (int32_t(Load(at)) < 5) {
    Store(at, 5);
  }
  const int32_t side = Menu(s).side();
  int32_t v = int32_t(Load(at));
  v = side == -1 ? v - 1 : v + 1;
  Store(at, uint32_t(v));
  if (v < 5) {
    v = 6;
  }
  Store(at, uint32_t(v));
  if (v > 6) {
    v = 5;
  }
  Store(at, uint32_t(v));
}

// The word at `at` from A on m: the cursor below 5, else 5 unless it's 6.
void PickRow(uint32_t s, uint32_t at) {
  const uint32_t k = Cursor(s);
  if (int32_t(k) < 5) {
    Store(at, k);
  } else if (int32_t(Load(at)) != 6) {
    Store(at, 5);
  }
}

// B on the two-button pages: the word at `at` 6, m+0 = 5, m+4 = the cursor.
void BackToButton(uint32_t s, uint32_t at) {
  Store(at, 6);
  const schema::FrontEndMenu menu = Menu(s);
  const uint32_t k = menu.cursor();
  menu.set_cursor(5);
  menu.set_previous(k);
}

// UpdateSoloHold answering 1: ShowSlotNode(s, id(j), j, l) for j 0 to 4.
// r3 as they leave it.
uint64_t SoloBySlot(const tuning_screen::GuestCall& call, uint32_t s,
                    uint32_t frame, bool native_slots) {
  uint64_t r3 = UpdateSoloHold(call, s, frame, native_slots);
  if (r3 == 0) {
    return r3;
  }
  for (uint32_t j = 0; j < 5; j++) {
    r3 = ShowSlotNode(s, PlacedId(s, j), j, Screen(s).layer_cursor(), frame);
  }
  return r3;
}

// L[i]: the order list's word i (s + 33480 + 4i).
uint32_t OrderAt(uint32_t s, uint32_t i) {
  return Screen(s).sticker_order_address() + (i << 2);
}

// owned(i): i not negative and its bit set in the save data's sticker words.
bool Owned(uint32_t s, uint32_t i) {
  if (int32_t(i) < 0) {
    return false;
  }
  const schema::CareerSave save(Screen(s).save_data().address);
  const uint32_t word =
      Load(save.reward_set_40_address() + (uint32_t(int32_t(i) >> 5) << 2));
  return (word & (1u << (i & 31))) != 0;
}

// The picker's group scans (+30768 the group, G's records): from the
// group's first id up to its last until one is owned, then owned(i) asked
// once more for the id the scan stopped at (one past the last too), else
// the next group. Loop forever when nothing is owned, as the original.
uint32_t ScanGroup(uint32_t s, uint32_t i, uint32_t last) {
  while (int32_t(i) <= int32_t(last)) {
    if (Owned(s, i)) {
      break;
    }
    i += 1;
    last = uint32_t(Group(Screen(s).image_group()).last_id());
  }
  return i;
}

uint32_t ScanBack(uint32_t s) {
  for (;;) {
    const uint32_t g = Screen(s).image_group() - 1;
    Screen(s).set_image_group(g);
    if (int32_t(g) < 0) {
      Screen(s).set_image_group(5);
    }
    const uint32_t i = uint32_t(Group(Screen(s).image_group()).first_id());
    const uint32_t found =
        ScanGroup(s, i, uint32_t(Group(Screen(s).image_group()).last_id()));
    if (Owned(s, found)) {
      return found;
    }
  }
}

uint32_t ScanForward(uint32_t s) {
  uint32_t group = Screen(s).image_group();
  for (;;) {
    // (g + 1) mod 6 as divw leaves it (negative for a negative g + 1).
    const uint32_t n = group + 1;
    const uint32_t g = n - uint32_t(int32_t(n) / 6) * 6;
    Screen(s).set_image_group(g);
    const uint32_t found = ScanGroup(s, uint32_t(Group(g).first_id()),
                                     uint32_t(Group(g).last_id()));
    if (Owned(s, found)) {
      return found;
    }
    group = g;
  }
}

// States 44 to 46's edits of D and their calls' return addresses.
enum class StickerEdit { kMove, kRotate, kScale };

struct StickerEditCalls {
  uint32_t moves[8];  // the fired reads, in the original's order
  uint32_t a, a_sound, b, b_sound, memcpy;
};

constexpr StickerEditCalls kMoveCalls = {
    {0x821F9A38, 0x821F9A64, 0x821F9A90, 0x821F9ABC, 0x821F9AE8, 0x821F9B14, 0x821F9B40,
     0x821F9B6C},
    0x821F9B98, 0x821F9BBC, 0x821F9C0C, 0x821F9C30, 0x821F9C44};
constexpr StickerEditCalls kRotateCalls = {
    {0x821F9DC0, 0x821F9DEC, 0x821F9E18, 0x821F9E44}, 0x821F9E70, 0x821F9E94, 0x821F9EE4,
    0x821F9F08, 0x821F9F1C};
constexpr StickerEditCalls kScaleCalls = {
    {0x821FA048, 0x821FA074, 0x821FA0A0, 0x821FA0CC}, 0x821FA0F8, 0x821FA11C, 0x821FA16C,
    0x821FA190, 0x821FA1A4};


// One step of D: `move` 0 up, 1 down, 2 left, 3 right (the position, inside
// the area's bounds), or 0 left (minus), 1 right (plus) for the angle (1.0,
// wrapped between 0.0 and 359.0) and the scale (clamped).
void StepSticker(uint32_t s, StickerEdit edit, uint32_t move) {
  if (edit == StickerEdit::kMove) {
    const schema::StickerMoveBounds bounds((Screen(s).editor_page() << 4) + G::move_bounds_address());
    const schema::TuningScreen sc = Screen(s);
    const uint32_t at = move < 2 ? sc.edited_y_address() : sc.edited_x_address();
    const bool minus = move % 2 == 0;
    const int32_t limit = move < 2 ? (minus ? bounds.y_min() : bounds.y_max())
                                   : (minus ? bounds.x_min() : bounds.x_max());
    const int32_t value = int32_t(Load(at));
    if (minus ? limit < value : limit > value) {
      Store(at, uint32_t(value) + (minus ? 0xFFFFFFFFu : 1u));
    }
    return;
  }
  const bool plus = move == 1;
  const bool angle = edit == StickerEdit::kRotate;
  const schema::TuningScreen sc = Screen(s);
  const uint32_t at = angle ? sc.edited_angle_address() : sc.edited_scale_address();
  const double old = fs::Lfs(at);
  const double step = fs::Lfs(angle ? G::one_address() : G::scale_step_address());
  const double value = fs::Single(plus ? old + step : old - step);
  fs::Stfs(at, value);
  if (angle) {
    if (plus ? value > fs::Lfs(G::angle_max_address())
             : value < fs::Lfs(G::zero_address())) {
      fs::Stfs(at, fs::Lfs(plus ? G::zero_address() : G::angle_max_address()));
    }
  } else {
    const double limit = fs::Lfs(plus ? G::scale_max_address() : G::scale_min_address());
    if (plus ? value > limit : value < limit) {
      fs::Stfs(at, limit);
    }
  }
}

// InputStickerMove, InputStickerRotate and InputStickerScale.
uint64_t InputStickerTransform(const tuning_screen::GuestCall& call,
                               uint64_t screen, uint32_t sp, const PageFlags& flags,
                               StickerEdit edit) {
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = PlayerEntry();
  Reads asked{call, entry, frame, flags.native_slots, 0};
  asked.r3 = SoloBySlot(call, s, frame, flags.native_slots);
  const StickerEditCalls& lr = edit == StickerEdit::kMove     ? kMoveCalls
                               : edit == StickerEdit::kRotate ? kRotateCalls
                                                              : kScaleCalls;
  static constexpr uint32_t kMoves[8] = {153, 167, 154, 168, 155, 169, 156, 170};
  static constexpr uint32_t kTurns[4] = {155, 169, 156, 170};
  const uint32_t* moves = edit == StickerEdit::kMove ? kMoves : kTurns;
  const uint32_t count = edit == StickerEdit::kMove ? 8 : 4;
  for (uint32_t k = 0; k < count; k++) {
    if (asked(48, moves[k], lr.moves[k])) {
      // Queued moved or not: Command46, 48 or 50.
      StepSticker(s, edit, k / 2);
      const uint32_t command = edit == StickerEdit::kMove     ? 46
                               : edit == StickerEdit::kRotate ? 48
                                                              : 50;
      Queue(s, Screen(s).state(), command);
      return asked.r3;
    }
  }
  const bool a = asked(40, 159, lr.a);
  if (!a && !asked(40, 158, lr.b)) {
    return asked.r3;
  }
  if (int32_t(Screen(s).holding_164()) != 0) {
    return asked.r3;
  }
  // A keeps D, B puts its backup (+33512) back; then the edit page.
  uint64_t r3 = call(kMenuSoundPlay, {a ? 0u : 2u}, frame, a ? lr.a_sound : lr.b_sound);
  if (!a) {
    r3 = Memcpy(call, Wide(screen, Screen(s).edited_x_address()),
                Wide(screen, Screen(s).sticker_backup_address()), 24, frame, lr.memcpy);
  }
  Screen(s).set_command_end_after_setup(1);
  Go(s, 41, 38);
  return r3;
}

}  // namespace

uint32_t UpdateSoloHold(const tuning_screen::GuestCall& call, uint32_t screen,
                        uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821F7298 TuningScreen::UpdateSoloHold");
  return UpdateHold(call, sp, native_slots, 164, Screen(screen).holding_164_address(),
                    0x821F72E8);
}

uint32_t UpdatePreviewHold(const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t sp, bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821F7348 TuningScreen::UpdatePreviewHold");
  return UpdateHold(call, sp, native_slots, 163, Screen(screen).holding_163_address(),
                    0x821F7398);
}

uint64_t StepUnlockedSticker(uint32_t screen, uint64_t id, uint64_t n) {
  ITC_NATIVE_RUNNING("sub_821F73F8 TuningScreen::StepUnlockedSticker");
  // The rest as the original works it: the quotient's low word (zero
  // extended) times 199 taken from all 64 bits of v.
  auto rest = [](uint64_t v) -> uint64_t {
    const int32_t v32 = int32_t(uint32_t(v));
    const uint64_t q = uint32_t(v32 / 199);
    return v - q * 199;
  };
  uint64_t v = id + 199;
  int32_t count = int32_t(uint32_t(n));
  uint64_t r5 = n;
  while (count != 0) {
    v += count < 0 ? uint64_t(-1) : 1;
    const uint64_t r = rest(v);
    bool owned = false;
    if (int32_t(uint32_t(r)) >= 0) {
      const int32_t i = int32_t(uint32_t(r));
      const schema::CareerSave save(Screen(screen).save_data().address);
      const uint32_t word = Load(save.reward_set_40_address() + (uint32_t(i >> 5) << 2));
      owned = (word & (1u << (i & 31))) != 0;
    }
    if (owned) {
      r5 += count < 0 ? 1 : uint64_t(-1);
      count = int32_t(uint32_t(r5));
    }
  }
  return rest(v);
}

uint32_t ShowSlotNode(uint32_t screen, uint32_t id, uint32_t j, uint32_t current,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8070 TuningScreen::ShowSlotNode");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, kStickerMovie));
  const uint32_t first = (j << 1) + 19, second = (j + 10) << 1;
  if (int32_t(Screen(screen).holding_164()) != 0 && int32_t(j) != int32_t(current) &&
      int32_t(current) < 5) {
    SetShown(movie, kAll, first, 0);
    return SetShown(movie, kAll, second, 0);
  }
  if (OnSecondNode(id)) {
    SetShown(movie, kAll, first, 0);
    return SetShown(movie, kAll, second, 1);
  }
  SetShown(movie, kAll, first, 1);
  return SetShown(movie, kAll, second, 0);
}

uint32_t PlaceStickerNode(uint32_t screen, uint32_t id, uint32_t j,
                          uint32_t record, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8160 TuningScreen::PlaceStickerNode");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, kStickerMovie));
  // The selected garage car's layout for the area, copied into the frame.
  const uint32_t car = tuning_screen::SelectedGarageCar(screen);
  const uint32_t area = Screen(screen).editor_page();
  const schema::StickerLayout layout((((Load(car + 224) << 3) + area) << 4) +
                                         G::layouts_address());
  const uint32_t words[4] = {uint32_t(layout.x()), uint32_t(layout.y()),
                             Load(layout.size_address()),
                             Load(layout.position_scale_address())};
  for (uint32_t k = 0; k < 4; k++) {
    Store(frame + 112 + 4 * k, words[k]);
  }
  const bool second = OnSecondNode(id);
  const uint32_t child = second ? (j + 10) << 1 : (j << 1) + 19;
  const schema::UiMovieNode looked(ui_movies::FindNode(movie, kAll, child));
  const schema::UiMovieNode node(ui_movies::FindNode(movie, kAll, child));
  const schema::StickerRecord sticker(record);
  const double scale = fs::Lfs(frame + 124);
  const int64_t ox = int32_t(Load(frame + 112) + (second ? 816 : 824));
  const int64_t oy = int32_t(Load(frame + 116) + (second ? 284 : 252));
  const int64_t x = sticker.x();
  const int64_t y = sticker.y();
  fs::Stfs(node.x_address(),
           fs::Single(std::fma(fs::ToSingle(x), scale, fs::ToSingle(ox))));
  fs::Stfs(node.y_address(),
           fs::Single(std::fma(fs::ToSingle(y), scale, fs::ToSingle(oy))));
  // The conversions' doublewords as each path leaves them.
  if (second) {
    StoreDoubleword(frame + 80, oy);
    StoreDoubleword(frame + 88, ox);
  } else {
    StoreDoubleword(frame + 80, x);
    StoreDoubleword(frame + 88, ox);
  }
  StoreDoubleword(frame + 96, y);
  const double half = fs::Lfs(G::half_address());
  node.set_pivot_x(
      uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(looked.width_address()) * half))));
  node.set_pivot_y(
      uint32_t(fs::Fctiwz(fs::Single(fs::Lfs(looked.height_address()) * half))));
  fs::Stfs(node.rotation_address(), fs::Lfs(sticker.angle_address()));
  const double size = fs::Lfs(frame + 120);
  if (second) {
    fs::Stfs(node.scale_x_address(),
             fs::Single(size * fs::Lfs(sticker.scale_address())));
    fs::Stfs(node.scale_y_address(),
             fs::Single(size * fs::Lfs(sticker.scale_address())));
  } else {
    const double factor = fs::Lfs(G::first_node_scale_address());
    fs::Stfs(node.scale_x_address(),
             fs::Single(fs::Single(size * fs::Lfs(sticker.scale_address())) * factor));
    fs::Stfs(node.scale_y_address(),
             fs::Single(fs::Single(size * fs::Lfs(sticker.scale_address())) * factor));
  }
  looked.set_flip_x(sticker.flip());
  return node.address;
}

uint32_t SetSlot(uint32_t images, uint32_t sub, uint32_t first, uint32_t second,
                 uint32_t first_slot, uint32_t second_slot, uint32_t i, uint32_t flag,
                 uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8868 DecalImages::SetSlot");
  const uint32_t frame = Frame(sp, 176);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, kStickerMovie));
  if (int32_t(i) < 0) {
    SetShown(movie, sub, first, 1);
    const uint32_t node = SetShown(movie, sub, second, 0);
    const schema::UiMovieTexture texture = MovieTexture(movie, first_slot);
    // The last entry's (199) texture header.
    texture.set_texture(DecalEntryAt(images, 199).texture_header());
    return node;
  }
  uint32_t node, slot;
  if (int32_t(flag) == 1) {
    SetShown(movie, sub, first, 1);
    node = SetShown(movie, sub, second, 0);
    slot = first_slot;
  } else {
    SetShown(movie, sub, first, 0);
    node = SetShown(movie, sub, second, 1);
    slot = second_slot;
  }
  const schema::UiMovieTexture texture = MovieTexture(movie, slot);
  texture.set_texture(DecalEntryAt(images, i).texture_header());
  return node;
}

uint64_t SetStickerEditTexts(const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7F20 TuningScreen::SetStickerEditTexts");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Load(tuning_screen::MovieEntry(frame, kStickerMovie));
  auto text = [&](uint32_t child, uint32_t id, uint64_t argument, uint32_t lr) {
    return call(tuning_screen::kSetText, {movie, kAll, child, id, argument, 0, 0, 0}, frame, lr);
  };
  text(41, 462, 0, 0x821F7F68);
  text(43, 463, 0, 0x821F7F7C);
  text(45, 457, 0, 0x821F7F90);
  text(46, 458, 0, 0x821F7FA4);
  text(47, 459, 0, 0x821F7FB8);
  text(36, 8, Screen(screen).edited_x(), 0x821F7FD0);
  text(37, 8, Screen(screen).edited_y(), 0x821F7FE8);
  // The angle truncated through the frame's +80 (fctiwz, stfiwx).
  Store(frame + 80,
        uint32_t(fs::Fctiwz(fs::Lfs(Screen(screen).edited_angle_address()))));
  text(38, 8, Load(frame + 80), 0x821F8010);
  // The scale as a double (f1), its bits in r7 through the frame's +48.
  const double scale = fs::Lfs(Screen(screen).edited_scale_address());
  const uint64_t bits = std::bit_cast<uint64_t>(scale);
  Store(frame + 48, uint32_t(bits >> 32));
  Store(frame + 52, uint32_t(bits));
  text(39, 452, bits, 0x821F8030);
  return text(40, int32_t(Screen(screen).edited_flip()) == 0 ? 1 : 0, 0, 0x821F8054);
}

uint32_t ShowSlotImage(uint32_t screen, uint32_t id, uint32_t j, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA408 TuningScreen::ShowSlotImage");
  const uint32_t flag = OnSecondNode(id) ? 0 : 1;
  return SetSlot(Screen(screen).decal_images_address(), kAll, (j << 1) + 19,
                 (j + 10) << 1, (j + 95) << 1, (j << 1) + 191, id, flag, sp);
}

uint32_t ShowStickerListImage(uint32_t screen, uint32_t id, uint32_t n,
                              uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA460 TuningScreen::ShowStickerListImage");
  const uint32_t flag = OnSecondNode(id) ? 0 : 1;
  if (int32_t(n) == 0) {
    return SetSlot(Screen(screen).decal_images_address(), 30, 14, 15, 173, 174, id,
                   flag, sp);
  }
  return SetSlot(Screen(screen).decal_images_address(), 30, n * 5 + 14, (n + 3) * 5,
                 (n + 87) << 1, (n << 1) + 175, id, flag, sp);
}

uint64_t AnimateStickerAreas(const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA508 TuningScreen::AnimateStickerAreas");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 144);
  const uint32_t entry = tuning_screen::MovieEntry(frame, kStickerMovie);
  const uint32_t k = Cursor(s);
  Movie m{call, Load(entry), frame};
  uint64_t r3 =
      call(kStageCommand, {Holder(), uint64_t(int64_t(int32_t(k)) + 42)},
           frame, 0x821FA558);
  for (uint32_t i = 6; i < 14; i++) {
    r3 = m.Stop(i, 0x821FA56C, 0x821FA578);
  }
  if (int32_t(show) == 0) {
    return r3;
  }
  if (int32_t(Screen(s).no_car_id()) != 0 && (int32_t(k) == 6 || int32_t(k) == 7)) {
    m.Play(30, 0x821FA5CC, 0, 0);
    m.Finish(30, 0x821FA5D8);
    m.Play(31, 0x821FA5F4, 1, 1);
    m.Finish(31, 0x821FA600);
  } else {
    const uint64_t named = uint64_t(int64_t(int32_t(k)) + 6);
    m.PlayLooped(named, 0x821FA624, 0x821FA630);
  }
  for (uint32_t j = 0; j < 5; j++) {
    r3 = ShowSlotImage(s, kAll, j, frame);
  }
  return r3;
}

uint64_t AnimateStickerOperate(const tuning_screen::GuestCall& call,
                               uint32_t screen, uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F7518 TuningScreen::AnimateStickerOperate");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 144);
  Movie m{call, Load(tuning_screen::MovieEntry(frame, kStickerMovie)), frame};
  const uint32_t k = Cursor(s);
  for (uint32_t i = 49; i < 53; i++) {
    m.Stop(i, 0x821F7564, 0x821F7570);
  }
  if (int32_t(show) == 0) {
    // l + 173 passed as Unloop's sub-scene index, as written.
    m.Finish(uint64_t(int64_t(int32_t(Screen(s).layer_cursor())) + 173), 0x821F7594);
    return call(kUnloop,
                {m.movie, uint64_t(int64_t(int32_t(Screen(s).layer_cursor())) + 173), 1},
                frame, 0x821F75A8);
  }
  const uint64_t named = uint64_t(int64_t(int32_t(k)) + 49);
  m.PlayLooped(named, 0x821F75C8, 0x821F75D4);
  return m.Help(uint64_t(int64_t(int32_t(k)) + 827), {0x821F75E4, 0x821F75F8, 0x821F760C});
}

uint64_t AnimateStickerSlots(const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8A50 TuningScreen::AnimateStickerSlots");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 144);
  Movie m{call, Load(tuning_screen::MovieEntry(frame, kStickerMovie)), frame};
  const uint32_t k = Cursor(s);
  uint64_t r3 = 0;
  for (uint32_t i = 168; i < 173; i++) {
    m.Stop(i, 0x821F8A9C, 0x821F8AA8);
  }
  m.Stop(46, 0x821F8AC0, 0x821F8ACC);
  m.Stop(178, 0x821F8AD8, 0x821F8AE4);
  m.Stop(44, 0x821F8AF0, 0x821F8AFC);
  r3 = m.Stop(45, 0x821F8B08, 0x821F8B14);
  if (int32_t(show) == 0) {
    return r3;
  }
  if (int32_t(k) < 5) {
    m.PlayLooped(uint64_t(int64_t(int32_t(k)) + 168), 0x821F8B44, 0x821F8B50);
    m.Play(46, 0x821F8B6C);
    m.Loop(46, 0x821F8BF0);
  } else if (int32_t(Screen(s).layer_cursor()) <= 5) {
    m.PlayLooped(178, 0x821F8B88, 0x821F8B94);
    m.Play(44, 0x821F8BB0);
    m.Loop(44, 0x821F8BF0);
  } else {
    m.PlayLooped(178, 0x821F8BBC, 0x821F8BC8);
    m.Play(45, 0x821F8BE4);
    m.Loop(45, 0x821F8BF0);
  }
  for (uint32_t j = 0; j < 5; j++) {
    r3 = ShowSlotNode(s, PlacedId(s, j), j, k, frame);
  }
  return r3;
}

uint64_t AnimateStickerEditMenu(const tuning_screen::GuestCall& call,
                                uint32_t screen, uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8D68 TuningScreen::AnimateStickerEditMenu");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 160);
  Movie m{call, Load(tuning_screen::MovieEntry(frame, kStickerMovie)), frame};
  const uint32_t k = Cursor(s);
  for (uint32_t i = 130; i < 135; i++) {
    m.Stop(i - 38, 0x821F8DB8, 0x821F8DC4);
    m.Stop(i, 0x821F8DD0, 0x821F8DDC);
  }
  m.Stop(98, 0x821F8DF4, 0x821F8E00);
  m.Stop(99, 0x821F8E0C, 0x821F8E18);
  m.Stop(136, 0x821F8E24, 0x821F8E30);
  uint64_t r3 = m.Stop(137, 0x821F8E3C, 0x821F8E48);
  if (int32_t(show) == 0) {
    m.Finish(uint64_t(int64_t(int32_t(Screen(s).layer_cursor())) + 173), 0x821F8E60);
    return m.Unloop(uint64_t(int64_t(int32_t(Screen(s).layer_cursor())) + 173), 0x821F8E70);
  }
  const int64_t ks = int32_t(k);
  if (ks < 5) {
    m.PlayLooped(uint64_t(ks + 92), 0x821F8E98, 0x821F8EA4);
    m.Play(uint64_t(ks + 130), 0x821F8EC4);
    m.Loop(uint64_t(ks + 130), 0x821F8F4C);
  } else if (int32_t(Screen(s).edit_cursor()) <= 5) {
    m.PlayLooped(98, 0x821F8EE0, 0x821F8EEC);
    m.Play(136, 0x821F8F08);
    m.Loop(136, 0x821F8F4C);
  } else {
    m.PlayLooped(99, 0x821F8F18, 0x821F8F24);
    m.Play(137, 0x821F8F40);
    m.Loop(137, 0x821F8F4C);
  }
  uint64_t id;
  if (ks < 4) {
    id = uint64_t(ks + 833);
  } else if (ks == 4) {
    id = 838;
  } else {
    id = int32_t(Screen(s).edit_cursor()) > 5 ? 840 : 839;
  }
  m.Help(id, {0x821F8F5C, 0x821F8F9C, 0x821F8FB0});
  r3 = SetStickerEditTexts(call, s, frame);
  return r3;
}

uint32_t ShowStickerOrder(uint32_t screen, uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FBA40 TuningScreen::ShowStickerOrder");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 144);
  const uint32_t entry = tuning_screen::MovieEntry(frame, kStickerMovie);
  const uint32_t movie = Load(entry);
  if (int32_t(show) == 0) {
    return entry;
  }
  const uint32_t k = Cursor(s);
  SetShown(movie, 30, (Screen(s).order_position() << 1) + 47, int32_t(k) > 0 ? 1 : 0);
  SetShown(movie, 30, (Screen(s).order_position() + 24) << 1, int32_t(k) < 4 ? 1 : 0);
  uint32_t r3 = 0;
  for (uint32_t j = 0; j < 5; j++) {
    const uint32_t order = Load(OrderAt(s, j));
    const uint32_t id = Load(IdAt(s, Screen(s).editor_page(), order));
    ShowSlotImage(s, id, j, frame);
    const uint32_t record =
        (Screen(s).editor_page() * 5 + Load(OrderAt(s, j))) * 24 +
        Screen(s).stickers_address();
    PlaceStickerNode(s, id, j, record, frame);
    r3 = ShowSlotNode(s, id, j, k, frame);
  }
  return r3;
}

uint64_t ShowStickerPicker(const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t show, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FBB60 TuningScreen::ShowStickerPicker");
  const uint32_t s = screen;
  const uint32_t frame = Frame(sp, 160);
  Movie m{call, Load(tuning_screen::MovieEntry(frame, kStickerMovie)), frame};
  if (int32_t(show) == 0) {
    m.Stop(uint64_t(int64_t(int32_t(Screen(s).layer_cursor())) + 173), 0x821FBBA8,
           0x821FBBB8);
    return m.Stop(159, 0x821FBBC4, 0x821FBBD0);
  }
  const uint32_t k = Cursor(s);
  ShowSlotImage(s, k, Screen(s).layer_cursor(), frame);
  PlaceStickerNode(s, k, Screen(s).layer_cursor(), Screen(s).edited_x_address(),
                   frame);
  for (uint32_t t = 0; t < 6; t++) {
    const uint64_t i = StepUnlockedSticker(s, Cursor(s), uint64_t(int64_t(t) - 2));
    const uint32_t flag = OnSecondNode(uint32_t(i)) ? 0 : 1;
    SetSlot(Screen(s).decal_images_address(), 29, 22 + 4 * t, 23 + 4 * t, 159 + 2 * t,
            160 + 2 * t, uint32_t(i), flag, frame);
  }
  const uint32_t cursor = Cursor(s);
  const int64_t c = int32_t(cursor);
  m.Text(29, 1, uint64_t(c + 1447), 0, 0x821FBCA0);
  // +30768: the first group whose last id isn't below the cursor, else 5.
  Screen(s).set_image_group(5);
  for (uint32_t g = 0; g < 6; g++) {
    if (int32_t(cursor) <= Group(g).last_id()) {
      Screen(s).set_image_group(g);
      break;
    }
  }
  m.Text(29, 9, uint64_t(int64_t(int32_t(Screen(s).image_group())) + 1242), 0, 0x821FBCF0);
  m.Text(29, 6, 449, 0, 0x821FBD04);
  m.Text(29, 7, 8, uint64_t(c + 1), 0x821FBD1C);
  return m.Text(29, 8, 8, 199, 0x821FBD34);
}

uint64_t InputStickerAreas(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags&) {
  ITC_NATIVE_RUNNING("sub_821FBD40 TuningScreen::InputStickerAreas");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 96);
  call(kMenuStep, {Menu(s).address}, frame, 0x821FBD5C);
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821FBD74);
    Queue(s, Screen(s).state(), 33);
    return r3;
  }
  uint64_t r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821FBDB0);
  if (int32_t(r3) != 0) {
    // A: rows 6 and 7 refused without the garage car's id.
    const int32_t k = int32_t(Cursor(s));
    if (int32_t(Screen(s).no_car_id()) != 0 && (k == 6 || k == 7)) {
      return call(kMenuSoundPlay, {3}, frame, 0x821FBDEC);
    }
    call(kMenuSoundPlay, {0}, frame, 0x821FBDF8);
    const uint32_t area = Cursor(s);
    Screen(s).set_layer_cursor(0);
    Screen(s).set_state(39);
    Screen(s).set_holding_164(0);
    Screen(s).set_editor_page(area);
    Queue(s, 39, 34);
    AnimateStickerAreas(call, s, 0, frame);
    r3 = RestoreAreaDecals(call, screen, Screen(s).editor_page(), frame);
    Screen(s).set_stand_action(1);
    return r3;
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821FBE7C);
  if (int32_t(r3) == 0) {
    return r3;
  }
  // B: state 61 (leaving the editor) with no command, as the vinyl
  // editor's state 23, and the fade out.
  call(kMenuSoundPlay, {2}, frame, 0x821FBE8C);
  schema::TuningCommandQueue queue = Screen(s).queue();
  Screen(s).set_state(61);
  queue.set_resume(61);
  queue.set_page(61);
  queue.set_previous_page(61);
  queue.set_step(kNone);
  queue.set_current(kNone);
  queue.set_started(0);
  queue = Screen(s).queue();
  const uint32_t state = Screen(s).state();
  const uint32_t current = queue.page();
  if (int32_t(state) != int32_t(current)) {
    queue.set_previous_page(current);
  }
  queue.set_page(state);
  queue.set_current(kNone);
  queue.set_next(kNone);
  mode_tick::ScreenFadesFadeOut(15, 0, 0, 0, 0);
  return AnimateStickerAreas(call, s, 0, frame);
}

uint64_t InputStickerSlots(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F9050 TuningScreen::InputStickerSlots");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  StepAndSolo(call, s, frame, 0x821F9068, false, flags.native_slots);
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821F90D0);
    Queue(s, Screen(s).state(), 35);
    return r3;
  }
  if (int32_t(menu.cursor()) == 5 && menu.side() != 0) {
    // The two buttons (l 5 and 6) swapped.
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821F90F4);
    StepButtons(s, Screen(s).layer_cursor_address());
    Queue(s, Screen(s).state(), 35);
    return r3;
  }
  uint64_t r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821F915C);
  if (int32_t(r3) != 0) {
    if (int32_t(Screen(s).holding_164()) != 0) {
      return r3;
    }
    r3 = call(kMenuSoundPlay, {0}, frame, 0x821F9180);
    PickRow(s, Screen(s).layer_cursor_address());
    const int32_t l = int32_t(Screen(s).layer_cursor());
    if (l < 5) {
      if (int32_t(PlacedId(s, uint32_t(l))) == -1) {
        // An empty slot: D reset, the picker (state 43).
        ResetDecal(s);
        Screen(s).set_picked_image(0);
        Screen(s).set_image_group(0);
        Screen(s).set_state(43);
        Screen(s).set_picker_wait(0);
        Queue(s, 43, 43);
      } else {
        // A sticker: its operations (state 40).
        Screen(s).set_operation_cursor(0);
        Go(s, 40, 36);
      }
      return AnimateStickerSlots(call, s, 0, frame);
    }
    // The buttons: the sticker dialog (states 50 and 51).
    Go(s, l == 5 ? 50 : 51, 52);
    return r3;
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821F92C0);
  if (int32_t(r3) == 0) {
    return r3;
  }
  r3 = call(kMenuSoundPlay, {2}, frame, 0x821F92D0);
  BackToButton(s, Screen(s).layer_cursor_address());
  Queue(s, Screen(s).state(), 35);
  return r3;
}

uint64_t InputStickerOperate(const tuning_screen::GuestCall& call, uint64_t screen,
                             uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F9328 TuningScreen::InputStickerOperate");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  StepAndSolo(call, s, frame, 0x821F9340, true, flags.native_slots);
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821F93A4);
    Queue(s, Screen(s).state(), 37);
    return r3;
  }
  uint64_t r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821F93E0);
  if (int32_t(r3) != 0) {
    if (int32_t(Screen(s).holding_164()) != 0) {
      return r3;
    }
    r3 = call(kMenuSoundPlay, {0}, frame, 0x821F9404);
    const int32_t k = int32_t(Cursor(s));
    Screen(s).set_operation_cursor(uint32_t(k));
    if (k == 0) {
      // Edit: the record into D.
      r3 = Memcpy(call, Wide(screen, Screen(s).edited_x_address()),
                  SlotRecord(screen), 24, frame, 0x821F9440);
      Screen(s).set_edit_cursor(0);
      Go(s, 41, 38);
    } else if (k == 1) {
      // Order: the order list 0 to 4.
      Screen(s).set_order_position(Screen(s).layer_cursor());
      for (uint32_t i = 0; i < 5; i++) {
        Store(OrderAt(s, i), i);
      }
      Go(s, 42, 40);
    } else if (k == 2) {
      // Image: D reset, the picker from the placed sticker.
      ResetDecal(s);
      const uint32_t id = PlacedId(s, Screen(s).layer_cursor());
      Screen(s).set_image_group(0);
      Screen(s).set_picked_image(0);
      if (int32_t(id) > -1) {
        Screen(s).set_picked_image(id);
      }
      Screen(s).set_state(43);
      Screen(s).set_picker_wait(0);
      Queue(s, 43, 43);
    } else {
      // Remove: the dialog.
      Screen(s).set_before_state_37(0);
      Go(s, 52, 52);
    }
    if (int32_t(Screen(s).operation_cursor()) == 3) {
      return r3;
    }
    return AnimateStickerOperate(call, s, 0, frame);
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821F9594);
  if (int32_t(r3) == 0 || int32_t(Screen(s).holding_164()) != 0) {
    return r3;
  }
  call(kMenuSoundPlay, {2}, frame, 0x821F95B8);
  Screen(s).set_command_end_after_setup(1);
  Go(s, 39, 34);
  return AnimateStickerOperate(call, s, 0, frame);
}

uint64_t InputStickerEdit(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F9608 TuningScreen::InputStickerEdit");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  StepAndSolo(call, s, frame, 0x821F9620, true, flags.native_slots);
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821F9680);
    Queue(s, Screen(s).state(), 39);
    return r3;
  }
  if (int32_t(menu.cursor()) == 5 && menu.side() != 0) {
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821F96A4);
    StepButtons(s, Screen(s).edit_cursor_address());
    Queue(s, Screen(s).state(), 39);
    return r3;
  }
  uint64_t r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821F970C);
  if (int32_t(r3) != 0) {
    if (int32_t(Screen(s).holding_164()) != 0) {
      return r3;
    }
    call(kMenuSoundPlay, {0}, frame, 0x821F9730);
    PickRow(s, Screen(s).edit_cursor_address());
    // D's backup, put back by states 44 to 46's B.
    Memcpy(call, Wide(screen, Screen(s).sticker_backup_address()),
           Wide(screen, Screen(s).edited_x_address()), 24, frame, 0x821F976C);
    switch (int32_t(Screen(s).edit_cursor())) {
      case 0: Go(s, 44, 45); break;
      case 1: Go(s, 45, 47); break;
      case 2: Go(s, 46, 49); break;
      case 3: {
        // The flip toggled and the node placed again.
        const uint32_t flip = Screen(s).edited_flip();
        Screen(s).set_edited_flip(flip == 0 ? 1 : 0);
        const uint32_t l = Screen(s).layer_cursor();
        PlaceStickerNode(s, PlacedId(s, l), l, Screen(s).edited_x_address(),
                         frame);
        break;
      }
      case 4:
        // The free look (state 49).
        Go(s, 49, 51);
        Screen(s).set_stand_action(3);
        break;
      case 5:
        // The edit kept.
        Screen(s).set_command_end_after_setup(1);
        Go(s, 39, 34);
        Memcpy(call, SlotRecord(screen),
               Wide(screen, Screen(s).edited_x_address()), 24, frame, 0x821F98E8);
        break;
      default:
        Screen(s).set_command_end_after_setup(1);
        Go(s, 40, 36);
        break;
    }
    return AnimateStickerEditMenu(call, s,
                                  int32_t(Screen(s).edit_cursor()) == 3 ? 1 : 0, frame);
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821F9944);
  if (int32_t(r3) == 0) {
    return r3;
  }
  // B (not refused while solo here).
  r3 = call(kMenuSoundPlay, {2}, frame, 0x821F9954);
  BackToButton(s, Screen(s).edit_cursor_address());
  Queue(s, Screen(s).state(), 39);
  return r3;
}

uint64_t InputStickerView(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F7000 TuningScreen::InputStickerView");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 112);
  Reads asked{call, PlayerEntry(), frame, flags.native_slots, screen};
  if (asked(40, 159, 0x821F7054) || asked(40, 158, 0x821F7080)) {
    // A or B: back to the edit page (state 41), the stand's decals applied.
    asked.r3 = call(kMenuSoundPlay, {2}, frame, 0x821F7090);
    Screen(s).set_command_end_after_setup(1);
    Go(s, 41, 38);
    Screen(s).set_stand_action(4);
  }
  // The held buttons: the distance, the turns and the tilt.
  if (asked(32, 163, 0x821F70FC)) {
    asked.r3 = call(kCameraCommand, {Holder(), 4}, frame, 0x821F7114);
  }
  if (asked(32, 164, 0x821F7138)) {
    asked.r3 = call(kCameraCommand, {Holder(), 5}, frame, 0x821F7150);
  }
  if (asked(32, 173, 0x821F7174)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x821F718C);
    asked.r3 = call(kStageCommand, {Holder(), 9}, frame, 0x821F719C);
  }
  if (asked(32, 174, 0x821F71C0)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x821F71D8);
    asked.r3 = call(kStageCommand, {Holder(), 10}, frame, 0x821F71E8);
  }
  if (asked(32, 171, 0x821F720C)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x821F7224);
    asked.r3 = call(kCameraCommand, {Holder(), 0}, frame, 0x821F7234);
  }
  if (asked(32, 172, 0x821F7258)) {
    call(kStageCommand, {Holder(), 1}, frame, 0x821F7270);
    asked.r3 = call(kCameraCommand, {Holder(), 1}, frame, 0x821F7280);
  }
  return asked.r3;
}

uint64_t InputStickerOrder(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821FBF18 TuningScreen::InputStickerOrder");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 272);
  if (int32_t(Screen(s).command_end_after_setup()) != 0) {
    // The command's end asked: the order page again while the slot isn't
    // in its own place, else the operations (state 40).
    const uint32_t moving = Screen(s).order_position();
    if (int32_t(moving) != int32_t(Load(OrderAt(s, moving)))) {
      Queue(s, Screen(s).state(), 42);
    } else {
      Go(s, 40, 36);
    }
    return screen;
  }
  call(kMenuStep, {Menu(s).address}, frame, 0x821FBF94);
  if (UpdateSoloHold(call, s, frame, flags.native_slots) != 0) {
    for (uint32_t j = 0; j < 5; j++) {
      ShowSlotNode(s, PlacedId(s, Load(OrderAt(s, j))), j, Cursor(s),
                   frame);
    }
  }
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    // Moved: L[previous] = L[k], L[k] = the moving slot.
    const uint64_t r3 = call(kMenuSoundPlay, {4}, frame, 0x821FC00C);
    schema::FrontEndMenu m = Menu(s);
    const uint32_t k = m.cursor(), previous = m.previous();
    Store(OrderAt(s, previous), Load(OrderAt(s, k)));
    m = Menu(s);
    const uint32_t moving = Screen(s).order_position();
    Store(OrderAt(s, m.cursor()), moving);
    Queue(s, Screen(s).state(), 41);
    return r3;
  }
  uint64_t r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821FC084);
  if (int32_t(r3) != 0) {
    if (int32_t(Screen(s).holding_164()) != 0) {
      return r3;
    }
    call(kMenuSoundPlay, {0}, frame, 0x821FC0A8);
    // A: the area reordered by L, through copies of its five records (the
    // frame's +112) and ids (+80).
    const uint32_t area = Screen(s).editor_page();
    const uint32_t k = Cursor(s);
    Screen(s).set_order_position(k);
    Screen(s).set_layer_cursor(k);
    const uint32_t records =
        uint32_t(uint64_t(area) * 120 + Wide(screen, Screen(s).stickers_address()));
    for (uint32_t b = 0; b < 120; b += 4) {
      Store(frame + 112 + b, Load(records + b));
    }
    const uint32_t ids =
        uint32_t(uint64_t(area) * 20 + Wide(screen, Screen(s).sticker_ids_address()));
    for (uint32_t b = 0; b < 20; b += 4) {
      Store(frame + 80 + b, Load(ids + b));
    }
    for (uint32_t j = 0; j < 5; j++) {
      const uint32_t id = Load((Load(OrderAt(s, j)) << 2) + frame + 80);
      Store(IdAt(s, Screen(s).editor_page(), j), id);
      const uint64_t destination = (uint64_t(Screen(s).editor_page()) * 5 + j) * 24 +
                                   Wide(screen, Screen(s).stickers_address());
      const uint64_t source = uint64_t(Load(OrderAt(s, j))) * 24 + frame + 112;
      Memcpy(call, destination, source, 24, frame, 0x821FC174);
    }
    Screen(s).set_command_end_after_setup(1);
    Go(s, 39, 34);
    return ShowStickerOrder(s, 0, frame);
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821FC1BC);
  if (int32_t(r3) == 0 || int32_t(Screen(s).holding_164()) != 0) {
    return r3;
  }
  // B: the slot back at the cursor, then as the command's end.
  call(kMenuSoundPlay, {2}, frame, 0x821FC1E0);
  const uint32_t k = Cursor(s);
  Screen(s).set_command_end_after_setup(1);
  Screen(s).set_order_position(k);
  if (int32_t(k) != int32_t(Load(OrderAt(s, k)))) {
    Queue(s, Screen(s).state(), 42);
  } else {
    Go(s, 40, 36);
  }
  return ShowStickerOrder(s, 0, frame);
}

uint64_t InputStickerPicker(const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821FC270 TuningScreen::InputStickerPicker");
  const uint32_t s = uint32_t(screen);
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = PlayerEntry();
  const uint32_t wait = Screen(s).picker_wait_address();
  if (int32_t(Load(wait)) > 0) {
    Store(wait, Load(wait) - 1);
    return screen;
  }
  call(kMenuStep, {Menu(s).address}, frame, 0x821FC2C0);
  uint64_t r3 = UpdateSoloHold(call, s, frame, flags.native_slots);
  if (r3 != 0) {
    for (uint32_t j = 0; j < 5; j++) {
      // The slot being picked shows the cursor's sticker.
      const uint32_t l = Screen(s).layer_cursor();
      const uint32_t id = int32_t(l) == int32_t(j) ? Cursor(s) : PlacedId(s, j);
      r3 = ShowSlotNode(s, id, j, l, frame);
    }
  }
  r3 = UpdatePreviewHold(call, s, frame, flags.native_slots);
  if (r3 != 0) {
    // 163 held: the slot's placed record previewed, else D reset.
    if (int32_t(Screen(s).holding_163()) == 1) {
      Memcpy(call, Wide(screen, Screen(s).edited_x_address()), SlotRecord(screen),
             24, frame, 0x821FC370);
    } else {
      ResetDecal(s);
    }
    r3 = PlaceStickerNode(s, Cursor(s), Screen(s).layer_cursor(),
                          Screen(s).edited_x_address(), frame);
  }
  Reads asked{call, entry, frame, flags.native_slots, r3};
  const bool back = asked(48, 161, 0x821FC3D8);
  if (back || asked(48, 162, 0x821FC51C)) {
    // The group before or after: the cursor on its first owned sticker.
    const uint32_t i = back ? ScanBack(s) : ScanForward(s);
    call(kMenuSoundPlay, {4}, frame, 0x821FC4C8);
    schema::FrontEndMenu m = Menu(s);
    const uint32_t k = m.cursor();
    m.set_cursor(i);
    m.set_previous(k);
    m = Menu(s);
    m.set_up_down_locked(1);
    m.set_left_right_steps(1);
    m.set_shoulders_step(0);
    Store(wait, 2);
    return ShowStickerPicker(call, s, 1, frame);
  }
  const schema::FrontEndMenu menu = Menu(s);
  if (int32_t(menu.moved()) != 0) {
    r3 = call(kMenuSoundPlay, {4}, frame, 0x821FC62C);
    const uint32_t state = Screen(s).state();
    Store(wait, 2);
    Queue(s, state, 44);
    return r3;
  }
  r3 = call(kMenuPressed, {menu.address, 159}, frame, 0x821FC670);
  if (int32_t(r3) != 0) {
    if (int32_t(Screen(s).holding_164()) != 0) {
      return r3;
    }
    // A: the sticker placed in slot l, the edit page (state 41).
    call(kMenuSoundPlay, {0}, frame, 0x821FC694);
    const uint32_t k = Cursor(s);
    Screen(s).set_edit_cursor(0);
    Screen(s).set_state(41);
    Screen(s).set_picked_image(k);
    Queue(s, 41, 38);
    Store(IdAt(s, Screen(s).editor_page(), Screen(s).layer_cursor()),
          Screen(s).picked_image());
    return ShowStickerPicker(call, s, 0, frame);
  }
  r3 = call(kMenuPressed, {Menu(s).address, 158}, frame, 0x821FC708);
  if (int32_t(r3) == 0 || int32_t(Screen(s).holding_164()) != 0) {
    return r3;
  }
  // B: back to the page the picker came from (q+8: 40 or 39).
  call(kMenuSoundPlay, {2}, frame, 0x821FC72C);
  const int32_t from = int32_t(Screen(s).queue().previous_page());
  if (from == 40) {
    Screen(s).set_command_end_after_setup(1);
    Go(s, 40, 36);
  } else if (from == 39) {
    Screen(s).set_command_end_after_setup(1);
    Go(s, 39, 34);
  }
  return ShowStickerPicker(call, s, 0, frame);
}

uint64_t InputStickerMove(const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F99A8 TuningScreen::InputStickerMove");
  return InputStickerTransform(call, screen, sp, flags, StickerEdit::kMove);
}

uint64_t InputStickerRotate(const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F9D30 TuningScreen::InputStickerRotate");
  return InputStickerTransform(call, screen, sp, flags, StickerEdit::kRotate);
}

uint64_t InputStickerScale(const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821F9FB8 TuningScreen::InputStickerScale");
  return InputStickerTransform(call, screen, sp, flags, StickerEdit::kScale);
}

}  // namespace tuning_pages
