#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/views/cef_window.h"

class PhosphoriumApp : public CefApp {
 public:
  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override {
    // WSL has no usable GPU pipeline for Chromium - disable it entirely.
    command_line->AppendSwitch("disable-gpu");
    command_line->AppendSwitch("disable-gpu-compositing");
    command_line->AppendSwitch("disable-software-rasterizer");
    command_line->AppendSwitch("in-process-gpu");
  }

  IMPLEMENT_REFCOUNTING(PhosphoriumApp);
};

int main(int argc, char* argv[]) {
    CefMainArgs args(argc, argv);

    CefRefPtr<PhosphoriumApp> app(new PhosphoriumApp);

    // Required: lets CEF handle subprocess launches (renderer, gpu, etc.)
    int exit_code = CefExecuteProcess(args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }

    CefSettings settings;
    settings.no_sandbox = true;

    CefString(&settings.resources_dir_path).FromASCII("/home/lowpolyphosphorus/Phosphorium/cef/Resources");
    CefString(&settings.locales_dir_path).FromASCII("/home/lowpolyphosphorus/Phosphorium/cef/Resources/locales");

    CefInitialize(args, settings, app, nullptr);

    CefWindowInfo window;
    window.SetAsChild(0, {0, 0, 1280, 720});

    CefBrowserSettings browser_settings;
    CefBrowserHost::CreateBrowserSync(
        window,
        nullptr,
        "https://example.com",
        browser_settings,
        nullptr,
        nullptr
    );

    CefRunMessageLoop();
    CefShutdown();
    return 0;
}
