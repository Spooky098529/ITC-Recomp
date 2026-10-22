// The race overlays' update side as plain C++ (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/race-overlays.md). The overlays are
// guest objects shared with the recompiled code (their recordings, the
// update loops that call these), so these work on guest memory (level 1 of
// "The boundary rule"). Adapters: src/hooks/race_overlays.cpp.

#pragma once

#include <cstdint>

namespace race_overlays {

// --- The car overlays' views and the vehicles behind them ---
//
// A view (OverlayView, 16 bytes): +4 the vehicle, +8 its scene model, +12
// the viewport. Wheel records are 368 bytes apart in the vehicle.

// CarOverlays::View (sub_82126550): the view in `slot` (+4 + 4 x slot), 0
// for slot 25.
uint32_t View(uint8_t* base, uint32_t overlays, uint32_t slot);

// CarOverlays::ViewportEntry (sub_821264A8): the address of the 12-byte
// entry `index` of the table at 0x82008558.
uint32_t ViewportEntry(uint32_t index);

// OverlayView::SpeedKmh (sub_821268C0): the view's vehicle's speed in km/h
// (Vehicle::GetSpeedKmh, sub_820C0128, which the original tail-calls).
float SpeedKmh(uint8_t* base, uint32_t view);

// OverlayView::WheelSpin (sub_821268F0): the float at vehicle + 664 + 368 x
// wheel.
float WheelSpin(uint8_t* base, uint32_t view, uint32_t wheel);

// sub_82126908: the float at vehicle + 512 + 368 x wheel.
float WheelValue512(uint8_t* base, uint32_t view, uint32_t wheel);

// sub_82126920: the vehicle's float +3764 for wheels 0-1, +4036 for 2-3,
// else the game's 0.0 (0x82000A34).
float AxleValue3764(uint8_t* base, uint32_t view, uint32_t wheel);

// sub_82126958: the vehicle's float +3384 for wheels 0-1, +3388 for 2-3,
// else the game's 0.0, times the float at 0x82001FBC (rounded to single).
float AxleValue3384(uint8_t* base, uint32_t view, uint32_t wheel);

// OverlayView::WheelOnGround (sub_82126998): the byte at vehicle + 433 + 368
// x wheel.
uint32_t WheelOnGround(uint8_t* base, uint32_t view, uint32_t wheel);

// sub_82126A80: the vehicle's byte +2504.
uint32_t VehicleByte2504(uint8_t* base, uint32_t view);
// sub_82126AA0: the vehicle's word +2252.
uint32_t VehicleWord2252(uint8_t* base, uint32_t view);
// sub_82126AB0: the vehicle's word +340 (an object pointer).
uint32_t VehicleWord340(uint8_t* base, uint32_t view);
// sub_82126AC0: the vehicle's float +6256.
float VehicleFloat6256(uint8_t* base, uint32_t view);
// sub_82126AD0: the vehicle's float +6252.
float VehicleFloat6252(uint8_t* base, uint32_t view);

// sub_82126D18: the word at [0x826C5B2C] + 0xA3AF4 (probably the race mode).
uint32_t RaceMode(uint8_t* base);

// OverlayView::RacerFlags (sub_82127178): [[vehicle + 5712] + 3888].
uint32_t RacerFlags(uint8_t* base, uint32_t view);

// OverlayView::RacerId (sub_82126AE0): which of the 93 racer records (3,936
// bytes from [0x826C5B2C] + 48) is the vehicle's +5712, and that record's
// word +3912; -1 when none is.
int32_t RacerId(uint8_t* base, uint32_t view);

}  // namespace race_overlays

