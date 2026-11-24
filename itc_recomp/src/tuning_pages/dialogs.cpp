// The confirm dialogs' yes answers and the stickers' records they store
// (docs/functions/tuning-pages.md): see tuning_pages.h. Single-precision steps
// as the originals' (each result rounded to a float); needs flush-to-zero off,
// as the originals' lfs leaves it.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "native/buffers.h"
#include "schema/car_makers.h"
#include "schema/menu_helpers.h"
#include "schema/sticker_editor.h"
#include "schema/tuning_screen.h"
#include "strings_and_font/steps.h"
#include "tuning_screen/tuning_screen.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>
#include <cmath>

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint8_t* base, uint32_t s) { return schema::TuningScreen(base, s); }
// The screen's command queue (tuning-screen.toml's TuningCommandQueue).
schema::TuningCommandQueue CommandQueue(uint8_t* base, uint32_t q) {
  return schema::TuningCommandQueue(base, q);
}
// A front-end menu (menu-helpers.toml's FrontEndMenu).
schema::FrontEndMenu MenuOf(uint8_t* base, uint32_t m) { return schema::FrontEndMenu(base, m); }

using G = schema::TuningPagesGlobals;

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
// stfs: a double rounded to a float.
void Stfs(uint8_t* base, uint32_t address, double value) {
  Store(base, address, std::bit_cast<uint32_t>(float(value)));
}
// An int made a float (std, lfd, fcfid, frsp); the doubleword the original
// passes it through on the stack is a C++ value since E5 step 6.
double IntToFloat(int32_t value) {
  const uint64_t word = uint64_t(int64_t(value));
  return double(float(double(int64_t(word))));
}

constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kPickNext = 0x821159B0;        // Career::PickNext (its adapter)

constexpr uint32_t kState = 9652;
constexpr uint32_t kCategory = 9656;
constexpr uint32_t kItem = 9660;
constexpr uint32_t kDepth = 10008;   // the state history's depth (a byte)
constexpr uint32_t kQueue = 10036;   // the command queue
constexpr uint32_t kMenu = 10040;    // the front-end menu
constexpr uint32_t kImages = 10048;  // the decal images

// memcpy(destination, source, 24) from `frame` with return address `lr`:
// Buffers_Memcpy's plain C++ where it takes it, else the guest's; r3 the
// destination.
uint64_t Copy24(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t destination,
                uint64_t source, uint32_t frame, uint32_t lr) {
  if (native::Buffers_Memcpy(base, uint32_t(destination), uint32_t(source), 24, lr)) {
    return destination;
  }
  return call(kMemcpy, {destination, source, 24}, frame, lr);
}

// The state history popped (its depth byte stepped down, wrapping) until the
// state +9652 it gives is `target` (no bound, as the original). Returns it.
uint32_t PopTo(uint8_t* base, uint32_t s, int32_t target) {
  uint32_t depth = uint32_t(Screen(base, s).history_depth());
  for (;;) {
    depth = (depth + 255) & 0xFF;
    Screen(base, s).set_history_depth(uint8_t(depth));
    const uint32_t state = Load(base, s + (depth + 2439) * 4);
    Screen(base, s).set_state(state);
    if (int32_t(state) == target) {
      return state;
    }
  }
}

// The state `state` pushed on the history: its word at the depth, the depth
// up one.
void Push(uint8_t* base, uint32_t s, uint32_t state) {
  Store(base, s + (uint32_t(Screen(base, s).history_depth()) + 2439) * 4, state);
}

// The queue [+10036] reset to `state`: +4, +0, +8 = state, +20 = +12 = -1,
// +24 = 0.
void Queue(uint8_t* base, uint32_t s, uint32_t state) {
  const uint32_t q = Screen(base, s).queue().address;
  CommandQueue(base, q).set_resume(state);
  CommandQueue(base, q).set_page(state);
  CommandQueue(base, q).set_previous_page(state);
  CommandQueue(base, q).set_step(0xFFFFFFFFu);
  CommandQueue(base, q).set_current(0xFFFFFFFFu);
  CommandQueue(base, q).set_started(0);
}

// The queue's command `command` then `next` for the state `current`: +8 = +0
// where +0 differs from it, +12, +16, +4 = current, +0 = -1.
void Then(uint8_t* base, uint32_t s, uint32_t current, uint32_t command, uint32_t next) {
  const uint32_t q = Screen(base, s).queue().address;
  const uint32_t head = CommandQueue(base, q).page();
  if (int32_t(current) != int32_t(head)) {
    CommandQueue(base, q).set_previous_page(head);
  }
  CommandQueue(base, q).set_current(command);
  CommandQueue(base, q).set_next(next);
  CommandQueue(base, q).set_resume(current);
  CommandQueue(base, q).set_page(0xFFFFFFFFu);
}

