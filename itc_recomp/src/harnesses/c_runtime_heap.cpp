// The C runtime's heap over the allocation layer (src/memory/c_runtime_heap.h)
// against the game's own functions (__imp__), both over the layer: the two
// hand out different addresses, so each case compares what can be seen of a
// block without its address (that one came back, its alignment, its size in
// the layer, the contents cleared or kept), r3 where the function returns
// something, and errno. Each side frees what it allocated, with its own free.
// The native side runs the plain C++ as the adapters do, falling back to the
// game's function for the error paths. The failure cases ask for sizes the
// layer refuses (over 0x7FFFFFFF), and run only while _newmode
// (0x8297119C) is 0, so no new handler is called; operator new's failure
// (std::bad_alloc) and _invalid_parameter's paths aren't run.

#include "harness.h"

#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

namespace harness {

namespace {

namespace crt = memory::crt;

constexpr uint32_t kNewMode = 0x8297119C;

// The native sides: the plain C++ with the adapters' fallbacks.
void NativeGetProcessHeap(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = crt::GetProcessHeap(base);
}
void NativeMalloc(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::Malloc(base, ctx.r3.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825A9A90(ctx, base);
}
void NativeFree(PPCContext& ctx, uint8_t* base) {
  if (!crt::Free(ctx.r3.u32)) {
    __imp__sub_825A9B58(ctx, base);
  }
}
void NativeCallocImpl(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::CallocImpl(ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825AC248(ctx, base);
}
void NativeCalloc(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::Calloc(ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825AC320(ctx, base);
}
void NativeRealloc(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::Realloc(base, ctx.r3.u32, ctx.r4.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825AEBC8(ctx, base);
}
void NativeMsize(PPCContext& ctx, uint8_t* base) {
  if (auto s = crt::Msize(ctx.r3.u32)) {
    ctx.r3.u64 = *s;
    return;
  }
  __imp__sub_825AED30(ctx, base);
}
void NativeLocalAlloc(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = crt::LocalAlloc(ctx.r3.u32, ctx.r4.u32);
}
void NativeLocalFree(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = crt::LocalFree(ctx.r3.u32);
}
void NativeXMemAlloc(PPCContext& ctx, uint8_t* base) {
  ctx.r3.u64 = crt::XMemAlloc(base, ctx.r3.u32, ctx.r4.u32);
}
void NativeXMemFree(PPCContext& ctx, uint8_t*) {
  ctx.r3.u64 = crt::XMemFree(ctx.r3.u32, ctx.r4.u32);
}
void NativeOperatorNew(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::OperatorNew(base, ctx.r3.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825A6880(ctx, base);
}
void NativeOperatorNewArray(PPCContext& ctx, uint8_t* base) {
  if (auto b = crt::OperatorNewArray(base, ctx.r3.u32)) {
    ctx.r3.u64 = *b;
    return;
  }
  __imp__sub_825A6940(ctx, base);
}
void NativeOperatorDelete(PPCContext& ctx, uint8_t* base) {
  if (!crt::OperatorDelete(ctx.r3.u32)) {
    __imp__sub_825A70E8(ctx, base);
  }
}
void NativeOperatorDeleteArray(PPCContext& ctx, uint8_t* base) {
  if (!crt::OperatorDeleteArray(ctx.r3.u32)) {
    __imp__sub_825A71C0(ctx, base);
  }
}

// One function's two sides.
struct Pair {
  GuestFunction recompiled;
  GuestFunction native;
  GuestFunction operator[](int side) const { return side == 0 ? recompiled : native; }
};

const Pair kMalloc{__imp__sub_825A9A90, NativeMalloc};
const Pair kFree{__imp__sub_825A9B58, NativeFree};
const Pair kCallocImpl{__imp__sub_825AC248, NativeCallocImpl};
const Pair kCalloc{__imp__sub_825AC320, NativeCalloc};
const Pair kRealloc{__imp__sub_825AEBC8, NativeRealloc};
const Pair kMsize{__imp__sub_825AED30, NativeMsize};
const Pair kLocalAlloc{__imp__sub_82297F48, NativeLocalAlloc};
const Pair kLocalFree{__imp__sub_82297F90, NativeLocalFree};
const Pair kXMemAlloc{__imp__sub_822976F8, NativeXMemAlloc};
const Pair kXMemFree{__imp__sub_82297790, NativeXMemFree};
const Pair kNew{__imp__sub_825A6880, NativeOperatorNew};
const Pair kNewArray{__imp__sub_825A6940, NativeOperatorNewArray};
const Pair kDelete{__imp__sub_825A70E8, NativeOperatorDelete};
const Pair kDeleteArray{__imp__sub_825A71C0, NativeOperatorDeleteArray};

// Calls `f` with r3, r4, ... set from `args`; returns r3.
uint32_t Call(Context& h, GuestFunction f, std::initializer_list<uint32_t> args) {
  PPCContext t = CallContext(h);
  PPCRegister* registers[] = {&t.r3, &t.r4, &t.r5, &t.r6, &t.r7};
  int i = 0;
  for (uint32_t a : args) {
    registers[i++]->u64 = a;
  }
  f(t, h.base);
  return t.r3.u32;
}

void Append(std::vector<uint8_t>& out, uint32_t value) {
  for (int i = 0; i < 4; i++) {
    out.push_back(uint8_t(value >> (24 - 8 * i)));
  }
}

bool AllZero(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    if (guest::LoadU8(base, address + i) != 0) {
      return false;
    }
  }
  return true;
}

uint8_t Pattern(uint32_t i) {
  return uint8_t(i * 5 + 3);
}

void WritePattern(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, Pattern(i));
  }
}

bool HasPattern(uint8_t* base, uint32_t address, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    if (guest::LoadU8(base, address + i) != Pattern(i)) {
      return false;
    }
  }
  return true;
}

// errno (through the game's _errno, sub_825A81E0).
uint32_t ErrnoAddress(Context& h) {
  return Call(h, sub_825A81E0, {});
}

// Runs `body(side, out)` for the recompiled side (0), then the native one
// (1), errno set to 0 before each and appended after; compares.
template <typename Body>
void Case(Context& h, Results& results, const std::string& what, Body body) {
  std::vector<uint8_t> result[2];
  for (int side = 0; side < 2; side++) {
    const uint32_t error = ErrnoAddress(h);
    guest::StoreU32(h.base, error, 0);
    body(side, result[side]);
    Append(result[side], guest::LoadU32(h.base, error));
  }
  results.Check(result[0], result[1], what);
}

// What can be seen of a heap block: that it came back, its alignment and
// its size in the layer.
void AppendBlock(std::vector<uint8_t>& out, uint32_t p) {
  Append(out, p != 0);
  if (p != 0) {
    Append(out, p % 16);
    Append(out, memory::Size(p));
  }
}

void MallocCases(Context& h, bool failures) {
  Results results("c_runtime_heap: GetProcessHeap, malloc, free, _msize");
  Case(h, results, "GetProcessHeap", [&](int side, std::vector<uint8_t>& out) {
    Append(out, Call(h, side == 0 ? __imp__sub_8229B670 : NativeGetProcessHeap, {}));
  });
  std::vector<uint32_t> sizes = {0, 1, 7, 16, 33, 100, 4096, 5000, 70000};
  if (failures) {
    sizes.push_back(0x80000000);
    sizes.push_back(0xFFFFF000);
  }
  for (uint32_t size : sizes) {
    Case(h, results, fmt::format("malloc {}", size), [&](int side, std::vector<uint8_t>& out) {
      const uint32_t p = Call(h, kMalloc[side], {size});
      AppendBlock(out, p);
      if (p == 0) {
        return;
      }
      Append(out, Call(h, kMsize[side], {p}));
      WritePattern(h.base, p, size);
      Append(out, HasPattern(h.base, p, size));
      Call(h, kFree[side], {p});
    });
  }
  Case(h, results, "free of a null block", [&](int side, std::vector<uint8_t>&) {
    Call(h, kFree[side], {0});
  });
  results.Report();
}

void CallocCases(Context& h, bool failures) {
  Results results("c_runtime_heap: calloc, _calloc_impl");
  std::vector<std::pair<uint32_t, uint32_t>> counts = {
      {0, 0}, {0, 8}, {1, 0}, {1, 1}, {3, 7}, {100, 12}, {1000, 70}, {16, 4096},
  };
  if (failures) {
    counts.push_back({1, 0x80000000});
    counts.push_back({0x80000000, 1});
  }
  for (auto [count, size] : counts) {
    for (bool impl : {false, true}) {
      Case(h, results, fmt::format("{} {} x {}", impl ? "_calloc_impl" : "calloc", count, size),
           [&](int side, std::vector<uint8_t>& out) {
             // _calloc_impl's errno goes to a word in the scratch area.
             guest::StoreU32(h.base, h.data, 0);
             const uint32_t p = impl ? Call(h, kCallocImpl[side], {count, size, h.data})
                                     : Call(h, kCalloc[side], {count, size});
             AppendBlock(out, p);
             if (impl) {
               Append(out, guest::LoadU32(h.base, h.data));
             }
             if (p == 0) {
               return;
             }
             const uint32_t bytes = count * size;
             Append(out, AllZero(h.base, p, bytes));
             Call(h, kFree[side], {p});
           });
    }
  }
  results.Report();
}

void ReallocCases(Context& h, bool failures) {
  Results results("c_runtime_heap: realloc");
  std::vector<std::pair<uint32_t, uint32_t>> sizes = {
      {0, 16}, {0, 0}, {16, 0}, {16, 8}, {16, 24}, {100, 100}, {4000, 16}, {24, 70000}, {1, 2},
  };
  if (failures) {
    // Not over 0xFFFFF000: realloc then calls _callnewh whatever _newmode.
    sizes.push_back({16, 0x80000000});
  }
  for (auto [from, to] : sizes) {
    Case(h, results, fmt::format("realloc {} to {}", from, to),
         [&](int side, std::vector<uint8_t>& out) {
           // `from` 0 is a null block.
           const uint32_t p = from == 0 ? 0 : Call(h, kMalloc[side], {from});
           WritePattern(h.base, p, from);
           // Something after the block, so growing may have to move it.
           const uint32_t after = Call(h, kMalloc[side], {8});
           const uint32_t q = Call(h, kRealloc[side], {p, to});
           AppendBlock(out, q);
           if (q != 0) {
             Append(out, HasPattern(h.base, q, std::min(from, to)));
             Call(h, kFree[side], {q});
           } else if (to != 0 && p != 0) {
             // A failed realloc leaves the block.
             Append(out, HasPattern(h.base, p, from));
             Call(h, kFree[side], {p});
           }
           Call(h, kFree[side], {after});
         });
  }
  results.Report();
}

void LocalCases(Context& h) {
  Results results("c_runtime_heap: LocalAlloc, LocalFree, XMemAlloc, XMemFree");
  for (uint32_t size : {0u, 5u, 100u, 70000u}) {
    for (uint32_t flags : {0u, 0x40u}) {
      Case(h, results, fmt::format("LocalAlloc {} flags {:#x}", size, flags),
           [&](int side, std::vector<uint8_t>& out) {
             const uint32_t p = Call(h, kLocalAlloc[side], {flags, size});
             AppendBlock(out, p);
             if (p == 0) {
               return;
             }
             if (flags) {
               Append(out, AllZero(h.base, p, size));
             }
             Append(out, Call(h, kLocalFree[side], {p}));
           });
    }
  }
  Case(h, results, "LocalFree of a null block", [&](int side, std::vector<uint8_t>& out) {
    Append(out, Call(h, kLocalFree[side], {0}));
  });
  for (uint32_t size : {0u, 24u, 1000u}) {
    for (uint32_t attributes : {0u, 0x40000000u, 0x0C000000u}) {
      Case(h, results, fmt::format("XMemAlloc {} attributes {:#x}", size, attributes),
           [&](int side, std::vector<uint8_t>& out) {
             const uint32_t p = Call(h, kXMemAlloc[side], {size, attributes});
             AppendBlock(out, p);
             if (p == 0) {
               return;
             }
             if (attributes & 0x40000000) {
               Append(out, AllZero(h.base, p, size));
             }
             Append(out, Call(h, kXMemFree[side], {p, attributes}));
           });
    }
  }
  for (uint32_t size : {100u, 4096u, 70000u}) {
    for (uint32_t attributes :
         {0x80000000u, 0xC0000000u, 0x84000000u, 0x8C000000u, 0x8D000000u, 0x90000000u,
          0xA0000000u, 0xB0000000u, 0xE4000000u, 0xC8000000u}) {
      Case(h, results, fmt::format("XMemAlloc physical {} attributes {:#x}", size, attributes),
           [&](int side, std::vector<uint8_t>& out) {
             const uint32_t p = Call(h, kXMemAlloc[side], {size, attributes});
             Append(out, p != 0);
             if (p == 0) {
               return;
             }
             // Which physical view (by page size), and the alignment.
             uint32_t field = (attributes >> 24) & 0xF;
             Append(out, p >> 29);
             Append(out, p % (1u << (field == 0 ? 12 : field)));
             if (attributes & 0x40000000) {
               Append(out, AllZero(h.base, p, size));
             }
             // Dirty, so the next case's clearing shows.
             WritePattern(h.base, p, size);
             Append(out, Call(h, kXMemFree[side], {p, attributes}) == p);
           });
    }
  }
  Case(h, results, "XMemFree of a null physical block", [&](int side, std::vector<uint8_t>& out) {
    Append(out, Call(h, kXMemFree[side], {0, 0x80000000}));
  });
  results.Report();
}

void NewCases(Context& h) {
  Results results("c_runtime_heap: operator new, new[], delete, delete[]");
  for (uint32_t size : {0u, 1u, 24u, 1000u, 70000u}) {
    for (bool array : {false, true}) {
      Case(h, results, fmt::format("operator new{} {}", array ? "[]" : "", size),
           [&](int side, std::vector<uint8_t>& out) {
             const uint32_t p = Call(h, (array ? kNewArray : kNew)[side], {size});
             AppendBlock(out, p);
             if (p == 0) {
               return;
             }
             WritePattern(h.base, p, size);
             Append(out, HasPattern(h.base, p, size));
             Call(h, (array ? kDeleteArray : kDelete)[side], {p});
           });
    }
  }
  for (bool array : {false, true}) {
    Case(h, results, fmt::format("operator delete{} of a null block", array ? "[]" : ""),
         [&](int side, std::vector<uint8_t>&) {
           Call(h, (array ? kDeleteArray : kDelete)[side], {0});
         });
  }
  results.Report();
}

}  // namespace

void CRuntimeHeap(Context& h) {
  if (!hooks::NativeHeapActive()) {
    // The recompiled side would use the game's own heap.
    ITC_LOG_WARN("harness: c_runtime_heap needs --native_heap; not run");
    return;
  }
  const bool failures = guest::LoadU32(h.base, kNewMode) == 0;
  MallocCases(h, failures);
  CallocCases(h, failures);
  ReallocCases(h, failures);
  LocalCases(h);
  NewCases(h);
}

}  // namespace harness
