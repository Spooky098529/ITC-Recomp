// The action mappers as plain C++ (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/action-mappers.md): one mapper per player
// (2,896 bytes, vtable 0x82007F78) turning a logical pad's sticks and
// triggers and its listener's buttons into 30 actions (ids 151-180), the
// client list they're kept in, and their repeat and curve objects. The
// mappers are guest objects shared with the recompiled code (the menus, the
// pause menu and the car read them through the vtable), so these work on
// guest memory, with the originals' float steps (single rounding after each
// operation, as the translated code does; built with -ffp-model=strict and
// -ffp-contract=off, flush-to-zero off: the adapters' job). Adapters:
// src/hooks/action_mappers.cpp.
//
// The calls the originals make through a repeat or curve object's vtable
// (slot 1) are made natively only for the functions seen in play
// (ActionRepeat::Update, sub_8210B568, and ActionCurve::Apply,
// sub_8210B5B8): the functions that make them check every object first
// (MapperCallsKnown) and the adapter runs the original when one isn't.

#pragma once

#include <cstdint>

namespace action_mappers {

// The functions the objects' vtable slot 1 points at in play.
constexpr uint32_t kRepeatUpdateFunction = 0x8210B568;
constexpr uint32_t kCurveApplyFunction = 0x8210B5B8;

// The mapper's fields (docs/known-data.md, "action mapper").
constexpr uint32_t kActions = 30;
constexpr uint32_t kWords = 448;          // held, held before, pressed, released, fired, pending
constexpr uint32_t kConfig = 372;         // four pointers: kinds, links, analog kinds, links
constexpr uint32_t kRaw = 388;            // 30 u16s: the raw values
constexpr uint32_t kRepeatPointers = 472; // 30 pointers to the repeat objects
constexpr uint32_t kAnalog = 1072;        // 30 u16s, then 30 floats at +60
constexpr uint32_t kCurvePointers = 1252; // 30 pointers to the curve objects
constexpr uint32_t kThresholds = 1852;    // 30 entries of 20 bytes
constexpr uint32_t kListener = 2464;      // the listener (an input processor)
constexpr uint32_t kPadValues = 2880;     // pointer to the logical pad's values

// --- The leaves ---

// ActionCurve::Apply (sub_8210B5B8): the curve objects' slot 1, on `x` (the
// double in f1); returns f1.
double CurveApply(uint8_t* base, uint32_t curve, double x);
// ActionRepeat::Update (sub_8210B568): the repeat objects' slot 1; `held`
// is r4's low byte. Returns whether it fires (r3).
uint32_t RepeatUpdate(uint8_t* base, uint32_t repeat, uint32_t held);
// ActionMapper::IndexOf (sub_8210B140, its case 0 sub_8210B174 merged in):
// id - 151 for ids 151-180, else 30.
uint32_t IndexOf(uint32_t id);
// sub_825924E0: 1 when every bit of `mask` is set in the listener's held
// word (+272), else 0.
uint32_t ListenerHeld(uint8_t* base, uint32_t listener, uint32_t mask);
// sub_8258CF10: the same with the mask read from `mask_address`.
uint32_t ListenerTest(uint8_t* base, uint32_t listener, uint32_t mask_address);

// sub_8258CF10's work on a mask already read (what the mappers' update
// calls).
uint32_t ListenerTestValue(uint8_t* base, uint32_t listener, uint32_t mask);

// Diagnostics (--log_input_reads): counts a held test made from `caller`
// (the original's call to sub_8258CF10, from its return address) as the
// originals' wrappers did: sub_825924E0 from sub_8258CF10, then
// sub_8258CF10 itself. Nothing when the flag is off.
void NoteListenerTest(uint32_t caller, uint32_t listener, uint32_t mask, uint32_t result);

// --- The client list (one, at [0x82970C14]) and its nodes ---

constexpr uint32_t kClientList = 0x82970C14;

// ClientNode::ClientNode (sub_8259C6D0): vtable 0x82071380, +4 the type, +8
// the next node (0).
void ClientNodeConstruct(uint8_t* base, uint32_t node, uint32_t type);
// ClientList::ClientList (sub_8259C7B8): vtable 0x820713C0, the first and
// last nodes and six counts 0; kept at [0x82970C14].
void ClientListConstruct(uint8_t* base, uint32_t list);
// ClientList::Add (sub_8259C960): `node` appended to the list at
// [0x82970C14], its type's count up by one; nothing when there's no list.
void ClientListAdd(uint8_t* base, uint32_t node);
// ClientList::Count and BoundCount (sub_8259C868, sub_8259C898): the count
// for `type` at +12 or +24 (+ 4 × type), up by one first when `bump`'s low
// byte is nonzero.
uint32_t ClientListCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump);
uint32_t ClientListBoundCount(uint8_t* base, uint32_t list, uint32_t type, uint32_t bump);
// ClientList::Find (sub_8259C818): the n-th node of `type` (compared as
// signed words) in the list at [0x82970C14], or 0.
uint32_t ClientListFind(uint8_t* base, uint32_t type, uint32_t n);

