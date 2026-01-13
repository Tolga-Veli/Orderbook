#pragma once

#include "Orderbook.hpp"
#include "Renderer.hpp"

namespace ob {
class Application {
public:
  Application();
  ~Application();

  void Init();
  void Run();
  void Shutdown();

private:
  engine::Orderbook m_Orderbook;
  render::Renderer m_Renderer;
  bool m_Running = false;
  const std::chrono::milliseconds m_FrameTime{200}; // ~30 FPS

  void Tick();
};

}; // namespace ob