// The game's side of the UI movies natively: adapters in front of
// src/ui_movies/game.cpp (docs/functions/ui-movies-game.md).

#include "guest.h"

#include <bit>

#include "base/flags.h"
#include "base/log.h"
#include "c_runtime/strings.h"
#include "hooks/guest_call.h"
#include "hooks/heap.h"
#include "hooks/ui_movies_game.h"
#include "strings_and_font/strings_and_font.h"
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
              "sub_8211B9B8 and sub_8211B940, and UiMovies::IsLoaded sub_8211BE98; the moves "
              "between the animation lists UiMovie::MoveAnimation sub_8211DBE0, "
              "UiMovie::MoveNamedAnimation sub_8211DCE8, UiMovie::Loop sub_8211DDD8, "
              "UiMovie::LoopNamed sub_8211DDE0, UiMovie::LoopLast sub_8211DDE8, the "
              "stops UiMovie::Finish sub_8211DE38, UiMovie::FinishNamed sub_8211DF20 and "
              "UiMovie::FinishAll sub_8211DFF8 (UiMovie::ApplyAnimation through its override), "
              "and the "
              "starts UiMovie::Play sub_8211D978 and UiMovie::PlayNamed sub_8211DAC8 (these "
              "without --log_pause_movie); the texts drawn UiMovie::DrawTexts sub_8211CD90 (its "
              "Text::Draw through that adapter) with UiMovie::ScrollOffset sub_8211B800; "
              "UiMovie::Draw sub_8211D280 and UiMovie::Run sub_8211E1B8 (their calls through "
              "their overrides); with --native_heap and --native_c_runtime_heap, the vectors' "
              "sub_8211B0E8, sub_8211C168 (sub_8211B458, sub_8211B160) and sub_8211AE68 "
              "(sub_8211A788), UiMovie::UiMovie sub_8211E1C0 (sub_8211AE18, sub_8211B018, "
              "sub_8211D210), UiMovie::~UiMovie sub_8211E160 (sub_8211D1B8, sub_8211AF90) and "
              "the texts freed sub_8211AC90, UiMovie::GetText sub_8211B660, and the named animations "
              "added sub_8211E378 (sub_8211E318; its strings through std::string's plain C++) "
              "(same results; checked by --run_harnesses=ui_movies_game and ui_movie_objects). sub_8211A660, "
              "sub_8211B230, AnimationList::Get, UiMovie::ScrollOffset, sub_8211B7B8, sub_8211B9B8 and "
              "sub_8211B940 have no adapters: only native callers reach "
              "them, and the flag switches those");

ITC_FLAG_DECLARE(bool, native_ui_movies);
ITC_FLAG_DECLARE(bool, log_pause_movie);
ITC_FLAG_DECLARE(bool, native_c_runtime_heap);

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

// The text's +20: r3 = movie, r4 = a text id. (Its +24 and +4,
// sub_8211B7B8 and sub_8211B9B8, and the scroll counter's sub_8211B940 have
// no adapters: only DrawTexts' plain C++ calls them in play.)
extern "C" REX_FUNC(sub_8211B770) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211B770);
  ctx.r3.u64 = ui_movies::TextField(base, ctx.r3.u32, ctx.r4.u32, 20);
}

namespace {
// The moves between the animation lists and the starts call
// AnimationList::Insert and RemoveAt (watched for --log_pause_movie):
// natively only where nothing watches them.
bool NativeUnwatched() {
  return ITC_FLAG(native_ui_movies_game) && !ITC_FLAG(log_pause_movie);
}

// The r3 the moves leave: the slot appended to; with no match, the copy in
// their 176-byte frame (+80) when the list had slots, else the movie.
uint64_t MovedR3(const ui_movies::Moved& moved, uint32_t r1, uint32_t movie) {
  if (moved.slot) {
    return *moved.slot;
  }
  return moved.looked ? r1 - 176 + 80 : movie;
}
}  // namespace

