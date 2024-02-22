#pragma once

#include "ui/ui.hpp"

namespace ui
{

class Text final : public Widget
{
public:
  Text();
  Text(const std::string &value, SDL_Renderer *renderer, TTF_Font *font,
       Vec2<int32_t> position, int32_t height, bool active = false);

  void handle_events(SDL_Event &event);
  void render();
  void enable(bool active);
  std::string_view get_value() const;

protected:
  std::string m_value{};
  SDL_Rect m_font_rect{};
};

}; // namespace ui