namespace race_overlays {

// --- The child overlays' setters ---
//
// Each copies a 16-byte vector word by word (the original's lfs then stfs:
// the translated code keeps the bits, signaling NaNs included).

// Smoke::SetVelocity (sub_8212A840): `vector` to the smoke's +32,544.
void SmokeSetVelocity(uint8_t* base, uint32_t smoke, uint32_t vector);
// Smoke::SetRegion (sub_8212A998): `a` to +32,608, `b` to +32,592, the view
// mode to +32,624.
void SmokeSetRegion(uint8_t* base, uint32_t smoke, uint32_t a, uint32_t b, uint32_t mode);
// ParticlesB::SetPosition (sub_8212BA70): `position` to +2,784.
void ParticlesBSetPosition(uint8_t* base, uint32_t effect, uint32_t position);
// ParticlesC::SetPosition (sub_8212C6A8): `position` to +2,672 and `speed`
// (rounded to single) to +2,704.
void ParticlesCSetPosition(uint8_t* base, uint32_t effect, uint32_t position, double speed);
// Sparks::SetPosition (sub_8212DF90): `position` to +28,960.
void SparksSetPosition(uint8_t* base, uint32_t sparks, uint32_t position);
// Trail::SetLastPosition (sub_821299D8): `position` to +64,144.
void TrailSetLastPosition(uint8_t* base, uint32_t trail, uint32_t position);
// Marker::SetCameraPosition (sub_8212CE18): `position` to +272.
void MarkerSetCameraPosition(uint8_t* base, uint32_t marker, uint32_t position);
// LightSprite::SetPosition (sub_8211EBE0): when shown (+240), `position` to
// the sprite's position (+192).
void LightSpriteSetPosition(uint8_t* base, uint32_t sprite, uint32_t position);
// ModelSprite::PlaceHidden (sub_82121560): `position` to +272, +336 = 0.
void ModelSpritePlaceHidden(uint8_t* base, uint32_t sprite, uint32_t position);

// Overlay::SetTextures104 (sub_8212C0E0) and SetTextures108 (sub_8212D6D8):
// `value` to the overlay's +104 or +108.
void SetTextures104(uint8_t* base, uint32_t overlay, uint32_t value);
void SetTextures108(uint8_t* base, uint32_t overlay, uint32_t value);

// Marker::SetTextures (sub_8212CC10, 4 at +104), LightSprite::SetTextures
// (sub_8211E9C8, 9 at +104), ModelSprite::SetTextures (sub_8211F408, 25 at
// +108): the addresses of consecutive words from `table` (the texture
// table's entries, not their contents).
void MarkerSetTextures(uint8_t* base, uint32_t marker, uint32_t table);
void LightSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table);
void ModelSpriteSetTextures(uint8_t* base, uint32_t sprite, uint32_t table);

}  // namespace race_overlays

namespace race_overlays {

// --- Random numbers ---

// rand (sub_82293568), the game's own (not the C runtime's per-thread one):
// the seed at 0x82638188 = seed x 1,103,515,245 + 12,345; returns the new
// seed's upper halfword's low 15 bits.
uint32_t Rand(uint8_t* base);

// RandomRange (sub_8211F118): low + (high - low) x rand() x the float at
// 0x82002BD4, with the original's roundings (the difference and the product
// to single, then a fused multiply-add rounded to single). `drawn` gets
// rand()'s result (the original leaves it in r3).
float RandomRange(uint8_t* base, double low, double high, uint32_t* drawn = nullptr);

}  // namespace race_overlays

namespace race_overlays {

// --- The marker above opponents ---
//
// Its sprite is at +128: +176 the position, +208 to +216 the colors, +220
// the alpha as an integer, +224 shown, +228 the size, +232 the alpha, +236 a
// counter (the table's +0 when shown, counted up by the update), +240 the
// texture index. The marker's +120 is its parameter table, +256 the view
// mode, +272 the camera position, +288 1 for the camera-facing quad.

// Marker::Set (sub_8212CCE0): places the marker at `position`; `distance`
// 0.0 makes it the camera-facing quad (size from the table's +4), else the
// screen rectangle sized by the distance (clamped); `kind` 0-3 picks the
// texture (others 0); when not shown yet, shows it with the table's alpha
// (+8) and life (+0), colors 255. Returns 1.
uint32_t MarkerSet(uint8_t* base, uint32_t marker, uint32_t position, double distance,
                   uint32_t mode, uint32_t kind);

// Marker::Update (sub_8212CBE0, its slot 4): when shown, the age counts up
// (back to 0 when it would turn negative).
void MarkerUpdate(uint8_t* base, uint32_t marker);

}  // namespace race_overlays

namespace race_overlays {

// --- The trails ---
//
// A trail (64,160 bytes): its points (128 bytes each) from +128, +104 the
// capacity, +112 the parameter table, +64,128 the oldest point's index,
// +64,136 the count.

// Trail::Update (sub_821292E8, its slot 4; once a tick): drops the oldest
// point when its age (+60) is 0 or below (clearing its +112 and +48, and
// setting the next one's +112 to 1), then for each point from the oldest:
// the age counts down to 0; +48 = 1 once the table's +0 minus the age is
// above 1; the width (+56) fades from +124 by age over the table's +12
// ticks (age / the table's +12 x +124), clamped to 0 and +124; +44 the
// width as an integer. False, with nothing done, for a capacity of 0 or -1
// (where the original's division would trap); the original runs there.
bool TrailUpdate(uint8_t* base, uint32_t trail);

}  // namespace race_overlays

