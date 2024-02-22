#pragma once

#include "ui/textinput.hpp"
#include "ui/ui.hpp"
#include "vec2.hpp"
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace ui
{
class TextInput final : public Widget
{
public:
  TextInput();
  TextInput(SDL_Renderer *renderer, TTF_Font *font, Vec2<int32_t> position,
            int32_t height);
  void handle_events(SDL_Event &event);
  void render();
  std::string_view get_value() const;
  void clear();

protected:
  UniqueTexture m_lbracket_texture{};
  UniqueTexture m_rbracket_texture{};
  SDL_Rect m_lbracket_rect{};
  SDL_Rect m_rbracket_rect{};
};

}; // namespace ui
