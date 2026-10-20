// itc - ReXGlue Recompiled Project
//
// Customize your app by overriding virtual hooks from rex::ReXApp.

#pragma once

#include <rex/filesystem.h>
#include <rex/rex_app.h>

#include "audio/native_audio.h"
#include "debug/thread_cpu.h"
#include "native/app_window.h"

class ItcApp : public rex::ReXApp {
 public:
  using rex::ReXApp::ReXApp;

  static std::unique_ptr<rex::ui::WindowedApp> Create(
      rex::ui::WindowedAppContext& ctx) {
    return std::unique_ptr<ItcApp>(new ItcApp(ctx, "itc",
        PPCImageConfig));
  }

  // The native renderer's exclusive mode hides this window and routes its
  // own window's events to it (src/native/app_window.cpp).
  bool SetupPresentation() override {
    if (!rex::ReXApp::SetupPresentation()) {
      return false;
    }
    native::AppWindow_SetSdkWindow(window());
    return true;
  }

  // Our own audio output instead of the SDK's (src/audio/native_audio.cpp).
  void OnPreSetup(rex::RuntimeConfig& config) override {
    native_audio::Configure(config);
  }

  // --log_thread_cpu (src/debug/thread_cpu.cpp).
  void OnPostSetup() override { debug::ThreadCpu_Start(); }
  void OnShutdown() override { debug::ThreadCpu_Stop(); }

  // Without --game_data_root, the game's files are next to the executable
  // (where run.sh points it). The SDK reads that flag before itc.toml is
  // loaded, so the config file can't set it.
  void OnConfigurePaths(rex::PathConfig& paths) override {
    if (paths.game_data_root.empty()) {
      paths.game_data_root = rex::filesystem::GetExecutableFolder();
    }
  }

  // Override virtual hooks for customization:
  // void OnPostInitLogging() override {}
  // void OnLoadXexImage(std::string& xex_image) override {}
  // void OnPostLoadXexImage() override {}
  // void OnCreateDialogs(rex::ui::ImGuiDrawer* drawer) override {}
  // std::unique_ptr<rex::ui::ImGuiDialog> CreateAchievementsOverlay() override;
  // std::unique_ptr<rex::ui::AchievementNotificationDialog>
  // CreateAchievementNotificationDialog() override;
  // void OnConfigurePaths(rex::PathConfig& paths) override {}
};
