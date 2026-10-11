#include "state_capture.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include "guest.h"

#include <algorithm>
#include <bit>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

REXCVAR_DEFINE_STRING(capture_dir, "", "ITC",
                      "Folder to record game state into each tick; empty disables");

namespace debug {
namespace {

constexpr uint32_t kVehicleWindowSize = 0x2000;
constexpr uint32_t kReferrerWindowBefore = 0x200;
constexpr uint32_t kReferrerWindowAfter = 0x200;
constexpr size_t kMaxReferrersPerVehicle = 32;
constexpr size_t kMaxVehicles = 16;

constexpr int kTicksBetweenScans = 300;  // 10 s
constexpr int kBurstTicks = 60;          // 2 s

// Guest addresses that can hold game objects: below the physical memory
// views at 0xA0000000.
constexpr uint64_t kGuestScanEnd = 0xA0000000;

constexpr uint32_t kRecordMagic = 0x49544331;  // "ITC1"

// The vehicle's world matrix (rotation rows, then position row).
constexpr uint32_t kVehicleWorldMatrixOffset = 0xD0;
constexpr uint32_t kMatrixSize = 64;

// Matrices positioned this far from the car are camera candidates.
constexpr float kNearbyMinDistance = 0.3f;
constexpr float kNearbyMaxDistance = 25.0f;
constexpr size_t kMaxNearbyMatrices = 512;
constexpr size_t kMaxNearbyPoints = 4096;
constexpr uint32_t kPointSize = 16;

bool IsHeapPointer(uint32_t address) {
  return address >= 0x30000000 && address < 0xA0000000;
}

struct Region {
  uint32_t address;
  uint32_t size;
};

struct Referrer {
  uint32_t vehicle;
  uint32_t address;  // where the pointer to the vehicle is stored
};

class StateCapture {
 public:
  void OnVehicleSeen(uint32_t vehicle) {
    if (!IsHeapPointer(vehicle)) {
      return;
    }
    std::lock_guard lock(mutex_);
    if (vehicles_.size() < kMaxVehicles &&
        std::find(vehicles_.begin(), vehicles_.end(), vehicle) == vehicles_.end()) {
      vehicles_.push_back(vehicle);
      REXLOG_INFO("capture: vehicle {:08x}", vehicle);
    }
  }

  void OnTickDone(uint8_t* base) {
    if (!Open()) {
      return;
    }
    std::vector<uint32_t> vehicles;
    {
      std::lock_guard lock(mutex_);
      vehicles = vehicles_;
    }
    if (vehicles.empty()) {
      return;
    }

    if (ticks_until_scan_ <= 0) {
      ScanForReferrers(base, vehicles);
      ScanForNearbyMatrices(base, vehicles[0]);
      ticks_until_scan_ = kTicksBetweenScans;
      burst_ticks_left_ = kBurstTicks;
    }
    ticks_until_scan_--;

    std::vector<Region> regions;
    for (uint32_t vehicle : vehicles) {
      regions.push_back({vehicle, kVehicleWindowSize});
    }
    if (burst_ticks_left_ > 0) {
      burst_ticks_left_--;
      for (const Referrer& referrer : referrers_) {
        regions.push_back({referrer.address - kReferrerWindowBefore,
                           kReferrerWindowBefore + kReferrerWindowAfter});
      }
    }
    // Small, so recorded every tick to catch camera switches.
    for (uint32_t matrix : nearby_matrices_) {
      regions.push_back({matrix, kMatrixSize});
    }
    for (uint32_t point : nearby_points_) {
      regions.push_back({point, kPointSize});
    }
    WriteRecord(base, regions);
    tick_++;
  }

 private:
  bool Open() {
    if (failed_) {
      return false;
    }
    if (out_.is_open()) {
      return true;
    }
    const std::string& dir = REXCVAR_GET(capture_dir);
    if (dir.empty()) {
      failed_ = true;
      return false;
    }
    std::filesystem::create_directories(dir);
    auto path = std::filesystem::path(dir) / "capture.bin";
    out_.open(path, std::ios::binary | std::ios::trunc);
    if (!out_) {
      REXLOG_ERROR("capture: can't open {}", path.string());
      failed_ = true;
      return false;
    }
    REXLOG_INFO("capture: writing {}", path.string());
    start_ = std::chrono::steady_clock::now();
    return true;
  }

