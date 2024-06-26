#pragma once

#include "particles.hpp"
#include "star.hpp"
#include "ui/selection.hpp"
#include "ui/text.hpp"
#include "ui/textinput.hpp"
#include "ui/ui.hpp"
#include "word.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Game
{
public:
  enum class GameStates
  {
    MainMenu = 0x0,
    MainLoop,
    GameOver,
    Pause
  };

  enum class DifficultyLevels
  {
    Easy = 0x0,
    Medium,
    Hard,
    Punishing,
    Zen
  };

  Game();
  void update(std::chrono::milliseconds delta);
  void handle_events();
  void render();

  SDL_Renderer *get_renderer();
  bool is_running() const;

protected:
  std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>> m_window{};
  std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>
      m_renderer{};
  std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> m_title_font{};
  std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>> m_font{};
  bool m_running{};
  SDL_Event m_event{};
  GameStates m_state{};
  ui::TextInput m_textinput{};
  std::vector<std::string> m_wordlist{};
  std::vector<Word> m_current_words{};
  Vec2<float> m_current_cursor{};
  int32_t m_score{};
  int32_t m_lives{5};
  std::vector<ui::UniqueTexture> m_static_text{};
  ui::UniqueTexture m_score_texture{};
  ui::UniqueTexture m_lives_texture{};
  std::vector<Star> m_stars{};
  std::vector<SDL_Vertex> m_stars_vertices{};
  std::vector<int32_t> m_stars_indices{};
  Vec2<int32_t> m_mouse_position{};
  std::chrono::milliseconds m_delta{};
  std::vector<ParticleSystem> m_particle_systems{};
  std::vector<std::unique_ptr<ui::Widget>> m_menu_widgets{};
  std::vector<std::unique_ptr<ui::Widget>> m_game_over_widgets{};
  int32_t m_menu_selected{};
  int32_t m_game_over_selected{};
  DifficultyLevels m_difficulty{};

  std::string m_player_name{"Unknown"};
  ui::TextInput m_player_name_input{};

  void generate_word();

  const std::unordered_map<std::string_view, DifficultyLevels>
      m_difficulty_string_map{
          {"Easy", DifficultyLevels::Easy},
          {"Medium", DifficultyLevels::Medium},
          {"Hard", DifficultyLevels::Hard},
          {"Punishing", DifficultyLevels::Punishing},
          {"Zen", DifficultyLevels::Zen},
      };
};
