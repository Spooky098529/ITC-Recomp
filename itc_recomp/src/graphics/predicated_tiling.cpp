// The graphics library's predicated tiling
// (docs/functions/predicated-tiling.md): see predicated_tiling.h.

#include "graphics/predicated_tiling.h"

#include "guest.h"

#include "base/log.h"
#include "graphics/device_state.h"
#include "schema/command_buffer.h"
#include "schema/device_state.h"
#include "schema/predicated_tiling.h"

#include <mutex>

namespace graphics::predicated_tiling {

namespace cb = command_buffer;

namespace {

// Ends the segment when the write position has passed its soft end, as
// every packet writer does before writing.
void EndSegmentIfFull(uint8_t* base, uint32_t device,
                      const std::function<void(uint32_t)>& end_segment) {
  const schema::Device d(base, device);
  if (d.write_position() > d.soft_end()) {
    end_segment(device);
  }
}

}  // namespace

uint32_t SetPredication(uint8_t* base, uint32_t device, uint32_t select,
                        const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_825BEB80 Device::SetPredication");
  const schema::Device d(base, device);
  if (select == 0) {
    d.set_predicated(0);
    device_state::UpdatePredicatedTiling(base, device);
    d.set_bin_mask(0xFFFFFFFFu);
  } else {
    d.set_bin_mask(select);
    d.set_predicated(1);
    d.set_tiling_flags(uint8_t(d.tiling_flags() & ~0x2));
  }
  d.set_bin_mask_high(0);
  d.set_predication_select(select);
  d.set_predication_select_high(0);
  EndSegmentIfFull(base, device, calls.end_segment);
  return d.write_position();
}

uint32_t BeginTiledSegments(uint8_t* base, uint32_t device, const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B6AD8 Device::BeginTiledSegments");
  const schema::Device d(base, device);
  calls.end_segment(device);
  cb::SaveSegment(base, device);
  d.set_tiling_end(d.tail_position());
  d.set_tiling_generation(d.tail_generation() + 1);
  // The segment chain copied word by word (a load, then its store).
  const schema::CommandChain from(base, d.segment_chain_first_address());
  const schema::CommandChain to(base, d.saved_chain_first_address());
  to.set_first(from.first());
  to.set_block(from.block());
  to.set_cursor(from.cursor());
  to.set_end(from.end());
  to.set_device(from.device());
  const uint32_t segments = d.segment_chain_first_address();
  cb::InitChain(base, segments, device);
  cb::InitChain(base, d.draw_chain_first_address(), device);
  cb::InitChain(base, d.third_chain_first_address(), device);
  const schema::CommandChain chain(base, segments);
  uint32_t header = chain.cursor();
  if (header + kHeaderBytes > chain.end()) {
    header = cb::NewBlock(base, segments);
  }
  chain.set_cursor(header + kHeaderBytes);
  return header + 16;
}

uint32_t EndTiledSegments(uint8_t* base, uint32_t device, uint32_t tiles, uint32_t cpus,
                          const GuestCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B6BF8 Device::EndTiledSegments");
  const schema::Device d(base, device);
  const uint32_t callback = cb::Reserve(base, device, 100, 32);
  uint32_t callback_end = 0;
  if (callback != 0) {
    const uint32_t first =
        schema::CommandChain(base, d.segment_chain_first_address()).first().address;
    callback_end = cb::WriteCallback(base, device, callback - 4, cpus | 1, kWakeWorker,
                                     cb::PhysicalAddress(first) + 0xC0000000u) +
                   4;
  }
  // When either reservation fails, the device is out of space (Reserve sets
  // +10809 bit 6), so the path below that would read their results (the
  // original's stack words then) isn't taken.
  const uint32_t entry = cb::Reserve(base, device, 32, 4);
  d.set_tiling_end(0);
  d.set_tiling_generation(0);
  const uint8_t flags = d.flags();
  if (flags & 0x40) {
    cb::RestoreSegment(base, device);
    d.set_flags(uint8_t(d.flags() & ~0x40));
    return 1;
  }
  cb::CloseChain(base, d.segment_chain_first_address());
  cb::CloseChain(base, d.draw_chain_first_address());
  cb::CloseChain(base, d.third_chain_first_address());
  if (d.tap().address != 0 || !(d.flags() & 0x2)) {
    const uint32_t words = uint32_t(int32_t(callback_end - callback) >> 2);
    calls.kick(device, entry, cb::PhysicalAddress(callback), words, tiles > 1 ? 2 : 1,
               d.saved_chain_first_address());
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
  const schema::Device d(base, device);
  if (d.options() & 0x100) {
    return 1;
  }
  const uint32_t state = d.worker_lock_address();  // the shared state (+10812)
  d.set_worker_ring(d.ring());
  d.set_worker_ring_mask(d.ring_mask());
  d.set_worker_lock2(0);
  d.set_worker_c0000000(0xC0000000u);
  d.set_worker_l2_area(kL2Area);
  EventHeader(base, d.worker_event_address());
  uint32_t slices = 0;
  for (uint32_t cpu = 0; cpu < 6; cpu++) {
    if (!(d.options() & (0x01000000u << cpu))) {
      continue;
    }
    // The CPU's 56-byte block (no type of its own: the measure meets it as
    // the device; see the schema file's header).
    const uint32_t block = d.workers_address() + kWorkerBytes * cpu;
    const uint32_t signal = kWorkerSignals + 128 * cpu;
    d.set_last_worker_cpu(cpu);
    guest::StoreU32(base, block + 4, cpu);
    guest::StoreU32(base, block, state);
    guest::StoreU32(base, block + 16, d.worker_l2_area() + slices);
    slices += kWorkerSlice;
    guest::StoreU32(base, block + 24, (signal + 0x41000000u) | 2);
    guest::StoreU32(base, block + 28, signal);
    guest::StoreU32(base, block + 8, guest::LoadU32(base, block + 16));
    guest::StoreU32(base, block + 20, d.worker_l2_area() + slices);
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

namespace {

// The device's lock (+13548): only the workers take it.
std::mutex g_device_lock;

}  // namespace

uint32_t WorkerThread(uint8_t* base, uint32_t block, uint32_t timeout,
                      const WorkerLoopCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822B69A8 Device::WorkerThread");
  const uint32_t event = block + 32;
  for (;;) {
    const uint32_t state = guest::LoadU32(base, block);
    guest::StoreU32(base, timeout, 0xFFFFFFFFu);
    guest::StoreU32(base, timeout + 4, uint32_t(int32_t(-300000)));
    const uint32_t wait_timeout =
        guest::LoadU32(base, block + 4) == schema::TilingWorkerState(base, state).last_worker_cpu()
            ? timeout
            : 0;
    while (calls.wait(event, 3, 1, 0, wait_timeout) == 258) {
      const uint32_t device = guest::LoadU32(base, guest::LoadU32(base, kVdGlobalDeviceImport));
      std::lock_guard lock(g_device_lock);
      if (schema::Device(base, device).flags2() & 0x4) {
        calls.present(device);
      }
    }
    const uint32_t r3 = calls.reset_event(event);
    if (schema::TilingWorkerState(base, state).ring() == 0) {
      return r3;
    }
    calls.job(block);
  }
}

uint32_t WakeWorker(uint8_t* base, uint32_t argument, uint32_t cpu,
                    const std::function<uint32_t(uint32_t event, uint32_t increment,
                                                 uint32_t wait)>& set_event) {
  ITC_NATIVE_RUNNING("sub_822B6A78 Device::WakeWorker");
  const uint32_t device = guest::LoadU32(base, guest::LoadU32(base, kVdGlobalDeviceImport));
  const schema::Device d(base, device);
  d.set_worker_callback_argument(argument);
  return set_event(d.workers_address() + cpu * kWorkerBytes + 32, 1, 0);
}

}  // namespace graphics::predicated_tiling