// Then for the state +9652 as it is now.
void ThenHere(uint8_t* base, uint32_t s, uint32_t command, uint32_t next) {
  const uint32_t q = Screen(base, s).queue().address;
  const uint32_t current = Screen(base, s).state();
  const uint32_t head = CommandQueue(base, q).page();
  if (int32_t(current) != int32_t(head)) {
    CommandQueue(base, q).set_previous_page(head);
  }
  CommandQueue(base, q).set_current(command);
  CommandQueue(base, q).set_next(next);
  CommandQueue(base, q).set_resume(current);
  CommandQueue(base, q).set_page(0xFFFFFFFFu);
}

// The menu [+10040] reset to `count` items with the cursor `cursor`.
void ResetMenu(uint8_t* base, uint32_t s, uint32_t count, uint32_t cursor) {
  const uint32_t m = Screen(base, s).category_menu().address;
  MenuOf(base, m).set_left_right_steps(0);
  MenuOf(base, m).set_up_down_locked(0);
  MenuOf(base, m).set_player(0);
  MenuOf(base, m).set_side(0);
  MenuOf(base, m).set_rows_shown(8);
  MenuOf(base, m).set_first_row(0);
  MenuOf(base, m).set_list_wrapped(0);
  MenuOf(base, m).set_previous(cursor);
  MenuOf(base, m).set_cursor(cursor);
  MenuOf(base, m).set_moved(0);
  MenuOf(base, m).set_wraps(1);
  MenuOf(base, m).set_last_item(count - 1);
  MenuOf(base, m).set_first_item(0);
}

// The menu's rows +48, +52 from the screen's +25100, +25104.
void MenuRows(uint8_t* base, uint32_t s) {
  const uint32_t first = Screen(base, s).category_menu().address;
  MenuOf(base, first).set_visible_row(Screen(base, s).list_scroll_row());
  const uint32_t m = Screen(base, s).category_menu().address;
  MenuOf(base, m).set_previous_row(Screen(base, s).unknown_25104());
}

// A word of the screen at +offset + 4 x [+30492] (the light kit group, read
// again for each store, as the original does).
uint32_t GroupWord(uint8_t* base, uint32_t s, uint32_t index) {
  return s + (Screen(base, s).light_kit_group() + index) * 4;
}

// The light kits' defaults for group 3 (state 12's other groups).
void GroupThreeDefaults(uint8_t* base, uint32_t s) {
  Store(base, Screen(base, s).light_kit_a_address() + 12, 0);
  Store(base, Screen(base, s).light_kit_a_address() + 28, 180);
  Store(base, Screen(base, s).light_kit_a_address() + 44, 180);
  Store(base, Screen(base, s).light_kit_a_address() + 60, 180);
  Store(base, Screen(base, s).light_kit_b_address() + 12, 0);
  Store(base, Screen(base, s).light_kit_b_address() + 28, 0);
  Store(base, Screen(base, s).light_kit_b_address() + 44, 180);
}

}  // namespace

void SwapKept(uint8_t* base, uint64_t images, uint64_t k, uint64_t i) {
  ITC_NATIVE_RUNNING("sub_821FE798 DecalImages::SwapKept");
  const schema::DecalImageKept kept = DecalKeptAt(base, images, uint32_t(k));
  const schema::DecalImageEntry entry = DecalEntryAt(base, images, i);
  const uint32_t kept_header = kept.texture_header();
  const uint32_t header = entry.texture_header();
  const uint32_t pixels = entry.texture_data();
  entry.set_texture_header(kept_header);
  entry.set_texture_data(kept.texture_data());
  kept.set_texture_header(header);
  kept.set_texture_data(pixels);
}

uint32_t GarageDecal(uint8_t* base, uint32_t screen, uint32_t area, uint64_t slot) {
  ITC_NATIVE_RUNNING("sub_821F84C8 TuningScreen::GarageDecal");
  const uint32_t c = tuning_screen::SelectedGarageCar(base, screen) + 212;
  static constexpr uint32_t kBases[8] = {9, 19, 14, 24, 29, 34, 44, 39};
  if (area >= 8) {
    return 0;
  }
  return uint32_t((slot + kBases[area]) << 5) + c;
}

