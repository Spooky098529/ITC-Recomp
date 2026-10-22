// The game's side of the UI movies natively: adapters in front of
// src/ui_movies/game.cpp (docs/functions/ui-movies-game.md).

#include "guest.h"

#include <bit>

#include "base/flags.h"
#include "c_runtime/strings.h"
#include "hooks/guest_call.h"
#include "hooks/ui_movies_game.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

ITC_FLAG_BOOL(native_ui_movies_game, true, "ITC",
              "Run the game's side of the UI movies natively instead of through the translated "
              "code: the node lookups and setters UiMovie::FindNode sub_8211A578, "
              "UiMovie::SetNodeColor sub_8211A5B8, sub_8211A630, sub_8211A660, sub_8211A6C0, "
              "UiMovie::SetNodeTexture sub_8211A708, UiMovie::ShowNode sub_8211AC00, "
              "UiMovie::HideNode sub_8211AC48, UiMovie::SetPictureTexture sub_8211AAA8, "
              "UiMovie::GetPictureTexture sub_8211AB28, UiMovie::SwapTexture sub_8211AB90; "
              "the pose resets UiMovie::ResetPose sub_8211ABA8 and UiMovie::ResetNode "
              "sub_8211A1E0 (these two with --native_ui_movies and without --log_pause_movie); "
              "the quads' and text items' set-up Quad::Quad sub_8211A948, TextItem::TextItem "
              "sub_8211A818, TextItem::~TextItem sub_8211A8C8 and the copy sub_8211AED0; the "
              "animation lists' sub_8211A9E8, sub_8211B230, AnimationList::Insert "
              "sub_8211B4D0, AnimationList::RemoveAt sub_8211B080, AnimationList::Get "
              "sub_8211B298, UiMovie::AnimationTime sub_8211BA00 and "
              "UiMovie::NamedAnimationTime sub_8211BB18; the pairs' lookups TextMap::Find "
              "sub_8211B310, TextMap::Set sub_8211B370 and UiMovies::Find sub_8211B3F0; the "
              "screens' movie table reset sub_8211AD00 (its renderer calls still the game's); the "
              "layers sub_8211BD10 (its buckets in its frame on the guest stack, as the "
              "original's; the original runs for trees that would overflow them); the texts' "
              "accessors UiMovie::TextString sub_8211B720, sub_8211B770, sub_8211B7B8, "
              "sub_8211B9B8 and sub_8211B940, and UiMovies::IsLoaded sub_8211BE98 "
              "(same results; checked by --run_harnesses=ui_movies_game)");

ITC_FLAG_DECLARE(bool, native_ui_movies);
ITC_FLAG_DECLARE(bool, log_pause_movie);

#define NATIVE_OR_ORIGINAL(address)          \
  if (!ITC_FLAG(native_ui_movies_game)) {    \
    __imp__sub_##address(ctx, base);         \
    return;                                  \
  }

// UiMovie::FindNode: r3 = movie, r4 = a sub-scene's index or -1, r5 = a
// child's index; the node.
extern "C" REX_FUNC(sub_8211A578) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A578);
  ctx.r3.u64 = ui_movies::FindNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// UiMovie::SetNodeColor: the lookup, then color r6; returns the node.
extern "C" REX_FUNC(sub_8211A5B8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A5B8);
  const uint32_t node = ui_movies::FindNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ui_movies::SetColor(base, node, ctx.r6.u32);
  ctx.r3.u64 = node;
}

// sub_8211A630: r4 = a node, r5 = a color (r3 left).
extern "C" REX_FUNC(sub_8211A630) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A630);
  ui_movies::SetColor(base, ctx.r4.u32, ctx.r5.u32);
}

// sub_8211A660: r4 = a node, r5 = a number (r3 left).
extern "C" REX_FUNC(sub_8211A660) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A660);
  ui_movies::SetLayer(base, ctx.r4.u32, ctx.r5.u32);
}

// sub_8211A6C0: r3 = movie (left).
extern "C" REX_FUNC(sub_8211A6C0) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A6C0);
  ui_movies::ClearTextIds(base, ctx.r3.u32);
}

// UiMovie::SetNodeTexture: the lookup, then r6; returns the node.
extern "C" REX_FUNC(sub_8211A708) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A708);
  const uint32_t node = ui_movies::FindNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = ui_movies::SetNodeTexture(base, node, ctx.r6.u32);
}

// UiMovie::ShowNode and HideNode: the lookup, then +80 = 1 or 0; return the
// node.
extern "C" REX_FUNC(sub_8211AC00) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AC00);
  const uint32_t node = ui_movies::FindNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  guest::StoreU32(base, node + 80, 1);
  ctx.r3.u64 = node;
}

extern "C" REX_FUNC(sub_8211AC48) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AC48);
  const uint32_t node = ui_movies::FindNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  guest::StoreU32(base, node + 80, 0);
  ctx.r3.u64 = node;
}

