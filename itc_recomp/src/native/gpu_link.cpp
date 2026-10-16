// Where the graphics library meets the GPU (see "Plan for milestone 6" in
// docs/native-renderer.md): the kick that hands the emulated GPU its work,
// the block the GPU writes its progress into, and the library's interrupt
// callback.
//
// --native_renderer_log_gpu logs, every 5 s, the packets in the kicked
// buffers that have effects outside the GPU (memory writes, interrupts,
// waits, swaps; and register writes outside the context registers), the
// progress block's values, and the interrupts the callback got. Milestone 6
// step 1: what exclusive mode has to do in the GPU's place.
//
// In exclusive mode (--native_renderer_exclusive) the kick never reaches
// the emulated GPU: its buffers are walked here and only their effects
// outside the GPU are made (see "Exclusive mode" in
// docs/native-renderer.md), so the library sees everything done at once.

#include "guest.h"
#include "renderer.h"
#include "state.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <array>
#include <atomic>
#include <chrono>
#include <map>
#include <mutex>
#include <string>

REXCVAR_DEFINE_BOOL(native_renderer_log_gpu, false, "ITC",
                    "Log what the graphics library hands the GPU that has effects outside it "
                    "(memory writes, interrupts, waits, swaps), and the GPU's progress (a "
                    "debugging aid for the native renderer's exclusive mode)");

