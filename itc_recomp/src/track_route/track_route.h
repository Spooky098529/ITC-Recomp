// The course's route as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/track-route.md, the layout in
// docs/known-data.md): the road as a graph of sections, each a run of
// 84-byte slice records, and the route positions (two halfwords: a section
// and a slice) the race code and the track positions move along it. The
// route (0x826C5B10: +0 the file) and the positions are guest data shared
// with the recompiled code, so these work on guest memory, with the
// originals' loads and stores in their order and their float arithmetic
// step for step (src/car_physics/float_ops.h), so the results are the
// translated code's bit for bit. The originals' stack temporaries (a
// position or a float passed through their frame, the integers converted
// through the red zone) aren't written: nothing reads them after the
// return. Flush-to-zero off on entry (the adapters' job). Built with
// -ffp-model=strict and -ffp-contract=off (CMakeLists.txt).
// Adapters: src/hooks/track_route.cpp.
//
// A route's file ([route + 0]): +0 the sections' count, +8 a float (a
// slice's length), +12 the sections' table (a pointer each). A section: +0
// halfword its slices' count, +2 byte its kind (low four bits), +3 byte its
// branch number, +4 to +24 its bounds, +28 twelve links (halfwords: a
// section, -1 none, and a slice there; 0 to 2 forward, 3 to 5 back), +76 a
// byte table (8 a link: the lane on the other side), +124 the slices'
// records, then two tables of 12-byte points (+124 + 84 x count, +124 + 96
// x count). Sections are looked up by the low 16 bits of their number.

#pragma once

#include <cstdint>

namespace track_route {

// The course's route.
constexpr uint32_t kRoute = 0x826C5B10;

// --- Lookups (src/track_route/track_route.cpp) ---

// TrackRoute::LanePoint (sub_820F8668): out (x, y, z, 1.0) = the slice's
// center moved across the road by (lane - (n - 1) / 2) / n of its width (n
// the slice's lanes; a lane at n or above taken as n - 1).
void LanePoint(uint8_t* base, uint32_t route, uint32_t out, uint32_t position, int32_t lane);
// TrackRoute::LaneAt (sub_820F8920): the lane the point is in (0 to n - 1;
// 0 for slices with one lane).
uint32_t LaneAt(uint8_t* base, uint32_t route, uint32_t point, uint32_t position);
// TrackRoute::Link (sub_820F89F0): the link's section; when it's -1, link 0
// (for links 0 to 2) or 3 (the others); when that's -1 too, the section.
// Sign-extended from 16 bits (the original's r3).
int32_t Link(uint8_t* base, uint32_t route, uint32_t section, uint32_t link);
// TrackRoute::LinkLane (sub_820F8CE8): the signed byte at the position's
// section's +76 + 8 x link + lane; -1 when the link is -1.
int32_t LinkLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t link_address,
                 uint32_t lane_address);
// TrackRoute::SpecialSection (sub_820F8D38): 0 to 5 for the sections 293,
// 230, 156, 20, 39 and 77 of kind 4 whose slice's byte +57 is 4; else -1.
int32_t SpecialSection(uint8_t* base, uint32_t route, uint32_t position);
// TrackRoute::SliceTest (sub_820F8DE8): by the section's kind (its row).
uint32_t SliceTest(uint8_t* base, uint32_t route, uint32_t position);
// TrackRoute::IsMainRoad (sub_820F8E98): 1 for a section not of kinds 1 to
// 4 whose branch number isn't 7, 8 or 29.
uint32_t IsMainRoad(uint8_t* base, uint32_t route, uint32_t position);
// TrackRoute::SectionOutcode (sub_820F8EF0): the point against the
// section's bounds widened by [0x82002568]: bits 1, 2 x below or above, 4,
// 8 y, 16, 32 z.
uint32_t SectionOutcode(uint8_t* base, uint32_t route, uint32_t point, uint32_t section);

// --- Steps between sections (src/track_route/track_route.cpp) ---

// TrackRoute::StepForward (sub_820F8B48): the next section by the forward
// links 2, 1, 0 (2 and 1 not to kinds 2 or 4), the slice less the word at
// `count`. 1 when one was taken.
uint32_t StepForward(uint8_t* base, uint32_t route, uint32_t position, uint32_t count);
// TrackRoute::BranchesMatching (sub_820F8BD0): the forward links of the
// section (a pointer) not to kinds 2 or 4 whose branch number's bit is in
// the word at `mask`, written to `out` (section, link index); their count.
uint32_t BranchesMatching(uint8_t* base, uint32_t route, uint32_t out, uint32_t section,
                          uint32_t mask);