namespace race_overlays {

// --- The car-model sprites' kinds and textures ---

// CarOverlays::ModelSpriteKind (sub_82123188): the car-model sprites' kind
// for a racer id: 6 for 382, 392-394 and 397; 4 for 391; 1 for 357, 395,
// 396 and 399; 5 for 398; else 0. (The original first reads the view in
// the overlays' slot, sub_82126550, and ignores it: a load, left out.)
uint32_t ModelSpriteKind(int32_t racer);

// ModelSprite::Entry (sub_8211F300): word `index` (0-8; 0 above 8) of the
// kind's table: kind 1 0x82616CC4, 2 0x82616CE8, 3-5 0x82616D0C, 6
// 0x82616D30, 7 0x82616D54, 8 0x82616D78, others 0x826C5CF0. Covers its
// jump table's first case, sub_8211F3B4 (word 0), and kind 1's
// sub_8211F334 (never run in play).
uint32_t ModelSpriteEntry(uint8_t* base, uint32_t kind, uint32_t index);

}  // namespace race_overlays

namespace race_overlays {

// --- The emitters ---

// ParticlesC::Emit (sub_8212C1C0): when the count (+2,696) is below the
// capacity (+2,700, unsigned), a new 128-byte entry at the head (+2,692; the
// entries from +112): +48 the position, +64 the effect's position (+2,672),
// +32 their difference with w the game's 1.0, +108 life, +100 size and +104
// alpha from the table (+108's +0, +4, +8), +92 the alpha as an integer,
// +96 = 1, +112 = 0, colors 255, 0, 0 at +80; the head moves on (modulo the
// capacity) and the count goes up. Returns 1, or 0 when full. (The
// original's f1 isn't read.)
uint32_t ParticlesCEmit(uint8_t* base, uint32_t effect, uint32_t position);

// Trail::AddPoint (sub_82129598): when the count (+64,136) is below the
// capacity (+104, signed), `state` isn't 0 and `position` is away from the
// last position (+64,144: the length of their difference, worked out as the
// original's vector code does, isn't 0.0), a 128-byte point at the tail
// (+64,132; the points from +128): +0 the position, +16 the last position,
// +64 their difference; +52 the width; +112 the state (2 becomes 1 for a
// trail's first point); +124 and +56 the table's +8 plus speed times the
// float at 0x82001FBC, capped by the float at 0x82000C44; +44 that as an
// integer; +48 `value`; +60 the table's life (+0); +120 = 0; colors 255;
// +96 and +80 the position plus and minus `direction` crossed with the
// difference, scaled to the width over its length and by the float at
// 0x82001BE4. The tail moves on (modulo the capacity) and the count goes
// up. `direction` is the vector passed in r5 and r6 (words 0-1 and 2-3; the
// original also stores them in its caller's parameter area, which no caller
// reads). `*result` gets 1 for a point added, else 0. False, with nothing
// done, for a capacity of 0 or -1 (where the original's division can trap);
// the original runs there.
bool TrailAddPoint(uint8_t* base, uint32_t trail, uint32_t position, double speed, double width,
                   const uint32_t direction[4], uint32_t state, uint32_t value,
                   uint32_t* result);

}  // namespace race_overlays

namespace race_overlays {

// ParticlesB::Emit (sub_8212B4A8): when the count (+2,776) is below the
// capacity (+104, signed), a new 176-byte entry at the head (+2,772; the
// entries from +128): +48 the position, +64 the effect's position (+2,784),
// their difference with w the game's 1.0 to the effect's +2,800, +108 life
// and +100 size from the table (+112's +0, +4), +104 and +112 the table's
// +8 times `a` times the float at 0x82008540, +92 that as an integer, +80
// to +88 = 200, +96 and +116 = 1; then each of its +116 particles (48
// bytes from +128): placed around the position at a random angle (cos and
// sin of RandomRange(0x820029C4, 0x82002240)) and distance
// (RandomRange(0x82006908, 0x82002208)) times the float at 0x82001BE4,
// less the effect's +2,800 (a vector subtraction, flush-to-zero on), +16
// the effect's +2,800, +32 rand() % 3, +36 rand() % 4. The head moves on
// (modulo the capacity) and the count goes up. `*result` gets 1, or 0 when
// full. `*sine` gets the last particle's sine, unrounded (the original
// leaves it in f1; untouched when no particle is made). (The original's f2
// and f3 aren't read.) False, with nothing done,
// for a capacity of 0 or -1 (where the original's division can trap); the
// original runs there.
bool ParticlesBEmit(uint8_t* base, uint32_t effect, uint32_t position, double a,
                    uint32_t* result, double* sine);

}  // namespace race_overlays

