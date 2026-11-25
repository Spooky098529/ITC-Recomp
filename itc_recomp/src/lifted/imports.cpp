// The import thunks' dispatch table entries (docs/rexglue-removal.md,
// "Before E1: the rewiring", stage 4): written by
// itc_recomp/tools/import_entries.py. Don't edit it by hand: run the tool
// again. A call to an import thunk's address runs the import through
// lifted::Import (our answer, or a log and a stop).

#include "lifted/lifted.h"
#include "lifted/sdk.h"

#include <iterator>

namespace lifted {

namespace {

Result i825DB4A4(const Args& args) {
  return Import(__imp__XNotifyGetNext, args);
}
Result i825DB4B4(const Args& args) {
  return Import(__imp__XMsgStartIORequest, args);
}
Result i825DB4C4(const Args& args) {
  return Import(__imp__XamUserGetName, args);
}
Result i825DB4D4(const Args& args) {
  return Import(__imp__XamUserGetSigninState, args);
}
Result i825DB4E4(const Args& args) {
  return Import(__imp__XamUserAreUsersFriends, args);
}
Result i825DB4F4(const Args& args) {
  return Import(__imp__XamUserCheckPrivilege, args);
}
Result i825DB504(const Args& args) {
  return Import(__imp__XamGetSystemVersion, args);
}
Result i825DB514(const Args& args) {
  return Import(__imp__XamUserCreateStatsEnumerator, args);
}
Result i825DB524(const Args& args) {
  return Import(__imp__XamUserGetXUID, args);
}
Result i825DB534(const Args& args) {
  return Import(__imp__XamNotifyCreateListener, args);
}
Result i825DB544(const Args& args) {
  return Import(__imp__XamShowSigninUI, args);
}
Result i825DB554(const Args& args) {
  return Import(__imp__XamShowFriendsUI, args);
}
Result i825DB564(const Args& args) {
  return Import(__imp__XamShowGamerCardUIForXUID, args);
}
Result i825DB574(const Args& args) {
  return Import(__imp__XamShowPlayerReviewUI, args);
}
Result i825DB584(const Args& args) {
  return Import(__imp__XamShowDeviceSelectorUI, args);
}
Result i825DB594(const Args& args) {
  return Import(__imp__XamShowMessageBoxUI, args);
}
Result i825DB5A4(const Args& args) {
  return Import(__imp__XamShowDirtyDiscErrorUI, args);
}
Result i825DB5B4(const Args& args) {
  return Import(__imp__XamContentCreate, args);
}
Result i825DB5C4(const Args& args) {
  return Import(__imp__XamContentDelete, args);
}
Result i825DB5D4(const Args& args) {
  return Import(__imp__XamContentClose, args);
}
Result i825DB5E4(const Args& args) {
  return Import(__imp__XamContentSetThumbnail, args);
}
Result i825DB5F4(const Args& args) {
  return Import(__imp__XamContentGetCreator, args);
}
Result i825DB604(const Args& args) {
  return Import(__imp__XamContentCreateEnumerator, args);
}
Result i825DB614(const Args& args) {
  return Import(__imp__XamContentGetDeviceData, args);
}
Result i825DB624(const Args& args) {
  return Import(__imp__XamEnumerate, args);
}
Result i825DB634(const Args& args) {
  return Import(__imp__XamLoaderLaunchTitle, args);
}
Result i825DB644(const Args& args) {
  return Import(__imp__XamShowMessageBoxUIEx, args);
}
Result i825DB654(const Args& args) {
  return Import(__imp__XGetLanguage, args);
}
Result i825DB664(const Args& args) {
  return Import(__imp__XGetAVPack, args);
}
Result i825DB674(const Args& args) {
  return Import(__imp__XamLoaderTerminateTitle, args);
}
Result i825DB684(const Args& args) {
  return Import(__imp__XamGetExecutionId, args);
}
Result i825DB694(const Args& args) {
  return Import(__imp__NtQueryVirtualMemory, args);
}
Result i825DB6A4(const Args& args) {
  return Import(__imp__MmQueryStatistics, args);
}
Result i825DB6B4(const Args& args) {
  return Import(__imp__NtClose, args);
}
Result i825DB6C4(const Args& args) {
  return Import(__imp__NtFreeVirtualMemory, args);
}
Result i825DB6D4(const Args& args) {
  return Import(__imp__NtAllocateVirtualMemory, args);
}
Result i825DB6E4(const Args& args) {
  return Import(__imp__NtQueryInformationFile, args);
}
Result i825DB6F4(const Args& args) {
  return Import(__imp__NtQueryVolumeInformationFile, args);
}
Result i825DB704(const Args& args) {
  return Import(__imp__RtlNtStatusToDosError, args);
}
Result i825DB714(const Args& args) {
  return Import(__imp__NtReadFile, args);
}
Result i825DB724(const Args& args) {
  return Import(__imp__NtWriteFile, args);
}
Result i825DB734(const Args& args) {
  return Import(__imp__NtWaitForSingleObjectEx, args);
}
Result i825DB744(const Args& args) {
  return Import(__imp__NtCreateFile, args);
}
Result i825DB754(const Args& args) {
  return Import(__imp__RtlInitAnsiString, args);
}
Result i825DB764(const Args& args) {
  return Import(__imp__NtOpenFile, args);
}
Result i825DB774(const Args& args) {
  return Import(__imp__NtCreateEvent, args);
}
Result i825DB784(const Args& args) {
  return Import(__imp__MmAllocatePhysicalMemoryEx, args);
}
Result i825DB794(const Args& args) {
  return Import(__imp__MmQueryAllocationSize, args);
}
Result i825DB7A4(const Args& args) {
  return Import(__imp__MmFreePhysicalMemory, args);
}
Result i825DB7B4(const Args& args) {
  return Import(__imp__MmQueryAddressProtect, args);
}
Result i825DB7C4(const Args& args) {
  return Import(__imp__RtlMultiByteToUnicodeN, args);
}
Result i825DB7D4(const Args& args) {
  return Import(__imp__RtlUnicodeToMultiByteN, args);
}
Result i825DB7E4(const Args& args) {
  return Import(__imp__vswprintf, args);
}
Result i825DB7F4(const Args& args) {
  return Import(__imp__RtlTimeToTimeFields, args);
}
Result i825DB804(const Args& args) {
  return Import(__imp__KeQuerySystemTime, args);
}
Result i825DB814(const Args& args) {
  return Import(__imp__ExGetXConfigSetting, args);
}
Result i825DB824(const Args& args) {
  return Import(__imp__XexCheckExecutablePrivilege, args);
}
Result i825DB834(const Args& args) {
  return Import(__imp__DbgPrint, args);
}
Result i825DB844(const Args& args) {
  return Import(__imp____C_specific_handler, args);
}
Result i825DB854(const Args& args) {
  return Import(__imp__NtSetInformationFile, args);
}
Result i825DB864(const Args& args) {
  return Import(__imp__XexGetProcedureAddress, args);
}
Result i825DB874(const Args& args) {
  return Import(__imp__KeDelayExecutionThread, args);
}
Result i825DB884(const Args& args) {
  return Import(__imp__ObDereferenceObject, args);
}
Result i825DB894(const Args& args) {
  return Import(__imp__KeSetBasePriorityThread, args);
}
Result i825DB8A4(const Args& args) {
  return Import(__imp__ObReferenceObjectByHandle, args);
}
Result i825DB8B4(const Args& args) {
  return Import(__imp__NtSuspendThread, args);
}
Result i825DB8C4(const Args& args) {
  return Import(__imp__KeSetAffinityThread, args);
}
Result i825DB8D4(const Args& args) {
  return Import(__imp__RtlLeaveCriticalSection, args);
}
Result i825DB8E4(const Args& args) {
  return Import(__imp__RtlEnterCriticalSection, args);
}
Result i825DB8F4(const Args& args) {
  return Import(__imp__RtlImageXexHeaderField, args);
}
Result i825DB904(const Args& args) {
  return Import(__imp__HalReturnToFirmware, args);
}
Result i825DB914(const Args& args) {
  return Import(__imp__RtlFillMemoryUlong, args);
}
Result i825DB924(const Args& args) {
  return Import(__imp__KeBugCheckEx, args);
}
Result i825DB934(const Args& args) {
  return Import(__imp__KeGetCurrentProcessType, args);
}
Result i825DB944(const Args& args) {
  return Import(__imp__RtlCompareMemoryUlong, args);
}
Result i825DB954(const Args& args) {
  return Import(__imp__RtlInitializeCriticalSection, args);
}
Result i825DB964(const Args& args) {
  return Import(__imp__RtlRaiseException, args);
}
Result i825DB974(const Args& args) {
  return Import(__imp__RtlTimeFieldsToTime, args);
}
Result i825DB984(const Args& args) {
  return Import(__imp__VdEnableDisableClockGating, args);
}
Result i825DB994(const Args& args) {
  return Import(__imp__KeReleaseSpinLockFromRaisedIrql, args);
}
Result i825DB9A4(const Args& args) {
  return Import(__imp__KeAcquireSpinLockAtRaisedIrql, args);
}
Result i825DB9B4(const Args& args) {
  return Import(__imp__KfReleaseSpinLock, args);
}
Result i825DB9C4(const Args& args) {
  return Import(__imp__KfAcquireSpinLock, args);
}
Result i825DB9D4(const Args& args) {
  return Import(__imp__KiApcNormalRoutineNop, args);
}
Result i825DB9E4(const Args& args) {
  return Import(__imp__VdEnableRingBufferRPtrWriteBack, args);
}
Result i825DB9F4(const Args& args) {
  return Import(__imp__VdInitializeRingBuffer, args);
}
Result i825DBA04(const Args& args) {
  return Import(__imp__MmGetPhysicalAddress, args);
}
Result i825DBA14(const Args& args) {
  return Import(__imp__VdSetSystemCommandBufferGpuIdentifierAddress, args);
}
Result i825DBA24(const Args& args) {
  return Import(__imp__VdPersistDisplay, args);
}
Result i825DBA34(const Args& args) {
  return Import(__imp__VdSwap, args);
}
Result i825DBA44(const Args& args) {
  return Import(__imp__VdGetSystemCommandBuffer, args);
}
Result i825DBA54(const Args& args) {
  return Import(__imp___vsnprintf, args);
}
Result i825DBA64(const Args& args) {
  return Import(__imp__sprintf, args);
}
Result i825DBA74(const Args& args) {
  return Import(__imp__VdShutdownEngines, args);
}
Result i825DBA84(const Args& args) {
  return Import(__imp__ExRegisterTitleTerminateNotification, args);
}
Result i825DBA94(const Args& args) {
  return Import(__imp__VdQueryVideoMode, args);
}
Result i825DBAA4(const Args& args) {
  return Import(__imp__VdGetCurrentDisplayInformation, args);
}
Result i825DBAB4(const Args& args) {
  return Import(__imp__VdSetDisplayMode, args);
}
Result i825DBAC4(const Args& args) {
  return Import(__imp__VdSetGraphicsInterruptCallback, args);
}
Result i825DBAD4(const Args& args) {
  return Import(__imp__VdInitializeEngines, args);
}
Result i825DBAE4(const Args& args) {
  return Import(__imp__VdIsHSIOTrainingSucceeded, args);
}
Result i825DBAF4(const Args& args) {
  return Import(__imp__VdGetCurrentDisplayGamma, args);
}
Result i825DBB04(const Args& args) {
  return Import(__imp__KeLeaveCriticalRegion, args);
}
Result i825DBB14(const Args& args) {
  return Import(__imp__VdRetrainEDRAM, args);
}
Result i825DBB24(const Args& args) {
  return Import(__imp__VdRetrainEDRAMWorker, args);
}
Result i825DBB34(const Args& args) {
  return Import(__imp__KeEnterCriticalRegion, args);
}
Result i825DBB44(const Args& args) {
  return Import(__imp__VdQueryVideoFlags, args);
}
Result i825DBB54(const Args& args) {
  return Import(__imp__VdCallGraphicsNotificationRoutines, args);
}
Result i825DBB64(const Args& args) {
  return Import(__imp__VdInitializeScalerCommandBuffer, args);
}
Result i825DBB74(const Args& args) {
  return Import(__imp__ObDeleteSymbolicLink, args);
}
Result i825DBB84(const Args& args) {
  return Import(__imp__ObCreateSymbolicLink, args);
}
Result i825DBB94(const Args& args) {
  return Import(__imp__KeLockL2, args);
}
Result i825DBBA4(const Args& args) {
  return Import(__imp__KeUnlockL2, args);
}
Result i825DBBB4(const Args& args) {
  return Import(__imp__KeWaitForSingleObject, args);
}
Result i825DBBC4(const Args& args) {
  return Import(__imp__KeResetEvent, args);
}
Result i825DBBD4(const Args& args) {
  return Import(__imp__KeSetEvent, args);
}
Result i825DBBE4(const Args& args) {
  return Import(__imp__ExCreateThread, args);
}
Result i825DBBF4(const Args& args) {
  return Import(__imp__KeTlsGetValue, args);
}
Result i825DBC04(const Args& args) {
  return Import(__imp__KeTlsSetValue, args);
}
Result i825DBC14(const Args& args) {
  return Import(__imp__KeTlsAlloc, args);
}
Result i825DBC24(const Args& args) {
  return Import(__imp__KeTlsFree, args);
}
Result i825DBC34(const Args& args) {
  return Import(__imp__NetDll_WSAStartup, args);
}
Result i825DBC44(const Args& args) {
  return Import(__imp__NetDll_socket, args);
}
Result i825DBC54(const Args& args) {
  return Import(__imp__NetDll_closesocket, args);
}
Result i825DBC64(const Args& args) {
  return Import(__imp__NetDll_ioctlsocket, args);
}
Result i825DBC74(const Args& args) {
  return Import(__imp__NetDll_bind, args);
}
Result i825DBC84(const Args& args) {
  return Import(__imp__NetDll_connect, args);
}
Result i825DBC94(const Args& args) {
  return Import(__imp__NetDll_listen, args);
}
Result i825DBCA4(const Args& args) {
  return Import(__imp__NetDll_accept, args);
}
Result i825DBCB4(const Args& args) {
  return Import(__imp__NetDll_select, args);
}
Result i825DBCC4(const Args& args) {
  return Import(__imp__NetDll_recvfrom, args);
}
Result i825DBCD4(const Args& args) {
  return Import(__imp__NetDll_sendto, args);
}
Result i825DBCE4(const Args& args) {
  return Import(__imp__NetDll_WSAGetLastError, args);
}
Result i825DBCF4(const Args& args) {
  return Import(__imp__NetDll___WSAFDIsSet, args);
}
Result i825DBD04(const Args& args) {
  return Import(__imp__NetDll_XNetXnAddrToInAddr, args);
}
Result i825DBD14(const Args& args) {
  return Import(__imp__NetDll_XNetXnAddrToMachineId, args);
}
Result i825DBD24(const Args& args) {
  return Import(__imp__NetDll_XNetGetTitleXnAddr, args);
}
Result i825DBD34(const Args& args) {
  return Import(__imp__XGetVideoMode, args);
}
Result i825DBD44(const Args& args) {
  return Import(__imp__XMsgInProcessCall, args);
}
Result i825DBD54(const Args& args) {
  return Import(__imp__XamFree, args);
}
Result i825DBD64(const Args& args) {
  return Import(__imp__XamAlloc, args);
}
Result i825DBD74(const Args& args) {
  return Import(__imp__XamSessionRefObjByHandle, args);
}
Result i825DBD84(const Args& args) {
  return Import(__imp__XamSessionCreateHandle, args);
}
Result i825DBD94(const Args& args) {
  return Import(__imp__XamUserReadProfileSettings, args);
}
Result i825DBDA4(const Args& args) {
  return Import(__imp__XamVoiceHeadsetPresent, args);
}
Result i825DBDB4(const Args& args) {
  return Import(__imp__XamVoiceClose, args);
}
Result i825DBDC4(const Args& args) {
  return Import(__imp__XMsgCancelIORequest, args);
}
Result i825DBDD4(const Args& args) {
  return Import(__imp__XamVoiceSubmitPacket, args);
}
Result i825DBDE4(const Args& args) {
  return Import(__imp__XamVoiceCreate, args);
}
Result i825DBDF4(const Args& args) {
  return Import(__imp__XMsgStartIORequestEx, args);
}
Result i825DBE04(const Args& args) {
  return Import(__imp__XamInputGetCapabilities, args);
}
Result i825DBE14(const Args& args) {
  return Import(__imp__XamInputGetState, args);
}
Result i825DBE24(const Args& args) {
  return Import(__imp__XamInputSetState, args);
}
Result i825DBE34(const Args& args) {
  return Import(__imp__XexGetModuleHandle, args);
}
Result i825DBE44(const Args& args) {
  return Import(__imp__KeRaiseIrqlToDpcLevel, args);
}
Result i825DBE54(const Args& args) {
  return Import(__imp__KfLowerIrql, args);
}
Result i825DBE64(const Args& args) {
  return Import(__imp__KeReleaseSemaphore, args);
}
Result i825DBE74(const Args& args) {
  return Import(__imp__XAudioGetVoiceCategoryVolume, args);
}
Result i825DBE84(const Args& args) {
  return Import(__imp__XAudioGetVoiceCategoryVolumeChangeMask, args);
}
Result i825DBE94(const Args& args) {
  return Import(__imp__KeWaitForMultipleObjects, args);
}
Result i825DBEA4(const Args& args) {
  return Import(__imp__KeResumeThread, args);
}
Result i825DBEB4(const Args& args) {
  return Import(__imp__KeInitializeSemaphore, args);
}
Result i825DBEC4(const Args& args) {
  return Import(__imp__XMAReleaseContext, args);
}
Result i825DBED4(const Args& args) {
  return Import(__imp__XMACreateContext, args);
}
Result i825DBEE4(const Args& args) {
  return Import(__imp__KeQueryPerformanceFrequency, args);
}
Result i825DBEF4(const Args& args) {
  return Import(__imp__RtlTryEnterCriticalSection, args);
}
Result i825DBF04(const Args& args) {
  return Import(__imp__KeTryToAcquireSpinLockAtRaisedIrql, args);
}
Result i825DBF14(const Args& args) {
  return Import(__imp__XAudioRegisterRenderDriverClient, args);
}
Result i825DBF24(const Args& args) {
  return Import(__imp__XAudioUnregisterRenderDriverClient, args);
}
Result i825DBF34(const Args& args) {
  return Import(__imp__XAudioSubmitRenderDriverFrame, args);
}
Result i825DBF44(const Args& args) {
  return Import(__imp__KeBugCheck, args);
}
Result i825DBF54(const Args& args) {
  return Import(__imp__KeEnableFpuExceptions, args);
}
Result i825DBF64(const Args& args) {
  return Import(__imp__RtlUnwind, args);
}
Result i825DBF74(const Args& args) {
  return Import(__imp__RtlCaptureContext, args);
}
Result i825DBF84(const Args& args) {
  return Import(__imp__NtQueryFullAttributesFile, args);
}
Result i825DBF94(const Args& args) {
  return Import(__imp__DbgBreakPoint, args);
}
Result i825DBFA4(const Args& args) {
  return Import(__imp__NtResumeThread, args);
}
Result i825DBFB4(const Args& args) {
  return Import(__imp__NtClearEvent, args);
}
Result i825DBFC4(const Args& args) {
  return Import(__imp__NtCancelTimer, args);
}
Result i825DBFD4(const Args& args) {
  return Import(__imp__NtSetEvent, args);
}
Result i825DBFE4(const Args& args) {
  return Import(__imp__NtSetTimerEx, args);
}
Result i825DBFF4(const Args& args) {
  return Import(__imp__ExTerminateThread, args);
}
Result i825DC004(const Args& args) {
  return Import(__imp__NtCreateTimer, args);
}
Result i825DC014(const Args& args) {
  return Import(__imp__NtDuplicateObject, args);
}
Result i825DC024(const Args& args) {
  return Import(__imp__RtlFreeAnsiString, args);
}
Result i825DC034(const Args& args) {
  return Import(__imp__RtlUnicodeStringToAnsiString, args);
}
Result i825DC044(const Args& args) {
  return Import(__imp__RtlInitUnicodeString, args);
}
Result i825DC054(const Args& args) {
  return Import(__imp__NtFlushBuffersFile, args);
}

const bool kOn = true;

const Registration kImports[] = {
    {0x825DB4A4, i825DB4A4},
    {0x825DB4B4, i825DB4B4},
    {0x825DB4C4, i825DB4C4},
    {0x825DB4D4, i825DB4D4},
    {0x825DB4E4, i825DB4E4},
    {0x825DB4F4, i825DB4F4},
    {0x825DB504, i825DB504},
    {0x825DB514, i825DB514},
    {0x825DB524, i825DB524},
    {0x825DB534, i825DB534},
    {0x825DB544, i825DB544},
    {0x825DB554, i825DB554},
    {0x825DB564, i825DB564},
    {0x825DB574, i825DB574},
    {0x825DB584, i825DB584},
    {0x825DB594, i825DB594},
    {0x825DB5A4, i825DB5A4},
    {0x825DB5B4, i825DB5B4},
    {0x825DB5C4, i825DB5C4},
    {0x825DB5D4, i825DB5D4},
    {0x825DB5E4, i825DB5E4},
    {0x825DB5F4, i825DB5F4},
    {0x825DB604, i825DB604},
    {0x825DB614, i825DB614},
    {0x825DB624, i825DB624},
    {0x825DB634, i825DB634},
    {0x825DB644, i825DB644},
    {0x825DB654, i825DB654},
    {0x825DB664, i825DB664},
    {0x825DB674, i825DB674},
    {0x825DB684, i825DB684},
    {0x825DB694, i825DB694},
    {0x825DB6A4, i825DB6A4},
    {0x825DB6B4, i825DB6B4},
    {0x825DB6C4, i825DB6C4},
    {0x825DB6D4, i825DB6D4},
    {0x825DB6E4, i825DB6E4},
    {0x825DB6F4, i825DB6F4},
    {0x825DB704, i825DB704},
    {0x825DB714, i825DB714},
    {0x825DB724, i825DB724},
    {0x825DB734, i825DB734},
    {0x825DB744, i825DB744},
    {0x825DB754, i825DB754},
    {0x825DB764, i825DB764},
    {0x825DB774, i825DB774},
    {0x825DB784, i825DB784},
    {0x825DB794, i825DB794},
    {0x825DB7A4, i825DB7A4},
    {0x825DB7B4, i825DB7B4},
    {0x825DB7C4, i825DB7C4},
    {0x825DB7D4, i825DB7D4},
    {0x825DB7E4, i825DB7E4},
    {0x825DB7F4, i825DB7F4},
    {0x825DB804, i825DB804},
    {0x825DB814, i825DB814},
    {0x825DB824, i825DB824},
    {0x825DB834, i825DB834},
    {0x825DB844, i825DB844},
    {0x825DB854, i825DB854},
    {0x825DB864, i825DB864},
    {0x825DB874, i825DB874},
    {0x825DB884, i825DB884},
    {0x825DB894, i825DB894},
    {0x825DB8A4, i825DB8A4},
    {0x825DB8B4, i825DB8B4},
    {0x825DB8C4, i825DB8C4},
    {0x825DB8D4, i825DB8D4},
    {0x825DB8E4, i825DB8E4},
    {0x825DB8F4, i825DB8F4},
    {0x825DB904, i825DB904},
    {0x825DB914, i825DB914},
    {0x825DB924, i825DB924},
    {0x825DB934, i825DB934},
    {0x825DB944, i825DB944},
    {0x825DB954, i825DB954},
    {0x825DB964, i825DB964},
    {0x825DB974, i825DB974},
    {0x825DB984, i825DB984},
    {0x825DB994, i825DB994},
    {0x825DB9A4, i825DB9A4},
    {0x825DB9B4, i825DB9B4},
    {0x825DB9C4, i825DB9C4},
    {0x825DB9D4, i825DB9D4},
    {0x825DB9E4, i825DB9E4},
    {0x825DB9F4, i825DB9F4},
    {0x825DBA04, i825DBA04},
    {0x825DBA14, i825DBA14},
    {0x825DBA24, i825DBA24},
    {0x825DBA34, i825DBA34},
    {0x825DBA44, i825DBA44},
    {0x825DBA54, i825DBA54},
    {0x825DBA64, i825DBA64},
    {0x825DBA74, i825DBA74},
    {0x825DBA84, i825DBA84},
    {0x825DBA94, i825DBA94},
    {0x825DBAA4, i825DBAA4},
    {0x825DBAB4, i825DBAB4},
    {0x825DBAC4, i825DBAC4},
    {0x825DBAD4, i825DBAD4},
    {0x825DBAE4, i825DBAE4},
    {0x825DBAF4, i825DBAF4},
    {0x825DBB04, i825DBB04},
    {0x825DBB14, i825DBB14},
    {0x825DBB24, i825DBB24},
    {0x825DBB34, i825DBB34},
    {0x825DBB44, i825DBB44},
    {0x825DBB54, i825DBB54},
    {0x825DBB64, i825DBB64},
    {0x825DBB74, i825DBB74},
    {0x825DBB84, i825DBB84},
    {0x825DBB94, i825DBB94},
    {0x825DBBA4, i825DBBA4},
    {0x825DBBB4, i825DBBB4},
    {0x825DBBC4, i825DBBC4},
    {0x825DBBD4, i825DBBD4},
    {0x825DBBE4, i825DBBE4},
    {0x825DBBF4, i825DBBF4},
    {0x825DBC04, i825DBC04},
    {0x825DBC14, i825DBC14},
    {0x825DBC24, i825DBC24},
    {0x825DBC34, i825DBC34},
    {0x825DBC44, i825DBC44},
    {0x825DBC54, i825DBC54},
    {0x825DBC64, i825DBC64},
    {0x825DBC74, i825DBC74},
    {0x825DBC84, i825DBC84},
    {0x825DBC94, i825DBC94},
    {0x825DBCA4, i825DBCA4},
    {0x825DBCB4, i825DBCB4},
    {0x825DBCC4, i825DBCC4},
    {0x825DBCD4, i825DBCD4},
    {0x825DBCE4, i825DBCE4},
    {0x825DBCF4, i825DBCF4},
    {0x825DBD04, i825DBD04},
    {0x825DBD14, i825DBD14},
    {0x825DBD24, i825DBD24},
    {0x825DBD34, i825DBD34},
    {0x825DBD44, i825DBD44},
    {0x825DBD54, i825DBD54},
    {0x825DBD64, i825DBD64},
    {0x825DBD74, i825DBD74},
    {0x825DBD84, i825DBD84},
    {0x825DBD94, i825DBD94},
    {0x825DBDA4, i825DBDA4},
    {0x825DBDB4, i825DBDB4},
    {0x825DBDC4, i825DBDC4},
    {0x825DBDD4, i825DBDD4},
    {0x825DBDE4, i825DBDE4},
    {0x825DBDF4, i825DBDF4},
    {0x825DBE04, i825DBE04},
    {0x825DBE14, i825DBE14},
    {0x825DBE24, i825DBE24},
    {0x825DBE34, i825DBE34},
    {0x825DBE44, i825DBE44},
    {0x825DBE54, i825DBE54},
    {0x825DBE64, i825DBE64},
    {0x825DBE74, i825DBE74},
    {0x825DBE84, i825DBE84},
    {0x825DBE94, i825DBE94},
    {0x825DBEA4, i825DBEA4},
    {0x825DBEB4, i825DBEB4},
    {0x825DBEC4, i825DBEC4},
    {0x825DBED4, i825DBED4},
    {0x825DBEE4, i825DBEE4},
    {0x825DBEF4, i825DBEF4},
    {0x825DBF04, i825DBF04},
    {0x825DBF14, i825DBF14},
    {0x825DBF24, i825DBF24},
    {0x825DBF34, i825DBF34},
    {0x825DBF44, i825DBF44},
    {0x825DBF54, i825DBF54},
    {0x825DBF64, i825DBF64},
    {0x825DBF74, i825DBF74},
    {0x825DBF84, i825DBF84},
    {0x825DBF94, i825DBF94},
    {0x825DBFA4, i825DBFA4},
    {0x825DBFB4, i825DBFB4},
    {0x825DBFC4, i825DBFC4},
    {0x825DBFD4, i825DBFD4},
    {0x825DBFE4, i825DBFE4},
    {0x825DBFF4, i825DBFF4},
    {0x825DC004, i825DC004},
    {0x825DC014, i825DC014},
    {0x825DC024, i825DC024},
    {0x825DC034, i825DC034},
    {0x825DC044, i825DC044},
    {0x825DC054, i825DC054},
};

const bool kRegistered = (Register(kImports, std::size(kImports), &kOn), true);

}  // namespace

}  // namespace lifted