// --- The update ---
//
// `stack_word` is what the original finds in the uninitialized stack word it
// builds the new held word in (sub_8210BBA0's +80): bits 30 and 31 are never
// set or cleared there, so they're that word's. Called from
// sub_8210C0E0 it's always the high half of r25 as sub_8210B7D0 saved it
// (its __savegprlr_24 at the same address), so the adapters pass that.

// Whether every repeat object's and curve object's slot 1 is the function
// seen in play (and the mapper isn't 0): the native update calls them
// directly, so the adapters run the original otherwise.
bool RepeatsKnown(uint8_t* base, uint32_t mapper);
bool CurvesKnown(uint8_t* base, uint32_t mapper);
bool MapperCallsKnown(uint8_t* base, uint32_t mapper);

// ActionMapper::CurveUnsigned and CurveCentered (sub_8210B4A8,
// sub_8210B500): `analog` is mapper + 1072, `raw` the u16 in r5; the curve
// object's slot 1 on raw × [0x82002BD4], or (raw - [0x82007F64]) ×
// [0x82007F60]. Returns f1 (r3 is left as the curve object, as the
// original's).
double CurveUnsigned(uint8_t* base, uint32_t analog, uint32_t index, uint32_t raw);
double CurveCentered(uint8_t* base, uint32_t analog, uint32_t index, uint32_t raw);
// ActionMapper::Analog (sub_8210B7D0): `analog` = mapper + 1072, `config` =
// the config's +8 (analog kinds, links), `raw` the raw values. Returns the
// original's r3: the last curve object called, or `analog` when none was;
// `f1` gets the last curve's result (unchanged when none was called).
uint32_t Analog(uint8_t* base, uint32_t analog, uint32_t config, uint32_t raw, double* f1);
// ActionMapper::Hysteresis (sub_8210B718): `words` = mapper + 448; returns
// the threshold entry's state byte.
uint32_t Hysteresis(uint8_t* base, uint32_t words, uint32_t index, double value,
                    uint32_t config);
// ActionMapper::Words (sub_8210BBA0): `words` = mapper + 448, `states` the
// address of the hysteresis states' word. Returns what the last repeat
// update returned (the original's r3).
uint32_t Words(uint8_t* base, uint32_t words, uint32_t config, uint32_t states,
               uint32_t stack_word);
// ActionMapper::Rebuild (sub_8210C0E0): `words` = mapper + 448, `config` =
// mapper + 372, `raw` = mapper + 388. Returns the original's r3; `f1` gets
// the original's f1.
uint32_t Rebuild(uint8_t* base, uint32_t words, uint32_t config, uint32_t raw,
                 uint32_t stack_word, double* f1);
// ActionMapper::Update (sub_8210C370): the raw values from the logical pad
// and the listener, the axes, then Rebuild. Returns the original's r3.
uint32_t MapperUpdate(uint8_t* base, uint32_t mapper, uint32_t stack_word, double* f1);