// UiMovie::SetPictureTexture: the lookup (a picture), then its texture in
// the movie's array replaced with r6; returns the old one.
extern "C" REX_FUNC(sub_8211AAA8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AAA8);
  const uint32_t movie = ctx.r3.u32;
  const uint32_t picture = ui_movies::FindNode(base, movie, ctx.r4.u32, ctx.r5.u32);
  const uint32_t slot = ui_movies::PictureTextureSlot(base, movie, picture);
  const uint32_t old = guest::LoadU32(base, slot);
  guest::StoreU32(base, slot, ctx.r6.u32);
  ctx.r3.u64 = old;
}

// UiMovie::GetPictureTexture: the lookup, then that texture.
extern "C" REX_FUNC(sub_8211AB28) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AB28);
  const uint32_t movie = ctx.r3.u32;
  const uint32_t picture = ui_movies::FindNode(base, movie, ctx.r4.u32, ctx.r5.u32);
  ctx.r3.u64 = guest::LoadU32(base, ui_movies::PictureTextureSlot(base, movie, picture));
}

// UiMovie::SwapTexture: r3 = movie, r4 = i, r5 = a texture; the old one.
extern "C" REX_FUNC(sub_8211AB90) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AB90);
  ctx.r3.u64 = ui_movies::SwapTexture(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

namespace {
// The pose resets call UiMovie::RestorePose (sub_82119B80): natively only
// where its override would run it natively (--native_ui_movies, and not
// watched for --log_pause_movie).
bool NativeRestorePose() {
  return ITC_FLAG(native_ui_movies_game) && ITC_FLAG(native_ui_movies) &&
         !ITC_FLAG(log_pause_movie);
}
}  // namespace

// UiMovie::ResetPose: r3 = movie.
extern "C" REX_FUNC(sub_8211ABA8) {
  REX_FUNC_PROLOGUE();
  if (!NativeRestorePose()) {
    __imp__sub_8211ABA8(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::ResetPose(base, ctx.r3.u32);
}

// UiMovie::ResetNode: r3 = movie, r4 = a sub-scene's index or -1, r5 = a
// child's index.
extern "C" REX_FUNC(sub_8211A1E0) {
  REX_FUNC_PROLOGUE();
  if (!NativeRestorePose()) {
    __imp__sub_8211A1E0(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::ResetNode(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Quad::Quad: r3 = the quad; returns it.
extern "C" REX_FUNC(sub_8211A948) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A948);
  ctx.r3.u64 = ui_movies::MakeQuad(base, ctx.r3.u32);
}

// TextItem::TextItem: r3 = the item; returns it.
extern "C" REX_FUNC(sub_8211A818) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A818);
  ctx.r3.u64 = ui_movies::MakeTextItem(base, ctx.r3.u32);
}

// TextItem::~TextItem: r3 = the item; r3 left at item + 16.
extern "C" REX_FUNC(sub_8211A8C8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A8C8);
  ctx.r3.u64 = ui_movies::ClearTextItem(base, ctx.r3.u32);
}

// sub_8211AED0: r3 = destination, r4 = a text item; returns r3.
extern "C" REX_FUNC(sub_8211AED0) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies_game) || !ui_movies::CopyTextItem(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_8211AED0(ctx, base);
  }
}

// sub_8211A9E8: r3 = movie (left).
extern "C" REX_FUNC(sub_8211A9E8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211A9E8);
  ui_movies::InitAnimationLists(base, ctx.r3.u32);
}

// sub_8211B230: r3 = a list (left), r4 = a position (16-bit).
extern "C" REX_FUNC(sub_8211B230) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B230);
  ui_movies::LinkFree(base, ctx.r3.u32, int16_t(ctx.r4.u16));
}

namespace {
// The 40-byte slot copies: the host's where the two can't share memory,
// else the game's memcpy (sub_825A7410) from a frame like the original's.
void CopySlot(PPCContext& ctx, uint8_t* base, uint32_t to, uint32_t from, uint32_t frame_bytes,
              uint32_t return_address) {
  if (c_runtime::Memcpy(base, to, from, 40)) {
    return;
  }
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, frame_bytes, return_address, [&](uint32_t) {
    ctx.r3.u64 = to;
    ctx.r4.u64 = from;
    ctx.r5.u64 = 40;
    sub_825A7410(ctx, base);
  });
  ctx.lr = lr;
}
}  // namespace

// AnimationList::Get: r3 = a 40-byte destination, r4 = a list, r5 = a
// position; returns r3.
extern "C" REX_FUNC(sub_8211B298) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B298);
  const uint32_t to = ctx.r3.u32, list = ctx.r4.u32;
  const uint32_t from = ui_movies::SlotOf(base, list, ui_movies::LinkAt(base, list, ctx.r5.u32));
  CopySlot(ctx, base, to, from, 96, 0x8211B2F4);
  ctx.r3.u64 = to;
}

// UiMovie::AnimationTime: r3 = movie (left), r4 = a sub-scene's index or -1,
// r5 = an animation's index; the time in f1.
extern "C" REX_FUNC(sub_8211BA00) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211BA00);
  const uint32_t time = ui_movies::AnimationTime(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(std::bit_cast<float>(time));
}

