// The type 6 and 7 modes' objects through the schema's views
// (itc_recomp/schema/type6-type7-modes.toml, docs/e5-native.md step 2), for
// the twins type 6 and type 7 share: their first screens' words sit 4 bytes
// further in type 7's (its machine at +4), the dialog's status word at
// +9756 or +9760. Each accessor is one load or store of the type's field.

#pragma once

#include "schema/boot_mode.h"
#include "schema/car_makers.h"
#include "schema/career.h"
#include "schema/game_modes.h"
#include "schema/menu_helpers.h"
#include "schema/menu_sounds.h"
#include "schema/options_screen.h"
#include "schema/save_screen.h"
#include "schema/stand_models.h"
#include "schema/strings_and_font.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "schema/ui_movies_game.h"
#include "schema/type6_type7_modes.h"

#include <cstdint>

namespace type67_modes {

// A first screen (Type6Screen0 or Type7Screen0) by its address.
struct Screen0View {
  schema::Type6Screen0 t6;
  schema::Type7Screen0 t7;
  bool type7;

  Screen0View(uint32_t screen, bool type7_)
      : t6(screen), t7(screen), type7(type7_) {}

  uint32_t state() const { return type7 ? t7.state() : t6.state(); }
  void set_state(uint32_t v) const { type7 ? t7.set_state(v) : t6.set_state(v); }
  uint32_t loaded() const { return type7 ? t7.loaded() : t6.loaded(); }
  void set_loaded(uint32_t v) const { type7 ? t7.set_loaded(v) : t6.set_loaded(v); }
  uint32_t reenter() const { return type7 ? t7.reenter() : t6.reenter(); }
  schema::Type67Screen0Words words() const { return type7 ? t7.words() : t6.words(); }
  void set_words(uint32_t v) const { type7 ? t7.set_words(v) : t6.set_words(v); }
  schema::PageObject pages() const { return type7 ? t7.pages() : t6.pages(); }
  void set_pages(uint32_t v) const { type7 ? t7.set_pages(v) : t6.set_pages(v); }
  schema::FrontEndMenu menu() const { return type7 ? t7.menu() : t6.menu(); }
  void set_menu(uint32_t v) const { type7 ? t7.set_menu(v) : t6.set_menu(v); }
  uint32_t item_picked() const { return type7 ? t7.item_picked() : t6.item_picked(); }
  void set_item_picked(uint32_t v) const {
    type7 ? t7.set_item_picked(v) : t6.set_item_picked(v);
  }
};

// A mode (Type6Mode or Type7Mode: the same base, the block's type differs).
struct ModeView {
  schema::Type6Mode t6;
  schema::Type7Mode t7;
  bool type7;

  ModeView(uint32_t mode, bool type7_)
      : t6(mode), t7(mode), type7(type7_) {}

  // The block's address (Type6Screens or Type7Screens, type67-screens.toml).
  uint32_t block() const {
    return type7 ? t7.block<schema::View>().address : t6.block<schema::View>().address;
  }
  void set_block(uint32_t v) const { type7 ? t7.set_block(v) : t6.set_block(v); }
  uint32_t ring_count() const { return type7 ? t7.ring_count() : t6.ring_count(); }
  void set_ring_count(uint32_t v) const { type7 ? t7.set_ring_count(v) : t6.set_ring_count(v); }
  uint32_t ring_start() const { return type7 ? t7.ring_start() : t6.ring_start(); }
  // The ring's and the handlers' bytes (guest order in either shape: the
  // views are dual, the modes in host memory since E5's step 4).
  uint8_t* ring() const { return type7 ? t7.ring() : t6.ring(); }
  uint32_t handler_count() const { return type7 ? t7.handler_count() : t6.handler_count(); }
  void set_handler_count(uint32_t v) const {
    type7 ? t7.set_handler_count(v) : t6.set_handler_count(v);
  }
  uint8_t* handlers() const { return type7 ? t7.handlers() : t6.handlers(); }
  // The dispatcher (+36) and the delegate (+8): addresses inside the mode (a
  // handle's interior in host memory).
  uint32_t dispatcher() const { return t6.address + 36; }
  uint32_t delegate() const { return t6.address + 8; }
  void set_delegate_ptr(uint32_t v) const {
    type7 ? t7.set_delegate_ptr(v) : t6.set_delegate_ptr(v);
  }
  void set_delegate_vtable(uint32_t v) const {
    type7 ? t7.set_delegate_vtable(v) : t6.set_delegate_vtable(v);
  }
  void set_delegate_object(uint32_t v) const {
    type7 ? t7.set_delegate_object(v) : t6.set_delegate_object(v);
  }
  void set_delegate_function(uint32_t v) const {
    type7 ? t7.set_delegate_function(v) : t6.set_delegate_function(v);
  }
  void set_delegate_adjustment(uint32_t v) const {
    type7 ? t7.set_delegate_adjustment(v) : t6.set_delegate_adjustment(v);
  }
};

// A mode's screen block (Type6Screens or Type7Screens, type67-screens.toml):
// the words the modes' delegate functions use.
struct BlockView {
  schema::Type6Screens t6;
  schema::Type7Screens t7;
  bool type7;

  BlockView(uint32_t block, bool type7_)
      : t6(block), t7(block), type7(type7_) {}

  uint32_t phase() const { return type7 ? t7.phase() : t6.phase(); }
  void set_phase(uint32_t v) const { type7 ? t7.set_phase(v) : t6.set_phase(v); }
  uint32_t phase_address() const { return type7 ? t7.phase_address() : t6.phase_address(); }
  uint32_t answer_word0() const { return type7 ? t7.answer_word0() : t6.answer_word0(); }
  void set_answer_word0(uint32_t v) const {
    type7 ? t7.set_answer_word0(v) : t6.set_answer_word0(v);
  }
  uint32_t answer_word() const { return type7 ? t7.answer_word() : t6.answer_word(); }
  void set_answer_word(uint32_t v) const {
    type7 ? t7.set_answer_word(v) : t6.set_answer_word(v);
  }
};

// The dialog's status word: type 6's (+9756) or type 7's (+9760).
inline uint32_t DialogStatus(const schema::Type67Dialog& d, bool type7) {
  return type7 ? d.type7_status() : d.type6_status();
}
inline void SetDialogStatus(const schema::Type67Dialog& d, bool type7, uint32_t v) {
  type7 ? d.set_type7_status(v) : d.set_type6_status(v);
}

}  // namespace type67_modes
