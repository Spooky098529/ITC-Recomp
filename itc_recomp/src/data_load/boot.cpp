// The data load itself and the two UI movie sets it makes (data_load.h,
// docs/functions/data-load.md).

#include "data_load/data_load.h"

#include "base/log.h"
#include "guest.h"
#include "memory/c_runtime_heap.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace data_load {

namespace {

std::optional<int64_t> MovieListAdd(uint8_t* base, uint32_t list, uint32_t id,
                                    const MovieCalls& calls) {
  const auto made = memory::crt::OperatorNew(base, kMovieBytes);
  if (!made) {
    return std::nullopt;
  }
  uint32_t movie = *made;
  if (movie != 0) {
    movie = calls.construct(movie);
  }
  ui_movies::InitAnimationLists(base, movie);
  const int64_t serial = ui_movies::MovieLoad(base, movie, id, calls.load);
  const uint32_t count = guest::LoadU32(base, list + 64);
  guest::StoreU32(base, list + 64, count + 1);
  guest::StoreU32(base, list + 4 * count, movie);
  return serial;
}

}  // namespace

std::optional<int64_t> LoadingScreenAddMovie(uint8_t* base, uint32_t id, const MovieCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82247AA0 LoadingScreen::AddMovie");
  return MovieListAdd(base, kLoadingMovies, id, calls);
}

std::optional<int64_t> OtherMoviesAdd(uint8_t* base, uint32_t id, const MovieCalls& calls) {
  ITC_NATIVE_RUNNING("sub_822912A0 (the other UI movie set's add)");
  return MovieListAdd(base, kOtherMovies, id, calls);
}

int64_t LoadingScreenLoad(uint8_t* base, const MovieAdd& add) {
  ITC_NATIVE_RUNNING("sub_82247B98 LoadingScreen::Load");
  guest::StoreU32(base, kLoadingMovies + 64, 0);
  guest::StoreU32(base, kLoadingShown, 0);
  add(22575);
  add(22574);
  return add(22576);
}

uint32_t OtherMoviesLoad(uint8_t* base, const MovieAdd& add,
                         const std::function<uint32_t(int which)>& render_object) {
  ITC_NATIVE_RUNNING("sub_82291328 (the other UI movie set's load)");
  for (uint32_t i = 0; i < 3; i++) {
    add(guest::LoadU32(base, kOtherMovieIds + 4 * i));
  }
  for (uint32_t i = 0; i < 9; i++) {
    guest::StoreU32(base, kOtherWords + 4 * i, 0);
  }
  guest::StoreU32(base, kOtherWord8600, 0);
  guest::StoreU32(base, kOtherWord8130, 0);
  const uint32_t list = render_object(0);
  const uint32_t renderer = render_object(1);
  ui_movies::SetRenderer(base, renderer, list);
  return renderer;
}

uint32_t CreateObjects(uint8_t* base, const BootCall& call) {
  ITC_NATIVE_RUNNING("sub_82105868 BootData::CreateObjects");
  call(0x821057C8, 0, 0x8210587C);                    // SaveData::Create(0)
  call(0x82102208, std::nullopt, 0x82105880);         // RaceData::Create
  call(0x821021A8, std::nullopt, 0x82105884);         // RaceData::ResetGlobal
  call(0x820CCDC0, std::nullopt, 0x82105888);         // Defaults::Init
  call(0x82105508, std::nullopt, 0x8210588C);         // InputRecordings::Create
  call(0x82105560, std::nullopt, 0x82105890);         // Replay::Create
  call(0x82116DB0, std::nullopt, 0x82105894);         // DataTables::Init
  return call(0x82117240, std::nullopt, 0x82105898);  // DataTables::Load
}

std::optional<uint32_t> BootDataLoad(uint8_t* base, uint32_t frame, const BootCall& call) {
  ITC_NATIVE_RUNNING("sub_82106618 BootData::Load");
  if (guest::LoadU32(base, kLoaded) != 0) {
    return std::nullopt;
  }
  guest::StoreU32(base, kLoaded, 1);
  // GlobalMemoryStatus(frame + 80) (Dropped) left out: r3 its buffer, as
  // Shaders::LoadAll gets it.
  call(0x820A08A0, frame + 80, 0x82106648);  // Shaders::LoadAll
  call(0x8210ADF8, std::nullopt, 0x8210664C);  // Text::Load
  call(0x82105868, std::nullopt, 0x82106650);  // BootData::CreateObjects
  call(0x82247B98, std::nullopt, 0x82106654);  // LoadingScreen::Load
  call(0x82291328, std::nullopt, 0x82106658);  // the other movie set
  call(0x820C9960, std::nullopt, 0x8210665C);  // Params::Init
  call(0x820CC2A8, std::nullopt, 0x82106660);  // Params::Load
  // GlobalMemoryStatus(frame + 80) again (Dropped): r3 its buffer.
  return frame + 80;
}

}  // namespace data_load
