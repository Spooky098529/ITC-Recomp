// The collision shapes (src/collision/) against the recompiled functions:
// boxes, a shape record, a contact and a query in a region of guest stack
// further down (below the car physics' regions), filled anew for each case
// with float words, the boxes' and the record's template's vtable the box's;
// the static vector BoxShape::FaceDirection writes (0x826C2DE0) set for
// each case and put back after. Compared: the region, the static vector,
// r3 and (where the original leaves a value there) f1. The originals run
// with the native flags of their callees off (the collision shapes, the
// vector library and the car physics), so the whole chain is the
// recompiled code's. Three groups, as the car physics' harness: numbers
// only (below 10^6 in size, denormals and signed zeros among them), those
// numbers with one NaN word put in the region or f1, and anything
// anywhere, NaNs and infinities too (expected to differ: where two NaNs
// meet in one operation, which comes out depends on the order the compiler
// gave the operands, in the translated code as in ours; the game's boxes
// hold no NaNs).

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "car_physics/car_physics.h"
#include "collision/collision.h"
#include "hooks/heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <iterator>
#include <string>

ITC_FLAG_DECLARE(bool, native_collision);
ITC_FLAG_DECLARE(bool, native_car_physics);
ITC_FLAG_DECLARE(bool, native_vector_library);
ITC_FLAG_DECLARE(bool, native_matrix_math);
ITC_FLAG_DECLARE(bool, native_vehicle_accessors);