// UiMovie::NamedAnimationTime: r3 = movie (left), r4 = a named animation's
// index; the time in f1.
extern "C" REX_FUNC(sub_8211BB18) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211BB18);
  const uint32_t time = ui_movies::NamedAnimationTime(base, ctx.r3.u32, ctx.r4.u32);
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = double(std::bit_cast<float>(time));
}

// TextMap::Find: r3 = a movie's texts, r4 = an id's address.
extern "C" REX_FUNC(sub_8211B310) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B310);
  ctx.r3.u64 = ui_movies::TextMapFind(base, ctx.r3.u32, ctx.r4.u32);
}

// TextMap::Set: r3 = the texts (left), r4 = an id's address, r5 = a text's
// address.
extern "C" REX_FUNC(sub_8211B370) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B370);
  ui_movies::TextMapSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// UiMovies::Find: r3 = a vector of pairs, r4 = the key's address.
extern "C" REX_FUNC(sub_8211B3F0) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B3F0);
  ctx.r3.u64 = ui_movies::PairsFind(base, ctx.r3.u32, ctx.r4.u32);
}

// sub_8211AD00: the renderer and command list from the object at
// [0x826BABA0] through four calls on vtables (slot 8, then slot 1 for the
// list; slot 8, then slot 2 for the renderer), made as the original's from a
// frame like its own; the rest natively. Returns the renderer (r3 as the
// original leaves it).
extern "C" REX_FUNC(sub_8211AD00) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211AD00);
  constexpr uint32_t kScene = 0x826BABA0;
  ui_movies::ClearScreenFlag(base);
  const uint64_t lr = ctx.lr;
  uint32_t list = 0, renderer = 0;
  // A call through slot `slot` of r3's vtable.
  auto call = [&](uint32_t slot, uint32_t return_address) {
    ctx.lr = return_address;
    REX_CALL_INDIRECT_FUNC(
        guest::LoadU32(base, guest::LoadU32(base, ctx.r3.u32) + 4 * slot));
  };
  guest_call::InFrame(ctx, base, 96, 0x8211AD34, [&](uint32_t) {
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    call(8, 0x8211AD34);
    call(1, 0x8211AD44);
    list = ctx.r3.u32;
    ctx.r3.u64 = guest::LoadU32(base, kScene);
    call(8, 0x8211AD60);
    call(2, 0x8211AD70);
    renderer = ctx.r3.u32;
  });
  ctx.lr = lr;
  ui_movies::ResetMovieTable(base, renderer, list);
  ctx.r3.u64 = renderer;
}

// sub_8211BD10: r3 = movie, r4 = the scene, r5 = a group, r6 = a layer, r7
// = 1 to number by bucket.
extern "C" REX_FUNC(sub_8211BD10) {
  REX_FUNC_PROLOGUE();
  if (!ITC_FLAG(native_ui_movies_game) || !ui_movies::LayersFit(base, ctx.r4.u32, ctx.r5.u32)) {
    __imp__sub_8211BD10(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::AssignLayers(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                       ctx.r6.u32, ctx.r7.u32 != 0);
}

// UiMovies::IsLoaded: r3 = a movie id; 1 or 0.
extern "C" REX_FUNC(sub_8211BE98) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211BE98);
  ctx.r3.u64 = ui_movies::IsLoaded(base, ctx.r3.u32);
}

// UiMovie::TextString: r3 = movie, r4 = a text id; the text's buffer.
extern "C" REX_FUNC(sub_8211B720) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B720);
  ctx.r3.u64 = ui_movies::TextString(base, ctx.r3.u32, ctx.r4.u32);
}

// The text's +20, +24 and +4: r3 = movie, r4 = a text id.
extern "C" REX_FUNC(sub_8211B770) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B770);
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 20);
}

extern "C" REX_FUNC(sub_8211B7B8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B7B8);
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 24);
}

extern "C" REX_FUNC(sub_8211B9B8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B9B8);
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 4);
}

// sub_8211B940: r3 = movie, r4 = a text id; r3 left at the movie's texts
// (TextMap::Set's).
extern "C" REX_FUNC(sub_8211B940) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B940);
  const uint32_t movie = ctx.r3.u32;
  ui_movies::AdvanceTextScroll(base, movie, ctx.r4.u32);
  ctx.r3.u64 = movie + ui_movies::kMovieTexts;
}

namespace ui_movies_game_hooks {
void Insert(PPCContext& ctx, uint8_t* base) {
  NATIVE_OR_ORIGINAL(8211B4D0);
  const uint32_t list = ctx.r3.u32, from = ctx.r4.u32, position = ctx.r5.u32;
  ui_movies::LinkFree(base, list, int16_t(uint16_t(position)));
  const uint32_t to = ui_movies::SlotOf(base, list, ui_movies::LinkAt(base, list, position));
  CopySlot(ctx, base, to, from, 112, 0x8211B538);
  ctx.r3.u64 = to;
}

void RemoveAt(PPCContext& ctx, uint8_t* base) {
  NATIVE_OR_ORIGINAL(8211B080);
  ui_movies::RemoveAt(base, ctx.r3.u32, ctx.r4.u32);
}
}  // namespace ui_movies_game_hooks
