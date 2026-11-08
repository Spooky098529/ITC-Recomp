// The car dealer's page transitions (docs/functions/car-dealer.md): see
// car_dealer.h. Each is called by CarDealer::Update with the page object's
// +24 (the started word) and answers whether it's done.

#include "car_dealer/car_dealer.h"

#include "base/log.h"
#include "car_dealer/internal.h"
#include "guest.h"

namespace car_dealer {

namespace {

// The screen fades' counts (mode-tick.md).
constexpr uint32_t kFadeOutCount = 0x826A50E4;
constexpr uint32_t kFadeInCount = 0x826A50DC;
// ScreenFades::FadeOut's layer word.
constexpr uint32_t kFadeOutLayer = 0x82001BF0;

}  // namespace

uint64_t Transition3(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82181D68 CarDealer::Transition3");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, 4, frame + 80);
  if (Load(base, step) == 0) {
    calls.Call(kFadeOut, frame, 0x82181DC4, Load(base, kFadeOutLayer), 0, 0, 0);
    Store(base, step, 1);
  }
  calls.Call(kRun, frame, 0x82181DD8, movie, 0);
  return (Load(base, kFadeOutCount) | Load(base, kFadeInCount)) == 0 ? 1 : 0;
}

}  // namespace car_dealer