uint64_t SaveAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint64_t area, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6CE8 TuningScreen::SaveAreaDecals");
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain; its callees and guest calls are given `sp`.
  uint32_t id = uint32_t((area + 1610) * 20 + screen);
  uint64_t record = area * 120 + screen + 31240;
  uint64_t r3 = screen;
  for (int k = 0; k < 5; k++) {
    Store(base, id, Load(base, id + 1120));
    r3 = Copy24(base, call, record, record + 1120, sp, 0x821F6D28);
    id += 4;
    record += 24;
  }
  return r3;
}

uint64_t RestoreAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint64_t area, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F6C90 TuningScreen::RestoreAreaDecals");
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain; its callees and guest calls are given `sp`.
  uint32_t id = uint32_t((area + 1666) * 20 + screen);
  uint64_t record = area * 120 + screen + 32360;
  uint64_t r3 = screen;
  for (int k = 0; k < 5; k++) {
    Store(base, id, Load(base, id - 1120));
    r3 = Copy24(base, call, record, record - 1120, sp, 0x821F6CD0);
    id += 4;
    record += 24;
  }
  return r3;
}

uint64_t LoadStickers(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                      uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F86C0 TuningScreen::LoadStickers");
  namespace steps = strings_and_font::steps;
  const uint32_t s = uint32_t(screen);
  // No guest frame (E5 step 6): the original's 176 bytes held only the back
  // chain; its callees and guest calls are given `sp`.
  const double offset = double(G::mirror_angle(base));
  const double scale = double(G::load_scale(base));
  const double minus = double(G::minus_one(base));
  const uint32_t one = Load(base, G::one_address());
  const double zero = double(G::zero(base));
  const uint32_t zero_word = Load(base, G::zero_address());
  uint32_t ids = Screen(base, s).saved_sticker_ids_address();
  uint32_t r = Screen(base, s).saved_stickers_address();  // the record (the original's r31 its +8)
  uint64_t r3 = screen;
  for (uint32_t a = 0; a < 8; a++) {
    const bool mirrored = a == 2 || a == 7;
    for (uint32_t k = 0; k < 5; k++) {
      const uint32_t g = GarageDecal(base, s, a, k);
      Store(base, ids, schema::CarMakersCarDecal(base, g).image());
      schema::StickerRecord(base, r).set_angle(std::bit_cast<float>(uint32_t(zero_word)));  // stfs of lfs: 0.0 and 1.0 as they are
      schema::StickerRecord(base, r).set_scale(std::bit_cast<float>(uint32_t(one)));
      schema::StickerRecord(base, r).set_x(0);
      schema::StickerRecord(base, r).set_y(0);
      schema::StickerRecord(base, r).set_flip(0);
      schema::StickerRecord(base, r).set_word(0);
      if (int32_t(schema::CarMakersCarDecal(base, g).image()) > -1) {
        double x = double(schema::CarMakersCarDecal(base, g).x());
        if (mirrored) {
          x = double(float(x * minus));
        }
        schema::StickerRecord(base, r).set_x(uint32_t(steps::Fctiwz(x)));
        schema::StickerRecord(base, r).set_y(uint32_t(steps::Fctiwz(double(schema::CarMakersCarDecal(base, g).y()))));
        const double first = double(schema::CarMakersCarDecal(base, g).flag_scale());
        uint32_t flag;
        double angle;
        if (mirrored) {
          flag = first < zero ? 0 : 1;
          angle = double(float(double(schema::CarMakersCarDecal(base, g).angle()) * scale));
          angle = double(float(std::fma(angle, minus, offset)));
        } else {
          flag = first < zero ? 1 : 0;
          angle = double(float(double(schema::CarMakersCarDecal(base, g).angle()) * scale));
        }
        schema::StickerRecord(base, r).set_angle(float(angle));
        // The original stores g+0 (times -1 with the flag) at +12 first and
        // then g+4 over it.
        schema::StickerRecord(base, r).set_scale(float(flag == 1 ? double(float(double(schema::CarMakersCarDecal(base, g).flag_scale()) * minus)) : double(schema::CarMakersCarDecal(base, g).flag_scale())));
        schema::StickerRecord(base, r).set_flip(flag);
        schema::StickerRecord(base, r).set_scale(float(double(schema::CarMakersCarDecal(base, g).scale())));
      }
      ids += 4;
      r += 24;
    }
    r3 = RestoreAreaDecals(base, call, screen, a, sp);
  }
  return r3;
}

uint64_t ResetStickerEditor(uint8_t* base, uint64_t screen) {
  ITC_NATIVE_RUNNING("sub_821F8440 TuningScreen::ResetStickerEditor");
  const uint32_t s = uint32_t(screen);
  const uint32_t id = Load(base, tuning_screen::SelectedGarageCar(base, s) + 224);
  for (const uint32_t offset : {10016u, 30724u, 30732u, 30740u, 30748u, 30756u, 30768u, 30764u}) {
    Store(base, s + offset, 0);
  }
  Screen(base, s).set_no_car_id(id == 0 ? 1 : 0);
  return screen;
}