#define NATIVE_UNWATCHED_OR_ORIGINAL(address) \
  if (!NativeUnwatched()) {                   \
    __imp__sub_##address(ctx, base);          \
    return;                                   \
  }

// UiMovie::MoveAnimation: r3 = movie, r4 = a sub-scene's index or -1, r5 =
// an animation's index, r6 = 1 to the looping list, 0 to the queue.
extern "C" REX_FUNC(sub_8211DBE0) {
  REX_FUNC_PROLOGUE();
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DBE0);
  const uint32_t movie = ctx.r3.u32;
  ctx.r3.u64 = MovedR3(
      ui_movies::MoveAnimation(base, movie, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32 != 0),
      ctx.r1.u32, movie);
}

// UiMovie::MoveNamedAnimation: r3 = movie, r4 = a named animation's index,
// r5 the direction.
extern "C" REX_FUNC(sub_8211DCE8) {
  REX_FUNC_PROLOGUE();
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DCE8);
  const uint32_t movie = ctx.r3.u32;
  ctx.r3.u64 = MovedR3(ui_movies::MoveNamedAnimation(base, movie, ctx.r4.u32, ctx.r5.u32 != 0),
                       ctx.r1.u32, movie);
}

// UiMovie::LoopNamed: sub_8211DCE8 with r5 = 1.
extern "C" REX_FUNC(sub_8211DDE0) {
  REX_FUNC_PROLOGUE();
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DDE0);
  const uint32_t movie = ctx.r3.u32;
  ctx.r3.u64 = MovedR3(ui_movies::MoveNamedAnimation(base, movie, ctx.r4.u32, true), ctx.r1.u32,
                       movie);
}

// UiMovie::LoopLast: r3 = movie (its tail calls make their frames where
// its caller's ends).
extern "C" REX_FUNC(sub_8211DDE8) {
  REX_FUNC_PROLOGUE();
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DDE8);
  const uint32_t movie = ctx.r3.u32;
  ctx.r3.u64 = MovedR3(ui_movies::LoopLast(base, movie), ctx.r1.u32, movie);
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

void Play(PPCContext& ctx, uint8_t* base) {
  NATIVE_UNWATCHED_OR_ORIGINAL(8211D978);
  ctx.r3.u64 = ui_movies::Play(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32, ctx.r7.u32,
                               ctx.r8.u32, ctx.r9.u32);
}

void PlayNamed(PPCContext& ctx, uint8_t* base) {
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DAC8);
  ctx.r3.u64 = ui_movies::PlayNamed(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                                    ctx.r7.u32, ctx.r8.u32);
}

void DrawTexts(PPCContext& ctx, uint8_t* base) {
  NATIVE_OR_ORIGINAL(8211CD90);
  const uint32_t movie = ctx.r3.u32, list = ctx.r4.u32;
  const bool draw_only = ctx.r5.u32 != 0;
  if (guest::LoadU32(base, list + 8) == 0) {
    return;
  }
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, ui_movies::kDrawTextsFrame, 0x8211CE30, [&](uint32_t frame) {
    const uint32_t item = frame + ui_movies::kDrawTextsItem;
    do {
      const uint32_t from = ui_movies::DrawTextsPop(base, frame, list);
      if (!ui_movies::CopyTextItem(base, item, from)) {
        ctx.r3.u64 = item;
        ctx.r4.u64 = from;
        ctx.lr = 0x8211CE30;
        __imp__sub_8211AED0(ctx, base);
      }
      ui_movies::DrawTextsItem(base, frame, movie, draw_only);
      // Text::Draw (its adapter: the render queue's guest call).
      ctx.r3.u64 = guest::LoadU32(base, strings_and_font::kStrings);
      ctx.r4.u64 = frame + ui_movies::kDrawTextsParams;
      ctx.r5.u64 = frame + ui_movies::kDrawTextsBuffer;
      ctx.lr = 0x8211D190;
      sub_8210A758(ctx, base);
      ctx.fpscr.disableFlushMode();
      ctx.r3.u64 = ui_movies::ClearTextItem(base, item);
    } while (guest::LoadU32(base, list + 8) != 0);
  });
  ctx.lr = lr;
}