namespace native {
namespace {

using Clock = std::chrono::steady_clock;

// Device fields (see "Plan for milestone 6").
constexpr uint32_t kDeviceProgressBlock = 10768;  // pointer; the GPU writes into it
constexpr uint32_t kDeviceCallbackBlock = 10772;  // pointer; the interrupt callback's
constexpr uint32_t kDeviceCommandPosition = 10780;
constexpr uint32_t kDeviceFlags = 10809;  // bit 1 (0x2): the kick takes another path
constexpr uint32_t kDevicePrimaryWrite = 10820;
constexpr uint32_t kDevicePendingKicks = 10868;
constexpr uint32_t kDeviceVblankCount = 15148;
// sub_822B6A78: inserted by sub_822B6BF8 (at EndTiling's end); wakes the
// library's worker thread on CPU 2, which submits the deferred kicks.
constexpr uint32_t kDeferredKickWorkerCallback = 0x822B6A78;

const char* OpcodeName(uint32_t opcode) {
  switch (opcode) {
    case 0x10: return "NOP";
    case 0x21: return "REG_RMW";
    case 0x22: return "DRAW_INDX";
    case 0x23: return "VIZ_QUERY";
    case 0x25: return "SET_STATE";
    case 0x27: return "IM_LOAD";
    case 0x2B: return "IM_LOAD_IMMEDIATE";
    case 0x2C: return "IM_STORE";
    case 0x2D: return "SET_CONSTANT";
    case 0x2E: return "LOAD_CONSTANT_CONTEXT";
    case 0x2F: return "LOAD_ALU_CONSTANT";
    case 0x36: return "DRAW_INDX_2";
    case 0x37: return "INDIRECT_BUFFER_PFD";
    case 0x3B: return "INVALIDATE_STATE";
    case 0x3C: return "WAIT_REG_MEM";
    case 0x3D: return "MEM_WRITE";
    case 0x3E: return "REG_TO_MEM";
    case 0x3F: return "INDIRECT_BUFFER";
    case 0x44: return "COND_EXEC";
    case 0x45: return "COND_WRITE";
    case 0x46: return "EVENT_WRITE";
    case 0x48: return "ME_INIT";
    case 0x4F: return "MEM_WRITE_CNTR";
    case 0x54: return "INTERRUPT";
    case 0x55: return "SET_CONSTANT2";
    case 0x56: return "SET_SHADER_CONSTANTS";
    case 0x58: return "EVENT_WRITE_SHD";
    case 0x59: return "EVENT_WRITE_CFL";
    case 0x5A: return "EVENT_WRITE_EXT";
    case 0x5B: return "EVENT_WRITE_ZPD";
    case 0x5E: return "CONTEXT_UPDATE";
    case 0x64: return "XE_SWAP";
    default: return "?";
  }
}

// Packets whose effects reach past the GPU's own state (or that wait).
bool HasOutsideEffect(uint32_t opcode) {
  switch (opcode) {
    case 0x23: case 0x2C: case 0x3C: case 0x3D: case 0x3E: case 0x45: case 0x46: case 0x4F:
    case 0x54: case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x64: case 0x21:
      return true;
    default:
      return false;
  }
}

struct Stats {
  uint64_t kicks = 0, kicks_other_path = 0, buffers = 0, words = 0, unreadable = 0;
  std::map<uint32_t, uint64_t> opcodes;              // type-3 opcode: count
  std::map<uint32_t, std::string> opcode_samples;    // opcode: an example's words
  std::map<uint32_t, uint64_t> registers;            // non-context register: writes
  std::map<uint32_t, uint32_t> register_values;      // its last value
} g_stats;
std::mutex g_mutex;
Clock::time_point g_last_log = Clock::now();
std::atomic<uint64_t> g_interrupts[2] = {};
std::atomic<uint32_t> g_last_interrupt_cpu{0};
// A detailed trace of the kicks: the first ones, a while every 10 s, and
// those with rare packets (up to a limit).
uint32_t g_trace_kicks = 60;
Clock::time_point g_last_trace = Clock::now();
uint32_t g_rare_traces = 300;
bool g_rare = false;  // the walk met a rare packet
std::atomic<int> g_tiling_logs{400};  // see sub_822B6BF8

uint32_t ReadPhysical(uint8_t* base, uint32_t physical) {
  const uint8_t* p = PhysicalHostAddress(base, physical & 0x1FFFFFFCu);
  return (uint32_t(p[0]) << 24) | (uint32_t(p[1]) << 16) | (uint32_t(p[2]) << 8) | p[3];
}

std::string Words(uint8_t* base, uint32_t address, uint32_t count) {
  std::string out;
  for (uint32_t i = 0; i < count && i < 8; i++) {
    out += fmt::format("{}{:08X}", i ? " " : "", ReadPhysical(base, address + 4 * i));
  }
  return out;
}

// Walks one buffer's packets (address and size in words as INDIRECT_BUFFER
// takes them), into indirect buffers too.
void WalkBuffer(uint8_t* base, uint32_t address, uint32_t size, int depth, std::string* trace) {
  Stats& s = g_stats;
  s.buffers++;
  s.words += size;
  address &= 0x1FFFFFFFu;
  if (size > 0x100000 || address + 4 * size > kPhysicalMemorySize) {
    s.unreadable++;
    return;
  }
  uint32_t i = 0;
  while (i < size) {
    uint32_t header = ReadPhysical(base, address + 4 * i);
    uint32_t type = header >> 30;
    if (type == 0) {
      uint32_t count = ((header >> 16) & 0x3FFF) + 1;
      uint32_t first = header & 0x7FFF;
      bool one_register = (header >> 15) & 1;
      for (uint32_t k = 0; k < count; k++) {
        uint32_t reg = one_register ? first : first + k;
        if (reg < 0x2000) {
          // Scratch registers (written back to memory) and their set-up.
          g_rare |= (reg >= 0x578 && reg <= 0x57F) || reg == 0x1DC || reg == 0x1DD;
          s.registers[reg]++;
          s.register_values[reg] = ReadPhysical(base, address + 4 * (i + 1 + k));
          if (trace) {
            *trace += fmt::format(" [reg {:04X}={:08X}]", reg, s.register_values[reg]);
          }
        }
      }
      i += 1 + count;
    } else if (type == 1) {
      uint32_t regs[2] = {header & 0x7FF, (header >> 11) & 0x7FF};
      for (uint32_t k = 0; k < 2; k++) {
        if (regs[k] < 0x2000) {
          s.registers[regs[k]]++;
          s.register_values[regs[k]] = ReadPhysical(base, address + 4 * (i + 1 + k));
        }
      }
      i += 3;
    } else if (type == 2) {
      i += 1;
    } else {
      uint32_t opcode = (header >> 8) & 0x7F;
      uint32_t count = ((header >> 16) & 0x3FFF) + 1;
      s.opcodes[opcode]++;
      uint32_t payload = address + 4 * (i + 1);
      if (HasOutsideEffect(opcode)) {
        // All but the fences, the coherency waits, cache events and swaps.
        g_rare |= opcode != 0x58 && opcode != 0x3C && opcode != 0x46 && opcode != 0x64;
        s.opcode_samples[opcode] = Words(base, payload, count);
        if (trace) {
          *trace += fmt::format(" [{} {}]", OpcodeName(opcode), Words(base, payload, count));
        }
      }
      if ((opcode == 0x3F || opcode == 0x37) && depth < 4) {
        WalkBuffer(base, ReadPhysical(base, payload), ReadPhysical(base, payload + 4) & 0xFFFFF,
                   depth + 1, trace);
      }
      i += 1 + count;
    }
  }
}

std::string ProgressBlock(uint8_t* base, uint32_t device) {
  uint32_t block = guest::LoadU32(base, device + kDeviceProgressBlock);
  if (!block) {
    return "no progress block";
  }
  return fmt::format("block {:08X}: +0 {:08X} +4 {:08X} +8 {:08X} +60 {:08X}; command position "
                     "{:08X}, primary write {:08X}, pending kicks {}, vblanks {}",
                     block, guest::LoadU32(base, block), guest::LoadU32(base, block + 4),
                     guest::LoadU32(base, block + 8), guest::LoadU32(base, block + 60),
                     guest::LoadU32(base, device + kDeviceCommandPosition),
                     guest::LoadU32(base, device + kDevicePrimaryWrite),
                     guest::LoadU32(base, device + kDevicePendingKicks),
                     guest::LoadU32(base, device + kDeviceVblankCount));
}

void LogStats(uint8_t* base, uint32_t device) {
  Stats& s = g_stats;
  std::string opcodes;
  for (auto& [opcode, count] : s.opcodes) {
    opcodes += fmt::format(" {}({:02X})={}", OpcodeName(opcode), opcode, count);
  }
  REXLOG_INFO("native renderer: gpu: {} kicks ({} by the other path), {} buffers, {} words "
              "({} unreadable); type-3 packets:{}",
              s.kicks, s.kicks_other_path, s.buffers, s.words, s.unreadable, opcodes);
  for (auto& [opcode, sample] : s.opcode_samples) {
    REXLOG_INFO("native renderer: gpu: e.g. {} {}", OpcodeName(opcode), sample);
  }
  std::string registers;
  for (auto& [reg, count] : s.registers) {
    registers += fmt::format(" {:04X}={} (last {:08X})", reg, count, s.register_values[reg]);
  }
  REXLOG_INFO("native renderer: gpu: register writes below 0x2000:{}", registers);
  REXLOG_INFO("native renderer: gpu: interrupts: {} vblank, {} from the command stream (last "
              "cpu {}); {}",
              g_interrupts[0].exchange(0), g_interrupts[1].exchange(0),
              g_last_interrupt_cpu.load(), ProgressBlock(base, device));
  s = Stats{};
}


// Exclusive mode: the GPU's effects outside itself, made as the SDK's
// command processor makes them (command_processor.cpp).
struct Effects {
  // Registers below the context registers, as last written (for the scratch
  // registers' write-back, COND_WRITE, REG_TO_MEM and REG_RMW).
  std::array<uint32_t, 0x2000> registers{};
  uint64_t fences = 0, scratch_writes = 0, memory_writes = 0, interrupts = 0, callbacks = 0,
           kicks = 0, words = 0, worker_wakes_skipped = 0;
  std::map<uint32_t, uint64_t> unhandled;  // opcode: count
} g_effects;

// The display's gamma table (registers 19xx, written at boot), kept as the
// SDK's command processor keeps it (command_processor.cpp): the 256-entry
// table (DC_LUT_30_COLOR, 10 bits a channel, or three DC_LUT_SEQ_COLOR
// writes) and the 128-entry PWL one (DC_LUT_PWL_DATA, a channel a write).
// Logged at the next swap after it changes (a debugging aid: --xbox_gamma
// uses our own copy, xbox_gamma_table.h).
constexpr uint32_t kDcLutRwMode = 0x1921, kDcLutRwIndex = 0x1922, kDcLutSeqColor = 0x1923,
                   kDcLutPwlData = 0x1924, kDcLut30Color = 0x1925, kDcLutWriteEnMask = 0x1927,
                   kDcLutaControl = 0x1930;
struct GammaRamp {
  std::array<uint32_t, 256> table{};                // DC_LUT_30_COLOR words
  std::array<std::array<uint32_t, 3>, 128> pwl{};  // DC_LUT_PWL_DATA words, r g b
  uint32_t component = 0;  // the next of the three sequential writes
  uint64_t writes = 0;
  bool changed = false;
} g_gamma;

void OnGammaRegister(uint32_t reg, uint32_t value) {
  GammaRamp& g = g_gamma;
  const auto& regs = g_effects.registers;
  uint32_t index = regs[kDcLutRwIndex] & 0xFF;
  bool enabled = false;
  switch (reg) {
    case kDcLutRwIndex:
      g.component = 0;
      return;
    case kDcLutSeqColor: {
      enabled = (regs[kDcLutWriteEnMask] >> (2 - g.component)) & 1;
      if (enabled) {
        uint32_t shift = 20 - 10 * g.component;  // red, green, blue
        g.table[index] = (g.table[index] & ~(0x3FFu << shift)) | (((value & 0xFFFF) >> 6) << shift);
      }
      break;
    }
    case kDcLutPwlData:
      enabled = (regs[kDcLutWriteEnMask] >> (2 - g.component)) & 1;
      if (enabled) {
        g.pwl[index & 0x7F][g.component] = value & 0xFFC0FFC0u;
      }
      break;
    case kDcLut30Color: {
      uint32_t mask = regs[kDcLutWriteEnMask] & 7;
      for (uint32_t c = 0; c < 3; c++) {  // bit 0 blue, 1 green, 2 red
        if ((mask >> c) & 1) {
          g.table[index] = (g.table[index] & ~(0x3FFu << (10 * c))) | (value & (0x3FFu << (10 * c)));
        }
      }
      g.writes += mask != 0;
      g.changed |= mask != 0;
      return;
    }
    default:
      g.changed = true;  // the mode or the control
      return;
  }
  g.writes += enabled;
  g.changed |= enabled;
  if (++g.component >= 3) {  // the index steps after the third channel
    g.component = 0;
    uint32_t next = reg == kDcLutPwlData ? (index & ~0x7Fu) | ((index + 1) & 0x7F) : index + 1;
    g_effects.registers[kDcLutRwIndex] = (regs[kDcLutRwIndex] & ~0xFFu) | (next & 0xFF);
  }
}

void LogGammaRamp() {
  GammaRamp& g = g_gamma;
  const auto& regs = g_effects.registers;
  REXLOG_INFO("native renderer: gamma: {} writes; DC_LUTA_CONTROL {:08X}, DC_LUT_RW_MODE {:08X} "
              "({}), D1GRPH_CONTROL {:08X}",
              g.writes, regs[kDcLutaControl], regs[kDcLutRwMode],
              (regs[kDcLutRwMode] & 1) ? "PWL" : "256 entries", regs[0x1841]);
  for (uint32_t row = 0; row < 256; row += 16) {
    std::string line;
    for (uint32_t i = row; i < row + 16; i++) {
      uint32_t v = g.table[i];
      line += fmt::format(" {}:{}/{}/{}", i, (v >> 20) & 0x3FF, (v >> 10) & 0x3FF, v & 0x3FF);
    }
    REXLOG_INFO("native renderer: gamma table (r/g/b of 1023):{}", line);
  }
  for (uint32_t row = 0; row < 128; row += 16) {
    std::string line;
    for (uint32_t i = row; i < row + 16; i++) {
      line += fmt::format(" {}:{:08X}/{:08X}/{:08X}", i, g.pwl[i][0], g.pwl[i][1], g.pwl[i][2]);
    }
    REXLOG_INFO("native renderer: gamma PWL (delta<<16|base, r/g/b):{}", line);
  }
  g.writes = 0;
  g.changed = false;
}


void StorePhysical(uint8_t* base, uint32_t address, uint32_t value) {
  // Endianness in the address's low bits as the GPU takes it; the game uses
  // 8-in-32 (2), the guest's own order. 0 (none) stores it little-endian.
  uint32_t endian = address & 3;
  uint8_t* p = PhysicalHostAddress(base, address & 0x1FFFFFFCu);
  if (endian == 0) {
    p[0] = uint8_t(value); p[1] = uint8_t(value >> 8);
    p[2] = uint8_t(value >> 16); p[3] = uint8_t(value >> 24);
    return;
  }
  if (endian == 1) {  // 8-in-16
    value = ((value & 0xFF00FF00u) >> 8) | ((value & 0x00FF00FFu) << 8);
    value = (value >> 16) | (value << 16);
  } else if (endian == 3) {  // 16-in-32
    value = (value >> 16) | (value << 16);
  }
  p[0] = uint8_t(value >> 24); p[1] = uint8_t(value >> 16);
  p[2] = uint8_t(value >> 8); p[3] = uint8_t(value);
}

uint32_t LoadPhysical(uint8_t* base, uint32_t address) {
  uint32_t value = ReadPhysical(base, address);  // big-endian, i.e. 8-in-32
  uint32_t endian = address & 3;
  if (endian == 0) {
    value = __builtin_bswap32(value);
  } else if (endian == 1) {
    value = (value >> 16) | (value << 16);
    value = ((value & 0xFF00FF00u) >> 8) | ((value & 0x00FF00FFu) << 8);
  } else if (endian == 3) {
    value = (value >> 16) | (value << 16);
  }
  return value;
}

void WriteRegister(uint8_t* base, uint32_t reg, uint32_t value) {
  Effects& e = g_effects;
  if (reg >= 0x2000) {
    return;  // context registers: the GPU's own state
  }
  e.registers[reg] = value;
  if (reg >= kDcLutRwMode && reg <= kDcLutaControl) {
    OnGammaRegister(reg, value);
  }
  // The scratch registers write back to memory where SCRATCH_UMSK says so.
  if (reg >= 0x578 && reg <= 0x57F && (e.registers[0x1DC] >> (reg - 0x578)) & 1) {
    StorePhysical(base, (e.registers[0x1DD] + 4 * (reg - 0x578)) | 2, value);
    e.scratch_writes++;
  }
}

bool Compare(uint32_t function, uint32_t value, uint32_t reference) {
  switch (function & 7) {
    case 1: return value < reference;
    case 2: return value <= reference;
    case 3: return value == reference;
    case 4: return value != reference;
    case 5: return value >= reference;
    case 6: return value > reference;
    case 7: return true;
    default: return false;
  }
}

// The command stream's INTERRUPT on the CPUs in `mask`: what the library's
// interrupt callback (sub_8229DEA0, source 1) does, for the CPU named by the
// packet rather than the calling thread's: call the function in the
// callback block (+16, written by scratch register 4) with +20 (scratch 5),
// unless it's 0BADF00D, then clear the CPU's bit in +0. The thread's CPU
// number (PCR +268) is set to the packet's for the call, as the SDK's
// interrupt dispatch does: callbacks read it (sub_822B6A78 sets the event of
// that CPU's worker thread, sub_822B69A8; the render thread's own CPU left
// it unwoken, hanging loading screens).
void Interrupt(PPCContext& ctx, uint8_t* base, uint32_t device, uint32_t mask) {
  Effects& e = g_effects;
  uint32_t block = guest::LoadU32(base, device + kDeviceCallbackBlock);
  for (uint32_t cpu = 0; cpu < 6; cpu++) {
    if (!(mask & (1u << cpu)) || !block) {
      continue;
    }
    e.interrupts++;
    uint32_t function = guest::LoadU32(base, block + 16);
    if (function == kDeferredKickWorkerCallback) {
      // Wakes the worker that submits deferred kicks; there are none (see
      // sub_8229E848).
      e.worker_wakes_skipped++;
    } else if (function == 0x0BADF00D) {
      REXLOG_ERROR("native renderer: gpu: interrupt with no callback (0BADF00D)");
    } else if (function) {
      e.callbacks++;
      uint32_t own_cpu = guest::LoadU8(base, ctx.r13.u32 + 268);
      REXLOG_INFO("native renderer: gpu: interrupt on cpu {} (the kicking thread's is {}): "
                  "calling {:08X}({:08X})",
                  cpu, own_cpu, function, guest::LoadU32(base, block + 20));
      PPCContext saved = ctx;
      guest::StoreU8(base, ctx.r13.u32 + 268, uint8_t(cpu));
      ctx.r3.u32 = guest::LoadU32(base, block + 20);
      REX_CALL_INDIRECT_FUNC(function);
      ctx = saved;
      guest::StoreU8(base, ctx.r13.u32 + 268, uint8_t(own_cpu));
    }
    guest::StoreU32(base, block, guest::LoadU32(base, block) & ~(1u << cpu) & 0x3F);
  }
}

void ApplyBuffer(PPCContext& ctx, uint8_t* base, uint32_t device, uint32_t address,
                 uint32_t size, int depth) {
  Effects& e = g_effects;
  e.words += size;
  address &= 0x1FFFFFFFu;
  if (size > 0x100000 || address + 4 * size > kPhysicalMemorySize) {
    REXLOG_ERROR("native renderer: gpu: buffer {:08X} of {} words out of memory", address, size);
    return;
  }
  auto word = [&](uint32_t i) { return ReadPhysical(base, address + 4 * i); };
  uint32_t i = 0;
  while (i < size) {
    uint32_t header = word(i);
    uint32_t type = header >> 30;
    if (type == 0) {
      uint32_t count = ((header >> 16) & 0x3FFF) + 1;
      uint32_t first = header & 0x7FFF;
      if (first < 0x2000) {
        bool one_register = (header >> 15) & 1;
        for (uint32_t k = 0; k < count; k++) {
          WriteRegister(base, one_register ? first : first + k, word(i + 1 + k));
        }
      }
      i += 1 + count;
      continue;
    }
    if (type == 1) {
      WriteRegister(base, header & 0x7FF, word(i + 1));
      WriteRegister(base, (header >> 11) & 0x7FF, word(i + 2));
      i += 3;
      continue;
    }
    if (type == 2) {
      i += 1;
      continue;
    }
    uint32_t opcode = (header >> 8) & 0x7F;
    uint32_t count = ((header >> 16) & 0x3FFF) + 1;
    uint32_t p = i + 1;
    switch (opcode) {
      case 0x3F:  // INDIRECT_BUFFER
      case 0x37:  // INDIRECT_BUFFER_PFD
        if (depth < 4) {
          ApplyBuffer(ctx, base, device, word(p), word(p + 1) & 0xFFFFF, depth + 1);
        }
        break;
      case 0x58: {  // EVENT_WRITE_SHD: the fences
        uint32_t initiator = word(p);
        // Bit 31 writes the GPU's swap and vblank counter instead (not seen).
        if (initiator >> 31) {
          e.unhandled[0x158]++;
        }
        StorePhysical(base, word(p + 1), word(p + 2));
        e.fences++;
        break;
      }
      case 0x3D:  // MEM_WRITE
        for (uint32_t k = 1; k < count; k++) {
          StorePhysical(base, word(p) + 4 * (k - 1), word(p + k));
        }
        e.memory_writes++;
        break;
      case 0x3E: {  // REG_TO_MEM
        uint32_t reg = word(p);
        StorePhysical(base, word(p + 1), reg < 0x2000 ? e.registers[reg] : 0);
        e.memory_writes++;
        break;
      }
      case 0x21: {  // REG_RMW
        uint32_t info = word(p), reg = info & 0x1FFF;
        if (reg < 0x2000) {
          uint32_t and_mask = word(p + 1), or_mask = word(p + 2);
          uint32_t value = e.registers[reg];
          value &= (info >> 31) ? e.registers[and_mask & 0x1FFF] : and_mask;
          value |= ((info >> 30) & 1) ? e.registers[or_mask & 0x1FFF] : or_mask;
          WriteRegister(base, reg, value);
        }
        break;
      }
      case 0x45: {  // COND_WRITE
        uint32_t info = word(p), poll = word(p + 1);
        uint32_t value = (info & 0x10) ? LoadPhysical(base, poll)
                                       : poll < 0x2000 ? e.registers[poll] : 0;
        if (Compare(info, value & word(p + 3), word(p + 2))) {
          if (info & 0x100) {
            StorePhysical(base, word(p + 4), word(p + 5));
            e.memory_writes++;
          } else {
            WriteRegister(base, word(p + 4), word(p + 5));
          }
        }
        break;
      }
      case 0x54:  // INTERRUPT
        Interrupt(ctx, base, device, word(p));
        break;
      // No effect outside the GPU: waits (on our own effects or the GPU's
      // caches), cache events, the swap (our window presents itself), and
      // the GPU's own work.
      case 0x3C:  // WAIT_REG_MEM
        break;
      case 0x64:  // XE_SWAP
        if (g_gamma.changed) {
          LogGammaRamp();
        }
        break;
      case 0x46:  // EVENT_WRITE
        if (count > 1) {
          e.unhandled[opcode]++;
        }
        break;
      case 0x23: case 0x2C: case 0x4F: case 0x59: case 0x5A: case 0x5B:
        e.unhandled[opcode]++;  // queries, extents and the like: not seen yet
        break;
      default:
        break;
    }
    i += 1 + count;
  }
}

void LogEffects() {
  Effects& e = g_effects;
  std::string unhandled;
  for (auto& [opcode, count] : e.unhandled) {
    unhandled += fmt::format(" {}({:02X})={}", OpcodeName(opcode & 0xFF), opcode, count);
  }
  REXLOG_INFO("native renderer: exclusive: {} kicks ({} words) made by us: {} fences, {} scratch "
              "write-backs, {} memory writes, {} interrupts ({} callbacks, {} deferred kick "
              "worker wakes skipped); unhandled:{}",
              e.kicks, e.words, e.fences, e.scratch_writes, e.memory_writes, e.interrupts,
              e.callbacks, e.worker_wakes_skipped, unhandled.empty() ? " none" : unhandled);
  e.kicks = e.words = e.fences = e.scratch_writes = e.memory_writes = e.interrupts = e.callbacks =
      e.worker_wakes_skipped = 0;
  e.unhandled.clear();
}
}  // namespace
}  // namespace native

