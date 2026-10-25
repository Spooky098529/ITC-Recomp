// The UI movie library's adapters that other overrides call
// (src/hooks/ui_movies.cpp).

#pragma once

#include "guest.h"

namespace ui_movies_hooks {

// sub_8259DAC0's work (r4 = a key; r3 not read): sub_8259D7B0 with r3 = r4
// and f1 = the float at 0x82001C00, as a tail call. Natively or, with
// --native_ui_movies off, the original. Its override (WATCH_MOVIE_CALL's,
// src/debug/pause_movie_log.cpp) wraps this for --log_pause_movie.
void ResetAfterMovie(PPCContext& ctx, uint8_t* base);

// UiMovie::RestorePose (sub_82119B80; r3 = the movie, r4 and r5 = the live
// and rest scenes, r6 and r7 = a live and rest group or 0): natively or,
// with --native_ui_movies off, the original. Its override
// (src/debug/pause_movie_log.cpp) runs the original, watched, for
// --log_pause_movie.
void RestorePose(PPCContext& ctx, uint8_t* base);

// UiMovie::DrawPicture (sub_8259ED40) and UiMovie::DrawPrimitive
// (sub_8259F348; r3 = the draw context, r5 = the node, r6 = the layer):
// natively (the quad built by the plain C++, then handed to sub_8259EBE8's
// adapter from a frame like the original's, with its return address) or,
// with --native_ui_movies off, the original. r3 as sub_8259EBE8 leaves it.
void DrawPicture(PPCContext& ctx, uint8_t* base);
void DrawPrimitive(PPCContext& ctx, uint8_t* base);
// The same natively, whatever the flag (for the harness).
void RecordPicture(PPCContext& ctx, uint8_t* base);
void RecordPrimitive(PPCContext& ctx, uint8_t* base);

// UiMovie::Draw (sub_8259FA00; r3 = the draw context, r4 = the scene, r5
// = the textures, r6 = the text list, r7 = -1): the context's +96, +100 and
// +104 set, then UiMovie::DrawNode natively (ui_movies::DrawNode, its
// pictures and primitives through DrawPicture and DrawPrimitive above, a
// text that would grow its list through the original QueueText, each from
// the level's frame with the original's return address) or, with
// --native_ui_movies off, the original. Its override (WATCH_MOVIE_CALL's,
// src/debug/pause_movie_log.cpp) wraps this for --log_pause_movie.
void Draw(PPCContext& ctx, uint8_t* base);
// UiMovie::DrawNode natively, whatever the flag (r3 = the draw context, r4
// = the scene, r5 = the node, r6 = the layer; for the harness).
void RecordDrawNode(PPCContext& ctx, uint8_t* base);

// UiMovie::ApplyTrack (sub_825A0548; r3 = the node, r4 = the track, f1 = t,
// r5 and r6 = the keys): natively (with native atexit, hooks::NativeAtexit)
// or, with --native_ui_movies off, the original. Its override
// (src/frame_pacing/per_frame_logic.cpp) wraps this for
// --per_tick_ui_movie_tracks and --log_pause_movie.
void ApplyTrack(PPCContext& ctx, uint8_t* base);
// The same natively, whatever the flags (for the harness).
void ApplyTrackNative(PPCContext& ctx, uint8_t* base);

// UiMovie::ApplyAnimation (sub_825A08E0; r3 = the node, r4 = the animation,
// f1 = the time, r6 = loop) natively, whatever the flag (for the harness):
// each track through sub_825A0548 (its override) from the level's frame.
void ApplyAnimationNative(PPCContext& ctx, uint8_t* base);

// UiMovie::Update (sub_8211D2F8; r3 = movie, r4 = 0 to advance a frame and
// draw, else draw only): natively (the time steps and the lists' moves as
// plain C++, its calls through their overrides from a frame like the
// original's, with its return addresses) or, with --native_ui_movies off,
// the original. r3 1 while an animation plays or is queued, else 0. Its
// override (src/frame_pacing/per_frame_logic.cpp) wraps this.
void Update(PPCContext& ctx, uint8_t* base);

// For harnesses: while set, this is asked first by the overrides of the
// functions UiMovie::Update calls (UiMovie::ApplyAnimation sub_825A08E0,
// UiMovie::Draw sub_8259FA00, UiMovie::DrawTexts sub_8211CD90, sub_8259DAC0
// and UiMovie::RestorePose sub_82119B80), of the update itself
// (sub_8211D2F8, for UiMovie::Run's cases) and of sub_8259EBE8 (for
// sub_8211BEE8's), by address, and runs instead of them when it returns
// true.
using UpdateCalleeStandIn = bool (*)(uint32_t function, PPCContext& ctx, uint8_t* base);
extern UpdateCalleeStandIn g_update_callee_stand_in;

}  // namespace ui_movies_hooks
