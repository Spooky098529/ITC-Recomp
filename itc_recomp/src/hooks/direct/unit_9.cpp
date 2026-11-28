// Direct entries for the fallback originals (docs/missing-functions.md):
// hand ports with no adapter that lifted/originals.cpp calls by a constant
// address, reached through ITC_DIRECT_ENTRY's lifted::entry::e<address>.
// Swarm unit 9's shims that need nothing private to an area's hooks file.

#include "hooks/native_entry.h"

#include "guest.h"
#include "lifted/originals.h"

#include "archive/archive.h"
#include "career/career.h"
#include "collision/collision.h"
#include "matrix_math/matrix_math.h"
#include "model_animation/model_animation.h"
#include "schema/screen_objects.h"
#include "screen_objects/uv_trees.h"
#include "std_string/std_string.h"

#include <bit>
#include <cstdint>

namespace {

namespace so = screen_objects;

// A tree's key (a double) at its guest address.
double KeyAt(uint32_t address) { return std::bit_cast<double>(guest::LoadU64(address)); }

// A checked iterator stored where the original returns it (r3).
void StoreIterator(uint32_t out, so::Iterator it) {
  guest::StoreU32(out + 0, it.tree);
  guest::StoreU32(out + 4, it.node);
}

void StoreInsertResult(uint32_t out, so::InsertResult result) {
  StoreIterator(out, result.where);
  guest::StoreU8(out + 8, result.inserted ? 1 : 0);
}

// An iterator passed by value in one register (the tree in the high word).
so::Iterator IteratorOf(uint64_t reg) { return {uint32_t(reg >> 32), uint32_t(reg)}; }

// ItemRecords::Add*'s Opponent, read where r4 points (as the originals).
career::Opponent OpponentAt(uint32_t at) {
  return {guest::LoadU32(at), guest::LoadU32(at + 4), guest::LoadU32(at + 8)};
}

}  // namespace

// --- screen_objects: the UvColumn and UvMap trees (uv_trees.h) ---

// UvMap::AllocateNodes: r3 = the count; r3 the block.
ITC_DIRECT_ENTRY(8212FC50) {
  ctx.fpscr.disableFlushMode();
  if (const auto block = so::MapAllocateNodes(ctx.r3.u32)) {
    ctx.r3.u64 = *block;
    return;
  }
  lifted::original::c8212FC50(ctx);
}

// UvColumn::LowerBound and UvMap::LowerBound: r3 = the iterator out, r4 =
// the tree, r5 = the key's address; r3 the iterator out.
ITC_DIRECT_ENTRY(8212FFC8) {
  ctx.fpscr.disableFlushMode();
  if (const auto it = so::ColumnLowerBound(ctx.r4.u32, KeyAt(ctx.r5.u32))) {
    StoreIterator(ctx.r3.u32, *it);
    return;
  }
  lifted::original::c8212FFC8(ctx);
}
ITC_DIRECT_ENTRY(82130028) {
  ctx.fpscr.disableFlushMode();
  if (const auto it = so::MapLowerBound(ctx.r4.u32, KeyAt(ctx.r5.u32))) {
    StoreIterator(ctx.r3.u32, *it);
    return;
  }
  lifted::original::c82130028(ctx);
}

// UvColumn::Copy: r3 = the (constructed) column, r4 = the source; r3 the new
// root.
ITC_DIRECT_ENTRY(82130088) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = so::ColumnCopy(ctx.r3.u32, ctx.r4.u32);
}

// UvColumn::UvColumn: r3 = the column; r3 the head.
ITC_DIRECT_ENTRY(82130140) {
  ctx.fpscr.disableFlushMode();
  so::ColumnConstruct(ctx.r3.u32);
  ctx.r3.u64 = schema::ScreenObjectUvColumn(ctx.r3.u32).head().address;
}

