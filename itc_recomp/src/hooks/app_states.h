// The app's update and its state stack's register steps (src/app_states/,
// docs/functions/app-states.md), shared by the adapters
// (src/hooks/app_states.cpp, behind --native_app_states) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ refuses. The ones that allocate or free run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"

#include "app_states/app.h"

namespace hooks {

// The guest calls the plain C++ makes (app_states::AppCalls), on `ctx`.
app_states::AppCalls AppStatesCalls(PPCContext& ctx, uint8_t* base);

// MessageVector::Erase sub_82092E70: r3 = the result, r4 = the vector, r5
// and r6 the checked iterators (the vector in the high word; stored at the
// caller's r1 + 32 and + 40); the first stored at r3, r3 left.
void AppMessagesErase(PPCContext& ctx, uint8_t* base);
// MessageVector::Clear sub_82092F98: r3 = the vector; r3 = Erase's result in
// its 96-byte frame (r1 - 16).
void AppMessagesClear(PPCContext& ctx, uint8_t* base);
// MessageVector::Allocate sub_82092B78 and OpList::AllocateNodes
// sub_820ACF10: r3 = the count; r3 the block.
void AppMessagesAllocate(PPCContext& ctx, uint8_t* base);
void AppOpAllocateNodes(PPCContext& ctx, uint8_t* base);
// OpList::IncSize sub_820AD768: r3 = the list, r4 = the count; r3 left.
void AppOpIncSize(PPCContext& ctx, uint8_t* base);
// List::Clear sub_820ACF88: r3 = the list; r3 = its head.
void AppListClear(PPCContext& ctx, uint8_t* base);
// PtrList::Erase sub_82098E80: r3 = the result, r4 = the list, r5 = the
// iterator (the list in the high word); the next node's iterator at r3 and
// the caller's r1 + 32; r3 left.
void AppPtrListErase(PPCContext& ctx, uint8_t* base);
// Delegate::Clone sub_820ACA48: r3 = the delegate; r3 the copy or 0.
void AppDelegateClone(PPCContext& ctx, uint8_t* base);
// StateLayer::IsEmpty sub_820ACE50: r3 = the layer + 4; r3 = 1 or 0.
void AppLayerIsEmpty(PPCContext& ctx, uint8_t* base);
// OpList::OpList sub_820AD0B8: r3 = the list; r3 left.
void AppOpConstruct(PPCContext& ctx, uint8_t* base);
// OpList::BuyNode sub_820ACFF8: r4 = the next node, r5 = the previous, r6
// = the operation's address; r3 the node.
void AppOpBuyNode(PPCContext& ctx, uint8_t* base);
// StateLayer::Push sub_820ADAD0: r3 = the layer, r4 = a state; r3 = the
// layer + 36 (IncSize's).
void AppLayerPush(PPCContext& ctx, uint8_t* base);
// MessageVector::InsertN sub_820938C0: r3 = the vector, r4 = the iterator
// (its element in the low word; stored at the caller's r1 + 24), r5 = the
// count, r6 = the value's address. r3 isn't set (its only caller, Insert,
// doesn't read it).
void AppMessagesInsertN(PPCContext& ctx, uint8_t* base);
// MessageVector::Insert sub_82094110: r3 = the result, r4 = the vector, r5
// = the iterator (stored at the caller's r1 + 32), r6 = the value's
// address; the result iterator at r3, r3 left.
void AppMessagesInsert(PPCContext& ctx, uint8_t* base);
// MessageVector::PushBack sub_820944E0: r3 = the vector, r4 = the value's
// address; r3 the vector, or Insert's result in its 96-byte frame (r1 - 16)
// when it took Insert.
void AppMessagesPushBack(PPCContext& ctx, uint8_t* base);
// App::SendMessage sub_82094AB0: r3 = the +20 interface, r4 = the
// message's address; PushBack's r3.
void AppSendMessage(PPCContext& ctx, uint8_t* base);
// AppStates::IsEmpty sub_820AE1A0: r3 = the state stack; the root's slot
// 11's r3 (ours when it's StateLayer::IsEmpty).
void AppStatesIsEmpty(PPCContext& ctx, uint8_t* base);
// App::IsRunning sub_82092928: r3 = the +20 interface; r3 (and r11) 1 or 0.
void AppIsRunning(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