void Loop(PPCContext& ctx, uint8_t* base) {
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DDD8);
  const uint32_t movie = ctx.r3.u32;
  ctx.r3.u64 = MovedR3(ui_movies::MoveAnimation(base, movie, ctx.r4.u32, ctx.r5.u32, true),
                       ctx.r1.u32, movie);
}

// UiMovie::Finish and FinishNamed: the animation moved to the queue
// (MoveAnimation or MoveNamedAnimation), then, when the queue has it,
// applied at its end through UiMovie::ApplyAnimation's override and
// removed, from a frame like the original's with its return addresses. r3
// as the original leaves it: the move's, or the queue after the removal.
namespace {
template <typename Move, typename Find>
void FinishIn(PPCContext& ctx, uint8_t* base, uint32_t movie, uint32_t frame_bytes,
              uint32_t move_return, uint32_t apply_return, Move move, Find find) {
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, frame_bytes, move_return, [&](uint32_t frame) {
    ctx.r3.u64 = MovedR3(move(), frame, movie);
    const std::optional<ui_movies::Finishing> found = find();
    if (!found) {
      return;
    }
    ctx.r3.u64 = guest::LoadU32(base, found->slot + 12);
    ctx.r4.u64 = guest::LoadU32(base, found->slot + 16);
    ctx.r6.u64 = 0;
    ctx.fpscr.disableFlushMode();
    ctx.f1.f64 = double(std::bit_cast<float>(found->time));
    ctx.lr = apply_return;
    sub_825A08E0(ctx, base);
    const uint32_t queue = movie + ui_movies::kMovieQueue;
    ui_movies::RemoveAt(base, queue, found->position);
    ctx.r3.u64 = queue;
  });
  ctx.lr = lr;
}
}  // namespace

void Finish(PPCContext& ctx, uint8_t* base) {
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DE38);
  const uint32_t movie = ctx.r3.u32, sub_scene = ctx.r4.u32, index = ctx.r5.u32;
  FinishIn(
      ctx, base, movie, 128, 0x8211DE58, 0x8211DF08,
      [&] { return ui_movies::MoveAnimation(base, movie, sub_scene, index, false); },
      [&] { return ui_movies::FindFinishing(base, movie, sub_scene, index); });
}

void FinishNamed(PPCContext& ctx, uint8_t* base) {
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DF20);
  const uint32_t movie = ctx.r3.u32, named = ctx.r4.u32;
  FinishIn(
      ctx, base, movie, 112, 0x8211DF3C, 0x8211DFE0,
      [&] { return ui_movies::MoveNamedAnimation(base, movie, named, false); },
      [&] { return ui_movies::FindFinishingNamed(base, movie, named); });
}
}  // namespace ui_movies_game_hooks

