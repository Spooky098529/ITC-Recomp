// The app's update and its state stack's register steps (src/app_states/,
// docs/functions/app-states.md), shared by the adapters
// (src/hooks/app_states.cpp, behind --native_app_states) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ refuses. The ones that allocate or free run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"
#include "hooks/native_entry.h"

#include "app_states/app.h"

namespace hooks {

// The guest calls the plain C++ makes (app_states::AppCalls), on `ctx`.
app_states::AppCalls AppStatesCalls(lifted::Regs& ctx);

// MessageVector::Erase sub_82092E70: r3 = the result, r4 = the vector, r5
// and r6 the checked iterators (the vector in the high word; stored at the
// caller's r1 + 32 and + 40); the first stored at r3, r3 left.
void AppMessagesErase(lifted::Regs& ctx);
// MessageVector::Clear sub_82092F98: r3 = the vector; r3 = Erase's result in
// its 96-byte frame (r1 - 16).
void AppMessagesClear(lifted::Regs& ctx);
// MessageVector::Allocate sub_82092B78 and OpList::AllocateNodes
// sub_820ACF10: r3 = the count; r3 the block.
void AppMessagesAllocate(lifted::Regs& ctx);
void AppOpAllocateNodes(lifted::Regs& ctx);
// OpList::IncSize sub_820AD768: r3 = the list, r4 = the count; r3 left.
void AppOpIncSize(lifted::Regs& ctx);
// List::Clear sub_820ACF88: r3 = the list; r3 = its head.
void AppListClear(lifted::Regs& ctx);
// PtrList::Erase sub_82098E80: r3 = the result, r4 = the list, r5 = the
// iterator (the list in the high word); the next node's iterator at r3 and
// the caller's r1 + 32; r3 left.
void AppPtrListErase(lifted::Regs& ctx);
// Delegate::Clone sub_820ACA48: r3 = the delegate; r3 the copy or 0.
void AppDelegateClone(lifted::Regs& ctx);
// StateLayer::IsEmpty sub_820ACE50: r3 = the layer + 4; r3 = 1 or 0.
void AppLayerIsEmpty(lifted::Regs& ctx);
// OpList::OpList sub_820AD0B8: r3 = the list; r3 left.
void AppOpConstruct(lifted::Regs& ctx);
// OpList::BuyNode sub_820ACFF8: r4 = the next node, r5 = the previous, r6
// = the operation's address; r3 the node.
void AppOpBuyNode(lifted::Regs& ctx);
// StateLayer::Push sub_820ADAD0: r3 = the layer, r4 = a state; r3 = the
// layer + 36 (IncSize's).
void AppLayerPush(lifted::Regs& ctx);
// MessageVector::InsertN sub_820938C0: r3 = the vector, r4 = the iterator
// (its element in the low word; stored at the caller's r1 + 24), r5 = the
// count, r6 = the value's address. r3 isn't set (its only caller, Insert,
// doesn't read it).
void AppMessagesInsertN(lifted::Regs& ctx);
// MessageVector::Insert sub_82094110: r3 = the result, r4 = the vector, r5
// = the iterator (stored at the caller's r1 + 32), r6 = the value's
// address; the result iterator at r3, r3 left.
void AppMessagesInsert(lifted::Regs& ctx);
// MessageVector::PushBack sub_820944E0: r3 = the vector, r4 = the value's
// address; r3 the vector, or Insert's result in its 96-byte frame (r1 - 16)
// when it took Insert.
void AppMessagesPushBack(lifted::Regs& ctx);
// App::SendMessage sub_82094AB0: r3 = the +20 interface, r4 = the
// message's address; PushBack's r3.
void AppSendMessage(lifted::Regs& ctx);
// AppStates::IsEmpty sub_820AE1A0: r3 = the state stack; the root's slot
// 11's r3 (ours when it's StateLayer::IsEmpty).
void AppStatesIsEmpty(lifted::Regs& ctx);
// App::IsRunning sub_82092928: r3 = the +20 interface; r3 (and r11) 1 or 0.
void AppIsRunning(lifted::Regs& ctx);

// HandlerMap::ForEach sub_82092BF0: r3 = the result, r4 / r5 the range,
// r6 the functor (function, object), r7 its adjustment (high word); r3 the
// result.
void AppHandlerMapForEach(lifted::Regs& ctx);
// App::ForEachMessage sub_82092CD0 and ForEachRequest sub_82091C48: as
// HandlerMap::ForEach over the vectors.
void AppForEachMessage(lifted::Regs& ctx);
void AppForEachRequest(lifted::Regs& ctx);
// App::RunHandler sub_82092968 and RunListener sub_820929B0: r3 = the app,
// r4 = the key (high word) and the delegate; the slot's r3.
void AppRunHandler(lifted::Regs& ctx);
void AppRunListener(lifted::Regs& ctx);
// App::DeliverMessage sub_82092D50: r3 = the app, r4 = the message; r3 =
// ForEach's result in its 128-byte frame (r1 - 32).
void AppDeliverMessage(lifted::Regs& ctx);
// App::ApplyRequest sub_82093BE0: r3 = the app, r4 = the type (high word)
// and key, r5 = the delegate (high word), both also stored in the caller's
// frame (r1 + 32, + 40); r3 HandlerMap::Insert's result in its 128-byte
// frame (r1 - 32) when it inserted, else left.
void AppApplyRequest(lifted::Regs& ctx);
// The +20 interface's add slots (sub_82094830, sub_820948D0, sub_82094970,
// sub_82094A10): r3 = the interface, r4 = the key's address, r5 = the
// delegate; r3 as RequestVector::PushBack leaves it.
void AppAddHandler(lifted::Regs& ctx);
void AppAddToggledHandler(lifted::Regs& ctx);
void AppAddAlwaysHandler(lifted::Regs& ctx);
void AppAddListener(lifted::Regs& ctx);

// StateLayer::StateLayer sub_820AD118: r3 = the layer, r4 = the app's
// interface (app + 16); r3 the layer. StateLayer::StateLayer(state)
// sub_820ADBD8: r5 = the first state too.
void AppLayerConstruct(lifted::Regs& ctx);
void AppLayerConstructWithState(lifted::Regs& ctx);
// StateLayer::AddChild sub_820ADA08: r3 = the layer, r4 = the child; r3 the
// layer + 12 (PtrList::Insert's).
void AppLayerAddChild(lifted::Regs& ctx);
// AppStates::AddLayer sub_820AE130: r3 = the state stack, r4 = a state; r3
// the root + 12 (AddChild's).
void AppStatesAddLayer(lifted::Regs& ctx);
// AppStates::AppStates sub_820AE348: r3 = it, r4 = the set-up block; r3 it.
void AppStatesConstruct(lifted::Regs& ctx);
// AppStates::Create sub_820AE498: r3 = the set-up block; r3 the stack or 0.
void AppStatesCreate(lifted::Regs& ctx);
// App::Start sub_82092878: r3 = the +20 interface; r3 the stack's slot 1's.
void AppStart(lifted::Regs& ctx);

// PtrList::ForEach sub_820AD3A0 and OpList::ForEach sub_820AD440: r3 = the
// result, r4 / r5 the range, r6 the functor, r7 its adjustment; r3 the
// result.
void AppPtrListForEach(lifted::Regs& ctx);
void AppOpListForEach(lifted::Regs& ctx);
// The layers' child functors sub_820ACD80, sub_820ACDE8 and sub_820ACD60:
// r3 = the layer, r4 = the child; the last slot's r3.
void AppUpdateChild(lifted::Regs& ctx);
void AppUpdateChildToggled(lifted::Regs& ctx);
void AppToggleOffChild(lifted::Regs& ctx);
// StateLayer::ApplyOps sub_820ADE68: r3 = the layer; r3 List::Clear's.
void AppLayerApplyOps(lifted::Regs& ctx);

// StateLayer::Update sub_820ADF00 and UpdateToggled sub_820AE010: r3 = the
// layer; the layer's state slots 8 and 9 sub_820AE120 and sub_820AE128: r3
// = the layer + 4; AppStates::OnUpdate sub_820AE1B8 and OnUpdateToggled
// sub_820AE1C0: r3 = the stack. r3 ApplyOps's.
void AppLayerUpdate(lifted::Regs& ctx);
void AppLayerUpdateToggled(lifted::Regs& ctx);
void AppLayerStateUpdate(lifted::Regs& ctx);
void AppLayerStateUpdateToggled(lifted::Regs& ctx);
void AppStatesOnUpdate(lifted::Regs& ctx);
void AppStatesOnUpdateToggled(lifted::Regs& ctx);

// StateLayer::ApplyOp sub_820ADC60: r3 = the layer, r4 = the operation
// (stored at the caller's r1 + 24); r3 as the original leaves it.
void AppLayerApplyOp(lifted::Regs& ctx);
// StateLayer::OnToggleOn sub_820AD4E0 and OnToggleOff sub_820AD680: r3 = the
// layer; r3 the walk's result.
void AppLayerToggleOn(lifted::Regs& ctx);
void AppLayerToggleOff(lifted::Regs& ctx);
// sub_820ACD38 (r4 = the child) and sub_820AD760 (r3 = the layer + 4): the
// toggles as tail calls.
void AppToggleOnChild(lifted::Regs& ctx);
void AppLayerStateToggleOff(lifted::Regs& ctx);
// AppStates::OnMessage sub_820AE1C8: r3 = the stack, r4 = the app's +20
// interface, r5 = the message's address.
void AppStatesOnMessage(lifted::Regs& ctx);

// App::Update sub_82093DC0: r3 = the app; r3 the request vector's clear's
// (in its frame, r1 - 160, when ours).
void AppUpdate(lifted::Regs& ctx);

}  // namespace hooks