namespace race_overlays {

// --- The scene model's points ---
//
// A scene model (a view's +8): +5048 an object whose +24 holds the first
// four points (16 bytes each from +6976), +5052 and +5064 words that must
// be set, +5060 the model's parts (points 4-21: 4-7 from +34,256, 8-13 from
// +23,120, 14-21 from +24,640, 240 bytes apart, each with a word that must
// be set 140 bytes before it).

// ModelParts::HasPoint (sub_820DF2A0): for points 4-21, 1 when the point's
// word is set, else 0; others return `parts` itself (the original leaves
// r3 alone).
uint32_t ModelPartsHasPoint(uint8_t* base, uint32_t parts, int32_t index);

// Model::HasPoint (sub_820D9DB0): 0 unless the model's +5048, +5052, +5060
// and +5064 are all set; then 1 for points below 4, else
// ModelPartsHasPoint on +5060.
uint32_t ModelHasPoint(uint8_t* base, uint32_t model, int32_t index);

// OverlayView::HasPoint (sub_82126A78): ModelHasPoint on the view's model.
uint32_t OverlayViewHasPoint(uint8_t* base, uint32_t view, int32_t index);

}  // namespace race_overlays

namespace race_overlays {

// ModelParts::PointPosition (sub_820E0910): for points 4-21, the point's 16
// bytes to `out` (two doublewords, as the original's ld and std); nothing
// for others. Returns `out` (the original leaves r3 alone).
uint32_t ModelPartsPointPosition(uint8_t* base, uint32_t out, uint32_t parts, int32_t index);

// Model::PointPosition (sub_820DA988): when the model's four words are set
// (as ModelHasPoint), points below 4 from the table at the +5048 object's
// +24 plus 6,976 (16 bytes each), others ModelPartsPointPosition on +5060;
// else the game's 0.0 (0x82000A34) to the four words of `out`. Returns
// `out`.
uint32_t ModelPointPosition(uint8_t* base, uint32_t out, uint32_t model, int32_t index);

// OverlayView::PointPosition (sub_82127140): ModelPointPosition on the
// view's model. Returns `out`.
uint32_t OverlayViewPointPosition(uint8_t* base, uint32_t out, uint32_t view, int32_t index);

}  // namespace race_overlays

namespace race_overlays {

// --- The sparks ---
//
// A sparks emitter: its sparks (144 bytes each) from +128, +104 the
// capacity, +28,928 a streak counter, +28,932 the head, +28,940 the count,
// +28,944 the spread, +28,960 the position, +28,976 streaking, +28,992 and
// +29,008 the last two positions, +29,024 50 random vectors.

// Sparks::UpdateStreak (sub_8212E660): the spread (+28,944) from `speed`
// times the float at 0x820029E4, clamped to the floats at 0x82001FBC and
// 0x82002C50; `position` (the vector passed in r4 and r5: words 0-1 and
// 2-3) to +28,960; `streaking` to +28,976; the counter counts up. When
// streaking, the last positions move on (the newer to +28,992, `position`
// to +29,008), the counter goes back to 0, and the 50 vectors get x, y and
// z from RandomRange(-spread, spread) times the spread factor (1.0, or the
// float at 0x820035B0 for `mode` 1); else they're (0, 0, 0, 1). Once the
// counter is above 2, +29,008 is copied to +28,992 (before the reset).
// Then each of the count's sparks from the head: state 2 adds its +32
// vector to +64 (vaddfp, flush-to-zero on); state 1 while streaking: 2 when
// the length of +48 - +64 (as the original's vector code works it out)
// passes +136, else +64 = `position`; 2 again when rand() % 10 is 0; +112
// = +64 - +48; state 1 when not streaking: as state 2. The original also
// stores `position` in its caller's parameter area, which its caller
// doesn't read. `*drawn` gets the last rand() result and `*value` the last
// RandomRange value (the original leaves them in r3 and f1; untouched when
// none is drawn).
void SparksUpdateStreak(uint8_t* base, uint32_t sparks, double speed,
                        const uint32_t position[4], uint32_t streaking, uint32_t mode,
                        uint32_t* drawn, double* value);

}  // namespace race_overlays