// The kick (see "Plan for milestone 6"): r3 device, r4 pairs of (size in
// words, address), r5 pair count. Writes an INDIRECT_BUFFER per pair into
// the primary ring and CP_RB_WPTR.
extern "C" REX_FUNC(sub_8229E3F0) {
  REX_FUNC_PROLOGUE();
  using namespace native;
  if (REXCVAR_GET(native_renderer_log_gpu)) {
    uint32_t device = ctx.r3.u32, pairs = ctx.r4.u32, count = ctx.r5.u32;
    std::lock_guard lock(g_mutex);
    Stats& s = g_stats;
    s.kicks++;
    bool other_path = guest::LoadU8(base, device + kDeviceFlags) & 0x2;
    s.kicks_other_path += other_path;
    auto now = Clock::now();
    if (!g_trace_kicks && now - g_last_trace >= std::chrono::seconds(10)) {
      g_trace_kicks = 40;
      g_last_trace = now;
    }
    g_rare = false;
    for (uint32_t i = 0; i < count; i++) {
      WalkBuffer(base, guest::LoadU32(base, pairs + 8 * i + 4),
                 guest::LoadU32(base, pairs + 8 * i), 0, nullptr);
    }
    bool rare = g_rare && g_rare_traces;
    if (g_trace_kicks || rare) {
      g_rare_traces -= rare;
      // Again, describing it (the stats count it twice; it's rare).
      std::string trace;
      for (uint32_t i = 0; i < count; i++) {
        uint32_t size = guest::LoadU32(base, pairs + 8 * i);
        uint32_t address = guest::LoadU32(base, pairs + 8 * i + 4);
        trace += fmt::format(" buffer {:08X} {} words:", address, size);
        WalkBuffer(base, address, size, 0, &trace);
      }
      g_trace_kicks -= g_trace_kicks != 0;
      REXLOG_INFO("native renderer: gpu: kick (lr {:08X}{}): {};{}", uint32_t(ctx.lr),
                  other_path ? ", other path" : "", ProgressBlock(base, device), trace);
    }
    if (now - g_last_log >= std::chrono::seconds(5)) {
      g_last_log = now;
      LogStats(base, device);
    }
  }
  if (Renderer_Exclusive()) {
    uint32_t device = ctx.r3.u32, pairs = ctx.r4.u32, count = ctx.r5.u32;
    g_effects.kicks++;
    if (uint32_t cpu = guest::LoadU8(base, ctx.r13.u32 + 268);
        cpu != 1 && REXCVAR_GET(native_renderer_log_gpu) && g_tiling_logs > 0) {
      g_tiling_logs--;
      std::string buffers;
      for (uint32_t i = 0; i < count; i++) {
        buffers += fmt::format(" {:08X} ({} words)", guest::LoadU32(base, pairs + 8 * i + 4),
                               guest::LoadU32(base, pairs + 8 * i));
      }
      REXLOG_INFO("native renderer: tiling: kick on cpu {} (lr {:08X}):{}", cpu,
                  uint32_t(ctx.lr), buffers);
    }
    for (uint32_t i = 0; i < count; i++) {
      ApplyBuffer(ctx, base, device, guest::LoadU32(base, pairs + 8 * i + 4),
                  guest::LoadU32(base, pairs + 8 * i), 0);
    }
    static Clock::time_point last_log = Clock::now();
    if (auto now = Clock::now(); now - last_log >= std::chrono::seconds(5)) {
      last_log = now;
      LogEffects();
    }
    return;
  }
  __imp__sub_8229E3F0(ctx, base);
}

