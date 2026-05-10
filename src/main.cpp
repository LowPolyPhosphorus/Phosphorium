#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"

int main(int argc, char* argv[]) {
    // Basic CEF setup
    CefMainArgs args(argc, argv);
    CefSettings settings;
    settings.no_sandbox = true;

    // Start CEF
    CefInitialize(args, settings, nullptr, nullptr);

    // Window settings
    CefWindowInfo window;
    window.SetAsPopup(nullptr, "Phosphorium");

    // Open a browser window pointing to a homepage
    CefBrowserSettings browser_settings;
    CefBrowserHost::CreateBrowserSync(
        window,
        nullptr,
        "https://example.com",  // change this to your homepage later
        browser_settings,
        nullptr,
        nullptr
    );

    // Run until the user closes the window
    CefRunMessageLoop();
    CefShutdown();

    return 0;
}
