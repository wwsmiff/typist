#include "ui/selection.hpp"
#include "config.hpp"
#include <iostream>

namespace ui
{
Selection::Selection() : Widget{} {}

Selection::Selection(SDL_Renderer *renderer, TTF_Font *font,
                     Vec2<int32_t> position, int32_t height,
                     const std::vector<std::string> &values)
    : Widget{renderer, font, position, height}, m_values{values}
{
  m_left_arrow = UniqueTexture{
      load_word(m_font, m_renderer, "<", SDL_Color{255, 255, 255, 255})};
  m_right_arrow = UniqueTexture{
      load_word(m_font, m_renderer, ">", SDL_Color{255, 255, 255, 255})};

  m_left_arrow_rect = SDL_Rect{m_position.x - (ui::letter_width_v * 2),
                               m_position.y, ui::letter_width_v, m_height};

  auto max_element = std::max_element(m_values.cbegin(), m_values.cend());

  m_right_arrow_rect =
      SDL_Rect{m_position.x + ((1 + max_element->size()) * ui::letter_width_v),
               m_position.y, ui::letter_width_v, m_height};

  for (const auto &x : m_values)
  {
    m_value_textures.emplace_back(
        load_word(m_font, m_renderer, x, SDL_Color{255, 255, 255, 255}));
  }
}

void Selection::handle_events(SDL_Event &event)
{
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.sym == SDLK_RIGHT)
    {
      m_selected = (m_selected >= (m_values.size() - 1)) ? m_values.size() - 1
                                                         : m_selected + 1;
    }
    else if (event.key.keysym.sym == SDLK_LEFT)
    {
      m_selected = (m_selected <= 0) ? 0 : m_selected - 1;
    }
  }
}

void Selection::render()
{
  SDL_RenderCopy(m_renderer, m_left_arrow.get(), nullptr, &m_left_arrow_rect);
  SDL_RenderCopy(m_renderer, m_right_arrow.get(), nullptr, &m_right_arrow_rect);

  SDL_Rect selected_rect{
      ui::letter_width_v / 2 +
          (m_left_arrow_rect.x +
           ((m_right_arrow_rect.x - m_left_arrow_rect.x) / 2)) -
          (m_values.at(m_selected).size() * ui::letter_width_v) / 2,
      m_position.y, m_values.at(m_selected).size() * ui::letter_width_v,
      m_height};

  SDL_RenderCopy(m_renderer, m_value_textures.at(m_selected).get(), nullptr,
                 &selected_rect);
}

}; // namespace ui
