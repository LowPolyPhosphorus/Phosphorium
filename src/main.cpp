#include <SDL2/SDL.h>
#include <vector>
#include <cstring>
#include <string>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/internal/cef_types_runtime.h"

const int kWidth = 1280;
const int kHeight = 720;
const int kChromeHeight = 60;
const int kContentHeight = kHeight - kChromeHeight;

class RenderHandler : public CefRenderHandler {
 public:
  RenderHandler(SDL_Renderer* renderer, int width, int height)
      : width_(width), height_(height) {
    texture_ = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING,
        width_, height_);
  }

  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override {
    rect = CefRect(0, 0, width_, height_);
  }

  void OnPaint(CefRefPtr<CefBrowser> browser,
               PaintElementType type,
               const RectList& dirtyRects,
               const void* buffer,
               int width,
               int height) override {
    if (type != PET_VIEW) return;
    SDL_UpdateTexture(texture_, nullptr, buffer, width * 4);
  }

  SDL_Texture* texture() { return texture_; }

  IMPLEMENT_REFCOUNTING(RenderHandler);

 private:
  int width_, height_;
  SDL_Texture* texture_;
};

class BrowserClient : public CefClient,
                       public CefLifeSpanHandler {
 public:
  explicit BrowserClient(CefRefPtr<RenderHandler> render_handler)
      : render_handler_(render_handler) {}

  CefRefPtr<CefRenderHandler> GetRenderHandler() override {
    return render_handler_;
  }

  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
    return this;
  }

  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override {
    browser_ = browser;
  }

  bool DoClose(CefRefPtr<CefBrowser> browser) override {
    return false;
  }

  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override {
    closed_count_++;
  }

  CefRefPtr<CefBrowser> browser() { return browser_; }
  RenderHandler* render_handler() { return render_handler_.get(); }

  IMPLEMENT_REFCOUNTING(BrowserClient);

 private:
  CefRefPtr<RenderHandler> render_handler_;
  CefRefPtr<CefBrowser> browser_;
  int closed_count_ = 0;
};

class PhosphoriumApp : public CefApp {
 public:
  void OnBeforeCommandLineProcessing(
      const CefString& process_type,
      CefRefPtr<CefCommandLine> command_line) override {
    command_line->AppendSwitch("disable-gpu");
    command_line->AppendSwitch("disable-gpu-compositing");
    command_line->AppendSwitch("disable-software-rasterizer");
    command_line->AppendSwitch("in-process-gpu");
  }

  IMPLEMENT_REFCOUNTING(PhosphoriumApp);
};

CefRefPtr<BrowserClient> CreateBrowserInstance(
    SDL_Renderer* renderer, int width, int height, const std::string& url) {
  CefRefPtr<RenderHandler> render_handler(new RenderHandler(renderer, width, height));
  CefRefPtr<BrowserClient> client(new BrowserClient(render_handler));

  CefWindowInfo window_info;
  window_info.SetAsWindowless(0);
  window_info.runtime_style = CEF_RUNTIME_STYLE_ALLOY;

  CefBrowserSettings browser_settings;
  CefBrowserHost::CreateBrowser(window_info, client, url, browser_settings, nullptr, nullptr);

  return client;
}