// TrackRoute::StepAlongPath (sub_820F8C58): the first section of the path
// (halfwords, ending with a negative one) a forward link leads to, the slice
// less the word at `count`. 1 when found.
uint32_t StepAlongPath(uint8_t* base, uint32_t route, uint32_t position, uint32_t count,
                       uint32_t path);
// TrackRoute::StepForwardMasked (sub_820F93A0): the first mask (a list of
// words ending with 0) with a matching branch (BranchesMatching) gives the
// next section; none, StepForward. 1 when one was taken.
uint32_t StepForwardMasked(uint8_t* base, uint32_t route, uint32_t position, uint32_t count,
                           uint32_t masks);
// TrackRoute::NormalizeForward (sub_820F9EA8): while the slice is past the
// section's last, the next section (StepForwardMasked with masks, else
// StepForward); when none, the last slice (or the position (0, 0) with a
// section above 0x7FFF) and 0. 1 once it's in the section.
uint32_t NormalizeForward(uint8_t* base, uint32_t route, uint32_t position, uint32_t masks);
// TrackRoute::StepForwardLane (sub_820F9448): the forward link whose lanes
// hold the halfword lane at `lane` (changed: the lane over the link), else
// links 2, 1, 0 onto the last lane; the slice less the word at `count`. 1
// when a section was taken.
uint32_t StepForwardLane(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane,
                         uint32_t count);
// TrackRoute::StepForwardLaneMasked (sub_820F9600): StepForwardLane by the
// branch masks' first matches. 1 when a section was taken.
uint32_t StepForwardLaneMasked(uint8_t* base, uint32_t route, uint32_t position, uint32_t lane,
                               uint32_t count, uint32_t masks);
// TrackRoute::StepBack (sub_820F8A58): the previous section by the back
// links 5, 4, 3 (its row), the slice plus its slices' count. 1 when one was
// taken.
uint32_t StepBack(uint8_t* base, uint32_t route, uint32_t position);
// TrackRoute::NormalizeBack (sub_820F9320): StepBack while the slice is
// negative; when none, slice 0 (or the position (0, 0) with a negative
// section) and 0. 1 once the slice isn't negative.
uint32_t NormalizeBack(uint8_t* base, uint32_t route, uint32_t position);

// --- The linear index (the global route's file) ---

// TrackRoute::ToIndex (sub_820AB408): the slices before the packed position
// (section in the high halfword) plus its slice.
uint32_t ToIndex(uint8_t* base, uint32_t packed);
// TrackRoute::FromIndex (sub_820AB498): the packed position holding the
// index, kept on the route.
uint32_t FromIndex(uint8_t* base, uint32_t index);
// TrackRoute::ClampPosition (sub_820F8420): out = FromIndex(ToIndex(packed)).
void ClampPosition(uint8_t* base, uint32_t out, uint32_t packed);

// --- Distances ---

// TrackRoute::LinkDistance (sub_820F8720): from b's section over the links
// to a's, `depth` more levels deep; `distance` advanced as the original
// advances its float. True when found (the original writes its float only
// then).
bool LinkDistance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, float& distance,
                  int32_t depth);
// TrackRoute::Distance (sub_820F9278): the distance from a to b in meters
// (f1); 10000.0 ([0x82002814]) when they aren't linked. `linked` (when not
// null; set only for positions in different sections) whether
// LinkDistance found them (the original's r3 then).
double Distance(uint8_t* base, uint32_t route, uint32_t a, uint32_t b, bool* linked = nullptr);

// --- Points of a slice (the global route's file) ---

// TrackRoute::SlicePoint (sub_820F9768, with its case 0, sub_820F985C):
// out (x, y, z) = point `which` of the section's slice: 0 and 3 the center,
// 1 and 2 the two point tables', 4 the center's half-sum with itself;
// above 16, nothing. False, with nothing done, for 5 to 16 (sub_820F8FA0's
// cases, never seen in play: the original's).
bool SlicePoint(uint8_t* base, uint32_t out, uint32_t which, uint32_t section, uint32_t slice);

}  // namespace track_route