// UiMovie::Draw (the game's, sub_8211D280): r3 = movie; the pose saved
// (UiMovie::RestorePose), then the drawing as the update's (UiMovie::Draw
// sub_8259FA00, DrawTexts with r5 = 0, sub_8259DAC0 with 678 and 148), each
// through its override from a frame like the original's (112 bytes) with
// its return address. r3 as the last call leaves it.
extern "C" REX_FUNC(sub_8211D280) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211D280);
  ITC_NATIVE_RUNNING("sub_8211D280 UiMovie::Draw");
  const uint32_t movie = ctx.r3.u32;
  const uint64_t lr = ctx.lr;
  guest_call::InFrame(ctx, base, 112, 0x8211D2A4, [&](uint32_t) {
    ctx.r3.u64 = movie;
    ctx.r4.u64 = guest::LoadU32(base, movie + ui_movies::kMovieSaved);
    ctx.r5.u64 = guest::LoadU32(base, movie + ui_movies::kMovieLive);
    ctx.r6.u64 = 0;
    ctx.r7.u64 = 0;
    ctx.lr = 0x8211D2A4;
    sub_82119B80(ctx, base);
    ctx.r3.u64 = movie + 64;
    ctx.r4.u64 = guest::LoadU32(base, movie + ui_movies::kMovieLive);
    ctx.r5.u64 = movie + ui_movies::kMovieTextures;
    ctx.r6.u64 = movie + 40;
    ctx.r7.s64 = -1;
    ctx.lr = 0x8211D2C4;
    sub_8259FA00(ctx, base);
    ctx.r3.u64 = movie;
    ctx.r4.u64 = movie + 40;
    ctx.r5.u64 = 0;
    ctx.lr = 0x8211D2D4;
    sub_8211CD90(ctx, base);
    for (uint32_t k = 0; k < 2; k++) {
      ctx.r3.u64 = movie + 64;
      ctx.r4.u64 = k == 0 ? 678 : 148;
      ctx.lr = k == 0 ? 0x8211D2E0 : 0x8211D2EC;
      sub_8259DAC0(ctx, base);
    }
  });
  ctx.lr = lr;
}

// UiMovie::Run (sub_8211E1B8): a jump to the update (r3 = movie, r4 = 0 to
// advance, else draw only), its override (the frame pacing's); lr kept, so
// the update sees the caller's return address as the original's jump leaves
// it.
extern "C" REX_FUNC(sub_8211E1B8) {
  REX_FUNC_PROLOGUE();
  NATIVE_OR_ORIGINAL(8211E1B8);
  ITC_NATIVE_RUNNING("sub_8211E1B8 UiMovie::Run");
  sub_8211D2F8(ctx, base);
}

// UiMovie::FinishAll (sub_8211DFF8): r3 = movie. Every looping slot moved
// to the queue's end (copied into the frame, +80, unlinked, then
// AnimationList::Insert), then every queued slot copied there, applied at
// its end (UiMovie::ApplyAnimation, r6 = 0, at FinishTime) and unlinked:
// the live scene keeps each one's end pose. Its calls through their
// overrides from a frame like the original's (176 bytes) with its return
// addresses; the copies the host's (the game's memcpy where they might
// share memory). The counts are read once each, 16-bit, sign-extended, and
// counted down to 0 as the original's. r3 as the last call leaves it.
extern "C" REX_FUNC(sub_8211DFF8) {
  REX_FUNC_PROLOGUE();
  NATIVE_UNWATCHED_OR_ORIGINAL(8211DFF8);
  ITC_NATIVE_RUNNING("sub_8211DFF8 UiMovie::FinishAll");
  const uint32_t movie = ctx.r3.u32;
  const uint32_t queue = movie + ui_movies::kMovieQueue;
  const uint32_t looping = movie + ui_movies::kMovieLooping;
  const uint64_t lr = ctx.lr;
  auto count = [&](uint32_t list) {
    return uint32_t(int32_t(int16_t(guest::LoadU16(base, list + 136))));
  };
  guest_call::InFrame(ctx, base, 176, 0x8211E05C, [&](uint32_t frame) {
    const uint32_t copy = frame + 80;
    // The list's first slot copied to the frame (memcpy, r3 its
    // destination).
    auto copy_first = [&](uint32_t list, uint32_t return_address) {
      const uint32_t from = ui_movies::SlotOf(base, list, ui_movies::LinkAt(base, list, 0));
      if (!c_runtime::Memcpy(base, copy, from, 40)) {
        ctx.r3.u64 = copy;
        ctx.r4.u64 = from;
        ctx.r5.u64 = 40;
        ctx.lr = return_address;
        sub_825A7410(ctx, base);
      }
      ctx.r3.u64 = copy;
    };
    uint32_t n = count(looping);
    if (n != 0) {
      do {
        copy_first(looping, 0x8211E05C);
        ui_movies::UnlinkQueueFirst(base, looping);
        ctx.r3.u64 = queue;
        ctx.r4.u64 = copy;
        ctx.r5.s64 = int32_t(count(queue));
        ctx.lr = 0x8211E0A0;
        sub_8211B4D0(ctx, base);
      } while (--n != 0);
    }
    n = count(queue);
    if (n != 0) {
      do {
        copy_first(queue, 0x8211E0F0);
        ctx.r3.u64 = guest::LoadU32(base, copy + 12);
        ctx.r4.u64 = guest::LoadU32(base, copy + 16);
        ctx.r6.u64 = 0;
        ctx.fpscr.disableFlushMode();
        ctx.f1.f64 = double(std::bit_cast<float>(ui_movies::FinishTime(base, copy)));
        ctx.lr = 0x8211E118;
        sub_825A08E0(ctx, base);
        ui_movies::UnlinkQueueFirst(base, queue);
      } while (--n != 0);
    }
  });
  ctx.lr = lr;
}

