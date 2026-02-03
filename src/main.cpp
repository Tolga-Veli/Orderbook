#include "Core/Application.hpp"

int main() {
  auto app = new ob::Application();
  app->Run();
  app->Shutdown();
  delete app;
}