int main(int argc, char* argv[]) {
    CefMainArgs args(argc, argv);
    CefRefPtr<PhosphoriumApp> app(new PhosphoriumApp);

    int exit_code = CefExecuteProcess(args, app, nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    SDL_Window* sdl_window = SDL_CreateWindow(
        "Phosphorium",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        kWidth, kHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(
        sdl_window, -1, SDL_RENDERER_ACCELERATED);

    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;

    CefString(&settings.resources_dir_path).FromASCII("/home/lowpolyphosphorus/Phosphorium/cef/Resources");
    CefString(&settings.locales_dir_path).FromASCII("/home/lowpolyphosphorus/Phosphorium/cef/Resources/locales");

    CefInitialize(args, settings, app, nullptr);

    std::string chrome_path = "file:///home/lowpolyphosphorus/Phosphorium/src/ui/chrome.html";

    CefRefPtr<BrowserClient> chrome_client =
        CreateBrowserInstance(sdl_renderer, kWidth, kChromeHeight, chrome_path);
    CefRefPtr<BrowserClient> content_client =
        CreateBrowserInstance(sdl_renderer, kWidth, kContentHeight, "https://example.com");

    bool running = true;
    SDL_Event e;
    while (running) {
        CefDoMessageLoopWork();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }

            int mx = 0, my = 0;
            bool is_mouse_event = false;

            if (e.type == SDL_MOUSEMOTION) {
                mx = e.motion.x; my = e.motion.y; is_mouse_event = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                mx = e.button.x; my = e.button.y; is_mouse_event = true;
            } else if (e.type == SDL_MOUSEWHEEL) {
                SDL_GetMouseState(&mx, &my); is_mouse_event = true;
            }

            CefRefPtr<BrowserClient> target_client;
            int local_y = my;
            if (is_mouse_event) {
                if (my < kChromeHeight) {
                    target_client = chrome_client;
                    local_y = my;
                } else {
                    target_client = content_client;
                    local_y = my - kChromeHeight;
                }
            }

            if (target_client && target_client->browser()) {
                CefRefPtr<CefBrowserHost> host = target_client->browser()->GetHost();

                if (e.type == SDL_MOUSEMOTION) {
                    CefMouseEvent mouse_event;
                    mouse_event.x = mx;
                    mouse_event.y = local_y;
                    host->SendMouseMoveEvent(mouse_event, false);
                } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                    CefMouseEvent mouse_event;
                    mouse_event.x = mx;
                    mouse_event.y = local_y;
                    CefBrowserHost::MouseButtonType btn = MBT_LEFT;
                    if (e.button.button == SDL_BUTTON_RIGHT) btn = MBT_RIGHT;
                    else if (e.button.button == SDL_BUTTON_MIDDLE) btn = MBT_MIDDLE;
                    host->SendMouseClickEvent(mouse_event, btn,
                        e.type == SDL_MOUSEBUTTONDOWN, 1);
                } else if (e.type == SDL_MOUSEWHEEL) {
                    CefMouseEvent mouse_event;
                    mouse_event.x = mx;
                    mouse_event.y = local_y;
                    host->SendMouseWheelEvent(mouse_event, e.wheel.x * 60, e.wheel.y * 60);
                }
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
                if (content_client->browser()) {
                    CefRefPtr<CefBrowserHost> host = content_client->browser()->GetHost();
                    CefKeyEvent key_event;
                    key_event.type = (e.type == SDL_KEYDOWN) ? KEYEVENT_RAWKEYDOWN : KEYEVENT_KEYUP;
                    key_event.windows_key_code = e.key.keysym.sym;
                    key_event.native_key_code = e.key.keysym.scancode;
                    host->SendKeyEvent(key_event);
                }
            } else if (e.type == SDL_TEXTINPUT) {
                if (content_client->browser()) {
                    CefRefPtr<CefBrowserHost> host = content_client->browser()->GetHost();
                    CefKeyEvent key_event;
                    key_event.type = KEYEVENT_CHAR;
                    key_event.windows_key_code = e.text.text[0];
                    host->SendKeyEvent(key_event);
                }
            }
        }

        SDL_RenderClear(sdl_renderer);

        SDL_Rect chrome_rect = {0, 0, kWidth, kChromeHeight};
        SDL_RenderCopy(sdl_renderer, chrome_client->render_handler()->texture(), nullptr, &chrome_rect);

        SDL_Rect content_rect = {0, kChromeHeight, kWidth, kContentHeight};
        SDL_RenderCopy(sdl_renderer, content_client->render_handler()->texture(), nullptr, &content_rect);

        SDL_RenderPresent(sdl_renderer);

        SDL_Delay(1);
    }

    CefShutdown();
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
    return 0;
}
