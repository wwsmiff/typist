#include "game.hpp"
#include "config.hpp"
#include "random.hpp"

#include <algorithm>
#include <array>
#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>

namespace
{
std::filesystem::path find_resources()
{
  for (std::filesystem::path p :
       {".", "..", "../..", "../../..", "../../../..", "../../../../.."})
  {
    auto path = p / "resources";
    if (std::filesystem::is_directory(path))
      return path;
  }
  return std::filesystem::path{};
}

int32_t max(int32_t a, int32_t b) { return a * (a > b) + b * (b > a); }
int32_t min(int32_t a, int32_t b) { return a * (a < b) + b * (b < a); }

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
            int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

}; // namespace

namespace loader
{
SDL_Window *create_window_or_throw()
{
  SDL_Window *window =
      SDL_CreateWindow(config::window_title.data(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, config::window_width_v,
                       config::window_height_v, SDL_WINDOW_SHOWN);

  if (!window)
  {
    throw std::runtime_error(
        std::format("Failed to create window: {}", SDL_GetError()));
    return nullptr;
  }

  return window;
}

SDL_Renderer *create_renderer_or_throw(SDL_Window *window)
{
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  if (!renderer)
  {
    throw std::runtime_error(
        std::format("Failed to create renderer: {}", SDL_GetError()));
  }

  return renderer;
}

TTF_Font *create_font_or_throw()
{

  std::stringstream path_stream{};
  path_stream << find_resources().string() << "/" << config::font_name_v;

  TTF_Font *font = TTF_OpenFont(path_stream.str().c_str(), config::font_size_v);
  if (!font)
  {
    throw std::runtime_error(
        std::format("Failed to open font: {}", TTF_GetError()));
  }

  return font;
}

}; // namespace loader

Game::Game()
    : m_window{loader::create_window_or_throw(), SDL_DestroyWindow},
      m_renderer{loader::create_renderer_or_throw(m_window.get()),
                 SDL_DestroyRenderer},
      m_current_words{}, m_particle_system{Vec2<int32_t>{0, 0}, 50}
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    throw std::runtime_error(
        std::format("Failed to initialize SDL2: {}", SDL_GetError()));
  }

  if (TTF_Init() != 0)
  {
    throw std::runtime_error(
        std::format("Failed to initialize SDL2_ttf: {}", TTF_GetError()));
  }

  SDL_SetRenderDrawBlendMode(m_renderer.get(), SDL_BLENDMODE_BLEND);

  m_font = std::unique_ptr<TTF_Font, std::function<void(TTF_Font *)>>{
      loader::create_font_or_throw(), TTF_CloseFont};

  m_running = true;

  m_textinput =
      ui::TextInput(m_renderer.get(), m_font.get(),
                    config::text_input_position_vec2_v, config::font_size_v);

  std::ifstream wordlist_text{find_resources() / "wordlist.txt"};
  for (std::string line{}; std::getline(wordlist_text, line);)
  {
    line.pop_back();
    m_wordlist.push_back(std::move(line));
  }

  m_current_cursor = {rng::f32(-300.0f, -100.0f), 132.0f};
  for (int i = 0; i < config::max_words_on_screen_v; ++i)
    generate_word();

  m_static_text.push_back(ui::load_word(m_font.get(), m_renderer.get(),
                                        "Score: ", config::text_color_v));

  m_static_text.push_back(ui::load_word(m_font.get(), m_renderer.get(),
                                        "Lives: ", config::text_color_v));

  m_score_texture =
      ui::load_word(m_font.get(), m_renderer.get(), std::to_string(m_score),
                    config::text_color_v);

  m_lives_texture =
      ui::load_word(m_font.get(), m_renderer.get(), std::to_string(m_lives),
                    config::text_color_v);

  constexpr std::array<SDL_Color, 2> star_colors{
      {{255, 255, 255, 255}, {100, 100, 100, 255}}};

  for (int32_t i = 0; i < 1000; ++i)
  {
    const size_t offset{m_stars_vertices.size()};
    Vec2<int32_t> star_position{rng::i32(0, config::window_width_v),
                                rng::i32(0, config::window_height_v)};
    SDL_Color star_color = star_colors.at(rng::i64(0, 1));
    // m_stars.emplace_back(SDL_Rect{, 1, 1}, star_colors.at(rng::i64(0, 1)));

    m_stars_vertices.push_back(
        SDL_Vertex{{star_position.x, star_position.y + 1},
                   {star_color.r, star_color.g, star_color.b, star_color.a},
                   {0, 0}});

    m_stars_vertices.push_back(
        SDL_Vertex{{star_position.x + 1, star_position.y + 1},
                   {star_color.r, star_color.g, star_color.b, star_color.a},
                   {0, 0}});

    m_stars_vertices.push_back(
        SDL_Vertex{{star_position.x + 1, star_position.y},
                   {star_color.r, star_color.g, star_color.b, star_color.a},
                   {0, 0}});
    m_stars_vertices.push_back(
        SDL_Vertex{{star_position.x, star_position.y},
                   {star_color.r, star_color.g, star_color.b, star_color.a},
                   {0, 0}});
    m_stars_indices.push_back(offset);
    m_stars_indices.push_back(offset + 1);
    m_stars_indices.push_back(offset + 2);
    m_stars_indices.push_back(offset + 2);
    m_stars_indices.push_back(offset + 3);
    m_stars_indices.push_back(offset);
  }
}

