// The graphics library's predicated tiling
// (docs/functions/predicated-tiling.md): see predicated_tiling.h.

#include "graphics/predicated_tiling.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_state.h"

namespace graphics::predicated_tiling {

namespace cb = command_buffer;

namespace {

// Ends the segment when the write position has passed its soft end, as
// every packet writer does before writing.
void EndSegmentIfFull(uint8_t* base, uint32_t device,
                      const std::function<void(uint32_t)>& end_segment) {
  if (guest::LoadU32(base, device + cb::kWritePosition) >
      guest::LoadU32(base, device + cb::kSoftEnd)) {
    end_segment(device);
  }
}

}  // namespace

uint32_t SetPredication(uint8_t* base, uint32_t device, uint32_t select,
                        const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825BEB80 Device::SetPredication");
  if (select == 0) {
    guest::StoreU32(base, device + kPredicated, 0);
    device_state::UpdatePredicatedTiling(base, device);
    guest::StoreU32(base, device + kBinMask, 0xFFFFFFFFu);
  } else {
    guest::StoreU32(base, device + kBinMask, select);
    guest::StoreU32(base, device + kPredicated, 1);
    guest::StoreU8(base, device + kTilingFlags,
                   uint8_t(guest::LoadU8(base, device + kTilingFlags) & ~0x2));
  }
  guest::StoreU32(base, device + kBinMaskHigh, 0);
  guest::StoreU32(base, device + kPredicationSelect, select);
  guest::StoreU32(base, device + kPredicationHigh, 0);
  EndSegmentIfFull(base, device, calls.end_segment);
  return guest::LoadU32(base, device + cb::kWritePosition);
}

uint32_t BeginTiledSegments(uint8_t* base, uint32_t device, const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B6AD8 Device::BeginTiledSegments");
  calls.end_segment(device);
  cb::SaveSegment(base, device);
  guest::StoreU32(base, device + cb::kTilingEnd, guest::LoadU32(base, device + cb::kTailPosition));
  guest::StoreU32(base, device + cb::kTilingGeneration,
                  guest::LoadU32(base, device + cb::kTailGeneration) + 1);
  for (uint32_t k = 0; k < 20; k += 4) {
    guest::StoreU32(base, device + kSegmentChainCopy + k,
                    guest::LoadU32(base, device + cb::kSegmentChain + k));
  }
  const uint32_t segments = device + cb::kSegmentChain;
  cb::InitChain(base, segments, device);
  cb::InitChain(base, device + kDrawChain, device);
  cb::InitChain(base, device + kThirdChain, device);
  uint32_t header = guest::LoadU32(base, segments + cb::kChainCursor);
  if (header + kHeaderBytes > guest::LoadU32(base, segments + cb::kChainEnd)) {
    header = cb::NewBlock(base, segments);
  }
  guest::StoreU32(base, segments + cb::kChainCursor, header + kHeaderBytes);
  return header + 16;
}

uint32_t EndTiledSegments(uint8_t* base, uint32_t device, uint32_t tiles, uint32_t cpus,
                          const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B6BF8 Device::EndTiledSegments");
  const uint32_t callback = cb::Reserve(base, device, 100, 32);
  uint32_t callback_end = 0;
  if (callback != 0) {
    const uint32_t first = guest::LoadU32(base, device + cb::kSegmentChain + cb::kChainFirst);
    callback_end = cb::WriteCallback(base, device, callback - 4, cpus | 1, kWakeWorker,
                                     cb::PhysicalAddress(first) + 0xC0000000u) +
                   4;
  }
  // When either reservation fails, the device is out of space (Reserve sets
  // +10809 bit 6), so the path below that would read their results (the
  // original's stack words then) isn't taken.
  const uint32_t entry = cb::Reserve(base, device, 32, 4);
  guest::StoreU32(base, device + cb::kTilingEnd, 0);
  guest::StoreU32(base, device + cb::kTilingGeneration, 0);
  const uint8_t flags = guest::LoadU8(base, device + cb::kFlags);
  if (flags & 0x40) {
    cb::RestoreSegment(base, device);
    guest::StoreU8(base, device + cb::kFlags,
                   uint8_t(guest::LoadU8(base, device + cb::kFlags) & ~0x40));
    return 1;
  }
  cb::CloseChain(base, device + cb::kSegmentChain);
  cb::CloseChain(base, device + kDrawChain);
  cb::CloseChain(base, device + kThirdChain);
  if (guest::LoadU32(base, device + cb::kTailsDirect) != 0 ||
      !(guest::LoadU8(base, device + cb::kFlags) & 0x2)) {
    const uint32_t words = uint32_t(int32_t(callback_end - callback) >> 2);
    calls.kick(device, entry, cb::PhysicalAddress(callback), words, tiles > 1 ? 2 : 1,
               device + kSegmentChainCopy);
  }
  return 0;
}

namespace {

// A notification event's header written in place: type 0, not signaled, its
// wait list empty (pointing at itself).
void EventHeader(uint8_t* base, uint32_t event) {
  guest::StoreU8(base, event, 0);
  guest::StoreU32(base, event + 4, 0);
  guest::StoreU32(base, event + 8, event + 8);
  guest::StoreU32(base, event + 12, event + 8);
}

}  // namespace

uint32_t CreateWorkers(uint8_t* base, uint32_t device, const WorkerCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B6D70 Device::CreateWorkers");
  if (guest::LoadU32(base, device + kDeviceOptions) & 0x100) {
    return 1;
  }
  const uint32_t state = device + kWorkerState;
  guest::StoreU32(base, device + 10816, guest::LoadU32(base, device + 13496));
  guest::StoreU32(base, device + 10824, guest::LoadU32(base, device + 13500));
  guest::StoreU32(base, device + 10872, 0);
  guest::StoreU32(base, device + 10904, 0xC0000000u);
  guest::StoreU32(base, device + 10920, kL2Area);
  EventHeader(base, device + 10876);
  uint32_t slices = 0;
  for (uint32_t cpu = 0; cpu < 6; cpu++) {
    if (!(guest::LoadU32(base, device + kDeviceOptions) & (0x01000000u << cpu))) {
      continue;
    }
    const uint32_t block = device + kWorkers + kWorkerBytes * cpu;
    const uint32_t signal = kWorkerSignals + 128 * cpu;
    guest::StoreU32(base, device + kLastWorkerCpu, cpu);
    guest::StoreU32(base, block + 4, cpu);
    guest::StoreU32(base, block, state);
    guest::StoreU32(base, block + 16, guest::LoadU32(base, device + 10920) + slices);
    slices += kWorkerSlice;
    guest::StoreU32(base, block + 24, (signal + 0x41000000u) | 2);
    guest::StoreU32(base, block + 28, signal);
    guest::StoreU32(base, block + 8, guest::LoadU32(base, block + 16));
    guest::StoreU32(base, block + 20, guest::LoadU32(base, device + 10920) + slices);
    EndSegmentIfFull(base, device, calls.end_segment);
    EventHeader(base, block + 32);
    const uint32_t made = calls.create_thread(block + 48, kWorkerSlice, block + 52, 0,
                                              kWorkerThread, block, 0x80u | ((1u << cpu) << 24));
    if (int32_t(made) < 0) {
      return 0;
    }
    calls.set_priority(guest::LoadU32(base, block + 48), 17);
  }
  return 1;
}

}  // namespace graphics::predicated_tiling