// --- The movies' vectors and objects (src/ui_movies/objects.cpp) ---
//
// They allocate and free: natively only over the allocation layer, with the
// C runtime's heap native (--native_c_runtime_heap), as the callees' plain
// C++ is called directly; each falls back to the original where its checks
// say so (nothing done then).

namespace {
bool NativeHeap() {
  return ITC_FLAG(native_ui_movies_game) && ITC_FLAG(native_c_runtime_heap) &&
         hooks::NativeHeapActive();
}
}  // namespace

#define NATIVE_HEAP_OR_ORIGINAL(address, call) \
  if (NativeHeap()) {                          \
    if (const auto r3 = (call)) {              \
      ctx.r3.u64 = *r3;                        \
      return;                                  \
    }                                          \
  }                                            \
  __imp__sub_##address(ctx, base);

// sub_8211B0E8: r3 = a vector of pairs; returned.
extern "C" REX_FUNC(sub_8211B0E8) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211B0E8, ui_movies::MakePairs(base, ctx.r3.u32));
}

// sub_8211C168: r3 = a vector of pairs, r4 = the key's address, r5 = the
// value's address.
extern "C" REX_FUNC(sub_8211C168) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211C168,
                          ui_movies::PairsSet(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32),
                                              guest::LoadU32(base, ctx.r5.u32)));
}

// sub_8211AE68: r3 = a vector of words, r4 = the word's address.
extern "C" REX_FUNC(sub_8211AE68) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211AE68,
                          ui_movies::WordsAppend(base, ctx.r3.u32, guest::LoadU32(base, ctx.r4.u32)));
}

// UiMovie::UiMovie: r3 = movie; returned.
extern "C" REX_FUNC(sub_8211E1C0) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211E1C0, ui_movies::ConstructMovie(base, ctx.r3.u32));
}

// UiMovie::~UiMovie: r3 = movie.
extern "C" REX_FUNC(sub_8211E160) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211E160, ui_movies::DestroyMovie(base, ctx.r3.u32));
}

// sub_8211AC90: r3 = movie.
extern "C" REX_FUNC(sub_8211AC90) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211AC90, ui_movies::FreeTexts(base, ctx.r3.u32));
}

// UiMovie::GetText: r3 = movie, r4 = the address of a TXT node's id; the
// text record.
extern "C" REX_FUNC(sub_8211B660) {
  REX_FUNC_PROLOGUE();
  NATIVE_HEAP_OR_ORIGINAL(8211B660, ui_movies::GetText(base, ctx.r3.u32, ctx.r4.u32));
}

// sub_8211E378: r3 = movie, r4 = a scene; its temporaries in a frame like
// the original's below r1.
extern "C" REX_FUNC(sub_8211E378) {
  REX_FUNC_PROLOGUE();
  if (!NativeHeap() || !ui_movies::CanAllocate(base, 0)) {
    __imp__sub_8211E378(ctx, base);
    return;
  }
  ctx.r3.u64 = ui_movies::AddNamedAnimations(base, ctx.r1.u32, ctx.r3.u32, ctx.r4.u32);
}
