#pragma once

#include "ui.hpp"
#include "vec2.hpp"
#include <string>
#include <vector>

namespace ui
{
class Selection final : public Widget
{
public:
  Selection();
  Selection(SDL_Renderer *renderer, TTF_Font *font, Vec2<int32_t> position,
            int32_t height, const std::vector<std::string> &values);

  void handle_events(SDL_Event &event);
  void render();

protected:
  UniqueTexture m_left_arrow{};
  UniqueTexture m_right_arrow{};
  SDL_Rect m_left_arrow_rect{};
  SDL_Rect m_right_arrow_rect{};
  std::vector<UniqueTexture> m_value_textures{};
  std::vector<std::string> m_values{};
  size_t m_selected{};
};
}; // namespace ui
