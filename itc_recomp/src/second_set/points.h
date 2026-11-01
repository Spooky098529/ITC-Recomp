// The second set's point records as plain C++ (docs/functions/second-set.md):
// the 240-byte records a node or a shape of a part model is found into. Guest
// data shared with the recompiled code, so these work on guest memory with
// the original's stores, their frames' words included (the saved registers
// and v127's slot aren't written; Clear's back chain is, as a caller's
// Clear frame words lie under it). `stack` is the guest r1 at entry.
// Adapters: src/hooks/second_set.cpp.

#pragma once

#include <cstdint>

namespace second_set {

constexpr uint32_t kPointRecordBytes = 240;
constexpr uint32_t kClearFrameBytes = 208;
constexpr uint32_t kConstructFrameBytes = 96;
constexpr uint32_t kFindShapeFrameBytes = 208;

// PointRecord::SetNode (sub_820CE8D8): the string at `name` copied, with its
// 0, into +160.
void PointRecordSetNode(uint8_t* base, uint32_t record, uint32_t name);
// PointRecord::SetShape (sub_820CE900): the same into +192.
void PointRecordSetShape(uint8_t* base, uint32_t record, uint32_t name);
// PointRecord::Clear (sub_820CEA08): +224, +228 = -1; +0, +4, +8 = 0; the
// identity built in the frame (+96) and copied to +16 and +80 (memcpy, 64
// bytes each); the frame's +80 = (0.0, 0.0, 0.0, 1.0) copied to +144; the
// names at +160 and +192 emptied. Returns what memcpy leaves in r3 (+80).
// Only where memcpy is native (native::Buffers_NativeMemcpy, the adapter's
// check).
uint32_t PointRecordClear(uint8_t* base, uint32_t record, uint32_t stack);
// PointRecord::PointRecord (sub_820D0270): the w's of the nine rows at +16
// (+28 to +156) = 1.0, then PointRecord::Clear. Returns the record.
uint32_t PointRecordConstruct(uint8_t* base, uint32_t record, uint32_t stack);
// ModelPoint::FindShape (sub_820CEB30): +228 = -1, +0, +4, +8 = 0, the
// identity (from the frame's +80) into +16; then the model's nodes (+408 of
// them, 192 bytes each at [+36]): the first whose shape (+40 not -1; 544
// bytes each at [model + 40]) is named as the record's +192 (NameMatches)
// gives +4 = the model, +8 = the shape, +228 = the shape's index; 1. Else 0.
uint32_t ModelPointFindShape(uint8_t* base, uint32_t record, uint32_t model, uint32_t stack);

}  // namespace second_set