  // Guest address ranges that are readable, from the host's memory map.
  static std::vector<Region> ReadableGuestRanges(uint8_t* base) {
    std::vector<Region> ranges;
    std::ifstream maps("/proc/self/maps");
    std::string line;
    uint64_t guest_start = reinterpret_cast<uint64_t>(base);
    while (std::getline(maps, line)) {
      std::istringstream fields(line);
      std::string span, perms;
      fields >> span >> perms;
      if (perms.empty() || perms[0] != 'r') {
        continue;
      }
      uint64_t start = std::stoull(span.substr(0, span.find('-')), nullptr, 16);
      uint64_t end = std::stoull(span.substr(span.find('-') + 1), nullptr, 16);
      if (end <= guest_start || start >= guest_start + kGuestScanEnd) {
        continue;
      }
      start = std::max(start, guest_start);
      end = std::min(end, guest_start + kGuestScanEnd);
      ranges.push_back({uint32_t(start - guest_start), uint32_t(end - start)});
    }
    return ranges;
  }

  void ScanForReferrers(uint8_t* base, const std::vector<uint32_t>& vehicles) {
    auto started = std::chrono::steady_clock::now();
    referrers_.clear();
    std::vector<uint32_t> wanted;  // big-endian, as stored in guest memory
    for (uint32_t vehicle : vehicles) {
      wanted.push_back(__builtin_bswap32(vehicle));
    }
    std::vector<size_t> found(vehicles.size(), 0);
    for (const Region& range : ReadableGuestRanges(base)) {
      auto* words = reinterpret_cast<const uint32_t*>(base + range.address);
      size_t count = range.size / 4;
      for (size_t i = 0; i < count; i++) {
        uint32_t word = words[i];
        for (size_t v = 0; v < wanted.size(); v++) {
          if (word == wanted[v] && found[v] < kMaxReferrersPerVehicle) {
            uint32_t address = range.address + uint32_t(i * 4);
            // Skip pointers inside the vehicle's own recorded window.
            if (address - vehicles[v] < kVehicleWindowSize) {
              continue;
            }
            if (address < kReferrerWindowBefore) {
              continue;
            }
            referrers_.push_back({vehicles[v], address});
            found[v]++;
          }
        }
      }
    }
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - started)
                  .count();
    REXLOG_INFO("capture: tick {} found {} referrers in {} ms", tick_, referrers_.size(), ms);
  }

  static bool IsRotation(const float* rows) {
    // Three rows of a 4x4 matrix, 4 floats apart, must be orthonormal.
    for (int i = 0; i < 3; i++) {
      for (int j = i; j < 3; j++) {
        float dot = 0;
        for (int k = 0; k < 3; k++) {
          dot += rows[i * 4 + k] * rows[j * 4 + k];
        }
        float expected = i == j ? 1.0f : 0.0f;
        if (!(std::abs(dot - expected) < 0.01f)) {
          return false;
        }
      }
    }
    return true;
  }

  static float LoadMatrixFloat(const uint32_t* words, size_t index) {
    return std::bit_cast<float>(__builtin_bswap32(words[index]));
  }

  static float DistanceSquared(const float* a, const float* b) {
    float d2 = 0;
    for (int k = 0; k < 3; k++) {
      d2 += (a[k] - b[k]) * (a[k] - b[k]);
    }
    return d2;
  }

  static bool IsNear(const float* p, const float* car) {
    float d2 = DistanceSquared(p, car);
    return d2 > kNearbyMinDistance * kNearbyMinDistance &&
           d2 < kNearbyMaxDistance * kNearbyMaxDistance;
  }

  // Whether a 4x4 rotation+translation matrix places something near the car,
  // read either as a world transform or as a view matrix (whose implied
  // camera position is the inverse transform of the origin), in row-major
  // or column-major layout.
  static bool MatrixIsNear(const float* m, const float* car) {
    float row_t[3] = {m[12], m[13], m[14]};
    float col_t[3] = {m[3], m[7], m[11]};
    float row_view_camera[3];
    float col_view_camera[3];
    for (int k = 0; k < 3; k++) {
      row_view_camera[k] = -(row_t[0] * m[k * 4 + 0] + row_t[1] * m[k * 4 + 1] +
                             row_t[2] * m[k * 4 + 2]);
      col_view_camera[k] = -(m[0 * 4 + k] * col_t[0] + m[1 * 4 + k] * col_t[1] +
                             m[2 * 4 + k] * col_t[2]);
    }
    return IsNear(row_t, car) || IsNear(col_t, car) || IsNear(row_view_camera, car) ||
           IsNear(col_view_camera, car);
  }

