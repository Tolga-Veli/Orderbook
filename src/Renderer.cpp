#include <algorithm> // for std::max

#include "Renderer.hpp"
#include "globals.hpp"

namespace ob::render {

Renderer::Renderer() : m_Screen(ftxui::ScreenInteractive::TerminalOutput()) {
  Init();
}

Renderer::~Renderer() { Shutdown(); }

void Renderer::Init() {}
void Renderer::Shutdown() {}

void Renderer::Render(const ob::engine::OrderbookSnapshot &snapshot) {
  using namespace ftxui;

  Quantity max_vol = 1;
  for (const auto &p : snapshot.bids)
    max_vol = std::max(max_vol, p.second);
  for (const auto &p : snapshot.asks)
    max_vol = std::max(max_vol, p.second);

  auto make_row = [&](Price price, Quantity volume, Color c, bool is_bid) {
    float fraction = static_cast<float>(volume) / static_cast<float>(max_vol);

    auto bar = gauge(fraction) | color(c);
    auto price_txt =
        text(core::format_price(price)) | color(Color::White) | bold;
    auto size_txt =
        text(core::format_quantity(volume)) | color(Color::GrayLight);

    if (is_bid)
      return hbox({bar | size(WIDTH, EQUAL, 15), text(" "), size_txt, filler(),
                   price_txt});
    else
      return hbox({price_txt, filler(), size_txt, text(" "),
                   bar | size(WIDTH, EQUAL, 15)});
  };

  Elements bid_rows, ask_rows;
  for (const auto &bid : snapshot.bids)
    bid_rows.push_back(make_row(bid.first, bid.second, Color::Green, true));

  for (const auto &ask : snapshot.asks)
    ask_rows.push_back(make_row(ask.first, ask.second, Color::Red, false));

  Price best_bid = snapshot.bids.empty() ? 0 : snapshot.bids.front().first;
  Price best_ask = snapshot.asks.empty() ? 0 : snapshot.asks.front().first;

  Price spread_val = 0;
  if (best_ask > 0 && best_bid > 0)
    spread_val = best_ask - best_bid;
  else
    spread_val = 0;

  auto spread_display =
      text(" Spread: " + core::format_price(spread_val) + " ") |
      bgcolor(Color::GrayDark) | color(Color::White);

  auto bid_side = vbox({vbox(bid_rows), filler()}) | flex;
  auto ask_side = vbox({vbox(ask_rows), filler()}) | flex;

  auto main_layout = vbox({hbox({
                               text(" BIDS ") | color(Color::Green) | bold,
                               filler(),
                               spread_display,
                               filler(),
                               text(" ASKS ") | color(Color::Red) | bold,
                           }) | border,
                           hbox({bid_side, separator(), ask_side}) | flex}) |
                     border | flex;

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(main_layout));
  ftxui::Render(screen, main_layout);

  std::cout << "\x1b[H\x1b[J";
  screen.Print();
  std::cout << std::flush;
}

} // namespace ob::render