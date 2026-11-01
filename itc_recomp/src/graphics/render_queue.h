// The render queue's methods as plain C++ (docs/functions/device-shaders.md,
// "The render queue"): the queue at scene + 64 (vtable 0x8206CB8C), its
// state (+4: +0 the replay index, +4 the primitive drawer, +48 to +64 the
// render targets and depth surface Init keeps, +68 the release list, +84
// the submission sequence) and its two pass lists (+8, +12). Every read and
// write is the original's, in its order. Calls to code still recompiled (the
// drawer's Init, the render command handlers of the tables 0x8265EDE0 and
// 0x8265ED48, the release list's objects, a vtable slot that isn't the
// queue's own) are guest calls through `call`, from frames made where the
// originals make theirs. Adapters: src/hooks/render_queue.cpp
// (--native_render_queue).

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace graphics::render_queue {

constexpr uint32_t kVtable = 0x8206CB8C;

// What a guest call leaves: r3 in full and f1.
struct CallResult {
  uint64_t r3;
  double f1;
};
// A guest call to `function` with r1 = `sp`, lr = `return_address`, and
// the first `count` of r3, r4, r5 set (the others as they are).
using GuestCall = std::function<CallResult(uint32_t function, uint32_t sp, uint32_t return_address,
                                           int count, uint64_t r3, uint64_t r4, uint64_t r5)>;

// RenderQueue::Flip (sub_8258E5E0, slot 6): the state's +0 (the replay
// index) = 1 when it was 0, else 0.
void Flip(uint8_t* base, uint32_t queue);
// RenderQueue::Recording (sub_8258E5F8, slot 7): the pass list being
// recorded, +12 when the replay index is 0, else +8.
uint32_t Recording(uint8_t* base, uint32_t queue);
// RenderQueue::Replaying (sub_8258E618, slot 8): the word at +8 + 4 x the
// replay index.
uint32_t Replaying(uint8_t* base, uint32_t queue);
// QueueEntry::Sequence (sub_8258E5C0): the float at the entry's +8.
float Sequence(uint8_t* base, uint32_t entry);

// RenderQueue::ClearRecording (sub_8258ED78, slot 2), from its caller's
// stack pointer `sp`: the list the queue's slot 7 returns emptied
// (PassList::Clear), then the state's +84 = 0.0 (0x82000A34). Returns r3
// as the original leaves it (the list).
uint32_t ClearRecording(uint8_t* base, uint32_t queue, uint32_t sp, const GuestCall& call);

// RenderQueue::Init (sub_8258ED00, slot 0): the drawer's (state + 4) slot
// 0, PrimitiveDrawer::Init, with r4 = the device; state +48 + 4 x i =
// Device::GetRenderTarget(device, i) for i 0 to 3, +64 =
// GetDepthStencilSurface(device). Returns r3 (the depth surface).
uint32_t Init(uint8_t* base, uint32_t queue, uint32_t device, uint32_t sp, const GuestCall& call);

// PassList::Add (sub_8258EB88), a leaf from its caller's frame `sp`:
// `chunk_key` = {the chunk, the key's bits} and `sequence` = {the
// sequence's bits, a word PassList::Add doesn't read} stored at sp + 32 and
// sp + 40 (the caller's frame, as the original), then read back from there.
// 0 when `bucket` isn't below the bucket count (+12) or the items used (+16)
// aren't below the item count (+8); else the item at +20 + 20 x (+16): +0 =
// 0, +4 = the chunk, +8 = the key, +12 = the sequence, appended to the
// bucket (+24 + 16 x bucket: +0 the first when empty, else the last's +0;
// +4 the last; +12 += 1); +16 += 1; returns 1.
uint32_t PassListAdd(uint8_t* base, uint32_t list, uint64_t chunk_key, uint64_t sequence,
                     uint32_t bucket, uint32_t sp);

// RenderQueue::Add (sub_8258EDC8, slot 4): `chunk` (or 0) into `bucket`
// with `key` (f1). nullopt for a bucket above 2,056 or a key above
// 10,000.0 or below -10,000.0 (the warning's path, never seen in play: the
// original runs). Nothing when `chunk` is 0 (r3 the queue). Otherwise, in
// its 144-byte frame below `sp`: +80 = {the chunk, the key as a single},
// +88 = {the sequence (state +84), the frame's word at +92 as it is};
// PassList::Add on the list the queue's slot 7 returns; then the sequence
// += 0.1 (0x82001FBC, single precision). Returns r3 as the original leaves
// it (PassList::Add's answer).
std::optional<uint32_t> Add(uint8_t* base, uint32_t queue, uint32_t chunk, uint32_t bucket,
                            double key, uint32_t sp, const GuestCall& call);

// PtrVector::PushBack (sub_82591540, std::vector<T*>::push_back): r3 = the
// vector (+4 first, +8 last, +12 end), `value` the value's address. With
// room (first not 0, the size below the capacity) the value stored at last
// and +8 moved on; returns the vector (r3 as given). Otherwise, when
// `may_allocate`, PtrVector::Insert from its 96-byte frame below `sp` (+80 =
// last, where the answer goes); returns that address (the original's r3).
// nullopt where the original must run (no `may_allocate`, or
// TextureProvider::InsertN's refusal, models/texture_provider.h).
std::optional<uint32_t> PushBack(uint8_t* base, uint32_t vector, uint32_t value, uint32_t sp,
                                 bool may_allocate);
// PtrVector::Insert (sub_825911C8, std::vector<T*>::insert): the index
// (where - first; 0 for an empty vector), TextureProvider::InsertN(vector,
// where, 1, the value at `value`), then [answer] = first + 4 x the index;
// returns `answer`. nullopt (nothing changed) where InsertN refuses.
std::optional<uint32_t> PtrVectorInsert(uint8_t* base, uint32_t answer, uint32_t vector,
                                        uint32_t where, uint32_t value);

}  // namespace graphics::render_queue
