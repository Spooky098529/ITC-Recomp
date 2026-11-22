// The views of docs/functions/tuning-pages.md's types (docs/e5-native.md, step 2):
// written by itc_recomp/tools/schema_gen.py from itc_recomp/schema/tuning-pages.toml.
// Don't edit it by hand: edit the schema and run the tool again.

#pragma once

#include "schema/types.h"

namespace schema {

// embedded; measure: UiMovies::Find.ret.+0.+28
struct UiMovieTextureTable : View {
  using View::View;
  static constexpr uint32_t kSize = 2048;
  // 512 texture pointers (UiMovieTexture), by index
  uint32_t slots_address() const { return address + 0; }
  template <typename T = UiMovieTexture>
  T texture_8() const { return T(base, guest::LoadU32(base, address + 32)); }
  void set_texture_8(const View& v) const { guest::StoreU32(base, address + 32, v.address); }
  void set_texture_8(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t texture_8_address() const { return address + 32; }
  template <typename T = UiMovieTexture>
  T texture_9() const { return T(base, guest::LoadU32(base, address + 36)); }
  void set_texture_9(const View& v) const { guest::StoreU32(base, address + 36, v.address); }
  void set_texture_9(uint32_t v) const { guest::StoreU32(base, address + 36, v); }
  uint32_t texture_9_address() const { return address + 36; }
  template <typename T = UiMovieTexture>
  T texture_159() const { return T(base, guest::LoadU32(base, address + 636)); }
  void set_texture_159(const View& v) const { guest::StoreU32(base, address + 636, v.address); }
  void set_texture_159(uint32_t v) const { guest::StoreU32(base, address + 636, v); }
  uint32_t texture_159_address() const { return address + 636; }
  // movie 13: the vinyl images' first (to 165)
  template <typename T = UiMovieTexture>
  T texture_160() const { return T(base, guest::LoadU32(base, address + 640)); }
  void set_texture_160(const View& v) const { guest::StoreU32(base, address + 640, v.address); }
  void set_texture_160(uint32_t v) const { guest::StoreU32(base, address + 640, v); }
  uint32_t texture_160_address() const { return address + 640; }
  template <typename T = UiMovieTexture>
  T texture_161() const { return T(base, guest::LoadU32(base, address + 644)); }
  void set_texture_161(const View& v) const { guest::StoreU32(base, address + 644, v.address); }
  void set_texture_161(uint32_t v) const { guest::StoreU32(base, address + 644, v); }
  uint32_t texture_161_address() const { return address + 644; }
  template <typename T = UiMovieTexture>
  T texture_162() const { return T(base, guest::LoadU32(base, address + 648)); }
  void set_texture_162(const View& v) const { guest::StoreU32(base, address + 648, v.address); }
  void set_texture_162(uint32_t v) const { guest::StoreU32(base, address + 648, v); }
  uint32_t texture_162_address() const { return address + 648; }
  template <typename T = UiMovieTexture>
  T texture_163() const { return T(base, guest::LoadU32(base, address + 652)); }
  void set_texture_163(const View& v) const { guest::StoreU32(base, address + 652, v.address); }
  void set_texture_163(uint32_t v) const { guest::StoreU32(base, address + 652, v); }
  uint32_t texture_163_address() const { return address + 652; }
  template <typename T = UiMovieTexture>
  T texture_164() const { return T(base, guest::LoadU32(base, address + 656)); }
  void set_texture_164(const View& v) const { guest::StoreU32(base, address + 656, v.address); }
  void set_texture_164(uint32_t v) const { guest::StoreU32(base, address + 656, v); }
  uint32_t texture_164_address() const { return address + 656; }
  template <typename T = UiMovieTexture>
  T texture_165() const { return T(base, guest::LoadU32(base, address + 660)); }
  void set_texture_165(const View& v) const { guest::StoreU32(base, address + 660, v.address); }
  void set_texture_165(uint32_t v) const { guest::StoreU32(base, address + 660, v); }
  uint32_t texture_165_address() const { return address + 660; }
  template <typename T = UiMovieTexture>
  T texture_168() const { return T(base, guest::LoadU32(base, address + 672)); }
  void set_texture_168(const View& v) const { guest::StoreU32(base, address + 672, v.address); }
  void set_texture_168(uint32_t v) const { guest::StoreU32(base, address + 672, v); }
  uint32_t texture_168_address() const { return address + 672; }
  template <typename T = UiMovieTexture>
  T texture_169() const { return T(base, guest::LoadU32(base, address + 676)); }
  void set_texture_169(const View& v) const { guest::StoreU32(base, address + 676, v.address); }
  void set_texture_169(uint32_t v) const { guest::StoreU32(base, address + 676, v); }
  uint32_t texture_169_address() const { return address + 676; }
  template <typename T = UiMovieTexture>
  T texture_170() const { return T(base, guest::LoadU32(base, address + 680)); }
  void set_texture_170(const View& v) const { guest::StoreU32(base, address + 680, v.address); }
  void set_texture_170(uint32_t v) const { guest::StoreU32(base, address + 680, v); }
  uint32_t texture_170_address() const { return address + 680; }
  template <typename T = UiMovieTexture>
  T texture_171() const { return T(base, guest::LoadU32(base, address + 684)); }
  void set_texture_171(const View& v) const { guest::StoreU32(base, address + 684, v.address); }
  void set_texture_171(uint32_t v) const { guest::StoreU32(base, address + 684, v); }
  uint32_t texture_171_address() const { return address + 684; }
  template <typename T = UiMovieTexture>
  T texture_172() const { return T(base, guest::LoadU32(base, address + 688)); }
  void set_texture_172(const View& v) const { guest::StoreU32(base, address + 688, v.address); }
  void set_texture_172(uint32_t v) const { guest::StoreU32(base, address + 688, v); }
  uint32_t texture_172_address() const { return address + 688; }
  template <typename T = UiMovieTexture>
  T texture_173() const { return T(base, guest::LoadU32(base, address + 692)); }
  void set_texture_173(const View& v) const { guest::StoreU32(base, address + 692, v.address); }
  void set_texture_173(uint32_t v) const { guest::StoreU32(base, address + 692, v); }
  uint32_t texture_173_address() const { return address + 692; }
  template <typename T = UiMovieTexture>
  T texture_175() const { return T(base, guest::LoadU32(base, address + 700)); }
  void set_texture_175(const View& v) const { guest::StoreU32(base, address + 700, v.address); }
  void set_texture_175(uint32_t v) const { guest::StoreU32(base, address + 700, v); }
  uint32_t texture_175_address() const { return address + 700; }
  template <typename T = UiMovieTexture>
  T texture_177() const { return T(base, guest::LoadU32(base, address + 708)); }
  void set_texture_177(const View& v) const { guest::StoreU32(base, address + 708, v.address); }
  void set_texture_177(uint32_t v) const { guest::StoreU32(base, address + 708, v); }
  uint32_t texture_177_address() const { return address + 708; }
  template <typename T = UiMovieTexture>
  T texture_178() const { return T(base, guest::LoadU32(base, address + 712)); }
  void set_texture_178(const View& v) const { guest::StoreU32(base, address + 712, v.address); }
  void set_texture_178(uint32_t v) const { guest::StoreU32(base, address + 712, v); }
  uint32_t texture_178_address() const { return address + 712; }
  template <typename T = UiMovieTexture>
  T texture_181() const { return T(base, guest::LoadU32(base, address + 724)); }
  void set_texture_181(const View& v) const { guest::StoreU32(base, address + 724, v.address); }
  void set_texture_181(uint32_t v) const { guest::StoreU32(base, address + 724, v); }
  uint32_t texture_181_address() const { return address + 724; }
  template <typename T = UiMovieTexture>
  T texture_183() const { return T(base, guest::LoadU32(base, address + 732)); }
  void set_texture_183(const View& v) const { guest::StoreU32(base, address + 732, v.address); }
  void set_texture_183(uint32_t v) const { guest::StoreU32(base, address + 732, v); }
  uint32_t texture_183_address() const { return address + 732; }
  template <typename T = UiMovieTexture>
  T texture_184() const { return T(base, guest::LoadU32(base, address + 736)); }
  void set_texture_184(const View& v) const { guest::StoreU32(base, address + 736, v.address); }
  void set_texture_184(uint32_t v) const { guest::StoreU32(base, address + 736, v); }
  uint32_t texture_184_address() const { return address + 736; }
  // 4 x (191 + layer): a vinyl layer's texture
  template <typename T = UiMovieTexture>
  T texture_191() const { return T(base, guest::LoadU32(base, address + 764)); }
  void set_texture_191(const View& v) const { guest::StoreU32(base, address + 764, v.address); }
  void set_texture_191(uint32_t v) const { guest::StoreU32(base, address + 764, v); }
  uint32_t texture_191_address() const { return address + 764; }
  template <typename T = UiMovieTexture>
  T texture_192() const { return T(base, guest::LoadU32(base, address + 768)); }
  void set_texture_192(const View& v) const { guest::StoreU32(base, address + 768, v.address); }
  void set_texture_192(uint32_t v) const { guest::StoreU32(base, address + 768, v); }
  uint32_t texture_192_address() const { return address + 768; }
  template <typename T = UiMovieTexture>
  T texture_193() const { return T(base, guest::LoadU32(base, address + 772)); }
  void set_texture_193(const View& v) const { guest::StoreU32(base, address + 772, v.address); }
  void set_texture_193(uint32_t v) const { guest::StoreU32(base, address + 772, v); }
  uint32_t texture_193_address() const { return address + 772; }
  // 4 x (194 + layer)
  template <typename T = UiMovieTexture>
  T texture_194() const { return T(base, guest::LoadU32(base, address + 776)); }
  void set_texture_194(const View& v) const { guest::StoreU32(base, address + 776, v.address); }
  void set_texture_194(uint32_t v) const { guest::StoreU32(base, address + 776, v); }
  uint32_t texture_194_address() const { return address + 776; }
  template <typename T = UiMovieTexture>
  T texture_195() const { return T(base, guest::LoadU32(base, address + 780)); }
  void set_texture_195(const View& v) const { guest::StoreU32(base, address + 780, v.address); }
  void set_texture_195(uint32_t v) const { guest::StoreU32(base, address + 780, v); }
  uint32_t texture_195_address() const { return address + 780; }
  template <typename T = UiMovieTexture>
  T texture_196() const { return T(base, guest::LoadU32(base, address + 784)); }
  void set_texture_196(const View& v) const { guest::StoreU32(base, address + 784, v.address); }
  void set_texture_196(uint32_t v) const { guest::StoreU32(base, address + 784, v); }
  uint32_t texture_196_address() const { return address + 784; }
};

// embedded; measure: UiMovies::Find.ret.+0.+28.+640, UiMovies::Find.ret.+0.+28.+644, UiMovies::Find.ret.+0.+28.+648, UiMovies::Find.ret.+0.+28.+652, UiMovies::Find.ret.+0.+28.+656, UiMovies::Find.ret.+0.+28.+660, UiMovies::Find.ret.+0.+28.+672, UiMovies::Find.ret.+0.+28.+676, UiMovies::Find.ret.+0.+28.+680, UiMovies::Find.ret.+0.+28.+684, UiMovies::Find.ret.+0.+28.+688, UiMovies::Find.ret.+0.+28.+692, UiMovies::Find.ret.+0.+28.+700, UiMovies::Find.ret.+0.+28.+708, UiMovies::Find.ret.+0.+28.+712, UiMovies::Find.ret.+0.+28.+724, UiMovies::Find.ret.+0.+28.+732, UiMovies::Find.ret.+0.+28.+736, UiMovies::Find.ret.+0.+28.+764, UiMovies::Find.ret.+0.+28.+768, UiMovies::Find.ret.+0.+28.+772, UiMovies::Find.ret.+0.+28.+776, UiMovies::Find.ret.+0.+28.+780, UiMovies::Find.ret.+0.+28.+784, UiMovies::Find.ret.+0.+28.+636
struct UiMovieTexture : View {
  using View::View;
  static constexpr uint32_t kSize = 12;
  // the texture (header)
  uint32_t texture() const { return guest::LoadU32(base, address + 0); }
  void set_texture(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_address() const { return address + 0; }
};

// embedded; measure: DecalImages, TuningScreen.+10048, DecalImages::Load.r3, DecalImages::SwapKept.r3, DecalImages::Request.r3, DecalImages::Reload.r3, DecalImages::Unload.r3, DecalImages::Scroll.r3
struct DecalImages : View {
  using View::View;
  static constexpr uint32_t kSize = 8856;
  // 200 DecalImage entries: page 0's window 12 to 20, page 1's 33 to 41 and its group's 42 to 50, the layers' 0 to 8 and 21 to 29
  uint32_t entries_address() const { return address + 0; }
  // a DecalImage used for swaps
  uint32_t swap_entry_address() const { return address + 7200; }
  // 200 kept textures of 8 bytes (the texture's header and data, as an entry's +28 and +32)
  uint32_t kept_address() const { return address + 7236; }
  // movie 13's placeholder texture (w 0; DecalImages::SavePlaceholders)
  uint32_t vinyl_placeholder_0() const { return guest::LoadU32(base, address + 8836); }
  void set_vinyl_placeholder_0(uint32_t v) const { guest::StoreU32(base, address + 8836, v); }
  uint32_t vinyl_placeholder_0_address() const { return address + 8836; }
  // movie 13's placeholder texture (w 1)
  uint32_t vinyl_placeholder_1() const { return guest::LoadU32(base, address + 8840); }
  void set_vinyl_placeholder_1(uint32_t v) const { guest::StoreU32(base, address + 8840, v); }
  uint32_t vinyl_placeholder_1_address() const { return address + 8840; }
  // movie 14's placeholder texture (w 2)
  uint32_t sticker_placeholder_0() const { return guest::LoadU32(base, address + 8844); }
  void set_sticker_placeholder_0(uint32_t v) const { guest::StoreU32(base, address + 8844, v); }
  uint32_t sticker_placeholder_0_address() const { return address + 8844; }
  // movie 14's placeholder texture (w 3)
  uint32_t sticker_placeholder_1() const { return guest::LoadU32(base, address + 8848); }
  void set_sticker_placeholder_1(uint32_t v) const { guest::StoreU32(base, address + 8848, v); }
  uint32_t sticker_placeholder_1_address() const { return address + 8848; }
  // 0 at reset
  uint32_t unknown_8852() const { return guest::LoadU32(base, address + 8852); }
  void set_unknown_8852(uint32_t v) const { guest::StoreU32(base, address + 8852, v); }
  uint32_t unknown_8852_address() const { return address + 8852; }
};

// embedded; measure: DecalImages::OnLoaded.r6
struct DecalImageEntry : View {
  using View::View;
  static constexpr uint32_t kSize = 36;
  uint32_t asset_id() const { return guest::LoadU32(base, address + 0); }
  void set_asset_id(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t asset_id_address() const { return address + 0; }
  // the archive request's handle
  uint32_t handle() const { return guest::LoadU32(base, address + 4); }
  void set_handle(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t handle_address() const { return address + 4; }
  // 0 none, 1 loading, 2 loaded
  uint32_t state() const { return guest::LoadU32(base, address + 8); }
  void set_state(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t state_address() const { return address + 8; }
  // the TGA buffer
  uint32_t file() const { return guest::LoadU32(base, address + 12); }
  void set_file(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t file_address() const { return address + 12; }
  uint32_t file_size() const { return guest::LoadU32(base, address + 16); }
  void set_file_size(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t file_size_address() const { return address + 16; }
  // loaded and not yet made a texture
  uint32_t to_make() const { return guest::LoadU32(base, address + 20); }
  void set_to_make(uint32_t v) const { guest::StoreU32(base, address + 20, v); }
  uint32_t to_make_address() const { return address + 20; }
  uint32_t requested() const { return guest::LoadU32(base, address + 24); }
  void set_requested(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t requested_address() const { return address + 24; }
  uint32_t texture_header() const { return guest::LoadU32(base, address + 28); }
  void set_texture_header(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t texture_header_address() const { return address + 28; }
  uint32_t texture_data() const { return guest::LoadU32(base, address + 32); }
  void set_texture_data(uint32_t v) const { guest::StoreU32(base, address + 32, v); }
  uint32_t texture_data_address() const { return address + 32; }
};

// embedded; measure: -
struct DecalImageKept : View {
  using View::View;
  static constexpr uint32_t kSize = 8;
  uint32_t texture_header() const { return guest::LoadU32(base, address + 0); }
  void set_texture_header(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t texture_header_address() const { return address + 0; }
  uint32_t texture_data() const { return guest::LoadU32(base, address + 4); }
  void set_texture_data(uint32_t v) const { guest::StoreU32(base, address + 4, v); }
  uint32_t texture_data_address() const { return address + 4; }
};

// file; measure: sub_8259D628.ret
struct UiMovieVertex : View {
  using View::View;
  static constexpr uint32_t kSize = 32;
  // also copied as a word
  float x() const { return guest::LoadFloat(base, address + 0); }
  void set_x(float v) const { guest::StoreFloat(base, address + 0, v); }
  uint32_t x_address() const { return address + 0; }
  // also copied as a word
  float y() const { return guest::LoadFloat(base, address + 4); }
  void set_y(float v) const { guest::StoreFloat(base, address + 4, v); }
  uint32_t y_address() const { return address + 4; }
  // 0 to 255
  uint32_t red() const { return guest::LoadU32(base, address + 8); }
  void set_red(uint32_t v) const { guest::StoreU32(base, address + 8, v); }
  uint32_t red_address() const { return address + 8; }
  uint32_t green() const { return guest::LoadU32(base, address + 12); }
  void set_green(uint32_t v) const { guest::StoreU32(base, address + 12, v); }
  uint32_t green_address() const { return address + 12; }
  uint32_t blue() const { return guest::LoadU32(base, address + 16); }
  void set_blue(uint32_t v) const { guest::StoreU32(base, address + 16, v); }
  uint32_t blue_address() const { return address + 16; }
  // probably
  float alpha() const { return guest::LoadFloat(base, address + 20); }
  void set_alpha(float v) const { guest::StoreFloat(base, address + 20, v); }
  uint32_t alpha_address() const { return address + 20; }
  // probably a texture coordinate
  uint32_t unknown_24() const { return guest::LoadU32(base, address + 24); }
  void set_unknown_24(uint32_t v) const { guest::StoreU32(base, address + 24, v); }
  uint32_t unknown_24_address() const { return address + 24; }
  // probably a texture coordinate
  uint32_t unknown_28() const { return guest::LoadU32(base, address + 28); }
  void set_unknown_28(uint32_t v) const { guest::StoreU32(base, address + 28, v); }
  uint32_t unknown_28_address() const { return address + 28; }
};

// embedded; measure: TuningScreen::Leave.r4
struct TuningPagesMode : View {
  using View::View;
  static constexpr uint32_t kSize = 812;
  // 50 messages of 12 bytes (ModeMessage: id, two words)
  uint32_t messages_address() const { return address + 204; }
  uint32_t message_count() const { return guest::LoadU32(base, address + 804); }
  void set_message_count(uint32_t v) const { guest::StoreU32(base, address + 804, v); }
  uint32_t message_count_address() const { return address + 804; }
  // the ring's first slot
  uint32_t message_start() const { return guest::LoadU32(base, address + 808); }
  void set_message_start(uint32_t v) const { guest::StoreU32(base, address + 808, v); }
  uint32_t message_start_address() const { return address + 808; }
};

// embedded; measure: TuningScreen.+33548, TuningScreen.+33576
struct TuningPagesScreenWord : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  // measure split: the screen's own word
  uint32_t value() const { return guest::LoadU32(base, address + 0); }
  void set_value(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t value_address() const { return address + 0; }
};

// embedded; measure: TuningScreen.+10048.+0, TuningScreen.+33548.+0, TuningScreen.+33576.+0
struct TuningPagesIndexFault : View {
  using View::View;
  static constexpr uint32_t kSize = 44;
  // measure fault: a table word of the screen indexed by the value, not a field
  uint32_t fault_40() const { return guest::LoadU32(base, address + 40); }
  void set_fault_40(uint32_t v) const { guest::StoreU32(base, address + 40, v); }
  uint32_t fault_40_address() const { return address + 40; }
};

// embedded; measure: DecalImages@+28
struct DecalImagesAt28 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// embedded; measure: DecalImages@+32
struct DecalImagesAt32 : View {
  using View::View;
  static constexpr uint32_t kSize = 4;
  uint32_t unknown_0() const { return guest::LoadU32(base, address + 0); }
  void set_unknown_0(uint32_t v) const { guest::StoreU32(base, address + 0, v); }
  uint32_t unknown_0_address() const { return address + 0; }
};

// The image words docs/functions/tuning-pages.md's code uses.
struct TuningPagesGlobals {
  // layer 0's fade-in count (0 once done)
  static uint32_t fade_in_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50DCu); }
  static void set_fade_in_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50DCu, v); }
  static constexpr uint32_t fade_in_counts_address() { return 0x826A50DCu; }
  // layer 0's fade-out count
  static uint32_t fade_out_counts(uint8_t* base) { return guest::LoadU32(base, 0x826A50E4u); }
  static void set_fade_out_counts(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826A50E4u, v); }
  static constexpr uint32_t fade_out_counts_address() { return 0x826A50E4u; }
  // the fade-in's pass count
  static uint32_t fade_in_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BECu); }
  static void set_fade_in_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BECu, v); }
  static constexpr uint32_t fade_in_passes_address() { return 0x82001BECu; }
  // the fade-out's pass count
  static uint32_t fade_out_passes(uint8_t* base) { return guest::LoadU32(base, 0x82001BF0u); }
  static void set_fade_out_passes(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82001BF0u, v); }
  static constexpr uint32_t fade_out_passes_address() { return 0x82001BF0u; }
  // the layer's node shown when set
  static uint32_t show_picker_node(uint8_t* base) { return guest::LoadU32(base, 0x82636358u); }
  static void set_show_picker_node(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x82636358u, v); }
  static constexpr uint32_t show_picker_node_address() { return 0x82636358u; }
  // InputVinylPages' kept page's counter
  static uint32_t keep_counter(uint8_t* base) { return guest::LoadU32(base, 0x826D84A4u); }
  static void set_keep_counter(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84A4u, v); }
  static constexpr uint32_t keep_counter_address() { return 0x826D84A4u; }
  // a group change's frame count
  static uint32_t group_wait(uint8_t* base) { return guest::LoadU32(base, 0x826D84A8u); }
  static void set_group_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84A8u, v); }
  static constexpr uint32_t group_wait_address() { return 0x826D84A8u; }
  // the scroll's direction (1 left, 2 right)
  static uint32_t scroll_way(uint8_t* base) { return guest::LoadU32(base, 0x826D84ACu); }
  static void set_scroll_way(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84ACu, v); }
  static constexpr uint32_t scroll_way_address() { return 0x826D84ACu; }
  // a scroll's loads awaited
  static uint32_t scroll_wait(uint8_t* base) { return guest::LoadU32(base, 0x826D84B0u); }
  static void set_scroll_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84B0u, v); }
  static constexpr uint32_t scroll_wait_address() { return 0x826D84B0u; }
  // A's: the pick's loads awaited
  static uint32_t pick_wait(uint8_t* base) { return guest::LoadU32(base, 0x826D84B4u); }
  static void set_pick_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84B4u, v); }
  static constexpr uint32_t pick_wait_address() { return 0x826D84B4u; }
  // InputVinylLayers' load wait (a frame count)
  static uint32_t layers_wait(uint8_t* base) { return guest::LoadU32(base, 0x826D84B8u); }
  static void set_layers_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84B8u, v); }
  static constexpr uint32_t layers_wait_address() { return 0x826D84B8u; }
  // InputLayerActions' load wait (a frame count)
  static uint32_t actions_wait(uint8_t* base) { return guest::LoadU32(base, 0x826D84BCu); }
  static void set_actions_wait(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84BCu, v); }
  static constexpr uint32_t actions_wait_address() { return 0x826D84BCu; }
  // InputLayerOrder's frame count
  static uint32_t order_count(uint8_t* base) { return guest::LoadU32(base, 0x826D84C0u); }
  static void set_order_count(uint8_t* base, uint32_t v) { guest::StoreU32(base, 0x826D84C0u, v); }
  static constexpr uint32_t order_count_address() { return 0x826D84C0u; }
  // 1.0
  static float one(uint8_t* base) { return guest::LoadFloat(base, 0x82000A30u); }
  static void set_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A30u, v); }
  static constexpr uint32_t one_address() { return 0x82000A30u; }
  // 0.0
  static float zero(uint8_t* base) { return guest::LoadFloat(base, 0x82000A34u); }
  static void set_zero(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000A34u, v); }
  static constexpr uint32_t zero_address() { return 0x82000A34u; }
  // probably -1.0
  static float minus_one(uint8_t* base) { return guest::LoadFloat(base, 0x82001BE0u); }
  static void set_minus_one(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001BE0u, v); }
  static constexpr uint32_t minus_one_address() { return 0x82001BE0u; }
  // 40.0, a bar's step
  static float bar_step(uint8_t* base) { return guest::LoadFloat(base, 0x82001FB0u); }
  static void set_bar_step(uint8_t* base, float v) { guest::StoreFloat(base, 0x82001FB0u, v); }
  static constexpr uint32_t bar_step_address() { return 0x82001FB0u; }
  // probably 1/255
  static float byte_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002200u); }
  static void set_byte_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002200u, v); }
  static constexpr uint32_t byte_scale_address() { return 0x82002200u; }
  // probably 255.0
  static float byte_max(uint8_t* base) { return guest::LoadFloat(base, 0x82000C44u); }
  static void set_byte_max(uint8_t* base, float v) { guest::StoreFloat(base, 0x82000C44u, v); }
  static constexpr uint32_t byte_max_address() { return 0x82000C44u; }
  // the light bars' first scale
  static float bar_short(uint8_t* base) { return guest::LoadFloat(base, 0x82002C30u); }
  static void set_bar_short(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002C30u, v); }
  static constexpr uint32_t bar_short_address() { return 0x82002C30u; }
  // the light bars' second scale
  static float bar_long(uint8_t* base) { return guest::LoadFloat(base, 0x8200B5CCu); }
  static void set_bar_long(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200B5CCu, v); }
  static constexpr uint32_t bar_long_address() { return 0x8200B5CCu; }
  // the light bars' length
  static float bar_length(uint8_t* base) { return guest::LoadFloat(base, 0x820021FCu); }
  static void set_bar_length(uint8_t* base, float v) { guest::StoreFloat(base, 0x820021FCu, v); }
  static constexpr uint32_t bar_length_address() { return 0x820021FCu; }
  // StoreStickers' mirrored angle's offset
  static float mirror_angle(uint8_t* base) { return guest::LoadFloat(base, 0x82002244u); }
  static void set_mirror_angle(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002244u, v); }
  static constexpr uint32_t mirror_angle_address() { return 0x82002244u; }
  // pi/180: the decals' angle scale on store
  static float store_scale(uint8_t* base) { return guest::LoadFloat(base, 0x82002948u); }
  static void set_store_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x82002948u, v); }
  static constexpr uint32_t store_scale_address() { return 0x82002948u; }
  // the decals' angle scale on load (probably 180/pi)
  static float load_scale(uint8_t* base) { return guest::LoadFloat(base, 0x8200294Cu); }
  static void set_load_scale(uint8_t* base, float v) { guest::StoreFloat(base, 0x8200294Cu, v); }
  static constexpr uint32_t load_scale_address() { return 0x8200294Cu; }
};

}  // namespace schema
