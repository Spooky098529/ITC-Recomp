// The car model's file-loaded callbacks as plain C++ (B7 in
// docs/rexglue-removal.md; docs/functions/car-models.md):
// CarModel::OnFileLoaded (sub_820D47A8, set a's) and OnFileLoadedB
// (sub_820D4F80, set b's), the same code but for two strings, where the
// XPR2 comes from and the buffer's release. Guest data shared with the
// recompiled code, so these work on guest memory with the originals' loads
// and stores in their order, their frame's words included. Adapters:
// src/hooks/car_models.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "car_models/plate.h"

namespace car_models {

constexpr uint32_t kFileLoadedFrameBytes = 1552;

enum class FileLoadedSet { kA, kB };

// The guest calls the callbacks make: the originals of Skeleton::Bind
// (r5 = 0), Skeleton::UpdateMatrices and XPR::Create where their plain C++
// declines, the texture provider's vtable slot +16 (r4 = 0, r5 = the XPR),
// and MakePlate's Resource::Release; each from the callback's frame with
// the original's return address.
struct FileLoadedCalls {
  std::function<void(uint32_t skeleton, uint32_t file)> skeleton_bind;
  std::function<void(uint32_t skeleton)> update_matrices;
  std::function<void(uint32_t xpr, uint32_t file)> xpr_create;
  std::function<void(uint32_t provider, uint32_t xpr)> provider_set;
  ReleaseCall plate_release;
};

// Whether the callback runs natively for this request: not for the first
// texture set's (+3956) when +3724 is 1 (sub_820C4E68's image drawn into the
// sheet, not mapped: the original's; it runs in play), nor, for set a, when that
// set's buffer (+3968 + 24i) isn't pages the allocation layer gave.
bool FileLoadedNative(uint8_t* base, FileLoadedSet set, uint32_t request, uint32_t car);

// sub_820D47A8 / sub_820D4F80: r3 = the request's serial, r4 = its buffer,
// r6 = the car model. The car's name ([0x826018C8 + 4 x +32]) with
// "_inte_off" (set a) or "_inte_on" (set b) into the frame (+384, sprintf).
// One of the two texture sets (+3956 + 24i): its XPR2 (set a: the slot's
// buffer +3968; set b: `buffer`) made into the XPR object +4000 + 24i
// (XPR::Create), handed to the texture provider +4048 + 20i (its slot +16),
// the state +3972 = 3 and the buffer word +3968 = 0 (set a frees it first);
// for set 0 also the plate (MakePlate into +6700 to +6712, the struct at
// +3664) and the indices of the name and of name + "_inte_off_0" /
// "_inte_on_0" + 1 to 5 in the XPR (+7232 to +7252). Or the node file
// (+3732): the skeleton +3760 bound (Skeleton::Bind, ResetPose,
// LocalMatrices, UpdateMatrices), +3748 = 3, the tires', seats' and
// accessory's nodes found by name (matrices copied to +6720, +6784, +6848,
// +6912, +7040, +7104; the accessory's address to +7200), then their
// positions scaled by 0.1 and turned a quarter about y (+6976 to +7036,
// with the seats' +7168 to +7196 when all seven were found, the
// accessory's +7216). `stack` is the guest r1 at entry (the frame is
// `stack` - 1552). Only where FileLoadedNative, the allocation layer and
// memcpy are native (the adapter's checks).
void FileLoaded(uint8_t* base, FileLoadedSet set, uint32_t request, uint32_t buffer,
                uint32_t car, uint32_t stack, const FileLoadedCalls& calls);

}  // namespace car_models
