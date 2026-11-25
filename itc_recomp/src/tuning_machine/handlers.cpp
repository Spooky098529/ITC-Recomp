// The tuning screen machine's handlers (docs/functions/tuning-machine.md):
// see tuning_machine.h.

#include "tuning_machine/tuning_machine.h"

#include "base/log.h"
#include "game_modes/game_modes.h"
#include "tuning_pages/tuning_pages.h"
#include "guest.h"
#include "hooks/heap.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/tuning_machine.h"
#include "schema/tuning_pages.h"
#include "schema/ui_movies_game.h"
#include "ui_movies/game.h"

namespace tuning_machine {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kMemRelease = 0x8000;

using G = schema::TuningMachineGlobals;
using Screen = schema::TuningMachineScreen;
using TextureTable = schema::UiMovieTextureTable;

inline uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
inline void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

// The original's frame below `sp`, its back chain written.
inline uint32_t Frame(uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  Store(frame, sp);
  return frame;
}

// UiMovies::Find with the id as a word (the original stored it in its
// frame's +80 for the key; only Find read it): the pair's value's address.
inline uint32_t FindPair(uint32_t id) {
  return ui_movies::PairsFindKey(ui_movies::kMovieTable, id);
}

// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint32_t mode, uint32_t id) {
  const schema::TuningMachineMode m(mode);
  const uint32_t count = m.message_count() + 1;
  const uint32_t start = m.message_start();
  m.set_message_count(count);
  const game_modes::RingSlot slot{m.messages() + 12 * ((start + count - 1) % 50)};
  slot.set_id(id);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// A front-end menu's first words, as Create writes them (+44, +56 and +60
// not written).
void ResetMenu(const schema::FrontEndMenu& menu) {
  menu.set_first_item(0);
  menu.set_last_item(6);
  menu.set_wraps(1);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
}

}  // namespace

void ZeroRecord16(uint32_t record) {
  ITC_NATIVE_RUNNING("sub_821FE540 TuningMachine::ZeroRecord16");
  const uint32_t zero = G::float_zero();
  Store(record + 8, zero);
  Store(record + 0, 0);
  Store(record + 12, zero);
  Store(record + 4, 0);
}

void ResetImages(uint32_t images) {
  ITC_NATIVE_RUNNING("sub_821FE590 DecalImages::Reset");
  const schema::DecalImages im(images);
  for (uint32_t i = 0; i < 200; i++) {
    const schema::DecalImageEntry entry(im.entries_address() + 36 * i);
    entry.set_handle(0);
    entry.set_state(0);
    entry.set_file(0);
    entry.set_file_size(0);
    entry.set_to_make(0);
    entry.set_requested(0);
    entry.set_texture_header(0);
    entry.set_texture_data(0);
    entry.set_asset_id(kNone);
  }
  im.set_unknown_8852(0);
}

