// TuningScreen::Update (sub_821E6C50, with the recompiler's pieces
// sub_821E6CD4 and sub_821E6F90 merged; docs/functions/tuning-screen.md's
// "The update"): see tuning_screen.h. The original keeps the screen in r31
// (all 64 bits of r3 at entry), the dispatcher in r28 (r4), the queue's
// current command in r26, movie M in r29 and its 144-byte frame at r1;
// every callee gets r3 = r31. The callees seen in play by their plain C++
// (as their adapters run them), DrawPowerGraph through its override (its
// adapter works through the guest), UiMovie::Run through its override, the
// debug state handlers all by their plain C++ (debug.cpp), the command
// runners never seen in play 5, 11, 12, 17, 19, 20, 26, 27, 42, 52 to 58 and
// Command32Done by their plain C++ (commands.cpp), the sticker commands 32
// to 41 and 43 to 51 by theirs with native_pages (tuning_pages'
// sticker_commands.cpp), and the other command runners never seen in play
// (below) through the guest with the original's return addresses. The empty
// sub_82106540's calls (the "TUNING SHOP" print, commands 24 and 28) are
// left out, as other native callers of it do.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"
#include "tuning_pages/tuning_pages.h"

namespace tuning_screen {

namespace {

constexpr uint32_t kFrameBytes = 144;
constexpr uint32_t kRefreshing = 9708;  // 1: run the refresh this update
constexpr uint32_t kQueue = 10036;      // [it]: the command queue
constexpr uint32_t kStandDirty = 9732;  // set: put the stand back this update
constexpr uint32_t kRun = 0x8211E1B8;   // UiMovie::Run(movie, 0)

// The command runners never seen in play (the command's 8-bit table at
// 0x8200A9B8): their address and the return address (0 for a native
// runner, run by the switch below; the sticker commands 32 to 51 natively
// with native_pages, else through the guest).
struct Guest {
  uint32_t function;
  uint32_t lr;
};

Guest CommandGuest(uint32_t command) {
  switch (command) {
    case 10: return {0x821F35C0, 0x821E7010};
    case 13: return {0x821EA460, 0x821E7034};
    case 14: return {0x821EAFA0, 0x821E7040};
    case 32: return {0x821FC7B8, 0x821E7124};
    case 33: return {0x821FCA80, 0x821E7130};
    case 34: return {0x821FA658, 0x821E713C};
    case 35: return {0x821FA9C0, 0x821E7148};
    case 36: return {0x821FAA28, 0x821E7154};
    case 37: return {0x821F8C30, 0x821E7160};
    case 38: return {0x821FAD00, 0x821E716C};
    case 39: return {0x821FB0B8, 0x821E7178};
    case 40: return {0x821FCC48, 0x821E7184};
    case 41: return {0x821FCED0, 0x821E7190};
    case 43: return {0x821FD128, 0x821E71A8};
    case 44: return {0x821FD3F8, 0x821E71B4};
    case 45: return {0x821FB1F0, 0x821E71C0};
    case 46: return {0x821F8FC0, 0x821E71CC};
    case 47: return {0x821FB450, 0x821E71D8};
    case 48: return {0x821F8FC0, 0x821E71E4};
    case 49: return {0x821FB6B0, 0x821E71F0};
    case 50: return {0x821F8FC0, 0x821E71FC};
    case 51: return {0x821FB8D8, 0x821E7208};
    default: return {0, 0};
  }
}

}  // namespace

uint64_t Update(uint8_t* base, const UpdateCalls& calls, uint64_t r3, uint64_t r4, uint32_t sp,
                std::optional<double>* f1) {
  ITC_NATIVE_RUNNING("sub_821E6C50 TuningScreen::Update");
  const StandCalls& stand = calls.stand;
  const GuestCall& call = stand.call;
  const uint32_t frame = sp - kFrameBytes;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  const uint32_t s = uint32_t(r3);
  const auto word = [&](uint32_t offset) { return guest::LoadU32(base, s + offset); };
  const auto guest_call = [&](uint32_t function, std::initializer_list<uint64_t> args,
                              uint32_t lr) {
    const uint64_t answer = call(function, args, frame, lr);
    f1->reset();  // as the guest call left it
    return answer;
  };

  // Block 1: the refresh while +9708 is 1, +9708 = whether it answered 0.
  if (int32_t(word(kRefreshing)) == 1) {
    const uint64_t answer = Refresh(base, stand, r3, frame, f1);
    guest::StoreU32(base, s + kRefreshing, uint32_t(answer) == 0 ? 1 : 0);
  }

  // Block 2: the state's debug handler (states 3 to 58).
  const uint32_t state = word(kState);
  if (state - 3 <= 55) {
    switch (state) {
      case 3: DebugMainMenu(base, frame); break;
      case 4: DebugCategoryMenu(base, s, frame); break;
      case 5: DebugLevelSelect(base, frame); break;
      case 6: DebugTypeSelect(base, frame); break;
      case 7: DebugColorSelect(base, s, frame); break;
      case 8: DebugOptionSelect(base, s, frame); break;
      case 9: DebugConfirmBuy(base, frame); break;
      case 10: DebugWarnMoney(base, frame); break;
      case 11: DebugConfirmEquip(base, frame); break;
      case 12: DebugReflectSet(base, frame); break;
      case 13: DebugAnnulSet(base, frame); break;
      case 14: DebugRemoveConfirm(base, frame); break;
      case 15: DebugWheelPartSelect(base, frame); break;
      case 16: DebugWheelKindSelect(base, frame); break;
      case 17: DebugWheelInchSelect(base, frame); break;
      case 18: DebugBodyColor(base, frame); break;
      case 19: DebugNeonPart(base, frame); break;
      case 20: DebugNeonKind(base, frame); break;
      case 21: DebugNeonEdit(base, frame); break;
      case 22: DebugLedEdit(base, frame); break;
      case 23: case 38: DebugDecalTopMenu(base, s, frame); break;
      case 24: case 39: DebugLayerSelect(base, s, frame); break;
      case 25: case 40: DebugLayerOperate(base, s, frame); break;
      case 26: case 41: DebugLayerEdit(base, frame); break;
      case 27: case 42: DebugLayerOrder(base, s, frame); break;
      case 28: case 43: DebugLayerSet(base, frame); break;
      case 29: case 44: DebugEditPosition(); break;
      case 30: case 45: DebugEditRotate(); break;
      case 31: case 46: DebugEditScale(); break;
      case 32: case 47: DebugEditColor(); break;
      case 33: case 48: DebugEditFlip(); break;
      case 34: case 49: DebugEditView(); break;
      case 35: case 50: DebugPaintConfirm(base, frame); break;
      case 36: case 51: DebugPaintCancelConfirm(base, frame); break;
      case 37: case 52: DebugPaintRemoveConfirm(base, frame); break;
      case 53: DebugCockpitTop(base, frame); break;
      case 54: DebugCockpitType(base, frame); break;
      case 55: DebugCockpitColor(base, frame); break;
      case 56: DebugMeterKind(base, frame); break;
      case 57: case 58: DebugMeterPoint(base, frame); break;
      default: break;
    }
  }

  // Block 3: the queue, its current command and the state's movie (13 for
  // 23 to 37, 14 for 38 to 52, for 61 13 with the vinyl else 14, else 2).
  const uint32_t queue = word(kQueue);
  const uint32_t command = guest::LoadU32(base, queue + 12);
  uint32_t id = 2;
  {
    const int32_t st = int32_t(word(kState));
    if (st >= 23 && st <= 37) {
      id = 13;
    } else if (st >= 23 && st <= 52) {
      id = 14;
    } else if (st == 61) {
      id = int32_t(word(kCategory)) == 5 && guest::LoadU8(base, s + kItem) == 6 ? 13 : 14;
    }
  }
  const uint32_t movie = guest::LoadU32(base, MovieEntry(base, frame, id));
  UpdateBusyIcon(base, stand, r3, frame);

  // Blocks 4 and 5: the command's runner on its first update, then the
  // movie run and the queue's step when it has stopped. r30 the original's
  // at DrawPowerGraph below: whether the run stopped, or the queue for no
  // command and those above 64.
  uint64_t r30 = queue;
  const auto step = [&] {
    const uint32_t q = word(kQueue);
    const uint32_t next = guest::LoadU32(base, q + 16);
    if (int32_t(next) == -1) {
      guest::StoreU32(base, q + 0, guest::LoadU32(base, q + 4));
      guest::StoreU32(base, q + 12, 0xFFFFFFFFu);
    } else {
      guest::StoreU32(base, q + 20, 0);
      guest::StoreU32(base, q + 12, next);
      guest::StoreU32(base, q + 16, 0xFFFFFFFFu);
    }
    guest::StoreU32(base, q + 24, 0);
  };
  if (int32_t(command) == -1) {
    guest_call(kRun, {movie, 0}, 0x821E7308);
  } else if (command > 64) {
    guest_call(kRun, {movie, 0}, 0x821E72C4);
    step();
  } else {
    if (int32_t(guest::LoadU32(base, queue + 24)) == 0) {
      guest::StoreU32(base, queue + 24, 1);
      const uint32_t dispatcher = uint32_t(r4);
      const Guest g = CommandGuest(command);
      if (g.function != 0 && calls.native_pages && tuning_pages::IsStickerCommand(command)) {
        tuning_pages::StickerCommand(base, call, command, r3, frame);
      } else if (g.function != 0) {
        guest_call(g.function, {r3}, g.lr);
      } else {
        switch (command) {
          case 0: PlayIntro(base, call, s, frame); break;
          case 1: MoveCategory(base, call, s, frame, calls.native_slots); break;
          case 2: OpenItems(base, call, s, frame); break;
          case 3: ShowLevels(base, call, s, frame); break;
          case 4: ShowTypes(base, call, s, frame, calls.native_specs); break;
          case 5: Command5(base, call, s, frame); break;
          case 6: ShowOptions(base, call, s, frame); break;
          case 7: Command7(base, call, s, frame); break;
          case 8: Command8(base, call, r3, frame); break;
          case 9: Command9(base, call, s, frame); break;
          case 11: Command11(base, call, s, frame); break;
          case 12: Command12(base, call, s, frame); break;
          case 15: Command15(base, call, s, frame); break;
          case 16: Command16(base, call, s, frame); break;
          case 17: Command17(base, call, s, frame); break;
          case 18: Command18(base, call, s, frame); break;
          case 19: Command19(base, call, s, frame); break;
          case 20: Command20(base, call, s, frame, word(9644), word(9648)); break;
          case 21: Command21(base, call, r3, frame, calls.native_pages); break;
          case 22: Command22(base, call, s, frame); break;
          case 23: Command23(base, call, s, frame); break;
          case 25: Command25(base, call, s, frame); break;
          case 26: Command26(base, call, s, frame); break;
          case 27: Command27(base, call, s, frame); break;
          case 29: {
            double decal_f1 = 0.0;
            DecalCommand(base, stand, s, frame, &decal_f1);
            *f1 = decal_f1;
            break;
          }
          case 30: Command30(base, call, r3, dispatcher, frame); break;
          case 31: Command31(base, call, s, frame); break;
          case 42: Command42(base, call, s, frame); break;
          case 52: Command52(base, call, r3, dispatcher, frame); break;
          case 53: Command53(base, s); break;
          case 54: Command54(base, call, s, frame); break;
          case 55: Command55(base, call, s, frame); break;
          case 56: Command56(base, call, s, frame); break;
          case 57: Command57(base, call, s, frame, f1); break;
          case 58: Command58(base, call, s, frame); break;
          case 63: ShowDialog(base, call, r3, dispatcher, frame); break;
          case 64: MarkPick(base, call, s, frame); break;
          default: break;  // 24 and 28 the empty sub_82106540, 59 to 62 none
        }
      }
    }
    const uint32_t done = uint32_t(guest_call(kRun, {movie, 0}, 0x821E7288)) == 0 ? 1 : 0;
    r30 = done;
    if (done == 1 && command == 32) {
      Command32Done(base, s, frame);
    }
    if (done != 0) {
      step();
    }
  }

  // Block 6: every update's calls, then the stand put back when asked.
  UpdateItems(base, call, r3, frame, calls.native_slots);
  calls.draw_power_graph(frame, r3, r30);
  f1->reset();
  RefreshMenu(base, call, s, frame, f1);
  UpdateListDots(base, s, frame);
  UpdateListCounter(base, call, s, frame);
  uint64_t answer = UpdateStand(base, stand, calls.draw, calls.paint_poll, s, frame);
  if (int32_t(word(kStandDirty)) != 0) {
    StoreToGarageCar(base, r3, frame);
    double stats_f1 = 0.0;
    SetCarStats(base, stand.calls, s, frame, &stats_f1);
    *f1 = stats_f1;
    answer = RestoreStand(base, stand, r3, frame, f1);
    guest::StoreU32(base, s + kStandDirty, 0);
  }
  return answer;
}

}  // namespace tuning_screen