  // Finds 4x4 matrices (world or view) that place something near the car
  // but not on it, and loose 16-byte-aligned positions near the car.
  void ScanForNearbyMatrices(uint8_t* base, uint32_t vehicle) {
    auto started = std::chrono::steady_clock::now();
    nearby_matrices_.clear();
    nearby_points_.clear();
    uint32_t car_matrix = vehicle + kVehicleWorldMatrixOffset;
    float car[3];
    for (int k = 0; k < 3; k++) {
      car[k] = guest::LoadFloat(base, car_matrix + 48 + 4 * k);
    }
    for (const Region& range : ReadableGuestRanges(base)) {
      auto* words = reinterpret_cast<const uint32_t*>(base + range.address);
      size_t count = range.size / 4;
      for (size_t i = 0; i + 16 <= count; i += 4) {
        uint32_t address = range.address + uint32_t(i * 4);
        if (address - vehicle < kVehicleWindowSize) {
          continue;  // already recorded as part of the vehicle
        }
        float point[3] = {LoadMatrixFloat(words, i), LoadMatrixFloat(words, i + 1),
                          LoadMatrixFloat(words, i + 2)};
        if (nearby_points_.size() < kMaxNearbyPoints && IsNear(point, car)) {
          nearby_points_.push_back(address);
        }
        // Matrices have 1.0 in the last element in both layouts.
        if (words[i + 15] != __builtin_bswap32(0x3F800000) ||
            nearby_matrices_.size() >= kMaxNearbyMatrices) {
          continue;
        }
        float m[16];
        for (int k = 0; k < 16; k++) {
          m[k] = LoadMatrixFloat(words, i + k);
        }
        if (!IsRotation(m) || !MatrixIsNear(m, car)) {
          continue;
        }
        nearby_matrices_.push_back(address);
      }
    }
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::steady_clock::now() - started)
                  .count();
    REXLOG_INFO("capture: tick {} found {} nearby matrices and {} points in {} ms", tick_,
                nearby_matrices_.size(), nearby_points_.size(), ms);
  }

  void Write32(uint32_t value) {
    out_.write(reinterpret_cast<const char*>(&value), sizeof(value));
  }

  void WriteRecord(uint8_t* base, const std::vector<Region>& regions) {
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::steady_clock::now() - start_)
                  .count();
    Write32(kRecordMagic);
    Write32(tick_);
    Write32(uint32_t(ns / 1000));  // microseconds since capture start
    Write32(uint32_t(regions.size()));
    for (const Region& region : regions) {
      Write32(region.address);
      Write32(region.size);
      out_.write(reinterpret_cast<const char*>(base + region.address), region.size);
    }
    // Which pointer each referrer window belongs to, for the reader.
    if (burst_ticks_left_ + 1 == kBurstTicks) {
      std::ofstream index(std::filesystem::path(REXCVAR_GET(capture_dir)) / "referrers.txt",
                          std::ios::app);
      for (const Referrer& referrer : referrers_) {
        char line[64];
        std::snprintf(line, sizeof(line), "%u %08x %08x\n", tick_, referrer.vehicle,
                      referrer.address);
        index << line;
      }
    }
  }

  std::mutex mutex_;
  std::vector<uint32_t> vehicles_;

  std::ofstream out_;
  bool failed_ = false;
  std::chrono::steady_clock::time_point start_;
  uint32_t tick_ = 0;
  int ticks_until_scan_ = 0;
  int burst_ticks_left_ = 0;
  std::vector<Referrer> referrers_;
  std::vector<uint32_t> nearby_matrices_;
  std::vector<uint32_t> nearby_points_;
};

StateCapture& GetStateCapture() {
  static StateCapture capture;
  return capture;
}

}  // namespace

void StateCapture_OnVehicleSeen(uint32_t vehicle) {
  if (REXCVAR_GET(capture_dir).empty()) {
    return;
  }
  GetStateCapture().OnVehicleSeen(vehicle);
}

void StateCapture_OnTickDone(uint8_t* base) {
  if (REXCVAR_GET(capture_dir).empty()) {
    return;
  }
  GetStateCapture().OnTickDone(base);
}

}  // namespace debug
