// The second set's objects' register steps (src/second_set/,
// docs/functions/second-set.md), shared by the adapters
// (src/hooks/second_set.cpp, behind --native_second_set) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ can't (memcpy not native).

#pragma once

#include "guest.h"

namespace hooks {

// PointRecord::SetNode sub_820CE8D8 and SetShape sub_820CE900: r3 = the
// record (left), r4 = the string.
void SecondSetPointSetNode(PPCContext& ctx, uint8_t* base);
void SecondSetPointSetShape(PPCContext& ctx, uint8_t* base);
// PointRecord::Clear sub_820CEA08 (r3 = the record; r3 = +80) and
// PointRecord::PointRecord sub_820D0270 (r3 = the record, left).
void SecondSetPointClear(PPCContext& ctx, uint8_t* base);
void SecondSetPointConstruct(PPCContext& ctx, uint8_t* base);
// ModelPoint::FindShape sub_820CEB30: r3 = the record, r4 = the part model;
// r3 = 1 or 0.
void SecondSetFindShape(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
