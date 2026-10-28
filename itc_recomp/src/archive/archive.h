// The archive streaming's functions as plain C++ (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/archive.md and,
// for the older rows, docs/functions/files-and-saves.md): the TOC lookups,
// the request queue's list and slots, the loose files' table, a request's
// finish and Ucl::Unpack. The archive's globals are guest memory shared with
// the recompiled code (the queueing, the picker, the loader thread), so these
// work on guest memory. Adapters: src/hooks/archive.cpp. The functions that
// wait on, signal or read the archive's four semaphores stay recompiled.

#pragma once

#include "c_runtime/exit.h"

#include <cstdint>
#include <optional>

namespace archive {

// The first id of BUILD_CRS.DAT (its own table, id - 23011).
constexpr uint32_t kFirstCrsId = 23011;
// The TOCs' pointers: BUILD.TOC, BUILD_CRS.TOC.
constexpr uint32_t kBuildToc = 0x826A5120;
constexpr uint32_t kCrsToc = 0x826A5128;

// Archive::Toc (sub_8209D868): the TOC an asset id is in, [0x826A5120] below
// 23011, else [0x826A5128].
uint32_t Toc(uint8_t* base, uint32_t id);

// Archive::AssetSize (sub_820A0678): the asset's TOC entry (index id + 1, or
// id - 23011 + 1 in BUILD_CRS, 16 bytes each) +8, the unpacked size, or +4,
// the stored size, when that's 0.
uint32_t AssetSize(uint8_t* base, uint32_t id);

// --- The request queue's list and slots ---

// The request queue (a 20-byte List: +0 the vtable, +4 whether it deletes
// its nodes, +8 the count, +12 the head, +16 the tail; nodes linked by +4).
constexpr uint32_t kQueue = 0x826BAB08;
// The 960 request slots (92 bytes each, ListNodes: +0 the vtable, +4 the
// link, +8 the serial or -1 when free).
constexpr uint32_t kSlots = 0x826A5168;
constexpr uint32_t kSlotCount = 960;
constexpr uint32_t kSlotBytes = 92;
// The vtables the static set-ups store.
constexpr uint32_t kListVtable = 0x820128C8;
constexpr uint32_t kQueueVtable = 0x82001DD4;
constexpr uint32_t kNodeVtable = 0x82010B5C;
constexpr uint32_t kSlotVtable = 0x82001DD8;
// The clean-ups the static set-ups register (atexit).
constexpr uint32_t kQueueCleanUp = 0x825DACB8;
constexpr uint32_t kSlotsCleanUp = 0x825DAD38;

// List::List (sub_82294B80): the vtable 0x820128C8, +4 = `deletes`, the
// count, head and tail 0.
void ListConstruct(uint8_t* base, uint32_t list, uint32_t deletes);
// List::Append (sub_82294BA8): `node` linked after the tail (or as the head
// when there's no tail), its link 0, the tail it, the count up by one; 1, or
// 0 for no node.
uint32_t ListAppend(uint8_t* base, uint32_t list, uint32_t node);
// List::Remove (sub_82294C00): `node` found from the head by the links and
// unlinked (the previous node's link, the head and the tail fixed), the count
// down by one; 1, or 0 when it isn't there (or is 0).
uint32_t ListRemove(uint8_t* base, uint32_t list, uint32_t node);
// ListNode::ListNode (sub_82294B68): the vtable 0x82010B5C, the link `link`.
void NodeConstruct(uint8_t* base, uint32_t node, uint32_t link);

// The slot lookup (sub_8209D818): the first of the 960 slots whose serial
// (+8) is `serial` (-1 for a free one), or 0.
uint32_t FindSlot(uint8_t* base, uint32_t serial);

// The static set-ups (_cinit's table, once at start-up); each returns
// atexit's r3 (c_runtime::AtexitFrom). sub_825D9160: the queue made
// (List::List(0), then its vtable 0x82001DD4), its clean-up 0x825DACB8
// registered. sub_825D91B0: the 960 slots made (ListNode::ListNode(0), then
// the vtable 0x82001DD8), their clean-up 0x825DAD38 registered.
int32_t QueueStaticInit(uint8_t* base, const c_runtime::AtexitCall& call);
int32_t SlotsStaticInit(uint8_t* base, const c_runtime::AtexitCall& call);

// --- The loose files ---

// The loose files' tables: (id, path) pairs of 8 bytes, the first for the
// walk's first 23,011 steps, the second after (as the original picks them by
// the step, not the id); a path of 0 ends them.
constexpr uint32_t kLooseFiles = 0x82600AC8;
constexpr uint32_t kCrsLooseFiles = 0x82678240;

// Archive::FindLooseFile (sub_8209D8D8): the path of the first entry whose id
// is `id`, or 0 at the first entry with no path.
uint32_t FindLooseFile(uint8_t* base, uint32_t id);

// --- The loader thread's pieces ---

// The staging ring's half a compressed request read into (put back by the
// finish), and the serial of the request whose callback runs last.
constexpr uint32_t kStagingInUse = 0x826A511C;
constexpr uint32_t kCallbackSerial = 0x82600AA4;

// A request's finish's calls into the translated code: the request's
// callback (+48, Archive::Load's function pointer: about 20 of the loads'
// functions, some ours), with r3 to r6; and VirtualFree where the native one
// can't (hooks::VirtualFree's nullopt). The adapter makes them.
struct FinishCalls {
  void (*callback)(void* user, uint32_t function, uint32_t serial, uint32_t data, uint32_t size,
                   uint32_t context) = nullptr;
  void (*guest_free)(void* user, uint32_t address, uint32_t size, uint32_t type) = nullptr;
  void* user = nullptr;
};

// The request finish (sub_8209D688), on the loader thread: with flag 0x100
// and not 0x8 (+12), the staging half in use (0x826A511C) = the data (+76),
// and the data = the destination (+88); with flag 0x2000, the serial (+8)
// stored at 0x82600AA4 and the callback (+48) called as callback(serial,
// data, size +32, context +44); with flag 0x2 and not 0x1000,
// VirtualFree(data, 0, MEM_DECOMMIT). The flags are read again before each
// step, as the original does (the callback may change them). Returns
// VirtualFree's r3 when the native one freed (the original's r3 then), else
// nothing (r3 is the callback's or guest_free's, or the request).
std::optional<uint32_t> FinishRequest(uint8_t* base, uint32_t request, const FinishCalls& calls);

// LoaderFunctor::Invoke (sub_8209DD80): the function the functor calls (its
// +4), which the adapter tail-calls with the argument as r3. Its one functor
// (0x82600B68) holds the loader thread (0x8209E118).
uint32_t LoaderFunctorTarget(uint8_t* base, uint32_t functor);

// --- Ucl::Unpack ---

// One UCL block's unpacking (sub_822947F8, UCL's NRV2B decompressor with
// 32-bit little-endian bit words, the fast one: no bounds checks): `src_len`
// bytes at `src` into `dst`. r3 is 0 when the input was used up exactly,
// -201 when it ran past `src_len`, -205 when it stopped short; `out` the
// bytes written (what the original stores at its r6), `in` the bytes read.
struct UclBlockResult {
  int32_t r3;
  uint32_t out;
  uint32_t in;
};
UclBlockResult UclBlock(uint8_t* base, uint32_t src, uint32_t src_len, uint32_t dst);

// Ucl::Unpack's call into the translated code: memcpy (sub_825A7410) for a
// stored block where the native copy can't (native::Buffers_Memcpy's false).
struct UclCalls {
  void (*guest_memcpy)(void* user, uint32_t destination, uint32_t source, uint32_t size) =
      nullptr;
  void* user = nullptr;
};

// Ucl::Unpack (sub_82294610): a UCL stream (`uclpack`'s layout: after an
// 18-byte header, blocks of a big-endian unpacked size, a big-endian stored
// size and the data, until an unpacked size of 0) unpacked into `dst`: a
// block stored smaller than its size unpacked (UclBlock, its result not
// read), else copied as is (memcpy, its copies into the physical views
// recorded as the memcpy override's). Stops with 0 when the output would
// pass `expected` (an unpacked block is checked after it's written, a copied
// one before); else 1 when the output's size is `expected`.
uint32_t UclUnpack(uint8_t* base, uint32_t dst, uint32_t stream, uint32_t expected,
                   const UclCalls& calls);

}  // namespace archive
