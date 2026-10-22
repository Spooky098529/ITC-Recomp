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