// UvColumn::InsertUnique and UvMap::InsertUnique: r3 = the result out, r4 =
// the tree, r5 = the value; r3 the result out.
ITC_DIRECT_ENTRY(82131CB8) {
  ctx.fpscr.disableFlushMode();
  if (const auto result = so::ColumnInsertUnique(ctx.r4.u32, ctx.r5.u32)) {
    StoreInsertResult(ctx.r3.u32, *result);
    return;
  }
  lifted::original::c82131CB8(ctx);
}
ITC_DIRECT_ENTRY(821321F8) {
  ctx.fpscr.disableFlushMode();
  if (const auto result = so::MapInsertUnique(ctx.r4.u32, ctx.r5.u32)) {
    StoreInsertResult(ctx.r3.u32, *result);
    return;
  }
  lifted::original::c821321F8(ctx);
}

// ~UvColumn: r3 = the column; r3 the freed head.
ITC_DIRECT_ENTRY(82131EE0) {
  ctx.fpscr.disableFlushMode();
  if (const auto head = so::ColumnDestroy(ctx.r3.u32)) {
    ctx.r3.u64 = *head;
    return;
  }
  lifted::original::c82131EE0(ctx);
}

// UvMap::Erase: r3 = the iterator out, r4 = the map, r5 and r6 = the two
// iterators by value; r3 the iterator out.
ITC_DIRECT_ENTRY(82132BD8) {
  ctx.fpscr.disableFlushMode();
  if (const auto it =
          so::MapErase(ctx.r4.u32, IteratorOf(ctx.r5.u64), IteratorOf(ctx.r6.u64))) {
    StoreIterator(ctx.r3.u32, *it);
    return;
  }
  lifted::original::c82132BD8(ctx);
}

// --- collision ---

// Shape::Shape(const&): r3 = the shape, r4 = the source; r3 the shape (left).
ITC_DIRECT_ENTRY(820BAB48) {
  if (!collision::ShapeCopyConstruct(ctx.r3.u32, ctx.r4.u32)) {
    lifted::original::c820BAB48(ctx);
  }
}

// CollisionShape::Type: r3 = the record; r3 the type.
ITC_DIRECT_ENTRY(820BACB8) {
  uint32_t type = 0;
  if (!collision::CollisionShapeType(ctx.r3.u32, &type)) {
    lifted::original::c820BACB8(ctx);
    return;
  }
  ctx.r3.u64 = type;
}

// CollisionShape::Translate: r3 = the record, r4 = the offset; the
// original's frame below sp.
ITC_DIRECT_ENTRY(820BACD0) {
  ctx.fpscr.disableFlushMode();
  if (!collision::CollisionShapeTranslate(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192)) {
    lifted::original::c820BACD0(ctx);
  }
}

// Shape::Shape: r3 = the shape (left in r3); the original's 176-byte frame.
ITC_DIRECT_ENTRY(820BB020) {
  ctx.fpscr.disableFlushMode();
  collision::ShapeConstruct(ctx.r3.u32, ctx.r1.u32 - 176);
}

// Contact::Flip: r3 = the contact; f1 the -1.0, r3 the contact + 32.
ITC_DIRECT_ENTRY(820BC858) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = collision::ContactFlip(ctx.r3.u32);
  ctx.r3.u64 = ctx.r3.u32 + 32;
}

// BoxBox::Separated: r3 = the query, r4 = the cache; r3 the result. Shapes
// that aren't boxes run the original.
ITC_DIRECT_ENTRY(820BD280) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  if (!collision::BoxBoxSeparated(ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 160, &result)) {
    lifted::original::c820BD280(ctx);
    return;
  }
  ctx.r3.u64 = result;
}

// BoxBox::SeparatedAfterMove: r3 = the query; r3 the result.
ITC_DIRECT_ENTRY(820BD578) {
  ctx.fpscr.disableFlushMode();
  uint32_t result = 0;
  if (!collision::BoxBoxSeparatedAfterMove(ctx.r3.u32, ctx.r1.u32 - 240, &result)) {
    lifted::original::c820BD578(ctx);
    return;
  }
  ctx.r3.u64 = result;
}

// BoxBox::EdgesCrossing: r3 = out, r4 = the block.
ITC_DIRECT_ENTRY(820BF080) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = collision::BoxBoxEdgesCrossing(ctx.r3.u32, ctx.r4.u32);
}