namespace race_overlays {

// Sparks::Emit (sub_8212D818): when the count (+28,940) is below the
// capacity (+104, signed), a new spark (144 bytes) at the head (+28,936):
// +100 size drawn between the table's +4 (the table at +112) times the
// float at 0x82001BE4 and +4; +104 the table's +8; +108 life (the table's
// +0); +96 = 1, +132 = 0; +136 a limit drawn between the floats at
// 0x820021E8 and 0x820021E4; +128 a state (rand() % 5, 1 above 1; 2 for a
// positive `strength`); colors by rand() % 5 (200 or 255 with +92 the size
// as an integer); then its position (+48) and +64 from `position`, moved
// toward the sparks' position less `vector` (the vector passed in r6 and
// r7) and by a random angle and radius when `strength` is positive, or
// spread by random amounts in state 0; +112 = +48 - +64; and its velocity
// (+32) drawn by the state, toward the side of `vector` against
// `position`. The head moves on (modulo the capacity) and the count goes
// up. `*result` gets 1, or 0 when full; `*last` gets the last value the
// original leaves in f1 (a RandomRange value or a sine; untouched when
// full). False, with nothing done, for a capacity of 0 or -1 (where the
// original's division can trap); the original runs there.
bool SparksEmit(uint8_t* base, uint32_t sparks, uint32_t position, double strength,
                const uint32_t vector[4], uint32_t* result, double* last);

}  // namespace race_overlays

namespace race_overlays {

// --- The tire smoke ---
//
// A smoke emitter: its puffs (720 bytes each) from +128, +104 the capacity,
// +112 the parameter table, +32,532 the head, +32,536 the count, +32,544
// the velocity, +32,560 and +32,576 a region's two points (Smoke::SetRegion).

// Smoke::Emit (sub_8212A350): when the count is below the capacity
// (signed), a new puff at the head: +112 `size`, +96 `value`, +104 and +120
// the table's +8, +108 life (the table's +0), +100 and +124 the frame's +84
// times the float at 0x820021E8 plus a draw up to the float at 0x82002C4C,
// +116 `speed` times the float at 0x820029E4 plus a draw from 1.0 to the
// float at 0x820029D4; +48 the frame's +64, +64 the smoke's velocity
// (+32,544); colors 255, +92 the alpha as an integer; a drift (the region's
// span when `region` is set, else +48 - +64, times a draw, plus random
// offsets; vaddfp with flush-to-zero on); +128 the particle count (`speed`
// times the float at 0x820029EC as an integer, clamped to 2-10); then each
// particle (48 bytes from +240): placed around +48 at a random angle and
// radius (or along +64 - +48 by its index while the puff's life is within 2
// of the previous puff's; from the sixth on, at +48 itself, the count cut to
// 5), the drift, rand() % 3, rand() % 4 and 1. The head moves on (modulo the
// capacity) and the count goes up. `*result` gets 1, or 0 when full;
// `*last` the last sine (the original leaves it in f1; untouched when
// full). False, with nothing done, for a capacity of 0 or -1 (where the
// original's division can trap); the original runs there.
bool SmokeEmit(uint8_t* base, uint32_t smoke, double size, double speed, uint32_t frame,
               uint32_t region, uint32_t value, uint32_t* result, double* last);

// CarOverlays::EmitSmoke (sub_82123240): adds `rate` (unless it's 0.0) to
// the wheel's accumulator (the overlays' +528 + 48 x wheel); once that
// reaches 1.0, emits its whole part on the wheel's smoke (+164 + 4 x wheel;
// SmokeEmit with `region` 1 for wheels 4 and above) and keeps the rest.
// Returns 0 (the original's r3). `*last` gets what the original leaves in
// f1 (untouched when nothing is emitted). False, with nothing done, when the
// smoke's capacity is 0 or -1 or the accumulator is the smoke's capacity
// word (where Smoke::Emit would decline after the accumulator changed);
// the original runs there.
bool EmitSmoke(uint8_t* base, uint32_t overlays, double rate, double speed, uint32_t frame,
               uint32_t wheel, uint32_t value, double* last);

}  // namespace race_overlays

