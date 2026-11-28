// The game's functions by their names (docs/e5-native.md, step 0):
// written by itc_recomp/tools/function_names.py from the rows of
// docs/functions/. Don't edit it by hand: run the tool again. Each name
// refers to the function: a lifted one's typed `lifted::f<address>`, a
// hand port's entry `lifted::entry::e<address>`.

#pragma once

#include "lifted/entries.h"
#include "lifted/functions.h"

namespace game {
inline constexpr auto& AsyncFile_82593BC8 = lifted::f82593BC8;  // sub_82593BC8
inline constexpr auto& BeginIndexedVertices = lifted::entry::e825BDDC0;  // sub_825BDDC0
inline constexpr auto& BeginVertices = lifted::entry::e825BD8F0;  // sub_825BD8F0
inline constexpr auto& Bt709Curve = lifted::f822B0860;  // sub_822B0860
inline constexpr auto& CheckHeap = lifted::entry::e82297CE0;  // sub_82297CE0
inline constexpr auto& CheckPal50 = lifted::entry::e82296AB0;  // sub_82296AB0
inline constexpr auto& CheckTitleId = lifted::entry::e82296EE0;  // sub_82296EE0
inline constexpr auto& ComputeGammaRamp = lifted::entry::e822B0910;  // sub_822B0910
inline constexpr auto& ComputeGammaRampPwl = lifted::entry::e822B0AB0;  // sub_822B0AB0
inline constexpr auto& CountBodyKits = lifted::entry::e820CBD20;  // sub_820CBD20
inline constexpr auto& CreateEvent = lifted::f82295E78;  // sub_82295E78
inline constexpr auto& CreateFile = lifted::entry::e82295508;  // sub_82295508
inline constexpr auto& CreateHeap = lifted::entry::e82297C08;  // sub_82297C08
inline constexpr auto& CreateThread = lifted::entry::e825BA380;  // sub_825BA380
inline constexpr auto& CreateVertexDeclaration = lifted::entry::e822A8980;  // sub_822A8980
inline constexpr auto& D3DDevice_SetVertexShader = lifted::entry::e822A8958;  // sub_822A8958
inline constexpr auto& D3DXCompileShader = lifted::entry::e822B8F10;  // sub_822B8F10
inline constexpr auto& D3DXCreateBuffer = lifted::f822BE450;  // sub_822BE450
inline constexpr auto& DebugPrintf = lifted::f825A1318;  // sub_825A1318
inline constexpr auto& DrawIndexedVertices = lifted::entry::e825BE708;  // sub_825BE708
inline constexpr auto& DrawVertices = lifted::entry::e825BE300;  // sub_825BE300
inline constexpr auto& EndVertices = lifted::entry::e825BDDA0;  // sub_825BDDA0
inline constexpr auto& EnterCriticalSection = lifted::f825A6D28;  // sub_825A6D28
inline constexpr auto& FileTimeToSystemTime = lifted::entry::e8229B720;  // sub_8229B720
inline constexpr auto& FlushFileBuffers = lifted::entry::e825BA510;  // sub_825BA510
inline constexpr auto& FreePagesIn = lifted::entry::e820D09A0;  // sub_820D09A0
inline constexpr auto& GameFile_820A6B88 = lifted::f820A6B88;  // sub_820A6B88
inline constexpr auto& GamePath_820A6CE0 = lifted::f820A6CE0;  // sub_820A6CE0
inline constexpr auto& GetCommandLineA = lifted::entry::e822979D0;  // sub_822979D0
inline constexpr auto& GetCutoverTime = lifted::entry::e8229B1E8;  // sub_8229B1E8
inline constexpr auto& GetLastError = lifted::entry::e82295F18;  // sub_82295F18
inline constexpr auto& GetLocalTime = lifted::entry::e822968B8;  // sub_822968B8
inline constexpr auto& GetProcessHeap = lifted::entry::e8229B670;  // sub_8229B670
inline constexpr auto& GetTickCount = lifted::entry::e822956F0;  // sub_822956F0
inline constexpr auto& GetTimeZoneInformation = lifted::entry::e82297DC8;  // sub_82297DC8
inline constexpr auto& GlobalMemoryStatus = lifted::entry::e82294DA0;  // sub_82294DA0
inline constexpr auto& HeapAlloc = lifted::entry::e82299D60;  // sub_82299D60
inline constexpr auto& HeapFree = lifted::entry::e8229A648;  // sub_8229A648
inline constexpr auto& HeapReAlloc = lifted::entry::e8229A930;  // sub_8229A930
inline constexpr auto& HeapSize = lifted::entry::e82298FF8;  // sub_82298FF8
inline constexpr auto& InitNamedObjectAttributes = lifted::entry::e822970A8;  // sub_822970A8
inline constexpr auto& InitializeCriticalSection = lifted::f8258E548;  // sub_8258E548
inline constexpr auto& LeaveCriticalSection = lifted::f8258E578;  // sub_8258E578
inline constexpr auto& MultiByteToWideChar = lifted::entry::e82296648;  // sub_82296648
inline constexpr auto& NameMatches = lifted::entry::e820CE800;  // sub_820CE800
inline constexpr auto& ObjectInverse = lifted::entry::e820A9BC8;  // sub_820A9BC8
inline constexpr auto& OutputDebugStringA = lifted::entry::e82296270;  // sub_82296270
inline constexpr auto& OutputDebugStringW = lifted::entry::e825BA4A0;  // sub_825BA4A0
inline constexpr auto& Path_82593C58 = lifted::f82593C58;  // sub_82593C58
inline constexpr auto& QueryPerformanceCounter = lifted::entry::e825BA0C8;  // sub_825BA0C8
inline constexpr auto& QueryPerformanceFrequency = lifted::entry::e825B9A70;  // sub_825B9A70
inline constexpr auto& RacerInCurrentStage = lifted::entry::e82126B50;  // sub_82126B50
inline constexpr auto& RaiseException = lifted::entry::e825BA420;  // sub_825BA420
inline constexpr auto& ReadTimeZoneSettings = lifted::entry::e8229B440;  // sub_8229B440
inline constexpr auto& RecordClear = lifted::entry::e8209FA38;  // sub_8209FA38
inline constexpr auto& ReportReadError = lifted::f8209AC78;  // sub_8209AC78
inline constexpr auto& ResetEvent = lifted::entry::e825BA168;  // sub_825BA168
inline constexpr auto& RetrainEdram = lifted::entry::e822B2388;  // sub_822B2388
inline constexpr auto& SaveFlag199F8Or199FC = lifted::entry::e82126CB0;  // sub_82126CB0
inline constexpr auto& SaveFlag199FA = lifted::entry::e82126BB0;  // sub_82126BB0
inline constexpr auto& SetEndOfFile = lifted::f825B9D30;  // sub_825B9D30
inline constexpr auto& SetEvent = lifted::f825BA1E0;  // sub_825BA1E0
inline constexpr auto& SetLastErrorFromStatus = lifted::entry::e82296E88;  // sub_82296E88
inline constexpr auto& SetLastError_82296E70 = lifted::entry::e82296E70;  // sub_82296E70
inline constexpr auto& SetLastError_82296FD0 = lifted::entry::e82296FD0;  // sub_82296FD0
inline constexpr auto& SetUnhandledExceptionFilter = lifted::entry::e82297870;  // sub_82297870
inline constexpr auto& SinCos = lifted::entry::e820B0E10;  // sub_820B0E10
inline constexpr auto& Sleep = lifted::f82295500;  // sub_82295500
inline constexpr auto& SrgbCurve = lifted::f822B07B0;  // sub_822B07B0
inline constexpr auto& SwapWord = lifted::entry::e820C97D8;  // sub_820C97D8
inline constexpr auto& SystemTimeToFileTime = lifted::entry::e8229B680;  // sub_8229B680
inline constexpr auto& UnhandledExceptionFilter = lifted::entry::e82297940;  // sub_82297940
inline constexpr auto& VirtualAlloc = lifted::entry::e82295398;  // sub_82295398
inline constexpr auto& VirtualFree = lifted::entry::e82295330;  // sub_82295330
inline constexpr auto& WSAStartup = lifted::entry::e82459B68;  // sub_82459B68
inline constexpr auto& WaitForSingleObject = lifted::f822970A0;  // sub_822970A0
inline constexpr auto& WaitForSingleObjectEx = lifted::f82297D58;  // sub_82297D58
inline constexpr auto& WideCharToMultiByte = lifted::entry::e82296720;  // sub_82296720
inline constexpr auto& WildcardSearch = lifted::entry::e82592740;  // sub_82592740
inline constexpr auto& X3DAudioInitialize = lifted::entry::e82480710;  // sub_82480710
inline constexpr auto& XACT3CreateEngine = lifted::f825BF5C8;  // sub_825BF5C8
inline constexpr auto& XACTCreateRenderer = lifted::f825C3460;  // sub_825C3460
inline constexpr auto& XACTGetTimeMs = lifted::entry::e825BFE28;  // sub_825BFE28
inline constexpr auto& XACTHashName = lifted::entry::e825BFC50;  // sub_825BFC50
inline constexpr auto& XACTOctavesToCents = lifted::f825D0EA0;  // sub_825D0EA0
inline constexpr auto& XACTSpeakerMask = lifted::f825C9AF8;  // sub_825C9AF8
inline constexpr auto& XACTVoiceCallback = lifted::f825C0488;  // sub_825C0488
inline constexpr auto& XAudioCreateSourceVoice = lifted::entry::e8245A768;  // sub_8245A768
inline constexpr auto& XAudioCreateSubmixVoice = lifted::entry::e8245A7C8;  // sub_8245A7C8
inline constexpr auto& XAudioGetMasteringVoice = lifted::entry::e8245A568;  // sub_8245A568
inline constexpr auto& XAudioInitialize = lifted::entry::e8245A760;  // sub_8245A760
inline constexpr auto& XAudioLock = lifted::f8245ED60;  // sub_8245ED60
inline constexpr auto& XAudioRegisterProcessingCallback = lifted::entry::e8245A5E8;  // sub_8245A5E8
inline constexpr auto& XAudioShutDown = lifted::f8245A4E8;  // sub_8245A4E8
inline constexpr auto& XContentClose = lifted::entry::e82295BF0;  // sub_82295BF0
inline constexpr auto& XContentGetCreator = lifted::f82295C00;  // sub_82295C00
inline constexpr auto& XGCompressSurface = lifted::entry::e8235AAC0;  // sub_8235AAC0
inline constexpr auto& XGSetTextureHeader = lifted::entry::e8235A920;  // sub_8235A920
inline constexpr auto& XGSetVertexBufferHeader = lifted::entry::e8235A688;  // sub_8235A688
inline constexpr auto& XLaunchNewImage = lifted::f822967E0;  // sub_822967E0
inline constexpr auto& XMemAlloc = lifted::entry::e822976F8;  // sub_822976F8
inline constexpr auto& XMemCpy = lifted::entry::e82295890;  // sub_82295890
inline constexpr auto& XMemFree = lifted::entry::e82297790;  // sub_82297790
inline constexpr auto& XMemSet = lifted::entry::e822962B0;  // sub_822962B0
inline constexpr auto& XNetGetTitleXnAddr = lifted::entry::e82459D50;  // sub_82459D50
inline constexpr auto& XNetStartup = lifted::entry::e82575B08;  // sub_82575B08
inline constexpr auto& XNetXnAddrToMachineId = lifted::entry::e82459D40;  // sub_82459D40
inline constexpr auto& XNotifyCreateListener = lifted::entry::e822952B8;  // sub_822952B8
inline constexpr auto& XPhysicalAlloc = lifted::entry::e822964C8;  // sub_822964C8
inline constexpr auto& XPhysicalAllocEx = lifted::entry::e82296550;  // sub_82296550
inline constexpr auto& XPhysicalFree = lifted::entry::e822965C8;  // sub_822965C8
inline constexpr auto& XPhysicalSize = lifted::entry::e822965C0;  // sub_822965C0
inline constexpr auto& XSessionCreate = lifted::f82575E50;  // sub_82575E50
inline constexpr auto& XSessionDelete = lifted::f82576080;  // sub_82576080
inline constexpr auto& XSessionEnd = lifted::f82576540;  // sub_82576540
inline constexpr auto& XSessionModify = lifted::f82575FE0;  // sub_82575FE0
inline constexpr auto& XSessionSearchEx = lifted::f825765F0;  // sub_825765F0
inline constexpr auto& XSessionStart = lifted::f825764A8;  // sub_825764A8
inline constexpr auto& XShowFriendsUI = lifted::f822952C8;  // sub_822952C8
inline constexpr auto& XShowGamerCardUI = lifted::f822952D0;  // sub_822952D0
inline constexpr auto& XShowPlayerReviewUI = lifted::f822952D8;  // sub_822952D8
inline constexpr auto& XShowSigninUI = lifted::entry::e822952C0;  // sub_822952C0
inline constexpr auto& XUserGetName = lifted::entry::e82295020;  // sub_82295020
inline constexpr auto& XUserGetSigninState = lifted::entry::e82295028;  // sub_82295028
inline constexpr auto& XUserGetXUID = lifted::entry::e82295268;  // sub_82295268
inline constexpr auto& XUserReadProfileSettings = lifted::entry::e82576E78;  // sub_82576E78
inline constexpr auto& XUserSetContextEx = lifted::entry::e82294F70;  // sub_82294F70
inline constexpr auto& XUserSetPropertyEx = lifted::f82294F78;  // sub_82294F78
inline constexpr auto& XUserWriteAchievements = lifted::entry::e82294E58;  // sub_82294E58
inline constexpr auto& XapiCallThreadNotifyRoutines = lifted::entry::e82297B38;  // sub_82297B38
inline constexpr auto& XapiThreadStartup = lifted::entry::e825BA558;  // sub_825BA558
inline constexpr auto& _CxxThrowException = lifted::entry::e825AD198;  // sub_825AD198
inline constexpr auto& _FF_MSGBANNER = lifted::entry::e825AEDE8;  // sub_825AEDE8
inline constexpr auto& _NMSG_WRITE = lifted::entry::e825AEDA0;  // sub_825AEDA0
inline constexpr auto& _ValidateExecute = lifted::entry::e825B8910;  // sub_825B8910
inline constexpr auto& __CxxRestoreUnhandledExceptionFilter = lifted::entry::e825B4D90;  // sub_825B4D90
inline constexpr auto& __CxxSetUnhandledExceptionFilter = lifted::entry::e825DAC58;  // sub_825DAC58
inline constexpr auto& __CxxUnhandledExceptionFilter = lifted::entry::e825B4CD0;  // sub_825B4CD0
inline constexpr auto& __WSAFDIsSet = lifted::f82459D20;  // sub_82459D20
inline constexpr auto& ___dtold = lifted::entry::e825B6378;  // sub_825B6378
inline constexpr auto& __chkstk = lifted::entry::e825A9934;  // sub_825A9934
inline constexpr auto& __crtInitCritSecNoSpinCount = lifted::entry::e825B3570;  // sub_825B3570
inline constexpr auto& __crtTlsAlloc = lifted::entry::e825AA3F8;  // sub_825AA3F8
inline constexpr auto& __doserrno = lifted::entry::e825A8218;  // sub_825A8218
inline constexpr auto& __get_sigabrt = lifted::entry::e825B49B0;  // sub_825B49B0
inline constexpr auto& __initmbctable = lifted::entry::e825B6958;  // sub_825B6958
inline constexpr auto& __iob_func = lifted::entry::e825AD4C0;  // sub_825AD4C0
inline constexpr auto& __lock_fhandle = lifted::entry::e825B7958;  // sub_825B7958
inline constexpr auto& __mtold12 = lifted::entry::e825B7F10;  // sub_825B7F10
inline constexpr auto& __onexitinit = lifted::entry::e825A6F00;  // sub_825A6F00
inline constexpr auto& __ultod = lifted::entry::e825A9DB0;  // sub_825A9DB0
inline constexpr auto& _abstract_sw = lifted::entry::e825AB380;  // sub_825AB380
inline constexpr auto& _amsg_exit = lifted::entry::e825AA158;  // sub_825AA158
inline constexpr auto& _atodbl_l = lifted::entry::e825B6098;  // sub_825B6098
inline constexpr auto& _atoflt_l = lifted::entry::e825B6140;  // sub_825B6140
inline constexpr auto& _cexit = lifted::entry::e825AA388;  // sub_825AA388
inline constexpr auto& _cfltcvt = lifted::f825AEB70;  // sub_825AEB70
inline constexpr auto& _cfltcvt_l = lifted::entry::e825AEB20;  // sub_825AEB20
inline constexpr auto& _cftoa_l = lifted::f825AE2C0;  // sub_825AE2C0
inline constexpr auto& _cftoe2_l = lifted::entry::e825ADF28;  // sub_825ADF28
inline constexpr auto& _cftoe_l = lifted::entry::e825AE1A8;  // sub_825AE1A8
inline constexpr auto& _cftog_l = lifted::entry::e825AE9D0;  // sub_825AE9D0
inline constexpr auto& _cinit = lifted::f822979E0;  // sub_822979E0
inline constexpr auto& _clearfp = lifted::entry::e825AB568;  // sub_825AB568
inline constexpr auto& _commit = lifted::entry::e825B4828;  // sub_825B4828
inline constexpr auto& _cropzeros = lifted::entry::e825ADF20;  // sub_825ADF20
inline constexpr auto& _cropzeros_l = lifted::entry::e825ADDD8;  // sub_825ADDD8
inline constexpr auto& _crt_debugger_hook = lifted::entry::e825B68B8;  // sub_825B68B8
inline constexpr auto& _ctrlfp = lifted::entry::e825B0B80;  // sub_825B0B80
inline constexpr auto& _dosmaperr = lifted::entry::e825A8250;  // sub_825A8250
inline constexpr auto& _endstdio = lifted::entry::e825AD5F8;  // sub_825AD5F8
inline constexpr auto& _exit = lifted::entry::e825AA378;  // sub_825AA378
inline constexpr auto& _fassign = lifted::entry::e825ADF10;  // sub_825ADF10
inline constexpr auto& _fassign_l = lifted::entry::e825ADEB0;  // sub_825ADEB0
inline constexpr auto& _fcloseall = lifted::entry::e825B4E78;  // sub_825B4E78
inline constexpr auto& _fileno = lifted::entry::e825B0E88;  // sub_825B0E88
inline constexpr auto& _flsbuf = lifted::entry::e825AEE28;  // sub_825AEE28
inline constexpr auto& _fltin2 = lifted::entry::e825AFDC8;  // sub_825AFDC8
inline constexpr auto& _flush = lifted::entry::e825ACC78;  // sub_825ACC78
inline constexpr auto& _forcdecpt = lifted::entry::e825ADF18;  // sub_825ADF18
inline constexpr auto& _forcdecpt_l = lifted::entry::e825ADD38;  // sub_825ADD38
inline constexpr auto& _fpclass = lifted::entry::e825AB268;  // sub_825AB268
inline constexpr auto& _fptrap = lifted::entry::e825B64F8;  // sub_825B64F8
inline constexpr auto& _free_osfhnd = lifted::entry::e825B77E8;  // sub_825B77E8
inline constexpr auto& _ftbuf = lifted::entry::e825B41A0;  // sub_825B41A0
inline constexpr auto& _get_errno_from_oserr = lifted::entry::e825A8178;  // sub_825A8178
inline constexpr auto& _get_osfhandle = lifted::entry::e825B7880;  // sub_825B7880
inline constexpr auto& _getbuf = lifted::entry::e825B4790;  // sub_825B4790
inline constexpr auto& _getptd = lifted::entry::e825AA580;  // sub_825AA580
inline constexpr auto& _getptd_noexit = lifted::entry::e825AA4C0;  // sub_825AA4C0
inline constexpr auto& _init_pointers = lifted::entry::e825AA398;  // sub_825AA398
inline constexpr auto& _initstdio = lifted::entry::e825AD4D0;  // sub_825AD4D0
inline constexpr auto& _initterm = lifted::entry::e825AA1B8;  // sub_825AA1B8
inline constexpr auto& _initterm_e = lifted::entry::e82297AC0;  // sub_82297AC0
inline constexpr auto& _invalid_parameter = lifted::entry::e825AFCC8;  // sub_825AFCC8
inline constexpr auto& _invoke_watson = lifted::entry::e825AFD10;  // sub_825AFD10
inline constexpr auto& _ioinit = lifted::entry::e825B4668;  // sub_825B4668
inline constexpr auto& _ld12tof = lifted::entry::e825B70D0;  // sub_825B70D0
inline constexpr auto& _lock = lifted::entry::e825B3430;  // sub_825B3430
inline constexpr auto& _lock_file = lifted::entry::e825AD638;  // sub_825AD638
inline constexpr auto& _lock_file2 = lifted::entry::e825AD670;  // sub_825AD670
inline constexpr auto& _lseeki64 = lifted::entry::e825B65D0;  // sub_825B65D0
inline constexpr auto& _lseeki64_nolock = lifted::entry::e825B6500;  // sub_825B6500
inline constexpr auto& _msize = lifted::entry::e825AED30;  // sub_825AED30
inline constexpr auto& _mtinit = lifted::entry::e825AA708;  // sub_825AA708
inline constexpr auto& _positive = lifted::entry::e825ADE90;  // sub_825ADE90
inline constexpr auto& _purecall = lifted::entry::e825A7098;  // sub_825A7098
inline constexpr auto& _read = lifted::f825B81A0;  // sub_825B81A0
inline constexpr auto& _set_abort_behavior = lifted::entry::e825AD120;  // sub_825AD120
inline constexpr auto& _stbuf = lifted::entry::e825B40B8;  // sub_825B40B8
inline constexpr auto& _stricmp = lifted::entry::e825AA100;  // sub_825AA100
inline constexpr auto& _strnicmp = lifted::entry::e825AC710;  // sub_825AC710
inline constexpr auto& _unlock = lifted::entry::e825B32D0;  // sub_825B32D0
inline constexpr auto& _unlock_fhandle = lifted::entry::e825B7A58;  // sub_825B7A58
inline constexpr auto& _unlock_file = lifted::entry::e825AD688;  // sub_825AD688
inline constexpr auto& _unlock_file2 = lifted::entry::e825AD6C0;  // sub_825AD6C0
inline constexpr auto& _vsnprintf = lifted::entry::e825AB010;  // sub_825AB010
inline constexpr auto& _vswprintf_l = lifted::entry::e825A9980;  // sub_825A9980
inline constexpr auto& _write = lifted::entry::e825B11A8;  // sub_825B11A8
inline constexpr auto& abs = lifted::entry::e825AC870;  // sub_825AC870
inline constexpr auto& accept = lifted::f82459CA0;  // sub_82459CA0
inline constexpr auto& acos = lifted::entry::e825A8170;  // sub_825A8170
inline constexpr auto& asin = lifted::entry::e825A80A8;  // sub_825A80A8
inline constexpr auto& atan = lifted::entry::e825A8688;  // sub_825A8688
inline constexpr auto& atan2 = lifted::entry::e825A8770;  // sub_825A8770
inline constexpr auto& atexit = lifted::entry::e825A7068;  // sub_825A7068
inline constexpr auto& atof = lifted::entry::e825AB1C0;  // sub_825AB1C0
inline constexpr auto& atol = lifted::entry::e825AAF08;  // sub_825AAF08
inline constexpr auto& bind = lifted::f82459C60;  // sub_82459C60
inline constexpr auto& bsearch = lifted::entry::e825AC970;  // sub_825AC970
inline constexpr auto& calloc = lifted::entry::e825AC320;  // sub_825AC320
inline constexpr auto& ceil = lifted::entry::e825AA0B8;  // sub_825AA0B8
inline constexpr auto& closesocket = lifted::f82459C38;  // sub_82459C38
inline constexpr auto& copysign = lifted::entry::e825AB1C8;  // sub_825AB1C8
inline constexpr auto& cos = lifted::entry::e825A8978;  // sub_825A8978
inline constexpr auto& exp = lifted::entry::e825ABE30;  // sub_825ABE30
inline constexpr auto& fflush = lifted::entry::e825ACF70;  // sub_825ACF70
inline constexpr auto& floor = lifted::entry::e825A8CA0;  // sub_825A8CA0
inline constexpr auto& free = lifted::entry::e825A9B58;  // sub_825A9B58
inline constexpr auto& ioctlsocket = lifted::f82459C48;  // sub_82459C48
inline constexpr auto& isalnum = lifted::entry::e825AB0A0;  // sub_825AB0A0
inline constexpr auto& isalpha = lifted::entry::e825AB020;  // sub_825AB020
inline constexpr auto& isdigit = lifted::entry::e825AB040;  // sub_825AB040
inline constexpr auto& isleadbyte = lifted::entry::e825B6898;  // sub_825B6898
inline constexpr auto& ldexp = lifted::entry::e825AAD20;  // sub_825AAD20
inline constexpr auto& listen = lifted::f82459C90;  // sub_82459C90
inline constexpr auto& log = lifted::entry::e825A9C00;  // sub_825A9C00
inline constexpr auto& log10 = lifted::entry::e825A9D80;  // sub_825A9D80
inline constexpr auto& main_ = lifted::entry::e82090108;  // sub_82090108
inline constexpr auto& malloc = lifted::entry::e825A9A90;  // sub_825A9A90
inline constexpr auto& memcpy_825A7410 = lifted::entry::e825A7410;  // sub_825A7410
inline constexpr auto& memcpy_825A7830 = lifted::entry::e825A7830;  // sub_825A7830
inline constexpr auto& memcpy_s = lifted::entry::e825A7270;  // sub_825A7270
inline constexpr auto& memmove = lifted::entry::e825AA840;  // sub_825AA840
inline constexpr auto& memmove_s = lifted::entry::e825A7348;  // sub_825A7348
inline constexpr auto& memset = lifted::entry::e825A71D0;  // sub_825A71D0
inline constexpr auto& pow = lifted::entry::e825A8ED0;  // sub_825A8ED0
inline constexpr auto& rand_82293568 = lifted::entry::e82293568;  // sub_82293568
inline constexpr auto& rand_825A9BB8 = lifted::entry::e825A9BB8;  // sub_825A9BB8
inline constexpr auto& realloc = lifted::entry::e825AEBC8;  // sub_825AEBC8
inline constexpr auto& recvfrom = lifted::f82459CD8;  // sub_82459CD8
inline constexpr auto& select = lifted::f82459CB8;  // sub_82459CB8
inline constexpr auto& sin = lifted::entry::e825A88A0;  // sub_825A88A0
inline constexpr auto& socket = lifted::f82459C20;  // sub_82459C20
inline constexpr auto& sprintf = lifted::entry::e825A93D0;  // sub_825A93D0
inline constexpr auto& sqrtf = lifted::entry::e82293590;  // sub_82293590
inline constexpr auto& strncmp = lifted::entry::e825AACD0;  // sub_825AACD0
inline constexpr auto& strncpy = lifted::entry::e825A7F50;  // sub_825A7F50
inline constexpr auto& strrchr = lifted::entry::e825A8630;  // sub_825A8630
inline constexpr auto& strstr = lifted::entry::e825A98A8;  // sub_825A98A8
inline constexpr auto& strtoul = lifted::entry::e825A85E0;  // sub_825A85E0
inline constexpr auto& swprintf_8210A490 = lifted::entry::e8210A490;  // sub_8210A490
inline constexpr auto& swprintf_82296870 = lifted::entry::e82296870;  // sub_82296870
inline constexpr auto& tan = lifted::entry::e825A8A50;  // sub_825A8A50
inline constexpr auto& terminate = lifted::entry::e825B3498;  // sub_825B3498
inline constexpr auto& tolower = lifted::entry::e825A8618;  // sub_825A8618
inline constexpr auto& toupper = lifted::entry::e825A8600;  // sub_825A8600
inline constexpr auto& vsprintf = lifted::entry::e825A71B0;  // sub_825A71B0
inline constexpr auto& wcscat = lifted::entry::e825A7EF8;  // sub_825A7EF8
inline constexpr auto& wcscpy = lifted::entry::e825A7F30;  // sub_825A7F30
inline constexpr auto& wcslen = lifted::f825A9958;  // sub_825A9958
inline constexpr auto& write_char = lifted::entry::e825AF000;  // sub_825AF000
inline constexpr auto& write_multi_char = lifted::entry::e825AF098;  // sub_825AF098
}  // namespace game

namespace game::A16B16G16R16Codec {
inline constexpr auto& DecodeRow = lifted::f823F3228;  // sub_823F3228
inline constexpr auto& EncodeRow = lifted::f823EE750;  // sub_823EE750
}  // namespace game::A16B16G16R16Codec

namespace game::A16B16G16R16FCodec {
inline constexpr auto& DecodeRow = lifted::f823F4DA0;  // sub_823F4DA0
inline constexpr auto& EncodeRow = lifted::f823F1D90;  // sub_823F1D90
}  // namespace game::A16B16G16R16FCodec

namespace game::A1R5G5B5Codec {
inline constexpr auto& DecodeRow = lifted::f823F2A58;  // sub_823F2A58
inline constexpr auto& EncodeRow = lifted::f823EC460;  // sub_823EC460
}  // namespace game::A1R5G5B5Codec

namespace game::A2B10G10R10Codec {
inline constexpr auto& DecodeRow = lifted::f823F2CA8;  // sub_823F2CA8
inline constexpr auto& EncodeRow = lifted::f823ED328;  // sub_823ED328
}  // namespace game::A2B10G10R10Codec

namespace game::A2R10G10B10Codec {
inline constexpr auto& DecodeRow = lifted::f823F30E8;  // sub_823F30E8
inline constexpr auto& EncodeRow = lifted::f823EE2D0;  // sub_823EE2D0
}  // namespace game::A2R10G10B10Codec

namespace game::A2W10V10U10Codec {
inline constexpr auto& DecodeRow = lifted::f823F3D10;  // sub_823F3D10
inline constexpr auto& EncodeRow = lifted::f823F0370;  // sub_823F0370
}  // namespace game::A2W10V10U10Codec

namespace game::A32B32G32R32FCodec {
inline constexpr auto& DecodeRow = lifted::f823F5520;  // sub_823F5520
inline constexpr auto& EncodeRow = lifted::f823F25C0;  // sub_823F25C0
}  // namespace game::A32B32G32R32FCodec

namespace game::A4R4G4B4Codec {
inline constexpr auto& DecodeRow = lifted::f823F2B80;  // sub_823F2B80
inline constexpr auto& EncodeRow = lifted::f823EC8C8;  // sub_823EC8C8
}  // namespace game::A4R4G4B4Codec

namespace game::A8B8G8R8Codec {
inline constexpr auto& DecodeRow = lifted::f823F2DE8;  // sub_823F2DE8
inline constexpr auto& EncodeRow = lifted::f823ED7A8;  // sub_823ED7A8
}  // namespace game::A8B8G8R8Codec

namespace game::A8Codec {
inline constexpr auto& DecodeRow = lifted::f823F3348;  // sub_823F3348
inline constexpr auto& EncodeRow = lifted::f823ECD38;  // sub_823ECD38
}  // namespace game::A8Codec

namespace game::A8L8Codec {
inline constexpr auto& DecodeRow = lifted::f823F35E8;  // sub_823F35E8
inline constexpr auto& EncodeRow = lifted::f823EEE50;  // sub_823EEE50
}  // namespace game::A8L8Codec

namespace game::A8R8G8B8Codec {
inline constexpr auto& DecodeRow = lifted::entry::e823F2620;  // sub_823F2620
inline constexpr auto& EncodeRow = lifted::f823EB4A0;  // sub_823EB4A0
}  // namespace game::A8R8G8B8Codec

namespace game::Accessories {
inline constexpr auto& Acquire = lifted::entry::e820F0EB8;  // sub_820F0EB8
inline constexpr auto& Create = lifted::entry::e820F1038;  // sub_820F1038
inline constexpr auto& Ctor = lifted::entry::e820F0E60;  // sub_820F0E60
inline constexpr auto& Dtor = lifted::entry::e820F0FE0;  // sub_820F0FE0
inline constexpr auto& ReleaseAll = lifted::entry::e820F0AD8;  // sub_820F0AD8
inline constexpr auto& SetScale = lifted::entry::e820F0F00;  // sub_820F0F00
}  // namespace game::Accessories

namespace game::Accessory {
inline constexpr auto& BindSkin = lifted::f820EF468;  // sub_820EF468
inline constexpr auto& Draw = lifted::f820EFA78;  // sub_820EFA78
inline constexpr auto& DrawShape = lifted::f820EF790;  // sub_820EF790
inline constexpr auto& DrawShapes = lifted::f820EF9B8;  // sub_820EF9B8
inline constexpr auto& RecordEntry = lifted::f820EF4E8;  // sub_820EF4E8
inline constexpr auto& SetAnchor = lifted::f820EF418;  // sub_820EF418
inline constexpr auto& SetTexture = lifted::entry::e820EF568;  // sub_820EF568
inline constexpr auto& Step = lifted::f820EF250;  // sub_820EF250
}  // namespace game::Accessory

namespace game::AccessoryEntry {
inline constexpr auto& New = lifted::entry::e820F0920;  // sub_820F0920
inline constexpr auto& OnLoaded = lifted::entry::e820F0820;  // sub_820F0820
}  // namespace game::AccessoryEntry

namespace game::AccessoryFile {
inline constexpr auto& FreeData = lifted::entry::e820F08D0;  // sub_820F08D0
inline constexpr auto& Load = lifted::entry::e820F0C90;  // sub_820F0C90
inline constexpr auto& OnArchiveDone = lifted::entry::e820F0C20;  // sub_820F0C20
}  // namespace game::AccessoryFile

namespace game::AccessorySkeleton {
inline constexpr auto& Move = lifted::f820EFCF8;  // sub_820EFCF8
inline constexpr auto& Step = lifted::f820F0718;  // sub_820F0718
inline constexpr auto& Swing = lifted::f820F0120;  // sub_820F0120
inline constexpr auto& SwingNode = lifted::f820EFD70;  // sub_820EFD70
inline constexpr auto& Update = lifted::f820F0628;  // sub_820F0628
inline constexpr auto& WaitForStep = lifted::f820EFB28;  // sub_820EFB28
}  // namespace game::AccessorySkeleton

namespace game::Achievements {
inline constexpr auto& AllBeaten = lifted::entry::e82290698;  // sub_82290698
inline constexpr auto& BeatsCourseTime = lifted::entry::e82290710;  // sub_82290710
}  // namespace game::Achievements

namespace game::ActionCurve {
inline constexpr auto& Apply = lifted::entry::e8210B5B8;  // sub_8210B5B8
}  // namespace game::ActionCurve

namespace game::ActionMapper {
inline constexpr auto& Analog = lifted::entry::e8210B7D0;  // sub_8210B7D0
inline constexpr auto& AnalogRaw = lifted::entry::e8210BA28;  // sub_8210BA28
inline constexpr auto& AnalogValue = lifted::entry::e8210BA68;  // sub_8210BA68
inline constexpr auto& Fired = lifted::entry::e8210C010;  // sub_8210C010
inline constexpr auto& Held = lifted::entry::e8210BE70;  // sub_8210BE70
inline constexpr auto& HeldBefore = lifted::entry::e8210BED8;  // sub_8210BED8
inline constexpr auto& Hysteresis = lifted::entry::e8210B718;  // sub_8210B718
inline constexpr auto& Pressed = lifted::entry::e8210BF40;  // sub_8210BF40
inline constexpr auto& Released = lifted::entry::e8210BFA8;  // sub_8210BFA8
inline constexpr auto& ResetStages = lifted::entry::e8210C078;  // sub_8210C078
inline constexpr auto& SetCurve = lifted::entry::e8210BE30;  // sub_8210BE30
inline constexpr auto& SetRepeat = lifted::entry::e8210BDF0;  // sub_8210BDF0
inline constexpr auto& SetThreshold = lifted::entry::e8210B698;  // sub_8210B698
inline constexpr auto& Update = lifted::entry::e8210C370;  // sub_8210C370
inline constexpr auto& Words = lifted::entry::e8210BBA0;  // sub_8210BBA0
}  // namespace game::ActionMapper

namespace game::ActionMappers {
inline constexpr auto& Init = lifted::entry::e8209F430;  // sub_8209F430
inline constexpr auto& SetBlocked = lifted::entry::e8209F318;  // sub_8209F318
inline constexpr auto& Update = lifted::entry::e8210B018;  // sub_8210B018
}  // namespace game::ActionMappers

namespace game::ActionRepeat {
inline constexpr auto& Update = lifted::entry::e8210B568;  // sub_8210B568
}  // namespace game::ActionRepeat

namespace game::AeroParts {
inline constexpr auto& Get77 = lifted::entry::e820CB9E0;  // sub_820CB9E0
inline constexpr auto& Get82 = lifted::entry::e820CBC90;  // sub_820CBC90
}  // namespace game::AeroParts

namespace game::AfterRaceMenu {
inline constexpr auto& AnswerPage = lifted::entry::e8221A720;  // sub_8221A720
inline constexpr auto& CloseList = lifted::f8221B9C8;  // sub_8221B9C8
inline constexpr auto& CloseMain = lifted::entry::e8221A950;  // sub_8221A950
inline constexpr auto& ConfirmPage = lifted::entry::e8221A6C8;  // sub_8221A6C8
inline constexpr auto& Draw = lifted::entry::e8221BB48;  // sub_8221BB48
inline constexpr auto& HideListItem = lifted::f8221AB60;  // sub_8221AB60
inline constexpr auto& Intro = lifted::entry::e8221A730;  // sub_8221A730
inline constexpr auto& MainPage = lifted::entry::e8221A4A8;  // sub_8221A4A8
inline constexpr auto& Move = lifted::entry::e8221AA20;  // sub_8221AA20
inline constexpr auto& MoveList = lifted::f8221B3C8;  // sub_8221B3C8
inline constexpr auto& NotOnList = lifted::f8221ABE0;  // sub_8221ABE0
inline constexpr auto& OpenList = lifted::f8221B918;  // sub_8221B918
inline constexpr auto& SetUpList = lifted::f8221B6A0;  // sub_8221B6A0
inline constexpr auto& Show = lifted::entry::e8221A7A8;  // sub_8221A7A8
inline constexpr auto& ShowItem = lifted::f8221A8A8;  // sub_8221A8A8
inline constexpr auto& ShowItems = lifted::entry::e8221AC00;  // sub_8221AC00
inline constexpr auto& ShowList = lifted::f8221ACC0;  // sub_8221ACC0
inline constexpr auto& ShowListItem = lifted::f8221AAD8;  // sub_8221AAD8
inline constexpr auto& ShowReplay = lifted::f8221AE58;  // sub_8221AE58
inline constexpr auto& Start = lifted::entry::e8221B860;  // sub_8221B860
}  // namespace game::AfterRaceMenu

namespace game::AiDriver {
inline constexpr auto& AdjustForOther = lifted::entry::e821452A0;  // sub_821452A0
inline constexpr auto& Drive = lifted::entry::e8213B058;  // sub_8213B058
inline constexpr auto& DriveCar = lifted::entry::e8215F678;  // sub_8215F678
inline constexpr auto& Of = lifted::entry::e82139728;  // sub_82139728
inline constexpr auto& PickTarget = lifted::entry::e82145118;  // sub_82145118
inline constexpr auto& ReactToOthers = lifted::entry::e82145CA8;  // sub_82145CA8
inline constexpr auto& ReadCar = lifted::entry::e82144F50;  // sub_82144F50
inline constexpr auto& ReadVehicle = lifted::entry::e821467F0;  // sub_821467F0
inline constexpr auto& ReadVehicleOwnSlice = lifted::entry::e821469D0;  // sub_821469D0
inline constexpr auto& Reset = lifted::entry::e8213AAA8;  // sub_8213AAA8
inline constexpr auto& ResetControl = lifted::entry::e82139580;  // sub_82139580
inline constexpr auto& SetFlag2 = lifted::entry::e82138D20;  // sub_82138D20
inline constexpr auto& Setup = lifted::entry::e82170FF8;  // sub_82170FF8
inline constexpr auto& StartRace = lifted::entry::e82139DA0;  // sub_82139DA0
inline constexpr auto& UpdateIdle = lifted::entry::e821399A8;  // sub_821399A8
inline constexpr auto& UpdateLane = lifted::entry::e82144EF8;  // sub_82144EF8
inline constexpr auto& UpdateStarting = lifted::f8213A1B8;  // sub_8213A1B8
inline constexpr auto& UpdateTrack = lifted::f821398C0;  // sub_821398C0
}  // namespace game::AiDriver

namespace game::AiDrivers {
inline constexpr auto& AllSettled = lifted::entry::e8213ADF8;  // sub_8213ADF8
inline constexpr auto& ClearAll = lifted::entry::e8213A140;  // sub_8213A140
inline constexpr auto& Create = lifted::entry::e8213B1E0;  // sub_8213B1E0
inline constexpr auto& Destroy = lifted::entry::e8213B2B8;  // sub_8213B2B8
inline constexpr auto& ResetAll = lifted::entry::e82139750;  // sub_82139750
inline constexpr auto& StartGroup = lifted::entry::e8213A000;  // sub_8213A000
inline constexpr auto& StartRanked = lifted::entry::e8213A8B0;  // sub_8213A8B0
inline constexpr auto& UpdateAll = lifted::entry::e82139628;  // sub_82139628
}  // namespace game::AiDrivers

namespace game::AiOther {
inline constexpr auto& Mirror = lifted::entry::e82145610;  // sub_82145610
inline constexpr auto& Update = lifted::entry::e821454C8;  // sub_821454C8
inline constexpr auto& UpdateTimer = lifted::entry::e821456A0;  // sub_821456A0
}  // namespace game::AiOther

namespace game::AiPath {
inline constexpr auto& AvoidOthers = lifted::entry::e8213E690;  // sub_8213E690
inline constexpr auto& BranchMasksFor = lifted::entry::e8213D378;  // sub_8213D378
inline constexpr auto& DropPassed = lifted::entry::e8213F2E0;  // sub_8213F2E0
inline constexpr auto& Extend = lifted::entry::e8213ECF8;  // sub_8213ECF8
inline constexpr auto& FollowLeaderBranch = lifted::entry::e8213EB48;  // sub_8213EB48
inline constexpr auto& FollowTargets = lifted::entry::e8213D9D8;  // sub_8213D9D8
inline constexpr auto& NormalizeByBranch = lifted::entry::e8213C908;  // sub_8213C908
inline constexpr auto& NormalizeByLane = lifted::entry::e8213B428;  // sub_8213B428
inline constexpr auto& NoteFixedSections = lifted::f8213C248;  // sub_8213C248
inline constexpr auto& PlanLine = lifted::entry::e8213F4B0;  // sub_8213F4B0
inline constexpr auto& RefreshPoints = lifted::entry::e8213B5D8;  // sub_8213B5D8
inline constexpr auto& SetMode = lifted::entry::e8213D508;  // sub_8213D508
inline constexpr auto& SetRanges = lifted::entry::e8213D670;  // sub_8213D670
}  // namespace game::AiPath

namespace game::AiPathEntry {
inline constexpr auto& BlockLeft = lifted::entry::e8213C5C8;  // sub_8213C5C8
inline constexpr auto& BlockRight = lifted::entry::e8213C630;  // sub_8213C630
}  // namespace game::AiPathEntry

namespace game::AiSkill {
inline constexpr auto& Reset = lifted::entry::e8213F698;  // sub_8213F698
inline constexpr auto& Update = lifted::entry::e8213FCF8;  // sub_8213FCF8
}  // namespace game::AiSkill

namespace game::AltHud {
inline constexpr auto& GaugeFloat = lifted::f82216FD0;  // sub_82216FD0
inline constexpr auto& GaugeInt119 = lifted::f82217168;  // sub_82217168
inline constexpr auto& GaugeInt149 = lifted::f82217270;  // sub_82217270
inline constexpr auto& RunMovie = lifted::entry::e82216C00;  // sub_82216C00
inline constexpr auto& SetBar = lifted::entry::e822175A0;  // sub_822175A0
inline constexpr auto& SetBigSpeed = lifted::f82216EE8;  // sub_82216EE8
inline constexpr auto& SetBoost = lifted::entry::e82217538;  // sub_82217538
inline constexpr auto& SetBrake = lifted::entry::e82217690;  // sub_82217690
inline constexpr auto& SetGear = lifted::entry::e82217460;  // sub_82217460
inline constexpr auto& SetNode25 = lifted::entry::e822174E0;  // sub_822174E0
inline constexpr auto& SetSpeed = lifted::entry::e82216E00;  // sub_82216E00
inline constexpr auto& SetSteering = lifted::entry::e822176E8;  // sub_822176E8
inline constexpr auto& SetTach = lifted::entry::e822173E8;  // sub_822173E8
inline constexpr auto& SetThrottle = lifted::entry::e82217638;  // sub_82217638
inline constexpr auto& SetUpExtraGauges = lifted::entry::e822178A0;  // sub_822178A0
inline constexpr auto& SetUpGauges = lifted::entry::e82216C40;  // sub_82216C40
inline constexpr auto& ShowPanels = lifted::entry::e82216D78;  // sub_82216D78
inline constexpr auto& ToggleView = lifted::f82217778;  // sub_82217778
inline constexpr auto& UpdateExtraGauges = lifted::entry::e82217A30;  // sub_82217A30
}  // namespace game::AltHud

namespace game::Animation {
inline constexpr auto& Advance = lifted::entry::e82146AC0;  // sub_82146AC0
}  // namespace game::Animation

namespace game::AnimationList {
inline constexpr auto& Insert = lifted::entry::e8211B4D0;  // sub_8211B4D0
inline constexpr auto& RemoveAt = lifted::entry::e8211B080;  // sub_8211B080
}  // namespace game::AnimationList

namespace game::AnimationSet {
inline constexpr auto& CurrentFrame = lifted::entry::e82146B80;  // sub_82146B80
inline constexpr auto& FindByTexture = lifted::entry::e82146A38;  // sub_82146A38
}  // namespace game::AnimationSet

namespace game::App {
inline constexpr auto& AddAlwaysHandler = lifted::entry::e82094970;  // sub_82094970
inline constexpr auto& AddHandler = lifted::entry::e82094830;  // sub_82094830
inline constexpr auto& AddListener = lifted::entry::e82094A10;  // sub_82094A10
inline constexpr auto& AddToggledHandler = lifted::entry::e820948D0;  // sub_820948D0
inline constexpr auto& ApplyRequest = lifted::entry::e82093BE0;  // sub_82093BE0
inline constexpr auto& Create = lifted::entry::e82094AB8;  // sub_82094AB8
inline constexpr auto& Ctor = lifted::entry::e82094590;  // sub_82094590
inline constexpr auto& DeliverMessage = lifted::entry::e82092D50;  // sub_82092D50
inline constexpr auto& ForEachRequest = lifted::entry::e82091C48;  // sub_82091C48
inline constexpr auto& FrameDriver = lifted::entry::e82094320;  // sub_82094320
inline constexpr auto& FrameDriverFromInterface16 = lifted::entry::e82094498;  // sub_82094498
inline constexpr auto& FrameDriverFromInterface8 = lifted::entry::e82094480;  // sub_82094480
inline constexpr auto& IsRunning = lifted::entry::e82092928;  // sub_82092928
inline constexpr auto& OnRunLoopEvent = lifted::f82092DE0;  // sub_82092DE0
inline constexpr auto& PollSignIn = lifted::entry::e82099828;  // sub_82099828
inline constexpr auto& ReadUsers = lifted::f82099600;  // sub_82099600
inline constexpr auto& RunHandler = lifted::entry::e82092968;  // sub_82092968
inline constexpr auto& RunListener = lifted::entry::e820929B0;  // sub_820929B0
inline constexpr auto& SceneFromInterface16 = lifted::entry::e820944D8;  // sub_820944D8
inline constexpr auto& SceneFromInterface8 = lifted::entry::e82094338;  // sub_82094338
inline constexpr auto& SendMessage = lifted::entry::e82094AB0;  // sub_82094AB0
inline constexpr auto& ShutDown = lifted::f82106718;  // sub_82106718
inline constexpr auto& ShutDownServices = lifted::f82106678;  // sub_82106678
inline constexpr auto& Start = lifted::entry::e82092878;  // sub_82092878
inline constexpr auto& StartUp = lifted::entry::e82106548;  // sub_82106548
inline constexpr auto& Update = lifted::entry::e82093DC0;  // sub_82093DC0
}  // namespace game::App

namespace game::AppCamera {
inline constexpr auto& Ctor = lifted::entry::e820A8B88;  // sub_820A8B88
}  // namespace game::AppCamera

namespace game::AppCameraList {
inline constexpr auto& Add = lifted::f820A9928;  // sub_820A9928
}  // namespace game::AppCameraList

namespace game::AppStates {
inline constexpr auto& AddLayer = lifted::entry::e820AE130;  // sub_820AE130
inline constexpr auto& Create = lifted::entry::e820AE498;  // sub_820AE498
inline constexpr auto& IsEmpty = lifted::entry::e820AE1A0;  // sub_820AE1A0
inline constexpr auto& OnMessage = lifted::entry::e820AE1C8;  // sub_820AE1C8
inline constexpr auto& OnUpdate = lifted::entry::e820AE1B8;  // sub_820AE1B8
inline constexpr auto& OnUpdateToggled = lifted::entry::e820AE1C0;  // sub_820AE1C0
}  // namespace game::AppStates

namespace game::Archive {
inline constexpr auto& AssetSize = lifted::entry::e820A0678;  // sub_820A0678
inline constexpr auto& Cancel = lifted::entry::e8209DB20;  // sub_8209DB20
inline constexpr auto& CancelQueued = lifted::entry::e8209DB90;  // sub_8209DB90
inline constexpr auto& Init = lifted::entry::e8209E210;  // sub_8209E210
inline constexpr auto& Load = lifted::entry::e8209DE48;  // sub_8209DE48
inline constexpr auto& NextRequest = lifted::entry::e8209DD98;  // sub_8209DD98
inline constexpr auto& QueueLength = lifted::entry::e8209D7B0;  // sub_8209D7B0
inline constexpr auto& Shutdown = lifted::f8209D3B0;  // sub_8209D3B0
inline constexpr auto& Toc = lifted::entry::e8209D868;  // sub_8209D868
inline constexpr auto& WaitIdle = lifted::entry::e8209D730;  // sub_8209D730
}  // namespace game::Archive

namespace game::AreaLights {
inline constexpr auto& Key = lifted::entry::e820F1C20;  // sub_820F1C20
inline constexpr auto& Load = lifted::entry::e820F22F8;  // sub_820F22F8
inline constexpr auto& OnLoaded = lifted::entry::e820F1C30;  // sub_820F1C30
}  // namespace game::AreaLights

namespace game::AreaModel {
inline constexpr auto& Key = lifted::entry::e820F1CD8;  // sub_820F1CD8
inline constexpr auto& Load = lifted::entry::e820F23C0;  // sub_820F23C0
inline constexpr auto& OnLoaded = lifted::entry::e820F1CE8;  // sub_820F1CE8
inline constexpr auto& Reset = lifted::entry::e820F12F8;  // sub_820F12F8
}  // namespace game::AreaModel

namespace game::AsyncFile {
inline constexpr auto& Detach = lifted::f82593BB0;  // sub_82593BB0
}  // namespace game::AsyncFile

namespace game::AudioEngine {
inline constexpr auto& CreateSoundBank = lifted::f8259CF10;  // sub_8259CF10
inline constexpr auto& CreateWaveBank = lifted::f8259CE78;  // sub_8259CE78
inline constexpr auto& DoWork = lifted::entry::e8259D1A8;  // sub_8259D1A8
inline constexpr auto& GetCategory = lifted::f8259D0C8;  // sub_8259D0C8
inline constexpr auto& PauseCategory = lifted::f8259D120;  // sub_8259D120
inline constexpr auto& ResumeCategory = lifted::f8259D140;  // sub_8259D140
inline constexpr auto& SetCategoryVolume = lifted::f8259D100;  // sub_8259D100
inline constexpr auto& Start3D = lifted::entry::e8259CE10;  // sub_8259CE10
inline constexpr auto& StartXAudio = lifted::entry::e8259CD50;  // sub_8259CD50
}  // namespace game::AudioEngine

namespace game::AudioManager {
inline constexpr auto& ClearHandles = lifted::f82118C70;  // sub_82118C70
inline constexpr auto& Create = lifted::entry::e82119B38;  // sub_82119B38
inline constexpr auto& DeleteIfStopped = lifted::f821185C8;  // sub_821185C8
inline constexpr auto& Destroy = lifted::f82119278;  // sub_82119278
inline constexpr auto& DoWork = lifted::f82117E88;  // sub_82117E88
inline constexpr auto& Dtor = lifted::f82119450;  // sub_82119450
inline constexpr auto& FindBank = lifted::f82117FF0;  // sub_82117FF0
inline constexpr auto& FindFreeBankSlot = lifted::f82117F78;  // sub_82117F78
inline constexpr auto& FindHandle = lifted::f82118CF0;  // sub_82118CF0
inline constexpr auto& FreeReleasedBanks = lifted::entry::e821184E8;  // sub_821184E8
inline constexpr auto& GetCategory = lifted::f82117E68;  // sub_82117E68
inline constexpr auto& GetCueIndex = lifted::f82118320;  // sub_82118320
inline constexpr auto& GetEngine = lifted::entry::e82117EA8;  // sub_82117EA8
inline constexpr auto& GetSoundBank = lifted::entry::e82118400;  // sub_82118400
inline constexpr auto& IsAnyBankLoading = lifted::f82117EB8;  // sub_82117EB8
inline constexpr auto& IsBankLoaded = lifted::f82118270;  // sub_82118270
inline constexpr auto& IsBankLoading = lifted::entry::e82118200;  // sub_82118200
inline constexpr auto& IsBankSlotEmpty = lifted::entry::e821182E0;  // sub_821182E0
inline constexpr auto& IsCuePlaying = lifted::entry::e82118B58;  // sub_82118B58
inline constexpr auto& LoadBank = lifted::f821180B8;  // sub_821180B8
inline constexpr auto& PauseCategory = lifted::f82117E78;  // sub_82117E78
inline constexpr auto& PlayCue = lifted::f82119500;  // sub_82119500
inline constexpr auto& PlayCue2D = lifted::f82119638;  // sub_82119638
inline constexpr auto& PlayCue3D = lifted::f82119608;  // sub_82119608
inline constexpr auto& PlayHandle = lifted::f82119690;  // sub_82119690
inline constexpr auto& ReleaseBank = lifted::entry::e82118190;  // sub_82118190
inline constexpr auto& RemoveHandle = lifted::f82118F00;  // sub_82118F00
inline constexpr auto& RemoveStoppedSounds = lifted::entry::e82118C18;  // sub_82118C18
inline constexpr auto& ResumeCategory = lifted::f82117E80;  // sub_82117E80
inline constexpr auto& SetCategoryVolume = lifted::f82117E70;  // sub_82117E70
inline constexpr auto& SetHandle3D = lifted::f82118E10;  // sub_82118E10
inline constexpr auto& SetHandle3DParams = lifted::f82118E50;  // sub_82118E50
inline constexpr auto& SetHandlePosition = lifted::f82118D90;  // sub_82118D90
inline constexpr auto& SetHandleVelocity = lifted::f82118DD0;  // sub_82118DD0
inline constexpr auto& SetListenerOrientation = lifted::entry::e82117E10;  // sub_82117E10
inline constexpr auto& SetListenerParams = lifted::f82117E20;  // sub_82117E20
inline constexpr auto& SetListenerPosition = lifted::entry::e82117E08;  // sub_82117E08
inline constexpr auto& SetListenerVelocity = lifted::entry::e82117E18;  // sub_82117E18
inline constexpr auto& StartHandle = lifted::f82118EA0;  // sub_82118EA0
inline constexpr auto& StopBank = lifted::f821189F0;  // sub_821189F0
inline constexpr auto& StopCue = lifted::f82118A90;  // sub_82118A90
inline constexpr auto& StopHandle = lifted::f82118ED0;  // sub_82118ED0
inline constexpr auto& Update = lifted::f82119118;  // sub_82119118
inline constexpr auto& UpdateHandles = lifted::f821190B0;  // sub_821190B0
inline constexpr auto& UpdateSounds = lifted::entry::e82118FB0;  // sub_82118FB0
}  // namespace game::AudioManager

namespace game::AudioRenderDriver {
inline constexpr auto& Create = lifted::f82465970;  // sub_82465970
inline constexpr auto& Destroy = lifted::entry::e824658C8;  // sub_824658C8
inline constexpr auto& Register = lifted::entry::e824657E0;  // sub_824657E0
inline constexpr auto& Size = lifted::f824658B8;  // sub_824658B8
inline constexpr auto& Submit = lifted::entry::e82465870;  // sub_82465870
}  // namespace game::AudioRenderDriver

namespace game::BankPair {
inline constexpr auto& CanFree = lifted::f821176E8;  // sub_821176E8
inline constexpr auto& Ctor = lifted::f82117D08;  // sub_82117D08
inline constexpr auto& Dtor = lifted::f82117B88;  // sub_82117B88
inline constexpr auto& GetCueIndex = lifted::f821176C0;  // sub_821176C0
inline constexpr auto& HasName = lifted::f82117D00;  // sub_82117D00
inline constexpr auto& IsLoaded = lifted::f82117698;  // sub_82117698
inline constexpr auto& Load = lifted::f821178C8;  // sub_821178C8
}  // namespace game::BankPair

namespace game::BillboardMaterial {
inline constexpr auto& Init = lifted::entry::e8209F5A0;  // sub_8209F5A0
inline constexpr auto& SetLights = lifted::entry::e8209F5C8;  // sub_8209F5C8
}  // namespace game::BillboardMaterial

namespace game::Billboards {
inline constexpr auto& BuildVertices = lifted::entry::e8209C6E0;  // sub_8209C6E0
inline constexpr auto& Draw = lifted::entry::e8209CDB0;  // sub_8209CDB0
inline constexpr auto& FlipRing = lifted::entry::e8209BD18;  // sub_8209BD18
inline constexpr auto& Init = lifted::entry::e8209BE88;  // sub_8209BE88
inline constexpr auto& Record = lifted::entry::e8209CC38;  // sub_8209CC38
}  // namespace game::Billboards

namespace game::Bits24 {
inline constexpr auto& SetAll = lifted::f821128A0;  // sub_821128A0
}  // namespace game::Bits24

namespace game::BlinkTimers {
inline constexpr auto& Advance = lifted::entry::e820DC958;  // sub_820DC958
}  // namespace game::BlinkTimers

namespace game::Blitter {
inline constexpr auto& CopyBlocks = lifted::f82368D98;  // sub_82368D98
inline constexpr auto& FilterBox2D = lifted::f82369FF8;  // sub_82369FF8
inline constexpr auto& FilterBox3D = lifted::f8236A4F0;  // sub_8236A4F0
inline constexpr auto& FilterLinear2D = lifted::f8236AB00;  // sub_8236AB00
inline constexpr auto& FilterLinear3D = lifted::f8236AEA8;  // sub_8236AEA8
inline constexpr auto& FilterNone = lifted::f82369B48;  // sub_82369B48
inline constexpr auto& FilterPoint = lifted::f82369DB8;  // sub_82369DB8
inline constexpr auto& FilterTriangle2D = lifted::f8236BF40;  // sub_8236BF40
inline constexpr auto& FilterTriangle3D = lifted::f8236B7A0;  // sub_8236B7A0
inline constexpr auto& FreeWeights = lifted::f8236B718;  // sub_8236B718
inline constexpr auto& LinearWeights = lifted::f8236A9C8;  // sub_8236A9C8
inline constexpr auto& TriangleWeights = lifted::f8236B418;  // sub_8236B418
}  // namespace game::Blitter

namespace game::BodyState {
inline constexpr auto& Restore = lifted::entry::e821031A0;  // sub_821031A0
}  // namespace game::BodyState

namespace game::Boot {
inline constexpr auto& EndMovie = lifted::entry::e82107BF8;  // sub_82107BF8
inline constexpr auto& StartMovie = lifted::entry::e82107B90;  // sub_82107B90
inline constexpr auto& WaitForMovie = lifted::entry::e82107C58;  // sub_82107C58
}  // namespace game::Boot

namespace game::BootData {
inline constexpr auto& CreateObjects = lifted::entry::e82105868;  // sub_82105868
inline constexpr auto& Load = lifted::entry::e82106618;  // sub_82106618
inline constexpr auto& Unload = lifted::f821066D0;  // sub_821066D0
}  // namespace game::BootData

namespace game::BootMode {
inline constexpr auto& DeletingDestructor = lifted::entry::e82106D68;  // sub_82106D68
inline constexpr auto& Init = lifted::entry::e82107F68;  // sub_82107F68
inline constexpr auto& NextMode = lifted::entry::e82108018;  // sub_82108018
}  // namespace game::BootMode

namespace game::Box {
inline constexpr auto& Extend = lifted::entry::e82597150;  // sub_82597150
}  // namespace game::Box

namespace game::BoxBox {
inline constexpr auto& EdgesCrossing = lifted::entry::e820BF080;  // sub_820BF080
inline constexpr auto& FindImpact = lifted::entry::e820BF038;  // sub_820BF038
inline constexpr auto& FindPushOut = lifted::entry::e820BFB28;  // sub_820BFB28
inline constexpr auto& Separated = lifted::entry::e820BD280;  // sub_820BD280
inline constexpr auto& SeparatedAfterMove = lifted::entry::e820BD578;  // sub_820BD578
}  // namespace game::BoxBox

namespace game::BoxShape {
inline constexpr auto& CacheWord = lifted::entry::e820BA678;  // sub_820BA678
inline constexpr auto& Clone = lifted::entry::e820BB218;  // sub_820BB218
inline constexpr auto& CopyCache = lifted::entry::e820BAA50;  // sub_820BAA50
inline constexpr auto& Corner = lifted::entry::e820B9ED0;  // sub_820B9ED0
inline constexpr auto& CornerCount = lifted::entry::e820B9EC8;  // sub_820B9EC8
inline constexpr auto& EdgeCount = lifted::entry::e820B9EF8;  // sub_820B9EF8
inline constexpr auto& EdgeEnd = lifted::entry::e820B9FB0;  // sub_820B9FB0
inline constexpr auto& EdgeStart = lifted::entry::e820B9F98;  // sub_820B9F98
inline constexpr auto& Face = lifted::entry::e820B9EE8;  // sub_820B9EE8
inline constexpr auto& FaceCount = lifted::entry::e820B9EE0;  // sub_820B9EE0
inline constexpr auto& FaceDirection = lifted::entry::e820B9F00;  // sub_820B9F00
inline constexpr auto& LowerCorner = lifted::entry::e820BA368;  // sub_820BA368
inline constexpr auto& Place = lifted::entry::e820B9C40;  // sub_820B9C40
inline constexpr auto& SetCacheWord = lifted::entry::e820BA668;  // sub_820BA668
inline constexpr auto& Translate = lifted::entry::e820BA2F0;  // sub_820BA2F0
}  // namespace game::BoxShape

namespace game::Buffer28 {
inline constexpr auto& Dtor = lifted::entry::e82152D28;  // sub_82152D28
inline constexpr auto& Free = lifted::entry::e82152028;  // sub_82152028
inline constexpr auto& Reset = lifted::entry::e82152D38;  // sub_82152D38
}  // namespace game::Buffer28

namespace game::BuildList {
inline constexpr auto& Insert = lifted::entry::e8219C230;  // sub_8219C230
}  // namespace game::BuildList

namespace game::BuildScreen {
inline constexpr auto& BackToBuilds = lifted::f821A1258;  // sub_821A1258
inline constexpr auto& BackToGroups = lifted::entry::e821A0A38;  // sub_821A0A38
inline constexpr auto& BackToStand = lifted::entry::e8219FF78;  // sub_8219FF78
inline constexpr auto& BuildPage = lifted::entry::e8219F1C8;  // sub_8219F1C8
inline constexpr auto& BuildsPage = lifted::entry::e8219F048;  // sub_8219F048
inline constexpr auto& ChangeCategory = lifted::entry::e8219FC60;  // sub_8219FC60
inline constexpr auto& ChangeGroupInBuilds = lifted::f821A0E50;  // sub_821A0E50
inline constexpr auto& ChangeGroupOnStand = lifted::f821A0B38;  // sub_821A0B38
inline constexpr auto& Close = lifted::entry::e8219C300;  // sub_8219C300
inline constexpr auto& Create = lifted::entry::e8219BD80;  // sub_8219BD80
inline constexpr auto& Destroy = lifted::entry::e8219B8C8;  // sub_8219B8C8
inline constexpr auto& DetailsPage = lifted::entry::e8219B960;  // sub_8219B960
inline constexpr auto& Enter = lifted::entry::e821A0860;  // sub_821A0860
inline constexpr auto& FadeOut = lifted::entry::e8219C850;  // sub_8219C850
inline constexpr auto& GroupsPage = lifted::entry::e8219EEC0;  // sub_8219EEC0
inline constexpr auto& HighlightBuilds = lifted::entry::e8219C8F8;  // sub_8219C8F8
inline constexpr auto& HighlightDetails = lifted::entry::e8219CBD8;  // sub_8219CBD8
inline constexpr auto& HighlightGroups = lifted::entry::e8219C528;  // sub_8219C528
inline constexpr auto& HighlightGroups2 = lifted::entry::e8219C6D0;  // sub_8219C6D0
inline constexpr auto& HighlightStand = lifted::entry::e8219CA08;  // sub_8219CA08
inline constexpr auto& HighlightStand2 = lifted::entry::e8219CB18;  // sub_8219CB18
inline constexpr auto& InitStates = lifted::entry::e821A16C0;  // sub_821A16C0
inline constexpr auto& LeavePage = lifted::entry::e8219C3A8;  // sub_8219C3A8
inline constexpr auto& MoveBuild = lifted::f8219F3A0;  // sub_8219F3A0
inline constexpr auto& MoveGroup = lifted::f8219FB08;  // sub_8219FB08
inline constexpr auto& MoveStand = lifted::f8219F520;  // sub_8219F520
inline constexpr auto& Open = lifted::entry::e821A08D8;  // sub_821A08D8
inline constexpr auto& OpenBuilds = lifted::entry::e821A1138;  // sub_821A1138
inline constexpr auto& OpenDetails = lifted::entry::e8219FEC8;  // sub_8219FEC8
inline constexpr auto& OpenMovie = lifted::entry::e8219B950;  // sub_8219B950
inline constexpr auto& OpenStand = lifted::entry::e8219FE10;  // sub_8219FE10
inline constexpr auto& RestoreBuildList = lifted::entry::e8219BB80;  // sub_8219BB80
inline constexpr auto& ScrollDetails = lifted::f8219F690;  // sub_8219F690
inline constexpr auto& SetUpBuilds = lifted::entry::e821A1008;  // sub_821A1008
inline constexpr auto& Unload = lifted::entry::e8219C298;  // sub_8219C298
inline constexpr auto& UpdatePage = lifted::entry::e8219FA80;  // sub_8219FA80
inline constexpr auto& ValueText = lifted::entry::e8219BCC0;  // sub_8219BCC0
inline constexpr auto& WaitForLoad = lifted::entry::e8219BE98;  // sub_8219BE98
}  // namespace game::BuildScreen

namespace game::Camera {
inline constexpr auto& Ctor = lifted::entry::e82593908;  // sub_82593908
inline constexpr auto& SetFieldOfView = lifted::entry::e82593278;  // sub_82593278
inline constexpr auto& SetViewportSize = lifted::entry::e82593500;  // sub_82593500
inline constexpr auto& ShakeRandom = lifted::entry::e82152BD8;  // sub_82152BD8
inline constexpr auto& Update = lifted::entry::e82593568;  // sub_82593568
}  // namespace game::Camera

namespace game::CameraAnim {
inline constexpr auto& EvalTarget = lifted::entry::e821504E0;  // sub_821504E0
}  // namespace game::CameraAnim

namespace game::CameraAnimBlock {
inline constexpr auto& Free = lifted::entry::e8214FFF0;  // sub_8214FFF0
}  // namespace game::CameraAnimBlock

namespace game::CameraAnims {
inline constexpr auto& Clear = lifted::entry::e82150078;  // sub_82150078
inline constexpr auto& FreeAll = lifted::entry::e821507D8;  // sub_821507D8
inline constexpr auto& Load = lifted::entry::e82150B10;  // sub_82150B10
inline constexpr auto& OnLoaded = lifted::entry::e82150878;  // sub_82150878
inline constexpr auto& SetGlobals = lifted::f82150040;  // sub_82150040
}  // namespace game::CameraAnims

namespace game::CameraCurve {
inline constexpr auto& Load = lifted::entry::e820BFCE8;  // sub_820BFCE8
}  // namespace game::CameraCurve

namespace game::CameraDirector {
inline constexpr auto& InitStates = lifted::entry::e82159950;  // sub_82159950
inline constexpr auto& OnRaceStart = lifted::entry::e821596B0;  // sub_821596B0
inline constexpr auto& Release = lifted::entry::e82153098;  // sub_82153098
inline constexpr auto& Resume = lifted::entry::e821597F0;  // sub_821597F0
inline constexpr auto& SetMode = lifted::entry::e821591F0;  // sub_821591F0
inline constexpr auto& SetState3 = lifted::f82153150;  // sub_82153150
inline constexpr auto& Start = lifted::entry::e82152338;  // sub_82152338
inline constexpr auto& ToView3 = lifted::entry::e821524D8;  // sub_821524D8
inline constexpr auto& ToView3IfCourse = lifted::entry::e82153160;  // sub_82153160
inline constexpr auto& ToView4 = lifted::entry::e82152400;  // sub_82152400
inline constexpr auto& ToView5 = lifted::entry::e821524B8;  // sub_821524B8
inline constexpr auto& Update = lifted::entry::e821594B8;  // sub_821594B8
inline constexpr auto& UpdateThunk = lifted::f82159948;  // sub_82159948
}  // namespace game::CameraDirector

namespace game::CameraPass {
inline constexpr auto& Disable = lifted::entry::e820A0958;  // sub_820A0958
inline constexpr auto& Enable = lifted::entry::e820A0968;  // sub_820A0968
inline constexpr auto& FromViewport = lifted::entry::e820AA4A8;  // sub_820AA4A8
inline constexpr auto& Init = lifted::entry::e820A3DB0;  // sub_820A3DB0
inline constexpr auto& RecordBegin = lifted::entry::e820A0CC0;  // sub_820A0CC0
inline constexpr auto& RecordEnd = lifted::entry::e820A1C30;  // sub_820A1C30
inline constexpr auto& RecordViewport = lifted::entry::e820A0978;  // sub_820A0978
inline constexpr auto& ReleaseSurfaces = lifted::entry::e820A0C40;  // sub_820A0C40
inline constexpr auto& SetUpSurfaces = lifted::entry::e820A2F40;  // sub_820A2F40
inline constexpr auto& Update = lifted::entry::e820A3978;  // sub_820A3978
}  // namespace game::CameraPass

namespace game::CameraPasses {
inline constexpr auto& EnableForView = lifted::entry::e820AA1A8;  // sub_820AA1A8
inline constexpr auto& Init = lifted::entry::e8209F9B8;  // sub_8209F9B8
inline constexpr auto& MakeRenderTarget = lifted::entry::e8209FBF0;  // sub_8209FBF0
inline constexpr auto& Mode = lifted::entry::e8209F718;  // sub_8209F718
inline constexpr auto& RecordBegins = lifted::entry::e8209F8A0;  // sub_8209F8A0
inline constexpr auto& RecordEnds = lifted::entry::e8209F900;  // sub_8209F900
inline constexpr auto& RenderTarget = lifted::entry::e8209F7A8;  // sub_8209F7A8
inline constexpr auto& SetNear = lifted::entry::e820AA368;  // sub_820AA368
}  // namespace game::CameraPasses

namespace game::CameraPath {
inline constexpr auto& Dtor = lifted::f821540B0;  // sub_821540B0
inline constexpr auto& OnLoaded = lifted::entry::e82152078;  // sub_82152078
}  // namespace game::CameraPath

namespace game::CameraPreset {
inline constexpr auto& Ctor = lifted::entry::e8217BE08;  // sub_8217BE08
}  // namespace game::CameraPreset

namespace game::Car {
inline constexpr auto& Clear = lifted::entry::e820CD7F0;  // sub_820CD7F0
inline constexpr auto& DrawUpdate = lifted::entry::e820DAC68;  // sub_820DAC68
inline constexpr auto& FromBuild = lifted::entry::e820CDCE0;  // sub_820CDCE0
inline constexpr auto& Init = lifted::entry::e820CD5D8;  // sub_820CD5D8
inline constexpr auto& Refresh = lifted::entry::e820C4998;  // sub_820C4998
inline constexpr auto& SetModel = lifted::entry::e820CD8A8;  // sub_820CD8A8
inline constexpr auto& SetModelAndClass = lifted::entry::e821104D8;  // sub_821104D8
inline constexpr auto& SetupVehicle = lifted::entry::e820CD5C8;  // sub_820CD5C8
}  // namespace game::Car

namespace game::CarCard {
inline constexpr auto& Fill = lifted::entry::e821821C8;  // sub_821821C8
}  // namespace game::CarCard

namespace game::CarCards {
inline constexpr auto& FillAll = lifted::entry::e821822F8;  // sub_821822F8
}  // namespace game::CarCards

namespace game::CarData {
inline constexpr auto& Ctor = lifted::entry::e820AEF80;  // sub_820AEF80
}  // namespace game::CarData

namespace game::CarDealer {
inline constexpr auto& AfterDetail = lifted::entry::e82180C48;  // sub_82180C48
inline constexpr auto& Close = lifted::entry::e82181838;  // sub_82181838
inline constexpr auto& CloseDetail = lifted::f82180C30;  // sub_82180C30
inline constexpr auto& Create = lifted::entry::e82180D68;  // sub_82180D68
inline constexpr auto& Destroy = lifted::entry::e82180E70;  // sub_82180E70
inline constexpr auto& InitStates = lifted::entry::e82185920;  // sub_82185920
inline constexpr auto& Leave = lifted::entry::e821817C0;  // sub_821817C0
inline constexpr auto& OnDialogDone = lifted::entry::e82183160;  // sub_82183160
inline constexpr auto& OnMessage26 = lifted::f82183078;  // sub_82183078
inline constexpr auto& OnMessage40 = lifted::f82180C18;  // sub_82180C18
inline constexpr auto& OnNo = lifted::f82180CD0;  // sub_82180CD0
inline constexpr auto& OnYes = lifted::entry::e82181968;  // sub_82181968
inline constexpr auto& OpenMovie = lifted::entry::e82180C08;  // sub_82180C08
inline constexpr auto& Page0 = lifted::entry::e82180F90;  // sub_82180F90
inline constexpr auto& Page1 = lifted::entry::e82183428;  // sub_82183428
inline constexpr auto& Page2 = lifted::entry::e82183600;  // sub_82183600
inline constexpr auto& Page3 = lifted::entry::e82181A10;  // sub_82181A10
inline constexpr auto& Page4 = lifted::entry::e82181B98;  // sub_82181B98
inline constexpr auto& Page5 = lifted::f82181BE8;  // sub_82181BE8
inline constexpr auto& SetColorBar = lifted::entry::e82182770;  // sub_82182770
inline constexpr auto& SetKindBar = lifted::entry::e82182158;  // sub_82182158
inline constexpr auto& SetNameTexts = lifted::entry::e82182388;  // sub_82182388
inline constexpr auto& SetPriceTexts = lifted::entry::e82182418;  // sub_82182418
inline constexpr auto& SetScrollArrows = lifted::entry::e82182DD0;  // sub_82182DD0
inline constexpr auto& SetSpecTexts = lifted::entry::e821824A8;  // sub_821824A8
inline constexpr auto& SetUpSpecSheet = lifted::f821810E0;  // sub_821810E0
inline constexpr auto& ShowDriveDialog = lifted::f82182990;  // sub_82182990
inline constexpr auto& ShowFirstCarDialog = lifted::f82182A80;  // sub_82182A80
inline constexpr auto& ShowSwatches = lifted::entry::e821826C0;  // sub_821826C0
inline constexpr auto& Start = lifted::entry::e82184B80;  // sub_82184B80
inline constexpr auto& StartFirstRace = lifted::entry::e82180CE8;  // sub_82180CE8
inline constexpr auto& Transition0 = lifted::entry::e82184C70;  // sub_82184C70
inline constexpr auto& Transition1 = lifted::entry::e82181C38;  // sub_82181C38
inline constexpr auto& Transition10 = lifted::f82185178;  // sub_82185178
inline constexpr auto& Transition11 = lifted::entry::e82181ED0;  // sub_82181ED0
inline constexpr auto& Transition12 = lifted::f82181FC0;  // sub_82181FC0
inline constexpr auto& Transition13 = lifted::f82183F68;  // sub_82183F68
inline constexpr auto& Transition14 = lifted::f82185220;  // sub_82185220
inline constexpr auto& Transition15 = lifted::entry::e82185310;  // sub_82185310
inline constexpr auto& Transition16 = lifted::entry::e821820C0;  // sub_821820C0
inline constexpr auto& Transition17 = lifted::entry::e82184018;  // sub_82184018
inline constexpr auto& Transition18 = lifted::f82185400;  // sub_82185400
inline constexpr auto& Transition19 = lifted::entry::e821841B0;  // sub_821841B0
inline constexpr auto& Transition2 = lifted::f82183BD8;  // sub_82183BD8
inline constexpr auto& Transition20 = lifted::f82185518;  // sub_82185518
inline constexpr auto& Transition21 = lifted::f82184268;  // sub_82184268
inline constexpr auto& Transition3 = lifted::entry::e82181D68;  // sub_82181D68
inline constexpr auto& Transition4 = lifted::entry::e82184DA0;  // sub_82184DA0
inline constexpr auto& Transition5 = lifted::entry::e82184E60;  // sub_82184E60
inline constexpr auto& Transition6 = lifted::entry::e82181E10;  // sub_82181E10
inline constexpr auto& Transition7 = lifted::entry::e82183DE0;  // sub_82183DE0
inline constexpr auto& Transition8 = lifted::entry::e82184FB8;  // sub_82184FB8
inline constexpr auto& Transition9 = lifted::entry::e821850B8;  // sub_821850B8
inline constexpr auto& Unload = lifted::entry::e821816B8;  // sub_821816B8
inline constexpr auto& Update = lifted::entry::e82185600;  // sub_82185600
inline constexpr auto& UpdatePage = lifted::entry::e82184AF0;  // sub_82184AF0
inline constexpr auto& WaitForLoad = lifted::entry::e82180F20;  // sub_82180F20
}  // namespace game::CarDealer

namespace game::CarDetail {
inline constexpr auto& Open = lifted::entry::e821830C0;  // sub_821830C0
}  // namespace game::CarDetail

namespace game::CarInput {
inline constexpr auto& Clear = lifted::entry::e820C96E8;  // sub_820C96E8
}  // namespace game::CarInput

namespace game::CarInputs {
inline constexpr auto& ClearAll = lifted::f821022B0;  // sub_821022B0
}  // namespace game::CarInputs

namespace game::CarInterior {
inline constexpr auto& Load = lifted::entry::e820D79B0;  // sub_820D79B0
inline constexpr auto& LoadSetCBody = lifted::entry::e820D67E8;  // sub_820D67E8
}  // namespace game::CarInterior

namespace game::CarList {
inline constexpr auto& DrawCarPowerGraph = lifted::entry::e82250700;  // sub_82250700
inline constexpr auto& DrawEntryPowerGraph = lifted::entry::e82250670;  // sub_82250670
}  // namespace game::CarList

namespace game::CarModel {
inline constexpr auto& DeletingDestructor = lifted::entry::e820D5988;  // sub_820D5988
inline constexpr auto& IsLoaded = lifted::entry::e820D3C28;  // sub_820D3C28
inline constexpr auto& IsReady = lifted::entry::e820D4048;  // sub_820D4048
inline constexpr auto& LoadExterior = lifted::entry::e820D5EC8;  // sub_820D5EC8
inline constexpr auto& MakePlate = lifted::entry::e820ED6D0;  // sub_820ED6D0
inline constexpr auto& OnDecalLoaded = lifted::entry::e820D5750;  // sub_820D5750
inline constexpr auto& OnFileLoaded = lifted::entry::e820D47A8;  // sub_820D47A8
inline constexpr auto& OnFileLoadedB = lifted::entry::e820D4F80;  // sub_820D4F80
inline constexpr auto& Release = lifted::entry::e820D4508;  // sub_820D4508
inline constexpr auto& ReloadDecals = lifted::entry::e820D5D20;  // sub_820D5D20
inline constexpr auto& SelectedTexture = lifted::entry::e820D4588;  // sub_820D4588
inline constexpr auto& SetRecord = lifted::entry::e820D3C70;  // sub_820D3C70
inline constexpr auto& SetVinylTextures = lifted::entry::e820D5F88;  // sub_820D5F88
}  // namespace game::CarModel

namespace game::CarModels {
inline constexpr auto& BlockOf = lifted::entry::e8215AAD0;  // sub_8215AAD0
}  // namespace game::CarModels

namespace game::CarOverlays {
inline constexpr auto& BindViewport = lifted::entry::e821264C0;  // sub_821264C0
inline constexpr auto& ClearViews = lifted::entry::e82126530;  // sub_82126530
inline constexpr auto& CreateChildren = lifted::f82123450;  // sub_82123450
inline constexpr auto& Ctor = lifted::f82123350;  // sub_82123350
inline constexpr auto& DeleteChildren = lifted::f82125970;  // sub_82125970
inline constexpr auto& Dtor = lifted::f821261C0;  // sub_821261C0
inline constexpr auto& FindModel = lifted::f821253F0;  // sub_821253F0
inline constexpr auto& FreeView = lifted::f82126570;  // sub_82126570
inline constexpr auto& Init = lifted::f821256B8;  // sub_821256B8
inline constexpr auto& LoadTextures = lifted::f82125888;  // sub_82125888
inline constexpr auto& MarkerKind = lifted::entry::e82122EA8;  // sub_82122EA8
inline constexpr auto& ModelSpriteKind = lifted::entry::e82123188;  // sub_82123188
inline constexpr auto& NewView = lifted::f82126660;  // sub_82126660
inline constexpr auto& OnTexturesLoaded = lifted::f82125610;  // sub_82125610
inline constexpr auto& Record = lifted::f82122958;  // sub_82122958
inline constexpr auto& ReleaseTextures = lifted::f821258F8;  // sub_821258F8
inline constexpr auto& Reset = lifted::f82123880;  // sub_82123880
inline constexpr auto& Update = lifted::f82126220;  // sub_82126220
inline constexpr auto& UpdateEffects = lifted::f82126158;  // sub_82126158
inline constexpr auto& UpdateLightSprites = lifted::entry::e821251C0;  // sub_821251C0
inline constexpr auto& UpdateMarker = lifted::f82123A80;  // sub_82123A80
inline constexpr auto& UpdateModelSprites = lifted::f82123E48;  // sub_82123E48
inline constexpr auto& UpdateParticlesB = lifted::entry::e821252D0;  // sub_821252D0
inline constexpr auto& UpdateParticlesC = lifted::entry::e82125798;  // sub_82125798
inline constexpr auto& UpdateSmoke = lifted::entry::e82125B60;  // sub_82125B60
inline constexpr auto& UpdateSparks = lifted::f82124728;  // sub_82124728
inline constexpr auto& UpdateTrails = lifted::entry::e82124EE0;  // sub_82124EE0
inline constexpr auto& View = lifted::entry::e82126550;  // sub_82126550
inline constexpr auto& ViewportEntry = lifted::entry::e821264A8;  // sub_821264A8
}  // namespace game::CarOverlays

namespace game::CarPanel {
inline constexpr auto& Hide = lifted::f8224DC50;  // sub_8224DC50
inline constexpr auto& IsHidden = lifted::f8224E1A8;  // sub_8224E1A8
inline constexpr auto& IsShownAgain = lifted::f8224E168;  // sub_8224E168
inline constexpr auto& PlaceCursor = lifted::entry::e8224D9A0;  // sub_8224D9A0
inline constexpr auto& PlayStats = lifted::f8224DF90;  // sub_8224DF90
inline constexpr auto& PlayStatsOut = lifted::f8224DFF0;  // sub_8224DFF0
inline constexpr auto& SetCarName = lifted::entry::e822417E8;  // sub_822417E8
inline constexpr auto& SetPrice = lifted::f8224E7E8;  // sub_8224E7E8
inline constexpr auto& SetStats = lifted::entry::e8224E288;  // sub_8224E288
inline constexpr auto& SetTexts = lifted::entry::e8224D7F0;  // sub_8224D7F0
inline constexpr auto& ShowAgain = lifted::f8224DBA0;  // sub_8224DBA0
inline constexpr auto& ShowCarStats = lifted::f8224EA10;  // sub_8224EA10
inline constexpr auto& ShowStats = lifted::entry::e8224E920;  // sub_8224E920
inline constexpr auto& StatsHidden = lifted::f8224E248;  // sub_8224E248
inline constexpr auto& StatsShown = lifted::f8224E208;  // sub_8224E208
}  // namespace game::CarPanel

namespace game::CarPanelBase {
inline constexpr auto& DeletingDestructor = lifted::f8224EB68;  // sub_8224EB68
inline constexpr auto& Dtor = lifted::entry::e8224D7E0;  // sub_8224D7E0
inline constexpr auto& SetCarName = lifted::entry::e8224E878;  // sub_8224E878
}  // namespace game::CarPanelBase

namespace game::CarPart {
inline constexpr auto& Draw = lifted::entry::e820D06F0;  // sub_820D06F0
}  // namespace game::CarPart

namespace game::CarParts {
inline constexpr auto& DeletingDestructor = lifted::entry::e820D74C0;  // sub_820D74C0
inline constexpr auto& IsLoaded = lifted::entry::e820D7258;  // sub_820D7258
inline constexpr auto& OnPartLoadedA = lifted::f820D6C10;  // sub_820D6C10
inline constexpr auto& OnPartLoadedB = lifted::f820D6C88;  // sub_820D6C88
inline constexpr auto& Part = lifted::entry::e820D6E10;  // sub_820D6E10
inline constexpr auto& SetVinylTextures = lifted::entry::e820D72D8;  // sub_820D72D8
inline constexpr auto& WalkMaterials = lifted::entry::e820D6EB8;  // sub_820D6EB8
}  // namespace game::CarParts

namespace game::CarRecord {
inline constexpr auto& Copy = lifted::entry::e820D7EE8;  // sub_820D7EE8
inline constexpr auto& Reset = lifted::entry::e820D0A98;  // sub_820D0A98
inline constexpr auto& ResetBytes = lifted::entry::e820D0900;  // sub_820D0900
}  // namespace game::CarRecord

namespace game::CarScene {
inline constexpr auto& Create = lifted::entry::e820C8A50;  // sub_820C8A50
inline constexpr auto& Draw = lifted::entry::e820C5DE8;  // sub_820C5DE8
inline constexpr auto& GetTga = lifted::f820C4E68;  // sub_820C4E68
inline constexpr auto& LoadAssets = lifted::entry::e820C8B40;  // sub_820C8B40
inline constexpr auto& LoadsDone = lifted::entry::e820C5798;  // sub_820C5798
inline constexpr auto& OnFileLoaded = lifted::entry::e820C52A0;  // sub_820C52A0
inline constexpr auto& PlayerModel = lifted::entry::e820C4FB8;  // sub_820C4FB8
inline constexpr auto& SetInteriorWord = lifted::entry::e820C8950;  // sub_820C8950
inline constexpr auto& SetUp = lifted::entry::e820C8DE8;  // sub_820C8DE8
inline constexpr auto& StartTeardown = lifted::entry::e820C4D88;  // sub_820C4D88
inline constexpr auto& Update = lifted::entry::e820C9608;  // sub_820C9608
}  // namespace game::CarScene

namespace game::CarSettings {
inline constexpr auto& Apply = lifted::f821AFA08;  // sub_821AFA08
inline constexpr auto& Close = lifted::entry::e821AAF70;  // sub_821AAF70
inline constexpr auto& CoursePage = lifted::f821AB038;  // sub_821AB038
inline constexpr auto& Create = lifted::entry::e821AA2F8;  // sub_821AA2F8
inline constexpr auto& Destroy = lifted::entry::e821AA408;  // sub_821AA408
inline constexpr auto& Draw = lifted::f821B4BA0;  // sub_821B4BA0
inline constexpr auto& Fill = lifted::f821AB2C0;  // sub_821AB2C0
inline constexpr auto& GearAdjust = lifted::f821A8EF8;  // sub_821A8EF8
inline constexpr auto& GearBars = lifted::f821AD840;  // sub_821AD840
inline constexpr auto& GearBarsApplied = lifted::f821ADA08;  // sub_821ADA08
inline constexpr auto& GearPage = lifted::f821AFD78;  // sub_821AFD78
inline constexpr auto& GroupPage1 = lifted::f821AA7B8;  // sub_821AA7B8
inline constexpr auto& GroupPage2 = lifted::f821AA970;  // sub_821AA970
inline constexpr auto& GroupPage3 = lifted::f821AAAB8;  // sub_821AAAB8
inline constexpr auto& GroupPage4 = lifted::f821AAC60;  // sub_821AAC60
inline constexpr auto& Init = lifted::entry::e821B6FC0;  // sub_821B6FC0
inline constexpr auto& InitType5 = lifted::entry::e821B7878;  // sub_821B7878
inline constexpr auto& Leave = lifted::f821AFC88;  // sub_821AFC88
inline constexpr auto& LeavePage = lifted::f821AB250;  // sub_821AB250
inline constexpr auto& OnBack = lifted::f821A8E70;  // sub_821A8E70
inline constexpr auto& OnDialogClosed = lifted::f821A8E88;  // sub_821A8E88
inline constexpr auto& OnNo = lifted::f821A8E28;  // sub_821A8E28
inline constexpr auto& OnYes = lifted::f821A8DE0;  // sub_821A8DE0
inline constexpr auto& OpenMovie = lifted::entry::e821A8DD0;  // sub_821A8DD0
inline constexpr auto& Reenter = lifted::f821AA500;  // sub_821AA500
inline constexpr auto& ReenterToCourses = lifted::f821AA530;  // sub_821AA530
inline constexpr auto& RefitStandCar = lifted::f821AEF48;  // sub_821AEF48
inline constexpr auto& SetBar = lifted::f821ACFB0;  // sub_821ACFB0
inline constexpr auto& SetBarPercent = lifted::f821AD200;  // sub_821AD200
inline constexpr auto& SetMark = lifted::f821AD090;  // sub_821AD090
inline constexpr auto& SetMarkSingle = lifted::f821AD148;  // sub_821AD148
inline constexpr auto& SetUp = lifted::f821AF0A8;  // sub_821AF0A8
inline constexpr auto& SetUpTestRun = lifted::f821AED18;  // sub_821AED18
inline constexpr auto& SettingAdjust10 = lifted::f821A93B0;  // sub_821A93B0
inline constexpr auto& SettingAdjust11 = lifted::f821A9518;  // sub_821A9518
inline constexpr auto& SettingAdjust12 = lifted::f821A9658;  // sub_821A9658
inline constexpr auto& SettingAdjust13 = lifted::f821A9798;  // sub_821A9798
inline constexpr auto& SettingAdjust14 = lifted::f821A98D8;  // sub_821A98D8
inline constexpr auto& SettingAdjust15 = lifted::f821A9A18;  // sub_821A9A18
inline constexpr auto& SettingAdjust16 = lifted::f821A9B58;  // sub_821A9B58
inline constexpr auto& SettingAdjust17 = lifted::f821A9C98;  // sub_821A9C98
inline constexpr auto& SettingAdjust18 = lifted::f821A9DE0;  // sub_821A9DE0
inline constexpr auto& SettingAdjust19 = lifted::f821A9F28;  // sub_821A9F28
inline constexpr auto& SettingAdjust20 = lifted::f821AA070;  // sub_821AA070
inline constexpr auto& SettingAdjust21 = lifted::f821AA1B0;  // sub_821AA1B0
inline constexpr auto& SettingAdjust9 = lifted::f821A9248;  // sub_821A9248
inline constexpr auto& SettingBars10 = lifted::f821ADDB8;  // sub_821ADDB8
inline constexpr auto& SettingBars11 = lifted::f821ADF98;  // sub_821ADF98
inline constexpr auto& SettingBars12 = lifted::f821AE098;  // sub_821AE098
inline constexpr auto& SettingBars13 = lifted::f821AE240;  // sub_821AE240
inline constexpr auto& SettingBars14 = lifted::f821AE340;  // sub_821AE340
inline constexpr auto& SettingBars15 = lifted::f821AE440;  // sub_821AE440
inline constexpr auto& SettingBars16 = lifted::f821AE540;  // sub_821AE540
inline constexpr auto& SettingBars17 = lifted::f821AE748;  // sub_821AE748
inline constexpr auto& SettingBars18 = lifted::f821AE808;  // sub_821AE808
inline constexpr auto& SettingBars19 = lifted::f821AE8C8;  // sub_821AE8C8
inline constexpr auto& SettingBars20 = lifted::f821AE988;  // sub_821AE988
inline constexpr auto& SettingBars21 = lifted::f821AEA88;  // sub_821AEA88
inline constexpr auto& SettingBars9 = lifted::f821ADC48;  // sub_821ADC48
inline constexpr auto& SettingMarks10 = lifted::f821ADE70;  // sub_821ADE70
inline constexpr auto& SettingMarks11 = lifted::f821AE018;  // sub_821AE018
inline constexpr auto& SettingMarks12 = lifted::f821AE118;  // sub_821AE118
inline constexpr auto& SettingMarks13 = lifted::f821AE2C0;  // sub_821AE2C0
inline constexpr auto& SettingMarks14 = lifted::f821AE3C0;  // sub_821AE3C0
inline constexpr auto& SettingMarks15 = lifted::f821AE4C0;  // sub_821AE4C0
inline constexpr auto& SettingMarks16 = lifted::f821AE5C0;  // sub_821AE5C0
inline constexpr auto& SettingMarks17 = lifted::f821AE7A8;  // sub_821AE7A8
inline constexpr auto& SettingMarks18 = lifted::f821AE868;  // sub_821AE868
inline constexpr auto& SettingMarks19 = lifted::f821AE928;  // sub_821AE928
inline constexpr auto& SettingMarks20 = lifted::f821AEA08;  // sub_821AEA08
inline constexpr auto& SettingMarks21 = lifted::f821AEAE8;  // sub_821AEAE8
inline constexpr auto& SettingMarks9 = lifted::f821ADD00;  // sub_821ADD00
inline constexpr auto& SettingPage10 = lifted::f821B0100;  // sub_821B0100
inline constexpr auto& SettingPage11 = lifted::f821B02D0;  // sub_821B02D0
inline constexpr auto& SettingPage12 = lifted::f821B04C8;  // sub_821B04C8
inline constexpr auto& SettingPage13 = lifted::f821B06C0;  // sub_821B06C0
inline constexpr auto& SettingPage14 = lifted::f821B0890;  // sub_821B0890
inline constexpr auto& SettingPage15 = lifted::f821B0A60;  // sub_821B0A60
inline constexpr auto& SettingPage16 = lifted::f821B0C30;  // sub_821B0C30
inline constexpr auto& SettingPage17 = lifted::f821B0E28;  // sub_821B0E28
inline constexpr auto& SettingPage18 = lifted::f821B0FC8;  // sub_821B0FC8
inline constexpr auto& SettingPage19 = lifted::f821B1168;  // sub_821B1168
inline constexpr auto& SettingPage20 = lifted::f821B1308;  // sub_821B1308
inline constexpr auto& SettingPage21 = lifted::f821B14D8;  // sub_821B14D8
inline constexpr auto& SettingPage9 = lifted::f821AFF30;  // sub_821AFF30
inline constexpr auto& Start = lifted::f821AFBD0;  // sub_821AFBD0
inline constexpr auto& StartTestRunPage = lifted::f821AFD28;  // sub_821AFD28
inline constexpr auto& TopPage = lifted::f821AA560;  // sub_821AA560
inline constexpr auto& Unload = lifted::f821AAF08;  // sub_821AAF08
inline constexpr auto& WaitForMovie = lifted::entry::e821AA490;  // sub_821AA490
}  // namespace game::CarSettings

namespace game::CarSound {
inline constexpr auto& PauseAll = lifted::entry::e8210E2B8;  // sub_8210E2B8
inline constexpr auto& ResumeAll = lifted::entry::e8210E350;  // sub_8210E350
inline constexpr auto& Start = lifted::entry::e8210E030;  // sub_8210E030
inline constexpr auto& Stop = lifted::entry::e8210E218;  // sub_8210E218
inline constexpr auto& Update = lifted::entry::e82110048;  // sub_82110048
}  // namespace game::CarSound

namespace game::CarSounds {
inline constexpr auto& Init = lifted::entry::e8210FE00;  // sub_8210FE00
inline constexpr auto& SetBanks = lifted::entry::e8210CB48;  // sub_8210CB48
inline constexpr auto& StopAll = lifted::entry::e8210F7C8;  // sub_8210F7C8
}  // namespace game::CarSounds

namespace game::CarSpec {
inline constexpr auto& GetTire = lifted::entry::e820C9928;  // sub_820C9928
inline constexpr auto& Swap = lifted::entry::e820CC168;  // sub_820CC168
}  // namespace game::CarSpec

namespace game::Career {
inline constexpr auto& AddRivalWin = lifted::f821116E0;  // sub_821116E0
inline constexpr auto& EventQuery = lifted::entry::e82111638;  // sub_82111638
inline constexpr auto& NextDay = lifted::entry::e82116AF0;  // sub_82116AF0
inline constexpr auto& PickFromCar = lifted::entry::e82115CC0;  // sub_82115CC0
inline constexpr auto& PickNext = lifted::entry::e821159B0;  // sub_821159B0
inline constexpr auto& SetUpEvent = lifted::entry::e82114268;  // sub_82114268
}  // namespace game::Career

namespace game::CareerResults {
inline constexpr auto& Close1 = lifted::entry::e822193D8;  // sub_822193D8
inline constexpr auto& Close2 = lifted::f822194A0;  // sub_822194A0
inline constexpr auto& Close3 = lifted::f82219530;  // sub_82219530
inline constexpr auto& Intro = lifted::entry::e82219210;  // sub_82219210
inline constexpr auto& Next2 = lifted::f822195C0;  // sub_822195C0
inline constexpr auto& Next3 = lifted::f82219678;  // sub_82219678
inline constexpr auto& Next4 = lifted::f82219730;  // sub_82219730
inline constexpr auto& NextEntry = lifted::f8221A110;  // sub_8221A110
inline constexpr auto& Page1 = lifted::entry::e82219A78;  // sub_82219A78
inline constexpr auto& SetEntry = lifted::f82219D00;  // sub_82219D00
inline constexpr auto& SetTexts = lifted::entry::e822197A8;  // sub_822197A8
inline constexpr auto& Show = lifted::entry::e822192B0;  // sub_822192B0
inline constexpr auto& Start = lifted::entry::e822191D0;  // sub_822191D0
}  // namespace game::CareerResults

namespace game::ChaseCamera {
inline constexpr auto& Ctor = lifted::entry::e82154548;  // sub_82154548
inline constexpr auto& Update = lifted::entry::e82154808;  // sub_82154808
}  // namespace game::ChaseCamera

namespace game::ChunkChain {
inline constexpr auto& RunInline = lifted::f8258E698;  // sub_8258E698
inline constexpr auto& RunInlineLinked = lifted::f8258E718;  // sub_8258E718
}  // namespace game::ChunkChain

namespace game::ClientList {
inline constexpr auto& Add = lifted::entry::e8259C960;  // sub_8259C960
inline constexpr auto& Ctor = lifted::entry::e8259C7B8;  // sub_8259C7B8
}  // namespace game::ClientList

namespace game::Cockpit {
inline constexpr auto& BindTextures = lifted::entry::e820D6108;  // sub_820D6108
inline constexpr auto& DeletingDestructor = lifted::entry::e820D6798;  // sub_820D6798
inline constexpr auto& OnFileLoaded = lifted::entry::e820D6528;  // sub_820D6528
}  // namespace game::Cockpit

namespace game::CockpitPoints {
inline constexpr auto& Draw = lifted::entry::e820DBD38;  // sub_820DBD38
inline constexpr auto& Find = lifted::entry::e820DBC88;  // sub_820DBC88
}  // namespace game::CockpitPoints

namespace game::CodecBase {
inline constexpr auto& CompletePacket = lifted::entry::e824674D8;  // sub_824674D8
inline constexpr auto& FlushPackets = lifted::entry::e824699E0;  // sub_824699E0
inline constexpr auto& SetFlags = lifted::entry::e82467078;  // sub_82467078
}  // namespace game::CodecBase

namespace game::CollisionObject {
inline constexpr auto& AccumulateForce = lifted::entry::e820B7DF8;  // sub_820B7DF8
inline constexpr auto& AddContact = lifted::entry::e820B7C50;  // sub_820B7C50
inline constexpr auto& Ctor = lifted::entry::e820B7AE8;  // sub_820B7AE8
inline constexpr auto& FirstContact = lifted::entry::e820B7C90;  // sub_820B7C90
inline constexpr auto& Init = lifted::entry::e820B7A00;  // sub_820B7A00
inline constexpr auto& Link = lifted::entry::e820B7BF0;  // sub_820B7BF0
inline constexpr auto& ResetTransform = lifted::entry::e820B7998;  // sub_820B7998
inline constexpr auto& Unlink = lifted::entry::e820B7C18;  // sub_820B7C18
}  // namespace game::CollisionObject

namespace game::CollisionPair {
inline constexpr auto& NextCheckTime = lifted::entry::e820BBB20;  // sub_820BBB20
inline constexpr auto& Solve = lifted::entry::e820BBE30;  // sub_820BBE30
}  // namespace game::CollisionPair

namespace game::CollisionShape {
inline constexpr auto& Current = lifted::entry::e820BAE38;  // sub_820BAE38
inline constexpr auto& Place = lifted::entry::e820BB758;  // sub_820BB758
inline constexpr auto& Previous = lifted::entry::e820BADF8;  // sub_820BADF8
inline constexpr auto& Translate = lifted::entry::e820BACD0;  // sub_820BACD0
inline constexpr auto& Type = lifted::entry::e820BACB8;  // sub_820BACB8
inline constexpr auto& UpdateBounds = lifted::entry::e820BAC00;  // sub_820BAC00
}  // namespace game::CollisionShape

namespace game::CollisionSurface {
inline constexpr auto& A = lifted::entry::e820BC1F8;  // sub_820BC1F8
inline constexpr auto& B = lifted::entry::e820BC240;  // sub_820BC240
inline constexpr auto& C = lifted::entry::e820BC288;  // sub_820BC288
inline constexpr auto& D = lifted::entry::e820BC2D0;  // sub_820BC2D0
}  // namespace game::CollisionSurface

namespace game::CollisionWorld {
inline constexpr auto& AddPairs = lifted::entry::e820B94B0;  // sub_820B94B0
inline constexpr auto& Callback = lifted::entry::e820B8798;  // sub_820B8798
inline constexpr auto& MovePair = lifted::entry::e820BBD30;  // sub_820BBD30
inline constexpr auto& NewContact = lifted::entry::e820B8A48;  // sub_820B8A48
inline constexpr auto& RemoveObject = lifted::entry::e820B9448;  // sub_820B9448
inline constexpr auto& RemovePairsByGroups = lifted::entry::e820B8AF8;  // sub_820B8AF8
inline constexpr auto& Reset = lifted::entry::e820B85E8;  // sub_820B85E8
inline constexpr auto& ResetSurfaces = lifted::entry::e820BC550;  // sub_820BC550
inline constexpr auto& Setup = lifted::entry::e820FF250;  // sub_820FF250
inline constexpr auto& UnlinkPair = lifted::entry::e820BBC40;  // sub_820BBC40
inline constexpr auto& Update = lifted::entry::e820B92C8;  // sub_820B92C8
}  // namespace game::CollisionWorld

namespace game::Color {
inline constexpr auto& HsvToRgb_820D0D60 = lifted::entry::e820D0D60;  // sub_820D0D60
inline constexpr auto& HsvToRgb_8224AB50 = lifted::entry::e8224AB50;  // sub_8224AB50
}  // namespace game::Color

namespace game::ColorFilter {
inline constexpr auto& Dtor = lifted::entry::e82138570;  // sub_82138570
inline constexpr auto& Record = lifted::entry::e821351F0;  // sub_821351F0
inline constexpr auto& Set8112 = lifted::entry::e82135138;  // sub_82135138
inline constexpr auto& Set8116 = lifted::entry::e82135140;  // sub_82135140
inline constexpr auto& Set8120 = lifted::entry::e82135148;  // sub_82135148
inline constexpr auto& SetColor0 = lifted::entry::e821350F8;  // sub_821350F8
}  // namespace game::ColorFilter

namespace game::ColorListScreen {
inline constexpr auto& Ctor = lifted::entry::e822550F8;  // sub_822550F8
inline constexpr auto& DeletingDestructor = lifted::entry::e82255840;  // sub_82255840
inline constexpr auto& Draw = lifted::entry::e82256CA0;  // sub_82256CA0
inline constexpr auto& Dtor = lifted::entry::e82255198;  // sub_82255198
inline constexpr auto& LoadItem = lifted::entry::e82255238;  // sub_82255238
inline constexpr auto& Page2 = lifted::entry::e822552B8;  // sub_822552B8
inline constexpr auto& Page3 = lifted::entry::e82255490;  // sub_82255490
inline constexpr auto& Page4 = lifted::entry::e82255658;  // sub_82255658
inline constexpr auto& Page5 = lifted::entry::e82256508;  // sub_82256508
inline constexpr auto& Page6 = lifted::entry::e822565A0;  // sub_822565A0
inline constexpr auto& Page7 = lifted::entry::e822557F0;  // sub_822557F0
inline constexpr auto& Page9 = lifted::entry::e822562E0;  // sub_822562E0
inline constexpr auto& PlaceColorCursor = lifted::entry::e82256258;  // sub_82256258
inline constexpr auto& PlaceItemCursor = lifted::entry::e822561D0;  // sub_822561D0
inline constexpr auto& Reset = lifted::entry::e82255200;  // sub_82255200
inline constexpr auto& RunMovie = lifted::entry::e82255C88;  // sub_82255C88
inline constexpr auto& ShowPartNodes = lifted::entry::e82256360;  // sub_82256360
inline constexpr auto& Step0 = lifted::entry::e82258578;  // sub_82258578
inline constexpr auto& Step1 = lifted::entry::e82255890;  // sub_82255890
inline constexpr auto& Step10 = lifted::entry::e82255EB0;  // sub_82255EB0
inline constexpr auto& Step2 = lifted::entry::e82256740;  // sub_82256740
inline constexpr auto& Step3 = lifted::entry::e82255B58;  // sub_82255B58
inline constexpr auto& Step4 = lifted::entry::e82256908;  // sub_82256908
inline constexpr auto& Step6 = lifted::entry::e82255CD0;  // sub_82255CD0
inline constexpr auto& Step7 = lifted::entry::e82256AC0;  // sub_82256AC0
inline constexpr auto& Step8 = lifted::entry::e82256030;  // sub_82256030
inline constexpr auto& Update = lifted::entry::e82256E08;  // sub_82256E08
}  // namespace game::ColorListScreen

namespace game::ColorNodeTable {
inline constexpr auto& Clear = lifted::entry::e82247DD0;  // sub_82247DD0
inline constexpr auto& ClearColors = lifted::entry::e82248260;  // sub_82248260
inline constexpr auto& MarkSelected = lifted::entry::e822487B8;  // sub_822487B8
inline constexpr auto& Refresh = lifted::entry::e822483B0;  // sub_822483B0
inline constexpr auto& Reset = lifted::entry::e82248650;  // sub_82248650
inline constexpr auto& SavePairs = lifted::entry::e82247E50;  // sub_82247E50
inline constexpr auto& Set = lifted::entry::e82247E00;  // sub_82247E00
inline constexpr auto& SetColors = lifted::entry::e82248198;  // sub_82248198
}  // namespace game::ColorNodeTable

namespace game::ColorPanel {
inline constexpr auto& Hide = lifted::f8224D090;  // sub_8224D090
inline constexpr auto& IsHidden = lifted::f8224D230;  // sub_8224D230
inline constexpr auto& PlaceCursor = lifted::entry::e8224D3E0;  // sub_8224D3E0
inline constexpr auto& SetArrows = lifted::entry::e8224D2D0;  // sub_8224D2D0
inline constexpr auto& SetCount = lifted::entry::e8224D458;  // sub_8224D458
inline constexpr auto& SetSwatches = lifted::entry::e8224D518;  // sub_8224D518
}  // namespace game::ColorPanel

namespace game::Command {
inline constexpr auto& BeginTiling = lifted::entry::e82590968;  // sub_82590968
inline constexpr auto& Callbacks = lifted::entry::e8258EAE0;  // sub_8258EAE0
inline constexpr auto& Clears = lifted::entry::e82590620;  // sub_82590620
inline constexpr auto& DeferRelease = lifted::entry::e82591798;  // sub_82591798
inline constexpr auto& DrawIndexedVertices = lifted::entry::e825902E0;  // sub_825902E0
inline constexpr auto& DrawIndexedVerticesUP = lifted::entry::e82590698;  // sub_82590698
inline constexpr auto& DrawVertices = lifted::entry::e82590268;  // sub_82590268
inline constexpr auto& DrawVerticesUP = lifted::entry::e82590360;  // sub_82590360
inline constexpr auto& DrawerDraw = lifted::entry::e82590108;  // sub_82590108
inline constexpr auto& DrawerDrawIndexed = lifted::entry::e825901B0;  // sub_825901B0
inline constexpr auto& EndTiling = lifted::entry::e825909E8;  // sub_825909E8
inline constexpr auto& InlineIndices = lifted::entry::e8258EA20;  // sub_8258EA20
inline constexpr auto& InlineVertices = lifted::entry::e8258E9F8;  // sub_8258E9F8
inline constexpr auto& ReleaseResources = lifted::entry::e825903D8;  // sub_825903D8
inline constexpr auto& Resolves = lifted::entry::e825904C8;  // sub_825904C8
inline constexpr auto& RestoreSavedSurfaces = lifted::entry::e82590AD0;  // sub_82590AD0
inline constexpr auto& RestoreSurfaces = lifted::entry::e82590768;  // sub_82590768
inline constexpr auto& SaveSurfaces = lifted::entry::e82590A68;  // sub_82590A68
inline constexpr auto& SetBlendControls = lifted::entry::e8258FC90;  // sub_8258FC90
inline constexpr auto& SetDepthStencilSurfaces = lifted::entry::e82590478;  // sub_82590478
inline constexpr auto& SetIndices = lifted::entry::e825900B8;  // sub_825900B8
inline constexpr auto& SetPixelShaders = lifted::entry::e8258FFA0;  // sub_8258FFA0
inline constexpr auto& SetRenderStates = lifted::entry::e8258FBD0;  // sub_8258FBD0
inline constexpr auto& SetRenderTargets = lifted::entry::e82590428;  // sub_82590428
inline constexpr auto& SetSamplerStates = lifted::entry::e8258FC30;  // sub_8258FC30
inline constexpr auto& SetScissorRects = lifted::entry::e825908E0;  // sub_825908E0
inline constexpr auto& SetShaderConstants = lifted::entry::e8258FD48;  // sub_8258FD48
inline constexpr auto& SetStreamSources = lifted::entry::e82590040;  // sub_82590040
inline constexpr auto& SetTextures = lifted::entry::e8258FCE0;  // sub_8258FCE0
inline constexpr auto& SetUpDrawer = lifted::entry::e8258EA58;  // sub_8258EA58
inline constexpr auto& SetVertexDeclarations = lifted::entry::e8258FFF0;  // sub_8258FFF0
inline constexpr auto& SetVertexShaders = lifted::entry::e8258FF50;  // sub_8258FF50
inline constexpr auto& SetViewports = lifted::entry::e82590860;  // sub_82590860
inline constexpr auto& Skip = lifted::entry::e8258EAD0;  // sub_8258EAD0
}  // namespace game::Command

namespace game::CommandChain {
inline constexpr auto& NewBlock = lifted::entry::e8229EB10;  // sub_8229EB10
}  // namespace game::CommandChain

namespace game::CommandList {
inline constexpr auto& AllocateBytes = lifted::entry::e8258CB98;  // sub_8258CB98
inline constexpr auto& AllocateResolve = lifted::entry::e8258CC30;  // sub_8258CC30
inline constexpr auto& AllocateSlots = lifted::entry::e8258CAA0;  // sub_8258CAA0
inline constexpr auto& AllocateVertices = lifted::entry::e8258CB08;  // sub_8258CB08
inline constexpr auto& BeginTick = lifted::entry::e8258C998;  // sub_8258C998
inline constexpr auto& ChunkSlots = lifted::entry::e8258CA88;  // sub_8258CA88
inline constexpr auto& CloseChunk = lifted::entry::e8258CA50;  // sub_8258CA50
inline constexpr auto& Ctor = lifted::entry::e8258C960;  // sub_8258C960
inline constexpr auto& Flip = lifted::entry::e8258C9C0;  // sub_8258C9C0
inline constexpr auto& OpenChunk = lifted::entry::e8258C9D0;  // sub_8258C9D0
}  // namespace game::CommandList

namespace game::ConfirmPanel {
inline constexpr auto& Ctor = lifted::entry::e8224EBB0;  // sub_8224EBB0
inline constexpr auto& Hide = lifted::f8224EDC0;  // sub_8224EDC0
inline constexpr auto& IsHidden = lifted::f8224EE68;  // sub_8224EE68
inline constexpr auto& IsShown = lifted::f8224EE28;  // sub_8224EE28
inline constexpr auto& Loop = lifted::f8224EDA8;  // sub_8224EDA8
inline constexpr auto& Play = lifted::f8224EBF0;  // sub_8224EBF0
inline constexpr auto& PlayLooped = lifted::f8224EC50;  // sub_8224EC50
inline constexpr auto& Show = lifted::f8224ED38;  // sub_8224ED38
inline constexpr auto& ShowNode = lifted::f8224ECC0;  // sub_8224ECC0
}  // namespace game::ConfirmPanel

namespace game::ConfirmPanel8 {
inline constexpr auto& Ctor = lifted::entry::e8223A208;  // sub_8223A208
}  // namespace game::ConfirmPanel8

namespace game::Contact {
inline constexpr auto& Flip = lifted::entry::e820BC858;  // sub_820BC858
inline constexpr auto& Next = lifted::entry::e820B8D10;  // sub_820B8D10
inline constexpr auto& Object = lifted::entry::e820B8CC0;  // sub_820B8CC0
inline constexpr auto& Position = lifted::entry::e820B8D00;  // sub_820B8D00
inline constexpr auto& Strength = lifted::entry::e820B8D08;  // sub_820B8D08
}  // namespace game::Contact

namespace game::ControlMap {
inline constexpr auto& SetDefault = lifted::entry::e821058A8;  // sub_821058A8
}  // namespace game::ControlMap

namespace game::ControllerOptions {
inline constexpr auto& ChangePage1Value = lifted::f821C5F80;  // sub_821C5F80
inline constexpr auto& ChangePage2Value = lifted::f821C60D0;  // sub_821C60D0
inline constexpr auto& ClosePage0 = lifted::f821BFC68;  // sub_821BFC68
inline constexpr auto& ClosePage1 = lifted::f821BFF20;  // sub_821BFF20
inline constexpr auto& ClosePage2 = lifted::f821C01B8;  // sub_821C01B8
inline constexpr auto& Draw = lifted::entry::e821C7F28;  // sub_821C7F28
inline constexpr auto& HasAnalogClash = lifted::f821BB678;  // sub_821BB678
inline constexpr auto& HasButtonClash = lifted::f821BB4D0;  // sub_821BB4D0
inline constexpr auto& IsAnalogTaken = lifted::f821BB5F8;  // sub_821BB5F8
inline constexpr auto& IsButtonTaken = lifted::f821BB420;  // sub_821BB420
inline constexpr auto& Leave = lifted::entry::e821BFB48;  // sub_821BFB48
inline constexpr auto& MovePage0Cursor = lifted::f821C5D68;  // sub_821C5D68
inline constexpr auto& MovePage1Cursor = lifted::f821BFD98;  // sub_821BFD98
inline constexpr auto& MovePage2Cursor = lifted::f821C0030;  // sub_821C0030
inline constexpr auto& OnEnter = lifted::entry::e821BB120;  // sub_821BB120
inline constexpr auto& OnLeft = lifted::entry::e821BB1F8;  // sub_821BB1F8
inline constexpr auto& Open = lifted::entry::e821BC660;  // sub_821BC660
inline constexpr auto& OpenPage0 = lifted::f821BFBD8;  // sub_821BFBD8
inline constexpr auto& OpenPage1 = lifted::f821BFD08;  // sub_821BFD08
inline constexpr auto& OpenPage2 = lifted::f821BFFA0;  // sub_821BFFA0
inline constexpr auto& Page0 = lifted::f821C5A20;  // sub_821C5A20
inline constexpr auto& Page1 = lifted::f821C5B70;  // sub_821C5B70
inline constexpr auto& Page2 = lifted::f821C5C70;  // sub_821C5C70
inline constexpr auto& RefreshPage0 = lifted::f821C5F38;  // sub_821C5F38
inline constexpr auto& SetMarkY = lifted::entry::e821C0238;  // sub_821C0238
inline constexpr auto& SetUpPage0 = lifted::f821C61B0;  // sub_821C61B0
inline constexpr auto& SetUpPage1 = lifted::f821C6238;  // sub_821C6238
inline constexpr auto& SetUpPage2 = lifted::f821C6350;  // sub_821C6350
inline constexpr auto& Show = lifted::entry::e821BB260;  // sub_821BB260
inline constexpr auto& ShowAnalogClashes = lifted::f821C0750;  // sub_821C0750
inline constexpr auto& ShowAnalogList = lifted::f821C0640;  // sub_821C0640
inline constexpr auto& ShowButtonClashes = lifted::f821C04E0;  // sub_821C04E0
inline constexpr auto& ShowButtonList = lifted::f821C03D8;  // sub_821C03D8
inline constexpr auto& ShowSliderMarks = lifted::f821C0348;  // sub_821C0348
inline constexpr auto& ShowSliders = lifted::f821C02B8;  // sub_821C02B8
inline constexpr auto& StepAnalog = lifted::entry::e821BB590;  // sub_821BB590
inline constexpr auto& StepButtons = lifted::entry::e821BB3B8;  // sub_821BB3B8
inline constexpr auto& StepSliders = lifted::entry::e821BB290;  // sub_821BB290
inline constexpr auto& Update = lifted::entry::e821C7F98;  // sub_821C7F98
}  // namespace game::ControllerOptions

namespace game::Controls {
inline constexpr auto& CycleAnalog = lifted::f821BAD80;  // sub_821BAD80
inline constexpr auto& CycleButton = lifted::f821BA5F0;  // sub_821BA5F0
inline constexpr auto& CycleButtonPair = lifted::f821BA6A0;  // sub_821BA6A0
inline constexpr auto& CycleButtonWide = lifted::f821BA710;  // sub_821BA710
}  // namespace game::Controls

namespace game::CourseRecord {
inline constexpr auto& Entry = lifted::f821AAE90;  // sub_821AAE90
inline constexpr auto& Reset = lifted::entry::e82105C90;  // sub_82105C90
}  // namespace game::CourseRecord

namespace game::CourseRecords {
inline constexpr auto& Of = lifted::entry::e82105BF8;  // sub_82105BF8
}  // namespace game::CourseRecords

namespace game::CourseRoutes {
inline constexpr auto& Get = lifted::entry::e820FFF50;  // sub_820FFF50
}  // namespace game::CourseRoutes

namespace game::CourseSet {
inline constexpr auto& InitParts = lifted::entry::e820F2230;  // sub_820F2230
inline constexpr auto& Load = lifted::entry::e820F2520;  // sub_820F2520
inline constexpr auto& OnLoaded = lifted::entry::e820F22B8;  // sub_820F22B8
inline constexpr auto& Reset = lifted::entry::e820F1138;  // sub_820F1138
}  // namespace game::CourseSet

namespace game::CourseSetup {
inline constexpr auto& Begin = lifted::entry::e82150DE0;  // sub_82150DE0
inline constexpr auto& EndTick = lifted::entry::e821516F0;  // sub_821516F0
inline constexpr auto& Init = lifted::entry::e821517D8;  // sub_821517D8
inline constexpr auto& OnCourseReady = lifted::entry::e82150DD0;  // sub_82150DD0
inline constexpr auto& Restart = lifted::f82151698;  // sub_82151698
inline constexpr auto& Start = lifted::entry::e82151408;  // sub_82151408
inline constexpr auto& WaitCourse = lifted::entry::e821514A0;  // sub_821514A0
inline constexpr auto& WaitDraw = lifted::entry::e821515A0;  // sub_821515A0
}  // namespace game::CourseSetup

namespace game::CourseView {
inline constexpr auto& Draw = lifted::f822282E8;  // sub_822282E8
}  // namespace game::CourseView

namespace game::Cutscene {
inline constexpr auto& Ctor = lifted::entry::e82156CC0;  // sub_82156CC0
inline constexpr auto& OnPad = lifted::f82156248;  // sub_82156248
inline constexpr auto& Update = lifted::entry::e821561F8;  // sub_821561F8
}  // namespace game::Cutscene

namespace game::D3DXBuffer {
inline constexpr auto& Dtor = lifted::f822BE3E8;  // sub_822BE3E8
inline constexpr auto& Release = lifted::f822BE278;  // sub_822BE278
}  // namespace game::D3DXBuffer

namespace game::Dash {
inline constexpr auto& PoseDials = lifted::entry::e820E45A8;  // sub_820E45A8
inline constexpr auto& PoseTachometer = lifted::entry::e820E42E8;  // sub_820E42E8
}  // namespace game::Dash

namespace game::DataFile {
inline constexpr auto& GetEntry = lifted::entry::e82294730;  // sub_82294730
}  // namespace game::DataFile

namespace game::DataTables {
inline constexpr auto& Entry20 = lifted::entry::e82117210;  // sub_82117210
inline constexpr auto& Entry40 = lifted::entry::e82117228;  // sub_82117228
inline constexpr auto& Entry56 = lifted::entry::e82117190;  // sub_82117190
inline constexpr auto& Find20 = lifted::entry::e821171B0;  // sub_821171B0
inline constexpr auto& Init = lifted::entry::e82116DB0;  // sub_82116DB0
inline constexpr auto& IsLoading = lifted::entry::e82117158;  // sub_82117158
inline constexpr auto& Item = lifted::entry::e82117178;  // sub_82117178
inline constexpr auto& Load = lifted::entry::e82117240;  // sub_82117240
inline constexpr auto& On20Loaded = lifted::entry::e82116FF0;  // sub_82116FF0
inline constexpr auto& On40Loaded = lifted::entry::e82117058;  // sub_82117058
inline constexpr auto& On56Loaded = lifted::entry::e821170F0;  // sub_821170F0
inline constexpr auto& OnItemsLoaded = lifted::entry::e821170D8;  // sub_821170D8
inline constexpr auto& OnLoaded84 = lifted::entry::e821170C0;  // sub_821170C0
}  // namespace game::DataTables

namespace game::DayBlock {
inline constexpr auto& WinStreak = lifted::entry::e82110580;  // sub_82110580
inline constexpr auto& Word4832 = lifted::entry::e821105A0;  // sub_821105A0
}  // namespace game::DayBlock

namespace game::DayLog {
inline constexpr auto& Push = lifted::entry::e82112028;  // sub_82112028
inline constexpr auto& SumRaces = lifted::f82110928;  // sub_82110928
inline constexpr auto& SumWord8 = lifted::f82110970;  // sub_82110970
}  // namespace game::DayLog

namespace game::DebugText {
inline constexpr auto& Create = lifted::entry::e82091160;  // sub_82091160
inline constexpr auto& LoadFont = lifted::entry::e82090CC0;  // sub_82090CC0
inline constexpr auto& SetPosition = lifted::entry::e82091050;  // sub_82091050
}  // namespace game::DebugText

namespace game::DecalImages {
inline constexpr auto& FreeTexture = lifted::entry::e821FE6A0;  // sub_821FE6A0
inline constexpr auto& OnLoaded = lifted::entry::e821FE608;  // sub_821FE608
inline constexpr auto& RestoreMovieTexture = lifted::entry::e822081A0;  // sub_822081A0
inline constexpr auto& RestoreStickerTexture = lifted::entry::e821F89B8;  // sub_821F89B8
inline constexpr auto& SavePlaceholders14 = lifted::entry::e821F8950;  // sub_821F8950
}  // namespace game::DecalImages

namespace game::Defaults {
inline constexpr auto& Init = lifted::entry::e820CCDC0;  // sub_820CCDC0
inline constexpr auto& Table3044 = lifted::entry::e820CCCD0;  // sub_820CCCD0
}  // namespace game::Defaults

namespace game::Delegate {
inline constexpr auto& Clone = lifted::entry::e820ACA48;  // sub_820ACA48
inline constexpr auto& Invoke = lifted::f8223A3F0;  // sub_8223A3F0
}  // namespace game::Delegate

namespace game::Device {
inline constexpr auto& AcquireThreadOwnership = lifted::entry::e8229D9D0;  // sub_8229D9D0
inline constexpr auto& AllocateFromCommandBuffer = lifted::entry::e8229E308;  // sub_8229E308
inline constexpr auto& BeginTiledSegments = lifted::entry::e822B6AD8;  // sub_822B6AD8
inline constexpr auto& BeginTiling = lifted::f825BED08;  // sub_825BED08
inline constexpr auto& BlockOnFence = lifted::f8229EAF0;  // sub_8229EAF0
inline constexpr auto& Clear = lifted::f822A04F8;  // sub_822A04F8
inline constexpr auto& CreateIndexBuffer = lifted::entry::e822A6968;  // sub_822A6968
inline constexpr auto& CreateSurface = lifted::entry::e822A5E28;  // sub_822A5E28
inline constexpr auto& CreateTexture = lifted::entry::e822A5D08;  // sub_822A5D08
inline constexpr auto& CreateVertexBuffer = lifted::entry::e822A6830;  // sub_822A6830
inline constexpr auto& DeferredKick = lifted::entry::e8229E848;  // sub_8229E848
inline constexpr auto& DirtyAllState = lifted::entry::e822A3B90;  // sub_822A3B90
inline constexpr auto& EndSegment = lifted::entry::e8229EF48;  // sub_8229EF48
inline constexpr auto& EndTiledSegments = lifted::entry::e822B6BF8;  // sub_822B6BF8
inline constexpr auto& EndTiling = lifted::f825BF200;  // sub_825BF200
inline constexpr auto& GetDepthStencilSurface = lifted::entry::e822A39E0;  // sub_822A39E0
inline constexpr auto& GetRenderState_ViewportEnable = lifted::entry::e822A2470;  // sub_822A2470
inline constexpr auto& GetRenderTarget = lifted::entry::e822A3998;  // sub_822A3998
inline constexpr auto& GetStreamSource = lifted::entry::e822A3880;  // sub_822A3880
inline constexpr auto& GetViewport = lifted::entry::e822A3740;  // sub_822A3740
inline constexpr auto& Init = lifted::entry::e822AD298;  // sub_822AD298
inline constexpr auto& InsertCallback = lifted::entry::e8229F228;  // sub_8229F228
inline constexpr auto& IsFencePending = lifted::f8229F068;  // sub_8229F068
inline constexpr auto& Kick = lifted::entry::e8229E3F0;  // sub_8229E3F0
inline constexpr auto& LoadShaders = lifted::f822AED18;  // sub_822AED18
inline constexpr auto& LockL2 = lifted::f822B6070;  // sub_822B6070
inline constexpr auto& MaskTiledDrawBlock = lifted::f822B6188;  // sub_822B6188
inline constexpr auto& MaskTiledDraws = lifted::f822B6288;  // sub_822B6288
inline constexpr auto& PatchVertexShader = lifted::entry::e822AE340;  // sub_822AE340
inline constexpr auto& Release = lifted::f822AD500;  // sub_822AD500
inline constexpr auto& ReleaseThreadOwnership = lifted::entry::e8229DA10;  // sub_8229DA10
inline constexpr auto& ReserveWords = lifted::entry::e8229F148;  // sub_8229F148
inline constexpr auto& Resolve = lifted::entry::e822AFA58;  // sub_822AFA58
inline constexpr auto& SetBlendControl = lifted::entry::e822A3B30;  // sub_822A3B30
inline constexpr auto& SetDepthStencilSurface = lifted::entry::e822A4060;  // sub_822A4060
inline constexpr auto& SetDisplayMode = lifted::entry::e822AD0B0;  // sub_822AD0B0
inline constexpr auto& SetFVF = lifted::entry::e822AA0E8;  // sub_822AA0E8
inline constexpr auto& SetIndices = lifted::entry::e822A3908;  // sub_822A3908
inline constexpr auto& SetOutOfSpace = lifted::f8229E098;  // sub_8229E098
inline constexpr auto& SetPixelShader = lifted::entry::e822A8618;  // sub_822A8618
inline constexpr auto& SetPixelShaderConstantB = lifted::entry::e822A8470;  // sub_822A8470
inline constexpr auto& SetPixelShaderConstantF = lifted::entry::e822A8308;  // sub_822A8308
inline constexpr auto& SetPredication = lifted::entry::e825BEB80;  // sub_825BEB80
inline constexpr auto& SetRenderState_AlphaBlendEnable = lifted::entry::e822A1300;  // sub_822A1300
inline constexpr auto& SetRenderState_AlphaFunc = lifted::entry::e822A1790;  // sub_822A1790
inline constexpr auto& SetRenderState_AlphaRef = lifted::entry::e822A1728;  // sub_822A1728
inline constexpr auto& SetRenderState_AlphaTestEnable = lifted::entry::e822A12C0;  // sub_822A12C0
inline constexpr auto& SetRenderState_AlphaToMaskEnable = lifted::entry::e822A2870;  // sub_822A2870
inline constexpr auto& SetRenderState_AlphaToMaskOffsets = lifted::entry::e822A28A0;  // sub_822A28A0
inline constexpr auto& SetRenderState_BlendFactor = lifted::entry::e822A17C0;  // sub_822A17C0
inline constexpr auto& SetRenderState_BlendOp = lifted::entry::e822A1390;  // sub_822A1390
inline constexpr auto& SetRenderState_BlendOpAlpha = lifted::entry::e822A1540;  // sub_822A1540
inline constexpr auto& SetRenderState_CcwStencilFail = lifted::entry::e822A1B28;  // sub_822A1B28
inline constexpr auto& SetRenderState_CcwStencilFunc = lifted::entry::e822A1AF8;  // sub_822A1AF8
inline constexpr auto& SetRenderState_CcwStencilMask = lifted::entry::e822A1C58;  // sub_822A1C58
inline constexpr auto& SetRenderState_CcwStencilPass = lifted::entry::e822A1BA8;  // sub_822A1BA8
inline constexpr auto& SetRenderState_CcwStencilRef = lifted::entry::e822A1C38;  // sub_822A1C38
inline constexpr auto& SetRenderState_CcwStencilWriteMask = lifted::entry::e822A1C78;  // sub_822A1C78
inline constexpr auto& SetRenderState_CcwStencilZFail = lifted::entry::e822A1B68;  // sub_822A1B68
inline constexpr auto& SetRenderState_ClipPlaneEnable = lifted::entry::e822A1C98;  // sub_822A1C98
inline constexpr auto& SetRenderState_ColorWriteEnable = lifted::entry::e822A1EB0;  // sub_822A1EB0
inline constexpr auto& SetRenderState_ColorWriteEnable1 = lifted::entry::e822A1EE8;  // sub_822A1EE8
inline constexpr auto& SetRenderState_ColorWriteEnable2 = lifted::entry::e822A1F20;  // sub_822A1F20
inline constexpr auto& SetRenderState_ColorWriteEnable3 = lifted::entry::e822A1F58;  // sub_822A1F58
inline constexpr auto& SetRenderState_CullMode = lifted::entry::e822A1260;  // sub_822A1260
inline constexpr auto& SetRenderState_DepthBias = lifted::entry::e822A1DB8;  // sub_822A1DB8
inline constexpr auto& SetRenderState_DestBlend = lifted::entry::e822A14B0;  // sub_822A14B0
inline constexpr auto& SetRenderState_DestBlendAlpha = lifted::entry::e822A1620;  // sub_822A1620
inline constexpr auto& SetRenderState_DiscardBandX = lifted::entry::e822A2938;  // sub_822A2938
inline constexpr auto& SetRenderState_DiscardBandY = lifted::entry::e822A2960;  // sub_822A2960
inline constexpr auto& SetRenderState_FillMode = lifted::entry::e822A1290;  // sub_822A1290
inline constexpr auto& SetRenderState_GuardBandX = lifted::entry::e822A28D8;  // sub_822A28D8
inline constexpr auto& SetRenderState_GuardBandY = lifted::entry::e822A2908;  // sub_822A2908
inline constexpr auto& SetRenderState_HalfPixelOffset = lifted::entry::e822A27E8;  // sub_822A27E8
inline constexpr auto& SetRenderState_HiStencilEnable = lifted::entry::e822A2990;  // sub_822A2990
inline constexpr auto& SetRenderState_HiStencilFunc = lifted::entry::e822A29F0;  // sub_822A29F0
inline constexpr auto& SetRenderState_HiStencilRef = lifted::entry::e822A2A20;  // sub_822A2A20
inline constexpr auto& SetRenderState_HiStencilWriteEnable = lifted::entry::e822A29C0;  // sub_822A29C0
inline constexpr auto& SetRenderState_HighPrecisionBlendEnable = lifted::entry::e822A2480;  // sub_822A2480
inline constexpr auto& SetRenderState_HighPrecisionBlendEnable1 = lifted::entry::e822A2530;  // sub_822A2530
inline constexpr auto& SetRenderState_HighPrecisionBlendEnable2 = lifted::entry::e822A25E0;  // sub_822A25E0
inline constexpr auto& SetRenderState_HighPrecisionBlendEnable3 = lifted::entry::e822A2690;  // sub_822A2690
inline constexpr auto& SetRenderState_MaxTessellationLevel = lifted::entry::e822A2778;  // sub_822A2778
inline constexpr auto& SetRenderState_MinTessellationLevel = lifted::entry::e822A2740;  // sub_822A2740
inline constexpr auto& SetRenderState_MultiSampleAntiAlias = lifted::entry::e822A1E60;  // sub_822A1E60
inline constexpr auto& SetRenderState_MultiSampleMask = lifted::entry::e822A1E90;  // sub_822A1E90
inline constexpr auto& SetRenderState_PointSize = lifted::entry::e822A1FA0;  // sub_822A1FA0
inline constexpr auto& SetRenderState_PointSizeMax = lifted::entry::e822A2058;  // sub_822A2058
inline constexpr auto& SetRenderState_PointSizeMin = lifted::entry::e822A2000;  // sub_822A2000
inline constexpr auto& SetRenderState_PointSpriteEnable = lifted::entry::e822A1F90;  // sub_822A1F90
inline constexpr auto& SetRenderState_PresentImmediateThreshold = lifted::entry::e822A2A50;  // sub_822A2A50
inline constexpr auto& SetRenderState_PresentInterval = lifted::entry::e822A2A40;  // sub_822A2A40
inline constexpr auto& SetRenderState_PrimitiveResetEnable = lifted::entry::e822A2820;  // sub_822A2820
inline constexpr auto& SetRenderState_PrimitiveResetIndex = lifted::entry::e822A2850;  // sub_822A2850
inline constexpr auto& SetRenderState_ScissorTestEnable = lifted::entry::e822A3D20;  // sub_822A3D20
inline constexpr auto& SetRenderState_SeparateAlphaBlendEnable = lifted::entry::e822A1690;  // sub_822A1690
inline constexpr auto& SetRenderState_SlopeScaleDepthBias = lifted::entry::e822A1CF0;  // sub_822A1CF0
inline constexpr auto& SetRenderState_SrcBlend = lifted::entry::e822A1420;  // sub_822A1420
inline constexpr auto& SetRenderState_SrcBlendAlpha = lifted::entry::e822A15B0;  // sub_822A15B0
inline constexpr auto& SetRenderState_StencilEnable = lifted::entry::e822A1990;  // sub_822A1990
inline constexpr auto& SetRenderState_StencilFail = lifted::entry::e822A1A48;  // sub_822A1A48
inline constexpr auto& SetRenderState_StencilFunc = lifted::entry::e822A1A18;  // sub_822A1A18
inline constexpr auto& SetRenderState_StencilMask = lifted::entry::e822A1BF8;  // sub_822A1BF8
inline constexpr auto& SetRenderState_StencilPass = lifted::entry::e822A1AC8;  // sub_822A1AC8
inline constexpr auto& SetRenderState_StencilRef = lifted::entry::e822A1BD8;  // sub_822A1BD8
inline constexpr auto& SetRenderState_StencilWriteMask = lifted::entry::e822A1C18;  // sub_822A1C18
inline constexpr auto& SetRenderState_StencilZFail = lifted::entry::e822A1A88;  // sub_822A1A88
inline constexpr auto& SetRenderState_TessellationMode = lifted::entry::e822A27B0;  // sub_822A27B0
inline constexpr auto& SetRenderState_TwoSidedStencilMode = lifted::entry::e822A19D8;  // sub_822A19D8
inline constexpr auto& SetRenderState_ViewportEnable = lifted::entry::e822A2430;  // sub_822A2430
inline constexpr auto& SetRenderState_Wrap0 = lifted::entry::e822A20B0;  // sub_822A20B0
inline constexpr auto& SetRenderState_Wrap1 = lifted::entry::e822A20D8;  // sub_822A20D8
inline constexpr auto& SetRenderState_Wrap10 = lifted::entry::e822A2278;  // sub_822A2278
inline constexpr auto& SetRenderState_Wrap11 = lifted::entry::e822A22A8;  // sub_822A22A8
inline constexpr auto& SetRenderState_Wrap12 = lifted::entry::e822A22D8;  // sub_822A22D8
inline constexpr auto& SetRenderState_Wrap13 = lifted::entry::e822A2308;  // sub_822A2308
inline constexpr auto& SetRenderState_Wrap14 = lifted::entry::e822A2338;  // sub_822A2338
inline constexpr auto& SetRenderState_Wrap15 = lifted::entry::e822A2368;  // sub_822A2368
inline constexpr auto& SetRenderState_Wrap2 = lifted::entry::e822A2108;  // sub_822A2108
inline constexpr auto& SetRenderState_Wrap3 = lifted::entry::e822A2138;  // sub_822A2138
inline constexpr auto& SetRenderState_Wrap4 = lifted::entry::e822A2168;  // sub_822A2168
inline constexpr auto& SetRenderState_Wrap5 = lifted::entry::e822A2198;  // sub_822A2198
inline constexpr auto& SetRenderState_Wrap6 = lifted::entry::e822A21C8;  // sub_822A21C8
inline constexpr auto& SetRenderState_Wrap7 = lifted::entry::e822A21F8;  // sub_822A21F8
inline constexpr auto& SetRenderState_Wrap8 = lifted::entry::e822A2220;  // sub_822A2220
inline constexpr auto& SetRenderState_Wrap9 = lifted::entry::e822A2248;  // sub_822A2248
inline constexpr auto& SetRenderState_ZEnable = lifted::entry::e822A18D8;  // sub_822A18D8
inline constexpr auto& SetRenderState_ZFunc = lifted::entry::e822A1950;  // sub_822A1950
inline constexpr auto& SetRenderState_ZWriteEnable = lifted::entry::e822A1920;  // sub_822A1920
inline constexpr auto& SetRenderTarget_External = lifted::entry::e822A4340;  // sub_822A4340
inline constexpr auto& SetSamplerState_AddressU = lifted::entry::e822A31A0;  // sub_822A31A0
inline constexpr auto& SetSamplerState_AddressV = lifted::entry::e822A31F0;  // sub_822A31F0
inline constexpr auto& SetSamplerState_AddressW = lifted::entry::e822A3240;  // sub_822A3240
inline constexpr auto& SetSamplerState_AnisotropyBias = lifted::entry::e822A2EE8;  // sub_822A2EE8
inline constexpr auto& SetSamplerState_BorderColor = lifted::entry::e822A3130;  // sub_822A3130
inline constexpr auto& SetSamplerState_HGradientExpBias = lifted::entry::e822A32E8;  // sub_822A32E8
inline constexpr auto& SetSamplerState_MagFilter = lifted::entry::e822A2BF8;  // sub_822A2BF8
inline constexpr auto& SetSamplerState_MagFilterZ = lifted::entry::e822A2CE8;  // sub_822A2CE8
inline constexpr auto& SetSamplerState_MaxAnisotropy = lifted::entry::e822A2E70;  // sub_822A2E70
inline constexpr auto& SetSamplerState_MaxMipLevel = lifted::entry::e822A3030;  // sub_822A3030
inline constexpr auto& SetSamplerState_MinFilter = lifted::entry::e822A2A70;  // sub_822A2A70
inline constexpr auto& SetSamplerState_MinFilterZ = lifted::entry::e822A2B60;  // sub_822A2B60
inline constexpr auto& SetSamplerState_MinMipLevel = lifted::entry::e822A30B0;  // sub_822A30B0
inline constexpr auto& SetSamplerState_MipFilter = lifted::entry::e822A2D80;  // sub_822A2D80
inline constexpr auto& SetSamplerState_MipMapLodBias = lifted::entry::e822A2F90;  // sub_822A2F90
inline constexpr auto& SetSamplerState_PointBorderEnable = lifted::entry::e822A33F0;  // sub_822A33F0
inline constexpr auto& SetSamplerState_SeparateZFilterEnable = lifted::entry::e822A2DD8;  // sub_822A2DD8
inline constexpr auto& SetSamplerState_TrilinearThreshold = lifted::entry::e822A3290;  // sub_822A3290
inline constexpr auto& SetSamplerState_VGradientExpBias = lifted::entry::e822A3340;  // sub_822A3340
inline constexpr auto& SetSamplerState_WhiteBorderColorW = lifted::entry::e822A3398;  // sub_822A3398
inline constexpr auto& SetScissorRect = lifted::entry::e822A3450;  // sub_822A3450
inline constexpr auto& SetScreenScissor = lifted::entry::e822A0F80;  // sub_822A0F80
inline constexpr auto& SetShaderGprAllocation = lifted::entry::e822A8A60;  // sub_822A8A60
inline constexpr auto& SetStreamSource = lifted::entry::e822A3758;  // sub_822A3758
inline constexpr auto& SetSurfaceInfo = lifted::entry::e822A3C88;  // sub_822A3C88
inline constexpr auto& SetTexture = lifted::entry::e822A6008;  // sub_822A6008
inline constexpr auto& SetUpDisplay = lifted::entry::e822ACE00;  // sub_822ACE00
inline constexpr auto& SetUpRing = lifted::entry::e8229F280;  // sub_8229F280
inline constexpr auto& SetUpScaler = lifted::entry::e822B3FE0;  // sub_822B3FE0
inline constexpr auto& SetVertexDeclaration = lifted::entry::e822A8960;  // sub_822A8960
inline constexpr auto& SetVertexShaderConstantF = lifted::entry::e822A81F8;  // sub_822A81F8
inline constexpr auto& SetViewport = lifted::entry::e822A3518;  // sub_822A3518
inline constexpr auto& Swap = lifted::entry::e822A08A0;  // sub_822A08A0
inline constexpr auto& WaitForIdle = lifted::entry::e8229F1B0;  // sub_8229F1B0
inline constexpr auto& WaitForWorkers = lifted::f822B64A0;  // sub_822B64A0
inline constexpr auto& WakeWorker = lifted::entry::e822B6A78;  // sub_822B6A78
inline constexpr auto& WorkerJob = lifted::entry::e822B6780;  // sub_822B6780
inline constexpr auto& WorkerMarkers = lifted::f822B6528;  // sub_822B6528
inline constexpr auto& WorkerNextBlock = lifted::f822B60C8;  // sub_822B60C8
inline constexpr auto& WorkerThread = lifted::entry::e822B69A8;  // sub_822B69A8
inline constexpr auto& WriteBinSelect = lifted::f822B6370;  // sub_822B6370
inline constexpr auto& WriteCallback = lifted::entry::e8229E9D0;  // sub_8229E9D0
inline constexpr auto& WriteDefaultDrawRegisters = lifted::entry::e822AF978;  // sub_822AF978
inline constexpr auto& WriteFlushPackets = lifted::entry::e8229E5D0;  // sub_8229E5D0
inline constexpr auto& WriteIdlePackets = lifted::entry::e8229F0E0;  // sub_8229F0E0
inline constexpr auto& WriteSegmentTail = lifted::entry::e8229E768;  // sub_8229E768
}  // namespace game::Device

namespace game::Dialog {
inline constexpr auto& CloseConfirmStep = lifted::entry::e8223AA10;  // sub_8223AA10
inline constexpr auto& CloseNoticeStep = lifted::entry::e8223AAA8;  // sub_8223AAA8
inline constexpr auto& DrawWaiting = lifted::entry::e82241C18;  // sub_82241C18
inline constexpr auto& Leave = lifted::entry::e82225C70;  // sub_82225C70
inline constexpr auto& MoveCursorStep = lifted::entry::e8223AB50;  // sub_8223AB50
inline constexpr auto& OpenConfirmStep = lifted::entry::e822325E0;  // sub_822325E0
inline constexpr auto& OpenMovies = lifted::entry::e82225810;  // sub_82225810
inline constexpr auto& OpenNoticeStep = lifted::entry::e822326A8;  // sub_822326A8
}  // namespace game::Dialog

namespace game::Dxt {
inline constexpr auto& Unpremultiply = lifted::f82420640;  // sub_82420640
}  // namespace game::Dxt

namespace game::Dxt1 {
inline constexpr auto& EncodeBlock = lifted::f82421EB0;  // sub_82421EB0
}  // namespace game::Dxt1

namespace game::Dxt23 {
inline constexpr auto& DecodeBlock = lifted::f82422A20;  // sub_82422A20
inline constexpr auto& DecodeBlockPremultiplied = lifted::f82422848;  // sub_82422848
inline constexpr auto& DecodeBlockStraight = lifted::f82422738;  // sub_82422738
inline constexpr auto& EncodeBlock = lifted::f82422A50;  // sub_82422A50
inline constexpr auto& EncodeBlockPremultiplied = lifted::f824228D8;  // sub_824228D8
inline constexpr auto& EncodeBlockStraight = lifted::f824227F0;  // sub_824227F0
}  // namespace game::Dxt23

namespace game::Dxt3A {
inline constexpr auto& DecodeBlock = lifted::f82421BF0;  // sub_82421BF0
inline constexpr auto& EncodeBlock = lifted::f82422000;  // sub_82422000
}  // namespace game::Dxt3A

namespace game::Dxt45 {
inline constexpr auto& DecodeBlock = lifted::f82422A38;  // sub_82422A38
inline constexpr auto& DecodeBlockPremultiplied = lifted::f82422890;  // sub_82422890
inline constexpr auto& DecodeBlockStraight = lifted::f82422798;  // sub_82422798
inline constexpr auto& EncodeBlock = lifted::entry::e82422A68;  // sub_82422A68
}  // namespace game::Dxt45

namespace game::DxtAlpha {
inline constexpr auto& DecodeBlock = lifted::f82421C88;  // sub_82421C88
}  // namespace game::DxtAlpha

namespace game::DxtCodec {
inline constexpr auto& DecodeRow = lifted::f823F6858;  // sub_823F6858
inline constexpr auto& EncodeRow = lifted::entry::e823F6298;  // sub_823F6298
inline constexpr auto& QuantizeColorKey = lifted::f823F6170;  // sub_823F6170
}  // namespace game::DxtCodec

namespace game::DxtColor {
inline constexpr auto& DecodeBlock = lifted::f82421938;  // sub_82421938
inline constexpr auto& FitEndpoints = lifted::entry::e82420AB8;  // sub_82420AB8
inline constexpr auto& PackR5G6B5 = lifted::entry::e824206E8;  // sub_824206E8
}  // namespace game::DxtColor

namespace game::Emitter {
inline constexpr auto& Ctor = lifted::entry::e8259C9B0;  // sub_8259C9B0
inline constexpr auto& DestroyCue = lifted::f8259CBE8;  // sub_8259CBE8
inline constexpr auto& IsNotStopped = lifted::f8259CCA0;  // sub_8259CCA0
inline constexpr auto& IsPrepared = lifted::f8259CCF8;  // sub_8259CCF8
inline constexpr auto& Play = lifted::f8259CC38;  // sub_8259CC38
inline constexpr auto& Prepare = lifted::f8259CB48;  // sub_8259CB48
inline constexpr auto& Stop = lifted::f8259CC58;  // sub_8259CC58
}  // namespace game::Emitter

namespace game::EngineLayer {
inline constexpr auto& Dtor = lifted::f8210CDE8;  // sub_8210CDE8
}  // namespace game::EngineLayer

namespace game::Event {
inline constexpr auto& Create = lifted::f8258DCC8;  // sub_8258DCC8
inline constexpr auto& Signal = lifted::f8258DD78;  // sub_8258DD78
inline constexpr auto& Wait = lifted::f8258DD90;  // sub_8258DD90
}  // namespace game::Event

namespace game::EventCondition {
inline constexpr auto& Power = lifted::f82141668;  // sub_82141668
}  // namespace game::EventCondition

namespace game::EventGroup {
inline constexpr auto& Alloc = lifted::entry::e821433A8;  // sub_821433A8
inline constexpr auto& Free = lifted::entry::e82143358;  // sub_82143358
}  // namespace game::EventGroup

namespace game::EventPools {
inline constexpr auto& AddRivals = lifted::entry::e82144458;  // sub_82144458
inline constexpr auto& CopyIds = lifted::entry::e82143420;  // sub_82143420
}  // namespace game::EventPools

namespace game::File {
inline constexpr auto& Close = lifted::entry::e8258C718;  // sub_8258C718
inline constexpr auto& GetSize = lifted::entry::e822B80D8;  // sub_822B80D8
inline constexpr auto& OpenRead = lifted::entry::e8258C688;  // sub_8258C688
inline constexpr auto& OpenWrite = lifted::f8258C608;  // sub_8258C608
inline constexpr auto& Read = lifted::entry::e8258C820;  // sub_8258C820
inline constexpr auto& Seek = lifted::entry::e8258C768;  // sub_8258C768
inline constexpr auto& Write = lifted::f8258C898;  // sub_8258C898
}  // namespace game::File

namespace game::FloatPairItem {
inline constexpr auto& Ctor = lifted::f820A70F8;  // sub_820A70F8
inline constexpr auto& Dtor = lifted::f820A7180;  // sub_820A7180
}  // namespace game::FloatPairItem

namespace game::Font {
inline constexpr auto& ClipGlyph = lifted::entry::e82108C90;  // sub_82108C90
inline constexpr auto& Ctor = lifted::entry::e82109228;  // sub_82109228
inline constexpr auto& Draw = lifted::entry::e8210A400;  // sub_8210A400
inline constexpr auto& DrawPass = lifted::entry::e82109EF0;  // sub_82109EF0
inline constexpr auto& LineGlyphs = lifted::entry::e82108810;  // sub_82108810
inline constexpr auto& LineWidth = lifted::entry::e82108458;  // sub_82108458
inline constexpr auto& OnPageLoaded = lifted::f82109160;  // sub_82109160
inline constexpr auto& TextWidth = lifted::entry::e82108B30;  // sub_82108B30
}  // namespace game::Font

namespace game::FrameDriver {
inline constexpr auto& AddListener = lifted::entry::e820922B8;  // sub_820922B8
inline constexpr auto& ApplyRequest = lifted::entry::e82091FF0;  // sub_82091FF0
inline constexpr auto& CallListener = lifted::entry::e82091A98;  // sub_82091A98
inline constexpr auto& Create = lifted::f820925A0;  // sub_820925A0
inline constexpr auto& Dtor_820921A8 = lifted::f820921A8;  // sub_820921A8
inline constexpr auto& Dtor_82092268 = lifted::f82092268;  // sub_82092268
inline constexpr auto& GetVideoOutput = lifted::entry::e824960E0;  // sub_824960E0
inline constexpr auto& Init = lifted::f82092358;  // sub_82092358
inline constexpr auto& OnEvent6 = lifted::f82091DC0;  // sub_82091DC0
inline constexpr auto& Present = lifted::entry::e82091A60;  // sub_82091A60
inline constexpr auto& RemoveListener = lifted::f82092318;  // sub_82092318
inline constexpr auto& StartFrame = lifted::f82091A58;  // sub_82091A58
inline constexpr auto& ThreadLoop = lifted::entry::e82091EF8;  // sub_82091EF8
inline constexpr auto& WaitForFrame = lifted::entry::e820920D8;  // sub_820920D8
}  // namespace game::FrameDriver

namespace game::FramePasses {
inline constexpr auto& Boot = lifted::entry::e820962B8;  // sub_820962B8
inline constexpr auto& Ctor = lifted::entry::e82098630;  // sub_82098630
inline constexpr auto& Dtor = lifted::f820987E8;  // sub_820987E8
inline constexpr auto& FlipWorkMem = lifted::entry::e82096218;  // sub_82096218
inline constexpr auto& OnEvent = lifted::f82096220;  // sub_82096220
inline constexpr auto& Record = lifted::entry::e82096418;  // sub_82096418
inline constexpr auto& RecordConstants = lifted::entry::e82096360;  // sub_82096360
inline constexpr auto& RecordSamplers = lifted::entry::e82096030;  // sub_82096030
}  // namespace game::FramePasses

namespace game::FrameStart {
inline constexpr auto& Ctor = lifted::entry::e82092710;  // sub_82092710
inline constexpr auto& OnTick = lifted::f820925F8;  // sub_820925F8
}  // namespace game::FrameStart

namespace game::Framework {
inline constexpr auto& DeviceList = lifted::entry::e820908D8;  // sub_820908D8
inline constexpr auto& Init = lifted::f82090A08;  // sub_82090A08
inline constexpr auto& OnTick = lifted::entry::e820902C8;  // sub_820902C8
inline constexpr auto& Start = lifted::entry::e820902B0;  // sub_820902B0
}  // namespace game::Framework

namespace game::FrontEndMode {
inline constexpr auto& NextMode = lifted::f8222FCA0;  // sub_8222FCA0
}  // namespace game::FrontEndMode

namespace game::G16R16Codec {
inline constexpr auto& DecodeRow = lifted::f823F2FF8;  // sub_823F2FF8
inline constexpr auto& EncodeRow = lifted::f823EDFD8;  // sub_823EDFD8
}  // namespace game::G16R16Codec

namespace game::G16R16FCodec {
inline constexpr auto& DecodeRow = lifted::f823F4738;  // sub_823F4738
inline constexpr auto& EncodeRow = lifted::f823F1640;  // sub_823F1640
}  // namespace game::G16R16FCodec

namespace game::G32R32FCodec {
inline constexpr auto& DecodeRow = lifted::f823F5468;  // sub_823F5468
inline constexpr auto& EncodeRow = lifted::f823F2530;  // sub_823F2530
}  // namespace game::G32R32FCodec

namespace game::GameData {
inline constexpr auto& Check = lifted::entry::e8228FC50;  // sub_8228FC50
inline constexpr auto& Load = lifted::entry::e8228FBC0;  // sub_8228FBC0
inline constexpr auto& Save = lifted::entry::e8228FB00;  // sub_8228FB00
}  // namespace game::GameData

namespace game::GameFile {
inline constexpr auto& Ctor = lifted::entry::e820A6A00;  // sub_820A6A00
inline constexpr auto& Dtor = lifted::entry::e820A6A40;  // sub_820A6A40
inline constexpr auto& OpenAsync = lifted::f820A6B20;  // sub_820A6B20
inline constexpr auto& OpenWrite = lifted::f820A6A50;  // sub_820A6A50
}  // namespace game::GameFile

namespace game::GameMessages {
inline constexpr auto& Dispatch = lifted::entry::e821062E8;  // sub_821062E8
}  // namespace game::GameMessages

namespace game::GamePath {
inline constexpr auto& Ctor = lifted::entry::e820A6BE0;  // sub_820A6BE0
inline constexpr auto& Set = lifted::f820A6C68;  // sub_820A6C68
inline constexpr auto& SetGame = lifted::entry::e820A6BF0;  // sub_820A6BF0
}  // namespace game::GamePath

namespace game::Garage {
inline constexpr auto& Add = lifted::entry::e82116CE0;  // sub_82116CE0
inline constexpr auto& Remove = lifted::f82116BE0;  // sub_82116BE0
inline constexpr auto& Sell = lifted::f82116D88;  // sub_82116D88
}  // namespace game::Garage

namespace game::GarageAnims {
inline constexpr auto& CloseDoors = lifted::f8217B740;  // sub_8217B740
inline constexpr auto& CloseDoorsA = lifted::f8217A008;  // sub_8217A008
inline constexpr auto& CloseDoorsB = lifted::f8217A1D0;  // sub_8217A1D0
inline constexpr auto& OpenDoors = lifted::f8217B6B8;  // sub_8217B6B8
inline constexpr auto& OpenDoorsA = lifted::f82179F60;  // sub_82179F60
inline constexpr auto& OpenDoorsB = lifted::f8217A0A8;  // sub_8217A0A8
}  // namespace game::GarageAnims

namespace game::GarageCamera {
inline constexpr auto& FreePresets = lifted::entry::e8217D760;  // sub_8217D760
inline constexpr auto& OnPresetsLoaded = lifted::entry::e8217FBE8;  // sub_8217FBE8
inline constexpr auto& Reset = lifted::entry::e8217D888;  // sub_8217D888
inline constexpr auto& Update = lifted::entry::e8217BE98;  // sub_8217BE98
}  // namespace game::GarageCamera

namespace game::GarageCar {
inline constexpr auto& Reset = lifted::entry::e82110448;  // sub_82110448
}  // namespace game::GarageCar

namespace game::GarageCars {
inline constexpr auto& StaticInit = lifted::entry::e825D9EF0;  // sub_825D9EF0
}  // namespace game::GarageCars

namespace game::GarageList {
inline constexpr auto& Remove = lifted::f82113400;  // sub_82113400
}  // namespace game::GarageList

namespace game::GarageScreen {
inline constexpr auto& Close = lifted::entry::e82192850;  // sub_82192850
inline constexpr auto& Create = lifted::entry::e82191C58;  // sub_82191C58
inline constexpr auto& Destroy = lifted::entry::e821915B0;  // sub_821915B0
inline constexpr auto& HideBusyMarker = lifted::entry::e8219EDB0;  // sub_8219EDB0
inline constexpr auto& InitStates = lifted::entry::e82196520;  // sub_82196520
inline constexpr auto& OnDialogDone = lifted::entry::e82193F80;  // sub_82193F80
inline constexpr auto& OnNo = lifted::entry::e82192BA8;  // sub_82192BA8
inline constexpr auto& OnPlateCancelled = lifted::entry::e821928F8;  // sub_821928F8
inline constexpr auto& OnPlateDone = lifted::entry::e82193E80;  // sub_82193E80
inline constexpr auto& OnPlateEditorLeft = lifted::entry::e82193F18;  // sub_82193F18
inline constexpr auto& OnSpecSheetClosed = lifted::entry::e82193E38;  // sub_82193E38
inline constexpr auto& OnYes = lifted::f82192958;  // sub_82192958
inline constexpr auto& OpenMovie = lifted::entry::e82191618;  // sub_82191618
inline constexpr auto& ResetStand = lifted::entry::e82191B20;  // sub_82191B20
inline constexpr auto& SetUpActions = lifted::entry::e821958D0;  // sub_821958D0
inline constexpr auto& ShowBusyMarker = lifted::entry::e8219ED10;  // sub_8219ED10
inline constexpr auto& Start = lifted::entry::e82193DC0;  // sub_82193DC0
inline constexpr auto& Transition11 = lifted::f82195F10;  // sub_82195F10
inline constexpr auto& Transition15 = lifted::f82196030;  // sub_82196030
inline constexpr auto& Unload = lifted::entry::e821927E8;  // sub_821927E8
inline constexpr auto& Update = lifted::entry::e82196230;  // sub_82196230
inline constexpr auto& UpdatePage = lifted::entry::e82194EA8;  // sub_82194EA8
inline constexpr auto& WaitForLoad = lifted::entry::e82191D70;  // sub_82191D70
}  // namespace game::GarageScreen

namespace game::GarageStage {
inline constexpr auto& CameraCommand = lifted::entry::e821808F0;  // sub_821808F0
inline constexpr auto& Command = lifted::entry::e82180108;  // sub_82180108
inline constexpr auto& Draw = lifted::entry::e8217FE60;  // sub_8217FE60
inline constexpr auto& FitCar = lifted::entry::e82178C40;  // sub_82178C40
inline constexpr auto& Init = lifted::entry::e82180968;  // sub_82180968
inline constexpr auto& IsBusy = lifted::entry::e82179BE0;  // sub_82179BE0
inline constexpr auto& ReloadModel = lifted::entry::e8217F880;  // sub_8217F880
inline constexpr auto& SetCar = lifted::entry::e8217FD58;  // sub_8217FD58
inline constexpr auto& SetCarClass = lifted::entry::e8217FDC8;  // sub_8217FDC8
inline constexpr auto& SetDark = lifted::entry::e8217D0C8;  // sub_8217D0C8
inline constexpr auto& SetDrawWord = lifted::entry::e82178B40;  // sub_82178B40
inline constexpr auto& SetPlates = lifted::entry::e82178DE8;  // sub_82178DE8
inline constexpr auto& ShowCarById = lifted::entry::e82182C90;  // sub_82182C90
inline constexpr auto& ShowCarByIdWithClass = lifted::entry::e82182D30;  // sub_82182D30
}  // namespace game::GarageStage

namespace game::GlareDef {
inline constexpr auto& Ctor = lifted::entry::e8212ED78;  // sub_8212ED78
inline constexpr auto& Init = lifted::entry::e8212EC90;  // sub_8212EC90
}  // namespace game::GlareDef

namespace game::Glow {
inline constexpr auto& Dtor = lifted::entry::e821389C8;  // sub_821389C8
inline constexpr auto& Record = lifted::entry::e821379D8;  // sub_821379D8
inline constexpr auto& RecordStarPass = lifted::entry::e82134060;  // sub_82134060
inline constexpr auto& Set8436 = lifted::entry::e82136EF8;  // sub_82136EF8
inline constexpr auto& Set8440 = lifted::entry::e82136F00;  // sub_82136F00
inline constexpr auto& Set8444 = lifted::entry::e82136F08;  // sub_82136F08
inline constexpr auto& Set8448 = lifted::entry::e82136F10;  // sub_82136F10
inline constexpr auto& Set8456 = lifted::entry::e82136F20;  // sub_82136F20
inline constexpr auto& Set8464 = lifted::entry::e82136F30;  // sub_82136F30
inline constexpr auto& Set8468 = lifted::entry::e82136F38;  // sub_82136F38
inline constexpr auto& Set8472 = lifted::entry::e82136F40;  // sub_82136F40
inline constexpr auto& Set8476 = lifted::entry::e82136F48;  // sub_82136F48
inline constexpr auto& SetFromSpeed = lifted::entry::e82133878;  // sub_82133878
inline constexpr auto& SetHistoryWeight = lifted::entry::e82136F28;  // sub_82136F28
inline constexpr auto& SetStrength = lifted::entry::e82136F18;  // sub_82136F18
}  // namespace game::Glow

namespace game::GpuFormat {
inline constexpr auto& BlockSize = lifted::entry::e822A4410;  // sub_822A4410
}  // namespace game::GpuFormat

namespace game::GroupPair {
inline constexpr auto& Ctor = lifted::entry::e82249C08;  // sub_82249C08
inline constexpr auto& DeletingDestructor = lifted::entry::e8224ACE0;  // sub_8224ACE0
inline constexpr auto& Dtor = lifted::entry::e82249C20;  // sub_82249C20
inline constexpr auto& FinishAll = lifted::entry::e82249C70;  // sub_82249C70
inline constexpr auto& Reset = lifted::entry::e82249C40;  // sub_82249C40
inline constexpr auto& Run = lifted::entry::e82249BE0;  // sub_82249BE0
inline constexpr auto& SelectA = lifted::entry::e82249C30;  // sub_82249C30
inline constexpr auto& WaitForFade = lifted::entry::e82249BA8;  // sub_82249BA8
}  // namespace game::GroupPair

namespace game::HandleList {
inline constexpr auto& Remove = lifted::f82118790;  // sub_82118790
}  // namespace game::HandleList

namespace game::HandlerMap {
inline constexpr auto& AllocateNodes = lifted::entry::e82099C80;  // sub_82099C80
inline constexpr auto& Ctor = lifted::entry::e82099FB0;  // sub_82099FB0
inline constexpr auto& ForEach = lifted::entry::e82092BF0;  // sub_82092BF0
inline constexpr auto& Insert = lifted::entry::e82093740;  // sub_82093740
inline constexpr auto& NewNode = lifted::entry::e82099E20;  // sub_82099E20
}  // namespace game::HandlerMap

namespace game::HelpLine {
inline constexpr auto& Set = lifted::entry::e82199B38;  // sub_82199B38
}  // namespace game::HelpLine

namespace game::Hermite {
inline constexpr auto& Basis = lifted::entry::e82293A48;  // sub_82293A48
}  // namespace game::Hermite

namespace game::HudBar {
inline constexpr auto& Snap = lifted::f82212100;  // sub_82212100
}  // namespace game::HudBar

namespace game::IdentityMatrix {
inline constexpr auto& StaticInit = lifted::entry::e825D9060;  // sub_825D9060
}  // namespace game::IdentityMatrix

namespace game::ImageCodec {
inline constexpr auto& AddDiffusionError = lifted::f823EB408;  // sub_823EB408
inline constexpr auto& ApplyColorKey = lifted::f823EADB0;  // sub_823EADB0
inline constexpr auto& ConvertRange = lifted::f823EA0C0;  // sub_823EA0C0
inline constexpr auto& LinearToSrgb = lifted::f823EAE40;  // sub_823EAE40
inline constexpr auto& QuantizeColorKey = lifted::f823EACC8;  // sub_823EACC8
inline constexpr auto& SrgbToLinear = lifted::f823EB180;  // sub_823EB180
}  // namespace game::ImageCodec

namespace game::IndexBuffer {
inline constexpr auto& Lock = lifted::entry::e822A6A18;  // sub_822A6A18
inline constexpr auto& Unlock = lifted::entry::e822A6A78;  // sub_822A6A78
}  // namespace game::IndexBuffer

namespace game::Input {
inline constexpr auto& Create = lifted::entry::e820993E8;  // sub_820993E8
inline constexpr auto& Update = lifted::entry::e820990F0;  // sub_820990F0
}  // namespace game::Input

namespace game::InputClient {
inline constexpr auto& Ctor = lifted::entry::e8210AE60;  // sub_8210AE60
}  // namespace game::InputClient

namespace game::InputClientList {
inline constexpr auto& BindAll = lifted::entry::e8210AF08;  // sub_8210AF08
}  // namespace game::InputClientList

namespace game::InputManager {
inline constexpr auto& AddListener = lifted::entry::e82099430;  // sub_82099430
inline constexpr auto& Ctor = lifted::entry::e820991E0;  // sub_820991E0
inline constexpr auto& GetLogicalPad = lifted::entry::e82098CD0;  // sub_82098CD0
inline constexpr auto& GetPhysicalPad = lifted::entry::e82098FE0;  // sub_82098FE0
inline constexpr auto& GetProcessor = lifted::entry::e82098CB8;  // sub_82098CB8
}  // namespace game::InputManager

namespace game::InputProcessor {
inline constexpr auto& AllHeld = lifted::entry::e825924E0;  // sub_825924E0
inline constexpr auto& TestHeld = lifted::entry::e8258CF10;  // sub_8258CF10
inline constexpr auto& TestPressed = lifted::entry::e8258CEB0;  // sub_8258CEB0
inline constexpr auto& TestRepeat = lifted::entry::e8258CEE0;  // sub_8258CEE0
inline constexpr auto& Update = lifted::entry::e82592188;  // sub_82592188
}  // namespace game::InputProcessor

namespace game::InputProcessorBase {
inline constexpr auto& Ctor = lifted::entry::e825920D0;  // sub_825920D0
}  // namespace game::InputProcessorBase

namespace game::InputRecording {
inline constexpr auto& Append = lifted::entry::e821029A8;  // sub_821029A8
inline constexpr auto& HasRoom = lifted::entry::e82102968;  // sub_82102968
}  // namespace game::InputRecording

namespace game::InputRecordings {
inline constexpr auto& Create = lifted::entry::e82105508;  // sub_82105508
inline constexpr auto& Destroy = lifted::f82104F00;  // sub_82104F00
inline constexpr auto& Dtor = lifted::f82104CE0;  // sub_82104CE0
}  // namespace game::InputRecordings

namespace game::Interior {
inline constexpr auto& DeletingDestructor = lifted::entry::e820D7960;  // sub_820D7960
inline constexpr auto& IsLoaded = lifted::entry::e820D7790;  // sub_820D7790
inline constexpr auto& OnPartLoaded = lifted::entry::e820D7578;  // sub_820D7578
}  // namespace game::Interior

namespace game::InteriorDials {
inline constexpr auto& DrawFaces = lifted::entry::e820E3170;  // sub_820E3170
inline constexpr auto& DrawReadouts = lifted::entry::e820E51D0;  // sub_820E51D0
}  // namespace game::InteriorDials

namespace game::InteriorMeters {
inline constexpr auto& DrawFaces = lifted::entry::e820E3AD8;  // sub_820E3AD8
}  // namespace game::InteriorMeters

namespace game::InteriorPoints {
inline constexpr auto& DrawRollCage = lifted::entry::e820E3548;  // sub_820E3548
inline constexpr auto& DrawSeats = lifted::entry::e820E3CF0;  // sub_820E3CF0
inline constexpr auto& Find = lifted::entry::e820E3660;  // sub_820E3660
}  // namespace game::InteriorPoints

namespace game::IntroCamera {
inline constexpr auto& Ctor = lifted::entry::e82158F70;  // sub_82158F70
inline constexpr auto& Update = lifted::entry::e82155E58;  // sub_82155E58
}  // namespace game::IntroCamera

namespace game::ItemFlags {
inline constexpr auto& Set = lifted::entry::e82112860;  // sub_82112860
}  // namespace game::ItemFlags

namespace game::ItemRecords {
inline constexpr auto& Item = lifted::entry::e82116DA8;  // sub_82116DA8
}  // namespace game::ItemRecords

namespace game::Items {
inline constexpr auto& AllWonKind5Or4 = lifted::f82110B40;  // sub_82110B40
inline constexpr auto& Available = lifted::entry::e821435C8;  // sub_821435C8
inline constexpr auto& Beaten = lifted::entry::e82143528;  // sub_82143528
}  // namespace game::Items

namespace game::Job {
inline constexpr auto& Dtor = lifted::entry::e820AC9E8;  // sub_820AC9E8
inline constexpr auto& Wait = lifted::entry::e82094B10;  // sub_82094B10
}  // namespace game::Job

namespace game::Key {
inline constexpr auto& Interpolate = lifted::entry::e825A66F8;  // sub_825A66F8
inline constexpr auto& Value = lifted::entry::e820AC178;  // sub_820AC178
}  // namespace game::Key

namespace game::Kind6Menu {
inline constexpr auto& Close = lifted::entry::e8221C168;  // sub_8221C168
inline constexpr auto& Draw = lifted::entry::e8221EF60;  // sub_8221EF60
inline constexpr auto& Open = lifted::entry::e8221EC48;  // sub_8221EC48
inline constexpr auto& Update = lifted::entry::e8221F1E8;  // sub_8221F1E8
}  // namespace game::Kind6Menu

namespace game::L16Codec {
inline constexpr auto& DecodeRow = lifted::f823F3FF8;  // sub_823F3FF8
inline constexpr auto& EncodeRow = lifted::f823F0C68;  // sub_823F0C68
}  // namespace game::L16Codec

namespace game::L6V5U5Codec {
inline constexpr auto& DecodeRow = lifted::f823F37E8;  // sub_823F37E8
inline constexpr auto& EncodeRow = lifted::f823EF488;  // sub_823EF488
}  // namespace game::L6V5U5Codec

namespace game::L8Codec {
inline constexpr auto& DecodeRow = lifted::f823F3520;  // sub_823F3520
inline constexpr auto& EncodeRow = lifted::f823EEBD8;  // sub_823EEBD8
}  // namespace game::L8Codec

namespace game::Leaderboard {
inline constexpr auto& Read = lifted::entry::e82292CC0;  // sub_82292CC0
inline constexpr auto& SetUpSpec = lifted::entry::e82292648;  // sub_82292648
}  // namespace game::Leaderboard

namespace game::LightKit {
inline constexpr auto& ChaseLevel = lifted::f820DC5C8;  // sub_820DC5C8
}  // namespace game::LightKit

namespace game::LightPoints {
inline constexpr auto& DrawFrontLeds = lifted::f820DCC70;  // sub_820DCC70
inline constexpr auto& DrawFrontNeon = lifted::entry::e820DCC10;  // sub_820DCC10
inline constexpr auto& DrawFrontSpots = lifted::f820DCF98;  // sub_820DCF98
inline constexpr auto& DrawRearLeds = lifted::f820DD620;  // sub_820DD620
inline constexpr auto& DrawRearNeon = lifted::entry::e820DD5C0;  // sub_820DD5C0
inline constexpr auto& DrawRearSpots = lifted::f820DD948;  // sub_820DD948
inline constexpr auto& DrawSidesLeds = lifted::f820DD110;  // sub_820DD110
inline constexpr auto& DrawSidesNeon = lifted::entry::e820DD0B0;  // sub_820DD0B0
inline constexpr auto& DrawSidesSpots = lifted::f820DD438;  // sub_820DD438
inline constexpr auto& Find = lifted::entry::e820DC900;  // sub_820DC900
}  // namespace game::LightPoints

namespace game::LightSprite {
inline constexpr auto& Ctor = lifted::entry::e8211F060;  // sub_8211F060
inline constexpr auto& Dtor = lifted::f8211F0C0;  // sub_8211F0C0
inline constexpr auto& Init = lifted::f8211E8A8;  // sub_8211E8A8
inline constexpr auto& Reset = lifted::entry::e8211E9F0;  // sub_8211E9F0
inline constexpr auto& SetTextures = lifted::entry::e8211E9C8;  // sub_8211E9C8
inline constexpr auto& Update = lifted::f8211E8C0;  // sub_8211E8C0
}  // namespace game::LightSprite

namespace game::List {
inline constexpr auto& Append = lifted::entry::e82294BA8;  // sub_82294BA8
inline constexpr auto& At = lifted::entry::e82593FC0;  // sub_82593FC0
inline constexpr auto& Clear = lifted::f82294CF0;  // sub_82294CF0
inline constexpr auto& DeleteAll = lifted::entry::e82593FF8;  // sub_82593FF8
inline constexpr auto& DeleteNodes = lifted::f82294C88;  // sub_82294C88
inline constexpr auto& Dtor = lifted::f82294D48;  // sub_82294D48
inline constexpr auto& Init = lifted::entry::e82593FB0;  // sub_82593FB0
}  // namespace game::List

namespace game::List128 {
inline constexpr auto& Link = lifted::entry::e8228B5A0;  // sub_8228B5A0
}  // namespace game::List128

namespace game::List16 {
inline constexpr auto& Copy32 = lifted::entry::e82154300;  // sub_82154300
inline constexpr auto& Link = lifted::entry::e821531C0;  // sub_821531C0
}  // namespace game::List16

namespace game::List40 {
inline constexpr auto& Link = lifted::entry::e82153228;  // sub_82153228
}  // namespace game::List40

namespace game::List64 {
inline constexpr auto& Insert = lifted::entry::e820FE330;  // sub_820FE330
}  // namespace game::List64

namespace game::List80 {
inline constexpr auto& Link = lifted::entry::e820F40C0;  // sub_820F40C0
}  // namespace game::List80

namespace game::ListNode {
inline constexpr auto& Dtor = lifted::entry::e8224FB10;  // sub_8224FB10
}  // namespace game::ListNode

namespace game::ListPart {
inline constexpr auto& Ctor = lifted::entry::e82252908;  // sub_82252908
inline constexpr auto& DeletingDestructor = lifted::entry::e82252A38;  // sub_82252A38
inline constexpr auto& Dtor = lifted::entry::e82252968;  // sub_82252968
}  // namespace game::ListPart

namespace game::ListPartBase {
inline constexpr auto& CanLeave = lifted::entry::e822525B8;  // sub_822525B8
inline constexpr auto& Close = lifted::entry::e82252530;  // sub_82252530
inline constexpr auto& DeletingDestructor = lifted::entry::e82252610;  // sub_82252610
inline constexpr auto& Dtor = lifted::entry::e82252420;  // sub_82252420
inline constexpr auto& Input = lifted::entry::e82252500;  // sub_82252500
inline constexpr auto& IsIdle = lifted::entry::e82252590;  // sub_82252590
inline constexpr auto& Open = lifted::entry::e82252430;  // sub_82252430
inline constexpr auto& Pick = lifted::entry::e82252490;  // sub_82252490
}  // namespace game::ListPartBase

namespace game::ListPartInner {
inline constexpr auto& CanLeave = lifted::entry::e822528C8;  // sub_822528C8
inline constexpr auto& Close = lifted::entry::e82252740;  // sub_82252740
inline constexpr auto& DeletingDestructor = lifted::entry::e822529B0;  // sub_822529B0
inline constexpr auto& Dtor = lifted::entry::e822526F8;  // sub_822526F8
inline constexpr auto& Input = lifted::entry::e82252DF0;  // sub_82252DF0
inline constexpr auto& IsIdle = lifted::entry::e82252748;  // sub_82252748
inline constexpr auto& Open = lifted::entry::e82252A88;  // sub_82252A88
inline constexpr auto& Page0 = lifted::entry::e82252778;  // sub_82252778
inline constexpr auto& Page1 = lifted::entry::e82252878;  // sub_82252878
inline constexpr auto& Page2 = lifted::entry::e822528A8;  // sub_822528A8
inline constexpr auto& Pick = lifted::entry::e82252A00;  // sub_82252A00
inline constexpr auto& Step0 = lifted::entry::e82252BB8;  // sub_82252BB8
inline constexpr auto& Step1 = lifted::entry::e82252C98;  // sub_82252C98
inline constexpr auto& Step2 = lifted::entry::e82252D38;  // sub_82252D38
}  // namespace game::ListPartInner

namespace game::ListPartInnerBase {
inline constexpr auto& DeletingDestructor = lifted::entry::e82252658;  // sub_82252658
inline constexpr auto& Dtor = lifted::entry::e82252600;  // sub_82252600
}  // namespace game::ListPartInnerBase

namespace game::ListScreen {
inline constexpr auto& ClearScrollText = lifted::entry::e82251DA0;  // sub_82251DA0
inline constexpr auto& DeletingDestructor = lifted::entry::e82252248;  // sub_82252248
inline constexpr auto& Draw = lifted::entry::e82251520;  // sub_82251520
inline constexpr auto& Dtor = lifted::entry::e82251EB8;  // sub_82251EB8
inline constexpr auto& IsLoaded = lifted::entry::e82242F70;  // sub_82242F70
inline constexpr auto& Leave = lifted::entry::e82251760;  // sub_82251760
inline constexpr auto& LeavePage = lifted::entry::e82255818;  // sub_82255818
inline constexpr auto& LoopItemNode = lifted::entry::e82251A20;  // sub_82251A20
inline constexpr auto& LoopSecondNode = lifted::entry::e82251B80;  // sub_82251B80
inline constexpr auto& LoopThirdNode = lifted::entry::e82251AD0;  // sub_82251AD0
inline constexpr auto& OnEnter = lifted::entry::e82251618;  // sub_82251618
inline constexpr auto& OnLeft = lifted::entry::e82251710;  // sub_82251710
inline constexpr auto& OnToggleMoved = lifted::entry::e82252068;  // sub_82252068
inline constexpr auto& Open = lifted::entry::e822517A0;  // sub_822517A0
inline constexpr auto& OpenMovie = lifted::entry::e82251508;  // sub_82251508
inline constexpr auto& Resume = lifted::entry::e82251590;  // sub_82251590
inline constexpr auto& SetArrows = lifted::entry::e82251E10;  // sub_82251E10
inline constexpr auto& SetIcons = lifted::entry::e82251C30;  // sub_82251C30
inline constexpr auto& SetScrollText = lifted::entry::e82251D00;  // sub_82251D00
inline constexpr auto& SetUpMovie = lifted::entry::e82251EC8;  // sub_82251EC8
inline constexpr auto& Show = lifted::entry::e82251F98;  // sub_82251F98
inline constexpr auto& ShowSelection = lifted::entry::e822518B0;  // sub_822518B0
inline constexpr auto& ShowToggle = lifted::entry::e82252198;  // sub_82252198
inline constexpr auto& Suspend = lifted::entry::e82251580;  // sub_82251580
inline constexpr auto& Update = lifted::entry::e822517B8;  // sub_822517B8
inline constexpr auto& WaitForMovie = lifted::entry::e82251678;  // sub_82251678
}  // namespace game::ListScreen

namespace game::Listener {
inline constexpr auto& Ctor = lifted::entry::e8259D478;  // sub_8259D478
inline constexpr auto& SetFloat52 = lifted::f8259D5B8;  // sub_8259D5B8
}  // namespace game::Listener

namespace game::LoadPage {
inline constexpr auto& Answer = lifted::entry::e821BC7B0;  // sub_821BC7B0
inline constexpr auto& CheckDevice = lifted::entry::e821BBC10;  // sub_821BBC10
inline constexpr auto& Close = lifted::entry::e821B9FB0;  // sub_821B9FB0
inline constexpr auto& DeviceAnswer = lifted::entry::e821BBC68;  // sub_821BBC68
inline constexpr auto& Leave = lifted::entry::e821C6468;  // sub_821C6468
inline constexpr auto& Note = lifted::entry::e821BC940;  // sub_821BC940
inline constexpr auto& Wait = lifted::entry::e821C2620;  // sub_821C2620
}  // namespace game::LoadPage

namespace game::LoadSlot {
inline constexpr auto& Load = lifted::entry::e820D07C8;  // sub_820D07C8
inline constexpr auto& LoadShared = lifted::entry::e820D0860;  // sub_820D0860
inline constexpr auto& ReleaseBuffer = lifted::entry::e820D0758;  // sub_820D0758
inline constexpr auto& Reset = lifted::entry::e820D0730;  // sub_820D0730
}  // namespace game::LoadSlot

namespace game::LoaderFunctor {
inline constexpr auto& Clone = lifted::f8209DF50;  // sub_8209DF50
inline constexpr auto& Invoke = lifted::entry::e8209DD80;  // sub_8209DD80
}  // namespace game::LoaderFunctor

namespace game::LoadingScreen {
inline constexpr auto& Finish = lifted::entry::e82247A60;  // sub_82247A60
inline constexpr auto& Free = lifted::f82247B28;  // sub_82247B28
inline constexpr auto& Load = lifted::entry::e82247B98;  // sub_82247B98
inline constexpr auto& Run = lifted::entry::e82247898;  // sub_82247898
inline constexpr auto& Show = lifted::entry::e822478D0;  // sub_822478D0
}  // namespace game::LoadingScreen

namespace game::LocationScreen {
inline constexpr auto& ApplyQueuedPage = lifted::entry::e8222C9B0;  // sub_8222C9B0
inline constexpr auto& Create = lifted::f82227A50;  // sub_82227A50
inline constexpr auto& Destroy = lifted::f82228548;  // sub_82228548
inline constexpr auto& Free = lifted::f82228220;  // sub_82228220
inline constexpr auto& FreePictures = lifted::entry::e82228498;  // sub_82228498
inline constexpr auto& HasMoreUnlocks = lifted::entry::e822281D8;  // sub_822281D8
inline constexpr auto& InitStates = lifted::entry::e8222F258;  // sub_8222F258
inline constexpr auto& IsPictureQueued = lifted::entry::e822291E8;  // sub_822291E8
inline constexpr auto& LoadPicture = lifted::entry::e8222C270;  // sub_8222C270
inline constexpr auto& NextOpponentText = lifted::entry::e82227430;  // sub_82227430
inline constexpr auto& NextResultText = lifted::entry::e822271F8;  // sub_822271F8
inline constexpr auto& NextResultTextB = lifted::f82227310;  // sub_82227310
inline constexpr auto& NextUnlockEntry = lifted::entry::e82227618;  // sub_82227618
inline constexpr auto& OnMessage14 = lifted::entry::e82227158;  // sub_82227158
inline constexpr auto& OnMessage15 = lifted::f822271B8;  // sub_822271B8
inline constexpr auto& OnMessage20 = lifted::f822271D8;  // sub_822271D8
inline constexpr auto& OnMessage3 = lifted::f82229610;  // sub_82229610
inline constexpr auto& OnMessage6 = lifted::f82229578;  // sub_82229578
inline constexpr auto& OnPictureLoadedA = lifted::entry::e82228E88;  // sub_82228E88
inline constexpr auto& OnPictureLoadedB = lifted::entry::e82228FC8;  // sub_82228FC8
inline constexpr auto& OpponentHasCard = lifted::entry::e82227708;  // sub_82227708
inline constexpr auto& Page0 = lifted::f8222CA48;  // sub_8222CA48
inline constexpr auto& Page1 = lifted::f82228A90;  // sub_82228A90
inline constexpr auto& Page11 = lifted::entry::e82228138;  // sub_82228138
inline constexpr auto& Page12 = lifted::f82229CF0;  // sub_82229CF0
inline constexpr auto& Page13 = lifted::f82229E98;  // sub_82229E98
inline constexpr auto& Page14 = lifted::f82228E18;  // sub_82228E18
inline constexpr auto& Page15 = lifted::f82229B98;  // sub_82229B98
inline constexpr auto& Page2 = lifted::f82229798;  // sub_82229798
inline constexpr auto& Page3 = lifted::f82228B80;  // sub_82228B80
inline constexpr auto& Page4 = lifted::f82229888;  // sub_82229888
inline constexpr auto& Page5 = lifted::f82228CC0;  // sub_82228CC0
inline constexpr auto& Page6 = lifted::f82229A48;  // sub_82229A48
inline constexpr auto& Page7 = lifted::f82229B38;  // sub_82229B38
inline constexpr auto& Page8 = lifted::f82228068;  // sub_82228068
inline constexpr auto& Page9 = lifted::entry::e822280D8;  // sub_822280D8
inline constexpr auto& PictureA = lifted::entry::e82229108;  // sub_82229108
inline constexpr auto& PictureB = lifted::entry::e82229158;  // sub_82229158
inline constexpr auto& PollLoad = lifted::entry::e82229238;  // sub_82229238
inline constexpr auto& PollSceneTeardown = lifted::f82227780;  // sub_82227780
inline constexpr auto& Reset = lifted::entry::e822279A8;  // sub_822279A8
inline constexpr auto& RunCardMovie = lifted::entry::e822291A8;  // sub_822291A8
inline constexpr auto& SceneryConfig = lifted::entry::e8228F0A0;  // sub_8228F0A0
inline constexpr auto& SetUp = lifted::f8222C300;  // sub_8222C300
inline constexpr auto& ShowPictures = lifted::entry::e8222A060;  // sub_8222A060
inline constexpr auto& Start = lifted::entry::e8222F0B0;  // sub_8222F0B0
inline constexpr auto& Transition0 = lifted::f8222CEB0;  // sub_8222CEB0
inline constexpr auto& Transition1 = lifted::entry::e8222AD50;  // sub_8222AD50
inline constexpr auto& Transition10 = lifted::f8222E1A0;  // sub_8222E1A0
inline constexpr auto& Transition11 = lifted::f8222E368;  // sub_8222E368
inline constexpr auto& Transition12 = lifted::entry::e8222B8A8;  // sub_8222B8A8
inline constexpr auto& Transition13 = lifted::f8222E5E8;  // sub_8222E5E8
inline constexpr auto& Transition14 = lifted::entry::e8222E6E0;  // sub_8222E6E0
inline constexpr auto& Transition16 = lifted::entry::e8222E8D8;  // sub_8222E8D8
inline constexpr auto& Transition17 = lifted::entry::e8222EAD0;  // sub_8222EAD0
inline constexpr auto& Transition18 = lifted::entry::e8222BE60;  // sub_8222BE60
inline constexpr auto& Transition19 = lifted::f8222BF18;  // sub_8222BF18
inline constexpr auto& Transition2 = lifted::entry::e8222D610;  // sub_8222D610
inline constexpr auto& Transition23 = lifted::entry::e8222A470;  // sub_8222A470
inline constexpr auto& Transition24 = lifted::entry::e8222D2E8;  // sub_8222D2E8
inline constexpr auto& Transition25 = lifted::entry::e8222D1A8;  // sub_8222D1A8
inline constexpr auto& Transition27 = lifted::f8222AC78;  // sub_8222AC78
inline constexpr auto& Transition28 = lifted::f8222D458;  // sub_8222D458
inline constexpr auto& Transition29 = lifted::entry::e8222CFF0;  // sub_8222CFF0
inline constexpr auto& Transition3 = lifted::f8222D4F8;  // sub_8222D4F8
inline constexpr auto& Transition30 = lifted::f8222AAB0;  // sub_8222AAB0
inline constexpr auto& Transition31 = lifted::f8222D3B8;  // sub_8222D3B8
inline constexpr auto& Transition32 = lifted::f8222A218;  // sub_8222A218
inline constexpr auto& Transition33 = lifted::entry::e8222A718;  // sub_8222A718
inline constexpr auto& Transition34 = lifted::entry::e8222D108;  // sub_8222D108
inline constexpr auto& Transition35 = lifted::f8222BF80;  // sub_8222BF80
inline constexpr auto& Transition4 = lifted::entry::e8222D828;  // sub_8222D828
inline constexpr auto& Transition5 = lifted::entry::e8222DA10;  // sub_8222DA10
inline constexpr auto& Transition6 = lifted::f8222DCE8;  // sub_8222DCE8
inline constexpr auto& Transition7 = lifted::entry::e8222AE00;  // sub_8222AE00
inline constexpr auto& Transition8 = lifted::entry::e8222DC08;  // sub_8222DC08
inline constexpr auto& Transition9 = lifted::entry::e8222DEE0;  // sub_8222DEE0
inline constexpr auto& Unload = lifted::f82229678;  // sub_82229678
inline constexpr auto& Update = lifted::f8222ED98;  // sub_8222ED98
inline constexpr auto& UpdatePage = lifted::entry::e8222EC08;  // sub_8222EC08
inline constexpr auto& UpdateView = lifted::entry::e82227E20;  // sub_82227E20
}  // namespace game::LocationScreen

namespace game::LogicalPad {
inline constexpr auto& Values = lifted::entry::e82591B30;  // sub_82591B30
}  // namespace game::LogicalPad

namespace game::MachineMember {
inline constexpr auto& Ctor = lifted::entry::e82108160;  // sub_82108160
}  // namespace game::MachineMember

namespace game::MachineScreen {
inline constexpr auto& Ctor = lifted::entry::e8224CF58;  // sub_8224CF58
inline constexpr auto& Dtor = lifted::entry::e8224CB18;  // sub_8224CB18
}  // namespace game::MachineScreen

namespace game::Marker {
inline constexpr auto& Ctor = lifted::entry::e8212D620;  // sub_8212D620
inline constexpr auto& Dtor = lifted::f8212D680;  // sub_8212D680
inline constexpr auto& Init = lifted::f8212CBC8;  // sub_8212CBC8
inline constexpr auto& Reset = lifted::entry::e8212CC38;  // sub_8212CC38
inline constexpr auto& Set = lifted::entry::e8212CCE0;  // sub_8212CCE0
inline constexpr auto& SetCameraPosition = lifted::entry::e8212CE18;  // sub_8212CE18
inline constexpr auto& SetTextures = lifted::entry::e8212CC10;  // sub_8212CC10
inline constexpr auto& Update = lifted::entry::e8212CBE0;  // sub_8212CBE0
}  // namespace game::Marker

namespace game::Matrix {
inline constexpr auto& Copy = lifted::entry::e820A0AC0;  // sub_820A0AC0
inline constexpr auto& CopyRows = lifted::entry::e820AFB78;  // sub_820AFB78
inline constexpr auto& FromQuaternion = lifted::entry::e822936F0;  // sub_822936F0
inline constexpr auto& FromRows = lifted::entry::e820BFBB8;  // sub_820BFBB8
inline constexpr auto& FromScaleRotationTranslation = lifted::entry::e820EF660;  // sub_820EF660
inline constexpr auto& Inverse = lifted::entry::e820A9988;  // sub_820A9988
inline constexpr auto& InverseRotate = lifted::entry::e82293998;  // sub_82293998
inline constexpr auto& Multiply = lifted::entry::e8209BD50;  // sub_8209BD50
inline constexpr auto& MultiplyReversed = lifted::entry::e82293988;  // sub_82293988
inline constexpr auto& MultiplyTranslation = lifted::entry::e820B0D68;  // sub_820B0D68
inline constexpr auto& RotateX = lifted::entry::e820A65F0;  // sub_820A65F0
inline constexpr auto& RotateY = lifted::entry::e820A67F8;  // sub_820A67F8
inline constexpr auto& RotateZ = lifted::entry::e820A2D30;  // sub_820A2D30
inline constexpr auto& SetIdentity = lifted::entry::e822936E0;  // sub_822936E0
inline constexpr auto& SetTranslation = lifted::entry::e82293818;  // sub_82293818
inline constexpr auto& Shadow = lifted::entry::e82597FE0;  // sub_82597FE0
inline constexpr auto& TransformPoint = lifted::entry::e822938F0;  // sub_822938F0
inline constexpr auto& TransposeRotation = lifted::entry::e820D1CB0;  // sub_820D1CB0
}  // namespace game::Matrix

namespace game::Member1136 {
inline constexpr auto& Ctor = lifted::entry::e82102888;  // sub_82102888
inline constexpr auto& Restore = lifted::entry::e82102C60;  // sub_82102C60
}  // namespace game::Member1136

namespace game::Member4080 {
inline constexpr auto& Ctor = lifted::entry::e82104690;  // sub_82104690
inline constexpr auto& Restore = lifted::entry::e82102B98;  // sub_82102B98
}  // namespace game::Member4080

namespace game::MemberDelegate {
inline constexpr auto& Invoke = lifted::f82233460;  // sub_82233460
}  // namespace game::MemberDelegate

namespace game::Memory {
inline constexpr auto& AllocPhysicalEx = lifted::entry::e8258E098;  // sub_8258E098
inline constexpr auto& AllocVirtual = lifted::entry::e8258DFF0;  // sub_8258DFF0
}  // namespace game::Memory

namespace game::Menu {
inline constexpr auto& Pressed = lifted::entry::e821864C8;  // sub_821864C8
inline constexpr auto& Step = lifted::entry::e82186568;  // sub_82186568
}  // namespace game::Menu

namespace game::MenuList {
inline constexpr auto& Accepted = lifted::entry::e8220BD50;  // sub_8220BD50
inline constexpr auto& Cancelled = lifted::entry::e8220BDE0;  // sub_8220BDE0
inline constexpr auto& Fit = lifted::entry::e8220BC40;  // sub_8220BC40
inline constexpr auto& GetPreviousSelection = lifted::entry::e8220BC28;  // sub_8220BC28
inline constexpr auto& Init = lifted::entry::e8220BAC0;  // sub_8220BAC0
inline constexpr auto& IsEnabled = lifted::entry::e8220BC10;  // sub_8220BC10
inline constexpr auto& Pressed152 = lifted::entry::e8220BE70;  // sub_8220BE70
inline constexpr auto& Pressed166 = lifted::entry::e8220BF90;  // sub_8220BF90
inline constexpr auto& PressedX = lifted::entry::e8220BF00;  // sub_8220BF00
inline constexpr auto& Release = lifted::entry::e8220BBB0;  // sub_8220BBB0
inline constexpr auto& Selection = lifted::entry::e8220BC20;  // sub_8220BC20
inline constexpr auto& SetEnabled = lifted::entry::e8220BC00;  // sub_8220BC00
inline constexpr auto& SetSelection = lifted::entry::e8220BC30;  // sub_8220BC30
inline constexpr auto& Update = lifted::entry::e8220C1E0;  // sub_8220C1E0
}  // namespace game::MenuList

namespace game::MenuSound {
inline constexpr auto& Play = lifted::entry::e821103F0;  // sub_821103F0
}  // namespace game::MenuSound

namespace game::Mesh {
inline constexpr auto& LoadShaders = lifted::entry::e82594D20;  // sub_82594D20
}  // namespace game::Mesh

namespace game::MessageVector {
inline constexpr auto& Allocate = lifted::entry::e82092B78;  // sub_82092B78
}  // namespace game::MessageVector

namespace game::Minimap {
inline constexpr auto& Draw = lifted::entry::e820F5740;  // sub_820F5740
}  // namespace game::Minimap

namespace game::Mode {
inline constexpr auto& Create = lifted::entry::e821077A8;  // sub_821077A8
inline constexpr auto& DeletingDestructor = lifted::entry::e82106E38;  // sub_82106E38
inline constexpr auto& NextModeThunk = lifted::entry::e82235C08;  // sub_82235C08
inline constexpr auto& NoNextMode = lifted::f8222FC90;  // sub_8222FC90
inline constexpr auto& Slot10Thunk = lifted::entry::e8223D4A0;  // sub_8223D4A0
inline constexpr auto& Slot11Thunk = lifted::entry::e8222FC60;  // sub_8222FC60
inline constexpr auto& Slot12Thunk = lifted::entry::e8222FC70;  // sub_8222FC70
inline constexpr auto& Slot13Thunk = lifted::entry::e8222FC80;  // sub_8222FC80
inline constexpr auto& Slot14Thunk = lifted::entry::e8222FC50;  // sub_8222FC50
inline constexpr auto& Slot8Thunk = lifted::entry::e8223D4B0;  // sub_8223D4B0
inline constexpr auto& Slot9Thunk = lifted::entry::e8223D4C0;  // sub_8223D4C0
}  // namespace game::Mode

namespace game::ModeDelegate {
inline constexpr auto& Invoke = lifted::entry::e8223D440;  // sub_8223D440
}  // namespace game::ModeDelegate

namespace game::ModeType10 {
inline constexpr auto& Init = lifted::entry::e82288940;  // sub_82288940
inline constexpr auto& Interrupt = lifted::entry::e82288A68;  // sub_82288A68
inline constexpr auto& LeaveScreen = lifted::entry::e82288DA0;  // sub_82288DA0
inline constexpr auto& Leaving = lifted::entry::e822890D8;  // sub_822890D8
inline constexpr auto& NextMode = lifted::entry::e82288E68;  // sub_82288E68
inline constexpr auto& Release = lifted::entry::e82288898;  // sub_82288898
inline constexpr auto& Resume = lifted::entry::e82288B20;  // sub_82288B20
inline constexpr auto& WaitForLoads = lifted::entry::e82288B70;  // sub_82288B70
inline constexpr auto& WaitForShown = lifted::entry::e82288CA0;  // sub_82288CA0
}  // namespace game::ModeType10

namespace game::ModeType12 {
inline constexpr auto& ForceExit = lifted::entry::e822788D8;  // sub_822788D8
inline constexpr auto& Init = lifted::entry::e82278058;  // sub_82278058
inline constexpr auto& Interrupt = lifted::f822785E0;  // sub_822785E0
inline constexpr auto& LeaveScreen = lifted::entry::e8227B868;  // sub_8227B868
inline constexpr auto& NextMode = lifted::entry::e8227B930;  // sub_8227B930
inline constexpr auto& Release = lifted::entry::e82277DB0;  // sub_82277DB0
inline constexpr auto& Resume = lifted::entry::e82278B98;  // sub_82278B98
inline constexpr auto& Teardown = lifted::entry::e8227BE10;  // sub_8227BE10
inline constexpr auto& WaitForLoads = lifted::entry::e8227B440;  // sub_8227B440
inline constexpr auto& WaitForShown = lifted::entry::e8227B770;  // sub_8227B770
}  // namespace game::ModeType12

namespace game::ModeType2 {
inline constexpr auto& AfterLeaveScreen = lifted::entry::e821977E0;  // sub_821977E0
inline constexpr auto& AllLoaded = lifted::entry::e82197130;  // sub_82197130
inline constexpr auto& FindShown = lifted::entry::e82197218;  // sub_82197218
inline constexpr auto& ForceExit = lifted::entry::e82197930;  // sub_82197930
inline constexpr auto& Init = lifted::entry::e82197278;  // sub_82197278
inline constexpr auto& Interrupt = lifted::entry::e82197858;  // sub_82197858
inline constexpr auto& LeaveScreen = lifted::entry::e82197C88;  // sub_82197C88
inline constexpr auto& Leaving = lifted::entry::e82198008;  // sub_82198008
inline constexpr auto& MachineOf = lifted::entry::e82196FC0;  // sub_82196FC0
inline constexpr auto& NextMode = lifted::entry::e82197D50;  // sub_82197D50
inline constexpr auto& Release = lifted::entry::e82196EC0;  // sub_82196EC0
inline constexpr auto& Restart = lifted::entry::e82197A30;  // sub_82197A30
inline constexpr auto& Resume = lifted::f821970B8;  // sub_821970B8
inline constexpr auto& TearDown = lifted::entry::e821970C0;  // sub_821970C0
inline constexpr auto& WaitForLoads = lifted::entry::e82197B00;  // sub_82197B00
}  // namespace game::ModeType2

namespace game::ModeType4 {
inline constexpr auto& ForceExit = lifted::entry::e82224ED0;  // sub_82224ED0
inline constexpr auto& Init = lifted::entry::e82224C70;  // sub_82224C70
inline constexpr auto& Interrupt = lifted::entry::e82224E20;  // sub_82224E20
inline constexpr auto& LeaveScreen = lifted::entry::e822252C8;  // sub_822252C8
inline constexpr auto& Leaving = lifted::entry::e82225698;  // sub_82225698
inline constexpr auto& NextMode = lifted::entry::e82225390;  // sub_82225390
inline constexpr auto& NextModeAnswer = lifted::entry::e82224B98;  // sub_82224B98
inline constexpr auto& Release = lifted::entry::e82224AB8;  // sub_82224AB8
inline constexpr auto& Restart = lifted::entry::e82225098;  // sub_82225098
inline constexpr auto& Resume = lifted::entry::e82225048;  // sub_82225048
inline constexpr auto& WaitForLeave = lifted::entry::e822251D0;  // sub_822251D0
}  // namespace game::ModeType4

namespace game::ModeType5 {
inline constexpr auto& ForceExit = lifted::entry::e8222FF60;  // sub_8222FF60
inline constexpr auto& Init = lifted::entry::e8222FD68;  // sub_8222FD68
inline constexpr auto& Interrupt = lifted::entry::e8222FED0;  // sub_8222FED0
inline constexpr auto& LeaveScreen = lifted::entry::e82230260;  // sub_82230260
inline constexpr auto& Leaving = lifted::entry::e82230328;  // sub_82230328
inline constexpr auto& NextMode = lifted::entry::e82230458;  // sub_82230458
inline constexpr auto& Release = lifted::entry::e8222FB90;  // sub_8222FB90
inline constexpr auto& Resume = lifted::entry::e8222FC38;  // sub_8222FC38
inline constexpr auto& WaitForLoads = lifted::entry::e82230050;  // sub_82230050
inline constexpr auto& WaitForShown = lifted::entry::e82230168;  // sub_82230168
}  // namespace game::ModeType5

namespace game::ModeType8 {
inline constexpr auto& Ctor = lifted::f82107670;  // sub_82107670
inline constexpr auto& DeletingDestructor = lifted::f82107758;  // sub_82107758
inline constexpr auto& Dtor = lifted::f821076F8;  // sub_821076F8
inline constexpr auto& ForceExit = lifted::entry::e8223DA00;  // sub_8223DA00
inline constexpr auto& Init = lifted::entry::e8223D5D8;  // sub_8223D5D8
inline constexpr auto& Interrupt = lifted::entry::e8223D960;  // sub_8223D960
inline constexpr auto& NextMode = lifted::entry::e8223DD90;  // sub_8223DD90
inline constexpr auto& PollTeardown = lifted::entry::e8223D8E8;  // sub_8223D8E8
inline constexpr auto& Release = lifted::entry::e8223D3B8;  // sub_8223D3B8
inline constexpr auto& TearDown = lifted::entry::e8223D4D0;  // sub_8223D4D0
inline constexpr auto& WaitForLoads = lifted::entry::e8223DBC0;  // sub_8223DBC0
}  // namespace game::ModeType8

namespace game::ModeType8Member {
inline constexpr auto& Ctor = lifted::f821075E0;  // sub_821075E0
inline constexpr auto& Dtor = lifted::f82106BB0;  // sub_82106BB0
}  // namespace game::ModeType8Member

namespace game::ModeType9 {
inline constexpr auto& ForceExit = lifted::entry::e8225D828;  // sub_8225D828
inline constexpr auto& Init = lifted::entry::e8225D6D0;  // sub_8225D6D0
inline constexpr auto& IsLoaded = lifted::entry::e8225D6A8;  // sub_8225D6A8
inline constexpr auto& LeaveScreen = lifted::entry::e8225DB70;  // sub_8225DB70
inline constexpr auto& Leaving = lifted::entry::e8225DE78;  // sub_8225DE78
inline constexpr auto& NextMode = lifted::entry::e8225DC38;  // sub_8225DC38
inline constexpr auto& NextModeAnswer = lifted::entry::e8225D638;  // sub_8225D638
inline constexpr auto& Release = lifted::entry::e8225D590;  // sub_8225D590
inline constexpr auto& WaitForLoads = lifted::entry::e8225D918;  // sub_8225D918
inline constexpr auto& WaitForShown = lifted::entry::e8225DA68;  // sub_8225DA68
}  // namespace game::ModeType9

namespace game::Model {
inline constexpr auto& Ctor = lifted::entry::e825948C0;  // sub_825948C0
inline constexpr auto& Dtor = lifted::entry::e82594F58;  // sub_82594F58
inline constexpr auto& HasPoint = lifted::entry::e820D9DB0;  // sub_820D9DB0
inline constexpr auto& IsShapeVisible = lifted::entry::e825951E0;  // sub_825951E0
inline constexpr auto& Load = lifted::entry::e82596920;  // sub_82596920
inline constexpr auto& Material = lifted::entry::e82594B20;  // sub_82594B20
inline constexpr auto& MaterialCount = lifted::entry::e82594AD8;  // sub_82594AD8
inline constexpr auto& PointPosition = lifted::entry::e820DA988;  // sub_820DA988
inline constexpr auto& RecordMaterials = lifted::entry::e820CECA0;  // sub_820CECA0
inline constexpr auto& RecordShape = lifted::entry::e82595550;  // sub_82595550
inline constexpr auto& RecordShapeDepth = lifted::entry::e825960D0;  // sub_825960D0
inline constexpr auto& Release = lifted::entry::e82594970;  // sub_82594970
inline constexpr auto& SetVinylTextures = lifted::entry::e820CE9F8;  // sub_820CE9F8
inline constexpr auto& SkinShape = lifted::f82594FD0;  // sub_82594FD0
inline constexpr auto& StreamData = lifted::entry::e82594A60;  // sub_82594A60
inline constexpr auto& StreamData2 = lifted::entry::e82594A90;  // sub_82594A90
inline constexpr auto& StreamSize = lifted::entry::e82594A78;  // sub_82594A78
inline constexpr auto& StreamSize2 = lifted::entry::e82594AA8;  // sub_82594AA8
}  // namespace game::Model

namespace game::ModelList {
inline constexpr auto& Init = lifted::entry::e820F1A20;  // sub_820F1A20
inline constexpr auto& Release = lifted::entry::e82147FB8;  // sub_82147FB8
}  // namespace game::ModelList

namespace game::ModelPoint {
inline constexpr auto& Find = lifted::entry::e820D02D0;  // sub_820D02D0
}  // namespace game::ModelPoint

namespace game::ModelSprite {
inline constexpr auto& Ctor = lifted::entry::e82122880;  // sub_82122880
inline constexpr auto& Dtor = lifted::f82122900;  // sub_82122900
inline constexpr auto& Entry = lifted::entry::e8211F300;  // sub_8211F300
inline constexpr auto& Init = lifted::f8211F178;  // sub_8211F178
inline constexpr auto& PlaceHidden = lifted::entry::e82121560;  // sub_82121560
inline constexpr auto& Record = lifted::f821225D8;  // sub_821225D8
inline constexpr auto& Reset = lifted::entry::e8211F430;  // sub_8211F430
inline constexpr auto& SetTextures = lifted::entry::e8211F408;  // sub_8211F408
inline constexpr auto& Update = lifted::f82121CC0;  // sub_82121CC0
}  // namespace game::ModelSprite

namespace game::Modes {
inline constexpr auto& Ctor = lifted::entry::e821067F8;  // sub_821067F8
inline constexpr auto& SetChecks = lifted::entry::e82106868;  // sub_82106868
inline constexpr auto& SetSceneTargetMode = lifted::entry::e82106740;  // sub_82106740
inline constexpr auto& Tick = lifted::entry::e82107910;  // sub_82107910
}  // namespace game::Modes

namespace game::Morph {
inline constexpr auto& SetUp = lifted::entry::e82594C18;  // sub_82594C18
}  // namespace game::Morph

namespace game::MorphJob {
inline constexpr auto& Dtor = lifted::entry::e820ACB90;  // sub_820ACB90
inline constexpr auto& IsReleased = lifted::entry::e820AC9D0;  // sub_820AC9D0
inline constexpr auto& Run = lifted::entry::e820AC998;  // sub_820AC998
inline constexpr auto& RunThunk = lifted::entry::e820AC9C0;  // sub_820AC9C0
inline constexpr auto& SetReleased = lifted::entry::e820AC9D8;  // sub_820AC9D8
inline constexpr auto& Wait = lifted::entry::e820AC9B8;  // sub_820AC9B8
}  // namespace game::MorphJob

namespace game::MotionBlur {
inline constexpr auto& Dtor = lifted::entry::e821385C0;  // sub_821385C0
inline constexpr auto& Record = lifted::entry::e821363F0;  // sub_821363F0
}  // namespace game::MotionBlur

namespace game::Movie {
inline constexpr auto& Ctor = lifted::entry::e820AEA88;  // sub_820AEA88
inline constexpr auto& OnMessage = lifted::f820AE780;  // sub_820AE780
inline constexpr auto& OnTick = lifted::f820AE688;  // sub_820AE688
inline constexpr auto& Open = lifted::f820AE5D0;  // sub_820AE5D0
inline constexpr auto& Pause = lifted::entry::e820AE898;  // sub_820AE898
inline constexpr auto& Play = lifted::entry::e820AEC10;  // sub_820AEC10
inline constexpr auto& Release = lifted::entry::e820AE528;  // sub_820AE528
inline constexpr auto& RenderFrame = lifted::entry::e820AE958;  // sub_820AE958
inline constexpr auto& Resume = lifted::entry::e820AE8F8;  // sub_820AE8F8
inline constexpr auto& Start = lifted::entry::e820AE4E0;  // sub_820AE4E0
inline constexpr auto& State = lifted::entry::e820AEA80;  // sub_820AEA80
inline constexpr auto& Stop = lifted::entry::e820AE828;  // sub_820AE828
}  // namespace game::Movie

namespace game::MoviePlayer {
inline constexpr auto& RenderNextFrame = lifted::f82481A70;  // sub_82481A70
inline constexpr auto& SetRect = lifted::f82481A80;  // sub_82481A80
}  // namespace game::MoviePlayer

namespace game::MovieSource {
inline constexpr auto& AtEnd = lifted::f824936E8;  // sub_824936E8
inline constexpr auto& Close = lifted::entry::e824880D8;  // sub_824880D8
inline constexpr auto& OpenFile = lifted::entry::e82487C48;  // sub_82487C48
inline constexpr auto& Read = lifted::entry::e82488598;  // sub_82488598
inline constexpr auto& ReadLooping = lifted::entry::e82488A10;  // sub_82488A10
inline constexpr auto& Seek = lifted::f82488548;  // sub_82488548
inline constexpr auto& SetUpBlocks = lifted::entry::e82487F68;  // sub_82487F68
inline constexpr auto& Skip = lifted::f82488508;  // sub_82488508
}  // namespace game::MovieSource

namespace game::MovieWrapper {
inline constexpr auto& Dtor = lifted::f82593EF0;  // sub_82593EF0
inline constexpr auto& Open = lifted::f82593E78;  // sub_82593E78
inline constexpr auto& Player = lifted::f82593D78;  // sub_82593D78
inline constexpr auto& Release = lifted::f82593E30;  // sub_82593E30
inline constexpr auto& Sync = lifted::f82593D88;  // sub_82593D88
}  // namespace game::MovieWrapper

namespace game::Movies {
inline constexpr auto& Clear = lifted::f820A4398;  // sub_820A4398
inline constexpr auto& State = lifted::entry::e820A4450;  // sub_820A4450
}  // namespace game::Movies

namespace game::Music {
inline constexpr auto& Play = lifted::entry::e821063B0;  // sub_821063B0
}  // namespace game::Music

namespace game::NetModeSelect {
inline constexpr auto& BackPage = lifted::entry::e8227CC78;  // sub_8227CC78
inline constexpr auto& BattlePage = lifted::entry::e8227C9F0;  // sub_8227C9F0
inline constexpr auto& Create = lifted::entry::e8227C398;  // sub_8227C398
inline constexpr auto& Destroy = lifted::entry::e8227C4B8;  // sub_8227C4B8
inline constexpr auto& Draw = lifted::entry::e8227D248;  // sub_8227D248
inline constexpr auto& FadeIn = lifted::entry::e8227CFA8;  // sub_8227CFA8
inline constexpr auto& FadeOut = lifted::entry::e8227D088;  // sub_8227D088
inline constexpr auto& Intro = lifted::entry::e8227CE50;  // sub_8227CE50
inline constexpr auto& Leave = lifted::entry::e8227C8C0;  // sub_8227C8C0
inline constexpr auto& LeavePage = lifted::entry::e8227C970;  // sub_8227C970
inline constexpr auto& MenuPage = lifted::entry::e8227C658;  // sub_8227C658
inline constexpr auto& MoveCursor = lifted::entry::e8227D380;  // sub_8227D380
inline constexpr auto& OnDialogClosed = lifted::entry::e8227BFD8;  // sub_8227BFD8
inline constexpr auto& OnNo = lifted::entry::e8227BF80;  // sub_8227BF80
inline constexpr auto& OnYes = lifted::entry::e8227BFB8;  // sub_8227BFB8
inline constexpr auto& Open = lifted::entry::e8227C5C8;  // sub_8227C5C8
inline constexpr auto& OpenMovie = lifted::entry::e8227C078;  // sub_8227C078
inline constexpr auto& ShowCursor = lifted::entry::e8227D138;  // sub_8227D138
inline constexpr auto& SignInPage = lifted::entry::e8227C088;  // sub_8227C088
inline constexpr auto& SignInPage2 = lifted::entry::e8227C210;  // sub_8227C210
inline constexpr auto& Start = lifted::entry::e8227D300;  // sub_8227D300
inline constexpr auto& TimeAttackPage = lifted::entry::e8227CB38;  // sub_8227CB38
inline constexpr auto& Unload = lifted::entry::e8227C7A8;  // sub_8227C7A8
inline constexpr auto& Unloaded = lifted::entry::e8227C810;  // sub_8227C810
inline constexpr auto& Update = lifted::entry::e8227D4E8;  // sub_8227D4E8
inline constexpr auto& WaitForMovie = lifted::entry::e8227C558;  // sub_8227C558
}  // namespace game::NetModeSelect

namespace game::NetRanking {
inline constexpr auto& Create = lifted::entry::e82282358;  // sub_82282358
inline constexpr auto& Destroy = lifted::entry::e82282508;  // sub_82282508
inline constexpr auto& Leave = lifted::entry::e82282848;  // sub_82282848
inline constexpr auto& OpenMovie = lifted::entry::e82282098;  // sub_82282098
inline constexpr auto& WaitForMovie = lifted::entry::e822825C0;  // sub_822825C0
}  // namespace game::NetRanking

namespace game::NetTaCourse {
inline constexpr auto& Create = lifted::entry::e8227FD40;  // sub_8227FD40
inline constexpr auto& Destroy = lifted::entry::e8227FE70;  // sub_8227FE70
inline constexpr auto& Leave = lifted::entry::e82280218;  // sub_82280218
inline constexpr auto& OpenMovie = lifted::entry::e8227FC70;  // sub_8227FC70
inline constexpr auto& WaitForMovie = lifted::entry::e8227FF10;  // sub_8227FF10
}  // namespace game::NetTaCourse

namespace game::Network {
inline constexpr auto& SetUp = lifted::entry::e8214E190;  // sub_8214E190
inline constexpr auto& SetUpVoice = lifted::entry::e8214F6E8;  // sub_8214F6E8
inline constexpr auto& Update = lifted::entry::e8214E268;  // sub_8214E268
}  // namespace game::Network

namespace game::NetworkManager {
inline constexpr auto& Ctor = lifted::f8214F660;  // sub_8214F660
}  // namespace game::NetworkManager

namespace game::NodeListScreen {
inline constexpr auto& DeletingDestructor = lifted::entry::e82257940;  // sub_82257940
inline constexpr auto& Dtor = lifted::entry::e82257000;  // sub_82257000
inline constexpr auto& ItemInfo = lifted::entry::e82256FE8;  // sub_82256FE8
inline constexpr auto& LoadItem = lifted::entry::e82257060;  // sub_82257060
}  // namespace game::NodeListScreen

namespace game::NodeScreen {
inline constexpr auto& Ctor = lifted::entry::e8225A3D8;  // sub_8225A3D8
inline constexpr auto& DeletingDestructor = lifted::entry::e8225A168;  // sub_8225A168
inline constexpr auto& Draw = lifted::entry::e8225A1B8;  // sub_8225A1B8
inline constexpr auto& Dtor = lifted::entry::e82259948;  // sub_82259948
inline constexpr auto& ItemInfo = lifted::entry::e82259998;  // sub_82259998
inline constexpr auto& Leave = lifted::entry::e822592D8;  // sub_822592D8
inline constexpr auto& LoadItem = lifted::entry::e82259050;  // sub_82259050
inline constexpr auto& OnEnter = lifted::entry::e822591B0;  // sub_822591B0
inline constexpr auto& OnLeft = lifted::entry::e82259298;  // sub_82259298
inline constexpr auto& OnToggleMoved = lifted::entry::e82259B70;  // sub_82259B70
inline constexpr auto& Open = lifted::entry::e82259318;  // sub_82259318
inline constexpr auto& OpenMovie = lifted::entry::e822590D0;  // sub_822590D0
inline constexpr auto& Page2 = lifted::entry::e82259D80;  // sub_82259D80
inline constexpr auto& Page4 = lifted::entry::e822590E8;  // sub_822590E8
inline constexpr auto& Page5 = lifted::entry::e82259328;  // sub_82259328
inline constexpr auto& Refresh = lifted::entry::e822599B0;  // sub_822599B0
inline constexpr auto& SetArrows = lifted::entry::e82259368;  // sub_82259368
inline constexpr auto& Show = lifted::entry::e82259CA0;  // sub_82259CA0
inline constexpr auto& ShowRecords = lifted::entry::e82259410;  // sub_82259410
inline constexpr auto& ShowSelection = lifted::entry::e82259A78;  // sub_82259A78
inline constexpr auto& Step0 = lifted::entry::e82259EC0;  // sub_82259EC0
inline constexpr auto& Step1 = lifted::entry::e82259680;  // sub_82259680
inline constexpr auto& Step2 = lifted::entry::e82259750;  // sub_82259750
inline constexpr auto& Update = lifted::entry::e8225A2C0;  // sub_8225A2C0
inline constexpr auto& WaitForMovie = lifted::entry::e82259208;  // sub_82259208
}  // namespace game::NodeScreen

namespace game::NodeTable {
inline constexpr auto& DeletingDestructor = lifted::entry::e82247E88;  // sub_82247E88
inline constexpr auto& Dtor = lifted::entry::e82247DC0;  // sub_82247DC0
inline constexpr auto& SetMarks = lifted::entry::e822480F0;  // sub_822480F0
inline constexpr auto& SetPairs = lifted::entry::e82247CB8;  // sub_82247CB8
inline constexpr auto& Show = lifted::entry::e82248300;  // sub_82248300
inline constexpr auto& ShowNode = lifted::entry::e82247ED0;  // sub_82247ED0
}  // namespace game::NodeTable

namespace game::ObjectPools {
inline constexpr auto& Init = lifted::entry::e820FBBE8;  // sub_820FBBE8
inline constexpr auto& Object = lifted::entry::e820FC0E8;  // sub_820FC0E8
inline constexpr auto& Shutdown = lifted::entry::e820FBDC8;  // sub_820FBDC8
}  // namespace game::ObjectPools

namespace game::Options {
inline constexpr auto& AnalogPage = lifted::entry::e821C21A8;  // sub_821C21A8
inline constexpr auto& AnalogPage2P = lifted::f821C2388;  // sub_821C2388
inline constexpr auto& ButtonsPage = lifted::entry::e821C1D08;  // sub_821C1D08
inline constexpr auto& ButtonsPage2P = lifted::f821C1ED8;  // sub_821C1ED8
inline constexpr auto& Category6Page = lifted::entry::e821BD228;  // sub_821BD228
inline constexpr auto& CategoryPage = lifted::entry::e821BC670;  // sub_821BC670
inline constexpr auto& ChangeAnalog = lifted::f821C47A0;  // sub_821C47A0
inline constexpr auto& ChangeAnalog2P = lifted::f821C4A58;  // sub_821C4A58
inline constexpr auto& ChangeButton = lifted::f821C3E48;  // sub_821C3E48
inline constexpr auto& ChangeButton2P = lifted::f821C41D0;  // sub_821C41D0
inline constexpr auto& ChangeCategory6Dialog = lifted::entry::e821C0A50;  // sub_821C0A50
inline constexpr auto& ChangeHudValue = lifted::entry::e821BD800;  // sub_821BD800
inline constexpr auto& ChangeSaveDialog = lifted::entry::e821C08B0;  // sub_821C08B0
inline constexpr auto& ChangeSlidersValue = lifted::entry::e821C35F0;  // sub_821C35F0
inline constexpr auto& ChangeSlidersValue2P = lifted::f821C3910;  // sub_821C3910
inline constexpr auto& ChangeSoundValue = lifted::entry::e821C3278;  // sub_821C3278
inline constexpr auto& Close = lifted::entry::e821BCD28;  // sub_821BCD28
inline constexpr auto& CloseAnalog = lifted::entry::e821C7D18;  // sub_821C7D18
inline constexpr auto& CloseAnalog2P = lifted::f821C7E00;  // sub_821C7E00
inline constexpr auto& CloseButtons = lifted::entry::e821C7B08;  // sub_821C7B08
inline constexpr auto& CloseButtons2P = lifted::f821C7BF0;  // sub_821C7BF0
inline constexpr auto& CloseHud = lifted::entry::e821C75E0;  // sub_821C75E0
inline constexpr auto& ClosePage = lifted::entry::e821BD330;  // sub_821BD330
inline constexpr auto& CloseSliders = lifted::entry::e821C7868;  // sub_821C7868
inline constexpr auto& CloseSliders2P = lifted::f821C79B8;  // sub_821C79B8
inline constexpr auto& CloseSound = lifted::entry::e821C7710;  // sub_821C7710
inline constexpr auto& Draw = lifted::entry::e821C80F0;  // sub_821C80F0
inline constexpr auto& EnterAnalog = lifted::entry::e821BDE88;  // sub_821BDE88
inline constexpr auto& EnterAnalog2P = lifted::f821BDFA8;  // sub_821BDFA8
inline constexpr auto& EnterButtons = lifted::entry::e821BDAD0;  // sub_821BDAD0
inline constexpr auto& EnterButtons2P = lifted::f821BDC18;  // sub_821BDC18
inline constexpr auto& EnterHud = lifted::entry::e821BD728;  // sub_821BD728
inline constexpr auto& EnterSliders = lifted::entry::e821BD9A0;  // sub_821BD9A0
inline constexpr auto& EnterSliders2P = lifted::f821BDA38;  // sub_821BDA38
inline constexpr auto& EnterSound = lifted::entry::e821BD908;  // sub_821BD908
inline constexpr auto& FadeIn = lifted::entry::e821C7080;  // sub_821C7080
inline constexpr auto& FadeOut = lifted::f821BD680;  // sub_821BD680
inline constexpr auto& HudPage = lifted::entry::e821C1270;  // sub_821C1270
inline constexpr auto& Leave = lifted::entry::e821BCF08;  // sub_821BCF08
inline constexpr auto& LeaveAnalog = lifted::entry::e821C2A10;  // sub_821C2A10
inline constexpr auto& LeaveButtons = lifted::entry::e821C2778;  // sub_821C2778
inline constexpr auto& LeaveSoundPage = lifted::entry::e821BA380;  // sub_821BA380
inline constexpr auto& MoveAnalogCursor = lifted::f821C43D0;  // sub_821C43D0
inline constexpr auto& MoveAnalogCursor2P = lifted::f821BE040;  // sub_821BE040
inline constexpr auto& MoveButtonsCursor = lifted::f821C3A10;  // sub_821C3A10
inline constexpr auto& MoveButtonsCursor2P = lifted::f821BDCB0;  // sub_821BDCB0
inline constexpr auto& MoveCategory6DialogCursor = lifted::entry::e821C0B50;  // sub_821C0B50
inline constexpr auto& MoveCategoryCursor = lifted::entry::e821C2BE0;  // sub_821C2BE0
inline constexpr auto& MoveHudCursor = lifted::entry::e821C2E28;  // sub_821C2E28
inline constexpr auto& MoveSaveDialogCursor = lifted::entry::e821C09B0;  // sub_821C09B0
inline constexpr auto& MoveSlidersCursor = lifted::entry::e821C3398;  // sub_821C3398
inline constexpr auto& MoveSlidersCursor2P = lifted::f821C36F0;  // sub_821C36F0
inline constexpr auto& MoveSoundCursor = lifted::entry::e821C3008;  // sub_821C3008
inline constexpr auto& OnClosed = lifted::entry::e821BCE38;  // sub_821BCE38
inline constexpr auto& OnDialogCancel = lifted::f821B9F98;  // sub_821B9F98
inline constexpr auto& OnEnter = lifted::entry::e821BB9A8;  // sub_821BB9A8
inline constexpr auto& OnLeft = lifted::entry::e821B9E70;  // sub_821B9E70
inline constexpr auto& OpenCategories = lifted::entry::e821BD468;  // sub_821BD468
inline constexpr auto& OpenCategory = lifted::f821C7188;  // sub_821C7188
inline constexpr auto& OpenMovie = lifted::entry::e821B9F88;  // sub_821B9F88
inline constexpr auto& PlayMusicPreview = lifted::f821BA0F0;  // sub_821BA0F0
inline constexpr auto& ReturnToCategories = lifted::entry::e821BD5B0;  // sub_821BD5B0
inline constexpr auto& SavePage = lifted::entry::e821C6F58;  // sub_821C6F58
inline constexpr auto& SetHudArrow = lifted::f821BE4A0;  // sub_821BE4A0
inline constexpr auto& SetSlidersArrow = lifted::f821BEDC8;  // sub_821BEDC8
inline constexpr auto& SetSlidersArrow2P = lifted::f821BEF68;  // sub_821BEF68
inline constexpr auto& SetSoundArrow = lifted::f821BE7A8;  // sub_821BE7A8
inline constexpr auto& SetUpAnalogPage = lifted::entry::e821C57D0;  // sub_821C57D0
inline constexpr auto& SetUpAnalogPage2P = lifted::f821C5900;  // sub_821C5900
inline constexpr auto& SetUpButtonsPage = lifted::entry::e821C5580;  // sub_821C5580
inline constexpr auto& SetUpButtonsPage2P = lifted::f821C56B0;  // sub_821C56B0
inline constexpr auto& SetUpCategory6Page = lifted::entry::e821C6E50;  // sub_821C6E50
inline constexpr auto& SetUpCategoryPage = lifted::entry::e821C4BE0;  // sub_821C4BE0
inline constexpr auto& SetUpHudPage = lifted::entry::e821C4DB0;  // sub_821C4DB0
inline constexpr auto& SetUpSavePage = lifted::entry::e821C6D48;  // sub_821C6D48
inline constexpr auto& SetUpSlidersPage = lifted::entry::e821C51D0;  // sub_821C51D0
inline constexpr auto& SetUpSlidersPage2P = lifted::f821C5350;  // sub_821C5350
inline constexpr auto& SetUpSoundPage = lifted::entry::e821C4FD0;  // sub_821C4FD0
inline constexpr auto& Show = lifted::entry::e821C1158;  // sub_821C1158
inline constexpr auto& ShowAnalogClashes2P = lifted::f821BF908;  // sub_821BF908
inline constexpr auto& ShowAnalogList2P = lifted::f821BF7F8;  // sub_821BF7F8
inline constexpr auto& ShowButtonClashes2P = lifted::f821BF3C0;  // sub_821BF3C0
inline constexpr auto& ShowButtonList2P = lifted::f821BF2B8;  // sub_821BF2B8
inline constexpr auto& ShowCareerInfo = lifted::f821C0EF0;  // sub_821C0EF0
inline constexpr auto& ShowCategory6Dialog = lifted::entry::e821C69D0;  // sub_821C69D0
inline constexpr auto& ShowPrompts = lifted::entry::e821C1068;  // sub_821C1068
inline constexpr auto& ShowSaveDialog = lifted::entry::e821C6638;  // sub_821C6638
inline constexpr auto& ShowSliderMark = lifted::entry::e821BEA48;  // sub_821BEA48
inline constexpr auto& ShowSliderMarks2P = lifted::f821BEED8;  // sub_821BEED8
inline constexpr auto& ShowSliderNumbers2P = lifted::f821BEE48;  // sub_821BEE48
inline constexpr auto& ShowVolumeBar = lifted::entry::e821BE5A8;  // sub_821BE5A8
inline constexpr auto& SlidersPage = lifted::entry::e821C1888;  // sub_821C1888
inline constexpr auto& SlidersPage2P = lifted::f821C1AA0;  // sub_821C1AA0
inline constexpr auto& SoundPage = lifted::entry::e821C1498;  // sub_821C1498
inline constexpr auto& StopMusicPreview = lifted::f821BA010;  // sub_821BA010
inline constexpr auto& Update = lifted::entry::e821C81E8;  // sub_821C81E8
inline constexpr auto& Wait = lifted::entry::e821BD3C8;  // sub_821BD3C8
inline constexpr auto& WaitForMovie = lifted::entry::e821BBBA0;  // sub_821BBBA0
}  // namespace game::Options

namespace game::OptionsSavePages {
inline constexpr auto& Cancel = lifted::entry::e821B9FF8;  // sub_821B9FF8
}  // namespace game::OptionsSavePages

namespace game::Overlay {
inline constexpr auto& Ctor = lifted::entry::e82126498;  // sub_82126498
inline constexpr auto& LayerFor = lifted::entry::e82126630;  // sub_82126630
inline constexpr auto& SetTextures104 = lifted::entry::e8212C0E0;  // sub_8212C0E0
inline constexpr auto& SetTextures108 = lifted::entry::e8212D6D8;  // sub_8212D6D8
}  // namespace game::Overlay

namespace game::OverlayView {
inline constexpr auto& BodyMatrixCopy = lifted::entry::e82126FF0;  // sub_82126FF0
inline constexpr auto& BodyPosition = lifted::entry::e82126F80;  // sub_82126F80
inline constexpr auto& Ctor = lifted::entry::e82126870;  // sub_82126870
inline constexpr auto& ModelMatrix = lifted::entry::e821270E8;  // sub_821270E8
inline constexpr auto& ModelPoint = lifted::f82126ED0;  // sub_82126ED0
inline constexpr auto& RacerFlags = lifted::entry::e82127178;  // sub_82127178
inline constexpr auto& RacerId = lifted::entry::e82126AE0;  // sub_82126AE0
inline constexpr auto& Set = lifted::entry::e82126890;  // sub_82126890
inline constexpr auto& SpeedKmh = lifted::entry::e821268C0;  // sub_821268C0
inline constexpr auto& VehicleWord340 = lifted::entry::e82126AB0;  // sub_82126AB0
}  // namespace game::OverlayView

namespace game::Overlays {
inline constexpr auto& SetRenderTargets = lifted::entry::e82128190;  // sub_82128190
}  // namespace game::Overlays

namespace game::PaintPalette {
inline constexpr auto& Init = lifted::entry::e820D2330;  // sub_820D2330
}  // namespace game::PaintPalette

namespace game::PairedNodeTable {
inline constexpr auto& Refresh = lifted::entry::e82248800;  // sub_82248800
inline constexpr auto& SavePairs = lifted::entry::e82247CE0;  // sub_82247CE0
}  // namespace game::PairedNodeTable

namespace game::Panel6C {
inline constexpr auto& Ctor = lifted::entry::e82231CD0;  // sub_82231CD0
inline constexpr auto& DeletingDestructor = lifted::f82231EF0;  // sub_82231EF0
inline constexpr auto& SetStats = lifted::entry::e8224E490;  // sub_8224E490
inline constexpr auto& ShowArrows = lifted::f82231DF0;  // sub_82231DF0
}  // namespace game::Panel6C

namespace game::Panel6D {
inline constexpr auto& Ctor = lifted::entry::e82231E40;  // sub_82231E40
}  // namespace game::Panel6D

namespace game::Panel6F {
inline constexpr auto& Ctor = lifted::entry::e822308E8;  // sub_822308E8
}  // namespace game::Panel6F

namespace game::Panel7C {
inline constexpr auto& Ctor = lifted::entry::e822416D8;  // sub_822416D8
inline constexpr auto& DeletingDestructor = lifted::f82241968;  // sub_82241968
inline constexpr auto& ShowArrows = lifted::entry::e82241868;  // sub_82241868
}  // namespace game::Panel7C

namespace game::Panel7D {
inline constexpr auto& Ctor = lifted::entry::e822418B8;  // sub_822418B8
}  // namespace game::Panel7D

namespace game::Panel7F {
inline constexpr auto& Ctor = lifted::entry::e82240268;  // sub_82240268
}  // namespace game::Panel7F

namespace game::Panel8C {
inline constexpr auto& Ctor = lifted::entry::e82239E78;  // sub_82239E78
inline constexpr auto& DeletingDestructor = lifted::entry::e8223A348;  // sub_8223A348
inline constexpr auto& SetStats = lifted::entry::e8223A090;  // sub_8223A090
inline constexpr auto& ShowArrows = lifted::entry::e8223A098;  // sub_8223A098
}  // namespace game::Panel8C

namespace game::Panel8D {
inline constexpr auto& Ctor = lifted::entry::e8223A100;  // sub_8223A100
}  // namespace game::Panel8D

namespace game::Panel8E {
inline constexpr auto& Ctor = lifted::entry::e82239D58;  // sub_82239D58
}  // namespace game::Panel8E

namespace game::Panel8F {
inline constexpr auto& Ctor = lifted::entry::e82239C70;  // sub_82239C70
}  // namespace game::Panel8F

namespace game::PanelE {
inline constexpr auto& Ctor = lifted::entry::e82230800;  // sub_82230800
}  // namespace game::PanelE

namespace game::PanelFade {
inline constexpr auto& In = lifted::entry::e82249BE8;  // sub_82249BE8
}  // namespace game::PanelFade

namespace game::PanelGroup {
inline constexpr auto& Ctor = lifted::entry::e82240110;  // sub_82240110
inline constexpr auto& DeletingDestructor = lifted::entry::e82240348;  // sub_82240348
inline constexpr auto& Dtor = lifted::entry::e82231CC0;  // sub_82231CC0
inline constexpr auto& Movie = lifted::entry::e82258FC8;  // sub_82258FC8
inline constexpr auto& SetCaption = lifted::entry::e8224EF18;  // sub_8224EF18
}  // namespace game::PanelGroup

namespace game::PanelGroup6B {
inline constexpr auto& Ctor = lifted::entry::e82231C68;  // sub_82231C68
}  // namespace game::PanelGroup6B

namespace game::PanelGroup7B {
inline constexpr auto& Ctor = lifted::entry::e82241680;  // sub_82241680
}  // namespace game::PanelGroup7B

namespace game::PanelGroup8 {
inline constexpr auto& Ctor = lifted::entry::e82239C10;  // sub_82239C10
inline constexpr auto& DeletingDestructor = lifted::entry::e8223A2A8;  // sub_8223A2A8
}  // namespace game::PanelGroup8

namespace game::PanelGroupBase {
inline constexpr auto& DeletingDestructor = lifted::f82230B50;  // sub_82230B50
inline constexpr auto& Dtor = lifted::entry::e8224EF08;  // sub_8224EF08
}  // namespace game::PanelGroupBase

namespace game::PanelScreen {
inline constexpr auto& DeleteMovies = lifted::entry::e82240208;  // sub_82240208
inline constexpr auto& InitWords = lifted::entry::e82230740;  // sub_82230740
inline constexpr auto& IsLoaded = lifted::entry::e82230790;  // sub_82230790
inline constexpr auto& Leave = lifted::entry::e82249E38;  // sub_82249E38
inline constexpr auto& OnBack = lifted::entry::e822306D0;  // sub_822306D0
inline constexpr auto& OnDialogClosed = lifted::entry::e822306E0;  // sub_822306E0
inline constexpr auto& OnEnter = lifted::entry::e82249D18;  // sub_82249D18
inline constexpr auto& OnLeft = lifted::entry::e82248B08;  // sub_82248B08
inline constexpr auto& OnMessage24 = lifted::entry::e82248DF8;  // sub_82248DF8
inline constexpr auto& OnNo = lifted::entry::e822609D0;  // sub_822609D0
inline constexpr auto& OnYes = lifted::entry::e822609E0;  // sub_822609E0
inline constexpr auto& Open = lifted::entry::e82249DF8;  // sub_82249DF8
inline constexpr auto& OpenEmpty = lifted::entry::e822404D8;  // sub_822404D8
inline constexpr auto& OpenMovie = lifted::entry::e82248CD0;  // sub_82248CD0
inline constexpr auto& Show = lifted::entry::e82248CE0;  // sub_82248CE0
inline constexpr auto& Suspend = lifted::entry::e82248DE0;  // sub_82248DE0
inline constexpr auto& Update = lifted::entry::e82248C48;  // sub_82248C48
inline constexpr auto& WaitForMovie = lifted::entry::e82249D88;  // sub_82249D88
}  // namespace game::PanelScreen

namespace game::PanelSet {
inline constexpr auto& Build = lifted::entry::e8224CAD8;  // sub_8224CAD8
inline constexpr auto& Ctor = lifted::entry::e8224B660;  // sub_8224B660
inline constexpr auto& DeletingDestructor = lifted::f8224C9D8;  // sub_8224C9D8
inline constexpr auto& Dtor = lifted::entry::e8224B708;  // sub_8224B708
inline constexpr auto& Free = lifted::entry::e8224B880;  // sub_8224B880
inline constexpr auto& HandleInput = lifted::entry::e8224CA28;  // sub_8224CA28
inline constexpr auto& Open = lifted::entry::e82249730;  // sub_82249730
inline constexpr auto& OpenKind = lifted::f8224B6E0;  // sub_8224B6E0
inline constexpr auto& Picked = lifted::entry::e82249848;  // sub_82249848
inline constexpr auto& Reset = lifted::entry::e822496C8;  // sub_822496C8
inline constexpr auto& SetKind = lifted::f8224B700;  // sub_8224B700
inline constexpr auto& SetOpenKind = lifted::entry::e8224B6D8;  // sub_8224B6D8
inline constexpr auto& SetPreviousColor = lifted::entry::e8224B6F8;  // sub_8224B6F8
inline constexpr auto& SetPreviousEntry = lifted::entry::e8224B6F0;  // sub_8224B6F0
inline constexpr auto& SetPreviousTab = lifted::entry::e8224B6E8;  // sub_8224B6E8
inline constexpr auto& Update = lifted::entry::e8224CF18;  // sub_8224CF18
inline constexpr auto& WantsPowerGraph = lifted::entry::e82249898;  // sub_82249898
inline constexpr auto& WentBack = lifted::entry::e82249870;  // sub_82249870
}  // namespace game::PanelSet

namespace game::PanelSetBase {
inline constexpr auto& BackPage7 = lifted::f8224A368;  // sub_8224A368
inline constexpr auto& BackPage8 = lifted::entry::e8224A3F8;  // sub_8224A3F8
inline constexpr auto& CarPage = lifted::entry::e8224B8D8;  // sub_8224B8D8
inline constexpr auto& ColorsPage = lifted::entry::e8224AEA8;  // sub_8224AEA8
inline constexpr auto& ConfirmPage = lifted::f82248E60;  // sub_82248E60
inline constexpr auto& DeletingDestructor = lifted::f8224B888;  // sub_8224B888
inline constexpr auto& Dtor = lifted::entry::e8224AE60;  // sub_8224AE60
inline constexpr auto& PickedPage = lifted::entry::e82248EF0;  // sub_82248EF0
inline constexpr auto& PowerGraphPage = lifted::entry::e8224BDC0;  // sub_8224BDC0
inline constexpr auto& RowsPage = lifted::entry::e8224A178;  // sub_8224A178
inline constexpr auto& SaveData = lifted::entry::e82249668;  // sub_82249668
inline constexpr auto& SetKind = lifted::f82249610;  // sub_82249610
inline constexpr auto& ShowStandCar = lifted::entry::e8224A490;  // sub_8224A490
inline constexpr auto& Step0 = lifted::entry::e8224A570;  // sub_8224A570
inline constexpr auto& Step1 = lifted::entry::e8224A6A0;  // sub_8224A6A0
inline constexpr auto& Step10 = lifted::f8224CC00;  // sub_8224CC00
inline constexpr auto& Step11 = lifted::entry::e8224A7E0;  // sub_8224A7E0
inline constexpr auto& Step12 = lifted::entry::e82249128;  // sub_82249128
inline constexpr auto& Step13 = lifted::f8224A850;  // sub_8224A850
inline constexpr auto& Step14 = lifted::f8224B1C8;  // sub_8224B1C8
inline constexpr auto& Step15 = lifted::f82249198;  // sub_82249198
inline constexpr auto& Step16 = lifted::f822491F8;  // sub_822491F8
inline constexpr auto& Step17 = lifted::entry::e8224C450;  // sub_8224C450
inline constexpr auto& Step18 = lifted::entry::e82249258;  // sub_82249258
inline constexpr auto& Step19 = lifted::entry::e822492C8;  // sub_822492C8
inline constexpr auto& Step2 = lifted::entry::e82248F00;  // sub_82248F00
inline constexpr auto& Step20 = lifted::f8224CC78;  // sub_8224CC78
inline constexpr auto& Step21 = lifted::f822493D8;  // sub_822493D8
inline constexpr auto& Step22 = lifted::f82249458;  // sub_82249458
inline constexpr auto& Step23 = lifted::f822494A8;  // sub_822494A8
inline constexpr auto& Step24 = lifted::f82249510;  // sub_82249510
inline constexpr auto& Step25 = lifted::f8224C4D8;  // sub_8224C4D8
inline constexpr auto& Step26 = lifted::entry::e8224A8E0;  // sub_8224A8E0
inline constexpr auto& Step3 = lifted::entry::e8224A718;  // sub_8224A718
inline constexpr auto& Step4 = lifted::entry::e8224B078;  // sub_8224B078
inline constexpr auto& Step5 = lifted::f8224B138;  // sub_8224B138
inline constexpr auto& Step6 = lifted::entry::e82248F70;  // sub_82248F70
inline constexpr auto& Step7 = lifted::entry::e82249098;  // sub_82249098
inline constexpr auto& Step8 = lifted::entry::e82248FE0;  // sub_82248FE0
inline constexpr auto& Step9 = lifted::entry::e8224CB28;  // sub_8224CB28
inline constexpr auto& TabsPage = lifted::entry::e82249EB8;  // sub_82249EB8
inline constexpr auto& UpdateStand = lifted::entry::e8224C008;  // sub_8224C008
}  // namespace game::PanelSetBase

namespace game::PanelSetInterface {
inline constexpr auto& DeletingDestructor = lifted::entry::e8224AC98;  // sub_8224AC98
}  // namespace game::PanelSetInterface

namespace game::PanelSetPair {
inline constexpr auto& Build = lifted::entry::e8224CAE0;  // sub_8224CAE0
inline constexpr auto& Ctor = lifted::entry::e8224B750;  // sub_8224B750
inline constexpr auto& DeletingDestructor = lifted::entry::e8224CA30;  // sub_8224CA30
inline constexpr auto& Dtor = lifted::entry::e8224B830;  // sub_8224B830
inline constexpr auto& HandleInput = lifted::entry::e8224CA80;  // sub_8224CA80
inline constexpr auto& Open = lifted::entry::e822499F0;  // sub_822499F0
inline constexpr auto& Picked = lifted::entry::e82249B60;  // sub_82249B60
inline constexpr auto& Reset = lifted::entry::e822498C0;  // sub_822498C0
inline constexpr auto& Update = lifted::entry::e8224CF20;  // sub_8224CF20
}  // namespace game::PanelSetPair

namespace game::Params {
inline constexpr auto& Exists = lifted::entry::e820C9DD8;  // sub_820C9DD8
inline constexpr auto& FirstWheelOfModel = lifted::entry::e820C9EE0;  // sub_820C9EE0
inline constexpr auto& Free = lifted::f820C9D90;  // sub_820C9D90
inline constexpr auto& GetCarParts = lifted::entry::e820C9E00;  // sub_820C9E00
inline constexpr auto& GetCarSpec = lifted::entry::e820C9E98;  // sub_820C9E98
inline constexpr auto& Init = lifted::entry::e820C9960;  // sub_820C9960
inline constexpr auto& Load = lifted::entry::e820CC2A8;  // sub_820CC2A8
inline constexpr auto& OnCarSpecsLoaded = lifted::entry::e820CC248;  // sub_820CC248
inline constexpr auto& OnLoaded = lifted::entry::e820C9DF0;  // sub_820C9DF0
inline constexpr auto& OnTable83Loaded = lifted::entry::e820CBD90;  // sub_820CBD90
inline constexpr auto& OnWheelsLoaded = lifted::entry::e820CC0A8;  // sub_820CC0A8
}  // namespace game::Params

namespace game::PartModel {
inline constexpr auto& ApplyPalette = lifted::entry::e820D2EA8;  // sub_820D2EA8
inline constexpr auto& Clear = lifted::entry::e820CE9C8;  // sub_820CE9C8
inline constexpr auto& Ctor = lifted::entry::e820CE928;  // sub_820CE928
inline constexpr auto& Dtor = lifted::entry::e820CE990;  // sub_820CE990
inline constexpr auto& FlipCulling = lifted::entry::e820D1950;  // sub_820D1950
inline constexpr auto& ScaleBlended = lifted::entry::e820D1A68;  // sub_820D1A68
inline constexpr auto& SetAlpha = lifted::entry::e820D1C00;  // sub_820D1C00
inline constexpr auto& SetMaterials = lifted::entry::e820D3318;  // sub_820D3318
inline constexpr auto& SetPaint = lifted::entry::e820D0EA8;  // sub_820D0EA8
inline constexpr auto& SetPaintBody = lifted::entry::e820D1378;  // sub_820D1378
inline constexpr auto& SetPair1002Alpha = lifted::entry::e820D1B38;  // sub_820D1B38
inline constexpr auto& SetTexture = lifted::entry::e820D17D8;  // sub_820D17D8
}  // namespace game::PartModel

namespace game::Particle {
inline constexpr auto& Ctor = lifted::entry::e8212B3D0;  // sub_8212B3D0
}  // namespace game::Particle

namespace game::ParticlesB {
inline constexpr auto& Ctor = lifted::entry::e8212BFE8;  // sub_8212BFE8
inline constexpr auto& Dtor = lifted::entry::e8212C070;  // sub_8212C070
inline constexpr auto& Reset = lifted::entry::e8212BA98;  // sub_8212BA98
inline constexpr auto& Update = lifted::entry::e8212B748;  // sub_8212B748
}  // namespace game::ParticlesB

namespace game::ParticlesC {
inline constexpr auto& Ctor = lifted::entry::e8212CAF8;  // sub_8212CAF8
inline constexpr auto& Dtor = lifted::f8212CB70;  // sub_8212CB70
inline constexpr auto& Init = lifted::f8212C0C0;  // sub_8212C0C0
inline constexpr auto& Reset = lifted::entry::e8212C0E8;  // sub_8212C0E8
inline constexpr auto& Update = lifted::entry::e8212C2F0;  // sub_8212C2F0
}  // namespace game::ParticlesC

namespace game::PartsPoints {
inline constexpr auto& DrawBonnet = lifted::entry::e820E0998;  // sub_820E0998
inline constexpr auto& DrawCabinWindow01 = lifted::entry::e820DEFA8;  // sub_820DEFA8
inline constexpr auto& DrawCabinWindow02 = lifted::entry::e820DEF38;  // sub_820DEF38
inline constexpr auto& DrawDoorWindows01 = lifted::entry::e820DE948;  // sub_820DE948
inline constexpr auto& DrawDoorWindows02 = lifted::entry::e820DE760;  // sub_820DE760
inline constexpr auto& DrawDoorsBody = lifted::entry::e820DEA88;  // sub_820DEA88
inline constexpr auto& DrawDoorsWindows = lifted::entry::e820DEAF0;  // sub_820DEAF0
inline constexpr auto& DrawEngine = lifted::entry::e820DF070;  // sub_820DF070
inline constexpr auto& DrawFenderLights = lifted::entry::e820DEDD8;  // sub_820DEDD8
inline constexpr auto& DrawFendersBody = lifted::entry::e820DED88;  // sub_820DED88
inline constexpr auto& DrawFrontBumper = lifted::entry::e820DF308;  // sub_820DF308
inline constexpr auto& DrawFrontBumperLights = lifted::entry::e820DE1C8;  // sub_820DE1C8
inline constexpr auto& DrawHeadlightBody = lifted::entry::e820E13E0;  // sub_820E13E0
inline constexpr auto& DrawHeadlights = lifted::entry::e820E1690;  // sub_820E1690
inline constexpr auto& DrawLeftDoor = lifted::entry::e820E0B40;  // sub_820E0B40
inline constexpr auto& DrawLeftDoorPanel = lifted::entry::e820DE6E0;  // sub_820DE6E0
inline constexpr auto& DrawMirror = lifted::entry::e820E1138;  // sub_820E1138
inline constexpr auto& DrawRearBumper = lifted::entry::e820DF458;  // sub_820DF458
inline constexpr auto& DrawRearBumperLights = lifted::entry::e820DE3C0;  // sub_820DE3C0
inline constexpr auto& DrawRightDoor = lifted::entry::e820E0E38;  // sub_820E0E38
inline constexpr auto& DrawRightDoorPanel = lifted::entry::e820DE8C8;  // sub_820DE8C8
inline constexpr auto& DrawShadows = lifted::entry::e820E1D88;  // sub_820E1D88
inline constexpr auto& DrawTailLights = lifted::entry::e820DF8B0;  // sub_820DF8B0
inline constexpr auto& DrawWithShadow = lifted::entry::e820E1260;  // sub_820E1260
inline constexpr auto& Find = lifted::entry::e820E0258;  // sub_820E0258
inline constexpr auto& Reset = lifted::entry::e820E00F0;  // sub_820E00F0
}  // namespace game::PartsPoints

namespace game::PassList {
inline constexpr auto& Dtor = lifted::f82590C48;  // sub_82590C48
}  // namespace game::PassList

namespace game::Path {
inline constexpr auto& Text = lifted::f82593C40;  // sub_82593C40
}  // namespace game::Path

namespace game::PauseMenu {
inline constexpr auto& AdjustPage = lifted::entry::e8220EBD8;  // sub_8220EBD8
inline constexpr auto& AnswerPage = lifted::entry::e8220C5D0;  // sub_8220C5D0
inline constexpr auto& BackToMain = lifted::entry::e8220C668;  // sub_8220C668
inline constexpr auto& CloseAdjust = lifted::entry::e8220F2A8;  // sub_8220F2A8
inline constexpr auto& CloseMain = lifted::entry::e8220C700;  // sub_8220C700
inline constexpr auto& CloseOptions = lifted::entry::e8220EFA8;  // sub_8220EFA8
inline constexpr auto& CloseVolume = lifted::entry::e8220F0A0;  // sub_8220F0A0
inline constexpr auto& ConfirmPage = lifted::entry::e8220C578;  // sub_8220C578
inline constexpr auto& Draw = lifted::entry::e8220FB10;  // sub_8220FB10
inline constexpr auto& LoadData = lifted::entry::e8220F840;  // sub_8220F840
inline constexpr auto& MainPage = lifted::entry::e8220D898;  // sub_8220D898
inline constexpr auto& MoveAdjust = lifted::entry::e8220E1F8;  // sub_8220E1F8
inline constexpr auto& MoveMain = lifted::entry::e8220C7B8;  // sub_8220C7B8
inline constexpr auto& MoveOptions = lifted::entry::e8220DD98;  // sub_8220DD98
inline constexpr auto& MoveVolume = lifted::f8220DF30;  // sub_8220DF30
inline constexpr auto& OpenAdjust = lifted::entry::e8220F788;  // sub_8220F788
inline constexpr auto& OpenMain = lifted::entry::e8220ED90;  // sub_8220ED90
inline constexpr auto& OpenOptions = lifted::entry::e8220EEF0;  // sub_8220EEF0
inline constexpr auto& OpenVolume = lifted::entry::e8220F6D0;  // sub_8220F6D0
inline constexpr auto& OptionsPage = lifted::entry::e8220DB98;  // sub_8220DB98
inline constexpr auto& RunTen = lifted::entry::e8220C5E0;  // sub_8220C5E0
inline constexpr auto& SaveOptions = lifted::entry::e8220D6A0;  // sub_8220D6A0
inline constexpr auto& SetAdjust = lifted::entry::e8220F1B8;  // sub_8220F1B8
inline constexpr auto& SetArrowB = lifted::f8220CE68;  // sub_8220CE68
inline constexpr auto& SetArrowC = lifted::f8220D1D8;  // sub_8220D1D8
inline constexpr auto& SetVolume = lifted::f8220E0F8;  // sub_8220E0F8
inline constexpr auto& ShowAdjustRow = lifted::entry::e8220CAB0;  // sub_8220CAB0
inline constexpr auto& ShowMain = lifted::entry::e8220EE28;  // sub_8220EE28
inline constexpr auto& ShowOptionsRow = lifted::entry::e8220C868;  // sub_8220C868
inline constexpr auto& ShowVolume = lifted::entry::e8220D040;  // sub_8220D040
inline constexpr auto& ShowVolumeRow = lifted::entry::e8220CA28;  // sub_8220CA28
inline constexpr auto& ToggleOptions = lifted::entry::e8220C930;  // sub_8220C930
inline constexpr auto& VolumePage = lifted::entry::e8220E988;  // sub_8220E988
}  // namespace game::PauseMenu

namespace game::PauseMenu4 {
inline constexpr auto& Draw = lifted::entry::e82211590;  // sub_82211590
inline constexpr auto& Open = lifted::entry::e82211390;  // sub_82211390
inline constexpr auto& Update = lifted::entry::e82211458;  // sub_82211458
}  // namespace game::PauseMenu4

namespace game::PcmCodec {
inline constexpr auto& Process = lifted::entry::e8246D860;  // sub_8246D860
inline constexpr auto& SetFormat = lifted::entry::e8246CFA8;  // sub_8246CFA8
inline constexpr auto& SetPitch = lifted::entry::e8246D0C8;  // sub_8246D0C8
inline constexpr auto& SetVolume = lifted::entry::e8246D008;  // sub_8246D008
inline constexpr auto& Start = lifted::entry::e8246D410;  // sub_8246D410
inline constexpr auto& Stop = lifted::entry::e8246D500;  // sub_8246D500
}  // namespace game::PcmCodec

namespace game::PendingFrees {
inline constexpr auto& FreeDue = lifted::entry::e8258E1D8;  // sub_8258E1D8
inline constexpr auto& Insert = lifted::entry::e8258E398;  // sub_8258E398
}  // namespace game::PendingFrees

namespace game::PhysicalPad {
inline constexpr auto& Values = lifted::entry::e820988B8;  // sub_820988B8
}  // namespace game::PhysicalPad

namespace game::Pick {
inline constexpr auto& IsSpecial = lifted::f821115C8;  // sub_821115C8
}  // namespace game::Pick

namespace game::PixelShader {
inline constexpr auto& Create = lifted::entry::e822A9EE8;  // sub_822A9EE8
}  // namespace game::PixelShader

namespace game::Plane {
inline constexpr auto& Intersect3 = lifted::entry::e825976C8;  // sub_825976C8
}  // namespace game::Plane

namespace game::PlateEditor {
inline constexpr auto& CharsPage = lifted::entry::e82198540;  // sub_82198540
inline constexpr auto& Close = lifted::entry::e82198498;  // sub_82198498
inline constexpr auto& ConfirmPage = lifted::f82198830;  // sub_82198830
inline constexpr auto& Create = lifted::entry::e82198210;  // sub_82198210
inline constexpr auto& Destroy = lifted::entry::e82198118;  // sub_82198118
inline constexpr auto& InitStates = lifted::entry::e8219B2E8;  // sub_8219B2E8
inline constexpr auto& LeavePage = lifted::entry::e82198988;  // sub_82198988
inline constexpr auto& NumberPage = lifted::entry::e8219A138;  // sub_8219A138
inline constexpr auto& OnMessage32 = lifted::entry::e82198190;  // sub_82198190
inline constexpr auto& OpenMovie = lifted::entry::e82198180;  // sub_82198180
inline constexpr auto& RegionPage = lifted::entry::e82199ED0;  // sub_82199ED0
inline constexpr auto& SetUpConfirmMenu = lifted::f82199E48;  // sub_82199E48
inline constexpr auto& ShowCharRow = lifted::f821995D8;  // sub_821995D8
inline constexpr auto& ShowConfirmCells = lifted::f821997E8;  // sub_821997E8
inline constexpr auto& ShowDigitCells = lifted::entry::e82199350;  // sub_82199350
inline constexpr auto& ShowRegionRow = lifted::f82199220;  // sub_82199220
inline constexpr auto& Start = lifted::entry::e82199BB8;  // sub_82199BB8
inline constexpr auto& Transition0 = lifted::entry::e8219A3A8;  // sub_8219A3A8
inline constexpr auto& Transition1 = lifted::entry::e82198A40;  // sub_82198A40
inline constexpr auto& Transition10 = lifted::entry::e8219AA00;  // sub_8219AA00
inline constexpr auto& Transition11 = lifted::entry::e82198DC8;  // sub_82198DC8
inline constexpr auto& Transition12 = lifted::f8219AAB0;  // sub_8219AAB0
inline constexpr auto& Transition13 = lifted::entry::e8219AC88;  // sub_8219AC88
inline constexpr auto& Transition14 = lifted::f8219AD68;  // sub_8219AD68
inline constexpr auto& Transition15 = lifted::f8219AE90;  // sub_8219AE90
inline constexpr auto& Transition16 = lifted::f82198E70;  // sub_82198E70
inline constexpr auto& Transition17 = lifted::f82198F08;  // sub_82198F08
inline constexpr auto& Transition18 = lifted::f8219AF50;  // sub_8219AF50
inline constexpr auto& Transition19 = lifted::entry::e82198FB8;  // sub_82198FB8
inline constexpr auto& Transition2 = lifted::f82198B58;  // sub_82198B58
inline constexpr auto& Transition3 = lifted::f8219A450;  // sub_8219A450
inline constexpr auto& Transition4 = lifted::entry::e82198C20;  // sub_82198C20
inline constexpr auto& Transition5 = lifted::entry::e8219A5E8;  // sub_8219A5E8
inline constexpr auto& Transition6 = lifted::entry::e82198D00;  // sub_82198D00
inline constexpr auto& Transition7 = lifted::f8219A6B8;  // sub_8219A6B8
inline constexpr auto& Transition8 = lifted::entry::e8219A820;  // sub_8219A820
inline constexpr auto& Transition9 = lifted::f8219A8E0;  // sub_8219A8E0
inline constexpr auto& Update = lifted::entry::e8219B108;  // sub_8219B108
inline constexpr auto& UpdatePage = lifted::entry::e8219B050;  // sub_8219B050
inline constexpr auto& WaitForLoad = lifted::entry::e821982F0;  // sub_821982F0
}  // namespace game::PlateEditor

namespace game::PlayTime {
inline constexpr auto& Add = lifted::entry::e821109B8;  // sub_821109B8
inline constexpr auto& Mark = lifted::entry::e82111628;  // sub_82111628
inline constexpr auto& Update = lifted::entry::e82112838;  // sub_82112838
}  // namespace game::PlayTime

namespace game::PlayerControl {
inline constexpr auto& ReadPad = lifted::entry::e8215FCA8;  // sub_8215FCA8
}  // namespace game::PlayerControl

namespace game::Players {
inline constexpr auto& ResetControllers = lifted::entry::e8209F5E0;  // sub_8209F5E0
inline constexpr auto& SignedInSlot = lifted::entry::e8209F658;  // sub_8209F658
}  // namespace game::Players

namespace game::PointList {
inline constexpr auto& Draw = lifted::entry::e82148058;  // sub_82148058
}  // namespace game::PointList

namespace game::Pool4Object {
inline constexpr auto& DeletingDestructor = lifted::entry::e820D6BC0;  // sub_820D6BC0
inline constexpr auto& IsLoaded = lifted::entry::e820D6A08;  // sub_820D6A08
inline constexpr auto& SetRecord = lifted::entry::e820D6A60;  // sub_820D6A60
}  // namespace game::Pool4Object

namespace game::PoolMaker {
inline constexpr auto& Ctor = lifted::entry::e820AC8F8;  // sub_820AC8F8
inline constexpr auto& MakeSkinJob = lifted::entry::e820AC8A0;  // sub_820AC8A0
}  // namespace game::PoolMaker

namespace game::PoolMaker2 {
inline constexpr auto& MakeMorphJob = lifted::entry::e820AC7F0;  // sub_820AC7F0
}  // namespace game::PoolMaker2

namespace game::PoolNode {
inline constexpr auto& Ctor = lifted::entry::e82594050;  // sub_82594050
inline constexpr auto& Dtor = lifted::entry::e825940B0;  // sub_825940B0
}  // namespace game::PoolNode

namespace game::PoolObject {
inline constexpr auto& DeletingDestructor = lifted::entry::e820DB400;  // sub_820DB400
inline constexpr auto& InUse = lifted::entry::e820FB8E0;  // sub_820FB8E0
inline constexpr auto& IsLoaded = lifted::entry::e820FB8F0;  // sub_820FB8F0
inline constexpr auto& Key = lifted::entry::e820FB9F0;  // sub_820FB9F0
inline constexpr auto& Reset = lifted::entry::e820FB9D8;  // sub_820FB9D8
}  // namespace game::PoolObject

namespace game::PostEffect {
inline constexpr auto& AddQuad = lifted::entry::e82133E30;  // sub_82133E30
inline constexpr auto& RecordBeginStates = lifted::entry::e82133340;  // sub_82133340
inline constexpr auto& RecordEndStates = lifted::entry::e82133608;  // sub_82133608
inline constexpr auto& RecordPixelConstants = lifted::entry::e82133D00;  // sub_82133D00
inline constexpr auto& RecordResolve = lifted::entry::e82133F58;  // sub_82133F58
inline constexpr auto& RecordTargets = lifted::entry::e82133790;  // sub_82133790
inline constexpr auto& RecordTexelSize = lifted::entry::e82133DD8;  // sub_82133DD8
}  // namespace game::PostEffect

namespace game::PostEffects {
inline constexpr auto& Create = lifted::entry::e82138A18;  // sub_82138A18
inline constexpr auto& DestroyRace = lifted::entry::e82133A70;  // sub_82133A70
inline constexpr auto& Get = lifted::entry::e82133AE8;  // sub_82133AE8
inline constexpr auto& InitEdramBase = lifted::entry::e825D99D0;  // sub_825D99D0
inline constexpr auto& RecordAll = lifted::entry::e82133960;  // sub_82133960
}  // namespace game::PostEffects

namespace game::PowerCurve {
inline constexpr auto& FromEngine = lifted::entry::e820CE7D8;  // sub_820CE7D8
inline constexpr auto& ShiftPeak = lifted::entry::e820CE1D8;  // sub_820CE1D8
}  // namespace game::PowerCurve

namespace game::PowerGraph {
inline constexpr auto& Draw = lifted::entry::e8217C708;  // sub_8217C708
}  // namespace game::PowerGraph

namespace game::PowerGraphScreen {
inline constexpr auto& Close = lifted::entry::e821B8320;  // sub_821B8320
inline constexpr auto& Create = lifted::entry::e821B8120;  // sub_821B8120
inline constexpr auto& Destroy = lifted::entry::e821B80B8;  // sub_821B80B8
inline constexpr auto& Open = lifted::f821B8238;  // sub_821B8238
inline constexpr auto& OpenMovie = lifted::entry::e82250328;  // sub_82250328
inline constexpr auto& Page0 = lifted::f821B83A8;  // sub_821B83A8
inline constexpr auto& Page1 = lifted::f821B84C8;  // sub_821B84C8
inline constexpr auto& UpdatePage = lifted::f821B8840;  // sub_821B8840
inline constexpr auto& WaitForLoad = lifted::entry::e821B8900;  // sub_821B8900
}  // namespace game::PowerGraphScreen

namespace game::PowerGraphScreen2 {
inline constexpr auto& InitStates = lifted::entry::e821B90D8;  // sub_821B90D8
}  // namespace game::PowerGraphScreen2

namespace game::PowerGraphScreen6 {
inline constexpr auto& InitMachine = lifted::entry::e82237B78;  // sub_82237B78
inline constexpr auto& PostClosed = lifted::entry::e82237B28;  // sub_82237B28
inline constexpr auto& PostClosing = lifted::entry::e82237AD8;  // sub_82237AD8
inline constexpr auto& PostLeave = lifted::entry::e82237A88;  // sub_82237A88
}  // namespace game::PowerGraphScreen6

namespace game::PowerGraphScreen67 {
inline constexpr auto& ClosedPage = lifted::entry::e82250348;  // sub_82250348
inline constexpr auto& Draw = lifted::entry::e82250A20;  // sub_82250A20
inline constexpr auto& Leave = lifted::entry::e82250900;  // sub_82250900
inline constexpr auto& MenuPage = lifted::entry::e82250948;  // sub_82250948
inline constexpr auto& OnEnter = lifted::entry::e82250778;  // sub_82250778
inline constexpr auto& OnLeft = lifted::entry::e822502C0;  // sub_822502C0
inline constexpr auto& Open = lifted::entry::e82250890;  // sub_82250890
inline constexpr auto& Shown = lifted::entry::e82250338;  // sub_82250338
inline constexpr auto& Step0 = lifted::entry::e82251320;  // sub_82251320
inline constexpr auto& Step1 = lifted::entry::e82250AA8;  // sub_82250AA8
inline constexpr auto& Update = lifted::entry::e822513B8;  // sub_822513B8
inline constexpr auto& WaitForLoad = lifted::entry::e82250D70;  // sub_82250D70
}  // namespace game::PowerGraphScreen67

namespace game::PowerGraphScreen7 {
inline constexpr auto& InitMachine = lifted::entry::e822470A8;  // sub_822470A8
inline constexpr auto& PostClosing = lifted::entry::e82247008;  // sub_82247008
inline constexpr auto& PostLeave = lifted::entry::e82246FB8;  // sub_82246FB8
}  // namespace game::PowerGraphScreen7

namespace game::Presence {
inline constexpr auto& SetForMode = lifted::entry::e822934B8;  // sub_822934B8
}  // namespace game::Presence

namespace game::Primitive {
inline constexpr auto& RecordTransform = lifted::entry::e8217AEA8;  // sub_8217AEA8
}  // namespace game::Primitive

namespace game::PrimitiveDrawer {
inline constexpr auto& Ctor = lifted::entry::e825A1378;  // sub_825A1378
inline constexpr auto& Draw = lifted::f825A17A0;  // sub_825A17A0
inline constexpr auto& Init = lifted::f825A1438;  // sub_825A1438
inline constexpr auto& SetShaders = lifted::f825A16B0;  // sub_825A16B0
}  // namespace game::PrimitiveDrawer

namespace game::Progress {
inline constexpr auto& Clear = lifted::entry::e82113BD8;  // sub_82113BD8
inline constexpr auto& Restart = lifted::f82114128;  // sub_82114128
inline constexpr auto& SetDefaults = lifted::entry::e82113E60;  // sub_82113E60
inline constexpr auto& TablesDoneAtLeast = lifted::f82110BE8;  // sub_82110BE8
}  // namespace game::Progress

namespace game::Projector {
inline constexpr auto& Build = lifted::entry::e82597610;  // sub_82597610
inline constexpr auto& BuildOrtho = lifted::entry::e82597518;  // sub_82597518
inline constexpr auto& Matrices = lifted::entry::e820D1EB8;  // sub_820D1EB8
}  // namespace game::Projector

namespace game::PtrList {
inline constexpr auto& AllocateNodes = lifted::entry::e82099C08;  // sub_82099C08
inline constexpr auto& Ctor = lifted::entry::e820AD058;  // sub_820AD058
inline constexpr auto& Erase = lifted::entry::e82098E80;  // sub_82098E80
inline constexpr auto& Insert = lifted::entry::e82095958;  // sub_82095958
}  // namespace game::PtrList

namespace game::PtrVector {
inline constexpr auto& PushBack = lifted::entry::e82591540;  // sub_82591540
}  // namespace game::PtrVector

namespace game::Q16W16V16U16Codec {
inline constexpr auto& DecodeRow = lifted::f823F3E90;  // sub_823F3E90
inline constexpr auto& EncodeRow = lifted::f823F07E8;  // sub_823F07E8
}  // namespace game::Q16W16V16U16Codec

namespace game::Q8W8V8U8Codec {
inline constexpr auto& DecodeRow = lifted::f823F3A88;  // sub_823F3A88
inline constexpr auto& EncodeRow = lifted::f823EFC18;  // sub_823EFC18
}  // namespace game::Q8W8V8U8Codec

namespace game::Quad {
inline constexpr auto& Ctor = lifted::entry::e8211A948;  // sub_8211A948
}  // namespace game::Quad

namespace game::Quaternion {
inline constexpr auto& FromAngles = lifted::entry::e8217C120;  // sub_8217C120
inline constexpr auto& FromAxisAngle = lifted::entry::e82293E68;  // sub_82293E68
inline constexpr auto& FromMatrix = lifted::entry::e82293AD8;  // sub_82293AD8
inline constexpr auto& Multiply = lifted::entry::e82293D78;  // sub_82293D78
inline constexpr auto& NormalizeSelf = lifted::entry::e82293F10;  // sub_82293F10
inline constexpr auto& XAxis = lifted::entry::e82293DF0;  // sub_82293DF0
}  // namespace game::Quaternion

namespace game::QueueEntry {
inline constexpr auto& Sequence = lifted::entry::e8258E5C0;  // sub_8258E5C0
}  // namespace game::QueueEntry

namespace game::R16FCodec {
inline constexpr auto& DecodeRow = lifted::f823F40C8;  // sub_823F40C8
inline constexpr auto& EncodeRow = lifted::f823F0EF0;  // sub_823F0EF0
}  // namespace game::R16FCodec

namespace game::R32FCodec {
inline constexpr auto& DecodeRow = lifted::f823F53B8;  // sub_823F53B8
inline constexpr auto& EncodeRow = lifted::f823F24A8;  // sub_823F24A8
}  // namespace game::R32FCodec

namespace game::R5G6B5Codec {
inline constexpr auto& DecodeRow = lifted::f823F2830;  // sub_823F2830
inline constexpr auto& EncodeRow = lifted::f823EBCD0;  // sub_823EBCD0
}  // namespace game::R5G6B5Codec

namespace game::Race {
inline constexpr auto& AddRacer = lifted::entry::e821017C0;  // sub_821017C0
inline constexpr auto& AddStage = lifted::entry::e82101828;  // sub_82101828
inline constexpr auto& CourseLayout = lifted::entry::e8228F408;  // sub_8228F408
inline constexpr auto& DrawStep = lifted::entry::e8215B4F8;  // sub_8215B4F8
inline constexpr auto& FindPlayerEntry = lifted::entry::e82170608;  // sub_82170608
inline constexpr auto& PlaceRacers = lifted::entry::e82102048;  // sub_82102048
inline constexpr auto& RacerCount = lifted::entry::e821705F8;  // sub_821705F8
inline constexpr auto& SetRoute = lifted::entry::e82101D38;  // sub_82101D38
inline constexpr auto& SetUp = lifted::entry::e82115D98;  // sub_82115D98
inline constexpr auto& SetUpRival = lifted::entry::e82112DB0;  // sub_82112DB0
}  // namespace game::Race

namespace game::RaceData {
inline constexpr auto& Create = lifted::entry::e82102208;  // sub_82102208
inline constexpr auto& Ctor = lifted::entry::e821021B8;  // sub_821021B8
inline constexpr auto& ResetGlobal = lifted::entry::e821021A8;  // sub_821021A8
}  // namespace game::RaceData

namespace game::RaceDialog {
inline constexpr auto& Answer = lifted::entry::e82217D40;  // sub_82217D40
inline constexpr auto& AskSlot = lifted::entry::e82219058;  // sub_82219058
inline constexpr auto& Move = lifted::entry::e82218788;  // sub_82218788
inline constexpr auto& OpenAnim = lifted::entry::e822186A8;  // sub_822186A8
inline constexpr auto& Question = lifted::entry::e82217F40;  // sub_82217F40
inline constexpr auto& ShowMessage = lifted::entry::e82218C10;  // sub_82218C10
inline constexpr auto& ShowSaving = lifted::entry::e82218AB0;  // sub_82218AB0
}  // namespace game::RaceDialog

namespace game::RaceEvent {
inline constexpr auto& Current = lifted::entry::e821434E8;  // sub_821434E8
inline constexpr auto& Destroy = lifted::entry::e82144DF0;  // sub_82144DF0
inline constexpr auto& Entry = lifted::entry::e82143508;  // sub_82143508
inline constexpr auto& FirstOwned = lifted::entry::e821430A0;  // sub_821430A0
inline constexpr auto& GroupItem = lifted::entry::e821431C0;  // sub_821431C0
inline constexpr auto& PlaceStartsCurrent = lifted::entry::e82144A38;  // sub_82144A38
inline constexpr auto& PlaceUnlockedCurrent = lifted::entry::e821442D8;  // sub_821442D8
inline constexpr auto& SectionRivalCurrent = lifted::entry::e821434A0;  // sub_821434A0
}  // namespace game::RaceEvent

namespace game::RaceFlow {
inline constexpr auto& AiUnlessFlagged = lifted::f8215FB08;  // sub_8215FB08
inline constexpr auto& Init = lifted::entry::e82160AE0;  // sub_82160AE0
inline constexpr auto& OnAbort = lifted::f8215F7B0;  // sub_8215F7B0
inline constexpr auto& OnEnd = lifted::entry::e8215F5E8;  // sub_8215F5E8
inline constexpr auto& OnHold = lifted::entry::e8215FC28;  // sub_8215FC28
inline constexpr auto& OnSetUp = lifted::entry::e8215F728;  // sub_8215F728
inline constexpr auto& OnState14 = lifted::f8215FC30;  // sub_8215FC30
inline constexpr auto& OnState16 = lifted::entry::e8215FB90;  // sub_8215FB90
inline constexpr auto& OnState22 = lifted::entry::e82160948;  // sub_82160948
inline constexpr auto& OnState23 = lifted::entry::e821609E0;  // sub_821609E0
inline constexpr auto& OnState34 = lifted::entry::e8215FC38;  // sub_8215FC38
inline constexpr auto& OnState7 = lifted::entry::e82160940;  // sub_82160940
}  // namespace game::RaceFlow

namespace game::RaceHud {
inline constexpr auto& GaugeFloat = lifted::f82214330;  // sub_82214330
inline constexpr auto& GaugeInt119 = lifted::f822144B0;  // sub_822144B0
inline constexpr auto& GaugeInt149 = lifted::f822145A0;  // sub_822145A0
inline constexpr auto& Init = lifted::entry::e82215260;  // sub_82215260
inline constexpr auto& RunMovie = lifted::entry::e822141E8;  // sub_822141E8
inline constexpr auto& SetBar = lifted::entry::e82214698;  // sub_82214698
inline constexpr auto& SetBoost = lifted::entry::e82212E18;  // sub_82212E18
inline constexpr auto& SetGear = lifted::entry::e82212DB0;  // sub_82212DB0
inline constexpr auto& SetSpeed = lifted::entry::e82214260;  // sub_82214260
inline constexpr auto& SetTach = lifted::entry::e82212D58;  // sub_82212D58
inline constexpr auto& SetUpRacers = lifted::entry::e82213F20;  // sub_82213F20
inline constexpr auto& SetUpTimer = lifted::entry::e822123C8;  // sub_822123C8
inline constexpr auto& ShowGauge = lifted::entry::e82212D08;  // sub_82212D08
inline constexpr auto& StopCountdown = lifted::f82212888;  // sub_82212888
inline constexpr auto& TickRacers = lifted::entry::e822151F8;  // sub_822151F8
inline constexpr auto& TickTimer = lifted::entry::e82214D78;  // sub_82214D78
inline constexpr auto& UpdateExtraGauges = lifted::entry::e82215160;  // sub_82215160
inline constexpr auto& UpdateRecordingFull = lifted::entry::e82212E60;  // sub_82212E60
inline constexpr auto& UpdateSigns = lifted::entry::e82214718;  // sub_82214718
}  // namespace game::RaceHud

namespace game::RaceHudMachine {
inline constexpr auto& AfterRaceDraw = lifted::entry::e8216C6D8;  // sub_8216C6D8
inline constexpr auto& AfterRaceUpdate = lifted::entry::e8216DD88;  // sub_8216DD88
inline constexpr auto& ClosePause = lifted::entry::e8216C658;  // sub_8216C658
inline constexpr auto& CloseResults = lifted::entry::e8216D198;  // sub_8216D198
inline constexpr auto& CountdownDraw = lifted::entry::e8216C330;  // sub_8216C330
inline constexpr auto& CountdownPauseDraw = lifted::f8216D0C8;  // sub_8216D0C8
inline constexpr auto& CountdownUpdate = lifted::entry::e8216CB98;  // sub_8216CB98
inline constexpr auto& CreateScreens = lifted::entry::e8216CA28;  // sub_8216CA28
inline constexpr auto& DestroyScreens = lifted::entry::e8216C208;  // sub_8216C208
inline constexpr auto& EndInCountdown = lifted::f8216E668;  // sub_8216E668
inline constexpr auto& EndIntro = lifted::entry::e8216C520;  // sub_8216C520
inline constexpr auto& EndMovie2 = lifted::f8216C500;  // sub_8216C500
inline constexpr auto& Init = lifted::entry::e8216E6E8;  // sub_8216E6E8
inline constexpr auto& IntroDraw = lifted::entry::e8216C5B8;  // sub_8216C5B8
inline constexpr auto& IntroUpdate = lifted::entry::e8216C530;  // sub_8216C530
inline constexpr auto& Movie2Draw = lifted::f8216C480;  // sub_8216C480
inline constexpr auto& Movie2Update = lifted::f8216C4C0;  // sub_8216C4C0
inline constexpr auto& OnMessage30 = lifted::f8216C648;  // sub_8216C648
inline constexpr auto& OnRaceEnd = lifted::entry::e8216E5B0;  // sub_8216E5B0
inline constexpr auto& OpenPause = lifted::entry::e8216D600;  // sub_8216D600
inline constexpr auto& OpenPauseByStage = lifted::f8216C798;  // sub_8216C798
inline constexpr auto& OpenRacePause = lifted::entry::e8216DA40;  // sub_8216DA40
inline constexpr auto& PauseDraw = lifted::entry::e8216CF08;  // sub_8216CF08
inline constexpr auto& PauseUpdate = lifted::entry::e8216DB70;  // sub_8216DB70
inline constexpr auto& PreRaceDraw = lifted::entry::e8216C2A0;  // sub_8216C2A0
inline constexpr auto& PreRaceUpdate = lifted::entry::e8216C210;  // sub_8216C210
inline constexpr auto& RaceDraw = lifted::entry::e8216CE00;  // sub_8216CE00
inline constexpr auto& RacePauseDraw = lifted::entry::e8216CFE0;  // sub_8216CFE0
inline constexpr auto& RaceUpdate = lifted::entry::e8216D918;  // sub_8216D918
inline constexpr auto& Restart = lifted::f8215E140;  // sub_8215E140
inline constexpr auto& ResultsDraw = lifted::entry::e8216D6D0;  // sub_8216D6D0
inline constexpr auto& ResultsUpdate = lifted::entry::e8216DFD0;  // sub_8216DFD0
inline constexpr auto& SetUp = lifted::entry::e8216D450;  // sub_8216D450
inline constexpr auto& ShowKind6Results = lifted::f8216D7B8;  // sub_8216D7B8
inline constexpr auto& ShowResults = lifted::f8216D270;  // sub_8216D270
inline constexpr auto& ShowSplitResults = lifted::f8216D328;  // sub_8216D328
inline constexpr auto& StartCountdown = lifted::entry::e8216CC60;  // sub_8216CC60
inline constexpr auto& StartIntro = lifted::entry::e8216C510;  // sub_8216C510
inline constexpr auto& StartMovie2 = lifted::f8216C470;  // sub_8216C470
inline constexpr auto& StopCountdown = lifted::f8216C3E8;  // sub_8216C3E8
inline constexpr auto& ToggleView = lifted::f8216CEB8;  // sub_8216CEB8
inline constexpr auto& ViewDraw = lifted::f8216C460;  // sub_8216C460
}  // namespace game::RaceHudMachine

namespace game::RaceLights {
inline constexpr auto& SetUp = lifted::entry::e8215CF78;  // sub_8215CF78
}  // namespace game::RaceLights

namespace game::RaceManager {
inline constexpr auto& BranchesMeet = lifted::entry::e82170860;  // sub_82170860
inline constexpr auto& BuildNeighbors = lifted::entry::e82170CB8;  // sub_82170CB8
inline constexpr auto& EntryOfRecord = lifted::entry::e821705A8;  // sub_821705A8
inline constexpr auto& GiveVehicle = lifted::entry::e821706E8;  // sub_821706E8
inline constexpr auto& ListRacers = lifted::entry::e82171E50;  // sub_82171E50
inline constexpr auto& Refresh = lifted::entry::e82171978;  // sub_82171978
inline constexpr auto& ResetVehicles = lifted::entry::e82171718;  // sub_82171718
inline constexpr auto& ResetWorld = lifted::entry::e82170668;  // sub_82170668
}  // namespace game::RaceManager

namespace game::RaceMenu {
inline constexpr auto& Create = lifted::entry::e82188E20;  // sub_82188E20
inline constexpr auto& Destroy = lifted::entry::e82188FE0;  // sub_82188FE0
inline constexpr auto& Draw = lifted::entry::e8218C578;  // sub_8218C578
inline constexpr auto& Init = lifted::entry::e8218CE10;  // sub_8218CE10
inline constexpr auto& LeaveToHub = lifted::entry::e8218AF80;  // sub_8218AF80
inline constexpr auto& OnBack = lifted::entry::e82188938;  // sub_82188938
inline constexpr auto& OnDialogClosed = lifted::entry::e821896A0;  // sub_821896A0
inline constexpr auto& OnNo = lifted::entry::e82189648;  // sub_82189648
inline constexpr auto& OnYes = lifted::entry::e821895F0;  // sub_821895F0
inline constexpr auto& OpenMovie = lifted::entry::e821889E0;  // sub_821889E0
inline constexpr auto& Start = lifted::entry::e8218C9C8;  // sub_8218C9C8
inline constexpr auto& Unload = lifted::entry::e8218AF08;  // sub_8218AF08
inline constexpr auto& Update = lifted::entry::e8218C678;  // sub_8218C678
inline constexpr auto& WaitForMovie = lifted::entry::e821890D8;  // sub_821890D8
}  // namespace game::RaceMenu

namespace game::RaceMode {
inline constexpr auto& DeletingDestructor = lifted::f82107590;  // sub_82107590
inline constexpr auto& Dtor = lifted::f82107518;  // sub_82107518
inline constexpr auto& Init = lifted::f82151D20;  // sub_82151D20
inline constexpr auto& NextMode = lifted::entry::e82151EF0;  // sub_82151EF0
}  // namespace game::RaceMode

namespace game::RaceModeMember {
inline constexpr auto& Ctor = lifted::entry::e82107168;  // sub_82107168
inline constexpr auto& DeletingDestructor = lifted::f82106B60;  // sub_82106B60
inline constexpr auto& Dtor = lifted::entry::e82106AF0;  // sub_82106AF0
}  // namespace game::RaceModeMember

namespace game::RaceOverlays {
inline constexpr auto& Bind = lifted::f8215E2E0;  // sub_8215E2E0
inline constexpr auto& Clear = lifted::entry::e8215DE80;  // sub_8215DE80
inline constexpr auto& Create = lifted::f8215E208;  // sub_8215E208
inline constexpr auto& DeleteAll = lifted::f8215DF80;  // sub_8215DF80
inline constexpr auto& Destroy = lifted::f8215E1D0;  // sub_8215E1D0
inline constexpr auto& FreeViews = lifted::f8215E160;  // sub_8215E160
inline constexpr auto& OnUpdate = lifted::f8215E150;  // sub_8215E150
inline constexpr auto& RaceMode = lifted::entry::e82126D18;  // sub_82126D18
inline constexpr auto& RecordAll = lifted::f8215DF20;  // sub_8215DF20
inline constexpr auto& Run = lifted::entry::e8215DEA0;  // sub_8215DEA0
inline constexpr auto& ShareTextures = lifted::entry::e8215E000;  // sub_8215E000
inline constexpr auto& SlotOf = lifted::entry::e8215E048;  // sub_8215E048
}  // namespace game::RaceOverlays

namespace game::RaceOverlaysMachine {
inline constexpr auto& ClearFlags = lifted::entry::e8215DD90;  // sub_8215DD90
inline constexpr auto& Init = lifted::entry::e8215E6C8;  // sub_8215E6C8
inline constexpr auto& ResetOverlays = lifted::entry::e8215DE10;  // sub_8215DE10
inline constexpr auto& SetFlag820 = lifted::entry::e8215DDD0;  // sub_8215DDD0
inline constexpr auto& SetFlags = lifted::entry::e8215DD58;  // sub_8215DD58
}  // namespace game::RaceOverlaysMachine

namespace game::RaceProgress {
inline constexpr auto& ReleaseRacers = lifted::entry::e82163440;  // sub_82163440
inline constexpr auto& Reset = lifted::entry::e821648E0;  // sub_821648E0
inline constexpr auto& SetChecks = lifted::entry::e821649B8;  // sub_821649B8
inline constexpr auto& StartRoute = lifted::entry::e821635D0;  // sub_821635D0
}  // namespace game::RaceProgress

namespace game::RaceReplay {
inline constexpr auto& RestoreEnd = lifted::entry::e82104898;  // sub_82104898
inline constexpr auto& RestoreStart = lifted::entry::e821046F8;  // sub_821046F8
inline constexpr auto& SaveEnd = lifted::entry::e82104E68;  // sub_82104E68
inline constexpr auto& SaveStart = lifted::entry::e82104D40;  // sub_82104D40
}  // namespace game::RaceReplay

namespace game::RaceResults {
inline constexpr auto& Draw = lifted::entry::e8221F7D0;  // sub_8221F7D0
inline constexpr auto& Open = lifted::entry::e8221F990;  // sub_8221F990
inline constexpr auto& Update = lifted::entry::e8221F930;  // sub_8221F930
}  // namespace game::RaceResults

namespace game::RaceScene {
inline constexpr auto& Ctor = lifted::entry::e8215D6C8;  // sub_8215D6C8
inline constexpr auto& Destroy = lifted::entry::e8215D790;  // sub_8215D790
inline constexpr auto& LoadScenery = lifted::entry::e8215B790;  // sub_8215B790
inline constexpr auto& RemakeCarModel = lifted::entry::e8215C0D8;  // sub_8215C0D8
inline constexpr auto& SetEffectParams = lifted::entry::e8228F5C0;  // sub_8228F5C0
inline constexpr auto& SetUp = lifted::f8215D100;  // sub_8215D100
inline constexpr auto& SetUpCars = lifted::f8215C248;  // sub_8215C248
inline constexpr auto& Teardown = lifted::f8215B720;  // sub_8215B720
inline constexpr auto& UnloadScenery = lifted::f8215AA88;  // sub_8215AA88
inline constexpr auto& Update = lifted::entry::e8215D2B0;  // sub_8215D2B0
}  // namespace game::RaceScene

namespace game::RaceSceneMachine {
inline constexpr auto& Init = lifted::entry::e8215D808;  // sub_8215D808
inline constexpr auto& OnDraw = lifted::f8215C770;  // sub_8215C770
inline constexpr auto& OnEnd = lifted::f8215D7D8;  // sub_8215D7D8
inline constexpr auto& OnSetUp = lifted::f8215D528;  // sub_8215D528
inline constexpr auto& OnStart = lifted::f8215D098;  // sub_8215D098
inline constexpr auto& OnUpdate = lifted::f8215D538;  // sub_8215D538
inline constexpr auto& Resume = lifted::entry::e8215D0D8;  // sub_8215D0D8
inline constexpr auto& TogglePause = lifted::entry::e8215C750;  // sub_8215C750
}  // namespace game::RaceSceneMachine

namespace game::RaceScreens {
inline constexpr auto& Make1004 = lifted::entry::e82216A60;  // sub_82216A60
inline constexpr auto& MovieMask = lifted::entry::e82211BE8;  // sub_82211BE8
inline constexpr auto& RunMovie18 = lifted::f8220B9E8;  // sub_8220B9E8
}  // namespace game::RaceScreens

namespace game::RaceSnapshot {
inline constexpr auto& Destroy = lifted::f82104630;  // sub_82104630
}  // namespace game::RaceSnapshot

namespace game::RaceSounds {
inline constexpr auto& AfterRaceTick = lifted::entry::e82176C10;  // sub_82176C10
inline constexpr auto& Begin = lifted::entry::e82176618;  // sub_82176618
inline constexpr auto& EndTick = lifted::entry::e82175A40;  // sub_82175A40
inline constexpr auto& IdleTick = lifted::entry::e821767A8;  // sub_821767A8
inline constexpr auto& Init = lifted::entry::e82177378;  // sub_82177378
inline constexpr auto& OnAbort = lifted::entry::e82175CA0;  // sub_82175CA0
inline constexpr auto& OnAbortDone = lifted::entry::e82175CF8;  // sub_82175CF8
inline constexpr auto& OnCountdown = lifted::entry::e82176968;  // sub_82176968
inline constexpr auto& OnEffectsBankLoaded = lifted::entry::e82175ED0;  // sub_82175ED0
inline constexpr auto& OnLeaveAfterRace = lifted::entry::e82176E60;  // sub_82176E60
inline constexpr auto& OnNextStage = lifted::entry::e82176EC8;  // sub_82176EC8
inline constexpr auto& OnRaceEnd = lifted::entry::e82176BA0;  // sub_82176BA0
inline constexpr auto& OnRollingStart = lifted::entry::e821770C0;  // sub_821770C0
inline constexpr auto& OnRollingStartDone = lifted::entry::e82175D48;  // sub_82175D48
inline constexpr auto& OnStageCountdown = lifted::entry::e82175D20;  // sub_82175D20
inline constexpr auto& Pause = lifted::entry::e82175AE8;  // sub_82175AE8
inline constexpr auto& PauseCars = lifted::entry::e82175C00;  // sub_82175C00
inline constexpr auto& RaceTick = lifted::entry::e821769D0;  // sub_821769D0
inline constexpr auto& Resume = lifted::entry::e82175B80;  // sub_82175B80
inline constexpr auto& ResumeCars = lifted::entry::e82175C58;  // sub_82175C58
inline constexpr auto& RollingStartTick = lifted::entry::e82177130;  // sub_82177130
inline constexpr auto& StageTick = lifted::entry::e82176F28;  // sub_82176F28
inline constexpr auto& Unload = lifted::entry::e821772D8;  // sub_821772D8
}  // namespace game::RaceSounds

namespace game::RaceStart {
inline constexpr auto& Place = lifted::entry::e82165E20;  // sub_82165E20
inline constexpr auto& PlaceRolling = lifted::entry::e821662E0;  // sub_821662E0
}  // namespace game::RaceStart

namespace game::RaceStates {
inline constexpr auto& AbortTick = lifted::f821665F0;  // sub_821665F0
inline constexpr auto& AfterRaceTick = lifted::entry::e82167970;  // sub_82167970
inline constexpr auto& AfterReplayTick = lifted::f82167A40;  // sub_82167A40
inline constexpr auto& Begin = lifted::entry::e82164DC0;  // sub_82164DC0
inline constexpr auto& CarsLoaded = lifted::f82163218;  // sub_82163218
inline constexpr auto& ClearRaceFlags = lifted::f82164740;  // sub_82164740
inline constexpr auto& ClearStep = lifted::entry::e82163348;  // sub_82163348
inline constexpr auto& CountdownTick = lifted::entry::e821674A0;  // sub_821674A0
inline constexpr auto& DemoTick = lifted::f82169030;  // sub_82169030
inline constexpr auto& EndWhenFaded = lifted::entry::e82168F50;  // sub_82168F50
inline constexpr auto& FadeInFree = lifted::entry::e82163398;  // sub_82163398
inline constexpr auto& FadeOut = lifted::entry::e82162310;  // sub_82162310
inline constexpr auto& FreeRunTick = lifted::f82169290;  // sub_82169290
inline constexpr auto& FreeStartTick = lifted::entry::e82168B80;  // sub_82168B80
inline constexpr auto& Init = lifted::entry::e82169920;  // sub_82169920
inline constexpr auto& LinkTick = lifted::f82166658;  // sub_82166658
inline constexpr auto& MenuLoadTick = lifted::f82167BE0;  // sub_82167BE0
inline constexpr auto& NewStage = lifted::f821634D8;  // sub_821634D8
inline constexpr auto& NextMessage = lifted::f82164B50;  // sub_82164B50
inline constexpr auto& OnContinue = lifted::f82168FB8;  // sub_82168FB8
inline constexpr auto& OnCountdown = lifted::f821643E0;  // sub_821643E0
inline constexpr auto& OnGo = lifted::entry::e82164458;  // sub_82164458
inline constexpr auto& OnQuit = lifted::entry::e82163378;  // sub_82163378
inline constexpr auto& OnQuit2 = lifted::f82163388;  // sub_82163388
inline constexpr auto& OnRaceEnd = lifted::entry::e821644D8;  // sub_821644D8
inline constexpr auto& OnReplay = lifted::entry::e821647B0;  // sub_821647B0
inline constexpr auto& OnReplayStart = lifted::f82164820;  // sub_82164820
inline constexpr auto& OnRollingStart = lifted::entry::e821632C8;  // sub_821632C8
inline constexpr auto& OnStageEnd = lifted::f82164700;  // sub_82164700
inline constexpr auto& OnStandingStart = lifted::entry::e821643B0;  // sub_821643B0
inline constexpr auto& OnWorldLoaded = lifted::f82168AA8;  // sub_82168AA8
inline constexpr auto& QuitFree = lifted::f821633C8;  // sub_821633C8
inline constexpr auto& QuitTick = lifted::entry::e82168500;  // sub_82168500
inline constexpr auto& RaceTick = lifted::entry::e82167610;  // sub_82167610
inline constexpr auto& ReplayLoadTick = lifted::f82168910;  // sub_82168910
inline constexpr auto& ReplaySetUpTick = lifted::f82168708;  // sub_82168708
inline constexpr auto& RollingLoadTick = lifted::f82166F98;  // sub_82166F98
inline constexpr auto& RollingStartTick = lifted::entry::e82166E50;  // sub_82166E50
inline constexpr auto& SetUpStage = lifted::f821654F8;  // sub_821654F8
inline constexpr auto& ShowLoading = lifted::f82163328;  // sub_82163328
inline constexpr auto& ShowLoadingStep0 = lifted::f82163358;  // sub_82163358
inline constexpr auto& StageLoadTick = lifted::f82166B40;  // sub_82166B40
inline constexpr auto& StandingLoadTick = lifted::f821672F0;  // sub_821672F0
inline constexpr auto& StandingStartTick = lifted::entry::e821692C8;  // sub_821692C8
inline constexpr auto& StartDemo = lifted::f821633F0;  // sub_821633F0
inline constexpr auto& Unload = lifted::entry::e82163920;  // sub_82163920
}  // namespace game::RaceStates

namespace game::RaceTime {
inline constexpr auto& FromTicks = lifted::entry::e82101760;  // sub_82101760
}  // namespace game::RaceTime

namespace game::RaceWorld {
inline constexpr auto& Abort = lifted::f82171308;  // sub_82171308
inline constexpr auto& Init = lifted::entry::e82174D28;  // sub_82174D28
inline constexpr auto& OnLoad = lifted::f82170550;  // sub_82170550
inline constexpr auto& OnLoaded = lifted::entry::e82173A90;  // sub_82173A90
inline constexpr auto& OnPausedTick = lifted::entry::e82170560;  // sub_82170560
inline constexpr auto& Release = lifted::entry::e82171360;  // sub_82171360
inline constexpr auto& ReleaseForStage = lifted::entry::e821713C0;  // sub_821713C0
inline constexpr auto& Tick = lifted::entry::e82174BC0;  // sub_82174BC0
inline constexpr auto& Unload = lifted::entry::e82171250;  // sub_82171250
}  // namespace game::RaceWorld

namespace game::RacerRecords {
inline constexpr auto& At = lifted::entry::e820FFE58;  // sub_820FFE58
}  // namespace game::RacerRecords

namespace game::Racers {
inline constexpr auto& CarsCollide = lifted::entry::e820FEFF8;  // sub_820FEFF8
inline constexpr auto& IndexOf = lifted::entry::e820FF9F8;  // sub_820FF9F8
inline constexpr auto& NearestHumanVehicle = lifted::entry::e820C2248;  // sub_820C2248
inline constexpr auto& Vehicle = lifted::entry::e820C9718;  // sub_820C9718
}  // namespace game::Racers

namespace game::Record5232 {
inline constexpr auto& Copy = lifted::entry::e82104BC0;  // sub_82104BC0
inline constexpr auto& RestoreTrack = lifted::entry::e82104310;  // sub_82104310
inline constexpr auto& Save = lifted::entry::e82104538;  // sub_82104538
}  // namespace game::Record5232

namespace game::Recorder {
inline constexpr auto& RecordObject = lifted::entry::e825953D0;  // sub_825953D0
inline constexpr auto& RecordSceneLights = lifted::entry::e82595490;  // sub_82595490
}  // namespace game::Recorder

namespace game::RenderQueue {
inline constexpr auto& Add = lifted::entry::e8258EDC8;  // sub_8258EDC8
inline constexpr auto& ClearRecording = lifted::entry::e8258ED78;  // sub_8258ED78
inline constexpr auto& Ctor = lifted::entry::e825915E0;  // sub_825915E0
inline constexpr auto& Dtor_82590DE0 = lifted::f82590DE0;  // sub_82590DE0
inline constexpr auto& Dtor_82590EA0 = lifted::f82590EA0;  // sub_82590EA0
inline constexpr auto& Flip = lifted::entry::e8258E5E0;  // sub_8258E5E0
inline constexpr auto& Init = lifted::entry::e8258ED00;  // sub_8258ED00
inline constexpr auto& Recording = lifted::entry::e8258E5F8;  // sub_8258E5F8
inline constexpr auto& Replay = lifted::entry::e82591238;  // sub_82591238
inline constexpr auto& Replaying = lifted::entry::e8258E618;  // sub_8258E618
}  // namespace game::RenderQueue

namespace game::RenderTexture {
inline constexpr auto& Create = lifted::entry::e821330D0;  // sub_821330D0
}  // namespace game::RenderTexture

namespace game::Replay {
inline constexpr auto& CopyFrom = lifted::entry::e821A66A8;  // sub_821A66A8
inline constexpr auto& Create = lifted::entry::e82105560;  // sub_82105560
inline constexpr auto& Destroy = lifted::f82104F20;  // sub_82104F20
inline constexpr auto& SetUpRace = lifted::f82105228;  // sub_82105228
}  // namespace game::Replay

namespace game::Replay826D8520 {
inline constexpr auto& StaticInit = lifted::entry::e825D9E60;  // sub_825D9E60
}  // namespace game::Replay826D8520

namespace game::Replay82816E50 {
inline constexpr auto& StaticInit = lifted::entry::e825D9F68;  // sub_825D9F68
}  // namespace game::Replay82816E50

namespace game::ReplayTheater {
inline constexpr auto& AskList = lifted::entry::e82289958;  // sub_82289958
inline constexpr auto& AskLoad = lifted::entry::e822899E0;  // sub_822899E0
inline constexpr auto& Create = lifted::entry::e82289408;  // sub_82289408
inline constexpr auto& Destroy = lifted::entry::e822894E8;  // sub_822894E8
inline constexpr auto& Draw = lifted::entry::e8228A868;  // sub_8228A868
inline constexpr auto& FadeInList = lifted::entry::e82289CC0;  // sub_82289CC0
inline constexpr auto& FadeOut = lifted::entry::e82289BD0;  // sub_82289BD0
inline constexpr auto& HideCursor = lifted::entry::e82289E08;  // sub_82289E08
inline constexpr auto& Intro = lifted::entry::e82289AC0;  // sub_82289AC0
inline constexpr auto& Leave = lifted::entry::e822896C0;  // sub_822896C0
inline constexpr auto& LeavePage = lifted::entry::e82289A70;  // sub_82289A70
inline constexpr auto& ListPage = lifted::entry::e82289768;  // sub_82289768
inline constexpr auto& MoveCursor = lifted::entry::e8228A658;  // sub_8228A658
inline constexpr auto& OnBack = lifted::entry::e82289308;  // sub_82289308
inline constexpr auto& OnDone = lifted::entry::e8228AAB0;  // sub_8228AAB0
inline constexpr auto& OnFailed = lifted::entry::e82289208;  // sub_82289208
inline constexpr auto& OnInterrupt = lifted::entry::e82289358;  // sub_82289358
inline constexpr auto& OnResume = lifted::entry::e822893A8;  // sub_822893A8
inline constexpr auto& OnRetry = lifted::entry::e8228ABE0;  // sub_8228ABE0
inline constexpr auto& OpenMovie = lifted::entry::e822891F8;  // sub_822891F8
inline constexpr auto& PickReplay = lifted::entry::e82289EB8;  // sub_82289EB8
inline constexpr auto& ShowCursor = lifted::entry::e82289D70;  // sub_82289D70
inline constexpr auto& Start = lifted::entry::e8228AA58;  // sub_8228AA58
inline constexpr auto& Unload = lifted::entry::e82289600;  // sub_82289600
inline constexpr auto& Update = lifted::entry::e8228A8F8;  // sub_8228A8F8
inline constexpr auto& Wait = lifted::entry::e82289668;  // sub_82289668
inline constexpr auto& WaitForMovie = lifted::entry::e82289570;  // sub_82289570
inline constexpr auto& WaitPage = lifted::entry::e8228A4B0;  // sub_8228A4B0
}  // namespace game::ReplayTheater

namespace game::Replays {
inline constexpr auto& BuildList = lifted::entry::e82290378;  // sub_82290378
inline constexpr auto& Load = lifted::f8228FF88;  // sub_8228FF88
}  // namespace game::Replays

namespace game::RequestQueue {
inline constexpr auto& Dtor_8209D888 = lifted::f8209D888;  // sub_8209D888
inline constexpr auto& Dtor_82294D38 = lifted::f82294D38;  // sub_82294D38
inline constexpr auto& StaticInit = lifted::entry::e825D9160;  // sub_825D9160
}  // namespace game::RequestQueue

namespace game::RequestSlot {
inline constexpr auto& Dtor = lifted::f8223A2F8;  // sub_8223A2F8
}  // namespace game::RequestSlot

namespace game::RequestSlots {
inline constexpr auto& StaticInit = lifted::entry::e825D91B0;  // sub_825D91B0
}  // namespace game::RequestSlots

namespace game::RequestVector {
inline constexpr auto& Clear = lifted::entry::e8209A080;  // sub_8209A080
inline constexpr auto& InsertN = lifted::entry::e8209A0E8;  // sub_8209A0E8
inline constexpr auto& PushBack = lifted::entry::e8209A930;  // sub_8209A930
}  // namespace game::RequestVector

namespace game::Resampler {
inline constexpr auto& Process = lifted::entry::e8246F140;  // sub_8246F140
}  // namespace game::Resampler

namespace game::Resource {
inline constexpr auto& AddRef = lifted::entry::e822A6738;  // sub_822A6738
inline constexpr auto& Destroy = lifted::entry::e822A6210;  // sub_822A6210
inline constexpr auto& GetType = lifted::entry::e822A6180;  // sub_822A6180
inline constexpr auto& Release = lifted::entry::e822A67B0;  // sub_822A67B0
}  // namespace game::Resource

namespace game::Results {
inline constexpr auto& AllWord40Are = lifted::f82110750;  // sub_82110750
inline constexpr auto& AnyRepeated = lifted::f82110798;  // sub_82110798
inline constexpr auto& MeanFloat8 = lifted::f82110678;  // sub_82110678
inline constexpr auto& MeanWord0 = lifted::f821105A8;  // sub_821105A8
inline constexpr auto& MeanWord4 = lifted::f82110610;  // sub_82110610
inline constexpr auto& WonPercent = lifted::f821106D0;  // sub_821106D0
}  // namespace game::Results

namespace game::Reverb {
inline constexpr auto& Create = lifted::entry::e82465710;  // sub_82465710
inline constexpr auto& Process = lifted::entry::e82465428;  // sub_82465428
inline constexpr auto& SetParameters = lifted::entry::e82462148;  // sub_82462148
inline constexpr auto& Size = lifted::entry::e82463B78;  // sub_82463B78
}  // namespace game::Reverb

namespace game::RigidBody {
inline constexpr auto& AddForce = lifted::entry::e820B6958;  // sub_820B6958
inline constexpr auto& AddForceAt = lifted::entry::e820B6968;  // sub_820B6968
inline constexpr auto& AddTorque = lifted::entry::e820B6960;  // sub_820B6960
inline constexpr auto& AddTorqueAt = lifted::entry::e820B69D8;  // sub_820B69D8
inline constexpr auto& AngularVelocity = lifted::entry::e820B6948;  // sub_820B6948
inline constexpr auto& ContactImpulseWith = lifted::entry::e820B72C8;  // sub_820B72C8
inline constexpr auto& EffectiveMassAt = lifted::entry::e820B6A38;  // sub_820B6A38
inline constexpr auto& Force = lifted::entry::e820988C0;  // sub_820988C0
inline constexpr auto& Integrate = lifted::entry::e820B7658;  // sub_820B7658
inline constexpr auto& IsFixed = lifted::entry::e820B6638;  // sub_820B6638
inline constexpr auto& Mass = lifted::entry::e820B6708;  // sub_820B6708
inline constexpr auto& Matrix = lifted::entry::e820B6930;  // sub_820B6930
inline constexpr auto& PointVelocity = lifted::entry::e820B68C0;  // sub_820B68C0
inline constexpr auto& Position = lifted::entry::e820B67D8;  // sub_820B67D8
inline constexpr auto& SetAngularVelocity = lifted::entry::e820B7500;  // sub_820B7500
inline constexpr auto& SetCenterOfMassPosition = lifted::entry::e820B71B0;  // sub_820B71B0
inline constexpr auto& SetOrientation = lifted::entry::e820B71E8;  // sub_820B71E8
inline constexpr auto& SetPosition = lifted::entry::e820B6710;  // sub_820B6710
inline constexpr auto& SetVelocity = lifted::entry::e820B7238;  // sub_820B7238
inline constexpr auto& UpdateAngularVelocity = lifted::entry::e820B7478;  // sub_820B7478
inline constexpr auto& UpdateOrientation = lifted::entry::e820B6F38;  // sub_820B6F38
inline constexpr auto& UpdateOrigin = lifted::entry::e820B6E70;  // sub_820B6E70
inline constexpr auto& UpdateVelocity = lifted::entry::e820B70C8;  // sub_820B70C8
}  // namespace game::RigidBody

namespace game::RivalLogo {
inline constexpr auto& Draw = lifted::entry::e82211A28;  // sub_82211A28
inline constexpr auto& InitXpr = lifted::entry::e82211708;  // sub_82211708
inline constexpr auto& LoadXpr = lifted::f82211878;  // sub_82211878
inline constexpr auto& ReleaseXpr = lifted::f82211768;  // sub_82211768
}  // namespace game::RivalLogo

namespace game::RoutePosition {
inline constexpr auto& Step = lifted::entry::e82170F90;  // sub_82170F90
inline constexpr auto& StepToMainRoad = lifted::entry::e82170F18;  // sub_82170F18
}  // namespace game::RoutePosition

namespace game::RouteRange {
inline constexpr auto& AddBranch = lifted::entry::e820FE8C8;  // sub_820FE8C8
inline constexpr auto& Init = lifted::entry::e820FE058;  // sub_820FE058
inline constexpr auto& Test = lifted::entry::e820FE6F8;  // sub_820FE6F8
inline constexpr auto& Update = lifted::entry::e820FE878;  // sub_820FE878
}  // namespace game::RouteRange

namespace game::RouteSections {
inline constexpr auto& Before = lifted::entry::e82162E70;  // sub_82162E70
}  // namespace game::RouteSections

namespace game::RowPanel {
inline constexpr auto& Fill = lifted::entry::e8224F758;  // sub_8224F758
inline constexpr auto& Refresh = lifted::entry::e8224F888;  // sub_8224F888
inline constexpr auto& ShowFirst = lifted::f8224F158;  // sub_8224F158
}  // namespace game::RowPanel

namespace game::RunLoop {
inline constexpr auto& AddListener = lifted::entry::e8209AB48;  // sub_8209AB48
inline constexpr auto& AddTickHandler = lifted::entry::e8209AAA8;  // sub_8209AAA8
inline constexpr auto& ApplyRequest = lifted::entry::e8209A458;  // sub_8209A458
}  // namespace game::RunLoop

namespace game::RunningMean {
inline constexpr auto& Add = lifted::entry::e82101698;  // sub_82101698
}  // namespace game::RunningMean

namespace game::SaveBuffers {
inline constexpr auto& Create = lifted::entry::e8228F6B0;  // sub_8228F6B0
inline constexpr auto& Destroy = lifted::entry::e8228F8F8;  // sub_8228F8F8
}  // namespace game::SaveBuffers

namespace game::SaveData {
inline constexpr auto& Create = lifted::entry::e821057C8;  // sub_821057C8
inline constexpr auto& Delete = lifted::entry::e821055D8;  // sub_821055D8
inline constexpr auto& FirstUnsetFlag = lifted::entry::e82112190;  // sub_82112190
inline constexpr auto& Of = lifted::entry::e82105620;  // sub_82105620
inline constexpr auto& TwoPlayerBlock = lifted::entry::e82105678;  // sub_82105678
}  // namespace game::SaveData

namespace game::SaveDevices {
inline constexpr auto& Create = lifted::entry::e820A63C8;  // sub_820A63C8
inline constexpr auto& Dtor = lifted::entry::e820A6110;  // sub_820A6110
inline constexpr auto& OnEvent = lifted::entry::e820A4800;  // sub_820A4800
inline constexpr auto& SetRootName = lifted::entry::e820A5580;  // sub_820A5580
inline constexpr auto& Update = lifted::entry::e820A4610;  // sub_820A4610
}  // namespace game::SaveDevices

namespace game::SaveInterface {
inline constexpr auto& Clear = lifted::f820A0008;  // sub_820A0008
inline constexpr auto& Init = lifted::entry::e8209FFB8;  // sub_8209FFB8
inline constexpr auto& Request980 = lifted::f820A01B0;  // sub_820A01B0
inline constexpr auto& Word84 = lifted::f820A0260;  // sub_820A0260
inline constexpr auto& Word992 = lifted::f820A00F8;  // sub_820A00F8
}  // namespace game::SaveInterface

namespace game::SavePage {
inline constexpr auto& Answer = lifted::entry::e821BCA60;  // sub_821BCA60
inline constexpr auto& CheckDevice = lifted::entry::e821BBCC0;  // sub_821BBCC0
inline constexpr auto& Close = lifted::entry::e821BA508;  // sub_821BA508
inline constexpr auto& Leave = lifted::entry::e821C6550;  // sub_821C6550
inline constexpr auto& Note = lifted::entry::e821BCC10;  // sub_821BCC10
inline constexpr auto& Wait = lifted::entry::e821BBD18;  // sub_821BBD18
}  // namespace game::SavePage

namespace game::SaveScreen {
inline constexpr auto& Cancel = lifted::entry::e821A1D20;  // sub_821A1D20
inline constexpr auto& CloseDialog = lifted::entry::e821A5038;  // sub_821A5038
inline constexpr auto& Draw = lifted::entry::e821A6AF0;  // sub_821A6AF0
inline constexpr auto& Leave = lifted::entry::e821A4C58;  // sub_821A4C58
inline constexpr auto& MoveSelection = lifted::entry::e821A5118;  // sub_821A5118
inline constexpr auto& NextPlayer = lifted::f821A2350;  // sub_821A2350
inline constexpr auto& OnEnter = lifted::entry::e821A2608;  // sub_821A2608
inline constexpr auto& OnLeft = lifted::entry::e821A1CA0;  // sub_821A1CA0
inline constexpr auto& OpenKind0 = lifted::entry::e821A2760;  // sub_821A2760
inline constexpr auto& OpenKind1 = lifted::entry::e821A2790;  // sub_821A2790
inline constexpr auto& OpenKind2 = lifted::entry::e821A2820;  // sub_821A2820
inline constexpr auto& OpenKind3 = lifted::entry::e821A27F0;  // sub_821A27F0
inline constexpr auto& OpenKind4 = lifted::entry::e821A2850;  // sub_821A2850
inline constexpr auto& OpenKind5 = lifted::entry::e821A2880;  // sub_821A2880
inline constexpr auto& OpenKind6 = lifted::entry::e821A27C0;  // sub_821A27C0
inline constexpr auto& OpenKind7 = lifted::entry::e821A28B0;  // sub_821A28B0
inline constexpr auto& OpenMovie = lifted::entry::e821A1D10;  // sub_821A1D10
inline constexpr auto& Page0 = lifted::entry::e821A28E0;  // sub_821A28E0
inline constexpr auto& Page1 = lifted::entry::e821A2AF8;  // sub_821A2AF8
inline constexpr auto& Page10 = lifted::f821A3868;  // sub_821A3868
inline constexpr auto& Page11 = lifted::f821A3950;  // sub_821A3950
inline constexpr auto& Page12 = lifted::entry::e821A3A08;  // sub_821A3A08
inline constexpr auto& Page13 = lifted::entry::e821A3C60;  // sub_821A3C60
inline constexpr auto& Page14 = lifted::entry::e821A67E8;  // sub_821A67E8
inline constexpr auto& Page15 = lifted::entry::e821A3DE8;  // sub_821A3DE8
inline constexpr auto& Page16 = lifted::entry::e821A4E60;  // sub_821A4E60
inline constexpr auto& Page17 = lifted::f821A3ED8;  // sub_821A3ED8
inline constexpr auto& Page18 = lifted::f821A4228;  // sub_821A4228
inline constexpr auto& Page19 = lifted::f821A4450;  // sub_821A4450
inline constexpr auto& Page2 = lifted::entry::e821A2C98;  // sub_821A2C98
inline constexpr auto& Page20 = lifted::f821A4810;  // sub_821A4810
inline constexpr auto& Page21 = lifted::f821A4EF8;  // sub_821A4EF8
inline constexpr auto& Page22 = lifted::f821A2208;  // sub_821A2208
inline constexpr auto& Page23 = lifted::f821A4970;  // sub_821A4970
inline constexpr auto& Page24 = lifted::entry::e821A4AC0;  // sub_821A4AC0
inline constexpr auto& Page25 = lifted::entry::e821A4FD8;  // sub_821A4FD8
inline constexpr auto& Page26 = lifted::f821A2338;  // sub_821A2338
inline constexpr auto& Page3 = lifted::entry::e821A2EE8;  // sub_821A2EE8
inline constexpr auto& Page4 = lifted::entry::e821A4D30;  // sub_821A4D30
inline constexpr auto& Page5 = lifted::f821A2FB0;  // sub_821A2FB0
inline constexpr auto& Page6 = lifted::entry::e821A32C0;  // sub_821A32C0
inline constexpr auto& Page7 = lifted::entry::e821A33E8;  // sub_821A33E8
inline constexpr auto& Page8 = lifted::entry::e821A3728;  // sub_821A3728
inline constexpr auto& Page9 = lifted::entry::e821A4DE0;  // sub_821A4DE0
inline constexpr auto& Show = lifted::entry::e821A6608;  // sub_821A6608
inline constexpr auto& ShowKind3 = lifted::f821A1E18;  // sub_821A1E18
inline constexpr auto& ShowKind4 = lifted::f821A1ED8;  // sub_821A1ED8
inline constexpr auto& ShowKind5 = lifted::f821A1FA8;  // sub_821A1FA8
inline constexpr auto& ShowKind6 = lifted::f821A2070;  // sub_821A2070
inline constexpr auto& ShowMessage = lifted::entry::e821A5668;  // sub_821A5668
inline constexpr auto& ShowMessage3 = lifted::entry::e821A5980;  // sub_821A5980
inline constexpr auto& ShowMessage4 = lifted::entry::e821A5D00;  // sub_821A5D00
inline constexpr auto& ShowMessage5 = lifted::f821A6030;  // sub_821A6030
inline constexpr auto& ShowNotice = lifted::entry::e821A63B0;  // sub_821A63B0
inline constexpr auto& Update = lifted::entry::e821A64A0;  // sub_821A64A0
inline constexpr auto& WaitMovie = lifted::entry::e821A26F0;  // sub_821A26F0
}  // namespace game::SaveScreen

namespace game::SaveSlot {
inline constexpr auto& CheckDevice = lifted::entry::e820A45C0;  // sub_820A45C0
inline constexpr auto& CheckStep = lifted::entry::e820A5A88;  // sub_820A5A88
inline constexpr auto& Create = lifted::entry::e820A4C58;  // sub_820A4C58
inline constexpr auto& Delete = lifted::entry::e820A51E0;  // sub_820A51E0
inline constexpr auto& DeleteStep = lifted::entry::e820A6010;  // sub_820A6010
inline constexpr auto& Device = lifted::entry::e820A5768;  // sub_820A5768
inline constexpr auto& Enumerate = lifted::entry::e820A49D0;  // sub_820A49D0
inline constexpr auto& EnumerateStep = lifted::entry::e820A59C0;  // sub_820A59C0
inline constexpr auto& Failed = lifted::entry::e820A4890;  // sub_820A4890
inline constexpr auto& FreeBytes = lifted::entry::e820A5780;  // sub_820A5780
inline constexpr auto& LoadStep = lifted::entry::e820A5E68;  // sub_820A5E68
inline constexpr auto& OpenItem = lifted::entry::e820A4B18;  // sub_820A4B18
inline constexpr auto& Pending = lifted::entry::e820A5808;  // sub_820A5808
inline constexpr auto& ReadFile = lifted::entry::e820A4EF8;  // sub_820A4EF8
inline constexpr auto& Ready = lifted::entry::e820A58D8;  // sub_820A58D8
inline constexpr auto& RequestDelete = lifted::f820A54E8;  // sub_820A54E8
inline constexpr auto& RequestDevice = lifted::entry::e820A5750;  // sub_820A5750
inline constexpr auto& RequestLoad = lifted::entry::e820A53E8;  // sub_820A53E8
inline constexpr auto& RequestSave = lifted::entry::e820A52E0;  // sub_820A52E0
inline constexpr auto& SaveAt = lifted::entry::e820A57B0;  // sub_820A57B0
inline constexpr auto& SaveCount = lifted::entry::e820A5798;  // sub_820A5798
inline constexpr auto& SaveStep = lifted::entry::e820A5BE8;  // sub_820A5BE8
inline constexpr auto& SetThumbnail = lifted::entry::e820A5110;  // sub_820A5110
inline constexpr auto& ShowDeviceSelector = lifted::entry::e820A4960;  // sub_820A4960
inline constexpr auto& State = lifted::entry::e820A57F0;  // sub_820A57F0
inline constexpr auto& WaitDeviceSelector = lifted::entry::e820A48B8;  // sub_820A48B8
inline constexpr auto& WaitEnumerate = lifted::entry::e820A4A88;  // sub_820A4A88
inline constexpr auto& WaitForDevice = lifted::entry::e820A5878;  // sub_820A5878
inline constexpr auto& WaitForUser = lifted::entry::e820A4870;  // sub_820A4870
inline constexpr auto& WaitOpenItem = lifted::entry::e820A4B98;  // sub_820A4B98
inline constexpr auto& WriteFile = lifted::entry::e820A4D68;  // sub_820A4D68
}  // namespace game::SaveSlot

namespace game::SaveSlots {
inline constexpr auto& Device = lifted::entry::e8228FA90;  // sub_8228FA90
inline constexpr auto& Pending = lifted::entry::e8228FAC8;  // sub_8228FAC8
inline constexpr auto& RequestDevice = lifted::entry::e8228F9B0;  // sub_8228F9B0
inline constexpr auto& State = lifted::entry::e8228F9E8;  // sub_8228F9E8
}  // namespace game::SaveSlots

namespace game::Scene {
inline constexpr auto& Create = lifted::f82098C70;  // sub_82098C70
inline constexpr auto& Ctor = lifted::entry::e82098A98;  // sub_82098A98
inline constexpr auto& Draw = lifted::entry::e82098940;  // sub_82098940
inline constexpr auto& Dtor_82098860 = lifted::f82098860;  // sub_82098860
inline constexpr auto& Dtor_82098A48 = lifted::f82098A48;  // sub_82098A48
inline constexpr auto& OnEvent5 = lifted::f820989A8;  // sub_820989A8
inline constexpr auto& Update = lifted::entry::e820988C8;  // sub_820988C8
}  // namespace game::Scene

namespace game::SceneCarModel {
inline constexpr auto& Ctor = lifted::entry::e820DB770;  // sub_820DB770
inline constexpr auto& DeletingDestructor = lifted::entry::e820DB7C8;  // sub_820DB7C8
}  // namespace game::SceneCarModel

namespace game::SceneCockpit {
inline constexpr auto& Ctor = lifted::entry::e820DC3E8;  // sub_820DC3E8
inline constexpr auto& DeletingDestructor = lifted::entry::e820DC448;  // sub_820DC448
inline constexpr auto& Draw = lifted::entry::e820DC498;  // sub_820DC498
inline constexpr auto& Finish = lifted::entry::e820DBFF8;  // sub_820DBFF8
}  // namespace game::SceneCockpit

namespace game::SceneInterior {
inline constexpr auto& Ctor = lifted::entry::e820E5340;  // sub_820E5340
inline constexpr auto& DeletingDestructor = lifted::entry::e820E53A0;  // sub_820E53A0
inline constexpr auto& Draw = lifted::entry::e820E53F0;  // sub_820E53F0
inline constexpr auto& Finish = lifted::entry::e820E4140;  // sub_820E4140
inline constexpr auto& Link = lifted::entry::e820E37D0;  // sub_820E37D0
inline constexpr auto& SetGauges = lifted::entry::e820E3938;  // sub_820E3938
}  // namespace game::SceneInterior

namespace game::SceneLights {
inline constexpr auto& ApplyRecord = lifted::entry::e820DCBC0;  // sub_820DCBC0
inline constexpr auto& Ctor = lifted::entry::e820DE030;  // sub_820DE030
inline constexpr auto& DeletingDestructor = lifted::entry::e820DE090;  // sub_820DE090
inline constexpr auto& Draw = lifted::entry::e820DDE58;  // sub_820DDE58
inline constexpr auto& Finish = lifted::entry::e820DDA60;  // sub_820DDA60
}  // namespace game::SceneLights

namespace game::SceneModel {
inline constexpr auto& AddSecondWheels = lifted::entry::e820D9E00;  // sub_820D9E00
inline constexpr auto& ApplyPalettes = lifted::entry::e820D9EA0;  // sub_820D9EA0
inline constexpr auto& Beam = lifted::entry::e820DAAC8;  // sub_820DAAC8
inline constexpr auto& BeamColor = lifted::entry::e820DAA38;  // sub_820DAA38
inline constexpr auto& BeamShape = lifted::entry::e820DAB10;  // sub_820DAB10
inline constexpr auto& Ctor = lifted::entry::e820DB330;  // sub_820DB330
inline constexpr auto& IsNearPlayerHeight = lifted::entry::e820DAB58;  // sub_820DAB58
inline constexpr auto& PollLoad = lifted::entry::e820DA318;  // sub_820DA318
}  // namespace game::SceneModel

namespace game::SceneModelBlock {
inline constexpr auto& Ctor = lifted::entry::e820DA108;  // sub_820DA108
}  // namespace game::SceneModelBlock

namespace game::SceneModelRecord {
inline constexpr auto& Copy = lifted::entry::e820DB290;  // sub_820DB290
inline constexpr auto& Ctor = lifted::entry::e820DB240;  // sub_820DB240
inline constexpr auto& FromCar = lifted::entry::e8215B878;  // sub_8215B878
inline constexpr auto& Reset = lifted::entry::e820D9810;  // sub_820D9810
}  // namespace game::SceneModelRecord

namespace game::SceneParts {
inline constexpr auto& Build = lifted::entry::e820E05E0;  // sub_820E05E0
inline constexpr auto& Ctor = lifted::entry::e820E2F18;  // sub_820E2F18
inline constexpr auto& DeletingDestructor = lifted::entry::e820E2F78;  // sub_820E2F78
inline constexpr auto& Draw = lifted::entry::e820E2EE8;  // sub_820E2EE8
inline constexpr auto& Finish = lifted::entry::e820E2E68;  // sub_820E2E68
inline constexpr auto& Link = lifted::entry::e820DF1A8;  // sub_820DF1A8
}  // namespace game::SceneParts

namespace game::ScenePass {
inline constexpr auto& SetLight = lifted::entry::e820C4EB8;  // sub_820C4EB8
}  // namespace game::ScenePass

namespace game::ScenePools {
inline constexpr auto& ClaimB = lifted::entry::e820C5AB0;  // sub_820C5AB0
inline constexpr auto& CreateA = lifted::entry::e820C3FD8;  // sub_820C3FD8
inline constexpr auto& FindModel = lifted::entry::e820C5D70;  // sub_820C5D70
inline constexpr auto& MakeA = lifted::entry::e820C3C40;  // sub_820C3C40
inline constexpr auto& ReleaseA = lifted::entry::e820C4488;  // sub_820C4488
inline constexpr auto& ReleaseB = lifted::entry::e820C5C88;  // sub_820C5C88
inline constexpr auto& SetUpB = lifted::entry::e820C5B28;  // sub_820C5B28
}  // namespace game::ScenePools

namespace game::SceneSetUp {
inline constexpr auto& Ctor = lifted::entry::e820D0D50;  // sub_820D0D50
}  // namespace game::SceneSetUp

namespace game::SceneTarget {
inline constexpr auto& Create = lifted::entry::e8209B378;  // sub_8209B378
}  // namespace game::SceneTarget

namespace game::SceneTraffic {
inline constexpr auto& Ctor = lifted::entry::e820E6E70;  // sub_820E6E70
inline constexpr auto& DeletingDestructor = lifted::entry::e820E6ED8;  // sub_820E6ED8
inline constexpr auto& Draw = lifted::entry::e820E6A40;  // sub_820E6A40
inline constexpr auto& Finish = lifted::entry::e820E5A68;  // sub_820E5A68
}  // namespace game::SceneTraffic

namespace game::SceneWheels {
inline constexpr auto& Ctor = lifted::entry::e820ECE40;  // sub_820ECE40
inline constexpr auto& DeletingDestructor = lifted::entry::e820ECEB8;  // sub_820ECEB8
inline constexpr auto& Draw = lifted::entry::e820ECE28;  // sub_820ECE28
inline constexpr auto& Finish = lifted::entry::e820E7328;  // sub_820E7328
inline constexpr auto& SetUpPoints = lifted::entry::e820E8908;  // sub_820E8908
}  // namespace game::SceneWheels

namespace game::Scenery {
inline constexpr auto& Create = lifted::entry::e820F2F88;  // sub_820F2F88
inline constexpr auto& Destroy = lifted::entry::e820F2AF8;  // sub_820F2AF8
inline constexpr auto& Draw = lifted::entry::e820F6068;  // sub_820F6068
inline constexpr auto& MakePoolObject = lifted::entry::e820F2638;  // sub_820F2638
inline constexpr auto& PoolSizes = lifted::entry::e820F2030;  // sub_820F2030
inline constexpr auto& Setup = lifted::entry::e820F3020;  // sub_820F3020
inline constexpr auto& StartLoad = lifted::entry::e820F1E00;  // sub_820F1E00
inline constexpr auto& Unload = lifted::entry::e820F2B38;  // sub_820F2B38
inline constexpr auto& Update = lifted::entry::e820F4558;  // sub_820F4558
}  // namespace game::Scenery

namespace game::SceneryConfig {
inline constexpr auto& ModelAsset = lifted::entry::e820F1530;  // sub_820F1530
}  // namespace game::SceneryConfig

namespace game::SceneryList {
inline constexpr auto& LoadAll = lifted::entry::e820F1598;  // sub_820F1598
}  // namespace game::SceneryList

namespace game::SceneryModel {
inline constexpr auto& RecordShape = lifted::entry::e82146BD0;  // sub_82146BD0
}  // namespace game::SceneryModel

namespace game::ScopedLock {
inline constexpr auto& Ctor = lifted::entry::e8258E580;  // sub_8258E580
inline constexpr auto& Dtor = lifted::entry::e8258E5B8;  // sub_8258E5B8
}  // namespace game::ScopedLock

namespace game::Screen {
inline constexpr auto& LeaveToHub = lifted::entry::e821D6A20;  // sub_821D6A20
inline constexpr auto& Reenter = lifted::entry::e8223B740;  // sub_8223B740
inline constexpr auto& Wait = lifted::entry::e822508F0;  // sub_822508F0
}  // namespace game::Screen

namespace game::ScreenFades {
inline constexpr auto& FadeIn = lifted::entry::e8209D038;  // sub_8209D038
inline constexpr auto& FadeOut = lifted::entry::e8209D0C8;  // sub_8209D0C8
inline constexpr auto& Init = lifted::entry::e8209CDE8;  // sub_8209CDE8
inline constexpr auto& Update = lifted::entry::e8209D130;  // sub_8209D130
}  // namespace game::ScreenFades

namespace game::ScreenMachine {
inline constexpr auto& Ctor = lifted::entry::e82107358;  // sub_82107358
inline constexpr auto& Reenter = lifted::entry::e822895E0;  // sub_822895E0
inline constexpr auto& Wait = lifted::entry::e82277068;  // sub_82277068
}  // namespace game::ScreenMachine

namespace game::ScreenObject {
inline constexpr auto& BuildFan = lifted::f8212F148;  // sub_8212F148
inline constexpr auto& BuildRings = lifted::entry::e821329B8;  // sub_821329B8
inline constexpr auto& Ctor = lifted::f82133000;  // sub_82133000
inline constexpr auto& Dtor = lifted::entry::e82132FC8;  // sub_82132FC8
inline constexpr auto& Init = lifted::f82132CE0;  // sub_82132CE0
inline constexpr auto& Record = lifted::entry::e82130978;  // sub_82130978
inline constexpr auto& Start = lifted::f8212EDF8;  // sub_8212EDF8
inline constexpr auto& Stop = lifted::f8212EED0;  // sub_8212EED0
}  // namespace game::ScreenObject

namespace game::ScreenPages {
inline constexpr auto& Finish = lifted::entry::e8220BA28;  // sub_8220BA28
inline constexpr auto& Go = lifted::entry::e8220BA68;  // sub_8220BA68
inline constexpr auto& Init = lifted::entry::e8220BA98;  // sub_8220BA98
}  // namespace game::ScreenPages

namespace game::ScreenRect {
inline constexpr auto& Draw = lifted::entry::e82129280;  // sub_82129280
inline constexpr auto& Record = lifted::entry::e82128E70;  // sub_82128E70
}  // namespace game::ScreenRect

namespace game::Semaphore {
inline constexpr auto& Dtor = lifted::f82592638;  // sub_82592638
inline constexpr auto& Init = lifted::f825925F0;  // sub_825925F0
inline constexpr auto& Signal = lifted::entry::e825926E8;  // sub_825926E8
inline constexpr auto& Wait = lifted::entry::e82592670;  // sub_82592670
}  // namespace game::Semaphore

namespace game::Session {
inline constexpr auto& IsActive = lifted::f82260908;  // sub_82260908
inline constexpr auto& Release = lifted::entry::e82291670;  // sub_82291670
}  // namespace game::Session

namespace game::Settings {
inline constexpr auto& Reload = lifted::entry::e821C0FF0;  // sub_821C0FF0
}  // namespace game::Settings

namespace game::Shaders {
inline constexpr auto& LoadAll = lifted::entry::e820A08A0;  // sub_820A08A0
inline constexpr auto& OnPairLoaded = lifted::entry::e820A06D0;  // sub_820A06D0
}  // namespace game::Shaders

namespace game::Shape {
inline constexpr auto& Ctor = lifted::entry::e820BB020;  // sub_820BB020
inline constexpr auto& DeletingDestructor = lifted::entry::e820BB140;  // sub_820BB140
inline constexpr auto& Radius = lifted::entry::e820BAB40;  // sub_820BAB40
}  // namespace game::Shape

namespace game::SignInWatch {
inline constexpr auto& OnModeChange = lifted::f8209E7E8;  // sub_8209E7E8
inline constexpr auto& StaticInit = lifted::entry::e825D9518;  // sub_825D9518
inline constexpr auto& Update = lifted::entry::e8209EEC8;  // sub_8209EEC8
}  // namespace game::SignInWatch

namespace game::Single {
inline constexpr auto& Callback = lifted::entry::e8258E9B8;  // sub_8258E9B8
inline constexpr auto& Clear = lifted::f8258FAC8;  // sub_8258FAC8
inline constexpr auto& DrawIndexedPrimitiveUP = lifted::f8258FB20;  // sub_8258FB20
inline constexpr auto& DrawIndexedVertices = lifted::f8258F7C8;  // sub_8258F7C8
inline constexpr auto& DrawPrimitiveUP = lifted::f8258F828;  // sub_8258F828
inline constexpr auto& DrawVertices = lifted::f8258F770;  // sub_8258F770
inline constexpr auto& DrawerDraw = lifted::entry::e8258F5C0;  // sub_8258F5C0
inline constexpr auto& DrawerDrawIndexed = lifted::f8258F6A0;  // sub_8258F6A0
inline constexpr auto& InlineIndices = lifted::f8258E918;  // sub_8258E918
inline constexpr auto& InlineVertices = lifted::entry::e8258E8E8;  // sub_8258E8E8
inline constexpr auto& Release = lifted::f825916F0;  // sub_825916F0
inline constexpr auto& ReleaseResources = lifted::f8258F898;  // sub_8258F898
inline constexpr auto& Resolve = lifted::f8258F9A0;  // sub_8258F9A0
inline constexpr auto& SetBlendControls = lifted::f8258F028;  // sub_8258F028
inline constexpr auto& SetDepthStencilSurfaces = lifted::f8258F948;  // sub_8258F948
inline constexpr auto& SetIndices = lifted::f8258F568;  // sub_8258F568
inline constexpr auto& SetPixelShaders = lifted::f8258F438;  // sub_8258F438
inline constexpr auto& SetRenderStates = lifted::f8258EF60;  // sub_8258EF60
inline constexpr auto& SetRenderTargets = lifted::f8258F8F0;  // sub_8258F8F0
inline constexpr auto& SetSamplerStates = lifted::f8258EFC0;  // sub_8258EFC0
inline constexpr auto& SetShaderConstants = lifted::entry::e8258F0F0;  // sub_8258F0F0
inline constexpr auto& SetStreamSources = lifted::f8258F4E8;  // sub_8258F4E8
inline constexpr auto& SetTextures = lifted::f8258F080;  // sub_8258F080
inline constexpr auto& SetUpDrawer = lifted::entry::e8258E958;  // sub_8258E958
inline constexpr auto& SetVertexDeclarations = lifted::f8258F490;  // sub_8258F490
inline constexpr auto& SetVertexShaders = lifted::f8258F3E0;  // sub_8258F3E0
}  // namespace game::Single

namespace game::SingleGroup {
inline constexpr auto& Ctor = lifted::entry::e82249CB0;  // sub_82249CB0
inline constexpr auto& Reset = lifted::entry::e82249CC8;  // sub_82249CC8
}  // namespace game::SingleGroup

namespace game::Skeleton {
inline constexpr auto& ApplyAnimation = lifted::entry::e825998B8;  // sub_825998B8
inline constexpr auto& Ctor = lifted::entry::e82599758;  // sub_82599758
inline constexpr auto& Dtor = lifted::entry::e825999C8;  // sub_825999C8
inline constexpr auto& FreeNodeValues = lifted::entry::e825997A0;  // sub_825997A0
inline constexpr auto& LoadNodeValues = lifted::entry::e825997E0;  // sub_825997E0
inline constexpr auto& LocalMatrices = lifted::entry::e82599C58;  // sub_82599C58
inline constexpr auto& NodeCount = lifted::entry::e82599880;  // sub_82599880
inline constexpr auto& ResetPose = lifted::entry::e82599AF0;  // sub_82599AF0
inline constexpr auto& UpdateMatrices = lifted::entry::e825998A0;  // sub_825998A0
}  // namespace game::Skeleton

namespace game::SkeletonBase {
inline constexpr auto& Ctor = lifted::entry::e8259A140;  // sub_8259A140
inline constexpr auto& Dtor = lifted::entry::e8259A9D0;  // sub_8259A9D0
inline constexpr auto& Free = lifted::entry::e8259A228;  // sub_8259A228
}  // namespace game::SkeletonBase

namespace game::SkeletonCursor {
inline constexpr auto& Clear = lifted::entry::e825A2068;  // sub_825A2068
}  // namespace game::SkeletonCursor

namespace game::Smoke {
inline constexpr auto& Ctor = lifted::entry::e8212B2E8;  // sub_8212B2E8
inline constexpr auto& Dtor = lifted::entry::e8212B358;  // sub_8212B358
inline constexpr auto& Emit = lifted::entry::e8212A350;  // sub_8212A350
inline constexpr auto& Init = lifted::f8212B3A8;  // sub_8212B3A8
inline constexpr auto& Reset = lifted::entry::e8212A9E0;  // sub_8212A9E0
inline constexpr auto& Update = lifted::entry::e8212AA78;  // sub_8212AA78
}  // namespace game::Smoke

namespace game::SoftRaster {
inline constexpr auto& DrawSpan = lifted::entry::e820EDE00;  // sub_820EDE00
}  // namespace game::SoftRaster

namespace game::SortedList {
inline constexpr auto& Clear = lifted::entry::e82170A88;  // sub_82170A88
inline constexpr auto& Insert = lifted::entry::e82170AA0;  // sub_82170AA0
}  // namespace game::SortedList

namespace game::Sound {
inline constexpr auto& Ctor = lifted::f82117348;  // sub_82117348
inline constexpr auto& Dtor = lifted::f82117630;  // sub_82117630
inline constexpr auto& HasHandle = lifted::f82117598;  // sub_82117598
inline constexpr auto& IsBank = lifted::entry::e821175B8;  // sub_821175B8
inline constexpr auto& IsCue = lifted::entry::e82117578;  // sub_82117578
inline constexpr auto& IsStopped = lifted::f82117408;  // sub_82117408
inline constexpr auto& Play = lifted::entry::e82175850;  // sub_82175850
inline constexpr auto& Set3D = lifted::entry::e82117568;  // sub_82117568
inline constexpr auto& SetHandle = lifted::f82117550;  // sub_82117550
inline constexpr auto& SetPosition = lifted::entry::e82117560;  // sub_82117560
inline constexpr auto& Start = lifted::entry::e821173F0;  // sub_821173F0
inline constexpr auto& Stop = lifted::f82117400;  // sub_82117400
inline constexpr auto& Update = lifted::f82117438;  // sub_82117438
}  // namespace game::Sound

namespace game::SoundBankHolder {
inline constexpr auto& Ctor = lifted::entry::e8259D3D8;  // sub_8259D3D8
inline constexpr auto& GetCueIndex = lifted::f8259D3F0;  // sub_8259D3F0
}  // namespace game::SoundBankHolder

namespace game::SoundGlobals {
inline constexpr auto& Init = lifted::entry::e82110258;  // sub_82110258
inline constexpr auto& Shutdown = lifted::f82110388;  // sub_82110388
}  // namespace game::SoundGlobals

namespace game::SoundOptions {
inline constexpr auto& SetEffectsVolume = lifted::entry::e821B9C20;  // sub_821B9C20
inline constexpr auto& SetMusicVolume = lifted::entry::e821B9CF0;  // sub_821B9CF0
}  // namespace game::SoundOptions

namespace game::Sparks {
inline constexpr auto& Ctor = lifted::entry::e8212E9A0;  // sub_8212E9A0
inline constexpr auto& Dtor = lifted::f8212EA40;  // sub_8212EA40
inline constexpr auto& Emit = lifted::entry::e8212D818;  // sub_8212D818
inline constexpr auto& Reset = lifted::entry::e8212D6E0;  // sub_8212D6E0
inline constexpr auto& SetPosition = lifted::entry::e8212DF90;  // sub_8212DF90
inline constexpr auto& Update = lifted::entry::e8212DDC8;  // sub_8212DDC8
inline constexpr auto& UpdateStreak = lifted::entry::e8212E660;  // sub_8212E660
}  // namespace game::Sparks

namespace game::SplitResults {
inline constexpr auto& Draw = lifted::entry::e822200E0;  // sub_822200E0
inline constexpr auto& Open = lifted::entry::e82220050;  // sub_82220050
inline constexpr auto& Update = lifted::entry::e8221FFF0;  // sub_8221FFF0
}  // namespace game::SplitResults

namespace game::Sprite {
inline constexpr auto& Draw = lifted::f82129298;  // sub_82129298
inline constexpr auto& Record = lifted::entry::e821281A8;  // sub_821281A8
inline constexpr auto& Reset = lifted::entry::e8211EB60;  // sub_8211EB60
inline constexpr auto& ResetStyle2 = lifted::entry::e82123A00;  // sub_82123A00
}  // namespace game::Sprite

namespace game::SpriteList {
inline constexpr auto& Draw = lifted::f821292B0;  // sub_821292B0
inline constexpr auto& Record = lifted::entry::e82128750;  // sub_82128750
inline constexpr auto& Reset = lifted::entry::e821298C0;  // sub_821298C0
}  // namespace game::SpriteList

namespace game::Stage {
inline constexpr auto& Of = lifted::entry::e821121F8;  // sub_821121F8
}  // namespace game::Stage

namespace game::Stages {
inline constexpr auto& AddRacer = lifted::entry::e820FFAB8;  // sub_820FFAB8
inline constexpr auto& Clear = lifted::entry::e82101900;  // sub_82101900
inline constexpr auto& Copy = lifted::entry::e820FFD88;  // sub_820FFD88
inline constexpr auto& HasNext = lifted::entry::e820FFA30;  // sub_820FFA30
inline constexpr auto& HasRacer = lifted::entry::e820FFCF8;  // sub_820FFCF8
inline constexpr auto& Racer = lifted::entry::e820FFC98;  // sub_820FFC98
inline constexpr auto& SetRoute = lifted::entry::e82101E90;  // sub_82101E90
inline constexpr auto& SetType = lifted::entry::e820FFA50;  // sub_820FFA50
inline constexpr auto& Type = lifted::entry::e820FFBE8;  // sub_820FFBE8
}  // namespace game::Stages

namespace game::StandMode {
inline constexpr auto& Release = lifted::entry::e82246038;  // sub_82246038
}  // namespace game::StandMode

namespace game::StandScene {
inline constexpr auto& Create = lifted::entry::e82180B00;  // sub_82180B00
inline constexpr auto& PollTeardown = lifted::entry::e82179AE8;  // sub_82179AE8
inline constexpr auto& Release = lifted::entry::e82180738;  // sub_82180738
}  // namespace game::StandScene

namespace game::StateLayer {
inline constexpr auto& ApplyOp = lifted::entry::e820ADC60;  // sub_820ADC60
inline constexpr auto& IsEmpty = lifted::entry::e820ACE50;  // sub_820ACE50
}  // namespace game::StateLayer

namespace game::StateMachine {
inline constexpr auto& Add = lifted::entry::e82106158;  // sub_82106158
inline constexpr auto& Ctor = lifted::entry::e82107108;  // sub_82107108
inline constexpr auto& Handle = lifted::entry::e82106280;  // sub_82106280
}  // namespace game::StateMachine

namespace game::Stats {
inline constexpr auto& AddLoss = lifted::entry::e82113AB8;  // sub_82113AB8
inline constexpr auto& AddOutcome3 = lifted::entry::e82113B50;  // sub_82113B50
inline constexpr auto& AddWin = lifted::entry::e821139B8;  // sub_821139B8
inline constexpr auto& WinStreak = lifted::entry::e82110B10;  // sub_82110B10
}  // namespace game::Stats

namespace game::Std {
inline constexpr auto& _Throw = lifted::entry::e825A69D8;  // sub_825A69D8
inline constexpr auto& _Xran = lifted::f825944B0;  // sub_825944B0
}  // namespace game::Std

namespace game::Std::string {
inline constexpr auto& Ctor = lifted::entry::e825929E8;  // sub_825929E8
inline constexpr auto& _Copy = lifted::entry::e82091458;  // sub_82091458
inline constexpr auto& _Tidy = lifted::entry::e82091328;  // sub_82091328
}  // namespace game::Std::string

namespace game::String {
inline constexpr auto& ReplaceAll = lifted::entry::e82593088;  // sub_82593088
}  // namespace game::String

namespace game::Strings {
inline constexpr auto& CopyPage = lifted::entry::e8210A5E8;  // sub_8210A5E8
inline constexpr auto& Format = lifted::entry::e8210ABC0;  // sub_8210ABC0
inline constexpr auto& FormatV = lifted::entry::e8210A990;  // sub_8210A990
inline constexpr auto& Load = lifted::entry::e8210A8D0;  // sub_8210A8D0
inline constexpr auto& OnLoaded = lifted::entry::e8210A850;  // sub_8210A850
inline constexpr auto& PageCount = lifted::entry::e8210A5C0;  // sub_8210A5C0
inline constexpr auto& Release = lifted::entry::e8210A538;  // sub_8210A538
}  // namespace game::Strings

namespace game::Surface {
inline constexpr auto& EdramTiles = lifted::entry::e8209AD68;  // sub_8209AD68
inline constexpr auto& GetDesc = lifted::entry::e822A5F50;  // sub_822A5F50
inline constexpr auto& LockRect = lifted::entry::e822A5FE8;  // sub_822A5FE8
inline constexpr auto& Unlock = lifted::entry::e822A43F8;  // sub_822A43F8
}  // namespace game::Surface

namespace game::TGA {
inline constexpr auto& CreateImage = lifted::entry::e820ED020;  // sub_820ED020
inline constexpr auto& Decode = lifted::entry::e820EF168;  // sub_820EF168
inline constexpr auto& ParseHeader = lifted::entry::e820EEBF8;  // sub_820EEBF8
}  // namespace game::TGA

namespace game::TargetFilter {
inline constexpr auto& Record = lifted::entry::e82135B50;  // sub_82135B50
}  // namespace game::TargetFilter

namespace game::Text {
inline constexpr auto& Draw = lifted::entry::e8210A758;  // sub_8210A758
inline constexpr auto& Init = lifted::entry::e8210ADD0;  // sub_8210ADD0
inline constexpr auto& IsLoaded = lifted::entry::e82211C90;  // sub_82211C90
inline constexpr auto& Load = lifted::entry::e8210ADF8;  // sub_8210ADF8
inline constexpr auto& Unload = lifted::f8210AE38;  // sub_8210AE38
}  // namespace game::Text

namespace game::TextItem {
inline constexpr auto& Ctor = lifted::entry::e8211A818;  // sub_8211A818
inline constexpr auto& Dtor = lifted::entry::e8211A8C8;  // sub_8211A8C8
}  // namespace game::TextItem

namespace game::TextMap {
inline constexpr auto& Find = lifted::entry::e8211B310;  // sub_8211B310
inline constexpr auto& Set = lifted::entry::e8211B370;  // sub_8211B370
}  // namespace game::TextMap

namespace game::Texture {
inline constexpr auto& Create = lifted::entry::e820ECF08;  // sub_820ECF08
inline constexpr auto& Format = lifted::entry::e822A44A8;  // sub_822A44A8
inline constexpr auto& Free = lifted::entry::e820ECFC8;  // sub_820ECFC8
inline constexpr auto& GetLevelDesc = lifted::entry::e822A5B78;  // sub_822A5B78
inline constexpr auto& GetSurfaceLevel = lifted::entry::e822A5B80;  // sub_822A5B80
inline constexpr auto& LevelLayout = lifted::entry::e822A4B98;  // sub_822A4B98
inline constexpr auto& LevelOffset = lifted::entry::e822A4FF0;  // sub_822A4FF0
inline constexpr auto& LockLevel = lifted::entry::e822A54B0;  // sub_822A54B0
inline constexpr auto& LockRectLevel = lifted::entry::e822A5C38;  // sub_822A5C38
inline constexpr auto& Unlock = lifted::entry::e822A43E0;  // sub_822A43E0
}  // namespace game::Texture

namespace game::TextureAnimations {
inline constexpr auto& Advance = lifted::entry::e820F11A8;  // sub_820F11A8
}  // namespace game::TextureAnimations

namespace game::TextureLayout {
inline constexpr auto& LevelCount = lifted::entry::e822A4530;  // sub_822A4530
inline constexpr auto& Sizes = lifted::entry::e822A46C8;  // sub_822A46C8
}  // namespace game::TextureLayout

namespace game::TextureProjection {
inline constexpr auto& Fit = lifted::entry::e82598768;  // sub_82598768
}  // namespace game::TextureProjection

namespace game::TextureProvider {
inline constexpr auto& Clear = lifted::entry::e82590D68;  // sub_82590D68
inline constexpr auto& ClearAll = lifted::entry::e82594328;  // sub_82594328
inline constexpr auto& Ctor = lifted::entry::e82594850;  // sub_82594850
inline constexpr auto& Dtor = lifted::entry::e825943E0;  // sub_825943E0
inline constexpr auto& FindXprTexture = lifted::entry::e82594230;  // sub_82594230
inline constexpr auto& Get = lifted::entry::e825946B8;  // sub_825946B8
inline constexpr auto& GetXprTexture = lifted::entry::e825941D0;  // sub_825941D0
inline constexpr auto& InsertN = lifted::entry::e82590EF0;  // sub_82590EF0
inline constexpr auto& Set = lifted::entry::e82594718;  // sub_82594718
inline constexpr auto& UninitializedCopy = lifted::entry::e82594380;  // sub_82594380
}  // namespace game::TextureProvider

namespace game::TgaHeader {
inline constexpr auto& Clear = lifted::entry::e820EEBC0;  // sub_820EEBC0
}  // namespace game::TgaHeader

namespace game::TgaImage {
inline constexpr auto& Clear = lifted::entry::e820EED38;  // sub_820EED38
inline constexpr auto& Free = lifted::entry::e820EED58;  // sub_820EED58
}  // namespace game::TgaImage

namespace game::Thread {
inline constexpr auto& Begin = lifted::entry::e8258DF18;  // sub_8258DF18
inline constexpr auto& Ctor = lifted::entry::e8258DE88;  // sub_8258DE88
inline constexpr auto& Dtor = lifted::f8258DDD0;  // sub_8258DDD0
inline constexpr auto& Join = lifted::f8258DDE0;  // sub_8258DDE0
inline constexpr auto& Start = lifted::f8258DE50;  // sub_8258DE50
}  // namespace game::Thread

namespace game::TickHandler {
inline constexpr auto& Invoke = lifted::entry::e82099A98;  // sub_82099A98
}  // namespace game::TickHandler

namespace game::Tiling {
inline constexpr auto& LargestTile = lifted::entry::e8209ADD8;  // sub_8209ADD8
}  // namespace game::Tiling

namespace game::Time {
inline constexpr auto& Add = lifted::entry::e820FF458;  // sub_820FF458
inline constexpr auto& Clamp = lifted::f820FF790;  // sub_820FF790
inline constexpr auto& Less = lifted::entry::e820FF708;  // sub_820FF708
inline constexpr auto& Pack = lifted::entry::e82101700;  // sub_82101700
}  // namespace game::Time

namespace game::Tire {
inline constexpr auto& InchUp = lifted::entry::e820C46B8;  // sub_820C46B8
inline constexpr auto& MagicFormula = lifted::entry::e820B09C8;  // sub_820B09C8
}  // namespace game::Tire

namespace game::TitleLoad {
inline constexpr auto& Create = lifted::entry::e82220BA8;  // sub_82220BA8
inline constexpr auto& DebugPage0 = lifted::entry::e82220CD8;  // sub_82220CD8
inline constexpr auto& DebugPage1 = lifted::entry::e82220D60;  // sub_82220D60
inline constexpr auto& DebugPage2 = lifted::entry::e82220DE8;  // sub_82220DE8
inline constexpr auto& DebugPage3 = lifted::entry::e82220E70;  // sub_82220E70
inline constexpr auto& DebugStep0 = lifted::entry::e822204F8;  // sub_822204F8
inline constexpr auto& DebugStep1 = lifted::entry::e82220580;  // sub_82220580
inline constexpr auto& DebugStep10 = lifted::entry::e82220A80;  // sub_82220A80
inline constexpr auto& DebugStep11 = lifted::entry::e82220B10;  // sub_82220B10
inline constexpr auto& DebugStep2 = lifted::entry::e82220610;  // sub_82220610
inline constexpr auto& DebugStep3 = lifted::entry::e82220698;  // sub_82220698
inline constexpr auto& DebugStep4 = lifted::entry::e82220720;  // sub_82220720
inline constexpr auto& DebugStep5 = lifted::entry::e822207B0;  // sub_822207B0
inline constexpr auto& DebugStep6 = lifted::entry::e82220840;  // sub_82220840
inline constexpr auto& DebugStep7 = lifted::entry::e822208D0;  // sub_822208D0
inline constexpr auto& DebugStep8 = lifted::entry::e82220960;  // sub_82220960
inline constexpr auto& DebugStep9 = lifted::entry::e822209F0;  // sub_822209F0
inline constexpr auto& Destroy = lifted::entry::e82220468;  // sub_82220468
inline constexpr auto& Draw = lifted::entry::e82221198;  // sub_82221198
inline constexpr auto& Leave = lifted::entry::e82220FC8;  // sub_82220FC8
inline constexpr auto& OpenMovie = lifted::entry::e822204B0;  // sub_822204B0
inline constexpr auto& OpenTitle = lifted::entry::e82220F60;  // sub_82220F60
inline constexpr auto& PlayLogo = lifted::entry::e822210C8;  // sub_822210C8
inline constexpr auto& PostClose = lifted::entry::e82221078;  // sub_82221078
inline constexpr auto& Start = lifted::entry::e82220C88;  // sub_82220C88
inline constexpr auto& Unload = lifted::entry::e82220EF8;  // sub_82220EF8
inline constexpr auto& Update = lifted::entry::e82221228;  // sub_82221228
inline constexpr auto& WaitForArchive = lifted::entry::e822204C0;  // sub_822204C0
inline constexpr auto& WaitForMovie = lifted::entry::e82220C18;  // sub_82220C18
}  // namespace game::TitleLoad

namespace game::TitleScreen {
inline constexpr auto& BackFromCareer = lifted::entry::e82223E58;  // sub_82223E58
inline constexpr auto& BackFromMenu4 = lifted::entry::e82223FD8;  // sub_82223FD8
inline constexpr auto& CareerMenu = lifted::entry::e82221AC0;  // sub_82221AC0
inline constexpr auto& Create = lifted::entry::e82221D98;  // sub_82221D98
inline constexpr auto& Destroy = lifted::entry::e82221EC0;  // sub_82221EC0
inline constexpr auto& Draw = lifted::entry::e82223A18;  // sub_82223A18
inline constexpr auto& HighlightCareer = lifted::entry::e82223328;  // sub_82223328
inline constexpr auto& HighlightMain = lifted::entry::e82222DD0;  // sub_82222DD0
inline constexpr auto& Intro = lifted::entry::e82222B60;  // sub_82222B60
inline constexpr auto& Leave = lifted::entry::e822225D8;  // sub_822225D8
inline constexpr auto& LeavePage = lifted::entry::e822229D0;  // sub_822229D0
inline constexpr auto& LeavePressStart = lifted::entry::e82222D40;  // sub_82222D40
inline constexpr auto& LoadGameData = lifted::entry::e82222688;  // sub_82222688
inline constexpr auto& MainMenu = lifted::entry::e82222700;  // sub_82222700
inline constexpr auto& Menu4 = lifted::entry::e82222378;  // sub_82222378
inline constexpr auto& MoveCareerCursor = lifted::entry::e82223428;  // sub_82223428
inline constexpr auto& MoveMainCursor = lifted::entry::e82222E88;  // sub_82222E88
inline constexpr auto& MoveMenu4Cursor = lifted::entry::e822235D0;  // sub_822235D0
inline constexpr auto& OnControllerPicked = lifted::entry::e822219E0;  // sub_822219E0
inline constexpr auto& OnGameDataLoaded = lifted::entry::e822219A0;  // sub_822219A0
inline constexpr auto& OnInterrupt = lifted::entry::e82221A78;  // sub_82221A78
inline constexpr auto& OnResume = lifted::entry::e82221A90;  // sub_82221A90
inline constexpr auto& OpenMovie = lifted::entry::e82221990;  // sub_82221990
inline constexpr auto& OpenNext = lifted::entry::e82222538;  // sub_82222538
inline constexpr auto& PressStartLoop = lifted::entry::e82222BF8;  // sub_82222BF8
inline constexpr auto& ResetPlayer = lifted::entry::e82221D00;  // sub_82221D00
inline constexpr auto& SetUpAttractRace = lifted::f82222038;  // sub_82222038
inline constexpr auto& ShowCareerMenu = lifted::entry::e82223D70;  // sub_82223D70
inline constexpr auto& ShowMainMenu = lifted::entry::e82223B78;  // sub_82223B78
inline constexpr auto& ShowMainMenuAtOnce = lifted::entry::e82223C38;  // sub_82223C38
inline constexpr auto& ShowMenu4 = lifted::entry::e82223F30;  // sub_82223F30
inline constexpr auto& Start = lifted::entry::e82223AB8;  // sub_82223AB8
inline constexpr auto& Unload = lifted::entry::e822224D0;  // sub_822224D0
inline constexpr auto& Update_822221A0 = lifted::entry::e822221A0;  // sub_822221A0
inline constexpr auto& Update_822240B0 = lifted::entry::e822240B0;  // sub_822240B0
inline constexpr auto& WaitForMovie = lifted::entry::e82221FC8;  // sub_82221FC8
}  // namespace game::TitleScreen

namespace game::TitledListScreen {
inline constexpr auto& Ctor = lifted::entry::e822570F0;  // sub_822570F0
inline constexpr auto& DeletingDestructor = lifted::entry::e82257990;  // sub_82257990
inline constexpr auto& Draw = lifted::entry::e822579E8;  // sub_822579E8
inline constexpr auto& Dtor = lifted::entry::e82257138;  // sub_82257138
inline constexpr auto& OnItemMoved = lifted::entry::e82257B78;  // sub_82257B78
inline constexpr auto& Page2 = lifted::entry::e82257148;  // sub_82257148
inline constexpr auto& Page3 = lifted::entry::e822572F0;  // sub_822572F0
inline constexpr auto& Page4 = lifted::entry::e82257360;  // sub_82257360
inline constexpr auto& Page7 = lifted::entry::e82257490;  // sub_82257490
inline constexpr auto& Reset = lifted::entry::e82257558;  // sub_82257558
inline constexpr auto& Step0 = lifted::entry::e82257D48;  // sub_82257D48
inline constexpr auto& Step1 = lifted::entry::e82257FF8;  // sub_82257FF8
inline constexpr auto& Step2 = lifted::entry::e822580E0;  // sub_822580E0
inline constexpr auto& Update = lifted::entry::e82258998;  // sub_82258998
}  // namespace game::TitledListScreen

namespace game::TodaysEvent {
inline constexpr auto& Create = lifted::entry::e8225BE50;  // sub_8225BE50
inline constexpr auto& Destroy = lifted::entry::e8225BF78;  // sub_8225BF78
inline constexpr auto& Draw = lifted::entry::e8225C928;  // sub_8225C928
inline constexpr auto& EndPage = lifted::entry::e8225C590;  // sub_8225C590
inline constexpr auto& EventPage = lifted::entry::e8225C428;  // sub_8225C428
inline constexpr auto& FadeOut = lifted::entry::e8225C770;  // sub_8225C770
inline constexpr auto& Intro = lifted::entry::e8225CB18;  // sub_8225CB18
inline constexpr auto& Leave = lifted::entry::e8225C348;  // sub_8225C348
inline constexpr auto& NextPage = lifted::entry::e8225CD50;  // sub_8225CD50
inline constexpr auto& NextUnlock = lifted::entry::e8225CC80;  // sub_8225CC80
inline constexpr auto& OpenMovie = lifted::entry::e8225C130;  // sub_8225C130
inline constexpr auto& Start = lifted::entry::e8225C990;  // sub_8225C990
inline constexpr auto& Unload = lifted::entry::e8225C1F8;  // sub_8225C1F8
inline constexpr auto& Unloaded = lifted::entry::e8225C278;  // sub_8225C278
inline constexpr auto& Update = lifted::entry::e8225CDE8;  // sub_8225CDE8
inline constexpr auto& WaitForMovie = lifted::entry::e8225C160;  // sub_8225C160
}  // namespace game::TodaysEvent

namespace game::TodaysResult {
inline constexpr auto& Create = lifted::entry::e8225A7A0;  // sub_8225A7A0
inline constexpr auto& Destroy = lifted::entry::e8225A8A8;  // sub_8225A8A8
inline constexpr auto& Draw = lifted::entry::e8225B0C8;  // sub_8225B0C8
inline constexpr auto& FadeOut = lifted::entry::e8225AD20;  // sub_8225AD20
inline constexpr auto& Intro = lifted::entry::e8225ABD8;  // sub_8225ABD8
inline constexpr auto& Leave = lifted::entry::e8225AA88;  // sub_8225AA88
inline constexpr auto& LeavePage = lifted::entry::e8225AB30;  // sub_8225AB30
inline constexpr auto& OpenMovie = lifted::entry::e8225A510;  // sub_8225A510
inline constexpr auto& ShowLoop = lifted::entry::e8225AC88;  // sub_8225AC88
inline constexpr auto& Start = lifted::entry::e8225B300;  // sub_8225B300
inline constexpr auto& Unload = lifted::entry::e8225AA20;  // sub_8225AA20
inline constexpr auto& Unloaded = lifted::entry::e8225A9A0;  // sub_8225A9A0
inline constexpr auto& Update = lifted::entry::e8225B128;  // sub_8225B128
inline constexpr auto& WaitForMovie = lifted::entry::e8225A930;  // sub_8225A930
inline constexpr auto& WaitPage = lifted::entry::e8225A9B0;  // sub_8225A9B0
}  // namespace game::TodaysResult

namespace game::TrackCollision {
inline constexpr auto& GroundCallback = lifted::entry::e820FEAB0;  // sub_820FEAB0
inline constexpr auto& GroundHits = lifted::entry::e820FEBD0;  // sub_820FEBD0
inline constexpr auto& Load = lifted::f820FF368;  // sub_820FF368
inline constexpr auto& ResetWorld = lifted::entry::e820FEAF8;  // sub_820FEAF8
inline constexpr auto& Tick = lifted::entry::e820FF050;  // sub_820FF050
inline constexpr auto& Unload = lifted::entry::e820FEFB0;  // sub_820FEFB0
inline constexpr auto& WallHits = lifted::entry::e820FF1B0;  // sub_820FF1B0
}  // namespace game::TrackCollision

namespace game::TrackMesh {
inline constexpr auto& Free = lifted::entry::e820F6940;  // sub_820F6940
inline constexpr auto& GroundHeight = lifted::entry::e820F7AA0;  // sub_820F7AA0
inline constexpr auto& Load = lifted::f820F77A0;  // sub_820F77A0
inline constexpr auto& OnLoaded = lifted::entry::e820F69B0;  // sub_820F69B0
inline constexpr auto& Prepare = lifted::entry::e820F68E8;  // sub_820F68E8
}  // namespace game::TrackMesh

namespace game::TrackPosition {
inline constexpr auto& Copy = lifted::entry::e820C2150;  // sub_820C2150
inline constexpr auto& Init = lifted::entry::e820F8578;  // sub_820F8578
inline constexpr auto& Update = lifted::entry::e820F8470;  // sub_820F8470
}  // namespace game::TrackPosition

namespace game::TrackRoute {
inline constexpr auto& ClampPosition = lifted::entry::e820F8420;  // sub_820F8420
inline constexpr auto& Distance = lifted::entry::e820F9278;  // sub_820F9278
inline constexpr auto& Free = lifted::entry::e820F8618;  // sub_820F8618
inline constexpr auto& LaneAt = lifted::entry::e820F8920;  // sub_820F8920
inline constexpr auto& LanePoint = lifted::entry::e820F8668;  // sub_820F8668
inline constexpr auto& Link = lifted::entry::e820F89F0;  // sub_820F89F0
inline constexpr auto& Load = lifted::f820F9E28;  // sub_820F9E28
inline constexpr auto& LocateAt = lifted::entry::e820FB888;  // sub_820FB888
inline constexpr auto& Normalize = lifted::entry::e820F9FE8;  // sub_820F9FE8
inline constexpr auto& NormalizeLane = lifted::entry::e820FA210;  // sub_820FA210
inline constexpr auto& NormalizeLaneMasked = lifted::entry::e820FA350;  // sub_820FA350
inline constexpr auto& NormalizePath = lifted::entry::e820FA0F8;  // sub_820FA0F8
inline constexpr auto& OnLoaded = lifted::entry::e820F9050;  // sub_820F9050
inline constexpr auto& OnSlice = lifted::entry::e820FB1B8;  // sub_820FB1B8
inline constexpr auto& PathDistance = lifted::entry::e820FA9D0;  // sub_820FA9D0
inline constexpr auto& Prepare = lifted::entry::e820F8600;  // sub_820F8600
inline constexpr auto& SlicePoint = lifted::entry::e820F9768;  // sub_820F9768
inline constexpr auto& SliceTest = lifted::entry::e820F8DE8;  // sub_820F8DE8
inline constexpr auto& SpecialSection = lifted::entry::e820F8D38;  // sub_820F8D38
inline constexpr auto& SplinePoint = lifted::entry::e820AB6A0;  // sub_820AB6A0
inline constexpr auto& ToMainRoad = lifted::entry::e820FA728;  // sub_820FA728
}  // namespace game::TrackRoute

namespace game::TrackSet {
inline constexpr auto& Advance = lifted::entry::e825A1D68;  // sub_825A1D68
inline constexpr auto& Advance32 = lifted::entry::e825A10A8;  // sub_825A10A8
inline constexpr auto& Ctor = lifted::entry::e825A1B70;  // sub_825A1B70
inline constexpr auto& Release = lifted::entry::e825A1BA8;  // sub_825A1BA8
inline constexpr auto& ReleaseArray = lifted::entry::e825A11D0;  // sub_825A11D0
inline constexpr auto& Sample32 = lifted::entry::e825A0FE0;  // sub_825A0FE0
inline constexpr auto& SampleFloat4 = lifted::entry::e825A1C88;  // sub_825A1C88
inline constexpr auto& Setup208 = lifted::entry::e825A1A38;  // sub_825A1A38
inline constexpr auto& Setup32 = lifted::entry::e825A1218;  // sub_825A1218
inline constexpr auto& SetupFloat = lifted::entry::e825A1BE8;  // sub_825A1BE8
inline constexpr auto& SetupFloat4 = lifted::entry::e825A1E08;  // sub_825A1E08
}  // namespace game::TrackSet

namespace game::TrackZones {
inline constexpr auto& Create = lifted::entry::e8228E788;  // sub_8228E788
inline constexpr auto& Destroy = lifted::entry::e8228B500;  // sub_8228B500
inline constexpr auto& IsInside = lifted::entry::e8228BDD0;  // sub_8228BDD0
inline constexpr auto& Update = lifted::entry::e8228BD30;  // sub_8228BD30
}  // namespace game::TrackZones

namespace game::Traffic {
inline constexpr auto& PickModel = lifted::entry::e82171070;  // sub_82171070
}  // namespace game::Traffic

namespace game::TrafficCar {
inline constexpr auto& BindTextures = lifted::entry::e820D7D38;  // sub_820D7D38
inline constexpr auto& DeletingDestructor = lifted::entry::e820D89B8;  // sub_820D89B8
inline constexpr auto& OnFileLoaded = lifted::entry::e820D8430;  // sub_820D8430
}  // namespace game::TrafficCar

namespace game::TrafficPoints {
inline constexpr auto& DrawBody = lifted::entry::e820E55A0;  // sub_820E55A0
inline constexpr auto& DrawHeadlights = lifted::entry::e820E61D8;  // sub_820E61D8
inline constexpr auto& DrawTailLights = lifted::entry::e820E5B08;  // sub_820E5B08
inline constexpr auto& DrawWindow = lifted::entry::e820E5668;  // sub_820E5668
inline constexpr auto& Dtor = lifted::entry::e820E6980;  // sub_820E6980
inline constexpr auto& Find = lifted::entry::e820E58C8;  // sub_820E58C8
}  // namespace game::TrafficPoints

namespace game::Trail {
inline constexpr auto& Ctor = lifted::entry::e8212A140;  // sub_8212A140
inline constexpr auto& Dtor = lifted::f8212A1C0;  // sub_8212A1C0
inline constexpr auto& Init = lifted::f821292C8;  // sub_821292C8
inline constexpr auto& Reset = lifted::entry::e82129498;  // sub_82129498
inline constexpr auto& TurnSide = lifted::entry::e82129940;  // sub_82129940
inline constexpr auto& Update = lifted::entry::e821292E8;  // sub_821292E8
}  // namespace game::Trail

namespace game::Triangle {
inline constexpr auto& Init = lifted::entry::e820F4040;  // sub_820F4040
inline constexpr auto& Record = lifted::entry::e8217DAA0;  // sub_8217DAA0
}  // namespace game::Triangle

namespace game::TuningMachine {
inline constexpr auto& Close = lifted::entry::e821D6A78;  // sub_821D6A78
inline constexpr auto& Create = lifted::entry::e821CE4C8;  // sub_821CE4C8
inline constexpr auto& Destroy = lifted::entry::e821CBD40;  // sub_821CBD40
inline constexpr auto& Init = lifted::entry::e821E7568;  // sub_821E7568
inline constexpr auto& OnDialogClosed = lifted::entry::e821D8898;  // sub_821D8898
inline constexpr auto& OnDialogNo = lifted::entry::e821CEA88;  // sub_821CEA88
inline constexpr auto& OnNoticeClosed = lifted::entry::e821CF8B8;  // sub_821CF8B8
inline constexpr auto& OpenMovies = lifted::entry::e821CBDB0;  // sub_821CBDB0
inline constexpr auto& Start = lifted::entry::e821E7368;  // sub_821E7368
inline constexpr auto& Unload = lifted::entry::e821D69A0;  // sub_821D69A0
inline constexpr auto& WaitForMovies = lifted::entry::e821CE9E8;  // sub_821CE9E8
}  // namespace game::TuningMachine

namespace game::TuningScreen {
inline constexpr auto& AeroPartValue = lifted::entry::e821DE2D8;  // sub_821DE2D8
inline constexpr auto& AnimateInteriorGrid = lifted::f821ED108;  // sub_821ED108
inline constexpr auto& AnimateInteriorList = lifted::f821EC038;  // sub_821EC038
inline constexpr auto& AnimateInteriorModes = lifted::f821EBA00;  // sub_821EBA00
inline constexpr auto& AnimateInteriorOptions = lifted::f821EC7E0;  // sub_821EC7E0
inline constexpr auto& AnimateLightGroups = lifted::f821E9920;  // sub_821E9920
inline constexpr auto& AnimateLightKind0 = lifted::f821EA898;  // sub_821EA898
inline constexpr auto& AnimateLightKind1 = lifted::f821EB2A0;  // sub_821EB2A0
inline constexpr auto& AnimateLightKinds = lifted::f821E9DD8;  // sub_821E9DD8
inline constexpr auto& AnimatePaint = lifted::f821F3A68;  // sub_821F3A68
inline constexpr auto& AnimateSlots = lifted::f821EDCA8;  // sub_821EDCA8
inline constexpr auto& DrawPowerGraph = lifted::entry::e821DF350;  // sub_821DF350
inline constexpr auto& EquipChoice = lifted::entry::e821DF448;  // sub_821DF448
inline constexpr auto& HandleInput = lifted::entry::e821E6950;  // sub_821E6950
inline constexpr auto& HideNode19 = lifted::entry::e821DFBE8;  // sub_821DFBE8
inline constexpr auto& InputInteriorGrid = lifted::f821F1130;  // sub_821F1130
inline constexpr auto& InputInteriorList = lifted::f821F0998;  // sub_821F0998
inline constexpr auto& InputInteriorModes = lifted::f821F5B98;  // sub_821F5B98
inline constexpr auto& InputInteriorOptions = lifted::f821F0D38;  // sub_821F0D38
inline constexpr auto& InputLightGroups = lifted::f821EF320;  // sub_821EF320
inline constexpr auto& InputLightKind0 = lifted::f821EFA78;  // sub_821EFA78
inline constexpr auto& InputLightKind1 = lifted::f821F0318;  // sub_821F0318
inline constexpr auto& InputLightKinds = lifted::f821EF5E8;  // sub_821EF5E8
inline constexpr auto& InputOptionRows = lifted::f821D7C90;  // sub_821D7C90
inline constexpr auto& InputPaint = lifted::f821F5288;  // sub_821F5288
inline constexpr auto& InputPickSlot = lifted::f821F1780;  // sub_821F1780
inline constexpr auto& InputRemoveSlot = lifted::f821F1AB8;  // sub_821F1AB8
inline constexpr auto& IsItemAvailable = lifted::entry::e821CDB18;  // sub_821CDB18
inline constexpr auto& LoadCurrentChoices = lifted::entry::e821DA4B0;  // sub_821DA4B0
inline constexpr auto& LoadInteriorSlots = lifted::entry::e821DE280;  // sub_821DE280
inline constexpr auto& LoadOwnedParts = lifted::entry::e821DBAB0;  // sub_821DBAB0
inline constexpr auto& LoadUnlockedParts = lifted::entry::e821CFFC8;  // sub_821CFFC8
inline constexpr auto& MoveOptionRow = lifted::f821D4540;  // sub_821D4540
inline constexpr auto& OnDialogYes = lifted::entry::e821E2660;  // sub_821E2660
inline constexpr auto& PickCameraPreset = lifted::entry::e821D99A0;  // sub_821D99A0
inline constexpr auto& PlaceInteriorMarks = lifted::f821F32C8;  // sub_821F32C8
inline constexpr auto& PreviewItem = lifted::entry::e821E07B0;  // sub_821E07B0
inline constexpr auto& RefreshInterior = lifted::entry::e821F2AD8;  // sub_821F2AD8
inline constexpr auto& RefreshLightKits = lifted::entry::e821F2378;  // sub_821F2378
inline constexpr auto& RefreshMovie13 = lifted::entry::e822076E8;  // sub_822076E8
inline constexpr auto& RefreshMovie14 = lifted::entry::e821F7A98;  // sub_821F7A98
inline constexpr auto& RefreshPaint = lifted::entry::e821F2110;  // sub_821F2110
inline constexpr auto& SetAeroAvailability = lifted::entry::e821DE160;  // sub_821DE160
inline constexpr auto& SetCarStats = lifted::entry::e821DF788;  // sub_821DF788
inline constexpr auto& SetPrices = lifted::entry::e821DAD58;  // sub_821DAD58
inline constexpr auto& SetStateTexts = lifted::entry::e821F1DE0;  // sub_821F1DE0
inline constexpr auto& SetUpCarData = lifted::entry::e821DF2F8;  // sub_821DF2F8
inline constexpr auto& ShowInteriorPicture = lifted::entry::e821DA3F8;  // sub_821DA3F8
inline constexpr auto& ShowTireMarks = lifted::entry::e821E22E8;  // sub_821E22E8
inline constexpr auto& Update = lifted::entry::e821E6C50;  // sub_821E6C50
inline constexpr auto& WheelNumber = lifted::entry::e821CDED8;  // sub_821CDED8
}  // namespace game::TuningScreen

namespace game::TvCamera {
inline constexpr auto& Ctor = lifted::entry::e82155288;  // sub_82155288
inline constexpr auto& Track = lifted::entry::e82156980;  // sub_82156980
inline constexpr auto& Update = lifted::entry::e821593D8;  // sub_821593D8
}  // namespace game::TvCamera

namespace game::Type10SaveScreen {
inline constexpr auto& Init = lifted::entry::e821A7AC8;  // sub_821A7AC8
}  // namespace game::Type10SaveScreen

namespace game::Type12Dialog10 {
inline constexpr auto& Create = lifted::entry::e82287DD8;  // sub_82287DD8
inline constexpr auto& Destroy = lifted::entry::e82287EC0;  // sub_82287EC0
inline constexpr auto& Leave = lifted::entry::e82288028;  // sub_82288028
inline constexpr auto& OpenMovie = lifted::entry::e82287CD0;  // sub_82287CD0
inline constexpr auto& WaitForMovie = lifted::entry::e82287F40;  // sub_82287F40
}  // namespace game::Type12Dialog10

namespace game::Type12Dialog11 {
inline constexpr auto& Create = lifted::entry::e82276E60;  // sub_82276E60
inline constexpr auto& Destroy = lifted::entry::e82276F40;  // sub_82276F40
inline constexpr auto& Leave = lifted::entry::e822770B8;  // sub_822770B8
inline constexpr auto& OpenMovie = lifted::entry::e82276E40;  // sub_82276E40
inline constexpr auto& WaitForMovie = lifted::entry::e82276FC0;  // sub_82276FC0
}  // namespace game::Type12Dialog11

namespace game::Type12Dialog12 {
inline constexpr auto& Create = lifted::entry::e8227ED58;  // sub_8227ED58
inline constexpr auto& Destroy = lifted::entry::e8227EE40;  // sub_8227EE40
inline constexpr auto& Leave = lifted::entry::e8227F038;  // sub_8227F038
inline constexpr auto& OpenMovie = lifted::entry::e8227ECE8;  // sub_8227ECE8
inline constexpr auto& WaitForMovie = lifted::entry::e8227EEC0;  // sub_8227EEC0
}  // namespace game::Type12Dialog12

namespace game::Type12Dialog13 {
inline constexpr auto& Create = lifted::entry::e8227DED0;  // sub_8227DED0
inline constexpr auto& Destroy = lifted::entry::e8227DFB0;  // sub_8227DFB0
inline constexpr auto& Leave = lifted::entry::e8227E118;  // sub_8227E118
inline constexpr auto& OpenMovie = lifted::entry::e8227DEC0;  // sub_8227DEC0
inline constexpr auto& WaitForMovie = lifted::entry::e8227E030;  // sub_8227E030
}  // namespace game::Type12Dialog13

namespace game::Type12Screen1 {
inline constexpr auto& Create = lifted::entry::e822720A8;  // sub_822720A8
inline constexpr auto& Destroy = lifted::entry::e822722B0;  // sub_822722B0
inline constexpr auto& Leave = lifted::entry::e822727E0;  // sub_822727E0
inline constexpr auto& OpenMovie = lifted::entry::e82271D18;  // sub_82271D18
inline constexpr auto& WaitForMovie = lifted::entry::e82272398;  // sub_82272398
}  // namespace game::Type12Screen1

namespace game::Type12Screen3 {
inline constexpr auto& Create = lifted::entry::e8225DFE8;  // sub_8225DFE8
inline constexpr auto& Destroy = lifted::entry::e8225E100;  // sub_8225E100
inline constexpr auto& Leave = lifted::entry::e8225E518;  // sub_8225E518
inline constexpr auto& OpenMovie = lifted::entry::e8225DFD8;  // sub_8225DFD8
inline constexpr auto& WaitForMovie = lifted::entry::e8225E1B0;  // sub_8225E1B0
}  // namespace game::Type12Screen3

namespace game::Type12Screen6 {
inline constexpr auto& Create = lifted::entry::e82265A88;  // sub_82265A88
inline constexpr auto& Destroy = lifted::entry::e82265B70;  // sub_82265B70
inline constexpr auto& Leave = lifted::entry::e82265DC0;  // sub_82265DC0
inline constexpr auto& OpenMovie = lifted::entry::e82265920;  // sub_82265920
inline constexpr auto& WaitForMovie = lifted::entry::e82265C00;  // sub_82265C00
}  // namespace game::Type12Screen6

namespace game::Type12Screen7 {
inline constexpr auto& Create = lifted::entry::e82268D18;  // sub_82268D18
inline constexpr auto& Destroy = lifted::entry::e82268EC8;  // sub_82268EC8
inline constexpr auto& Leave = lifted::entry::e822697D8;  // sub_822697D8
inline constexpr auto& OpenMovie = lifted::entry::e822688A8;  // sub_822688A8
inline constexpr auto& WaitForMovie = lifted::entry::e82268F70;  // sub_82268F70
}  // namespace game::Type12Screen7

namespace game::Type12Screen8 {
inline constexpr auto& Create = lifted::entry::e82261030;  // sub_82261030
inline constexpr auto& Destroy = lifted::entry::e82261168;  // sub_82261168
inline constexpr auto& Leave = lifted::entry::e82261358;  // sub_82261358
inline constexpr auto& OpenMovie = lifted::entry::e822609C0;  // sub_822609C0
inline constexpr auto& WaitForMovie = lifted::entry::e822611F8;  // sub_822611F8
}  // namespace game::Type12Screen8

namespace game::Type12Screen9 {
inline constexpr auto& Create = lifted::entry::e822630C0;  // sub_822630C0
inline constexpr auto& Destroy = lifted::entry::e82263238;  // sub_82263238
inline constexpr auto& Leave = lifted::entry::e82263448;  // sub_82263448
inline constexpr auto& OpenMovie = lifted::entry::e82263048;  // sub_82263048
inline constexpr auto& WaitForMovie = lifted::entry::e822632C8;  // sub_822632C8
}  // namespace game::Type12Screen9

namespace game::Type2ControllerOptions {
inline constexpr auto& Init = lifted::entry::e821C8578;  // sub_821C8578
}  // namespace game::Type2ControllerOptions

namespace game::Type2Dialog {
inline constexpr auto& Cancel = lifted::entry::e82186E40;  // sub_82186E40
inline constexpr auto& CloseConfirm2Step = lifted::entry::e82187AC8;  // sub_82187AC8
inline constexpr auto& CloseConfirmStep = lifted::entry::e821877F8;  // sub_821877F8
inline constexpr auto& CloseNotice2Step = lifted::entry::e82187C18;  // sub_82187C18
inline constexpr auto& CloseNoticeStep = lifted::entry::e82187948;  // sub_82187948
inline constexpr auto& Confirm2Page = lifted::entry::e82187330;  // sub_82187330
inline constexpr auto& ConfirmPage = lifted::entry::e82187160;  // sub_82187160
inline constexpr auto& Draw = lifted::entry::e82187E50;  // sub_82187E50
inline constexpr auto& Init = lifted::entry::e82188178;  // sub_82188178
inline constexpr auto& MoveCursor2Step = lifted::entry::e82187D80;  // sub_82187D80
inline constexpr auto& MoveCursorStep = lifted::entry::e82187CC8;  // sub_82187CC8
inline constexpr auto& Notice2Page = lifted::entry::e82187618;  // sub_82187618
inline constexpr auto& NoticePage = lifted::entry::e82187508;  // sub_82187508
inline constexpr auto& OnEnter = lifted::entry::e821869B8;  // sub_821869B8
inline constexpr auto& OnLeft = lifted::entry::e82186428;  // sub_82186428
inline constexpr auto& OpenConfirm = lifted::entry::e82186C30;  // sub_82186C30
inline constexpr auto& OpenConfirm2 = lifted::entry::e82186D40;  // sub_82186D40
inline constexpr auto& OpenConfirm2Step = lifted::entry::e821879F8;  // sub_821879F8
inline constexpr auto& OpenConfirmStep = lifted::entry::e82187728;  // sub_82187728
inline constexpr auto& OpenNotice = lifted::entry::e82186AF0;  // sub_82186AF0
inline constexpr auto& OpenNotice2 = lifted::entry::e82186B90;  // sub_82186B90
inline constexpr auto& OpenNotice2Step = lifted::entry::e82187B80;  // sub_82187B80
inline constexpr auto& OpenNoticeStep = lifted::entry::e821878B0;  // sub_821878B0
inline constexpr auto& Shown = lifted::entry::e82186490;  // sub_82186490
inline constexpr auto& TakesInput = lifted::entry::e821864A0;  // sub_821864A0
inline constexpr auto& Update = lifted::entry::e82187EC8;  // sub_82187EC8
inline constexpr auto& Wait = lifted::entry::e82186AD8;  // sub_82186AD8
inline constexpr auto& WaitForMovies = lifted::entry::e821880D0;  // sub_821880D0
}  // namespace game::Type2Dialog

namespace game::Type2Hub {
inline constexpr auto& Back = lifted::entry::e8218F698;  // sub_8218F698
inline constexpr auto& Choose = lifted::entry::e8218E768;  // sub_8218E768
inline constexpr auto& Close = lifted::entry::e8218E688;  // sub_8218E688
inline constexpr auto& Create = lifted::entry::e8218E078;  // sub_8218E078
inline constexpr auto& Destroy = lifted::entry::e8218E1A8;  // sub_8218E1A8
inline constexpr auto& FreeImage = lifted::entry::e8218D650;  // sub_8218D650
inline constexpr auto& ImageLoaded = lifted::entry::e8218D8E8;  // sub_8218D8E8
inline constexpr auto& InitStates = lifted::entry::e82190B18;  // sub_82190B18
inline constexpr auto& MenuPage = lifted::entry::e8218FCC8;  // sub_8218FCC8
inline constexpr auto& NoticePage = lifted::entry::e8218E8F8;  // sub_8218E8F8
inline constexpr auto& OnAnswer2 = lifted::f8218D6D8;  // sub_8218D6D8
inline constexpr auto& OnDialogDone = lifted::entry::e8218F800;  // sub_8218F800
inline constexpr auto& OnMessage44 = lifted::f8218FAA0;  // sub_8218FAA0
inline constexpr auto& OnMessage48 = lifted::f8218D6F0;  // sub_8218D6F0
inline constexpr auto& OnNo = lifted::entry::e8218D6C0;  // sub_8218D6C0
inline constexpr auto& OnYes = lifted::entry::e8218D6A8;  // sub_8218D6A8
inline constexpr auto& OpenMovie = lifted::entry::e8218E368;  // sub_8218E368
inline constexpr auto& OpenSelected = lifted::entry::e8218E5B0;  // sub_8218E5B0
inline constexpr auto& RecordPage = lifted::entry::e821900B8;  // sub_821900B8
inline constexpr auto& Reenter = lifted::entry::e8218D9B8;  // sub_8218D9B8
inline constexpr auto& Resume = lifted::f8218DE18;  // sub_8218DE18
inline constexpr auto& SavePromptPage = lifted::f82190168;  // sub_82190168
inline constexpr auto& SelectCarSettings = lifted::f8218F620;  // sub_8218F620
inline constexpr auto& ShowNoticeImage = lifted::entry::e8218F3F8;  // sub_8218F3F8
inline constexpr auto& Start = lifted::entry::e8218F6E0;  // sub_8218F6E0
inline constexpr auto& Transition0 = lifted::entry::e821901C0;  // sub_821901C0
inline constexpr auto& Transition1 = lifted::entry::e8218EC78;  // sub_8218EC78
inline constexpr auto& Transition10 = lifted::f82190760;  // sub_82190760
inline constexpr auto& Transition11 = lifted::f8218F220;  // sub_8218F220
inline constexpr auto& Transition12 = lifted::f8218F2B8;  // sub_8218F2B8
inline constexpr auto& Transition2 = lifted::entry::e8218EE48;  // sub_8218EE48
inline constexpr auto& Transition3 = lifted::entry::e8218ED98;  // sub_8218ED98
inline constexpr auto& Transition4 = lifted::f8218EB78;  // sub_8218EB78
inline constexpr auto& Transition5 = lifted::entry::e82190548;  // sub_82190548
inline constexpr auto& Transition6 = lifted::entry::e8218F098;  // sub_8218F098
inline constexpr auto& Transition7 = lifted::entry::e821902C8;  // sub_821902C8
inline constexpr auto& Transition8 = lifted::f82190690;  // sub_82190690
inline constexpr auto& Transition9 = lifted::f8218F150;  // sub_8218F150
inline constexpr auto& Unload = lifted::entry::e8218E530;  // sub_8218E530
inline constexpr auto& Update = lifted::entry::e821907F8;  // sub_821907F8
inline constexpr auto& UpdatePage = lifted::entry::e82190498;  // sub_82190498
inline constexpr auto& WaitForLoad = lifted::entry::e8218E420;  // sub_8218E420
}  // namespace game::Type2Hub

namespace game::Type2Options {
inline constexpr auto& Init = lifted::entry::e821C9E00;  // sub_821C9E00
}  // namespace game::Type2Options

namespace game::Type2SaveScreen {
inline constexpr auto& Init = lifted::entry::e821A6E18;  // sub_821A6E18
}  // namespace game::Type2SaveScreen

namespace game::Type4ControllerOptions {
inline constexpr auto& Init = lifted::entry::e821C8A60;  // sub_821C8A60
}  // namespace game::Type4ControllerOptions

namespace game::Type4Options {
inline constexpr auto& Init = lifted::entry::e821CA458;  // sub_821CA458
}  // namespace game::Type4Options

namespace game::Type4SaveScreen {
inline constexpr auto& Init = lifted::entry::e821A7470;  // sub_821A7470
}  // namespace game::Type4SaveScreen

namespace game::Type5Dialog {
inline constexpr auto& CloseConfirm2Step = lifted::entry::e822264B8;  // sub_822264B8
inline constexpr auto& CloseConfirmStep = lifted::entry::e822262B8;  // sub_822262B8
inline constexpr auto& CloseNotice2Step = lifted::entry::e82226568;  // sub_82226568
inline constexpr auto& CloseNoticeStep = lifted::entry::e82226370;  // sub_82226370
inline constexpr auto& ConfirmPage = lifted::entry::e82225D10;  // sub_82225D10
inline constexpr auto& Draw = lifted::entry::e822266F8;  // sub_822266F8
inline constexpr auto& MoveCursorStep = lifted::entry::e82226618;  // sub_82226618
inline constexpr auto& Notice2Page = lifted::entry::e82226088;  // sub_82226088
inline constexpr auto& NoticePage = lifted::entry::e82225F30;  // sub_82225F30
inline constexpr auto& OnEnter = lifted::entry::e82225878;  // sub_82225878
inline constexpr auto& OnInterrupt = lifted::entry::e82225860;  // sub_82225860
inline constexpr auto& OnLeft = lifted::entry::e822257A8;  // sub_822257A8
inline constexpr auto& OpenConfirm = lifted::entry::e82225A90;  // sub_82225A90
inline constexpr auto& OpenConfirm2 = lifted::entry::e82225B88;  // sub_82225B88
inline constexpr auto& OpenConfirm2Step = lifted::entry::e82226420;  // sub_82226420
inline constexpr auto& OpenConfirmStep = lifted::entry::e822261E0;  // sub_822261E0
inline constexpr auto& OpenNotice = lifted::entry::e82225980;  // sub_82225980
inline constexpr auto& OpenNotice2 = lifted::entry::e82225A08;  // sub_82225A08
inline constexpr auto& Step9 = lifted::entry::e822266D0;  // sub_822266D0
inline constexpr auto& Update = lifted::entry::e82226788;  // sub_82226788
inline constexpr auto& WaitForMovies = lifted::entry::e82226978;  // sub_82226978
}  // namespace game::Type5Dialog

namespace game::Type6Dialog {
inline constexpr auto& Cancel = lifted::f822321B8;  // sub_822321B8
inline constexpr auto& ConfirmPage = lifted::entry::e82232328;  // sub_82232328
inline constexpr auto& Draw = lifted::entry::e82232740;  // sub_82232740
inline constexpr auto& IsTakingInput = lifted::f82231F70;  // sub_82231F70
inline constexpr auto& NoticePage = lifted::f822324E8;  // sub_822324E8
inline constexpr auto& OnEnter = lifted::entry::e82231F98;  // sub_82231F98
inline constexpr auto& OpenConfirm = lifted::entry::e822320D8;  // sub_822320D8
inline constexpr auto& OpenNotice = lifted::f82232058;  // sub_82232058
inline constexpr auto& QueueReopen = lifted::f82231F58;  // sub_82231F58
inline constexpr auto& Shown = lifted::entry::e82231F48;  // sub_82231F48
inline constexpr auto& Update = lifted::entry::e82232798;  // sub_82232798
inline constexpr auto& Wait = lifted::f82232040;  // sub_82232040
inline constexpr auto& WaitForMovies = lifted::entry::e82232910;  // sub_82232910
}  // namespace game::Type6Dialog

namespace game::Type6ListScreen {
inline constexpr auto& Back = lifted::entry::e822337B0;  // sub_822337B0
inline constexpr auto& DeletingDestructor = lifted::entry::e82233478;  // sub_82233478
inline constexpr auto& Dtor = lifted::entry::e822332D8;  // sub_822332D8
inline constexpr auto& InitMachine = lifted::entry::e8223F9A8;  // sub_8223F9A8
inline constexpr auto& ItemInfo = lifted::entry::e82237008;  // sub_82237008
inline constexpr auto& OpenMovie = lifted::entry::e82233320;  // sub_82233320
inline constexpr auto& Release = lifted::entry::e82233330;  // sub_82233330
inline constexpr auto& SetItemTexts = lifted::entry::e822336C0;  // sub_822336C0
inline constexpr auto& SetUpMenus = lifted::entry::e82233378;  // sub_82233378
inline constexpr auto& SetUpRace = lifted::entry::e822334C8;  // sub_822334C8
inline constexpr auto& ShowMarks = lifted::entry::e82233730;  // sub_82233730
}  // namespace game::Type6ListScreen

namespace game::Type6Mode {
inline constexpr auto& ForceExit = lifted::f82236608;  // sub_82236608
inline constexpr auto& Init = lifted::entry::e82236880;  // sub_82236880
inline constexpr auto& Interrupt = lifted::f82235D60;  // sub_82235D60
inline constexpr auto& Leaving = lifted::entry::e82236100;  // sub_82236100
inline constexpr auto& NextMode = lifted::entry::e822363B8;  // sub_822363B8
inline constexpr auto& Resume = lifted::f82235F10;  // sub_82235F10
inline constexpr auto& TearDown = lifted::entry::e82235C18;  // sub_82235C18
inline constexpr auto& WaitForLoads = lifted::entry::e82235F28;  // sub_82235F28
}  // namespace game::Type6Mode

namespace game::Type6NodeScreen {
inline constexpr auto& Back = lifted::entry::e82237110;  // sub_82237110
inline constexpr auto& DeletingDestructor = lifted::entry::e82237048;  // sub_82237048
inline constexpr auto& Dtor = lifted::entry::e82236EA0;  // sub_82236EA0
inline constexpr auto& InitMachine = lifted::entry::e82237320;  // sub_82237320
inline constexpr auto& IsLoaded = lifted::entry::e82236FD8;  // sub_82236FD8
inline constexpr auto& OpenMovie = lifted::entry::e82236EB0;  // sub_82236EB0
inline constexpr auto& Release = lifted::entry::e82236EC0;  // sub_82236EC0
inline constexpr auto& SetUpMenus = lifted::entry::e82236F00;  // sub_82236F00
}  // namespace game::Type6NodeScreen

namespace game::Type6PanelScreen {
inline constexpr auto& Back = lifted::entry::e8223E708;  // sub_8223E708
inline constexpr auto& CarEntry = lifted::entry::e822307E0;  // sub_822307E0
inline constexpr auto& CarList = lifted::entry::e822307D0;  // sub_822307D0
inline constexpr auto& CarSlot = lifted::entry::e822307F0;  // sub_822307F0
inline constexpr auto& DeletingDestructor = lifted::entry::e82230B00;  // sub_82230B00
inline constexpr auto& Draw = lifted::entry::e82230F40;  // sub_82230F40
inline constexpr auto& Dtor = lifted::entry::e82230668;  // sub_82230668
inline constexpr auto& InitMachine = lifted::entry::e82230F80;  // sub_82230F80
inline constexpr auto& OpenMovies = lifted::entry::e82230758;  // sub_82230758
inline constexpr auto& OpenPowerGraph = lifted::entry::e82247058;  // sub_82247058
inline constexpr auto& PickCar = lifted::entry::e82230C88;  // sub_82230C88
}  // namespace game::Type6PanelScreen

namespace game::Type6Screen0 {
inline constexpr auto& BackStep = lifted::entry::e82234E38;  // sub_82234E38
inline constexpr auto& BackToMenu = lifted::f82234928;  // sub_82234928
inline constexpr auto& ChosenPage = lifted::entry::e82234190;  // sub_82234190
inline constexpr auto& ConfirmAgain = lifted::entry::e822340B0;  // sub_822340B0
inline constexpr auto& CursorStep = lifted::entry::e82234D50;  // sub_82234D50
inline constexpr auto& Draw = lifted::entry::e82234ED8;  // sub_82234ED8
inline constexpr auto& FadeOutStep = lifted::entry::e822342F0;  // sub_822342F0
inline constexpr auto& GoOn = lifted::entry::e822339C0;  // sub_822339C0
inline constexpr auto& Leave = lifted::entry::e82233DF8;  // sub_82233DF8
inline constexpr auto& MenuPage = lifted::entry::e82234958;  // sub_82234958
inline constexpr auto& OnDialogClosed = lifted::entry::e82234768;  // sub_82234768
inline constexpr auto& OnEnter = lifted::entry::e822339F0;  // sub_822339F0
inline constexpr auto& OnLeft = lifted::entry::e82233870;  // sub_82233870
inline constexpr auto& OpenMovie = lifted::entry::e822338E8;  // sub_822338E8
inline constexpr auto& OpeningStep = lifted::entry::e82234C20;  // sub_82234C20
inline constexpr auto& PickBack = lifted::f822339B0;  // sub_822339B0
inline constexpr auto& PickNo = lifted::entry::e822339A0;  // sub_822339A0
inline constexpr auto& PickYes = lifted::entry::e82233990;  // sub_82233990
inline constexpr auto& PowerGraphPage = lifted::entry::e82234B80;  // sub_82234B80
inline constexpr auto& QuitPage = lifted::entry::e82234BF0;  // sub_82234BF0
inline constexpr auto& SetUpCar = lifted::entry::e82233E80;  // sub_82233E80
inline constexpr auto& Show = lifted::entry::e82233C80;  // sub_82233C80
inline constexpr auto& Suspend = lifted::f82233948;  // sub_82233948
inline constexpr auto& Update = lifted::entry::e82234F08;  // sub_82234F08
inline constexpr auto& WaitForMovie = lifted::entry::e82233B18;  // sub_82233B18
}  // namespace game::Type6Screen0

namespace game::Type6Screens {
inline constexpr auto& DeletingDestructor = lifted::entry::e82236830;  // sub_82236830
inline constexpr auto& Dtor = lifted::entry::e822367C0;  // sub_822367C0
}  // namespace game::Type6Screens

namespace game::Type7Dialog {
inline constexpr auto& Cancel = lifted::f82241CE8;  // sub_82241CE8
inline constexpr auto& ConfirmPage = lifted::entry::e82241EF8;  // sub_82241EF8
inline constexpr auto& Draw = lifted::entry::e822421C0;  // sub_822421C0
inline constexpr auto& IsTakingInput = lifted::f822419E0;  // sub_822419E0
inline constexpr auto& Leave = lifted::entry::e82241E58;  // sub_82241E58
inline constexpr auto& NoticePage = lifted::f822420C0;  // sub_822420C0
inline constexpr auto& OnEnter = lifted::entry::e82241A08;  // sub_82241A08
inline constexpr auto& OpenConfirm = lifted::entry::e82241B40;  // sub_82241B40
inline constexpr auto& OpenNotice = lifted::f82241AC8;  // sub_82241AC8
inline constexpr auto& QueueReopen = lifted::entry::e822419D0;  // sub_822419D0
inline constexpr auto& Shown = lifted::entry::e822419C0;  // sub_822419C0
inline constexpr auto& Update = lifted::entry::e82242218;  // sub_82242218
inline constexpr auto& Wait = lifted::f82241AB0;  // sub_82241AB0
inline constexpr auto& WaitForMovies = lifted::entry::e82242388;  // sub_82242388
}  // namespace game::Type7Dialog

namespace game::Type7ListScreen {
inline constexpr auto& Back = lifted::entry::e822434C8;  // sub_822434C8
inline constexpr auto& ClearItemText = lifted::entry::e822432A8;  // sub_822432A8
inline constexpr auto& DeletingDestructor = lifted::entry::e82242FC8;  // sub_82242FC8
inline constexpr auto& Dtor = lifted::entry::e82242EC8;  // sub_82242EC8
inline constexpr auto& InitMachine = lifted::entry::e822436A0;  // sub_822436A0
inline constexpr auto& ItemInfo = lifted::entry::e82242FA0;  // sub_82242FA0
inline constexpr auto& OpenMovie = lifted::entry::e82242F10;  // sub_82242F10
inline constexpr auto& Release = lifted::entry::e82242F20;  // sub_82242F20
inline constexpr auto& SetDetails = lifted::entry::e82243308;  // sub_82243308
inline constexpr auto& SetItemText = lifted::entry::e82243238;  // sub_82243238
inline constexpr auto& SetTitle = lifted::entry::e822433D8;  // sub_822433D8
inline constexpr auto& SetUpMenus = lifted::entry::e82243588;  // sub_82243588
inline constexpr auto& SetUpRace = lifted::entry::e82243018;  // sub_82243018
inline constexpr auto& ShowMarks = lifted::entry::e82243448;  // sub_82243448
}  // namespace game::Type7ListScreen

namespace game::Type7Mode {
inline constexpr auto& ForceExit = lifted::f82246A40;  // sub_82246A40
inline constexpr auto& Init = lifted::entry::e82246C90;  // sub_82246C90
inline constexpr auto& Interrupt = lifted::f82246228;  // sub_82246228
inline constexpr auto& Leaving = lifted::entry::e82246590;  // sub_82246590
inline constexpr auto& NextMode = lifted::entry::e82246800;  // sub_82246800
inline constexpr auto& Resume = lifted::f822463C0;  // sub_822463C0
inline constexpr auto& TearDown = lifted::entry::e82246098;  // sub_82246098
inline constexpr auto& WaitForLoads = lifted::entry::e822463D8;  // sub_822463D8
}  // namespace game::Type7Mode

namespace game::Type7PanelScreen {
inline constexpr auto& Back = lifted::entry::e82240860;  // sub_82240860
inline constexpr auto& CarEntry = lifted::entry::e82240248;  // sub_82240248
inline constexpr auto& CarList = lifted::entry::e82240238;  // sub_82240238
inline constexpr auto& CarSlot = lifted::entry::e82240258;  // sub_82240258
inline constexpr auto& Draw = lifted::entry::e82240958;  // sub_82240958
inline constexpr auto& Dtor = lifted::entry::e82240168;  // sub_82240168
inline constexpr auto& InitMachine = lifted::entry::e82240998;  // sub_82240998
inline constexpr auto& OpenMovies = lifted::entry::e822401D0;  // sub_822401D0
inline constexpr auto& OpenPowerGraph = lifted::entry::e822408B8;  // sub_822408B8
inline constexpr auto& PickCar = lifted::entry::e822405F8;  // sub_822405F8
}  // namespace game::Type7PanelScreen

namespace game::Type7Screen0 {
inline constexpr auto& BackStep = lifted::entry::e822450A8;  // sub_822450A8
inline constexpr auto& ChosenPage = lifted::entry::e82244610;  // sub_82244610
inline constexpr auto& CursorStep = lifted::entry::e82244FC0;  // sub_82244FC0
inline constexpr auto& Draw = lifted::entry::e82245148;  // sub_82245148
inline constexpr auto& FadeOutStep = lifted::entry::e82244760;  // sub_82244760
inline constexpr auto& Leave = lifted::entry::e82244350;  // sub_82244350
inline constexpr auto& MenuPage = lifted::entry::e82244BF8;  // sub_82244BF8
inline constexpr auto& OnDialogClosed = lifted::entry::e82244B60;  // sub_82244B60
inline constexpr auto& OnEnter = lifted::entry::e82243F58;  // sub_82243F58
inline constexpr auto& OnLeft = lifted::entry::e82243E08;  // sub_82243E08
inline constexpr auto& OpenMovie = lifted::entry::e82243E80;  // sub_82243E80
inline constexpr auto& OpeningStep = lifted::entry::e82244E70;  // sub_82244E70
inline constexpr auto& PickBack = lifted::f82243F48;  // sub_82243F48
inline constexpr auto& PickNo = lifted::f82243F38;  // sub_82243F38
inline constexpr auto& PickYes = lifted::entry::e82243F28;  // sub_82243F28
inline constexpr auto& PowerGraphPage = lifted::entry::e82244E00;  // sub_82244E00
inline constexpr auto& SetUpCar = lifted::entry::e822443E0;  // sub_822443E0
inline constexpr auto& Show = lifted::entry::e822441D8;  // sub_822441D8
inline constexpr auto& Suspend = lifted::f82243EE0;  // sub_82243EE0
inline constexpr auto& Update = lifted::entry::e82245200;  // sub_82245200
inline constexpr auto& WaitForMovie = lifted::entry::e82244080;  // sub_82244080
}  // namespace game::Type7Screen0

namespace game::Type7Screens {
inline constexpr auto& DeletingDestructor = lifted::entry::e82246C40;  // sub_82246C40
inline constexpr auto& Dtor = lifted::entry::e82246BE0;  // sub_82246BE0
}  // namespace game::Type7Screens

namespace game::Type8ColorListPart {
inline constexpr auto& Ctor = lifted::entry::e82253190;  // sub_82253190
inline constexpr auto& DeletingDestructor = lifted::entry::e822532B8;  // sub_822532B8
inline constexpr auto& Dtor = lifted::entry::e822531F0;  // sub_822531F0
inline constexpr auto& IsFirstChoice = lifted::entry::e82253238;  // sub_82253238
inline constexpr auto& ThirdChoice = lifted::entry::e82253240;  // sub_82253240
}  // namespace game::Type8ColorListPart

namespace game::Type8ColorListScreen {
inline constexpr auto& ClearItemText = lifted::entry::e8223E568;  // sub_8223E568
inline constexpr auto& Ctor = lifted::entry::e8223DFC0;  // sub_8223DFC0
inline constexpr auto& DeletingDestructor = lifted::entry::e8223E2B8;  // sub_8223E2B8
inline constexpr auto& InitMachine = lifted::entry::e8223E9D8;  // sub_8223E9D8
inline constexpr auto& ItemInfo = lifted::entry::e8223E4D0;  // sub_8223E4D0
inline constexpr auto& OpenMovie = lifted::entry::e8223E310;  // sub_8223E310
inline constexpr auto& SetDetails = lifted::entry::e8223E5C8;  // sub_8223E5C8
inline constexpr auto& SetItemText = lifted::entry::e8223E4F8;  // sub_8223E4F8
inline constexpr auto& SetLabels = lifted::entry::e8223E698;  // sub_8223E698
inline constexpr auto& SetUpMenus = lifted::entry::e8223E320;  // sub_8223E320
inline constexpr auto& SetUpRace = lifted::entry::e8223E760;  // sub_8223E760
}  // namespace game::Type8ColorListScreen

namespace game::Type8ColorPartInner {
inline constexpr auto& DeletingDestructor = lifted::entry::e82253268;  // sub_82253268
inline constexpr auto& Input = lifted::entry::e82253FC0;  // sub_82253FC0
inline constexpr auto& Open = lifted::entry::e822537A0;  // sub_822537A0
inline constexpr auto& Page0 = lifted::entry::e82254178;  // sub_82254178
inline constexpr auto& Pick = lifted::entry::e82254280;  // sub_82254280
inline constexpr auto& Step0 = lifted::entry::e82253E48;  // sub_82253E48
}  // namespace game::Type8ColorPartInner

namespace game::Type8Dialog {
inline constexpr auto& ConfirmPage = lifted::entry::e8223A6B8;  // sub_8223A6B8
inline constexpr auto& Draw = lifted::entry::e8223AC00;  // sub_8223AC00
inline constexpr auto& DrawWaiting = lifted::entry::e8223A5F0;  // sub_8223A5F0
inline constexpr auto& NoticePage = lifted::entry::e8223A858;  // sub_8223A858
inline constexpr auto& OnEnter = lifted::entry::e8223A408;  // sub_8223A408
inline constexpr auto& OpenConfirm = lifted::entry::e8223A510;  // sub_8223A510
inline constexpr auto& OpenConfirmStep = lifted::entry::e8223A940;  // sub_8223A940
inline constexpr auto& OpenMovies = lifted::entry::e8223A398;  // sub_8223A398
inline constexpr auto& OpenNotice = lifted::entry::e8223A4A0;  // sub_8223A4A0
inline constexpr auto& Update = lifted::entry::e8223AC58;  // sub_8223AC58
inline constexpr auto& WaitForMovies = lifted::entry::e8223ADC0;  // sub_8223ADC0
}  // namespace game::Type8Dialog

namespace game::Type8ListBase {
inline constexpr auto& Ctor = lifted::entry::e822574D0;  // sub_822574D0
inline constexpr auto& DeletingDestructor = lifted::entry::e82257B20;  // sub_82257B20
inline constexpr auto& Draw = lifted::entry::e82258D60;  // sub_82258D60
inline constexpr auto& Dtor = lifted::entry::e82257548;  // sub_82257548
inline constexpr auto& Page2 = lifted::entry::e82257588;  // sub_82257588
inline constexpr auto& Page3 = lifted::entry::e82257768;  // sub_82257768
inline constexpr auto& Page4 = lifted::entry::e82258A40;  // sub_82258A40
inline constexpr auto& Page5 = lifted::entry::e82258AD0;  // sub_82258AD0
inline constexpr auto& Page6 = lifted::entry::e822578D8;  // sub_822578D8
inline constexpr auto& Page8 = lifted::entry::e82257900;  // sub_82257900
inline constexpr auto& Step0 = lifted::entry::e82258158;  // sub_82258158
inline constexpr auto& Step1 = lifted::entry::e82258648;  // sub_82258648
inline constexpr auto& Step2 = lifted::entry::e82258BE0;  // sub_82258BE0
inline constexpr auto& Step3 = lifted::entry::e82258438;  // sub_82258438
inline constexpr auto& Update = lifted::entry::e82258EB0;  // sub_82258EB0
}  // namespace game::Type8ListBase

namespace game::Type8ListPart {
inline constexpr auto& Ctor = lifted::entry::e82254488;  // sub_82254488
inline constexpr auto& DeletingDestructor = lifted::entry::e82254580;  // sub_82254580
inline constexpr auto& Dtor = lifted::entry::e822544E8;  // sub_822544E8
}  // namespace game::Type8ListPart

namespace game::Type8ListPartInner {
inline constexpr auto& DeletingDestructor = lifted::entry::e82254530;  // sub_82254530
inline constexpr auto& Input = lifted::entry::e82254E18;  // sub_82254E18
inline constexpr auto& Open = lifted::entry::e822549D8;  // sub_822549D8
inline constexpr auto& Page0 = lifted::entry::e82254FB8;  // sub_82254FB8
inline constexpr auto& Pick = lifted::entry::e822550C0;  // sub_822550C0
inline constexpr auto& Step0 = lifted::entry::e82254D08;  // sub_82254D08
}  // namespace game::Type8ListPartInner

namespace game::Type8ListScreen {
inline constexpr auto& Ctor = lifted::entry::e8223F140;  // sub_8223F140
inline constexpr auto& DeletingDestructor = lifted::entry::e8223F388;  // sub_8223F388
inline constexpr auto& IsLoaded = lifted::entry::e8223F590;  // sub_8223F590
inline constexpr auto& ItemInfo = lifted::entry::e8223F598;  // sub_8223F598
inline constexpr auto& OpenMovie = lifted::entry::e8223F3E0;  // sub_8223F3E0
inline constexpr auto& Release = lifted::entry::e8223F588;  // sub_8223F588
inline constexpr auto& SetItemTexts = lifted::entry::e8223F5D8;  // sub_8223F5D8
inline constexpr auto& SetLabels = lifted::entry::e8223F648;  // sub_8223F648
inline constexpr auto& SetUpMenus = lifted::entry::e8223F3F0;  // sub_8223F3F0
inline constexpr auto& SetUpRace = lifted::entry::e8223F778;  // sub_8223F778
inline constexpr auto& ShowMarks = lifted::entry::e8223F6C8;  // sub_8223F6C8
}  // namespace game::Type8ListScreen

namespace game::Type8PanelScreen {
inline constexpr auto& Ctor = lifted::entry::e822386F0;  // sub_822386F0
inline constexpr auto& DeleteMovies = lifted::entry::e82238450;  // sub_82238450
inline constexpr auto& DeletingDestructor = lifted::entry::e822388D8;  // sub_822388D8
inline constexpr auto& Draw = lifted::entry::e82238E78;  // sub_82238E78
inline constexpr auto& Dtor = lifted::entry::e82238368;  // sub_82238368
inline constexpr auto& FadeInGroup = lifted::entry::e82238468;  // sub_82238468
inline constexpr auto& InitMachine = lifted::entry::e82238E98;  // sub_82238E98
inline constexpr auto& InitWords = lifted::entry::e82238420;  // sub_82238420
inline constexpr auto& IsLoaded = lifted::entry::e82238418;  // sub_82238418
inline constexpr auto& Leave = lifted::entry::e82238B10;  // sub_82238B10
inline constexpr auto& OnAnswer0 = lifted::entry::e82238540;  // sub_82238540
inline constexpr auto& OnAnswer1 = lifted::entry::e82238550;  // sub_82238550
inline constexpr auto& OnAnswer2 = lifted::entry::e82238560;  // sub_82238560
inline constexpr auto& OnDialogClosed = lifted::entry::e82238990;  // sub_82238990
inline constexpr auto& OnMessage2 = lifted::entry::e82238458;  // sub_82238458
inline constexpr auto& OnSaveLoaded = lifted::entry::e82238530;  // sub_82238530
inline constexpr auto& OpenMovies = lifted::entry::e82238440;  // sub_82238440
inline constexpr auto& PickCar = lifted::entry::e82238C88;  // sub_82238C88
inline constexpr auto& Show = lifted::entry::e82238928;  // sub_82238928
inline constexpr auto& UpdateOpening = lifted::entry::e82238A00;  // sub_82238A00
}  // namespace game::Type8PanelScreen

namespace game::Type8PartInner {
inline constexpr auto& Page1 = lifted::entry::e82253108;  // sub_82253108
inline constexpr auto& Page2 = lifted::entry::e82253838;  // sub_82253838
inline constexpr auto& Step1 = lifted::entry::e822538A8;  // sub_822538A8
}  // namespace game::Type8PartInner

namespace game::Type8Screen0 {
inline constexpr auto& ChosenPage = lifted::entry::e8223C0C0;  // sub_8223C0C0
inline constexpr auto& Confirm = lifted::entry::e8223C430;  // sub_8223C430
inline constexpr auto& ConfirmPage = lifted::entry::e8223B5C0;  // sub_8223B5C0
inline constexpr auto& Ctor = lifted::entry::e8223D2F0;  // sub_8223D2F0
inline constexpr auto& CursorStep = lifted::entry::e8223C330;  // sub_8223C330
inline constexpr auto& DeletingDestructor = lifted::entry::e8223C528;  // sub_8223C528
inline constexpr auto& Draw = lifted::entry::e8223C888;  // sub_8223C888
inline constexpr auto& Dtor = lifted::entry::e8223BF00;  // sub_8223BF00
inline constexpr auto& FadeOutStep = lifted::entry::e8223BAF0;  // sub_8223BAF0
inline constexpr auto& ItemStep = lifted::entry::e8223BBC8;  // sub_8223BBC8
inline constexpr auto& Leave = lifted::entry::e8223BA48;  // sub_8223BA48
inline constexpr auto& MenuPage = lifted::entry::e8223C6B8;  // sub_8223C6B8
inline constexpr auto& OnDialogClosed = lifted::entry::e8223C098;  // sub_8223C098
inline constexpr auto& OnEnter = lifted::entry::e8223B630;  // sub_8223B630
inline constexpr auto& OnLeft = lifted::entry::e8223B588;  // sub_8223B588
inline constexpr auto& OpenMovie = lifted::entry::e8223B530;  // sub_8223B530
inline constexpr auto& OpeningStep = lifted::entry::e8223C1A0;  // sub_8223C1A0
inline constexpr auto& PickBack = lifted::entry::e8223B5B0;  // sub_8223B5B0
inline constexpr auto& PickNo = lifted::entry::e8223B5A0;  // sub_8223B5A0
inline constexpr auto& PickYes = lifted::entry::e8223B590;  // sub_8223B590
inline constexpr auto& PlaceCursor = lifted::entry::e8223BE88;  // sub_8223BE88
inline constexpr auto& Show = lifted::entry::e8223BF18;  // sub_8223BF18
inline constexpr auto& ShowPage = lifted::entry::e8223BD20;  // sub_8223BD20
inline constexpr auto& Suspend = lifted::entry::e8223B540;  // sub_8223B540
inline constexpr auto& Update = lifted::entry::e8223C588;  // sub_8223C588
inline constexpr auto& WaitForMovie = lifted::entry::e8223B6D8;  // sub_8223B6D8
}  // namespace game::Type8Screen0

namespace game::Ucl {
inline constexpr auto& Unpack = lifted::entry::e82294610;  // sub_82294610
}  // namespace game::Ucl

namespace game::UiMovie {
inline constexpr auto& AnimationTime = lifted::entry::e8211BA00;  // sub_8211BA00
inline constexpr auto& ApplyAnimation = lifted::entry::e825A08E0;  // sub_825A08E0
inline constexpr auto& ApplyTrack = lifted::entry::e825A0548;  // sub_825A0548
inline constexpr auto& Child = lifted::entry::e8259D658;  // sub_8259D658
inline constexpr auto& ClearNodeText = lifted::entry::e8211CC50;  // sub_8211CC50
inline constexpr auto& ClearText = lifted::entry::e8211C470;  // sub_8211C470
inline constexpr auto& Ctor = lifted::entry::e8211E1C0;  // sub_8211E1C0
inline constexpr auto& DrawPicture = lifted::entry::e8259ED40;  // sub_8259ED40
inline constexpr auto& DrawPrimitive = lifted::entry::e8259F348;  // sub_8259F348
inline constexpr auto& DrawTexts = lifted::entry::e8211CD90;  // sub_8211CD90
inline constexpr auto& Draw_8211D280 = lifted::entry::e8211D280;  // sub_8211D280
inline constexpr auto& Draw_8259FA00 = lifted::entry::e8259FA00;  // sub_8259FA00
inline constexpr auto& Dtor = lifted::entry::e8211E160;  // sub_8211E160
inline constexpr auto& FindNode = lifted::entry::e8211A578;  // sub_8211A578
inline constexpr auto& Finish = lifted::entry::e8211DE38;  // sub_8211DE38
inline constexpr auto& FinishAll = lifted::entry::e8211DFF8;  // sub_8211DFF8
inline constexpr auto& FinishNamed = lifted::entry::e8211DF20;  // sub_8211DF20
inline constexpr auto& FirstLine = lifted::entry::e8211C3F8;  // sub_8211C3F8
inline constexpr auto& FixUpFile = lifted::entry::e825A0EF0;  // sub_825A0EF0
inline constexpr auto& GetText = lifted::entry::e8211B660;  // sub_8211B660
inline constexpr auto& HideNode = lifted::entry::e8211AC48;  // sub_8211AC48
inline constexpr auto& LineCount = lifted::entry::e8211C680;  // sub_8211C680
inline constexpr auto& Load = lifted::entry::e8211E798;  // sub_8211E798
inline constexpr auto& Loop = lifted::entry::e8211DDD8;  // sub_8211DDD8
inline constexpr auto& LoopLast = lifted::entry::e8211DDE8;  // sub_8211DDE8
inline constexpr auto& LoopNamed = lifted::entry::e8211DDE0;  // sub_8211DDE0
inline constexpr auto& MoveAnimation = lifted::entry::e8211DBE0;  // sub_8211DBE0
inline constexpr auto& NamedAnimationTime = lifted::entry::e8211BB18;  // sub_8211BB18
inline constexpr auto& OnLoaded = lifted::entry::e8211E500;  // sub_8211E500
inline constexpr auto& Play = lifted::entry::e8211D978;  // sub_8211D978
inline constexpr auto& PlayNamed = lifted::entry::e8211DAC8;  // sub_8211DAC8
inline constexpr auto& ResetNode = lifted::entry::e8211A1E0;  // sub_8211A1E0
inline constexpr auto& ResetNodeFields = lifted::f8211A130;  // sub_8211A130
inline constexpr auto& ResetNodeTextScroll = lifted::entry::e8211CD28;  // sub_8211CD28
inline constexpr auto& ResetPose = lifted::entry::e8211ABA8;  // sub_8211ABA8
inline constexpr auto& RestorePose = lifted::entry::e82119B80;  // sub_82119B80
inline constexpr auto& RevealText = lifted::entry::e8211D880;  // sub_8211D880
inline constexpr auto& Run = lifted::entry::e8211E1B8;  // sub_8211E1B8
inline constexpr auto& ScrollNodeText = lifted::entry::e8211CCC0;  // sub_8211CCC0
inline constexpr auto& ScrollText = lifted::entry::e8211C570;  // sub_8211C570
inline constexpr auto& SetCharText = lifted::f8211C2C8;  // sub_8211C2C8
inline constexpr auto& SetChildNameText = lifted::entry::e8211D930;  // sub_8211D930
inline constexpr auto& SetFirstLine = lifted::entry::e8211C370;  // sub_8211C370
inline constexpr auto& SetNodeCharText = lifted::entry::e8211C8A0;  // sub_8211C8A0
inline constexpr auto& SetNodeColor = lifted::entry::e8211A5B8;  // sub_8211A5B8
inline constexpr auto& SetNodeNumberText = lifted::entry::e8211CA18;  // sub_8211CA18
inline constexpr auto& SetNodeText = lifted::entry::e8211C7E8;  // sub_8211C7E8
inline constexpr auto& SetNodeTextV = lifted::entry::e8211CBD8;  // sub_8211CBD8
inline constexpr auto& SetNodeTexture = lifted::entry::e8211A708;  // sub_8211A708
inline constexpr auto& SetNumberText = lifted::entry::e8211C740;  // sub_8211C740
inline constexpr auto& SetPictureTexture = lifted::entry::e8211AAA8;  // sub_8211AAA8
inline constexpr auto& SetRenderer = lifted::entry::e8259D798;  // sub_8259D798
inline constexpr auto& SetText = lifted::entry::e8211C1F8;  // sub_8211C1F8
inline constexpr auto& ShowNode = lifted::entry::e8211AC00;  // sub_8211AC00
inline constexpr auto& SwapTexture = lifted::entry::e8211AB90;  // sub_8211AB90
inline constexpr auto& TextString = lifted::entry::e8211B720;  // sub_8211B720
inline constexpr auto& Unloop = lifted::entry::e8211DE28;  // sub_8211DE28
inline constexpr auto& UnloopNamed = lifted::entry::e8211DE30;  // sub_8211DE30
inline constexpr auto& Update = lifted::entry::e8211D2F8;  // sub_8211D2F8
}  // namespace game::UiMovie

namespace game::UiMovies {
inline constexpr auto& Delete = lifted::entry::e8211E258;  // sub_8211E258
inline constexpr auto& Find = lifted::entry::e8211B3F0;  // sub_8211B3F0
inline constexpr auto& IsLoaded = lifted::entry::e8211BE98;  // sub_8211BE98
inline constexpr auto& Open = lifted::entry::e8211E808;  // sub_8211E808
}  // namespace game::UiMovies

namespace game::UiNode {
inline constexpr auto& CopyFields = lifted::f82119EA8;  // sub_82119EA8
}  // namespace game::UiNode

namespace game::Unlock {
inline constexpr auto& State = lifted::entry::e82142EC0;  // sub_82142EC0
}  // namespace game::Unlock

namespace game::UserInfo {
inline constexpr auto& Close = lifted::entry::e8220ABB8;  // sub_8220ABB8
inline constexpr auto& Closing = lifted::entry::e8220AC60;  // sub_8220AC60
inline constexpr auto& Create = lifted::entry::e8220AA50;  // sub_8220AA50
inline constexpr auto& Destroy = lifted::entry::e8220A9D8;  // sub_8220A9D8
inline constexpr auto& InitStates = lifted::entry::e8220B408;  // sub_8220B408
inline constexpr auto& MenuPage = lifted::entry::e8220B0D8;  // sub_8220B0D8
inline constexpr auto& OpenMovie = lifted::entry::e8220AA40;  // sub_8220AA40
inline constexpr auto& Opening = lifted::entry::e8220B1C8;  // sub_8220B1C8
inline constexpr auto& Reenter = lifted::entry::e8220AB60;  // sub_8220AB60
inline constexpr auto& Update = lifted::entry::e8220B2C0;  // sub_8220B2C0
inline constexpr auto& UpdatePage = lifted::entry::e8220B260;  // sub_8220B260
inline constexpr auto& WaitForLoad = lifted::entry::e8220B070;  // sub_8220B070
}  // namespace game::UserInfo

namespace game::Users {
inline constexpr auto& Create = lifted::f82099A08;  // sub_82099A08
inline constexpr auto& SignedIn = lifted::entry::e820996D8;  // sub_820996D8
}  // namespace game::Users

namespace game::UvColumn {
inline constexpr auto& Copy = lifted::entry::e82130088;  // sub_82130088
inline constexpr auto& Ctor = lifted::entry::e82130140;  // sub_82130140
inline constexpr auto& Dtor = lifted::entry::e82131EE0;  // sub_82131EE0
inline constexpr auto& InsertUnique = lifted::entry::e82131CB8;  // sub_82131CB8
inline constexpr auto& LowerBound = lifted::entry::e8212FFC8;  // sub_8212FFC8
}  // namespace game::UvColumn

namespace game::UvMap {
inline constexpr auto& AllocateNodes = lifted::entry::e8212FC50;  // sub_8212FC50
inline constexpr auto& Ctor = lifted::entry::e82130198;  // sub_82130198
inline constexpr auto& Erase = lifted::entry::e82132BD8;  // sub_82132BD8
inline constexpr auto& InsertUnique = lifted::entry::e821321F8;  // sub_821321F8
inline constexpr auto& LowerBound = lifted::entry::e82130028;  // sub_82130028
}  // namespace game::UvMap

namespace game::V16U16Codec {
inline constexpr auto& DecodeRow = lifted::f823F3BF0;  // sub_823F3BF0
inline constexpr auto& EncodeRow = lifted::f823F0080;  // sub_823F0080
}  // namespace game::V16U16Codec

namespace game::V8U8Codec {
inline constexpr auto& DecodeRow = lifted::f823F36C8;  // sub_823F36C8
inline constexpr auto& EncodeRow = lifted::f823EF198;  // sub_823EF198
}  // namespace game::V8U8Codec

namespace game::Vec3 {
inline constexpr auto& Add = lifted::entry::e82294028;  // sub_82294028
inline constexpr auto& AddTo = lifted::entry::e82294320;  // sub_82294320
inline constexpr auto& CrossW1 = lifted::entry::e82294108;  // sub_82294108
inline constexpr auto& Difference = lifted::entry::e822942E0;  // sub_822942E0
inline constexpr auto& Direction = lifted::entry::e82294200;  // sub_82294200
inline constexpr auto& Dot = lifted::entry::e82294170;  // sub_82294170
inline constexpr auto& DotOf = lifted::entry::e82294608;  // sub_82294608
inline constexpr auto& Length = lifted::entry::e82294270;  // sub_82294270
inline constexpr auto& Lerp = lifted::entry::e82294568;  // sub_82294568
inline constexpr auto& Normalize = lifted::entry::e822941B0;  // sub_822941B0
inline constexpr auto& Sub = lifted::entry::e82294068;  // sub_82294068
inline constexpr auto& Sum = lifted::entry::e822942A0;  // sub_822942A0
inline constexpr auto& SwapXZ = lifted::entry::e820AB688;  // sub_820AB688
}  // namespace game::Vec3

namespace game::Vec4 {
inline constexpr auto& Copy = lifted::entry::e822940E0;  // sub_822940E0
inline constexpr auto& DivideBy = lifted::entry::e82294348;  // sub_82294348
inline constexpr auto& Equal = lifted::entry::e82123300;  // sub_82123300
inline constexpr auto& ScaleTo = lifted::entry::e822940A8;  // sub_822940A8
inline constexpr auto& Scaled = lifted::entry::e82294430;  // sub_82294430
inline constexpr auto& Transform = lifted::entry::e820AB560;  // sub_820AB560
}  // namespace game::Vec4

namespace game::Vector {
inline constexpr auto& AngleBetween = lifted::f821266C8;  // sub_821266C8
}  // namespace game::Vector

namespace game::Vector16 {
inline constexpr auto& Size = lifted::entry::e825971E8;  // sub_825971E8
}  // namespace game::Vector16

namespace game::Vehicle {
inline constexpr auto& AxleReactionTorque = lifted::entry::e820B3130;  // sub_820B3130
inline constexpr auto& Ctor = lifted::entry::e820B42C8;  // sub_820B42C8
inline constexpr auto& DestroyPhysics = lifted::entry::e820B0E70;  // sub_820B0E70
inline constexpr auto& EngineTorque = lifted::entry::e820B00C8;  // sub_820B00C8
inline constexpr auto& Get = lifted::entry::e820C0468;  // sub_820C0468
inline constexpr auto& GetDistancePerFrame = lifted::entry::e820C0150;  // sub_820C0150
inline constexpr auto& GetSpeedKmh = lifted::entry::e820C0128;  // sub_820C0128
inline constexpr auto& Identity = lifted::entry::e820C0168;  // sub_820C0168
inline constexpr auto& LinkTrackPositions = lifted::entry::e820C1D08;  // sub_820C1D08
inline constexpr auto& MaxWheelSlip = lifted::entry::e820C0290;  // sub_820C0290
inline constexpr auto& PhysicsStep = lifted::entry::e820B6498;  // sub_820B6498
inline constexpr auto& PlaceOnRoute = lifted::entry::e820C3608;  // sub_820C3608
inline constexpr auto& RampHandbrake = lifted::entry::e820B04C0;  // sub_820B04C0
inline constexpr auto& RampReverseThrottle = lifted::entry::e820B0478;  // sub_820B0478
inline constexpr auto& Refresh = lifted::entry::e820C0118;  // sub_820C0118
inline constexpr auto& RefreshFromBody = lifted::entry::e820B62E8;  // sub_820B62E8
inline constexpr auto& RemoveFromWorld = lifted::entry::e820C20E0;  // sub_820C20E0
inline constexpr auto& ResetForCar = lifted::entry::e820C33B8;  // sub_820C33B8
inline constexpr auto& ResetPhysics = lifted::entry::e820B5B08;  // sub_820B5B08
inline constexpr auto& SetBrake = lifted::entry::e820B3E08;  // sub_820B3E08
inline constexpr auto& SetGearValues = lifted::entry::e820B3C90;  // sub_820B3C90
inline constexpr auto& SetHandbrake = lifted::entry::e820B3E38;  // sub_820B3E38
inline constexpr auto& SetSteering = lifted::entry::e820B3D10;  // sub_820B3D10
inline constexpr auto& SetThrottle = lifted::entry::e820B3E68;  // sub_820B3E68
inline constexpr auto& SetUp = lifted::entry::e820C3150;  // sub_820C3150
inline constexpr auto& SetUpPhysics = lifted::entry::e820B5670;  // sub_820B5670
inline constexpr auto& SetValue2388 = lifted::entry::e820B3C20;  // sub_820B3C20
inline constexpr auto& SetupFromCar = lifted::entry::e820C27E8;  // sub_820C27E8
inline constexpr auto& SlipWheelValue = lifted::entry::e820C0310;  // sub_820C0310
inline constexpr auto& Speed = lifted::entry::e820C0148;  // sub_820C0148
inline constexpr auto& SteerTowards = lifted::entry::e820B5FD8;  // sub_820B5FD8
inline constexpr auto& SurfaceValue = lifted::entry::e820B0508;  // sub_820B0508
inline constexpr auto& SurfaceValue12 = lifted::entry::e820B05B8;  // sub_820B05B8
inline constexpr auto& SurfaceValue16 = lifted::entry::e820B0610;  // sub_820B0610
inline constexpr auto& SurfaceValue20 = lifted::entry::e820B0560;  // sub_820B0560
inline constexpr auto& SyncFromBody = lifted::entry::e820B13C8;  // sub_820B13C8
inline constexpr auto& Tick = lifted::entry::e820C3020;  // sub_820C3020
inline constexpr auto& UpdateDrivenAxle = lifted::entry::e820B31D0;  // sub_820B31D0
inline constexpr auto& UpdateFreeAxle = lifted::entry::e820B3010;  // sub_820B3010
inline constexpr auto& UpdateGearbox = lifted::entry::e820B4CF0;  // sub_820B4CF0
inline constexpr auto& UpdateWheelSuspension = lifted::entry::e820B5588;  // sub_820B5588
inline constexpr auto& WheelSlip = lifted::entry::e820C0180;  // sub_820C0180
}  // namespace game::Vehicle

namespace game::VehicleStatus {
inline constexpr auto& Update = lifted::entry::e820C23E8;  // sub_820C23E8
}  // namespace game::VehicleStatus

namespace game::Vehicles {
inline constexpr auto& Create = lifted::entry::e820C3BD0;  // sub_820C3BD0
inline constexpr auto& Destroy = lifted::entry::e820C3B78;  // sub_820C3B78
}  // namespace game::Vehicles

namespace game::VertexBuffer {
inline constexpr auto& Lock = lifted::entry::e822A68F8;  // sub_822A68F8
inline constexpr auto& Unlock = lifted::entry::e822A6958;  // sub_822A6958
}  // namespace game::VertexBuffer

namespace game::VertexShader {
inline constexpr auto& Create = lifted::entry::e822AA018;  // sub_822AA018
}  // namespace game::VertexShader

namespace game::VideoOutput {
inline constexpr auto& Ctor = lifted::entry::e8258DB18;  // sub_8258DB18
inline constexpr auto& LockDevice = lifted::entry::e8258D8C8;  // sub_8258D8C8
inline constexpr auto& OnVblank = lifted::f8258D4D8;  // sub_8258D4D8
inline constexpr auto& Present = lifted::f8258DC58;  // sub_8258DC58
inline constexpr auto& ResolveFrontBuffer = lifted::entry::e8258D998;  // sub_8258D998
inline constexpr auto& SetUpDisplay = lifted::entry::e8258D568;  // sub_8258D568
inline constexpr auto& UnbindAll = lifted::entry::e8258D9F8;  // sub_8258D9F8
inline constexpr auto& UnlockDevice = lifted::entry::e8258D908;  // sub_8258D908
inline constexpr auto& WaitForVblank = lifted::entry::e8258DAA8;  // sub_8258DAA8
}  // namespace game::VideoOutput

namespace game::ViewCamera1 {
inline constexpr auto& Ctor = lifted::entry::e82154A48;  // sub_82154A48
inline constexpr auto& Update = lifted::entry::e82153A50;  // sub_82153A50
}  // namespace game::ViewCamera1

namespace game::ViewCamera2 {
inline constexpr auto& Ctor = lifted::entry::e82154D38;  // sub_82154D38
inline constexpr auto& Update = lifted::entry::e82153E58;  // sub_82153E58
}  // namespace game::ViewCamera2

namespace game::ViewDesc {
inline constexpr auto& Clear = lifted::entry::e82126860;  // sub_82126860
}  // namespace game::ViewDesc

namespace game::Viewport {
inline constexpr auto& Set = lifted::entry::e820AAE48;  // sub_820AAE48
inline constexpr auto& SetRoll = lifted::entry::e820AA2D0;  // sub_820AA2D0
inline constexpr auto& SetZoom = lifted::entry::e820AAE90;  // sub_820AAE90
inline constexpr auto& Update = lifted::entry::e820AAE40;  // sub_820AAE40
}  // namespace game::Viewport

namespace game::Viewports {
inline constexpr auto& Init = lifted::entry::e820AB358;  // sub_820AB358
inline constexpr auto& Release = lifted::entry::e820AA430;  // sub_820AA430
}  // namespace game::Viewports

namespace game::Vinyl {
inline constexpr auto& DrawDecal = lifted::entry::e820EE638;  // sub_820EE638
}  // namespace game::Vinyl

namespace game::VolumeMeter {
inline constexpr auto& Create = lifted::entry::e825A6460;  // sub_825A6460
inline constexpr auto& GetInfo = lifted::entry::e825A5778;  // sub_825A5778
inline constexpr auto& Init = lifted::entry::e825A6138;  // sub_825A6138
inline constexpr auto& Process = lifted::entry::e825A6210;  // sub_825A6210
inline constexpr auto& Size = lifted::entry::e825A60E0;  // sub_825A60E0
}  // namespace game::VolumeMeter

namespace game::WallQuery {
inline constexpr auto& GatherCells = lifted::entry::e820FC188;  // sub_820FC188
inline constexpr auto& TestCell = lifted::entry::e820FDC18;  // sub_820FDC18
}  // namespace game::WallQuery

namespace game::WaveBankHolder {
inline constexpr auto& Ctor = lifted::entry::e8259D418;  // sub_8259D418
}  // namespace game::WaveBankHolder

namespace game::WaveBankView {
inline constexpr auto& Init = lifted::entry::e8210C9D8;  // sub_8210C9D8
}  // namespace game::WaveBankView

namespace game::Wheel {
inline constexpr auto& Copy = lifted::entry::e820B0A50;  // sub_820B0A50
inline constexpr auto& Swap = lifted::entry::e820CB588;  // sub_820CB588
}  // namespace game::Wheel

namespace game::WheelPoints {
inline constexpr auto& DrawBrakes = lifted::entry::e820EB388;  // sub_820EB388
inline constexpr auto& DrawBrakesMorphed = lifted::entry::e820EBEB8;  // sub_820EBEB8
inline constexpr auto& DrawRimA = lifted::entry::e820E9A28;  // sub_820E9A28
inline constexpr auto& DrawRimAMorphed = lifted::entry::e820EA148;  // sub_820EA148
inline constexpr auto& DrawRimB = lifted::entry::e820EA950;  // sub_820EA950
inline constexpr auto& DrawTire = lifted::entry::e820E8AE8;  // sub_820E8AE8
inline constexpr auto& DrawTireMorphed = lifted::entry::e820E9318;  // sub_820E9318
inline constexpr auto& SetUp = lifted::entry::e820E8668;  // sub_820E8668
}  // namespace game::WheelPoints

namespace game::WheelRecord2 {
inline constexpr auto& Copy = lifted::entry::e820B0CC0;  // sub_820B0CC0
}  // namespace game::WheelRecord2

namespace game::Wheels {
inline constexpr auto& ApplyPalette = lifted::entry::e820D92A0;  // sub_820D92A0
inline constexpr auto& DeletingDestructor = lifted::entry::e820D95E8;  // sub_820D95E8
inline constexpr auto& SetRecord = lifted::entry::e820D8EC0;  // sub_820D8EC0
inline constexpr auto& SetTireTextures = lifted::entry::e820D8F18;  // sub_820D8F18
inline constexpr auto& SetUp = lifted::entry::e820D9110;  // sub_820D9110
}  // namespace game::Wheels

namespace game::WordVector {
inline constexpr auto& Fill = lifted::entry::e821194A0;  // sub_821194A0
}  // namespace game::WordVector

namespace game::WorkItem {
inline constexpr auto& Call = lifted::entry::e820ACA30;  // sub_820ACA30
inline constexpr auto& Dtor = lifted::entry::e82090E40;  // sub_82090E40
inline constexpr auto& Finished = lifted::entry::e82090DF8;  // sub_82090DF8
inline constexpr auto& IsSkipped = lifted::entry::e82090CB8;  // sub_82090CB8
inline constexpr auto& Posted = lifted::entry::e82090DD0;  // sub_82090DD0
inline constexpr auto& Run = lifted::entry::e82090E18;  // sub_82090E18
inline constexpr auto& Skip = lifted::entry::e82090E08;  // sub_82090E08
inline constexpr auto& Started = lifted::entry::e82090DE8;  // sub_82090DE8
}  // namespace game::WorkItem

namespace game::WorkerPool {
inline constexpr auto& Create = lifted::f82095620;  // sub_82095620
}  // namespace game::WorkerPool

namespace game::X1R5G5B5Codec {
inline constexpr auto& DecodeRow = lifted::f823F2948;  // sub_823F2948
inline constexpr auto& EncodeRow = lifted::f823EC0A0;  // sub_823EC0A0
}  // namespace game::X1R5G5B5Codec

namespace game::X3DAudio {
inline constexpr auto& Delay = lifted::f82481708;  // sub_82481708
inline constexpr auto& EmitterAngle = lifted::f82480780;  // sub_82480780
inline constexpr auto& EvaluateCurve = lifted::entry::e82480818;  // sub_82480818
inline constexpr auto& Pan = lifted::entry::e82480F80;  // sub_82480F80
}  // namespace game::X3DAudio

namespace game::X4R4G4B4Codec {
inline constexpr auto& DecodeRow = lifted::f823F3410;  // sub_823F3410
inline constexpr auto& EncodeRow = lifted::f823ECF68;  // sub_823ECF68
}  // namespace game::X4R4G4B4Codec

namespace game::X8B8G8R8Codec {
inline constexpr auto& DecodeRow = lifted::f823F2EF8;  // sub_823F2EF8
inline constexpr auto& EncodeRow = lifted::f823EDC18;  // sub_823EDC18
}  // namespace game::X8B8G8R8Codec

namespace game::X8L8V8U8Codec {
inline constexpr auto& DecodeRow = lifted::f823F3948;  // sub_823F3948
inline constexpr auto& EncodeRow = lifted::f823EF850;  // sub_823EF850
}  // namespace game::X8L8V8U8Codec

namespace game::X8R8G8B8Codec {
inline constexpr auto& DecodeRow = lifted::f823F2730;  // sub_823F2730
inline constexpr auto& EncodeRow = lifted::f823EB910;  // sub_823EB910
}  // namespace game::X8R8G8B8Codec

namespace game::XACTComplexSound {
inline constexpr auto& Ctor = lifted::f825CC810;  // sub_825CC810
inline constexpr auto& DeletingDestructor = lifted::f825CC958;  // sub_825CC958
inline constexpr auto& GetTrackRecord = lifted::entry::e825CC9C8;  // sub_825CC9C8
inline constexpr auto& Prepare = lifted::f825CCA88;  // sub_825CCA88
}  // namespace game::XACTComplexSound

namespace game::XACTCue {
inline constexpr auto& AddListener = lifted::entry::e825C6140;  // sub_825C6140
inline constexpr auto& AudioUpdate = lifted::f825C4E88;  // sub_825C4E88
inline constexpr auto& CreateSound = lifted::f825C5A70;  // sub_825C5A70
inline constexpr auto& Ctor = lifted::entry::e825C67E8;  // sub_825C67E8
inline constexpr auto& DeletingDestructorThunk = lifted::f825C6A18;  // sub_825C6A18
inline constexpr auto& Destroy = lifted::f825C4B38;  // sub_825C4B38
inline constexpr auto& DetachSound = lifted::entry::e825C4FC0;  // sub_825C4FC0
inline constexpr auto& Dtor = lifted::f825C7570;  // sub_825C7570
inline constexpr auto& FindVariable = lifted::entry::e825C5758;  // sub_825C5758
inline constexpr auto& GetProperties = lifted::f825C5790;  // sub_825C5790
inline constexpr auto& GetState = lifted::f825C4E30;  // sub_825C4E30
inline constexpr auto& GetStateInternal = lifted::entry::e825C4D90;  // sub_825C4D90
inline constexpr auto& MakeInstanceVariable = lifted::entry::e825C5E28;  // sub_825C5E28
inline constexpr auto& Notify = lifted::f825C4A98;  // sub_825C4A98
inline constexpr auto& OnPlay = lifted::f825C5680;  // sub_825C5680
inline constexpr auto& OnStop = lifted::f825C5628;  // sub_825C5628
inline constexpr auto& Pause = lifted::f825C4CF8;  // sub_825C4CF8
inline constexpr auto& Play = lifted::f825C4B98;  // sub_825C4B98
inline constexpr auto& Prepare = lifted::f825CCF10;  // sub_825CCF10
inline constexpr auto& PrepareSound = lifted::f825C5C80;  // sub_825C5C80
inline constexpr auto& RemoveListener = lifted::entry::e825C6040;  // sub_825C6040
inline constexpr auto& Stop = lifted::f825C4C20;  // sub_825C4C20
inline constexpr auto& Update = lifted::f825C4F00;  // sub_825C4F00
inline constexpr auto& UpdateVolume = lifted::f825C5610;  // sub_825C5610
}  // namespace game::XACTCue

namespace game::XACTCueBase {
inline constexpr auto& Dtor = lifted::f825C6148;  // sub_825C6148
}  // namespace game::XACTCueBase

namespace game::XACTEngine {
inline constexpr auto& AddCue = lifted::entry::e825C1518;  // sub_825C1518
inline constexpr auto& AudioCallback = lifted::f825C0248;  // sub_825C0248
inline constexpr auto& AudioCallbackThunk = lifted::f825C2BA8;  // sub_825C2BA8
inline constexpr auto& CheckNotInCallback = lifted::f825BFEA0;  // sub_825BFEA0
inline constexpr auto& CreateInMemoryWaveBank = lifted::f825C0C78;  // sub_825C0C78
inline constexpr auto& CreateSoundBank = lifted::f825C0B60;  // sub_825C0B60
inline constexpr auto& DoWork = lifted::f825C1990;  // sub_825C1990
inline constexpr auto& GetCategory = lifted::f825C00F0;  // sub_825C00F0
inline constexpr auto& Initialize = lifted::f825BFF38;  // sub_825BFF38
inline constexpr auto& Pause = lifted::f825C1090;  // sub_825C1090
inline constexpr auto& SendNotification = lifted::entry::e825C1388;  // sub_825C1388
inline constexpr auto& SetVolume = lifted::f825C0178;  // sub_825C0178
}  // namespace game::XACTEngine

namespace game::XACTEvent {
inline constexpr auto& Cancel = lifted::f825D1830;  // sub_825D1830
inline constexpr auto& Dtor = lifted::f825D17E0;  // sub_825D17E0
inline constexpr auto& GetStartTime = lifted::f825D18F0;  // sub_825D18F0
inline constexpr auto& Prepare = lifted::f825D17D0;  // sub_825D17D0
inline constexpr auto& ResetTimes = lifted::f825D0E78;  // sub_825D0E78
inline constexpr auto& Schedule = lifted::entry::e825D1918;  // sub_825D1918
inline constexpr auto& SetQueued = lifted::f825D17B0;  // sub_825D17B0
}  // namespace game::XACTEvent

namespace game::XACTFader {
inline constexpr auto& Ctor = lifted::entry::e825CC0A0;  // sub_825CC0A0
inline constexpr auto& Dtor = lifted::entry::e825CC0D0;  // sub_825CC0D0
inline constexpr auto& Step = lifted::entry::e825CC1D0;  // sub_825CC1D0
}  // namespace game::XACTFader

namespace game::XACTGlobalSettings {
inline constexpr auto& ApplyInstanceLimit = lifted::f825C4138;  // sub_825C4138
inline constexpr auto& Ctor = lifted::entry::e825C21B0;  // sub_825C21B0
inline constexpr auto& FindCategory = lifted::f825C38A8;  // sub_825C38A8
inline constexpr auto& GetBuffer = lifted::entry::e825C2200;  // sub_825C2200
inline constexpr auto& GetCategoryRecord = lifted::entry::e825C23C8;  // sub_825C23C8
inline constexpr auto& GetInstanceBehavior = lifted::entry::e825C24B0;  // sub_825C24B0
inline constexpr auto& GetMaxInstances = lifted::entry::e825C2480;  // sub_825C2480
inline constexpr auto& GetNameBuckets = lifted::entry::e825C2208;  // sub_825C2208
inline constexpr auto& GetNameEntry = lifted::entry::e825C2268;  // sub_825C2268
inline constexpr auto& GetNameEntryIndex = lifted::entry::e825C2548;  // sub_825C2548
inline constexpr auto& GetParentCategory = lifted::entry::e825C24E8;  // sub_825C24E8
inline constexpr auto& GetPointer = lifted::entry::e825C22C8;  // sub_825C22C8
inline constexpr auto& GetVariableRecord = lifted::entry::e825C2428;  // sub_825C2428
inline constexpr auto& GetVolumeByte = lifted::entry::e825C2518;  // sub_825C2518
inline constexpr auto& IsInCategory = lifted::entry::e825C3548;  // sub_825C3548
inline constexpr auto& Load = lifted::entry::e825C26E0;  // sub_825C26E0
inline constexpr auto& SetCategoryVolume = lifted::entry::e825C4070;  // sub_825C4070
inline constexpr auto& SetUpDspPreset = lifted::entry::e825C27C0;  // sub_825C27C0
inline constexpr auto& SetUpRpcs = lifted::entry::e825C2910;  // sub_825C2910
}  // namespace game::XACTGlobalSettings

namespace game::XACTInMemoryWave {
inline constexpr auto& Ctor = lifted::entry::e825D37A8;  // sub_825D37A8
inline constexpr auto& Dtor = lifted::entry::e825D3820;  // sub_825D3820
inline constexpr auto& Play = lifted::entry::e825D3938;  // sub_825D3938
inline constexpr auto& Prepare = lifted::entry::e825D3B10;  // sub_825D3B10
inline constexpr auto& Stop = lifted::entry::e825D3A28;  // sub_825D3A28
inline constexpr auto& Update = lifted::entry::e825D3890;  // sub_825D3890
}  // namespace game::XACTInMemoryWave

namespace game::XACTNodePool {
inline constexpr auto& Alloc = lifted::entry::e825C2CC8;  // sub_825C2CC8
inline constexpr auto& Ctor = lifted::entry::e825C2060;  // sub_825C2060
inline constexpr auto& Free = lifted::entry::e825C2C60;  // sub_825C2C60
inline constexpr auto& FreeChunks = lifted::entry::e825C1F30;  // sub_825C1F30
}  // namespace game::XACTNodePool

namespace game::XACTPlayWaveEvent {
inline constexpr auto& ApplyVariation = lifted::entry::e825D7558;  // sub_825D7558
inline constexpr auto& Dtor = lifted::entry::e825D8660;  // sub_825D8660
inline constexpr auto& Fire = lifted::entry::e825D8378;  // sub_825D8378
inline constexpr auto& ForgetWave = lifted::entry::e825D8360;  // sub_825D8360
inline constexpr auto& GetDuration = lifted::entry::e825D8870;  // sub_825D8870
inline constexpr auto& GetLoopCount = lifted::entry::e825D8358;  // sub_825D8358
inline constexpr auto& GetTimeLeft = lifted::f825D85C8;  // sub_825D85C8
inline constexpr auto& GetVariationRecord = lifted::entry::e825D8920;  // sub_825D8920
inline constexpr auto& GetWaveBank = lifted::entry::e825D84C0;  // sub_825D84C0
inline constexpr auto& IsBusy = lifted::entry::e825D81E8;  // sub_825D81E8
inline constexpr auto& IsPlaying = lifted::entry::e825D7708;  // sub_825D7708
inline constexpr auto& IsPrepared = lifted::entry::e825D76A0;  // sub_825D76A0
inline constexpr auto& Pause = lifted::entry::e825D8268;  // sub_825D8268
inline constexpr auto& Prepare = lifted::entry::e825D86F8;  // sub_825D86F8
inline constexpr auto& SetChannelMatrix = lifted::f825D7480;  // sub_825D7480
inline constexpr auto& SetMainMatrix = lifted::f825D7418;  // sub_825D7418
inline constexpr auto& SetOutputMatrices = lifted::f825D8118;  // sub_825D8118
inline constexpr auto& SetOutputVoices = lifted::f825D80B0;  // sub_825D80B0
inline constexpr auto& SetOutputs = lifted::f825D8180;  // sub_825D8180
inline constexpr auto& SetPitch = lifted::f825D74F0;  // sub_825D74F0
inline constexpr auto& SetQueued = lifted::entry::e825D8530;  // sub_825D8530
inline constexpr auto& SetReverbSend = lifted::f825D8048;  // sub_825D8048
inline constexpr auto& SetVariation = lifted::f825D1478;  // sub_825D1478
inline constexpr auto& SetVolume = lifted::entry::e825D7FE0;  // sub_825D7FE0
inline constexpr auto& SetWave = lifted::entry::e82483208;  // sub_82483208
inline constexpr auto& Stop = lifted::entry::e825D83F8;  // sub_825D83F8
}  // namespace game::XACTPlayWaveEvent

namespace game::XACTRenderer {
inline constexpr auto& CommitVoices = lifted::f825C3048;  // sub_825C3048
inline constexpr auto& Create = lifted::f825C3110;  // sub_825C3110
inline constexpr auto& CreateReverb = lifted::f825C3398;  // sub_825C3398
inline constexpr auto& CreateVoice = lifted::entry::e825C31D8;  // sub_825C31D8
inline constexpr auto& ReleaseVoice = lifted::f825C2F00;  // sub_825C2F00
inline constexpr auto& StartXAudio = lifted::f825C2BB0;  // sub_825C2BB0
}  // namespace game::XACTRenderer

namespace game::XACTReverb {
inline constexpr auto& Create = lifted::f825CA4B8;  // sub_825CA4B8
inline constexpr auto& Dtor = lifted::f825CA680;  // sub_825CA680
inline constexpr auto& SetParameters = lifted::f825CA560;  // sub_825CA560
}  // namespace game::XACTReverb

namespace game::XACTSimpleSound {
inline constexpr auto& Ctor = lifted::f825CCC40;  // sub_825CCC40
inline constexpr auto& DeletingDestructor = lifted::f825CCCF0;  // sub_825CCCF0
inline constexpr auto& Prepare = lifted::f825CCDB0;  // sub_825CCDB0
}  // namespace game::XACTSimpleSound

namespace game::XACTSound {
inline constexpr auto& AnyTrackBusy = lifted::f825CAA58;  // sub_825CAA58
inline constexpr auto& AudioUpdate = lifted::f825CBD10;  // sub_825CBD10
inline constexpr auto& CheckInstanceLimit = lifted::f825CAAC0;  // sub_825CAAC0
inline constexpr auto& Ctor = lifted::f825CB958;  // sub_825CB958
inline constexpr auto& Dtor = lifted::f825CAE90;  // sub_825CAE90
inline constexpr auto& GetVolume = lifted::entry::e825CB5E8;  // sub_825CB5E8
inline constexpr auto& OnCueGone = lifted::f825CAFD8;  // sub_825CAFD8
inline constexpr auto& Pause = lifted::f825CA6F8;  // sub_825CA6F8
inline constexpr auto& Play = lifted::f825CBB28;  // sub_825CBB28
inline constexpr auto& SetState = lifted::f825CB1A8;  // sub_825CB1A8
inline constexpr auto& SetTrackParameter = lifted::entry::e825CA980;  // sub_825CA980
inline constexpr auto& SetUpRpcs = lifted::entry::e825CB470;  // sub_825CB470
inline constexpr auto& Stop = lifted::f825CBC58;  // sub_825CBC58
inline constexpr auto& Update = lifted::f825CB040;  // sub_825CB040
inline constexpr auto& UpdateVolume = lifted::f825CA860;  // sub_825CA860
}  // namespace game::XACTSound

namespace game::XACTSoundBank {
inline constexpr auto& AddCueInstance = lifted::f825C7510;  // sub_825C7510
inline constexpr auto& AddListener = lifted::entry::e825C7858;  // sub_825C7858
inline constexpr auto& Ctor = lifted::entry::e825C79B8;  // sub_825C79B8
inline constexpr auto& DeletingDestructorThunk = lifted::f825C7A30;  // sub_825C7A30
inline constexpr auto& Destroy = lifted::f825C6B50;  // sub_825C6B50
inline constexpr auto& Dtor_825C7DA8 = lifted::f825C7DA8;  // sub_825C7DA8
inline constexpr auto& Dtor_825C82B8 = lifted::f825C82B8;  // sub_825C82B8
inline constexpr auto& FirstEvent = lifted::entry::e825C7F30;  // sub_825C7F30
inline constexpr auto& GetCueCount = lifted::entry::e825C6928;  // sub_825C6928
inline constexpr auto& GetCueRecord = lifted::entry::e825C8230;  // sub_825C8230
inline constexpr auto& GetHeader = lifted::entry::e825C7AA8;  // sub_825C7AA8
inline constexpr auto& GetPointer = lifted::entry::e825C7C90;  // sub_825C7C90
inline constexpr auto& GetState = lifted::f825C7070;  // sub_825C7070
inline constexpr auto& GetWaveBank = lifted::entry::e825C7218;  // sub_825C7218
inline constexpr auto& GetWaveBankNames = lifted::entry::e825C7B70;  // sub_825C7B70
inline constexpr auto& Load = lifted::entry::e825C7E40;  // sub_825C7E40
inline constexpr auto& NextEvent = lifted::entry::e825C7F98;  // sub_825C7F98
inline constexpr auto& Notify = lifted::f825C6988;  // sub_825C6988
inline constexpr auto& OnWaveBankGone = lifted::entry::e825C7798;  // sub_825C7798
inline constexpr auto& Prepare = lifted::f825C6C10;  // sub_825C6C10
inline constexpr auto& ReleaseWaveBanks = lifted::entry::e825C6A20;  // sub_825C6A20
inline constexpr auto& RemoveCueInstance = lifted::f825C7540;  // sub_825C7540
inline constexpr auto& RemoveListener = lifted::entry::e825C7758;  // sub_825C7758
}  // namespace game::XACTSoundBank

namespace game::XACTSoundBankBase {
inline constexpr auto& Dtor = lifted::f825C7860;  // sub_825C7860
}  // namespace game::XACTSoundBankBase

namespace game::XACTStopEvent {
inline constexpr auto& Ctor = lifted::f825D8950;  // sub_825D8950
inline constexpr auto& Dtor = lifted::f825D8990;  // sub_825D8990
inline constexpr auto& Fire = lifted::f825D89F8;  // sub_825D89F8
}  // namespace game::XACTStopEvent

namespace game::XACTStreamEntry {
inline constexpr auto& Ctor = lifted::f825D52A8;  // sub_825D52A8
inline constexpr auto& Dtor = lifted::f825D5350;  // sub_825D5350
inline constexpr auto& IsReading = lifted::f825D5088;  // sub_825D5088
inline constexpr auto& Release = lifted::f825D50E8;  // sub_825D50E8
inline constexpr auto& SetUp = lifted::f825D50F8;  // sub_825D50F8
}  // namespace game::XACTStreamEntry

namespace game::XACTStreamingWave {
inline constexpr auto& Ctor = lifted::f825D3F28;  // sub_825D3F28
inline constexpr auto& Dtor_825D4020 = lifted::f825D4020;  // sub_825D4020
inline constexpr auto& Dtor_825D4690 = lifted::f825D4690;  // sub_825D4690
inline constexpr auto& IsBusy = lifted::f825D4530;  // sub_825D4530
inline constexpr auto& Play = lifted::f825D5058;  // sub_825D5058
inline constexpr auto& Prepare = lifted::f825D4B78;  // sub_825D4B78
inline constexpr auto& Start = lifted::f825D45D0;  // sub_825D45D0
inline constexpr auto& Stop = lifted::f825D4300;  // sub_825D4300
inline constexpr auto& Update = lifted::f825D46E8;  // sub_825D46E8
}  // namespace game::XACTStreamingWave

namespace game::XACTTrack {
inline constexpr auto& AudioUpdate = lifted::entry::e825D0D88;  // sub_825D0D88
inline constexpr auto& CreateNextInstance = lifted::f825D0B08;  // sub_825D0B08
inline constexpr auto& CreateRpcs = lifted::entry::e825D06B8;  // sub_825D06B8
inline constexpr auto& Ctor = lifted::entry::e825D0CE0;  // sub_825D0CE0
inline constexpr auto& Dtor = lifted::entry::e825D0978;  // sub_825D0978
inline constexpr auto& HasReleaseRpc = lifted::f825D05C0;  // sub_825D05C0
inline constexpr auto& IsBusy = lifted::entry::e825D0590;  // sub_825D0590
inline constexpr auto& Pause = lifted::entry::e825D05A8;  // sub_825D05A8
inline constexpr auto& Play = lifted::entry::e825D01D0;  // sub_825D01D0
inline constexpr auto& Prepare = lifted::entry::e825D0AA0;  // sub_825D0AA0
inline constexpr auto& PrepareSimple = lifted::entry::e825D09D0;  // sub_825D09D0
inline constexpr auto& ResetRpc = lifted::f825D0868;  // sub_825D0868
inline constexpr auto& SetChannelMatrix = lifted::f825D0570;  // sub_825D0570
inline constexpr auto& SetMainMatrix = lifted::f825D0558;  // sub_825D0558
inline constexpr auto& SetOutputMatrices = lifted::f825D0528;  // sub_825D0528
inline constexpr auto& SetOutputVoices = lifted::f825D0510;  // sub_825D0510
inline constexpr auto& SetOutputs = lifted::f825D0540;  // sub_825D0540
inline constexpr auto& SetPitch = lifted::f825D0C78;  // sub_825D0C78
inline constexpr auto& SetReverbSend = lifted::f825D0490;  // sub_825D0490
inline constexpr auto& SetRpcPitch = lifted::f825D06A0;  // sub_825D06A0
inline constexpr auto& SetRpcVolume = lifted::f825D0670;  // sub_825D0670
inline constexpr auto& StartRpcs = lifted::entry::e825D0800;  // sub_825D0800
inline constexpr auto& Stop = lifted::entry::e825D01D8;  // sub_825D01D8
inline constexpr auto& SwitchInstance = lifted::f825D0B70;  // sub_825D0B70
inline constexpr auto& Update = lifted::entry::e825D0E30;  // sub_825D0E30
inline constexpr auto& UpdatePitch = lifted::f825D04A8;  // sub_825D04A8
inline constexpr auto& UpdateVolume = lifted::entry::e825D0428;  // sub_825D0428
inline constexpr auto& UsesWaveBank = lifted::f825D03B0;  // sub_825D03B0
}  // namespace game::XACTTrack

namespace game::XACTTrackInstance {
inline constexpr auto& AddHelper = lifted::f825D5DE8;  // sub_825D5DE8
inline constexpr auto& CancelEvents = lifted::entry::e825D5520;  // sub_825D5520
inline constexpr auto& Ctor = lifted::entry::e825D57D0;  // sub_825D57D0
inline constexpr auto& Dtor = lifted::entry::e825D5E58;  // sub_825D5E58
inline constexpr auto& Prepare = lifted::entry::e825D5A48;  // sub_825D5A48
inline constexpr auto& RemoveHelper = lifted::f825D5E08;  // sub_825D5E08
inline constexpr auto& SetChannelMatrix = lifted::f825D5758;  // sub_825D5758
inline constexpr auto& SetMainMatrix = lifted::f825D5738;  // sub_825D5738
inline constexpr auto& SetOutputMatrices = lifted::f825D56F8;  // sub_825D56F8
inline constexpr auto& SetOutputVoices = lifted::f825D56D8;  // sub_825D56D8
inline constexpr auto& SetOutputs = lifted::f825D5718;  // sub_825D5718
inline constexpr auto& SetPitch = lifted::f825D56B8;  // sub_825D56B8
inline constexpr auto& SetReverbSend = lifted::f825D5698;  // sub_825D5698
inline constexpr auto& UsesWaveBank = lifted::f825D5620;  // sub_825D5620
}  // namespace game::XACTTrackInstance

namespace game::XACTTransitionEvent {
inline constexpr auto& Ctor = lifted::f825D1A80;  // sub_825D1A80
inline constexpr auto& Dtor = lifted::f825D1AD8;  // sub_825D1AD8
inline constexpr auto& Fire = lifted::f825D1B88;  // sub_825D1B88
inline constexpr auto& IsDue = lifted::f825D1B40;  // sub_825D1B40
}  // namespace game::XACTTransitionEvent

namespace game::XACTTransitionTable {
inline constexpr auto& Ctor = lifted::f825D3688;  // sub_825D3688
inline constexpr auto& Dtor = lifted::f825D36A0;  // sub_825D36A0
inline constexpr auto& Entry = lifted::f825D36F0;  // sub_825D36F0
}  // namespace game::XACTTransitionTable

namespace game::XACTVariable {
inline constexpr auto& Ctor = lifted::entry::e825C20B0;  // sub_825C20B0
inline constexpr auto& DeletingDestructor = lifted::entry::e825C3EE0;  // sub_825C3EE0
inline constexpr auto& Init = lifted::entry::e825C1DE0;  // sub_825C1DE0
inline constexpr auto& Notify = lifted::f825C1E08;  // sub_825C1E08
inline constexpr auto& SetValue = lifted::f825C1F88;  // sub_825C1F88
}  // namespace game::XACTVariable

namespace game::XACTVariationTable {
inline constexpr auto& Ctor = lifted::f825D3360;  // sub_825D3360
inline constexpr auto& Dtor_825D2AD8 = lifted::f825D2AD8;  // sub_825D2AD8
inline constexpr auto& Dtor_825D2D18 = lifted::f825D2D18;  // sub_825D2D18
inline constexpr auto& Next = lifted::f825D3428;  // sub_825D3428
inline constexpr auto& UsesWaveBank = lifted::f825D3338;  // sub_825D3338
}  // namespace game::XACTVariationTable

namespace game::XACTVoice {
inline constexpr auto& Create = lifted::entry::e825C9C40;  // sub_825C9C40
inline constexpr auto& Dtor = lifted::entry::e825CA448;  // sub_825CA448
inline constexpr auto& ExitLoop = lifted::f825C9C30;  // sub_825C9C30
inline constexpr auto& FlushPackets = lifted::entry::e825C9C28;  // sub_825C9C28
inline constexpr auto& GetState = lifted::entry::e825C9DB8;  // sub_825C9DB8
inline constexpr auto& SetMainMatrix = lifted::f825C9E90;  // sub_825C9E90
inline constexpr auto& SetOutputMatrices = lifted::f825C9EE0;  // sub_825C9EE0
inline constexpr auto& SetOutputVoices = lifted::f825C9ED8;  // sub_825C9ED8
inline constexpr auto& SetOutputs = lifted::f825C9D18;  // sub_825C9D18
inline constexpr auto& SetPitch = lifted::entry::e825C9BF0;  // sub_825C9BF0
inline constexpr auto& SetReverbSend = lifted::f825CA3B8;  // sub_825CA3B8
inline constexpr auto& SetVolume = lifted::entry::e825C9BE8;  // sub_825C9BE8
inline constexpr auto& Start = lifted::entry::e825C9BF8;  // sub_825C9BF8
inline constexpr auto& Stop = lifted::entry::e825C9C10;  // sub_825C9C10
inline constexpr auto& SubmitPacket = lifted::entry::e825C9E08;  // sub_825C9E08
}  // namespace game::XACTVoice

namespace game::XACTWave {
inline constexpr auto& AudioUpdate = lifted::entry::e825D2560;  // sub_825D2560
inline constexpr auto& Dtor = lifted::f825D2A48;  // sub_825D2A48
inline constexpr auto& GetDurationMs = lifted::f825D2380;  // sub_825D2380
inline constexpr auto& GetLengthMs = lifted::f825D2300;  // sub_825D2300
inline constexpr auto& Notify = lifted::entry::e825D2030;  // sub_825D2030
inline constexpr auto& OnBankEvent = lifted::f825D24B0;  // sub_825D24B0
inline constexpr auto& Pause = lifted::entry::e825D1ED8;  // sub_825D1ED8
inline constexpr auto& RequestNext = lifted::entry::e825D1C30;  // sub_825D1C30
inline constexpr auto& RequestNotify = lifted::f825D1C50;  // sub_825D1C50
inline constexpr auto& SetChannelMatrix = lifted::f825D1DD0;  // sub_825D1DD0
inline constexpr auto& SetMainMatrix = lifted::f825D1DB0;  // sub_825D1DB0
inline constexpr auto& SetOutputMatrices = lifted::f825D1D70;  // sub_825D1D70
inline constexpr auto& SetOutputVoices = lifted::f825D1D50;  // sub_825D1D50
inline constexpr auto& SetOutputs = lifted::f825D1D90;  // sub_825D1D90
inline constexpr auto& SetPitch = lifted::f825D1D30;  // sub_825D1D30
inline constexpr auto& SetReverbSend = lifted::f825D1CE8;  // sub_825D1CE8
inline constexpr auto& StopCue = lifted::f825D43F0;  // sub_825D43F0
}  // namespace game::XACTWave

namespace game::XACTWaveBank {
inline constexpr auto& AddListener = lifted::entry::e825C8690;  // sub_825C8690
inline constexpr auto& AudioUpdate = lifted::f825CC5C0;  // sub_825CC5C0
inline constexpr auto& CreateWave = lifted::f825CECE8;  // sub_825CECE8
inline constexpr auto& Ctor = lifted::entry::e825C8310;  // sub_825C8310
inline constexpr auto& DeletingDestructorThunk = lifted::f825C8380;  // sub_825C8380
inline constexpr auto& Destroy = lifted::f825CC470;  // sub_825CC470
inline constexpr auto& DoWork = lifted::f825CC560;  // sub_825CC560
inline constexpr auto& Dtor = lifted::f825C85F8;  // sub_825C85F8
inline constexpr auto& GetEntry = lifted::entry::e825C8398;  // sub_825C8398
inline constexpr auto& GetEntryCount = lifted::entry::e825C85E8;  // sub_825C85E8
inline constexpr auto& GetName = lifted::entry::e825C85B8;  // sub_825C85B8
inline constexpr auto& GetSeekTable = lifted::entry::e825C8580;  // sub_825C8580
inline constexpr auto& GetWaveDurations = lifted::entry::e825CC380;  // sub_825CC380
inline constexpr auto& GetWaveFormat = lifted::entry::e825CC320;  // sub_825CC320
inline constexpr auto& GetWaveRegion = lifted::entry::e825C8440;  // sub_825C8440
inline constexpr auto& Load = lifted::f825C83D8;  // sub_825C83D8
inline constexpr auto& Notify = lifted::f825C8710;  // sub_825C8710
inline constexpr auto& RemoveListener = lifted::entry::e825C8540;  // sub_825C8540
inline constexpr auto& SetPrepared = lifted::f825CC610;  // sub_825CC610
}  // namespace game::XACTWaveBank

namespace game::XACTWaveBankBase {
inline constexpr auto& Dtor = lifted::f825CEC38;  // sub_825CEC38
}  // namespace game::XACTWaveBankBase

namespace game::XACTWaveBankRoot {
inline constexpr auto& Dtor = lifted::f825CC6D0;  // sub_825CC6D0
}  // namespace game::XACTWaveBankRoot

namespace game::XACTWaveEntry {
inline constexpr auto& FindXmaBlock = lifted::entry::e825CEF50;  // sub_825CEF50
inline constexpr auto& GetLoopLength = lifted::entry::e82379D50;  // sub_82379D50
inline constexpr auto& GetPlayLength = lifted::f82099BA0;  // sub_82099BA0
}  // namespace game::XACTWaveEntry

namespace game::XACTWaveEventBase {
inline constexpr auto& Dtor = lifted::f825D72C0;  // sub_825D72C0
}  // namespace game::XACTWaveEventBase

namespace game::XACTWaveVariationEvent {
inline constexpr auto& Ctor = lifted::f825D7AA0;  // sub_825D7AA0
inline constexpr auto& Dtor_825D7BA0 = lifted::f825D7BA0;  // sub_825D7BA0
inline constexpr auto& Dtor_825D7F88 = lifted::f825D7F88;  // sub_825D7F88
inline constexpr auto& Fire = lifted::f825D7618;  // sub_825D7618
inline constexpr auto& GetDuration = lifted::f825D7790;  // sub_825D7790
inline constexpr auto& GetLoopCount = lifted::f825D7B70;  // sub_825D7B70
inline constexpr auto& GetTimeLeft = lifted::f825D79E8;  // sub_825D79E8
inline constexpr auto& GetVariationRecord = lifted::f825D7B80;  // sub_825D7B80
inline constexpr auto& NextVariation = lifted::f825D7698;  // sub_825D7698
inline constexpr auto& Pause = lifted::f825D75A8;  // sub_825D75A8
inline constexpr auto& Prepare = lifted::f825D7C48;  // sub_825D7C48
inline constexpr auto& ScheduleNext = lifted::f825D7908;  // sub_825D7908
inline constexpr auto& SetWave = lifted::entry::e825D7B78;  // sub_825D7B78
inline constexpr auto& Stop = lifted::f825D7848;  // sub_825D7848
}  // namespace game::XACTWaveVariationEvent

namespace game::XAudio {
inline constexpr auto& AddStat = lifted::entry::e8245F640;  // sub_8245F640
inline constexpr auto& Commit = lifted::entry::e8245A6E8;  // sub_8245A6E8
inline constexpr auto& CreateEngine = lifted::entry::e82459D60;  // sub_82459D60
inline constexpr auto& GetAllocator = lifted::entry::e82460688;  // sub_82460688
inline constexpr auto& MasterThread = lifted::entry::e8245BE08;  // sub_8245BE08
inline constexpr auto& MixerCallback = lifted::entry::e8245FBE0;  // sub_8245FBE0
inline constexpr auto& SetUpProcessors = lifted::entry::e8245A828;  // sub_8245A828
inline constexpr auto& UnregisterStats = lifted::f8245F5E8;  // sub_8245F5E8
inline constexpr auto& WorkerThread = lifted::entry::e8245BED0;  // sub_8245BED0
}  // namespace game::XAudio

namespace game::XAudioAllocator {
inline constexpr auto& Alloc = lifted::entry::e82460288;  // sub_82460288
inline constexpr auto& Commit = lifted::f82460210;  // sub_82460210
inline constexpr auto& Remaining = lifted::entry::e82460388;  // sub_82460388
inline constexpr auto& Reserve = lifted::f824601F8;  // sub_824601F8
}  // namespace game::XAudioAllocator

namespace game::XAudioBuffer {
inline constexpr auto& ClearThunk = lifted::entry::e8245FA10;  // sub_8245FA10
inline constexpr auto& Create = lifted::entry::e8245FA80;  // sub_8245FA80
inline constexpr auto& GetFormatThunk = lifted::entry::e8245F718;  // sub_8245F718
inline constexpr auto& Size = lifted::entry::e8245FA28;  // sub_8245FA28
}  // namespace game::XAudioBuffer

namespace game::XAudioCodec {
inline constexpr auto& Create = lifted::entry::e82466BA0;  // sub_82466BA0
inline constexpr auto& Destruct = lifted::entry::e82466EE8;  // sub_82466EE8
inline constexpr auto& EndOfStream = lifted::entry::e8246D630;  // sub_8246D630
inline constexpr auto& GetInfo = lifted::entry::e82466E80;  // sub_82466E80
inline constexpr auto& GetState = lifted::entry::e8246A920;  // sub_8246A920
inline constexpr auto& Size = lifted::entry::e82466B78;  // sub_82466B78
}  // namespace game::XAudioCodec

namespace game::XAudioEngine {
inline constexpr auto& Barrier = lifted::entry::e8245A940;  // sub_8245A940
inline constexpr auto& Commit = lifted::entry::e8245B638;  // sub_8245B638
inline constexpr auto& Dtor = lifted::f8245B940;  // sub_8245B940
inline constexpr auto& RegisterCallback = lifted::entry::e8245AB60;  // sub_8245AB60
inline constexpr auto& RunCallbacks = lifted::entry::e8245ACE0;  // sub_8245ACE0
inline constexpr auto& RunStage = lifted::entry::e8245B480;  // sub_8245B480
inline constexpr auto& UnregisterCallback = lifted::f8245B3A0;  // sub_8245B3A0
}  // namespace game::XAudioEngine

namespace game::XAudioFactory {
inline constexpr auto& Dtor = lifted::f8245DB60;  // sub_8245DB60
}  // namespace game::XAudioFactory

namespace game::XAudioMasteringVoice {
inline constexpr auto& Create = lifted::entry::e824600F0;  // sub_824600F0
inline constexpr auto& Dtor = lifted::f8245FB10;  // sub_8245FB10
inline constexpr auto& Pass = lifted::entry::e8245FDC8;  // sub_8245FDC8
inline constexpr auto& Process = lifted::entry::e8245FB80;  // sub_8245FB80
inline constexpr auto& Start = lifted::entry::e8245FE20;  // sub_8245FE20
}  // namespace game::XAudioMasteringVoice

namespace game::XAudioMatrix {
inline constexpr auto& Create = lifted::entry::e82466888;  // sub_82466888
inline constexpr auto& GetInfo = lifted::entry::e82465BE0;  // sub_82465BE0
inline constexpr auto& Init = lifted::entry::e82465C30;  // sub_82465C30
inline constexpr auto& Mix = lifted::entry::e82466440;  // sub_82466440
inline constexpr auto& SetLevels = lifted::entry::e82466188;  // sub_82466188
inline constexpr auto& SetMatrix = lifted::entry::e82465E50;  // sub_82465E50
inline constexpr auto& SetOverwrite = lifted::entry::e82465C08;  // sub_82465C08
inline constexpr auto& Size = lifted::entry::e82466840;  // sub_82466840
}  // namespace game::XAudioMatrix

namespace game::XAudioObject {
inline constexpr auto& Free = lifted::entry::e8245AAD8;  // sub_8245AAD8
inline constexpr auto& Release = lifted::entry::e8246CE70;  // sub_8246CE70
}  // namespace game::XAudioObject

namespace game::XAudioSourceVoice {
inline constexpr auto& ApplyVolume = lifted::entry::e8245DFC0;  // sub_8245DFC0
inline constexpr auto& DeletingDestructor = lifted::entry::e8245E7A0;  // sub_8245E7A0
inline constexpr auto& GetState = lifted::entry::e8245DDC8;  // sub_8245DDC8
inline constexpr auto& Pass = lifted::entry::e8245DED0;  // sub_8245DED0
inline constexpr auto& Reset = lifted::entry::e8245A0B0;  // sub_8245A0B0
inline constexpr auto& SetFormat = lifted::entry::e82459FF8;  // sub_82459FF8
inline constexpr auto& Start = lifted::entry::e8245EBA0;  // sub_8245EBA0
inline constexpr auto& Stop = lifted::entry::e8245E9F0;  // sub_8245E9F0
}  // namespace game::XAudioSourceVoice

namespace game::XAudioStage {
inline constexpr auto& Swap = lifted::entry::e8245B208;  // sub_8245B208
}  // namespace game::XAudioStage

namespace game::XAudioSubmixVoice {
inline constexpr auto& DeletingDestructor = lifted::entry::e8245F3A0;  // sub_8245F3A0
inline constexpr auto& GetState = lifted::f8245F218;  // sub_8245F218
inline constexpr auto& Pass = lifted::entry::e8245EFA0;  // sub_8245EFA0
inline constexpr auto& SetFormat = lifted::f8245F090;  // sub_8245F090
}  // namespace game::XAudioSubmixVoice

namespace game::XAudioVoice {
inline constexpr auto& Activate = lifted::entry::e82460EF0;  // sub_82460EF0
inline constexpr auto& AddRef_82459DB0 = lifted::entry::e82459DB0;  // sub_82459DB0
inline constexpr auto& AddRef_8245F860 = lifted::entry::e8245F860;  // sub_8245F860
inline constexpr auto& BaseSize = lifted::entry::e82461158;  // sub_82461158
inline constexpr auto& Construct = lifted::entry::e82461500;  // sub_82461500
inline constexpr auto& Deactivate = lifted::entry::e82460D50;  // sub_82460D50
inline constexpr auto& EngineBuffer = lifted::entry::e8245DEB0;  // sub_8245DEB0
inline constexpr auto& GetCallbackContext = lifted::f8245E308;  // sub_8245E308
inline constexpr auto& GetEffectEnabled = lifted::f82461490;  // sub_82461490
inline constexpr auto& GetEffectParameters = lifted::f824613A0;  // sub_824613A0
inline constexpr auto& GetFormat = lifted::entry::e8245E328;  // sub_8245E328
inline constexpr auto& GetOutputFormat = lifted::entry::e8245A3B8;  // sub_8245A3B8
inline constexpr auto& GetType = lifted::entry::e8245E318;  // sub_8245E318
inline constexpr auto& InitBase = lifted::entry::e82461D50;  // sub_82461D50
inline constexpr auto& MixToOutputs = lifted::entry::e82460CB8;  // sub_82460CB8
inline constexpr auto& Pass = lifted::entry::e82461070;  // sub_82461070
inline constexpr auto& Play = lifted::entry::e82461798;  // sub_82461798
inline constexpr auto& ProcessEffects = lifted::entry::e82461E88;  // sub_82461E88
inline constexpr auto& ReleaseOutputs = lifted::entry::e82460A58;  // sub_82460A58
inline constexpr auto& Release_82459E18 = lifted::entry::e82459E18;  // sub_82459E18
inline constexpr auto& Release_8245F878 = lifted::entry::e8245F878;  // sub_8245F878
inline constexpr auto& SetEffectEnabled = lifted::f824614C8;  // sub_824614C8
inline constexpr auto& SetEffectParameters = lifted::entry::e82461418;  // sub_82461418
inline constexpr auto& SetFormatWords = lifted::entry::e82461140;  // sub_82461140
inline constexpr auto& SetOutputMatrices = lifted::entry::e82460AB8;  // sub_82460AB8
inline constexpr auto& SetOutputMatrix = lifted::entry::e82459F48;  // sub_82459F48
inline constexpr auto& SetOutputVoices_82459EF0 = lifted::entry::e82459EF0;  // sub_82459EF0
inline constexpr auto& SetOutputVoices_82460BF0 = lifted::entry::e82460BF0;  // sub_82460BF0
inline constexpr auto& StopOrTail = lifted::entry::e824619B0;  // sub_824619B0
}  // namespace game::XAudioVoice

namespace game::XPR {
inline constexpr auto& Create = lifted::entry::e82596F68;  // sub_82596F68
inline constexpr auto& Ctor = lifted::entry::e82596E68;  // sub_82596E68
inline constexpr auto& Dtor = lifted::entry::e82597148;  // sub_82597148
inline constexpr auto& FindByName = lifted::entry::e820D09B8;  // sub_820D09B8
inline constexpr auto& FindTexture = lifted::f82596E88;  // sub_82596E88
inline constexpr auto& GetTexture = lifted::entry::e82597118;  // sub_82597118
inline constexpr auto& Release_82108860 = lifted::entry::e82108860;  // sub_82108860
inline constexpr auto& Release_825970A8 = lifted::entry::e825970A8;  // sub_825970A8
}  // namespace game::XPR

namespace game::XmaCodec {
inline constexpr auto& SetPitch = lifted::entry::e8246AC40;  // sub_8246AC40
inline constexpr auto& SetVolume = lifted::entry::e8246AB48;  // sub_8246AB48
inline constexpr auto& SubmitPacket = lifted::f8246A7D0;  // sub_8246A7D0
inline constexpr auto& SubmitRegion = lifted::entry::e8246A860;  // sub_8246A860
}  // namespace game::XmaCodec

namespace game::XmaObject {
inline constexpr auto& Create = lifted::entry::e8245C650;  // sub_8245C650
inline constexpr auto& Destroy = lifted::entry::e8245C880;  // sub_8245C880
inline constexpr auto& IsLocked = lifted::entry::e8245D350;  // sub_8245D350
inline constexpr auto& Lock = lifted::entry::e8245D208;  // sub_8245D208
inline constexpr auto& MakeContexts = lifted::entry::e8245D138;  // sub_8245D138
inline constexpr auto& ReleaseContexts = lifted::entry::e8245C8D0;  // sub_8245C8D0
inline constexpr auto& ResetStream = lifted::entry::e8245CE98;  // sub_8245CE98
inline constexpr auto& SetInputBuffer = lifted::entry::e8245C958;  // sub_8245C958
inline constexpr auto& SetLoopData = lifted::entry::e8245CF98;  // sub_8245CF98
inline constexpr auto& Unlock = lifted::entry::e8245D4D0;  // sub_8245D4D0
}  // namespace game::XmaObject

namespace game::XmaVoice {
inline constexpr auto& Update = lifted::f8246C320;  // sub_8246C320
}  // namespace game::XmaVoice

namespace game::XprHolder {
inline constexpr auto& Ctor = lifted::entry::e821067A0;  // sub_821067A0
}  // namespace game::XprHolder

namespace game::YuvCodec {
inline constexpr auto& ConvertPair = lifted::f823F59C8;  // sub_823F59C8
inline constexpr auto& DecodeRow = lifted::f823F5EF0;  // sub_823F5EF0
inline constexpr auto& Dtor = lifted::f823F6C10;  // sub_823F6C10
inline constexpr auto& EncodeRow = lifted::f823F5E30;  // sub_823F5E30
inline constexpr auto& Flush = lifted::f823F56A0;  // sub_823F56A0
}  // namespace game::YuvCodec

namespace game::ZeroMatrix {
inline constexpr auto& StaticInit = lifted::entry::e825D8F88;  // sub_825D8F88
}  // namespace game::ZeroMatrix

namespace game::Zone {
inline constexpr auto& IsInside = lifted::entry::e8228B4F8;  // sub_8228B4F8
inline constexpr auto& Update = lifted::entry::e8228BC80;  // sub_8228BC80
}  // namespace game::Zone

namespace game::Zone0 {
inline constexpr auto& Setup = lifted::entry::e8228BE20;  // sub_8228BE20
}  // namespace game::Zone0

namespace game::Zone1 {
inline constexpr auto& Setup = lifted::entry::e8228BE88;  // sub_8228BE88
}  // namespace game::Zone1

namespace game::Zone10 {
inline constexpr auto& Setup = lifted::entry::e8228C230;  // sub_8228C230
}  // namespace game::Zone10

namespace game::Zone11 {
inline constexpr auto& Setup = lifted::entry::e8228C298;  // sub_8228C298
}  // namespace game::Zone11

namespace game::Zone12 {
inline constexpr auto& Setup = lifted::entry::e8228C300;  // sub_8228C300
}  // namespace game::Zone12

namespace game::Zone13 {
inline constexpr auto& Setup = lifted::entry::e8228C368;  // sub_8228C368
}  // namespace game::Zone13

namespace game::Zone14 {
inline constexpr auto& Setup = lifted::entry::e8228C3D0;  // sub_8228C3D0
}  // namespace game::Zone14

namespace game::Zone15 {
inline constexpr auto& Setup = lifted::entry::e8228C438;  // sub_8228C438
}  // namespace game::Zone15

namespace game::Zone16 {
inline constexpr auto& Setup = lifted::entry::e8228C4A0;  // sub_8228C4A0
}  // namespace game::Zone16

namespace game::Zone17 {
inline constexpr auto& Setup = lifted::entry::e8228C508;  // sub_8228C508
}  // namespace game::Zone17

namespace game::Zone18 {
inline constexpr auto& Setup = lifted::entry::e8228C570;  // sub_8228C570
}  // namespace game::Zone18

namespace game::Zone19 {
inline constexpr auto& Setup = lifted::entry::e8228C5D8;  // sub_8228C5D8
}  // namespace game::Zone19

namespace game::Zone2 {
inline constexpr auto& Setup = lifted::entry::e8228BEF0;  // sub_8228BEF0
}  // namespace game::Zone2

namespace game::Zone20 {
inline constexpr auto& Setup = lifted::entry::e8228C640;  // sub_8228C640
}  // namespace game::Zone20

namespace game::Zone21 {
inline constexpr auto& Setup = lifted::entry::e8228C6A8;  // sub_8228C6A8
}  // namespace game::Zone21

namespace game::Zone22 {
inline constexpr auto& Setup = lifted::entry::e8228C710;  // sub_8228C710
}  // namespace game::Zone22

namespace game::Zone23 {
inline constexpr auto& Setup = lifted::entry::e8228C778;  // sub_8228C778
}  // namespace game::Zone23

namespace game::Zone24 {
inline constexpr auto& Setup = lifted::entry::e8228C7E0;  // sub_8228C7E0
}  // namespace game::Zone24

namespace game::Zone25 {
inline constexpr auto& Setup = lifted::entry::e8228C848;  // sub_8228C848
}  // namespace game::Zone25

namespace game::Zone26 {
inline constexpr auto& Setup = lifted::entry::e8228C8B0;  // sub_8228C8B0
}  // namespace game::Zone26

namespace game::Zone27 {
inline constexpr auto& Setup = lifted::entry::e8228C918;  // sub_8228C918
}  // namespace game::Zone27

namespace game::Zone28 {
inline constexpr auto& Setup = lifted::entry::e8228C980;  // sub_8228C980
}  // namespace game::Zone28

namespace game::Zone29 {
inline constexpr auto& Setup = lifted::entry::e8228C9E8;  // sub_8228C9E8
}  // namespace game::Zone29

namespace game::Zone3 {
inline constexpr auto& Setup = lifted::entry::e8228BF58;  // sub_8228BF58
}  // namespace game::Zone3

namespace game::Zone30 {
inline constexpr auto& Setup = lifted::entry::e8228CA50;  // sub_8228CA50
}  // namespace game::Zone30

namespace game::Zone31 {
inline constexpr auto& Setup = lifted::entry::e8228CAB8;  // sub_8228CAB8
}  // namespace game::Zone31

namespace game::Zone32 {
inline constexpr auto& Setup = lifted::entry::e8228CB20;  // sub_8228CB20
}  // namespace game::Zone32

namespace game::Zone33 {
inline constexpr auto& Setup = lifted::entry::e8228CBC8;  // sub_8228CBC8
}  // namespace game::Zone33

namespace game::Zone34 {
inline constexpr auto& Setup = lifted::entry::e8228CC70;  // sub_8228CC70
}  // namespace game::Zone34

namespace game::Zone35 {
inline constexpr auto& Setup = lifted::entry::e8228CD18;  // sub_8228CD18
}  // namespace game::Zone35

namespace game::Zone36 {
inline constexpr auto& Setup = lifted::entry::e8228CD80;  // sub_8228CD80
}  // namespace game::Zone36

namespace game::Zone37 {
inline constexpr auto& Setup = lifted::entry::e8228CE20;  // sub_8228CE20
}  // namespace game::Zone37

namespace game::Zone38 {
inline constexpr auto& Setup = lifted::entry::e8228CFC8;  // sub_8228CFC8
}  // namespace game::Zone38

namespace game::Zone39 {
inline constexpr auto& Setup = lifted::entry::e8228D070;  // sub_8228D070
}  // namespace game::Zone39

namespace game::Zone4 {
inline constexpr auto& Setup = lifted::entry::e8228BFC0;  // sub_8228BFC0
}  // namespace game::Zone4

namespace game::Zone40 {
inline constexpr auto& Setup = lifted::entry::e8228D220;  // sub_8228D220
}  // namespace game::Zone40

namespace game::Zone41 {
inline constexpr auto& Setup = lifted::entry::e8228D2C8;  // sub_8228D2C8
}  // namespace game::Zone41

namespace game::Zone42 {
inline constexpr auto& Setup = lifted::entry::e8228D370;  // sub_8228D370
}  // namespace game::Zone42

namespace game::Zone43 {
inline constexpr auto& Setup = lifted::entry::e8228D418;  // sub_8228D418
}  // namespace game::Zone43

namespace game::Zone44 {
inline constexpr auto& Setup = lifted::entry::e8228D5C0;  // sub_8228D5C0
}  // namespace game::Zone44

namespace game::Zone45 {
inline constexpr auto& Setup = lifted::entry::e8228D668;  // sub_8228D668
}  // namespace game::Zone45

namespace game::Zone46 {
inline constexpr auto& Setup = lifted::entry::e8228D710;  // sub_8228D710
}  // namespace game::Zone46

namespace game::Zone47 {
inline constexpr auto& Setup = lifted::entry::e8228D778;  // sub_8228D778
}  // namespace game::Zone47

namespace game::Zone48 {
inline constexpr auto& Setup = lifted::entry::e8228D870;  // sub_8228D870
}  // namespace game::Zone48

namespace game::Zone49 {
inline constexpr auto& Setup = lifted::entry::e8228D968;  // sub_8228D968
}  // namespace game::Zone49

namespace game::Zone5 {
inline constexpr auto& Setup = lifted::entry::e8228C028;  // sub_8228C028
}  // namespace game::Zone5

namespace game::Zone50 {
inline constexpr auto& Setup = lifted::entry::e8228DA08;  // sub_8228DA08
}  // namespace game::Zone50

namespace game::Zone51 {
inline constexpr auto& Setup = lifted::entry::e8228DBA8;  // sub_8228DBA8
}  // namespace game::Zone51

namespace game::Zone52 {
inline constexpr auto& Setup = lifted::entry::e8228DC60;  // sub_8228DC60
}  // namespace game::Zone52

namespace game::Zone53 {
inline constexpr auto& Setup = lifted::entry::e8228DD00;  // sub_8228DD00
}  // namespace game::Zone53

namespace game::Zone54 {
inline constexpr auto& Setup = lifted::entry::e8228DDE8;  // sub_8228DDE8
}  // namespace game::Zone54

namespace game::Zone55 {
inline constexpr auto& Setup = lifted::entry::e8228DE50;  // sub_8228DE50
}  // namespace game::Zone55

namespace game::Zone56 {
inline constexpr auto& Setup = lifted::entry::e8228DEB0;  // sub_8228DEB0
}  // namespace game::Zone56

namespace game::Zone57 {
inline constexpr auto& Setup = lifted::entry::e8228DF58;  // sub_8228DF58
}  // namespace game::Zone57

namespace game::Zone58 {
inline constexpr auto& Setup = lifted::entry::e8228E000;  // sub_8228E000
}  // namespace game::Zone58

namespace game::Zone59 {
inline constexpr auto& Setup = lifted::entry::e8228E068;  // sub_8228E068
}  // namespace game::Zone59

namespace game::Zone6 {
inline constexpr auto& Setup = lifted::entry::e8228C090;  // sub_8228C090
}  // namespace game::Zone6

namespace game::Zone60 {
inline constexpr auto& Setup = lifted::entry::e8228E0D0;  // sub_8228E0D0
}  // namespace game::Zone60

namespace game::Zone61 {
inline constexpr auto& Setup = lifted::entry::e8228E190;  // sub_8228E190
}  // namespace game::Zone61

namespace game::Zone62 {
inline constexpr auto& Setup = lifted::entry::e8228E238;  // sub_8228E238
}  // namespace game::Zone62

namespace game::Zone63 {
inline constexpr auto& Setup = lifted::entry::e8228E2E8;  // sub_8228E2E8
}  // namespace game::Zone63

namespace game::Zone64 {
inline constexpr auto& Setup = lifted::entry::e8228E390;  // sub_8228E390
}  // namespace game::Zone64

namespace game::Zone65 {
inline constexpr auto& Setup = lifted::entry::e8228E3F8;  // sub_8228E3F8
}  // namespace game::Zone65

namespace game::Zone66 {
inline constexpr auto& Setup = lifted::entry::e8228E498;  // sub_8228E498
}  // namespace game::Zone66

namespace game::Zone67 {
inline constexpr auto& Setup = lifted::entry::e8228E550;  // sub_8228E550
}  // namespace game::Zone67

namespace game::Zone68 {
inline constexpr auto& Setup = lifted::entry::e8228E5F0;  // sub_8228E5F0
}  // namespace game::Zone68

namespace game::Zone69 {
inline constexpr auto& Setup = lifted::entry::e8228E6A0;  // sub_8228E6A0
}  // namespace game::Zone69

namespace game::Zone7 {
inline constexpr auto& Setup = lifted::entry::e8228C0F8;  // sub_8228C0F8
}  // namespace game::Zone7

namespace game::Zone8 {
inline constexpr auto& Setup = lifted::entry::e8228C160;  // sub_8228C160
}  // namespace game::Zone8

namespace game::Zone9 {
inline constexpr auto& Setup = lifted::entry::e8228C1C8;  // sub_8228C1C8
}  // namespace game::Zone9