// ActionMappers::Update (sub_8210B018): every bound mapper of the client
// list `list` updated. UpdateAllKnown: every one passes MapperCallsKnown.
bool UpdateAllKnown(uint8_t* base, uint32_t list);
// Returns the original's r3 (the bound count); `f1` gets the original's f1
// (the last mapper's, or unchanged when there's none).
uint32_t UpdateAll(uint8_t* base, uint32_t list, uint32_t stack_word, double* f1);

// --- The read slots and the setters (vtable 0x82007F78; r4 an action id) ---
//
// Each returns the original's r3: the bit or value, 0 for an id out of
// range (the setters: IndexOf's result).

// Slots 8, 10 and 12 (sub_8210BE70, sub_8210BF40, sub_8210C010): bit i of
// held (+448), pressed (+456) or fired (+464).
uint32_t Held(uint8_t* base, uint32_t mapper, uint32_t id);
uint32_t Pressed(uint8_t* base, uint32_t mapper, uint32_t id);
uint32_t Fired(uint8_t* base, uint32_t mapper, uint32_t id);
// Slot 14 (sub_8210BA28): the u16 at +1072 + 2i.
uint32_t AnalogRaw(uint8_t* base, uint32_t mapper, uint32_t id);
// Slot 2 (sub_8210B698): the threshold entry's on (f1), off (f2) and
// suppress ticks (r7); its counter and state cleared.
uint32_t SetThreshold(uint8_t* base, uint32_t mapper, uint32_t id, double on, double off,
                      uint32_t ticks);
// Slot 4 (sub_8210BDF0): the repeat object's delay and period.
uint32_t SetRepeat(uint8_t* base, uint32_t mapper, uint32_t id, uint32_t delay,
                   uint32_t period);
// Slot 7 (sub_8210BE30): the curve object's center, dead zone and maximum.
uint32_t SetCurve(uint8_t* base, uint32_t mapper, uint32_t id, double center, double dead,
                  double maximum);

// ActionMappers::Configure (sub_8209F228): the settings' index kept at
// [0x82600B84], then for the four mappers ([0x826BAB80] + 8j) and each
// action, slots 2, 4 and 7 with the entry at 0x82600B88 + 32 × index.
// ConfigureKnown: the four mappers are there and their slots are the
// three above. `f` gets the original's f1-f3; returns its r3.
bool ConfigureKnown(uint8_t* base);
uint32_t Configure(uint8_t* base, uint32_t index, double f[3]);

// --- The set-up ---
//
// The repeat and curve objects' slot 0 (their reset) in play:
// sub_8243DC88 (the count, +4, set to 0) and sub_82106540 (nothing), both
// shared with other classes; their work is done here where the slot is
// those, and the adapters run the original otherwise (the *Known checks).
constexpr uint32_t kRepeatResetFunction = 0x8243DC88;
constexpr uint32_t kCurveResetFunction = 0x82106540;

// Whether the 30 objects at `first` + 4i (pointers) or `first` + 16i
// (objects) have that slot 0.
bool RepeatResetsKnown(uint8_t* base, uint32_t pointers);
bool CurveResetsKnown(uint8_t* base, uint32_t pointers);
bool RepeatObjectsResetKnown(uint8_t* base, uint32_t objects);
bool CurveObjectsResetKnown(uint8_t* base, uint32_t objects);

