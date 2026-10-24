// The game's movie class (`Movie`, vtable 0x82002910, docs/functions/movies.md)
// on our own player (movies/player.h) instead of the movie library linked into
// the game: its slots 0-7 as plain C++ over the guest object. The object keeps
// its state at +308 (0 none, 1 loaded or ended, 2 playing, 3 paused), which
// the game reads, and its path at +48; the player is ours, one a movie
// object. The open's command at +12 is no longer run: its status at +24 is
// set to 3 (done) when the player is opened, as the slots test it. The
// wrapper at +8 and the library aren't called. Adapters:
// src/hooks/movies.cpp. See docs/native-movies.md.

#pragma once

#include <cstdint>

namespace movies::movie {

constexpr uint32_t kCommandStatus = 24;  // 3 once the open's command is done
constexpr uint32_t kPath = 48;           // 260 bytes
constexpr uint32_t kPathBytes = 260;
constexpr uint32_t kState = 308;
constexpr uint32_t kCommandDone = 3;

enum State : uint32_t { kNone = 0, kLoaded = 1, kPlaying = 2, kPaused = 3 };

// Slot 1, Movie::Play (sub_820AEC10): `path` (the game's, "DATA\TXR\MOVIE\...")
// copied to +48 as strncpy copies it, the player opened, state 1.
void Play(uint8_t* base, uint32_t movie, uint32_t path);

// Slots 2-6 (sub_820AE4E0, sub_820AE828, sub_820AE898, sub_820AE8F8,
// sub_820AEA80): the state changes, done to the player. Each tests the
// open's status first, as the originals do.
void Start(uint8_t* base, uint32_t movie);
void Stop(uint8_t* base, uint32_t movie);
void Pause(uint8_t* base, uint32_t movie);
void Resume(uint8_t* base, uint32_t movie);
uint32_t GetState(uint8_t* base, uint32_t movie);

// Slot 7, Movie::Release (sub_820AE528): the player closed unless playing.
void Release(uint8_t* base, uint32_t movie);

// Slot 0, Movie::RenderFrame (sub_820AE958), on the render thread from
// command 28: the frame due drawn over render target 0 (native renderer);
// state 1 once the movie has ended.
void RenderFrame(uint8_t* base, uint32_t movie);

}  // namespace movies::movie