void Game::update(std::chrono::milliseconds delta)
{
  m_delta = delta;

  for (auto &word : m_current_words)
    word.position.x += config::text_velocity_v * delta.count();

  m_particle_system.update(delta);
}

void Game::handle_events()
{
  SDL_GetMouseState(&m_mouse_position.x, &m_mouse_position.y);

  while (SDL_PollEvent(&m_event))
  {
    if (m_event.type == SDL_QUIT)
      m_running = false;
    m_textinput.handle_events(m_event);

    if (m_event.type == SDL_KEYDOWN)
    {
      if (m_event.key.keysym.sym == SDLK_RETURN)
      {
        for (auto it = m_current_words.begin(); it != m_current_words.end();
             ++it)
        {
          if (it->position.x > config::window_width_v)
          {
            m_current_words.erase(it);
            // TODO: decrement lives
            generate_word();
          }
          if (it->type == WordType::Normal &&
              m_textinput.get_value() == it->value)
          {
            m_score += m_textinput.get_value().size();
            m_score_texture =
                ui::load_word(m_font.get(), m_renderer.get(),
                              std::to_string(m_score), config::text_color_v);
            Vec2<int32_t> destroyed_word_position{
                static_cast<int32_t>(
                    (it->position.x +
                     (it->value.size() * ui::letter_width_v) / 2)),
                static_cast<int32_t>(it->position.y)};
            m_current_words.erase(it);
            generate_word();
            m_particle_system.set_position(
                Vec2<int32_t>{static_cast<int32_t>(destroyed_word_position.x),
                              static_cast<int32_t>(destroyed_word_position.y)});
            m_particle_system.emit();
            break;
          }
          else if (it->type == WordType::Reverse &&
                   m_textinput.get_value() ==
                       std::string{it->value.rbegin(), it->value.rend()})
          {
            m_score += m_textinput.get_value().size() * 2;
            m_score_texture =
                ui::load_word(m_font.get(), m_renderer.get(),
                              std::to_string(m_score), config::text_color_v);
            m_current_words.erase(it);
            generate_word();
            break;
          }
        }

        m_textinput.clear();
      }
    }
  }
}
void Game::render()
{
  SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
  SDL_RenderClear(m_renderer.get());

  // for (const auto &star : m_stars)
  // {
  //   SDL_SetRenderDrawColor(m_renderer.get(), star.color.r, star.color.b,
  //                          star.color.g, star.color.a);
  //   SDL_RenderFillRect(m_renderer.get(), &star.rect);
  // }

  SDL_RenderGeometry(m_renderer.get(), nullptr, m_stars_vertices.data(),
                     m_stars_vertices.size(), m_stars_indices.data(),
                     m_stars_indices.size());

  m_textinput.render();

  static SDL_Rect score_rect{
      config::score_position_vec2_v.x, config::score_position_vec2_v.y,
      (std::string{"Score: "}.size() * ui::letter_width_v),
      config::font_size_v};

  SDL_RenderCopy(m_renderer.get(), m_static_text.at(0).get(), nullptr,
                 &score_rect);

  SDL_Rect score_contents_rect = {
      config::score_position_vec2_v.x + score_rect.w,
      config::score_position_vec2_v.y,
      static_cast<int32_t>(std::to_string(m_score).size() * ui::letter_width_v),
      config::font_size_v};

  SDL_RenderCopy(m_renderer.get(), m_score_texture.get(), nullptr,
                 &score_contents_rect);

  static SDL_Rect lives_rect{
      config::life_position_vec2_v.x, config::life_position_vec2_v.y,
      (std::string{"Lives: "}.size() * ui::letter_width_v),
      config::font_size_v};

  SDL_RenderCopy(m_renderer.get(), m_static_text.at(1).get(), nullptr,
                 &lives_rect);

  SDL_Rect lives_contents_rect = {
      config::life_position_vec2_v.x + lives_rect.w,
      config::life_position_vec2_v.y,
      static_cast<int32_t>(std::to_string(m_lives).size() * ui::letter_width_v),
      config::font_size_v};

  SDL_RenderCopy(m_renderer.get(), m_lives_texture.get(), nullptr,
                 &lives_contents_rect);

  for (auto &word : m_current_words)
  {
    ui::UniqueTexture text;
    if (word.type == WordType::Normal)
    {
      if (word.position.x > config::window_width_v * 0.5f &&
          word.position.x < config::window_width_v * 0.70f)
      {
        text = ui::load_word(m_font.get(), m_renderer.get(), word.value.data(),
                             config::warning_color_v);
      }
      else if (word.position.x > config::window_width_v * 0.70f)
      {
        text = ui::load_word(m_font.get(), m_renderer.get(), word.value.data(),
                             config::critical_color_v);
      }
      else
      {
        text = ui::load_word(m_font.get(), m_renderer.get(), word.value.data(),
                             config::text_color_v);
      }
    }

    // if (word.type == WordType::Normal)
    // {
    //   if (word.position.x < config::window_width_v * 0.5)
    //   {
    //     text = ui::load_word(
    //         m_font.get(), m_renderer.get(), word.value.data(),
    //         SDL_Color{255,
    //                   std::clamp(
    //                       static_cast<int32_t>((config::window_width_v *
    //                       0.5f) -
    //                                            word.position.x),
    //                       186, 255),
    //                   std::clamp(
    //                       static_cast<int32_t>((config::window_width_v *
    //                       0.5f) -
    //                                            word.position.x),
    //                       3, 255)});
    //   }
    //   else
    //   {
    //     text = ui::load_word(
    //         m_font.get(), m_renderer.get(), word.value.data(),
    //         SDL_Color{255,
    //                   std::clamp(
    //                       static_cast<int32_t>((config::window_width_v *
    //                       0.5f) -
    //                                            word.position.x),
    //                       0, 255),
    //                   std::clamp(
    //                       static_cast<int32_t>((config::window_width_v *
    //                       0.5f) -
    //                                            word.position.x),
    //                       0, 255)});
    //   }

    else if (word.type == WordType::Reverse)
    {
      text = ui::load_word(m_font.get(), m_renderer.get(), word.value.data(),
                           config::reverse_color_v);
    }

    SDL_Rect text_rect{word.position.x, word.position.y,
                       word.value.size() * ui::letter_width_v, 32};

    SDL_RenderCopy(m_renderer.get(), text.get(), nullptr, &text_rect);
  }

  m_particle_system.render(m_renderer.get());

  SDL_RenderPresent(m_renderer.get());
}

void Game::generate_word()
{
  std::string_view random_word = m_wordlist.at(rng::i64(0, m_wordlist.size()));
  m_current_cursor = {rng::f32(-350.0f, -200.0f),
                      ((m_current_cursor.y >= 500)
                           ? 20
                           : m_current_cursor.y + config::font_size_v)};
  WordType type = WordType::Normal;
  if (rng::f32(0.0f, 1.0f) > 0.95f)
    type = WordType::Reverse;
  m_current_words.emplace_back(Word{type, random_word, m_current_cursor});
}

SDL_Renderer *Game::get_renderer() { return m_renderer.get(); }
bool Game::is_running() const { return m_running; }
