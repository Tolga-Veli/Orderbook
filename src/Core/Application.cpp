#include <random>
#include <thread>

#include "Application.hpp"

namespace ob {
Application::Application()
    : m_Orderbook(), m_Renderer(), m_CommandQueue(m_Orderbook) {
  m_Running = true;
};
Application::~Application() { m_Running = false; }

void Application::Run() {
  static std::mt19937 rng{1};
  static std::uniform_int_distribution<uint32_t> dist(1, 1000);

  std::vector<OrderID> ids;
  while (m_Running) {
    Tick();
    m_CommandQueue.AddOrder(ClientID{1}, Price{dist(rng)}, Quantity{dist(rng)},
                            ob::Side::Buy, ob::OrderType::Limit,
                            ob::TimeInForce::GoodTillCancelled,
                            ob::Flags::None);
    m_CommandQueue.AddOrder(ClientID{2}, Price{dist(rng)}, Quantity{dist(rng)},
                            ob::Side::Sell, ob::OrderType::Limit,
                            ob::TimeInForce::GoodTillCancelled,
                            ob::Flags::None);

    std::optional<std::size_t> id1 = 0, id2 = 0;
    if (id1.has_value())
      ids.push_back(id1.value());
    if (id2.has_value())
      ids.push_back(id2.value());

    if (ids.size() % 50 == 0) {
      std::vector<OrderID> cancel;
      for (auto order_id : ids) {
        if (cancel.size() >= 10)
          break;

        auto order = m_Orderbook.GetOrder(order_id);
        if (order.has_value())
          cancel.push_back(order_id);
      }

      for (auto order_id : cancel)
        m_CommandQueue.CancelOrder(order_id);
    }
    std::this_thread::sleep_for(m_FrameTime);
    // m_Renderer.Clear();
  }
}

void Application::Tick() {
  auto snapshot = m_Orderbook.GetSnapshot(30);
  m_Renderer.Render(snapshot);
}
} // namespace ob