uint64_t Create(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821CE4C8 TuningMachine::Create");
  const Screen sc{uint32_t(r3)};
  // The original's 144-byte frame held nothing the port keeps (no call is
  // made with it, E5 step 6). The screen kept in the caller's parameter area (read back before the
  // menus are written).
  Store(sp + 20, sc.address);
  sc.set_movies_loaded(0);
  sc.set_screen_state(0);
  sc.set_category(0);
  sc.set_item(0);
  sc.set_level(0);
  sc.set_type(0);
  sc.set_bonnet_choice(0);
  sc.set_option(0);
  sc.set_confirm_cursor(0);
  sc.set_unknown_9666(0);
  sc.set_keep_cursor(0);
  sc.set_discard_cursor(0);
  sc.set_item_count(0);
  sc.set_level_count(0);
  sc.set_type_count(0);
  sc.set_color_rows(0);
  sc.set_option_count(0);
  sc.set_history_depth(0);
  sc.set_refresh(1);
  sc.set_bonnet_color_set(0);
  sc.set_pages_enabled(1);
  sc.set_unknown_9684(17);
  sc.set_open_mask(kNone);
  sc.set_unknown_9692(kNone);
  sc.set_unknown_9696(kNone);
  sc.set_unknown_9700(kNone);
  sc.set_unknown_9704(kNone);
  sc.set_menu_category(0);
  sc.set_menu_choice(0);
  sc.set_unknown_9748(0);
  sc.set_unknown_9752(0);
  sc.set_stand_action(kNone);
  sc.set_busy_shown(0);
  sc.set_busy_movie(kNone);
  sc.set_busy_animation_a(kNone);
  sc.set_busy_animation_b(kNone);
  sc.set_busy_animation_c(kNone);
  sc.set_queue(0);
  sc.set_category_menu(0);
  for (uint32_t w = sc.wheel_part_cursor_address(); w <= sc.second_wheels_step_address(); w += 4) {
    Store(w, 0);
  }
  for (uint32_t w = sc.paint_row_address(); w <= sc.unknown_30472_address(); w += 4) {
    Store(w, 0);
  }
  for (uint32_t w = sc.light_kit_group_address(); w <= sc.unknown_30576_address(); w += 4) {
    Store(w, 0);
  }
  for (uint32_t w = sc.editor_page_address(); w <= sc.order_position_address(); w += 4) {
    Store(w, 0);
  }
  sc.set_picked_image(0);
  sc.set_image_group(0);
  sc.set_remove_cursor(0);
  for (uint32_t w = sc.unknown_30772_address(); w <= sc.order_last_position_address(); w += 4) {
    Store(w, 0);
  }
  // The vinyl layers' ids: two passes of 0, 1, 2 (the second word list
  // runs on, the third is written again).
  {
    uint32_t r10 = sc.saved_vinyl_layer_ids_address();
    for (uint32_t pass = 0; pass < 2; pass++) {
      uint32_t r9 = sc.order_list_address();
      for (uint32_t j = 0; j < 3; j++) {
        Store(r10 - 24, j);
        Store(r10, j);
        r10 += 4;
        Store(r9, j);
        r9 += 4;
      }
    }
  }
  // The stickers' ids: eight passes of 0 to 4.
  {
    uint32_t r10 = sc.sticker_ids_address();
    for (uint32_t pass = 0; pass < 8; pass++) {
      uint32_t r9 = sc.sticker_order_address();
      for (uint32_t k = 0; k < 5; k++) {
        Store(r10 - 1120, k);
        Store(r10, k);
        r10 += 4;
        Store(r9, k);
        r9 += 4;
      }
    }
  }
  const uint32_t zero = G::float_zero();
  const uint32_t one = G::float_one();
  sc.set_edit_x(0);
  sc.set_edit_y(0);
  sc.set_edit_flip(0);
  sc.set_unknown_30852(0);
  Store(sc.edit_angle_address(), zero);
  Store(sc.edit_scale_address(), one);
  // The decal records: 24 bytes each, +8 0.0 and +12 1.0.
  const auto record = [&](uint32_t at) {
    Store(at + 8, zero);
    Store(at + 0, 0);
    Store(at + 12, one);
    Store(at + 4, 0);
    Store(at + 16, 0);
    Store(at + 20, 0);
  };
  for (uint32_t j = 0; j < 6; j++) {
    record(sc.vinyl_layers_address() + 24 * j);
    record(sc.saved_vinyl_layers_address() + 24 * j);
  }
  for (uint32_t j = 0; j < 40; j++) {
    record(sc.saved_stickers_address() + 24 * j);
    record(sc.stickers_address() + 24 * j);
  }
  for (uint32_t w = sc.interior_mode_address(); w <= sc.interior_mode_address() + 40; w += 4) {
    Store(w, 0);
  }
  sc.set_last_price(0);
  sc.set_stat_displacement(0);
  sc.set_stat_weight(0);
  sc.set_stat_power(0);
  Store(sc.stat_torque_address(), zero);
  sc.set_paint_kept(0);
  ZeroRecord16(sc.page0_origin_x_address());
  ZeroRecord16(sc.page1_origin_x_address());
  for (uint32_t k = 0; k < 63; k++) {
    Store(sc.state_history_address() + 4 * k, 0);
  }
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x821CE8D8
  // and the VirtualAlloc(68)s at 0x821CE8F4 and 0x821CE910; the screen's
  // words hold their handles.
  sc.set_queue(schema::PageObject::New().address);
  sc.set_category_menu(schema::FrontEndMenu::New().address);
  sc.set_second_menu(schema::FrontEndMenu::New().address);
  ResetImages(sc.decal_images_address());
  // The queue: TUNING SHOP MAIN (3); +16 not written.
  const schema::PageObject queue = sc.queue();
  const Screen screen(Load(sp + 20));
  queue.set_next_page(3);
  queue.set_page(3);
  queue.set_previous_page(3);
  queue.set_unknown_20(kNone);
  queue.set_step(kNone);
  queue.set_step_started(0);
  ResetMenu(screen.category_menu());
  ResetMenu(screen.second_menu());
  screen.set_state(screen.reenter() != 0 ? 1 : 9);
  return r3 + kImages;
}

uint64_t OpenMovies(uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821CBDB0 TuningMachine::OpenMovies");
  const uint32_t frame = Frame(sp, 96);
  calls.Call(kMoviesOpen, frame, 0x821CBDD0, kShopMovie, 22547);
  calls.Call(kMoviesOpen, frame, 0x821CBDDC, kVinylMovie, 22548);
  const uint64_t r3 = calls.Call(kMoviesOpen, frame, 0x821CBDE8, kStickerMovie, 22549);
  Screen(screen).set_movies_loaded(0);
  return r3;
}

