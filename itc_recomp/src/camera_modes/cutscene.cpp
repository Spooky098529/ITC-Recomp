// The cutscene's constructor (docs/functions/camera-modes.md):
// Cutscene::Cutscene's set-up, its racers' records, the script's choice, the
// rival logo's choice and the logo; the scripts themselves are
// CutsceneScripts (cutscene_scripts.cpp). Plain C++ over guest memory, the
// original's loads and stores in its order, its stack frame's records
// written where it writes them (the frame's back chain and saved registers
// aren't written).

#include "camera_modes/camera_modes.h"

#include "camera_modes/steps.h"

#include "base/log.h"
#include "cameras/cameras.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "track_collision/track_collision.h"
#include "vehicle_accessors/vehicle_accessors.h"
#include "schema/camera_modes.h"
#include "schema/minimap.h"
#include "schema/race_event.h"
#include "schema/race_states.h"
#include "schema/race_world.h"
#include "schema/video_output.h"

#include <algorithm>
#include <iterator>
#include <optional>

namespace camera_modes {

using namespace steps;

namespace {

using G = schema::CameraModesGlobals;

constexpr uint32_t kOne = G::one_address();                // 1.0
constexpr uint32_t kDegrees = G::chase_degrees_address();  // the zoom's angle (55.0)
constexpr uint32_t kVtable = schema::Cutscene::kVtable;    // slot 0 Cutscene::Update, 1 OnPad
constexpr uint32_t kRacersManager = G::racers_manager_address();  // the racers' manager (a pointer)

uint32_t Load(uint8_t* base, uint32_t a) { return guest::LoadU32(base, a); }
void Store(uint8_t* base, uint32_t a, uint32_t v) { guest::StoreU32(base, a, v); }

// A 16-slot list's reset (List16::Reset, inlined in the original).
void ResetList16(uint8_t* base, uint32_t list) {
  for (uint32_t i = 0; i < 16; i++) {
    guest::StoreU16(base, list + 4 * i + 2, uint16_t(i));
    guest::StoreU16(base, list + 4 * i + 0, uint16_t(i + 1));
  }
  const schema::List16 l(base, list);
  guest::StoreU16(base, l.nodes_address() + 60, 0xFFFF);
  l.set_head(0xFFFF);
  l.set_unknown_70(0xFFFF);
  l.set_free(0);
  l.set_unknown_66(0xFFFF);
  l.set_count(0);
}

// The script (+352) for the first record's id (the event), as the original's
// compare tree and its first jump table (0x82009380, bytes, events 386 to
// 399) give it; nullopt where +352 is left as it is.
std::optional<uint32_t> ScriptOfEvent(int32_t event) {
  if (event > 385) {
    static constexpr uint8_t kTable[] = {19, 19, 19, 14, 15, 27, 20, 23, 24, 25, 21, 22, 28, 29};
    const uint32_t key = uint32_t(event - 386);
    if (key > 13) {
      return std::nullopt;
    }
    return kTable[key];
  }
  switch (event) {
    case 385:
      return 18;
    case 384:
      return 17;
    case 383:
      return 16;
    case 382:
      return 26;
    case 381:
      return 13;
    case 380:
      return 12;
    case 379:
      return 11;
    case 378:
      return 10;
    case 377:
      return 9;
    case 376:
      return 8;
    case -1:
      return 0;
    case 27:
    case 28:
      return 6;
    case 181:
    case 182:
      return 7;
    default:
      return std::nullopt;
  }
}

// Script 1's texture (the logo 0's) for the event: its place in this list
// (the original's compare tree), else -1.
constexpr int16_t kTextureEvents[] = {
    0,   5,   12,  20,  23,  29,  36,  44,  47,  53,  58,  65,  72,  78,  83,  92,
    96,  104, 115, 119, 126, 133, 142, 150, 156, 159, 168, 175, 186, 195, 200, 210,
    217, 222, 228, 232, 241, 245, 253, 262, 272, 277, 282, 289, 297, 304};

uint32_t TextureOfEvent(int32_t event) {
  const auto* end = std::end(kTextureEvents);
  const auto* at = std::find(std::begin(kTextureEvents), end, event);
  return at == end ? 0xFFFFFFFFu : uint32_t(at - std::begin(kTextureEvents));
}

// The logo for scripts 1 to 28 (the third jump table, 0x82009320, bytes,
// keyed by the script - 1): -1 none; script 1's is 0 (with the event's
// texture).
constexpr int8_t kLogos[] = {0,  3,  -1, -1, -1, 1,  2,  4,  5,  6,  7,  8,  9,  18,
                             19, 15, 16, 17, 20, 10, 11, 12, 22, 23, 24, 13, 21, 14};

}  // namespace

uint32_t CutsceneConstruct(uint8_t* base, uint32_t cutscene, uint32_t viewport, uint32_t vehicle,
                           uint32_t frame, const CutsceneConstructCalls& calls) {
  ITC_NATIVE_RUNNING("sub_82156CC0 Cutscene::Cutscene");
  const schema::Cutscene c(base, cutscene);
  const schema::RivalLogo rival(base, c.rival_logo_address());
  const uint32_t f = frame;
  uint32_t r23 = cutscene;

  // --- (1) The set-up ---
  const uint32_t tracker = c.tracker_address();
  Store(base, f + 1652, cutscene);  // the caller's frame's +20, as the original keeps it
  const double one = Lfs(base, kOne);
  c.set_vehicle(0);
  c.set_viewport(0);
  for (uint32_t at = 12; at <= 220; at += 16) {
    Stfs(base, tracker + at, one);
  }
  for (uint32_t at : {252u, 268u, 284u, 300u}) {
    Stfs(base, tracker + at, one);
  }
  c.set_vtable(kVtable);
  game_modes::XprHolderConstruct(base, rival.holder_address());
  c.set_vehicle(vehicle);
  c.set_viewport(viewport);
  // The zoom: the video output's width (an integer, kept as a doubleword at
  // f + 768 as the original's conversion does), Camera::ZoomFromFov.
  const uint32_t video = calls.video_output(f, 0x82156D74);
  const double degrees = Lfs(base, kDegrees);
  const int32_t width = int32_t(schema::VideoOutput(base, video).display_width());
  guest::StoreU32(base, f + 768, uint32_t(uint64_t(int64_t(width)) >> 32));
  guest::StoreU32(base, f + 772, uint32_t(int64_t(width)));
  const double zoom = ZoomFromFov(base, degrees, Single(double(int64_t(width)))).value();
  cameras::ViewportSetZoom(base, c.viewport().address, zoom);
  {
    const uint32_t v = c.viewport().address;
    const double ratio =
        Fdivs(double(schema::Viewport(base, v).near()), double(schema::Viewport(base, v).zoom()));
    cameras::SetNear(base, Fmuls(ratio, zoom));
  }
  TargetTrack(base, tracker, vehicle, f - 256);
  c.set_frame(0);
  c.set_shot(0);
  c.set_script(0xFFFFFFFFu);
  ResetList16(base, c.cues_address());   // the fade cues (32 bytes each)
  ResetList16(base, c.shots_address());  // the shots (608 bytes each)
  c.set_unknown_10756(0);
  calls.free_all();
  AnimsClear(base);

  // --- (2) The racers' records ---
  // Four 8-byte records at f + 728 (+0 the item id's halfword, -1; +2 the
  // item's byte +35, -1; +4 the vehicle, 0), their count at f + 760, -1 at
  // f + 764, the last item's halfword +88 at f + 724 (-1).
  guest::StoreU16(base, f + 724, 0xFFFF);
  for (uint32_t i = 0; i < 4; i++) {
    const uint32_t record = f + kCutsceneRecords + 8 * i;
    guest::StoreU16(base, record + 0, 0xFFFF);
    guest::StoreU8(base, record + 2, 0xFF);
    Store(base, record + 4, 0);
  }
  uint32_t r19 = 0;  // the count as the register holds it
  Store(base, f + 764, 0xFFFFFFFFu);
  Store(base, f + kCutsceneRecordCount, r19);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const schema::RaceData race(base, G::race_data(base));
  const uint32_t stages = race.stages_address();
  Store(base, f + 720, career::StageOf(base, save));
  {
    const uint32_t stage = race.current_stage();
    if (!vehicle_accessors::StagesCopy(base, f + 1392, stages, stage)) {
      calls.stages_copy(f + 1392, stages, stage);
    }
  }
  // The first record's vehicle as the original's r9 holds it (read by the
  // scripts).
  uint32_t r9 = Load(base, f + 732);
  for (int32_t s = 0; s < int32_t(Load(base, f + 1468)); s++) {
    const uint32_t racer = Load(base, f + 1436 + 8 * uint32_t(s));
    const uint32_t record = uint32_t(uint64_t(racer) * 3936) + G::race_data(base);
    if (((schema::RaceData(base, record).record0_flags() >> 1) & 1) == 0) {
      continue;
    }
    // The racers' manager's racer with the same id: its vehicle and flag.
    uint32_t r20 = 0;
    bool flagged = false;
    for (int32_t n = 0; n < int32_t(track_collision::RaceRacerCount(base)); n++) {
      const uint32_t entry = Load(base, schema::RaceManager(base, Load(base, kRacersManager)).racers_address() + 4 * uint32_t(n));
      if (int32_t(racer) == int32_t(schema::MinimapRacerEntry(base, entry).record())) {
        r20 = vehicle_accessors::VehicleGet(base, schema::MinimapRacerEntry(base, entry).vehicle_index());
        flagged = (schema::MinimapRacerEntry(base, entry).flags() & 0x400) != 0;
        break;
      }
    }
    // Its item: byte +35 and halfword +88 (-1 without one).
    const uint32_t id = schema::RaceData(base, record).record0_racer_id();
    uint32_t r21 = 0xFFFFFFFFu, r24 = 0xFFFFFFFFu;
    if (int32_t(id) >= 0) {
      const uint32_t item = data_tables::Item(base, id);
      r21 = schema::OpponentItem(base, item).kind();
      r24 = schema::OpponentItem(base, item).race_kind();
    }
    // Appended (each index from the count as the original reads it).
    const uint16_t r10 = uint16_t(id);
    guest::StoreU16(base, f + kCutsceneRecords + (r19 << 3), r10);
    uint32_t r11 = Load(base, f + kCutsceneRecordCount);
    guest::StoreU8(base, f + kCutsceneRecords + 2 + (r11 << 3), uint8_t(r21));
    r11 = Load(base, f + kCutsceneRecordCount);
    Store(base, f + kCutsceneRecords + 4 + (r11 << 3), r20);
    guest::StoreU16(base, f + 724, uint16_t(r24));
    const int16_t kind = int16_t(uint16_t(r24));
    if (flagged && (kind == 1 || kind == 3)) {
      // Put first: the records moved up by one.
      r11 = Load(base, f + kCutsceneRecordCount);
      if (int32_t(r11) > 0) {
        uint32_t at = (r11 << 3) + f + kCutsceneRecords;
        int32_t k = int32_t(r11);
        do {
          const uint16_t h = guest::LoadU16(base, at - 8);
          const uint8_t b = guest::LoadU8(base, at - 6);
          const uint32_t w = Load(base, at - 4);
          guest::StoreU16(base, at + 0, h);
          guest::StoreU8(base, at + 2, b);
          Store(base, at + 4, w);
          at -= 8;
          k--;
        } while (k > 0);
        r11 = Load(base, f + kCutsceneRecordCount);
      }
      r9 = r20;
      guest::StoreU16(base, f + kCutsceneRecords + 0, r10);
      guest::StoreU8(base, f + kCutsceneRecords + 2, uint8_t(r21));
      Store(base, f + kCutsceneRecords + 4, r9);
    } else {
      r11 = Load(base, f + kCutsceneRecordCount);
      r9 = Load(base, f + 732);
    }
    r19 = r11 + 1;
    r23 = Load(base, f + 1652);
    Store(base, f + kCutsceneRecordCount, r19);
  }

  // --- (3a) The script ---
  const int32_t event = int16_t(guest::LoadU16(base, f + kCutsceneRecords));
  if (const auto script = ScriptOfEvent(event)) {
    schema::Cutscene(base, r23).set_script(*script);
  }
  if (int32_t(schema::Cutscene(base, r23).script()) == -1) {
    // By the first record's byte (sign-extended, compared unsigned).
    const uint32_t byte = uint32_t(int32_t(int8_t(guest::LoadU8(base, f + kCutsceneRecords + 2))));
    if (byte < 1) {
      schema::Cutscene(base, r23).set_script(0);
    } else if (byte == 1) {
      schema::Cutscene(base, r23).set_script(2);
    } else if (byte < 3) {
      schema::Cutscene(base, r23).set_script(1);
    }
  }
  const uint32_t script = schema::Cutscene(base, r23).script();
  if (int32_t(script) == -1) {
    return r23;
  }

  // --- (4) The scripts ---
  if (script <= 29) {
    r23 = CutsceneScripts(base, r23, script, r9, f, calls.scripts);
  }

  // --- (3b) The logo's choice ---
  uint32_t logo = 0xFFFFFFFFu, texture = 0xFFFFFFFFu;
  const uint32_t key = schema::Cutscene(base, r23).script() - 1;
  if (key <= 27) {
    logo = uint32_t(int32_t(kLogos[key]));
    if (key == 0) {
      texture = TextureOfEvent(event);
    }
  }

  // --- (5) The logo ---
  const uint32_t entry = schema::Cutscene(base, r23).rival_logo_address();
  RivalLogoConstruct(base, entry);
  RivalLogoOpen(base, entry, logo, texture, calls.open, calls.load_xpr);
  return r23;
}

}  // namespace camera_modes