namespace race_overlays {

// --- Set-up: constructors, clears and bindings ---
//
// Called while a race's overlays are made (their allocations stay
// recompiled); none of these allocates.

// Overlay::Overlay (sub_82126498): the base class's vtable (0x820085D8) to
// +0.
void OverlayConstruct(uint8_t* base, uint32_t overlay);

// CarOverlays::ClearViews (sub_82126530): the 25 view slots (+4 to +100)
// to 0.
void ClearViews(uint8_t* base, uint32_t overlays);

// ViewDesc::Clear (sub_82126860): +0 and +4 to 0.
void ViewDescClear(uint8_t* base, uint32_t desc);

// OverlayView::OverlayView (sub_82126870): the vtable (0x82010B5C) to +0,
// +4 and +8 to 0.
void OverlayViewConstruct(uint8_t* base, uint32_t view);

// OverlayView::Set (sub_82126890): the description's three words (the
// vehicle, its scene model, the viewport) to +4, +8 and +12.
void OverlayViewSet(uint8_t* base, uint32_t view, uint32_t desc);

// CarOverlays::BindViewport (sub_821264C0): `out` +0 = [[0x826C2CE8] + 4 x
// entry.0] + 80 x entry.1, +4 = the camera pass 0x826BAC00 + 592 x
// entry.2, +8 and +12 the two words at 0x820085A0 + 8 x `second` (`entry`
// the 12-byte entry `index` at 0x82008558).
void BindViewport(uint8_t* base, uint32_t out, uint32_t index, uint32_t second);

// Overlays::SetRenderTargets (sub_82128190): the render queue and the
// command list to 0x826C5D5C and 0x826C5D60.
void SetRenderTargets(uint8_t* base, uint32_t queue, uint32_t list);

// Particle::Particle (sub_8212B3D0): the game's 1.0 (0x82000A30, its bits)
// to +12 and +28.
void ParticleConstruct(uint8_t* base, uint32_t particle);

// RaceOverlays::Clear (sub_8215DE80): the list's 25 pointers to 0.
void RaceOverlaysClear(uint8_t* base, uint32_t list);

// RaceOverlays::ShareTextures (sub_8215E000): entries 1-24 that are set get
// overlay 0's texture table (SetTextures104 with [list[0] + 104]). Returns
// what the original leaves in r3 (entry 24).
uint32_t ShareTextures(uint8_t* base, uint32_t list);
// RaceOverlays::SlotOf (sub_8215E048): an overlay's index (0-24) to its
// view slot: the index itself (the jump table's cases, sub_8215E078 the
// first), 0 for anything above 24.
uint32_t SlotOf(uint32_t index);

}  // namespace race_overlays

namespace race_overlays {

// --- Resets ---
//
// The children's slot 3 (reached through their vtables) and the entries
// and sprites they clear. "0.0" and "1.0" are the game's words at
// 0x82000A34 and 0x82000A30, copied with their bits (the original's
// lfs/stfs); a "unit vector" is (0.0, 0.0, 0.0, 1.0). The identity rows the
// original stores with stvx go to the 16-byte-aligned address below, as
// stvx does.

// LightSprite::Reset (sub_8211E9F0): +272 a unit vector, the sprite at +144
// cleared.
void LightSpriteReset(uint8_t* base, uint32_t sprite);
// ModelSprite::Reset (sub_8211F430): the sprite at +224 cleared, +368 and
// +384 unit vectors, +404 and +408 -1, ten unit vectors from +416.
void ModelSpriteReset(uint8_t* base, uint32_t sprite);
// Sprite::ResetStyle2 (sub_82123A00): the identity matrix at +0 (stvx),
// +64 and +68 0, +72 2, 1.0 at +92 and +108.
void SpriteResetStyle2(uint8_t* base, uint32_t sprite);
// Trail::Reset (sub_82129498): head, tail and count (+64,128 to +64,136) 0,
// the last position (+64,144) a unit vector, then each of the +104 points
// (128 bytes from +128) cleared, the count read again each time.
void TrailReset(uint8_t* base, uint32_t trail);
// SmokeEmitter::Reset (sub_8212A218): the emitter's vectors cleared, the
// identity matrix at +144 (stvx), +92 1, +128 10, +112 and +224 the float
// at 0x82002A90, its ten 48-byte particles from +240 cleared.
void SmokeEmitterReset(uint8_t* base, uint32_t emitter);
// SmokeEmitter::SmokeEmitter (sub_8212B230): 1.0 in the w of its vectors
// and of its ten particles from +240.
void SmokeEmitterConstruct(uint8_t* base, uint32_t emitter);
// ParticlesB::ResetEntry (sub_8212B3E8): a 176-byte entry's vectors unit
// vectors, +92 and +116 1, the rest 0.
void ParticlesBResetEntry(uint8_t* base, uint32_t entry);
// ParticlesC::Reset (sub_8212C0E8): +2,704 0.0, +2,688 to +2,696 0, the
// position (+2,672) a unit vector, then each of the +2,700 entries (128
// bytes from +112) cleared, the count read again each time.
void ParticlesCReset(uint8_t* base, uint32_t particles);
// Marker::Reset (sub_8212CC38): the camera position (+272) a unit vector,
// +288 1, the sprite at +128 cleared.
void MarkerReset(uint8_t* base, uint32_t marker);
// Sparks::Reset (sub_8212D6E0): the counters (+28,928 to +28,940, +28,976)
// 0, its vectors unit vectors, the 50 vectors at +29,024, then each of the
// +104 entries (144 bytes from +128) cleared, the count read again each
// time.
void SparksReset(uint8_t* base, uint32_t sparks);

}  // namespace race_overlays

