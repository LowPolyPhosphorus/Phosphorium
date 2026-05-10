#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/views/cef_window.h"

int main(int argc, char* argv[]) {
    CefMainArgs args(argc, argv);
    CefSettings settings;
    settings.no_sandbox = true;

    CefInitialize(args, settings, nullptr, nullptr);

    CefWindowInfo window;
    // SetAsPopup was removed - use this instead on Linux
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
