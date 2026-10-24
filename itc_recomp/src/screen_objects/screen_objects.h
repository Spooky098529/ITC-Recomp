// The race's screen objects natively (docs/functions/screen-objects.md):
// the ring points' texture coordinates kept in their map (uv_trees.h), the
// rings built at set-up, the strength set every tick, and the object's
// teardown. Plain C++ over guest memory (the objects are shared with
// recompiled code); the device lock's video output and Resource::Release,
// which still take the context, go through the callbacks the adapters
// (src/hooks/screen_objects.cpp) give. The set-up's fan (rand), the object's
// constructor and Init above it, and its draw (ScreenObject::Record) stay
// recompiled.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace screen_objects {

constexpr uint32_t kUvMap = 76124;        // the UvMap
constexpr uint32_t kRingVertices = 76136;  // 20-byte vertices
constexpr uint32_t kRingCount = 127976;    // their count
constexpr uint32_t kRingPoints = 20 * 36 * 2;

// ScreenObject::AddRingPoint (sub_82132778): the point's keys (x and y
// rounded to 0.001: floor(v x 1000 + 0.5) x 0.001) and its (u, v) added to
// the object's map: a new column under x when x's isn't there, else (u, v)
// under y in x's column unless y is there. `stack` is the caller's r1: the
// original's temporaries (three columns, the value and the pair) go in its
// 272-byte frame below it. Returns r3 as the original leaves it (the first
// temporary column's freed head for a new column, else the frame's iterator
// out at +80 or +112); nullopt, with nothing done, where the original must
// run (no process heap, a map or column at its size limit).
std::optional<uint32_t> AddRingPoint(uint8_t* base, uint32_t object, double x, double y, double u, double v,
                  uint32_t stack);

// ScreenObject::BuildRings (sub_821329B8): 20 rings of 36 points at radii
// i x 0.035 and (i + 1) x 0.035 about the center, each point's vertex
// (width x u, height x v, white, u, v) appended at +76136 (the count at
// +127976) and added with AddRingPoint. `stack` is the caller's r1. Returns
// the last AddRingPoint's r3 (as the original leaves it); nullopt, with
// nothing done, unless the object's map is empty (as it is at set-up, the
// only call) and nodes can be allocated.
std::optional<uint32_t> BuildRings(uint8_t* base, uint32_t object, uint32_t stack);

// ScreenObject::SetSpeed (sub_8212FCC8): +27 = 1, +76 = the strength clamped
// to [0.0, 1.0] (the floats at 0x82000A34 and 0x82000A30). False, with
// nothing done, when the object is running (+24: the original calls
// sub_8212F778 first, never seen in play).
bool SetSpeed(uint8_t* base, uint32_t object, double strength);

// The guest calls ScreenObject::Release makes: the video output (the
// object at [0x8266D088]'s slot 7, then that one's slot 3; `which` 0 before
// the lock, 1 after the releases), and Resource::Release on a resource.
struct ReleaseCalls {
  std::function<uint32_t(int which)> video_output;
  std::function<void(uint32_t resource)> release;
};

// ScreenObject::Release (sub_8212F0B8): with the device locked
// (VideoOutput::LockDevice, its word at `frame` + 80, the original's local),
// Resource::Release on the vertex declaration +0 and the texture +4, both
// then 0; the lock given back. Returns the video output (r3 as left).
uint32_t Release(uint8_t* base, uint32_t object, uint32_t frame, const ReleaseCalls& calls);

// ScreenObject::~ScreenObject (sub_82132FC8): Release (its frame 128 bytes
// below `frame`, this one's), then UvMap::~UvMap on +76124. Returns r3 as the original leaves it (the
// map's head, freed).
uint32_t Destroy(uint8_t* base, uint32_t object, uint32_t frame, const ReleaseCalls& calls);

}  // namespace screen_objects