namespace race_overlays {

// --- The children's updates ---

// Smoke::InRegion (sub_8212A868): 1 when (point - a) . (a - b) < 0.0 and
// (point - b) . (b - a) < 0.0 (the region's points a at +32,592 and b at
// +32,608; differences by fsubs, dot products of x, y and z by vmsum3fp128
// with flush-to-zero on), else 0. Writes nothing to guest memory (the
// original's temporaries are below the stack pointer).
uint32_t SmokeInRegion(uint8_t* base, uint32_t smoke, uint32_t point);

// Sparks::Update (sub_8212DDC8): for each of the count (+28,940) entries
// from the head (+28,932; 144 bytes from +128, the index moving on modulo
// the capacity +104): its life (+108) counts down to 0; at 0, +96 = 0 and
// the head moves on and the count goes down (in memory; the loop runs the
// count it started with); +104 = the table's +8, times 1/the table's +12
// times life while life is below +12 (unsigned), clamped to 0.0 and the
// table's +8; +92 = that as an integer; +112 = +48 - +64 (x, y, z); +48 +=
// +32. False, with nothing done, when the original could trap or reach
// outside its entries (a capacity of 0 or -1, a head below 0 or INT_MAX,
// entries past the end of guest memory); the original runs there.
bool SparksUpdate(uint8_t* base, uint32_t sparks);

// Smoke::Update (sub_8212AA78): the oldest emitter (the head, +32,528; 720
// bytes from +128), when its life (+108) is out, reset (SmokeEmitterReset)
// and dropped (the head moves on modulo the capacity +104 and the count
// +32,536 goes down, both read again after the reset). Then for each of the
// count emitters from the head: its life counts down to 0; +96 = 1 once the
// table's (+112) +0 less life is above 1 (unsigned); +100 = +124 + +116 x
// +124 x (table +0 - life) / table +0; +104 = +120 times 1/the table's +12
// times life while life is below +12 (unsigned), clamped to 0.0 and +120;
// +92 = that as an integer; its +128 particles (48 bytes from +240) move by
// their velocity (+16) and get +40 = 1, or 0 when the region is on (+32,624
// = 1) and Smoke::InRegion says they're inside it. `r3` is left as the
// original leaves it (the last InRegion result, when it ran). False, with
// nothing done, when the original could trap or reach outside its emitters
// (EntriesInRange's cases); the original runs there.
bool SmokeUpdate(uint8_t* base, uint32_t smoke, uint32_t* r3);

// ParticlesB::Update (sub_8212B748): the oldest entry (+2,768; 176 bytes
// from +128), when its life (+108) is out, reset (ParticlesBResetEntry) and
// dropped (the head moves on modulo the capacity +104 and the count +2,776
// goes down, both read again after the reset). Then for each of the count
// entries from the head: life counts down to 0; +100 = the table's (+112)
// +4 + (table +0 - life) / (2 x table +0); +104 = +112 times life / the
// table's +12 while life is below it (unsigned), clamped to 0.0 and +112;
// +92 = that as an integer; each of its +116 particles (48 bytes from +128)
// gets the velocity (+16) the effect's +2,800 times (the table's +4 x its
// length + the float at 0x82002A3C, at most the one at 0x82002A34; w 1.0)
// and moves by it. False, with nothing done, when the original could trap
// or reach outside its entries (EntriesInRange's cases); the original runs
// there.
bool ParticlesBUpdate(uint8_t* base, uint32_t effect);

// ParticlesC::Update (sub_8212C2F0): for each of the count (+2,696)
// entries from the oldest (+2,688; 128 bytes from +112, the index moving on
// modulo the capacity +2,700, unsigned): life (+108) counts down to 0; at 0,
// +96 = 0 and the oldest moves on and the count goes down (in memory; the
// loop runs the count it started with); +104 = the table's (+108) +8 times
// life / the table's +12 while life is below it (unsigned), clamped to 0.0
// and the table's +8, times the effect's +2,704 x the float at 0x82002BF0
// (at most 1.0), plus rand() % 3; +92 = that as an integer. Then the newest
// entry (the one before +2,692) takes the effect's position (+2,672, at its
// +48), and for count - 1 entries from there back, the entry before each is
// pulled to within +2,704 x the float at 0x8200874C x the table's +4 of it
// (its +48 = the next one's +48 less their difference times that over its
// length, w 1.0; left alone at a length of 0.0). `r3` is left as the
// original leaves it (the last rand() result, when it ran). False, with
// nothing done, for a capacity of 0 (the original's divisions trap); the
// original runs there.
bool ParticlesCUpdate(uint8_t* base, uint32_t effect, uint32_t* r3);

}  // namespace race_overlays