uint64_t StoreStickers(uint8_t* base, uint64_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8598 TuningScreen::StoreStickers");
  const uint32_t s = uint32_t(screen);
  // No guest frame (E5 step 6): the original's 112 bytes held the back chain
  // and the conversions' doublewords at +80 and +88 (C++ values, only read
  // here); nothing here calls guest code.
  (void)sp;
  const double scale = double(G::store_scale(base));
  const double minus = double(G::minus_one(base));
  const double offset = double(G::mirror_angle(base));
  uint32_t record = Screen(base, s).saved_stickers_address();
  uint32_t ids = Screen(base, s).saved_sticker_ids_address();
  uint32_t g = 0;
  for (uint32_t a = 0; a < 8; a++) {
    const bool mirrored = a == 2 || a == 7;
    for (uint32_t k = 0; k < 5; k++) {
      g = GarageDecal(base, s, a, k);
      schema::CarMakersCarDecal(base, g).set_image(Load(base, ids));
      uint32_t x = uint32_t(schema::StickerRecord(base, record).x());
      if (mirrored) {
        x = 0u - x;
      }
      schema::CarMakersCarDecal(base, g).set_x(float(IntToFloat(int32_t(x))));
      schema::CarMakersCarDecal(base, g).set_y(float(IntToFloat(int32_t(uint32_t(schema::StickerRecord(base, record).y())))));
      double angle = double(schema::StickerRecord(base, record).angle());
      uint32_t flag = schema::StickerRecord(base, record).flip();
      if (mirrored) {
        angle = double(float(angle - offset));
        flag = flag == 0 ? 1 : 0;
        angle = double(float(angle * minus));
      }
      schema::CarMakersCarDecal(base, g).set_angle(float(double(float(angle * scale))));
      double first = double(schema::StickerRecord(base, record).scale());
      if (int32_t(flag) == 1) {
        first = double(float(first * minus));
      }
      schema::CarMakersCarDecal(base, g).set_flag_scale(float(first));
      schema::CarMakersCarDecal(base, g).set_scale(float(double(schema::StickerRecord(base, record).scale())));
      ids += 4;
      record += 24;
    }
  }
  return g;
}

uint64_t OnStickerDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FA230 TuningScreen::OnStickerDialogYes");
  const uint32_t s = uint32_t(screen);
  // No guest frame (E5 step 6): the original's 96 bytes held only the back
  // chain; its callees and guest calls are given `sp`.
  const int32_t state = int32_t(Screen(base, s).state());
  uint64_t r3 = screen;
  if (state == 52) {
    // Delete a sticker: back to state 39, the area's slot emptied.
    Screen(base, s).set_refresh_pending(1);
    Screen(base, s).set_command_end_after_setup(1);
    Queue(base, s, 52);
    const uint32_t q = Screen(base, s).queue().address;
    Screen(base, s).set_state(39);
    const uint32_t head = CommandQueue(base, q).page();
    if (int32_t(head) != 39) {
      CommandQueue(base, q).set_previous_page(head);
    }
    CommandQueue(base, q).set_resume(39);
    CommandQueue(base, q).set_page(0xFFFFFFFFu);
    const double zero = double(G::zero(base));
    CommandQueue(base, q).set_next(34);
    const double one = double(G::one(base));
    CommandQueue(base, q).set_current(53);
    uint32_t a = Screen(base, s).editor_page();
    uint32_t k = Screen(base, s).layer_cursor();
    Store(base, ((a + 1666) * 5 + k) * 4 + s, 0xFFFFFFFFu);
    a = Screen(base, s).editor_page();
    k = Screen(base, s).layer_cursor();
    const uint32_t r = (a * 5 + k) * 24 + s;
    const schema::StickerRecord sticker(base, r + 32360);  // the area's slot k
    sticker.set_angle(float(zero));
    sticker.set_x(0);
    sticker.set_scale(float(one));
    sticker.set_y(0);
    sticker.set_flip(0);
    sticker.set_word(0);
  } else if (state == 50 || state == 51) {
    // Keep (50) or discard (51) the area: back to state 38, the stand's
    // decals applied again.
    Screen(base, s).set_refresh_pending(1);
    Screen(base, s).set_command_end_after_setup(1);
    if (state == 50) {
      SaveAreaDecals(base, call, screen, Screen(base, s).editor_page(), sp);
      StoreStickers(base, screen, sp);
      call(kPickNext, {vehicle_accessors::SaveDataOf(base, 0), 1}, sp, 0x821FA360);
      const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
      r3 = save;
      for (uint32_t i = 0; i < 5; i++) {
        Store(base, s + 20792 + 4 * i, Load(base, save + 8 + 4 * i));
      }
    }
    const uint32_t q = Screen(base, s).queue().address;
    Screen(base, s).set_state(38);
    CommandQueue(base, q).set_resume(38);
    CommandQueue(base, q).set_page(38);
    CommandQueue(base, q).set_previous_page(38);
    CommandQueue(base, q).set_step(0xFFFFFFFFu);
    CommandQueue(base, q).set_current(0xFFFFFFFFu);
    CommandQueue(base, q).set_started(0);
    ThenHere(base, s, 53, 32);
    Screen(base, s).set_stand_action(2);
  }
  return r3;
}

