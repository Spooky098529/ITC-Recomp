// ShowStandCar's world for the car list's harness (car_list.cpp), laid as
// load_poll's re-applies lay their PanelSetBase::ShowStandCar cases
// (implemented in load_poll.cpp, beside them): the scene models' block (the
// stand's holder and stage, the scene's pool 7 model polled as the
// re-applies' world has it) and the car block (LayShowCarWorld's spec and
// parameters, the holder's word), so the car list's functions that reach
// ShowStandCar (step 17 through SetUpColors, the car's page) run it for
// real inside the car list's own world.

#pragma once

#include "harness.h"

#include <cstdint>
#include <optional>
#include <vector>

namespace harness {

class StandCarWorld {
 public:
  explicit StandCarWorld(uint8_t* base);
  ~StandCarWorld();
  StandCarWorld(const StandCarWorld&) = delete;
  StandCarWorld& operator=(const StandCarWorld&) = delete;
  // Whether its blocks were allocated and the callees' plain C++ is native
  // (the re-applies' conditions).
  bool ok() const;
  // Lays a case from `r`: both blocks, the stack from `sp` - 0x6000 to
  // `sp` - 1024 random (ShowStandCar's frame and its callees'), the car
  // list entry's car id (`entry` +8432) the world's car; points the scene
  // ([0x826C2E44]), the accessories' set, the parameters, the stand's
  // holders ([0x826D83E0], its three slots the world's holder), the stand's
  // car list ([0x826D83F4]) and the rand seed at the world's; records the
  // archive's loads until Settle.
  void Lay(Random& r, uint32_t entry, uint32_t sp);
  // After the run: the rand seed left, the block (the vinyl sheet a run
  // made by kind), the car block, the car on ShowStandCar's frame (entered
  // with `stand_sp`; when `frame_car`) and the loads; what the run made
  // freed, the globals put back.
  void Settle(Outputs& out, uint32_t stand_sp, bool frame_car = true);

 private:
  uint8_t* base_;
  uint32_t allocation_, block_, car_allocation_, car_block_;
  std::vector<std::vector<uint8_t>> saved_;
  std::optional<ArchiveLoads> loads_;
  uint32_t old_header_ = 0;
};

}  // namespace harness