// --- model_animation ---

// TrackSet's sampling and advance: r3 = the set, r4 = the value's index, f1
// = the step (Advance); r3 1 when it looped.
ITC_DIRECT_ENTRY(825A0FE0) {
  ctx.fpscr.disableFlushMode();
  model_animation::TrackSetSample32(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(825A10A8) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = model_animation::TrackSetAdvance32(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}
ITC_DIRECT_ENTRY(825A1C88) {
  ctx.fpscr.disableFlushMode();
  model_animation::TrackSetSampleFloat4(ctx.r3.u32, ctx.r4.u32);
}
ITC_DIRECT_ENTRY(825A1D68) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = model_animation::TrackSetAdvance(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}

// Key::Interpolate: r3, r4 = the keys, f1 = the time; f1 the value.
ITC_DIRECT_ENTRY(825A66F8) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = model_animation::KeyInterpolate(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64);
}

// --- career ---

// DayLog::Push: r3 = the log, r4 = the day block.
ITC_DIRECT_ENTRY(82112028) {
  ctx.fpscr.disableFlushMode();
  ctx.r3.u64 = career::DayLogPush(ctx.r3.u32, ctx.r4.u32);
}

// Stats::AddWin, AddLoss, AddOutcome3: r3 = the stats, r4 = &the Opponent,
// r5 = the mode.
ITC_DIRECT_ENTRY(821139B8) {
  ctx.r3.u64 = career::StatsAddWin(ctx.r3.u32, OpponentAt(ctx.r4.u32), ctx.r5.u32);
}
ITC_DIRECT_ENTRY(82113AB8) {
  ctx.r3.u64 = career::StatsAddLoss(ctx.r3.u32, OpponentAt(ctx.r4.u32), ctx.r5.u32);
}
ITC_DIRECT_ENTRY(82113B50) {
  ctx.r3.u64 = career::StatsAddOutcome3(ctx.r3.u32, OpponentAt(ctx.r4.u32), ctx.r5.u32);
}

// --- std_string ---

// _Copy: r3 = the string, r4 = the new length, r5 = the length kept. The
// original leaves r3 as its _Tidy did (the old block when it had one, else
// the string).
ITC_DIRECT_ENTRY(82091458) {
  const uint32_t string = ctx.r3.u32;
  const bool frees = guest::LoadU32(string + std_string::kCapacity) >= std_string::kInPlace;
  const uint32_t block = guest::LoadU32(string + std_string::kText);
  if (!std_string::Copy(string, ctx.r4.u32, ctx.r5.u32)) {
    lifted::original::c82091458(ctx);
    return;
  }
  if (frees) {
    ctx.r3.u64 = block;
  }
}

// The wildcard search: r3 = the text, r4 = the pattern, r5 = &the offset,
// r6 = &the length; r3 the match's start or 0.
ITC_DIRECT_ENTRY(82592740) {
  ctx.r3.u64 = std_string::WildcardSearch(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32);
}

// --- matrix_math ---

// Matrix::InverseRotate: r3 = out (left in r3), r4 = the matrix, r5 = the
// vector.
ITC_DIRECT_ENTRY(82293998) {
  ctx.fpscr.disableFlushMode();
  matrix_math::InverseRotate(ctx.r3.u32, ctx.r4.u32, ctx.r5.u32);
}

// Quaternion::FromAxisAngle: r3 = out, r4 = the axis, f1 = the angle; the
// sine and cosine at the original's r1 - 32; f1 as the original leaves it.
ITC_DIRECT_ENTRY(82293E68) {
  ctx.fpscr.disableFlushMode();
  ctx.f1.f64 = matrix_math::QuaternionFromAxisAngle(ctx.r3.u32, ctx.r4.u32, ctx.f1.f64,
                                                    ctx.r1.u32 - 32);
}

// --- archive ---

// List::Append: r3 = the list, r4 = the node; r3 1, or 0 for no node.
ITC_DIRECT_ENTRY(82294BA8) {
  ctx.r3.u64 = archive::ListAppend(ctx.r3.u32, ctx.r4.u32);
}