uint64_t OnDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, bool native_specs, const StandFit& fit) {
  ITC_NATIVE_RUNNING("sub_821E2660 TuningScreen::OnDialogYes");
  const uint32_t s = uint32_t(screen);
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain; its callees and guest calls are given `sp`.
  const int32_t state = int32_t(Screen(base, s).state());
  const auto category = [&] { return int32_t(Screen(base, s).category()); };
  const auto item = [&] { return uint32_t(Screen(base, s).item()); };
  const auto set_choice = [&] { SetCurrentChoice(base, call, screen, sp, native_specs); };
  const auto copy = [&](uint32_t from, uint32_t to) {
    uint32_t words[4];
    for (uint32_t i = 0; i < 4; i++) {
      words[i] = Load(base, s + from + 4 * i);
    }
    for (uint32_t i = 0; i < 4; i++) {
      Store(base, s + to + 4 * i, words[i]);
    }
  };
  if (state == 9) {
    // CONF BUY: the price paid, the pick owned, CONF EQUIP's page next.
    const uint32_t m = Screen(base, s).category_menu().address;
    const uint32_t price = Screen(base, s).price();
    const uint32_t money = Screen(base, s).money() - price;
    const uint32_t cursor = MenuOf(base, m).cursor();
    Screen(base, s).set_money(money);
    Screen(base, s).set_unknown_9748(cursor);
    Screen(base, s).set_confirm_cursor(uint8_t(cursor));
    MarkOwned(base, s);
    const uint32_t depth = uint32_t(Screen(base, s).history_depth());
    Screen(base, s).set_confirm_cursor(0);
    Screen(base, s).set_unknown_9666(0);
    Screen(base, s).set_refresh_pending(1);
    Store(base, s + (depth + 2439) * 4, 9);
    const uint32_t up = uint32_t(Screen(base, s).history_depth()) + 1;
    const uint32_t q = Screen(base, s).queue().address;
    Screen(base, s).set_state(11);
    Screen(base, s).set_history_depth(uint8_t(up));
    CommandQueue(base, q).set_resume(11);
    CommandQueue(base, q).set_page(11);
    CommandQueue(base, q).set_previous_page(11);
    CommandQueue(base, q).set_step(0xFFFFFFFFu);
    CommandQueue(base, q).set_current(0xFFFFFFFFu);
    CommandQueue(base, q).set_started(0);
    const uint32_t pick = uint32_t(Screen(base, s).confirm_cursor());
    const uint32_t menu = Screen(base, s).category_menu().address;
    MenuOf(base, menu).set_first_item(0);
    MenuOf(base, menu).set_last_item(1);
    MenuOf(base, menu).set_wraps(1);
    MenuOf(base, menu).set_moved(0);
    MenuOf(base, menu).set_cursor(pick);
    MenuOf(base, menu).set_previous(pick);
    MenuOf(base, menu).set_list_wrapped(0);
    MenuOf(base, menu).set_first_row(0);
    MenuOf(base, menu).set_rows_shown(8);
    MenuOf(base, menu).set_side(0);
    MenuOf(base, menu).set_player(0);
    MenuOf(base, menu).set_up_down_locked(0);
    MenuOf(base, menu).set_left_right_steps(0);
    const uint32_t q2 = Screen(base, s).queue().address;
    const uint32_t current = Screen(base, s).state();
    const uint32_t head = CommandQueue(base, q2).page();
    if (int32_t(current) != int32_t(head)) {
      CommandQueue(base, q2).set_previous_page(head);
    }
    CommandQueue(base, q2).set_page(current);
    CommandQueue(base, q2).set_current(0xFFFFFFFFu);
    CommandQueue(base, q2).set_next(0xFFFFFFFFu);
  } else if (state == 11) {
    // CONF EQUIP: the pick made current, back to its list.
    if (!(category() == 6 && item() == 3 && int32_t(Screen(base, s).interior_mode()) == 1)) {
      set_choice();
    }
    const int32_t c = category();
    Screen(base, s).set_rewrite_car(1);
    if (c == 2) {
      PopTo(base, s, 3);
    } else if (c == 5 && item() == 0) {
      PopTo(base, s, 15);
      Screen(base, s).set_part_group(0);
      Screen(base, s).set_unknown_25208(0);
    } else if (c == 6 && item() == 3 && int32_t(Screen(base, s).interior_mode()) == 0) {
      PopTo(base, s, 53);
    } else if (c == 6 && item() == 3 && int32_t(Screen(base, s).interior_mode()) == 1) {
      Screen(base, s).set_rewrite_car(0);
      Screen(base, s).set_interior_slot(0);
      Push(base, s, 11);
      const uint32_t up = uint32_t(Screen(base, s).history_depth()) + 1;
      Screen(base, s).set_state(57);
      Screen(base, s).set_history_depth(uint8_t(up));
    } else {
      PopTo(base, s, 4);
    }
    const uint32_t q = Screen(base, s).queue().address;
    const uint32_t current = Screen(base, s).state();
    Screen(base, s).set_refresh_pending(1);
    Screen(base, s).set_command_first_run(1);
    CommandQueue(base, q).set_step(0xFFFFFFFFu);
    CommandQueue(base, q).set_resume(current);
    CommandQueue(base, q).set_page(current);
    CommandQueue(base, q).set_previous_page(current);
    CommandQueue(base, q).set_current(0xFFFFFFFFu);
    CommandQueue(base, q).set_started(0);
    const int32_t c2 = category();
    const bool menu = !((c2 == 5 && item() == 0) || (c2 == 6 && item() == 3)) || c2 == 4;
    if (menu) {
      const uint32_t count = PageItemCount(base, s);
      const uint32_t cursor = PageCursor(base, s);
      ResetMenu(base, s, count, cursor);
    }
    if (category() == 4) {
      MenuRows(base, s);
    }
    const uint32_t now = Screen(base, s).state();
    switch (int32_t(now)) {
      case 3: Then(base, s, now, 64, 0); break;
      case 4: Then(base, s, now, 64, 2); break;
      case 15: Then(base, s, now, 64, 7); break;
      case 53: Then(base, s, now, 64, 54); break;
      case 56: Then(base, s, now, 64, 57); break;
      case 57: Then(base, s, now, 64, 58); break;
      default: break;
    }
  } else if (state == 12 || state == 13) {
    // Keep (12) or discard (13) the paint or the light kit.
    Screen(base, s).set_rewrite_car(1);
    Screen(base, s).set_refresh_pending(1);
    Screen(base, s).set_command_first_run(1);
    if (category() == 5) {
      const uint32_t it = item();
      if (it == 4) {
        PopTo(base, s, 4);
        Queue(base, s, 4);
        ThenHere(base, s, 64, 2);
        if (state == 12) {
          copy(30444, 30476);
        } else {
          copy(30476, 30444);
        }
      } else if (it == 5) {
        const int32_t kind = int32_t(Screen(base, s).light_kit_kind());
        if (kind == 0 || kind == 1) {
          PopTo(base, s, 19);
          Queue(base, s, 19);
          ThenHere(base, s, 64, 11);
          if (state == 12 && kind == 0) {
            if (int32_t(Screen(base, s).light_kit_group()) == 3) {
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t at = Screen(base, s).light_kit_b_address() + 4 * i;
                Store(base, at - 80, 1);
                Store(base, at, 0);
                Store(base, at - 64, Screen(base, s).kit0_value_0());
                Store(base, at - 48, Screen(base, s).kit0_value_1());
                Store(base, at - 32, Screen(base, s).kit0_value_2());
                Store(base, at - 16, Screen(base, s).kit0_value_3());
                Store(base, at + 48, Screen(base, s).kit0_value_3());
              }
            } else {
              Store(base, GroupWord(base, s, 7645), 1);
              Store(base, GroupWord(base, s, 7665), 0);
              {
                const uint32_t at = GroupWord(base, s, 7649);
                Store(base, at, Screen(base, s).kit0_value_0());
              }
              {
                const uint32_t at = GroupWord(base, s, 7653);
                Store(base, at, Screen(base, s).kit0_value_1());
              }
              {
                const uint32_t at = GroupWord(base, s, 7657);
                Store(base, at, Screen(base, s).kit0_value_2());
              }
              GroupThreeDefaults(base, s);
              for (uint32_t i = 0; i < 4; i++) {
                Store(base, Screen(base, s).light_kit_a_address() + 64 + 4 * i, Screen(base, s).kit0_value_3());
                Store(base, Screen(base, s).light_kit_b_address() + 48 + 4 * i, Screen(base, s).kit0_value_3());
              }
            }
          } else if (state == 12) {
            if (int32_t(Screen(base, s).light_kit_group()) == 3) {
              for (uint32_t i = 0; i < 4; i++) {
                const uint32_t at = Screen(base, s).light_kit_a_address() + 4 * i;
                Store(base, at + 80, 1);
                Store(base, at, 0);
                Store(base, at + 96, Screen(base, s).kit1_value_0());
                Store(base, at + 112, Screen(base, s).kit1_value_1());
                Store(base, at + 64, Screen(base, s).kit1_value_2());
                Store(base, at + 128, Screen(base, s).kit1_value_2());
              }
            } else {
              Store(base, GroupWord(base, s, 7665), 1);
              Store(base, GroupWord(base, s, 7645), 0);
              {
                const uint32_t at = GroupWord(base, s, 7669);
                Store(base, at, Screen(base, s).kit1_value_0());
              }
              {
                const uint32_t at = GroupWord(base, s, 7673);
                Store(base, at, Screen(base, s).kit1_value_1());
              }
              GroupThreeDefaults(base, s);
              for (uint32_t i = 0; i < 4; i++) {
                Store(base, Screen(base, s).light_kit_a_address() + 64 + 4 * i, Screen(base, s).kit1_value_2());
                Store(base, Screen(base, s).light_kit_b_address() + 48 + 4 * i, Screen(base, s).kit1_value_2());
              }
            }
          }
        }
      }
    }
  } else if (state == 14) {
    // Remove a part.
    Screen(base, s).set_rewrite_car(1);
    Screen(base, s).set_refresh_pending(1);
    Screen(base, s).set_command_first_run(1);
    const int32_t c = category();
    bool done = false;
    if (c == 4) {
      const uint32_t it = item();
      const uint32_t type = uint32_t(Screen(base, s).type());
      if ((it == 3 && type == 1) || (it == 9 && type == 0)) {
        PopTo(base, s, 4);
        Queue(base, s, 4);
        const uint32_t count = PageItemCount(base, s);
        const uint32_t cursor = PageCursor(base, s);
        const uint32_t m = Screen(base, s).category_menu().address;
        MenuOf(base, m).set_first_item(0);
        MenuOf(base, m).set_last_item(count - 1);
        MenuOf(base, m).set_wraps(1);
        MenuOf(base, m).set_cursor(cursor);
        MenuOf(base, m).set_previous(cursor);
        MenuOf(base, m).set_moved(0);
        MenuOf(base, m).set_list_wrapped(0);
        MenuOf(base, m).set_first_row(0);
        MenuOf(base, m).set_rows_shown(8);
        MenuOf(base, m).set_side(0);
        MenuOf(base, m).set_player(0);
        MenuOf(base, m).set_up_down_locked(0);
        MenuOf(base, m).set_left_right_steps(0);
        MenuRows(base, s);
        ThenHere(base, s, 64, 2);
        set_choice();
        done = true;
      }
    }
    if (!done && c == 5 && item() == 5) {
      PopTo(base, s, 19);
      Queue(base, s, 19);
      ThenHere(base, s, 64, 11);
      if (int32_t(Screen(base, s).light_kit_group()) == 3) {
        for (uint32_t i = 0; i < 4; i++) {
          const uint32_t at = Screen(base, s).light_kit_a_address() + 16 + 4 * i;
          Store(base, at - 16, 0);
          Store(base, at + 0, 180);
          Store(base, at + 16, 180);
          Store(base, at + 32, 180);
          Store(base, at + 48, 0);
          Store(base, at + 64, 0);
          Store(base, at + 80, 0);
          Store(base, at + 96, 180);
          Store(base, at + 112, 0);
        }
      } else {
        Store(base, GroupWord(base, s, 7645), 0);
        Store(base, GroupWord(base, s, 7649), 180);
        Store(base, GroupWord(base, s, 7653), 180);
        Store(base, GroupWord(base, s, 7657), 180);
        Store(base, GroupWord(base, s, 7665), 0);
        Store(base, GroupWord(base, s, 7669), 0);
        Store(base, GroupWord(base, s, 7673), 180);
      }
    } else if (!done && c == 6 && item() == 3) {
      const int32_t which = int32_t(Screen(base, s).interior_mode());
      if (which == 0) {
        PopTo(base, s, 53);
        Queue(base, s, 53);
        ThenHere(base, s, 64, 54);
        set_choice();
      } else if (which == 1) {
        PopTo(base, s, 56);
        Queue(base, s, 56);
        ThenHere(base, s, 64, 57);
        Store(base, s + (Screen(base, s).interior_slot() + 8450) * 4, 0xFFFFFFFFu);
      }
    }
  } else if (state == 35 || state == 36) {
    // The vinyl editor: discard (35) or keep (36) the page's layers.
    Screen(base, s).set_refresh_pending(1);
    PopTo(base, s, 23);
    Queue(base, s, 23);
    const bool vinyls = category() == 5 && item() == 6;
    {
      const uint32_t q = Screen(base, s).queue().address;
      const uint32_t current = Screen(base, s).state();
      const uint32_t head = CommandQueue(base, q).page();
      if (int32_t(current) != int32_t(head)) {
        CommandQueue(base, q).set_previous_page(head);
      }
      CommandQueue(base, q).set_page(0xFFFFFFFFu);
      CommandQueue(base, q).set_resume(current);
      CommandQueue(base, q).set_next(vinyls ? 15 : 32);
      CommandQueue(base, q).set_current(vinyls ? 31 : 53);
    }
    if (category() == 5 && item() == 6) {
      const uint32_t live_id = 7792, saved_id = 7798;
      const uint32_t live = 30880, saved = 31024;
      const uint32_t from_id = state == 35 ? saved_id : live_id;
      const uint32_t to_id = state == 35 ? live_id : saved_id;
      const uint32_t from = state == 35 ? saved : live;
      const uint32_t to = state == 35 ? live : saved;
      const uint32_t lr = state == 35 ? 0x821E35E4 : 0x821E3768;
      for (uint32_t j = 0; j < 3; j++) {
        uint64_t n = uint64_t(Screen(base, s).editor_page()) * 3 + j;
        Store(base, s + uint32_t(n + to_id) * 4, Load(base, s + uint32_t(n + from_id) * 4));
        n = uint64_t(Screen(base, s).editor_page()) * 3 + j;
        const uint64_t at = n * 24 + screen;
        Copy24(base, call, at + to, at + from, sp, lr);
      }
      if (state == 36) {
        const int32_t page = int32_t(Screen(base, s).editor_page());
        Screen(base, s).set_vinyl_kept(1);
        const uint64_t images = screen + kImages;
        if (page == 0 || page == 1) {
          static constexpr uint32_t kSwaps[2][6][2] = {
              {{0, 0}, {1, 1}, {2, 2}, {3, 6}, {4, 7}, {5, 8}},
              {{15, 21}, {16, 22}, {17, 23}, {18, 27}, {19, 28}, {20, 29}}};
          for (const auto& [k, i] : kSwaps[page]) {
            SwapKept(base, images, k, i);
          }
          for (const auto& [k, i] : kSwaps[page]) {
            BindVinylTexture(base, images, i, uint32_t(page), sp);
          }
        }
      }
    }
    StoreVinyls(base, screen, sp);
    Screen(base, s).set_stand_action(6);
    const uint32_t c = tuning_screen::SelectedGarageCar(base, s);
    const uint32_t holder = schema::TuningScreenGlobals::stand_holder(base).stage_block_0();
    const int64_t a = int8_t(Byte(base, c + 300));
    const int64_t b = int8_t(Byte(base, c + 301));
    FitSelectedCar(base, call, fit, holder, uint64_t(a), uint64_t(b), sp, 0x821E3984);
  } else if (state == 37) {
    // Delete a vinyl layer (item 6) or leave the sticker list (item 7).
    if (category() == 5) {
      const uint32_t it = item();
      if (it == 6 || it == 7) {
        Screen(base, s).set_refresh_pending(1);
        PopTo(base, s, 24);
        if (it == 6) {
          Queue(base, s, 24);
          ThenHere(base, s, 31, 16);
        }
        if (category() == 5 && item() == 6) {
          const uint32_t layer = Screen(base, s).layer_cursor();
          const double zero = double(G::zero(base));
          const double one = double(G::one(base));
          Store(base, s + (Screen(base, s).editor_page() * 3 + layer + 7798) * 4, 0xFFFFFFFFu);
          const uint32_t page = Screen(base, s).editor_page();
          const uint32_t r = (page * 3 + Screen(base, s).layer_cursor()) * 24 + s;
          Stfs(base, r + 31032, zero);
          Store(base, r + 31024, 0);
          Stfs(base, r + 31036, one);
          Store(base, r + 31028, 0);
          Store(base, r + 31040, 0);
          Store(base, r + 31044, 0);
        }
      }
    }
  }
  return OnStickerDialogYes(base, call, screen, sp);
}

}  // namespace tuning_pages