namespace harness {

namespace {

namespace co = collision;

// The register steps: the cases run the plain C++ through these (not the
// adapters), with the registers each adapter leaves.
#define ITC_NATIVE_STEP(name, body)           \
  void name(PPCContext& ctx, uint8_t* base) { \
    ctx.fpscr.disableFlushMode();             \
    body;                                     \
  }

ITC_NATIVE_STEP(NativeCornerCount, ctx.r3.u64 = co::BoxCornerCount())
ITC_NATIVE_STEP(NativeFaceCount, ctx.r3.u64 = co::BoxFaceCount())
ITC_NATIVE_STEP(NativeEdgeCount, ctx.r3.u64 = co::BoxEdgeCount())
ITC_NATIVE_STEP(NativeCorner, ctx.r3.u64 = co::BoxCorner(ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFace, ctx.r3.u64 = co::BoxFace(ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEdgeStart, ctx.r3.u64 = co::BoxEdgeStart(base, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEdgeEnd, ctx.r3.u64 = co::BoxEdgeEnd(base, ctx.r4.u32))
ITC_NATIVE_STEP(NativeEdgesStart, ctx.r3.u64 = co::BoxEdgesStart(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeEdgesEnd, ctx.r3.u64 = co::BoxEdgesEnd(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetCacheWord,
                co::BoxSetCacheWord(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeCacheWord, ctx.r3.u64 = co::BoxCacheWord(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeCopyCache, co::BoxCopyCache(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeRadius, ctx.f1.f64 = co::ShapeRadius(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeLowerCorner, ctx.r3.u64 = co::BoxLowerCorner(base, ctx.r3.u32, ctx.r4.u32))
ITC_NATIVE_STEP(NativeFaceDistance, uint32_t r3 = 0;
                ctx.f1.f64 = co::BoxFaceDistance(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, &r3);
                ctx.r3.u64 = r3)
ITC_NATIVE_STEP(NativeFaceDirection,
                ctx.r3.u64 = co::BoxFaceDirection(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 208))
ITC_NATIVE_STEP(NativeBoxTranslate, ctx.f1.f64 = co::BoxTranslate(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u32 + 48)
ITC_NATIVE_STEP(NativeShapeConstruct, co::ShapeConstruct(base, ctx.r3.u32, ctx.r1.u32 - 176))
void NativeShapeCopy(PPCContext& ctx, uint8_t* base) {
  if (!co::ShapeCopyConstruct(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820BAB48(ctx, base);
  }
}
void NativeBoxCopy(PPCContext& ctx, uint8_t* base) {
  if (!co::BoxCopyConstruct(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820BB688(ctx, base);
  }
}
ITC_NATIVE_STEP(NativeSetCorners,
                ctx.f1.f64 = co::BoxSetCorners(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                               ctx.r1.u32 - 224);
                ctx.r3.u64 = ctx.r3.u32 + 96)
ITC_NATIVE_STEP(NativeBoxConstruct,
                co::BoxConstruct(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 112))
void NativeBoxPlace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!co::BoxPlace(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32 - 288)) {
    __imp__sub_820B9C40(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 96;
}
ITC_NATIVE_STEP(NativeUpdateBounds, co::CollisionShapeUpdateBounds(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeUpdateMotion,
                co::CollisionShapeUpdateMotion(base, ctx.r3.u32, ctx.r1.u32 - 240);
                ctx.r3.u64 = ctx.r3.u32 + 96)
void NativeRecordPlace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!co::CollisionShapePlace(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 608)) {
    __imp__sub_820BB758(ctx, base);
  }
}
void NativeRecordTranslate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!co::CollisionShapeTranslate(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 192)) {
    __imp__sub_820BACD0(ctx, base);
  }
}
// CollisionObject::Translate (src/car_physics/): a record whose current
// copy isn't a box runs the original (as the adapter).
void NativeObjectTranslate(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!car_physics::CollisionObjectTranslate(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 128)) {
    __imp__sub_820B7B60(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 304;
}
// The shapes' lifetime (operator new and delete natively), with the
// adapters' fallbacks.
void NativeDeletingDestructor(PPCContext& ctx, uint8_t* base) {
  if (!co::ShapeDeletingDestructor(base, ctx.r3.u32, ctx.r4.u32)) {
    __imp__sub_820BB140(ctx, base);
  }
}
void NativeClone(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t copy = 0;
  if (!co::BoxClone(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 96, &copy)) {
    __imp__sub_820BB218(ctx, base);
    return;
  }
  ctx.r3.u64 = copy;
}
void NativeDestroy(PPCContext& ctx, uint8_t* base) {
  uint32_t r3 = 0;
  if (!co::CollisionShapeDestroy(base, ctx.r3.u32, &r3)) {
    __imp__sub_820BAAA0(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeSetUp(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!co::CollisionShapeSetUp(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.r6.u32,
                               ctx.r1.u32 - 128)) {
    __imp__sub_820BB580(ctx, base);
  }
}
ITC_NATIVE_STEP(NativeRelativePlacement,
                ctx.r3.u64 = co::BoxRelativePlacement(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeRelativeCenter,
                ctx.r3.u64 = co::BoxRelativeCenter(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeOutcode, ctx.r3.u64 = co::BoxBoxOutcode(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeBlockCorners, co::BoxBoxBlockCorners(base, ctx.r3.u32, ctx.r1.u32 - 320);
                ctx.r3.u64 = ctx.r3.u32 + 304)
ITC_NATIVE_STEP(NativeMoveCorners, co::BoxBoxMoveCorners(base, ctx.r3.u32, ctx.r4.u32);
                ctx.r3.u64 = ctx.r3.u32 + 224)
ITC_NATIVE_STEP(NativeFaceSeparates,
                ctx.r3.u64 = co::BoxBoxFaceSeparates(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32))
// Shapes that aren't boxes: the original (as the adapters).
#define ITC_NATIVE_TEST_STEP(name, address, call) \
  void name(PPCContext& ctx, uint8_t* base) {     \
    ctx.fpscr.disableFlushMode();                 \
    uint32_t result = 0;                          \
    if (!call) {                                  \
      __imp__sub_##address(ctx, base);            \
      return;                                     \
    }                                             \
    ctx.r3.u64 = result;                          \
  }
ITC_NATIVE_TEST_STEP(NativeSeparated, 820BD280,
                     co::BoxBoxSeparated(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 160, &result))
ITC_NATIVE_TEST_STEP(NativeSeparatedAfterMove, 820BD578,
                     co::BoxBoxSeparatedAfterMove(base, ctx.r3.u32, ctx.r1.u32 - 240, &result))
ITC_NATIVE_TEST_STEP(NativeQuerySeparated, 820BD508,
                     co::QuerySeparated(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112, &result))
ITC_NATIVE_TEST_STEP(NativeQuerySeparatedAfterMove, 820BD6E0,
                     co::QuerySeparatedAfterMove(base, ctx.r3.u32, ctx.r1.u32 - 96, &result))
ITC_NATIVE_STEP(NativeNextCheckTime, uint32_t r3 = 0;
                ctx.f1.f64 = co::CollisionPairNextCheckTime(base, ctx.r3.u32, ctx.r1.u32 - 160,
                                                            &r3);
                ctx.r3.u64 = r3)
ITC_NATIVE_STEP(NativeSwapForImpact, ctx.r3.u64 = co::BoxBoxSwapForImpact(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSwapForPushOut, ctx.r3.u64 = co::BoxBoxSwapForPushOut(base, ctx.r3.u32))
void NativeCornerImpacts(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  uint32_t r3 = 0;
  if (!co::BoxBoxCornerImpacts(base, ctx.r3.u32, &r3)) {
    __imp__sub_820BECA0(ctx, base);
    return;
  }
  ctx.r3.u64 = r3;
}
void NativeFindImpact(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  if (!co::BoxBoxFindImpact(base, ctx.r3.u32)) {
    __imp__sub_820BF038(ctx, base);
    return;
  }
  ctx.r3.u64 = ctx.r3.u32 + 64;
}
ITC_NATIVE_TEST_STEP(NativeQueryFindImpact, 820BCE80,
                     co::QueryFindImpact(base, ctx.r3.u32, &result))
ITC_NATIVE_STEP(NativeEdgesCrossing,
                ctx.r3.u64 = co::BoxBoxEdgesCrossing(base, ctx.r3.u32, ctx.r4.u32))
// The query's push-outs: shapes that aren't boxes run the original.
#define ITC_NATIVE_PUSH_OUT_STEP(name, address, call) \
  void name(PPCContext& ctx, uint8_t* base) {         \
    ctx.fpscr.disableFlushMode();                     \
    uint32_t r3 = 0;                                  \
    if (!call) {                                      \
      __imp__sub_##address(ctx, base);                \
      return;                                         \
    }                                                 \
    ctx.r3.u64 = r3;                                  \
  }
ITC_NATIVE_PUSH_OUT_STEP(NativePushOutEdges, 820BF320,
                         co::BoxBoxPushOutEdges(base, ctx.r3.u32, ctx.r1.u32 - 544, &r3))
ITC_NATIVE_PUSH_OUT_STEP(NativePushOutCorners, 820BE150,
                         co::BoxBoxPushOutCorners(base, ctx.r3.u32, ctx.r1.u32 - 304, &r3))
ITC_NATIVE_PUSH_OUT_STEP(NativeFindPushOut, 820BFB28,
                         co::BoxBoxFindPushOut(base, ctx.r3.u32, ctx.r1.u32 - 96, &r3))
ITC_NATIVE_TEST_STEP(NativeQueryFindPushOut, 820BCDD0,
                     co::QueryFindPushOut(base, ctx.r3.u32, ctx.r4.u32, ctx.r1.u32 - 112,
                                          &result))
ITC_NATIVE_STEP(NativeWordAt8, ctx.r3.u64 = co::WordAt8(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeCurrentCenter,
                ctx.r3.u64 = co::CollisionShapeCurrentCenter(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativePreviousCenter,
                ctx.r3.u64 = co::CollisionShapePreviousCenter(base, ctx.r3.u32))
// A template that isn't a box: the original (as the adapters).
#define ITC_NATIVE_RECORD_STEP(name, address, call) \
  void name(PPCContext& ctx, uint8_t* base) {       \
    uint32_t value = 0;                             \
    if (!call) {                                    \
      __imp__sub_##address(ctx, base);              \
      return;                                       \
    }                                               \
    ctx.r3.u64 = value;                             \
  }
ITC_NATIVE_RECORD_STEP(NativeType, 820BACB8, co::CollisionShapeType(base, ctx.r3.u32, &value))
ITC_NATIVE_RECORD_STEP(NativePrevious, 820BADF8,
                       co::CollisionShapePrevious(base, ctx.r3.u32, &value))
ITC_NATIVE_RECORD_STEP(NativeCurrent, 820BAE38,
                       co::CollisionShapeCurrent(base, ctx.r3.u32, &value))
ITC_NATIVE_STEP(NativeQueryConstruct, co::QueryConstruct(base, ctx.r3.u32))
ITC_NATIVE_STEP(NativeSetShapes, co::QuerySetShapes(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32))
ITC_NATIVE_STEP(NativeContactSet,
                co::ContactSet(base, ctx.r3.u32, ctx.r4.u32, ctx.r5.u32, ctx.f1.f64, ctx.f2.f64);
                ctx.r3.u64 = ctx.r3.u32 + 32)
ITC_NATIVE_STEP(NativeContactFlip, ctx.f1.f64 = co::ContactFlip(base, ctx.r3.u32);
                ctx.r3.u64 = ctx.r3.u32 + 32)

// Runs the recompiled version with the native flags off, so the functions
// it calls that are ours run the originals too.
template <GuestFunction recompiled>
void Original(PPCContext& ctx, uint8_t* base) {
  bool* flags[] = {&ITC_FLAG(native_collision), &ITC_FLAG(native_car_physics),
                   &ITC_FLAG(native_vector_library), &ITC_FLAG(native_matrix_math),
                   &ITC_FLAG(native_vehicle_accessors)};
  bool saved[std::size(flags)];
  for (size_t i = 0; i < std::size(flags); i++) {
    saved[i] = *flags[i];
    *flags[i] = false;
  }
  recompiled(ctx, base);
  for (size_t i = 0; i < std::size(flags); i++) {
    *flags[i] = saved[i];
  }
}

// NaNs of both kinds and signs, infinities, denormals, signed zeros.
constexpr uint32_t kOddFloats[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001,
                                   0x7F800000, 0xFF800000, 0x00000001, 0x807FFFFF,
                                   0x00000000, 0x80000000, 0x7F7FFFFF, 0x3F800000};

// Float words: odd floats, random bits, tiny values (below 2^-60, whose
// products are denormal: the vector unit's flush-to-zero), and ordinary
// values (-64 to 64) so the arithmetic is mostly on numbers.
uint32_t FloatWord(Random& r) {
  switch (r.Below(7)) {
    case 0:
      return kOddFloats[r.Below(std::size(kOddFloats))];
    case 1:
      return uint32_t(r.Next());
    case 2:
      return std::bit_cast<uint32_t>(
          std::ldexp(float(int32_t(r.Below(1 << 20)) - (1 << 19)), -80));
    default:
      return std::bit_cast<uint32_t>(float(int32_t(r.Below(1 << 20)) - (1 << 19)) / 8192.0f);
  }
}

// Numbers below 10^6 in size (denormals and signed zeros among them).
uint32_t NumberWord(Random& r) {
  uint32_t word;
  do {
    word = FloatWord(r);
  } while (!(std::fabs(std::bit_cast<float>(word)) < 1e6f));
  return word;
}

Random Seeded(uint64_t seed) {
  Random r;
  for (uint64_t i = 0; i < seed; i++) {
    r.Next();
  }
  return r;
}

// The region, below the car physics' (0x14000 below the scratch area).
constexpr uint32_t kRegion = 0x20000;
constexpr uint32_t kRegionBytes = 0x1C00;
// In the region: boxes A (+0x000), B (+0x200) and C (+0xC00); a shape record
// (+0x400: its template A, previous copy B, current copy C); a vector
// argument (+0x480); a contact (+0x4C0); a query (+0x500, 1,632 bytes).
// a second vector (+0x4A0); a matrix (the query's first 64 bytes).
constexpr uint32_t kBoxA = 0x000, kBoxB = 0x200, kBoxC = 0xC00, kRecord = 0x400,
                   kVector = 0x480, kVector2 = 0x4A0, kContact = 0x4C0, kQuery = 0x500,
                   kMatrix = 0x500;
// For the query's tests: a second record (+0x1400: its template D +0xE00,
// previous copy E +0x1000, current copy F +0x1200), the query (+0x500) set
// up with both records and its four blocks (+0x570 on, as Query::Query
// sets them), the pair's cache word (+0x1480). For the next check time: a
// pair (+0x1500: objects +0x1600 and +0x1900, each with its body at +16
// (+0x1800, +0x1B00), a shape at +20 (A, D) and a record at +32 whose
// current copy is C or F).
constexpr uint32_t kBoxD = 0xE00, kBoxE = 0x1000, kBoxF = 0x1200, kRecordB = 0x1400,
                   kBlock = kQuery + 112, kCache = 0x1480, kPair = 0x1500, kObjectA = 0x1600,
                   kObjectB = 0x1900, kBodyA = 0x1800, kBodyB = 0x1B00;
constexpr uint32_t kFaceDirection = 0x826C2DE0;

// The registers each function takes (`i`, an index in range).
enum Args {
  kNone,          // nothing
  kBoxIndex,      // r3 = box A, r4 = i
  kIndexR4,       // r4 = i
  kIndexR3,       // r3 = i
  kCacheSet,      // r3 = box A, r4 = i, r5 = a word
  kTwoBoxes,      // r3 = box A, r4 = box B
  kBoxR3,         // r3 = box A
  kBoxPoint,      // r3 = box A, r4 = i, r5 = the vector
  kBoxVector,     // r3 = box A, r4 = the vector
  kRecordR3,      // r3 = the record
  kQueryR3,       // r3 = the query
  kQueryShapes,   // r3 = the query, r4 = the record, r5 = box A
  kContactSet,    // r3 = the contact, r4 = the vector, r5 = box B + 16, f1, f2
  kContactR3,     // r3 = the contact
  kCorners,       // r3 = box A, r4 = the vector, r5 = the second vector
  kCopy,          // r3 = box B, r4 = box A
  kPlace,         // r3 = box B, r4 = the matrix, r5 = box A
  kRecordMatrix,  // r3 = the record, r4 = the matrix (its +112 0.0 one case in three)
  kRecordVector,  // r3 = the record, r4 = the vector
  kRelative,      // r3 = box A, r4 = the query's first block, r5 = box B
  kBlockR3,       // r3 = the query's first block
  kMoveCorners,   // r3 = the first block's corners, r4 = the vector
  kFaces,         // r3 = face i of box A, r4 = face i of box C, r5 = box B, r6 = box E
  kQueryCache,    // r3 = the query (set up), r4 = the cache word
  kQuerySetUp,    // r3 = the query (set up)
  kPairR3,        // r3 = the pair (set up)
  kObjectVector,  // r3 = object A (its body and a record of A, B, C), r4 = the vector
  kEdges,         // r3 = the vector (48 bytes out), r4 = the query's first block
  kQueryVector,   // r3 = the query (set up), r4 = the vector
};

void Append32(std::vector<uint8_t>& out, uint32_t word) {
  for (int i = 0; i < 4; i++) {
    out.push_back(uint8_t(word >> (24 - 8 * i)));
  }
}

// The shapes' lifetime: blocks from the allocation layer (the same sizes on
// both sides, at different addresses), so each case compares the region
// with the blocks' addresses in it replaced by their number, the blocks'
// contents while allocated, which were freed, and r3 likewise. The blocks
// left are freed after each side. Only with --native_heap (the adapters'
// condition): the recompiled side's operator new and delete then use the
// layer too.
void LifetimeCases(Context& h, uint32_t region) {
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_INFO("harness: collision: the shapes' lifetime skipped (--native_heap off)");
    return;
  }
  enum Kind { kDeleting, kClone, kDestroy, kSetUp };
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Kind kind;
  } functions[] = {
      {"Shape::DeletingDestructor (sub_820BB140)", Original<__imp__sub_820BB140>,
       NativeDeletingDestructor, kDeleting},
      {"BoxShape::Clone (sub_820BB218)", Original<__imp__sub_820BB218>, NativeClone, kClone},
      {"CollisionShape::~CollisionShape (sub_820BAAA0)", Original<__imp__sub_820BAAA0>,
       NativeDestroy, kDestroy},
      {"CollisionShape::SetUp (sub_820BB580)", Original<__imp__sub_820BB580>, NativeSetUp,
       kSetUp},
  };
  enum Group { kNumbers, kNaNs };
  const char* const kGroupNames[] = {"", ", NaNs and infinities anywhere (expected to differ)"};
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      for (uint64_t seed = 0; seed < 100; seed++) {
        Random pick = Seeded(seed * 19 + 7 * uint64_t(group) + 2);
        auto word = [group](Random& r) {
          return group == kNaNs ? FloatWord(r) : NumberWord(r);
        };
        const uint32_t flags = uint32_t(pick.Next());
        // Copies: 0 one time in four each; clones into given memory (box
        // B) one time in four.
        const bool no_previous = pick.Below(4) == 0, no_current = pick.Below(4) == 0;
        const bool given_memory = pick.Below(4) == 0;
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random words = Seeded(seed * 23 + 5 * uint64_t(group) + 9);
          for (uint32_t i = 0; i < kRegionBytes; i += 4) {
            guest::StoreU32(h.base, region + i, word(words));
          }
          for (uint32_t box : {kBoxA, kBoxB, kBoxC}) {
            guest::StoreU32(h.base, region + box, co::kBoxVtable);
          }
          // The blocks given: one for the destructor, two copies for the
          // record (filled as boxes).
          std::vector<uint32_t> blocks;
          auto new_box = [&](bool box_vtable) {
            const uint32_t b = memory::Alloc(496, 0);
            for (uint32_t i = 0; i < 496; i += 4) {
              guest::StoreU32(h.base, b + i, word(words));
            }
            if (box_vtable) {
              guest::StoreU32(h.base, b, co::kBoxVtable);
            }
            blocks.push_back(b);
            return b;
          };
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          switch (f.kind) {
            case kDeleting:
              t.r3.u64 = new_box(pick.Below(2) == 0);
              t.r4.u64 = flags;
              break;
            case kClone:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = given_memory ? region + kBoxB : 0;
              break;
            case kDestroy:
            case kSetUp:
              guest::StoreU32(h.base, region + kRecord + 4, no_previous ? 0 : new_box(true));
              guest::StoreU32(h.base, region + kRecord + 8, no_current ? 0 : new_box(true));
              t.r3.u64 = region + kRecord;
              t.r4.u64 = region + kBoxA;
              t.r5.u64 = region + kMatrix;
              t.r6.u64 = given_memory ? region + kBoxB : 0;
              break;
          }
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          // The blocks the function allocated: the record's copies and r3
          // where the layer gave them and they aren't the ones given.
          std::vector<uint32_t> made;
          auto is_new = [&](uint32_t b) {
            return std::find(blocks.begin(), blocks.end(), b) == blocks.end() &&
                   memory::IsHeapBlock(b);
          };
          for (uint32_t b : {guest::LoadU32(h.base, region + kRecord + 4),
                             guest::LoadU32(h.base, region + kRecord + 8), t.r3.u32}) {
            if ((f.kind == kSetUp || b == t.r3.u32) && is_new(b) &&
                std::find(made.begin(), made.end(), b) == made.end()) {
              made.push_back(b);
            }
          }
          auto name = [&](uint32_t w) {
            for (size_t i = 0; i < blocks.size(); i++) {
              if (w == blocks[i]) {
                return uint32_t(0xB10C0000u + i);
              }
            }
            for (size_t i = 0; i < made.size(); i++) {
              if (w == made[i]) {
                return uint32_t(0x0E550000u + i);
              }
            }
            return w;
          };
          std::vector<uint8_t>& out = result[v];
          for (uint32_t i = 0; i < kRegionBytes; i += 4) {
            Append32(out, name(guest::LoadU32(h.base, region + i)));
          }
          Append32(out, name(t.r3.u32));
          for (uint32_t b : blocks) {
            const bool freed = !memory::IsHeapBlock(b);
            out.push_back(freed ? 1 : 0);
            // The first word (the destructor's vtable) is read freed or not.
            const uint32_t bytes = freed ? 4 : 496;
            for (uint32_t i = 0; i < bytes; i += 4) {
              Append32(out, name(guest::LoadU32(h.base, b + i)));
            }
          }
          for (uint32_t b : made) {
            Append32(out, memory::Size(b));
            for (uint32_t i = 0; i < 496; i += 4) {
              Append32(out, name(guest::LoadU32(h.base, b + i)));
            }
          }
          for (uint32_t b : blocks) {
            if (memory::IsHeapBlock(b)) {
              memory::Free(b);
            }
          }
          for (uint32_t b : made) {
            memory::Free(b);
          }
        }
        results.Check(result[0], result[1], fmt::format("seed {} flags {:08X}", seed, flags));
      }
      results.Report();
    }
  }
}

}  // namespace

void Collision(Context& h) {
  const uint32_t region = (h.data - kRegion) & ~0xFu;
  const struct {
    const char* name;
    GuestFunction recompiled, native;
    Args args;
    uint32_t count;  // the index's range
    bool compare_f1;
    // The motion's row: minus the current center transformed, plus the
    // center (Vec3::Sum): one NaN there meets its own negation.
    bool motion_row = false;
    // The push-outs: the blocks' corners' outcodes worked out from their
    // coordinates (corners inside the other box, edges crossing it), the
    // blocks' own outcodes 0 three times in four and the depth (+80) 0.0
    // one time in two.
    bool push_out = false;
  } functions[] = {
      {"BoxShape::CornerCount (sub_820B9EC8)", Original<__imp__sub_820B9EC8>, NativeCornerCount,
       kNone, 1, false},
      {"BoxShape::FaceCount (sub_820B9EE0)", Original<__imp__sub_820B9EE0>, NativeFaceCount,
       kNone, 1, false},
      {"BoxShape::EdgeCount (sub_820B9EF8)", Original<__imp__sub_820B9EF8>, NativeEdgeCount,
       kNone, 1, false},
      {"BoxShape::Corner (sub_820B9ED0)", Original<__imp__sub_820B9ED0>, NativeCorner, kBoxIndex,
       8, false},
      {"BoxShape::Face (sub_820B9EE8)", Original<__imp__sub_820B9EE8>, NativeFace, kBoxIndex, 6,
       false},
      {"BoxShape::EdgeStart (sub_820B9F98)", Original<__imp__sub_820B9F98>, NativeEdgeStart,
       kIndexR4, 12, false},
      {"BoxShape::EdgeEnd (sub_820B9FB0)", Original<__imp__sub_820B9FB0>, NativeEdgeEnd,
       kIndexR4, 12, false},
      {"BoxEdges::Start (sub_820B9FC8)", Original<__imp__sub_820B9FC8>, NativeEdgesStart,
       kIndexR3, 12, false},
      {"BoxEdges::End (sub_820B9FE0)", Original<__imp__sub_820B9FE0>, NativeEdgesEnd, kIndexR3,
       12, false},
      {"BoxShape::SetCacheWord (sub_820BA668)", Original<__imp__sub_820BA668>,
       NativeSetCacheWord, kCacheSet, 4, false},
      {"BoxShape::CacheWord (sub_820BA678)", Original<__imp__sub_820BA678>, NativeCacheWord,
       kBoxIndex, 4, false},
      {"BoxShape::CopyCache (sub_820BAA50)", Original<__imp__sub_820BAA50>, NativeCopyCache,
       kTwoBoxes, 1, false},
      {"Shape::Radius (sub_820BAB40)", Original<__imp__sub_820BAB40>, NativeRadius, kBoxR3, 1,
       true},
      {"BoxShape::LowerCorner (sub_820BA368)", Original<__imp__sub_820BA368>, NativeLowerCorner,
       kBoxIndex, 4, false},
      {"BoxShape::FaceDistance (sub_820BA3B0)", Original<__imp__sub_820BA3B0>,
       NativeFaceDistance, kBoxPoint, 6, true},
      {"BoxShape::FaceDirection (sub_820B9F00)", Original<__imp__sub_820B9F00>,
       NativeFaceDirection, kBoxIndex, 12, false},
      {"BoxShape::Translate (sub_820BA2F0)", Original<__imp__sub_820BA2F0>, NativeBoxTranslate,
       kBoxVector, 1, true},
      {"Shape::Shape (sub_820BB020)", Original<__imp__sub_820BB020>, NativeShapeConstruct,
       kBoxR3, 1, false},
      {"Shape::Shape(const&) (sub_820BAB48)", Original<__imp__sub_820BAB48>, NativeShapeCopy,
       kCopy, 1, false},
      {"BoxShape::BoxShape(const&) (sub_820BB688)", Original<__imp__sub_820BB688>,
       NativeBoxCopy, kCopy, 1, false},
      {"BoxShape::SetCorners (sub_820B99D0)", Original<__imp__sub_820B99D0>, NativeSetCorners,
       kCorners, 1, true},
      {"BoxShape::BoxShape (sub_820BB188)", Original<__imp__sub_820BB188>, NativeBoxConstruct,
       kCorners, 1, false},
      {"BoxShape::Place (sub_820B9C40)", Original<__imp__sub_820B9C40>, NativeBoxPlace, kPlace,
       1, false},
      {"CollisionShape::UpdateBounds (sub_820BAC00)", Original<__imp__sub_820BAC00>,
       NativeUpdateBounds, kRecordR3, 1, false},
      {"CollisionShape::UpdateMotion (sub_820BAE78)", Original<__imp__sub_820BAE78>,
       NativeUpdateMotion, kRecordR3, 1, false, true},
      {"CollisionShape::Place (sub_820BB758)", Original<__imp__sub_820BB758>, NativeRecordPlace,
       kRecordMatrix, 3, false, true},
      {"CollisionShape::Translate (sub_820BACD0)", Original<__imp__sub_820BACD0>,
       NativeRecordTranslate, kRecordVector, 1, false, true},
      {"CollisionObject::Translate (sub_820B7B60)", Original<__imp__sub_820B7B60>,
       NativeObjectTranslate, kObjectVector, 1, false, true},
      {"BoxShape::RelativePlacement (sub_820BA408)", Original<__imp__sub_820BA408>,
       NativeRelativePlacement, kRelative, 1, false},
      {"BoxShape::RelativeCenter (sub_820BA5D8)", Original<__imp__sub_820BA5D8>,
       NativeRelativeCenter, kRelative, 1, false},
      {"BoxBox::Outcode (sub_820BD0C8)", Original<__imp__sub_820BD0C8>, NativeOutcode, kBlockR3,
       1, false},
      {"BoxBox::BlockCorners (sub_820BC978)", Original<__imp__sub_820BC978>, NativeBlockCorners,
       kBlockR3, 1, false},
      {"BoxBox::MoveCorners (sub_820BCCE8)", Original<__imp__sub_820BCCE8>, NativeMoveCorners,
       kMoveCorners, 1, false},
      {"BoxBox::FaceSeparates (sub_820BB300)", Original<__imp__sub_820BB300>,
       NativeFaceSeparates, kFaces, 6, false},
      {"BoxBox::Separated (sub_820BD280)", Original<__imp__sub_820BD280>, NativeSeparated,
       kQueryCache, 1, false},
      {"BoxBox::SeparatedAfterMove (sub_820BD578)", Original<__imp__sub_820BD578>,
       NativeSeparatedAfterMove, kQuerySetUp, 1, false},
      {"Query::Separated (sub_820BD508)", Original<__imp__sub_820BD508>, NativeQuerySeparated,
       kQueryCache, 1, false},
      {"Query::SeparatedAfterMove (sub_820BD6E0)", Original<__imp__sub_820BD6E0>,
       NativeQuerySeparatedAfterMove, kQuerySetUp, 1, false},
      {"CollisionPair::NextCheckTime (sub_820BBB20)", Original<__imp__sub_820BBB20>,
       NativeNextCheckTime, kPairR3, 1, true},
      {"BoxBox::SwapForImpact (sub_820BE490)", Original<__imp__sub_820BE490>,
       NativeSwapForImpact, kQuerySetUp, 1, false},
      {"BoxBox::SwapForPushOut (sub_820BEBF0)", Original<__imp__sub_820BEBF0>,
       NativeSwapForPushOut, kQuerySetUp, 1, false},
      {"BoxBox::CornerImpacts (sub_820BECA0)", Original<__imp__sub_820BECA0>,
       NativeCornerImpacts, kQuerySetUp, 1, false},
      {"BoxBox::FindImpact (sub_820BF038)", Original<__imp__sub_820BF038>, NativeFindImpact,
       kQuerySetUp, 1, false},
      {"Query::FindImpact (sub_820BCE80)", Original<__imp__sub_820BCE80>, NativeQueryFindImpact,
       kQuerySetUp, 1, false},
      {"BoxBox::EdgesCrossing (sub_820BF080)", Original<__imp__sub_820BF080>,
       NativeEdgesCrossing, kEdges, 1, false, false, true},
      {"BoxBox::PushOutEdges (sub_820BF320)", Original<__imp__sub_820BF320>,
       NativePushOutEdges, kQuerySetUp, 1, false, false, true},
      {"BoxBox::PushOutCorners (sub_820BE150)", Original<__imp__sub_820BE150>,
       NativePushOutCorners, kQuerySetUp, 1, false, false, true},
      {"BoxBox::FindPushOut (sub_820BFB28)", Original<__imp__sub_820BFB28>, NativeFindPushOut,
       kQuerySetUp, 1, false, false, true},
      {"Query::FindPushOut (sub_820BCDD0)", Original<__imp__sub_820BCDD0>,
       NativeQueryFindPushOut, kQueryVector, 1, false, false, true},
      {"sub_822BE2C8 (the word +8)", Original<__imp__sub_822BE2C8>, NativeWordAt8, kRecordR3, 1,
       false},
      {"CollisionShape::CurrentCenter (sub_820BAB20)", Original<__imp__sub_820BAB20>,
       NativeCurrentCenter, kRecordR3, 1, false},
      {"CollisionShape::PreviousCenter (sub_820BAB30)", Original<__imp__sub_820BAB30>,
       NativePreviousCenter, kRecordR3, 1, false},
      {"CollisionShape::Type (sub_820BACB8)", Original<__imp__sub_820BACB8>, NativeType,
       kRecordR3, 1, false},
      {"CollisionShape::Previous (sub_820BADF8)", Original<__imp__sub_820BADF8>, NativePrevious,
       kRecordR3, 1, false},
      {"CollisionShape::Current (sub_820BAE38)", Original<__imp__sub_820BAE38>, NativeCurrent,
       kRecordR3, 1, false},
      {"Query::Query (sub_820BD1C8)", Original<__imp__sub_820BD1C8>, NativeQueryConstruct,
       kQueryR3, 1, false},
      {"Query::SetShapes (sub_820BC848)", Original<__imp__sub_820BC848>, NativeSetShapes,
       kQueryShapes, 1, false},
      {"Contact::Set (sub_820BC908)", Original<__imp__sub_820BC908>, NativeContactSet,
       kContactSet, 1, false},
      {"Contact::Flip (sub_820BC858)", Original<__imp__sub_820BC858>, NativeContactFlip,
       kContactR3, 1, true},
  };
  enum Group { kNumbers, kOneNaN, kNaNs };
  const char* const kGroupNames[] = {"", ", one NaN",
                                     ", NaNs and infinities anywhere (expected to differ)"};
  constexpr uint32_t kNaNWords[] = {0x7FC00000, 0x7F800001, 0xFFC00001, 0xFF800001};
  const std::vector<uint8_t> saved_direction = Read(h.base, kFaceDirection, 16);
  for (const auto& f : functions) {
    for (Group group : {kNumbers, kOneNaN, kNaNs}) {
      Results results(std::string(f.name) + kGroupNames[group]);
      // Where one NaN put in reaches the motion's row through the current
      // center and its negation, the two meet in Vec3::Sum: which comes
      // out depends on the operand order the compiler gave it, in the
      // translated vector library as in ours (its harness's "two or more
      // NaN inputs" groups). Those cases: the differing words all in the
      // row (+96 to +108), NaNs in both runs with only their signs apart.
      Results negated(std::string(f.name) +
                      ", one NaN meeting its own negation (expected to differ)");
      for (uint64_t seed = 0; seed < 200; seed++) {
        Random pick = Seeded(seed * 11 + 5 * uint64_t(group) + 1);
        auto word = [group](Random& r) {
          return group == kNaNs ? FloatWord(r) : NumberWord(r);
        };
        const double f1 = double(std::bit_cast<float>(word(pick)));
        const double f2 = double(std::bit_cast<float>(word(pick)));
        const uint32_t index = pick.Below(f.count);
        const uint32_t cache_word = uint32_t(pick.Next());
        // The cache: 0, one of A's faces' bits, one of B's, or anything.
        const uint32_t cache_kind = pick.Below(4);
        const uint32_t cache = cache_kind == 0   ? 0
                               : cache_kind == 1 ? (cache_word & 0x3F)
                               : cache_kind == 2 ? (cache_word & 0x3F00)
                                                 : cache_word;
        // The pair's objects: +272 and +276 clear most of the time; the
        // bodies fixed (byte +0 set) one time in four each.
        const bool objects_clear = pick.Below(4) != 0;
        const bool fixed_a = pick.Below(4) == 0, fixed_b = pick.Below(4) == 0;
        const bool realistic = pick.Below(2) == 0;
        // The one NaN: in the region, or (one case in ten) in f1.
        const uint32_t nan_word = kNaNWords[pick.Below(std::size(kNaNWords))];
        const uint32_t nan_at = pick.Below(kRegionBytes / 4 + kRegionBytes / 40);
        std::vector<uint8_t> result[2];
        for (int v = 0; v < 2; v++) {
          Random words = Seeded(seed * 13 + 7 * uint64_t(group) + 3);
          for (uint32_t i = 0; i < kRegionBytes; i += 4) {
            guest::StoreU32(h.base, region + i, word(words));
          }
          for (uint32_t i = 0; i < 16; i += 4) {
            guest::StoreU32(h.base, kFaceDirection + i, word(words));
          }
          if (group == kOneNaN && nan_at < kRegionBytes / 4) {
            guest::StoreU32(h.base, region + 4 * nan_at, nan_word);
          }
          // The boxes' vtables, and the record's copies.
          for (uint32_t box : {kBoxD, kBoxE, kBoxF}) {
            guest::StoreU32(h.base, region + box, co::kBoxVtable);
          }
          guest::StoreU32(h.base, region + kRecordB + 0, region + kBoxD);
          guest::StoreU32(h.base, region + kRecordB + 4, region + kBoxE);
          guest::StoreU32(h.base, region + kRecordB + 8, region + kBoxF);
          if (f.args == kQueryCache || f.args == kQuerySetUp || f.args == kEdges ||
              f.args == kQueryVector) {
            guest::StoreU32(h.base, region + kQuery + 0, region + kRecord);
            guest::StoreU32(h.base, region + kQuery + 4, region + kRecordB);
            for (uint32_t k = 0; k < 4; k++) {
              guest::StoreU32(h.base, region + kQuery + 8 + 4 * k, region + kBlock + 336 * k);
            }
            guest::StoreU32(h.base, region + kCache, cache);
            // Blocks as the game's look some of the time: the corners'
            // outcodes 6 bits, their coordinates within 2.0, the impact
            // time 2.0 (so corners cross the unit box's faces).
            if (realistic) {
              Random r = Seeded(seed * 17 + 3 * uint64_t(group) + 5);
              for (uint32_t k = 0; k < 4; k++) {
                const uint32_t block = region + kBlock + 336 * k;
                for (uint32_t corner = 0; corner < 8; corner++) {
                  const uint32_t at = block + 80 + 32 * corner;
                  for (uint32_t axis = 0; axis < 12; axis += 4) {
                    guest::StoreFloat(h.base, at + axis,
                                      float(int32_t(r.Below(4001)) - 2000) / 1000.0f);
                  }
                  uint32_t outcode = r.Below(64);
                  if (f.push_out) {
                    outcode = 0;
                    for (uint32_t axis = 0; axis < 3; axis++) {
                      const float c = guest::LoadFloat(h.base, at + 4 * axis);
                      outcode |= (c > 1.0f ? 1u : 0u) << (2 * axis);
                      outcode |= (c < -1.0f ? 2u : 0u) << (2 * axis);
                    }
                  }
                  guest::StoreU32(h.base, at + 16, outcode);
                }
                const uint32_t block_outcode = r.Below(64);
                guest::StoreU32(h.base, block + 64,
                                f.push_out && (block_outcode & 3) != 0 ? 0 : block_outcode);
              }
              guest::StoreU32(h.base, region + kQuery + 84, 0x40000000);
              if (f.push_out && r.Below(2) == 0) {
                guest::StoreU32(h.base, region + kQuery + 80, 0);
              }
            }
          }
          if (f.args == kPairR3) {
            const uint32_t objects[] = {kObjectA, kObjectB}, bodies[] = {kBodyA, kBodyB},
                           shapes[] = {kBoxA, kBoxD}, copies[] = {kBoxC, kBoxF};
            const bool fixed[] = {fixed_a, fixed_b};
            for (int o = 0; o < 2; o++) {
              const uint32_t object = region + objects[o];
              guest::StoreU32(h.base, region + kPair + 16 + 4 * o, object);
              guest::StoreU32(h.base, object + 16, region + bodies[o]);
              guest::StoreU32(h.base, object + 20, region + shapes[o]);
              guest::StoreU32(h.base, object + 32 + 8, region + copies[o]);
              if (objects_clear) {
                guest::StoreU32(h.base, object + 272, 0);
                guest::StoreU32(h.base, object + 276, 0);
              }
              guest::StoreU8(h.base, region + bodies[o], fixed[o] ? 1 : 0);
            }
          }
          if (f.args == kObjectVector) {
            const uint32_t object = region + kObjectA;
            guest::StoreU32(h.base, object + 16, region + kBodyA);
            guest::StoreU32(h.base, object + 32 + 0, region + kBoxA);
            guest::StoreU32(h.base, object + 32 + 4, region + kBoxB);
            guest::StoreU32(h.base, object + 32 + 8, region + kBoxC);
          }
          for (uint32_t box : {kBoxA, kBoxB, kBoxC}) {
            guest::StoreU32(h.base, region + box, co::kBoxVtable);
          }
          guest::StoreU32(h.base, region + kRecord + 0, region + kBoxA);
          guest::StoreU32(h.base, region + kRecord + 4, region + kBoxB);
          guest::StoreU32(h.base, region + kRecord + 8, region + kBoxC);
          PPCContext t = CallContext(h);
          t.fpscr.disableFlushModeUnconditional();
          switch (f.args) {
            case kNone:
              break;
            case kBoxIndex:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = index;
              break;
            case kIndexR4:
              t.r4.u64 = index;
              break;
            case kIndexR3:
              t.r3.u64 = index;
              break;
            case kCacheSet:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = index;
              t.r5.u64 = cache_word;
              break;
            case kTwoBoxes:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = region + kBoxB;
              break;
            case kBoxR3:
              t.r3.u64 = region + kBoxA;
              break;
            case kBoxPoint:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = index;
              t.r5.u64 = region + kVector;
              break;
            case kBoxVector:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = region + kVector;
              break;
            case kRecordR3:
              t.r3.u64 = region + kRecord;
              break;
            case kQueryR3:
              t.r3.u64 = region + kQuery;
              break;
            case kQueryShapes:
              t.r3.u64 = region + kQuery;
              t.r4.u64 = region + kRecord;
              t.r5.u64 = region + kBoxA;
              break;
            case kContactSet:
              t.r3.u64 = region + kContact;
              t.r4.u64 = region + kVector;
              t.r5.u64 = region + kBoxB + 16;
              break;
            case kContactR3:
              t.r3.u64 = region + kContact;
              break;
            case kCorners:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = region + kVector;
              t.r5.u64 = region + kVector2;
              break;
            case kCopy:
              t.r3.u64 = region + kBoxB;
              t.r4.u64 = region + kBoxA;
              break;
            case kPlace:
              t.r3.u64 = region + kBoxB;
              t.r4.u64 = region + kMatrix;
              t.r5.u64 = region + kBoxA;
              break;
            case kRecordMatrix:
              t.r3.u64 = region + kRecord;
              t.r4.u64 = region + kMatrix;
              if (index == 0) {
                guest::StoreU32(h.base, region + kRecord + 112, 0);
              }
              break;
            case kRelative:
              t.r3.u64 = region + kBoxA;
              t.r4.u64 = region + kBlock;
              t.r5.u64 = region + kBoxB;
              break;
            case kBlockR3:
              t.r3.u64 = region + kBlock;
              break;
            case kMoveCorners:
              t.r3.u64 = region + kBlock + 80;
              t.r4.u64 = region + kVector;
              break;
            case kFaces:
              t.r3.u64 = region + kBoxA + 304 + 32 * index;
              t.r4.u64 = region + kBoxC + 304 + 32 * index;
              t.r5.u64 = region + kBoxB;
              t.r6.u64 = region + kBoxE;
              break;
            case kQueryCache:
              t.r3.u64 = region + kQuery;
              t.r4.u64 = region + kCache;
              break;
            case kQuerySetUp:
              t.r3.u64 = region + kQuery;
              break;
            case kPairR3:
              t.r3.u64 = region + kPair;
              break;
            case kRecordVector:
              t.r3.u64 = region + kRecord;
              t.r4.u64 = region + kVector;
              break;
            case kEdges:
              t.r3.u64 = region + kVector;
              t.r4.u64 = region + kBlock;
              break;
            case kQueryVector:
              t.r3.u64 = region + kQuery;
              t.r4.u64 = region + kVector;
              break;
            case kObjectVector:
              t.r3.u64 = region + kObjectA;
              t.r4.u64 = region + kVector;
              break;
          }
          t.f1.f64 = group == kOneNaN && nan_at >= kRegionBytes / 4
                         ? double(std::bit_cast<float>(nan_word))
                         : f1;
          t.f2.f64 = f2;
          (v == 0 ? f.recompiled : f.native)(t, h.base);
          result[v] = Read(h.base, region, kRegionBytes);
          const std::vector<uint8_t> direction = Read(h.base, kFaceDirection, 16);
          result[v].insert(result[v].end(), direction.begin(), direction.end());
          for (int i = 0; i < 4; i++) {
            result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
          }
          for (int i = 0; f.compare_f1 && i < 8; i++) {
            result[v].push_back(uint8_t(t.f1.u64 >> (56 - 8 * i)));
          }
        }
        auto word_at = [&](int v, size_t at) {
          uint32_t w = 0;
          for (int i = 0; i < 4; i++) {
            w = (w << 8) | result[v][at + i];
          }
          return w;
        };
        bool meets_negation = f.motion_row && group == kOneNaN && result[0] != result[1];
        const uint32_t record = f.args == kObjectVector ? kObjectA + 32 : kRecord;
        for (size_t at = 0; meets_negation && at + 4 <= result[0].size(); at += 4) {
          const uint32_t a = word_at(0, at), b = word_at(1, at);
          if (a == b) {
            continue;
          }
          meets_negation = at >= record + 96 && at < record + 112 &&
                           std::isnan(std::bit_cast<float>(a)) && (a ^ b) == 0x80000000u;
        }
        (meets_negation ? negated : results)
            .Check(result[0], result[1],
                   fmt::format("seed {} index {} f1 {:016X}", seed, index,
                               std::bit_cast<uint64_t>(f1)));
      }
      results.Report();
      if (f.motion_row && group == kOneNaN) {
        negated.Report();
      }
    }
  }
  Write(h.base, kFaceDirection, saved_direction.data(), 16);
  LifetimeCases(h, region);
}

}  // namespace harness