// The library's interrupt callback (registered with
// VdSetGraphicsInterruptCallback): r3 source (0 vblank, 1 the command
// stream's INTERRUPT packet), r4 the device.
extern "C" REX_FUNC(sub_8229DEA0) {
  REX_FUNC_PROLOGUE();
  if (REXCVAR_GET(native_renderer_log_gpu)) {
    uint32_t source = ctx.r3.u32;
    native::g_interrupts[source ? 1 : 0]++;
    if (source == 1) {
      uint32_t cpu = guest::LoadU8(base, ctx.r13.u32 + 268);
      native::g_last_interrupt_cpu = cpu;
      uint32_t block = guest::LoadU32(base, ctx.r4.u32 + native::kDeviceCallbackBlock);
      REXLOG_INFO("native renderer: gpu: interrupt from the command stream on cpu {}: callback "
                  "block {:08X}: +0 {:08X}, calls {:08X}({:08X})",
                  cpu, block, guest::LoadU32(base, block), guest::LoadU32(base, block + 16),
                  guest::LoadU32(base, block + 20));
    }
  }
  __imp__sub_8229DEA0(ctx, base);
}

// Tiling's worker (milestone 6, finding why loading screens hang in
// exclusive mode): EndTiling (sub_825BF200) calls sub_822B6BF8, which
// inserts the callback sub_822B6A78 into the command stream; the GPU's
// interrupt on CPU 2 runs it, and it wakes CPU 2's worker thread
// (sub_822B69A8), which runs sub_822B6780. Logged with
// --native_renderer_log_gpu, the first few hundred calls.
namespace native {
namespace {
bool TilingLog() {
  return REXCVAR_GET(native_renderer_log_gpu) && g_tiling_logs.fetch_sub(1) > 0;
}
uint32_t Cpu(PPCContext& ctx, uint8_t* base) {
  return guest::LoadU8(base, ctx.r13.u32 + 268);
}
}  // namespace
}  // namespace native