uint64_t WaitForMovies(uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CE9E8 TuningMachine::WaitForMovies");
  const Screen sc{uint32_t(r3)};
  // No frame (the original's 112 bytes held nothing; E5 step 6).
  if (sc.movies_loaded() == 0) {
    bool all = true;
    for (const uint32_t id : {kShopMovie, kVinylMovie, kStickerMovie}) {
      r3 = ui_movies::IsLoaded(id);
      if (uint32_t(r3) == 0) {
        all = false;
        break;
      }
    }
    if (all) {
      sc.set_movies_loaded(1);
    }
  }
  if (sc.movies_loaded() != 0) {
    const uint32_t reenter = sc.reenter();
    sc.set_refresh(1);
    sc.set_state(reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t OnDialogClosed(uint64_t r3, uint32_t mode, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821D8898 TuningMachine::OnDialogClosed");
  const Screen sc{uint32_t(r3)};
  const uint32_t frame = Frame(sp, 128);
  if (sc.screen_state() == 11) {
    const uint32_t movie = Load(FindPair(kDialogMovie));
    calls.CallArgs(kSetText, frame, 0x821D88E8, {movie, kNone, 0, 15, 0, 0, 0, 0});
    calls.CallArgs(kSetText, frame, 0x821D88FC, {movie, 1, 3, 2, 0, 0, 0, 0});
    r3 = calls.CallArgs(kSetText, frame, 0x821D8910, {movie, 1, 4, 3, 0, 0, 0, 0});
    Store(movie + 40960, 0);
    Post(mode, 34);
  }
  sc.set_pages_enabled(1);
  return r3;
}

uint64_t Unload(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821D69A0 TuningMachine::Unload");
  const Screen sc{uint32_t(r3)};
  const uint32_t frame = Frame(sp, 112);
  if (sc.movies_loaded() != 0) {
    const uint32_t movie = Load(FindPair(kShopMovie));
    r3 = calls.Call(kFinishAll, frame, 0x821D69E0, movie);
  }
  if (sc.delete_movies() != 0) {
    calls.Call(kMoviesDelete, frame, 0x821D69F4, kShopMovie);
    calls.Call(kMoviesDelete, frame, 0x821D69FC, kVinylMovie);
    r3 = calls.Call(kMoviesDelete, frame, 0x821D6A04, kStickerMovie);
    sc.set_movies_loaded(0);
  }
  return r3;
}

void Close(uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_821D6A78 TuningMachine::Close");
  Screen(machine).set_state(7);
  Post(mode, 4);
}

uint64_t Destroy(uint64_t r3, uint32_t sp, const Calls& calls,
                 const UnloadAllNative* unload) {
  ITC_NATIVE_RUNNING("sub_821CBD40 TuningMachine::Destroy");
  const Screen sc{uint32_t(r3)};
  const uint32_t frame = Frame(sp, 112);
  if (unload != nullptr) {
    tuning_pages::UnloadAll(*unload->call, r3 + kImages, frame, unload->native_free);
  } else {
    calls.Call(kUnloadAll, frame, 0x821CBD60, r3 + kImages);
  }
  // VirtualFree straight to the allocation layer (hooks::VirtualFree, its
  // adapter's answer); through the guest where the adapter would run the
  // original or answer an address of 0.
  auto virtual_free = [&](uint32_t address, uint32_t lr) -> uint64_t {
    if (address != 0) {
      if (const auto freed = hooks::VirtualFree(address, 0, kMemRelease, lr)) {
        return *freed;
      }
    }
    return calls.Call(kVirtualFree, frame, lr, address, 0, kMemRelease);
  };
  virtual_free(sc.queue().address, 0x821CBD78);
  virtual_free(sc.category_menu().address, 0x821CBD88);
  return virtual_free(sc.second_menu().address, 0x821CBD98);
}

uint32_t SavePlaceholders14(uint32_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F8950 DecalImages::SavePlaceholders14");
  const schema::DecalImages im(images);
  // No frame (E5 step 6: the original's 112 bytes held only Find's key).
  (void)sp;
  const uint32_t pair = FindPair(kStickerMovie);
  const schema::UiMovie movie(Load(pair));
  // Its texture table's (the movie's +28) textures 160 and 159.
  im.set_sticker_placeholder_0(TextureTable(movie.textures()).texture_160().texture());
  im.set_sticker_placeholder_1(TextureTable(movie.textures()).texture_159().texture());
  return pair;
}

uint32_t SavePlaceholders(uint32_t images, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82208130 DecalImages::SavePlaceholders");
  const schema::DecalImages im(images);
  // No frame (E5 step 6: the original's 112 bytes held only Find's key).
  const schema::UiMovie movie(Load(FindPair(kVinylMovie)));
  // Its texture table's (the movie's +28) textures 181 and 175.
  im.set_vinyl_placeholder_0(TextureTable(movie.textures()).texture_181().texture());
  im.set_vinyl_placeholder_1(TextureTable(movie.textures()).texture_175().texture());
  return SavePlaceholders14(images, sp - 112);
}

}  // namespace tuning_machine
