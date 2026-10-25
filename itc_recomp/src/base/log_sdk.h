// Our log takes over the SDK's log file (log_sdk.cpp), until A4.

#pragma once

namespace base {

// From the app's OnPostInitLogging, after the SDK opened its log file.
void TakeOverSdkLogFile();

}  // namespace base