extern "C" REX_FUNC(sub_822B6BF8) {
  REX_FUNC_PROLOGUE();
  if (native::TilingLog()) {
    REXLOG_INFO("native renderer: tiling: insert worker callback (822B6BF8) on cpu {}: r4 {:08X} "
                "r5 {:08X}, lr {:08X}",
                native::Cpu(ctx, base), ctx.r4.u32, ctx.r5.u32, uint32_t(ctx.lr));
  }
  __imp__sub_822B6BF8(ctx, base);
}

extern "C" REX_FUNC(sub_822B6A78) {
  REX_FUNC_PROLOGUE();
  if (native::TilingLog()) {
    REXLOG_INFO("native renderer: tiling: callback 822B6A78({:08X}) on cpu {}", ctx.r3.u32,
                native::Cpu(ctx, base));
  }
  __imp__sub_822B6A78(ctx, base);
}

extern "C" REX_FUNC(sub_822B6780) {
  REX_FUNC_PROLOGUE();
  uint32_t block = ctx.r3.u32;
  bool log = native::TilingLog();
  uint32_t state = log ? guest::LoadU32(base, block) : 0;
  if (log) {
    REXLOG_INFO("native renderer: tiling: worker job 822B6780({:08X}) on cpu {}: +0 {:08X} +4 "
                "{:08X}; state +4 {:08X} +396 {:08X}",
                block, native::Cpu(ctx, base), state, guest::LoadU32(base, block + 4),
                state ? guest::LoadU32(base, state + 4) : 0,
                state ? guest::LoadU32(base, state + 396) : 0);
  }
  __imp__sub_822B6780(ctx, base);
  if (log) {
    REXLOG_INFO("native renderer: tiling: worker job done on cpu {}: state +4 {:08X} +396 {:08X}",
                native::Cpu(ctx, base), state ? guest::LoadU32(base, state + 4) : 0,
                state ? guest::LoadU32(base, state + 396) : 0);
  }
}

