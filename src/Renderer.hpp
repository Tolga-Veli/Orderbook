#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <mutex>
#include <thread>

#include "OrderbookSnapshot.hpp"

namespace ob::render {
class Renderer {
public:
  Renderer();
  ~Renderer();

  void Init();
  void Shutdown();

  void Render(const ob::engine::OrderbookSnapshot &snashot);

private:
  ftxui::ScreenInteractive m_Screen;
  bool m_Running = false;
};
} // namespace ob::render