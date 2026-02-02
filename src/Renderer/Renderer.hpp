#pragma once

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "OrderBookSnapshot.hpp"

namespace ob::render {
class Renderer {
public:
  Renderer();
  ~Renderer();

  void Render(const ob::engine::OrderBookSnapshot &snapshot);

private:
  ftxui::ScreenInteractive m_Screen;
  bool m_Running = false;
};
} // namespace ob::render