// The library's deferred kick: r3 device, r4 returned as is, r5 address,
// r6 size in words, r7 interrupts the buffer will raise, r8 the list of
// deferred kicks (device +13036). While interrupts are outstanding (device
// +10868) it queues the buffer for the worker thread (sub_822B69A8, woken
// by sub_822B6A78) to kick once the GPU reaches them; otherwise it adds r7
// and kicks. The GPU's lag behind the CPU is why it exists. In exclusive
// mode every kick completes at once, so nothing is ever deferred: kick
// straight away and leave the count at 0. (Delivering the interrupts at
// the kick instead raced the worker against the render thread and drove
// the count below 0, hanging loading screens.)
extern "C" REX_FUNC(sub_8229E848) {
  REX_FUNC_PROLOGUE();
  if (!native::Renderer_Exclusive()) {
    __imp__sub_8229E848(ctx, base);
    return;
  }
  uint32_t device = ctx.r3.u32, result = ctx.r4.u32;
  PPCContext saved = ctx;
  {
    guest::StackFrame frame(ctx, 96);
    uint32_t pair = frame.Local(80);
    guest::StoreU32(base, pair, saved.r6.u32);      // size
    guest::StoreU32(base, pair + 4, saved.r5.u32);  // address
    ctx.r3.u32 = device;
    ctx.r4.u32 = pair;
    ctx.r5.u32 = 1;
    sub_8229E3F0(ctx, base);
  }
  ctx = saved;
  ctx.r3.u32 = result;
}
