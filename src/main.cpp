#include "game.hpp"
#include <chrono>

void quit()
{
  SDL_Quit();
  TTF_Quit();
}

int main(int argc, char *argv[])
{
  try
  {
    Game game{};
    auto start{std::chrono::high_resolution_clock::now()};

    while (game.is_running())
    {
      auto current{std::chrono::high_resolution_clock::now()};
      auto delta{current - start};

      game.update(std::chrono::duration_cast<std::chrono::milliseconds>(delta));
      game.handle_events();
      game.render();

      start = current;
    }
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  std::atexit(quit);

  return EXIT_SUCCESS;
}
