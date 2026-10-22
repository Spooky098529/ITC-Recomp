// The game's side of the UI movies natively: adapters in front of
// src/ui_movies/game.cpp (docs/functions/ui-movies-game.md).

#include "guest.h"

#include "base/flags.h"
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
              "sub_8211A818, TextItem::~TextItem sub_8211A8C8 and the copy sub_8211AED0 "
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
