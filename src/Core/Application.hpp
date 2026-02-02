#pragma once

#include "CommandQueue.hpp"
#include "OrderBook.hpp"
#include "Renderer.hpp"

namespace ob {
class Application {
public:
  Application();
  ~Application();

  void Run();

private:
  engine::OrderBook m_Orderbook;
  engine::CommandQueue m_CommandQueue;
  render::Renderer m_Renderer;

  bool m_Running = false;
  static constexpr std::chrono::milliseconds m_FrameTime{16};

  void Tick();
};

}; // namespace ob
