#pragma once
#include "vec2.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <memory>

namespace ui
{
using UniqueTexture =
    std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>>;

UniqueTexture load_word(TTF_Font *font, SDL_Renderer *renderer,
                        const std::string &text, const SDL_Color &color);

constexpr int32_t letter_width_v{16};

class Widget
{
public:
  Widget();
  Widget(SDL_Renderer *renderer, TTF_Font *font, Vec2<int32_t> position,
         int32_t height, bool active = false);

  virtual void handle_events(SDL_Event &event) = 0;
  virtual void render() = 0;

  void enable(bool active);

protected:
  SDL_Renderer *m_renderer{};
  TTF_Font *m_font{};
  Vec2<int32_t> m_position{};
  int32_t m_height{};
  std::string m_value{};
  UniqueTexture m_font_texture{};
  bool m_active{};
};

}; // namespace ui