namespace race_overlays {

// --- The children's constructors, and the resets made of their entries' ---
//
// The constructors run Overlay::Overlay, store their vtable and put the
// game's 1.0 (its word) in the w of their vectors; they return the object.

// LightSprite::LightSprite (sub_8211F060): vtable 0x82008444; 1.0 at +156
// to +220 (every 16 bytes) and +284.
void LightSpriteConstruct(uint8_t* base, uint32_t sprite);
// ModelSprite::ModelSprite (sub_82122880): vtable 0x820084E8; 1.0 at +236
// to +300, +380, +396 and +428 to +572.
void ModelSpriteConstruct(uint8_t* base, uint32_t sprite);
// Trail::Trail (sub_8212A140): vtable 0x820086C0; 1.0 at +12, +28, +76, +92
// and +108 of each of the 500 points (128 bytes from +128) and at +64,156.
void TrailConstruct(uint8_t* base, uint32_t trail);
// Smoke::Smoke (sub_8212B2E8): +116 0, vtable 0x820086F4, the 45 emitters
// from +128 constructed (SmokeEmitterConstruct), 1.0 at +32,556 to +32,620.
void SmokeConstruct(uint8_t* base, uint32_t smoke);
// ParticlesC::ParticlesC (sub_8212CAF8): vtable 0x82008750; 1.0 at +12 to
// +76 (every 16 bytes) of each of the 20 entries (128 bytes from +112) and
// at +2,684.
void ParticlesCConstruct(uint8_t* base, uint32_t particles);
// Marker::Marker (sub_8212D620): vtable 0x82008784; 1.0 at +140 to +204 and
// +284.
void MarkerConstruct(uint8_t* base, uint32_t marker);
// Sparks::Sparks (sub_8212E9A0): vtable 0x820087BC; 1.0 at +12 to +76 and
// +124 of each of the 200 entries (144 bytes from +128), at +28,972,
// +29,004, +29,020 and +29,036 to +29,820.
void SparksConstruct(uint8_t* base, uint32_t sparks);

// Smoke::Reset (sub_8212A9E0): +32,528 to +32,536 0, +32,544, +32,560 and
// +32,576 unit vectors, then each of the +104 emitters (720 bytes from
// +128) reset (SmokeEmitterReset), the count read again each time.
void SmokeReset(uint8_t* base, uint32_t smoke);
// ParticlesB::Reset (sub_8212BA98): +2,768 to +2,776 0, +2,784 and +2,800
// unit vectors, then each of the +104 entries (176 bytes from +128) reset
// (ParticlesBResetEntry), the count read again each time.
void ParticlesBReset(uint8_t* base, uint32_t particles);

}  // namespace race_overlays