// ActionMapper::ResetWords (sub_8210B8A8): `words` = mapper + 448; each
// repeat object reset, the six words 0. Returns the original's r3 (the
// last repeat object).
uint32_t ResetWords(uint8_t* base, uint32_t words);
// ActionMapper::ResetAnalog (sub_8210B438): `analog` = mapper + 1072; each
// curve object reset, the 30 u16s 0 and the 30 floats [0x82000A34].
// Returns the original's r3 (the last curve object).
uint32_t ResetAnalog(uint8_t* base, uint32_t analog);
// ActionMapper::InitRepeats (sub_8210BB38): `words` = mapper + 448; each
// pointer at +24 + 4i set to its object (+144 + 16i), reset, delay 20 and
// period 10; then ResetWords.
uint32_t InitRepeats(uint8_t* base, uint32_t words);
// ActionMapper::InitCurves (sub_8210BAB0): `analog` = mapper + 1072; each
// pointer at +180 + 4i set to its object (+300 + 16i), reset, center and
// dead zone [0x82000A34], maximum [0x82000A30]; then ResetAnalog.
uint32_t InitCurves(uint8_t* base, uint32_t analog);
// ActionMapper::ResetStages (sub_8210C078): `words` = mapper + 448;
// InitRepeats, InitCurves, and the 30 threshold entries (on and off
// [0x82000A34], the rest 0).
uint32_t ResetStages(uint8_t* base, uint32_t words);

// InputClientList::InputClientList (sub_8210AEB0): the client list, +36 the
// input manager, vtable 0x82007F18.
void InputClientListConstruct(uint8_t* base, uint32_t list, uint32_t manager);
// InputClient::InputClient (sub_8210AE60): the node of `type`, vtable
// 0x82007ED8.
void InputClientConstruct(uint8_t* base, uint32_t node, uint32_t type);
// InputClientList::Add (sub_8210AF00): ClientList::Add on `node`.
void InputClientListAdd(uint8_t* base, uint32_t node);

// ActionMapper::ActionMapper (sub_8210C180): the mapper (2,896 bytes) for
// `player` (+2888): its node (InputClient, type 2), vtable 0x82007F78, the
// repeat and curve objects' vtables (0x82007F68 at +592 + 16i, 0x82007F70 at
// +1372 + 16i), its listener (an input processor, +2464), not bound (+2884),
// ResetStages; the config tables (+12 the kinds, all 1; +132 the analog
// kinds, 2 for actions 24-29, else 1; +252 the links, each direction's
// opposite for actions 2-23, else 30), each action's threshold (on
// [0x82002A10], off [0x8200296C], the rest 0), repeat (delay 20, period 3)
// and curve (center and dead zone [0x82000A34], maximum [0x82000A30]); the
// four config pointers (+372); then ResetWords and ResetAnalog.
// MapperConstructKnown: the two vtables' slot 0 are the resets seen in play
// (the plain C++ does their work), else its adapter runs the original.
bool MapperConstructKnown(uint8_t* base);
void MapperConstruct(uint8_t* base, uint32_t mapper, uint32_t player);

// ActionMapper::Bind (sub_8210B270): `object` (the input manager, from
// InputClientList::BindAll) kept at +2892; for players (+2888) 0-3, the
// listener (+2464) added to that pad's listeners (the object's slot 3,
// InputManager::AddListener) and the logical pad's values (its slot 2,
// then the pad's slot 1) kept at +2880; then +2884 = 1 (bound). Returns 1.
// MapperBindKnown: the player isn't 0-3, or the object's slots 2 and 3 and
// the pad's slot 1 are the functions seen in play and AddListener runs
// natively (the list empty, a node operator new gives: the caller also
// needs --native_heap); otherwise the adapter runs the original.
bool MapperBindKnown(uint8_t* base, uint32_t mapper, uint32_t object);
uint32_t MapperBind(uint8_t* base, uint32_t mapper, uint32_t object);

// ActionMappers::Init (sub_8209F430), its own work before the binding: the
// manager (r3; the app object, whose slot +36 gives the input manager) kept
// at [0x826BAB78]; the client list (52 bytes from operator new,
// InputClientList with the manager) at [0x826BAB7C]; four mappers (2,896
// bytes each, the constructor with players 0-3) into the table at
// [0x826BAB80] + 8j; each added to the list. `list` gets the list. False
// (nothing kept, nothing changed) when operator new doesn't give every
// block: the adapter runs the original. The rest (InputClientList::BindAll,
// a guest call, then ActionMappers::Configure) is the adapter's.
bool InitCreate(uint8_t* base, uint32_t manager, uint32_t* list);

}  // namespace action_mappers
