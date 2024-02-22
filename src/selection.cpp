#include "ui/selection.hpp"
#include "config.hpp"
#include <iostream>

namespace ui
{
Selection::Selection() : Widget{} {}

Selection::Selection(SDL_Renderer *renderer, TTF_Font *font,
                     Vec2<int32_t> position, int32_t height,
                     const std::vector<std::string> &values, bool active)
    : Widget{Widget::Type::Selection, renderer, font, position, height, active},
      m_values{values}
{
  m_left_arrow_rect = SDL_Rect{m_position.x - (ui::letter_width_v * 2),
                               m_position.y, ui::letter_width_v, m_height};

  auto max_element =
      std::max_element(m_values.cbegin(), m_values.cend(),
                       [](const std::string &s1, const std::string &s2)
                       { return s1.size() < s2.size(); });

  m_right_arrow_rect =
      SDL_Rect{m_position.x + ((1 + max_element->size()) * ui::letter_width_v),
               m_position.y, ui::letter_width_v, m_height};

  if (m_active)
  {
    m_left_arrow = UniqueTexture{
        load_word(m_font, m_renderer, "<", config::active_widget_color_v)};
    m_right_arrow = UniqueTexture{
        load_word(m_font, m_renderer, ">", config::active_widget_color_v)};
    for (const auto &x : m_values)
    {
      m_value_textures.emplace_back(
          load_word(m_font, m_renderer, x, config::active_widget_color_v));
    }
  }
  else
  {
    m_left_arrow =
        load_word(m_font, m_renderer, "<", config::inactive_widget_color_v);
    m_right_arrow =
        load_word(m_font, m_renderer, ">", config::inactive_widget_color_v);
    for (const auto &x : m_values)
    {

      m_value_textures.emplace_back(
          load_word(m_font, m_renderer, x, config::inactive_widget_color_v));
    }
  }
}

void Selection::handle_events(SDL_Event &event)
{
  if (m_active)
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
    m_value = m_values.at(m_selected);
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

void Selection::enable(bool active)
{
  m_active = active;
  if (active)
  {
    m_left_arrow =
        load_word(m_font, m_renderer, "<", config::active_widget_color_v);
    m_right_arrow =
        load_word(m_font, m_renderer, ">", config::active_widget_color_v);
    for (size_t i{}; i < m_value_textures.size(); ++i)
    {
      m_value_textures.at(i) = load_word(m_font, m_renderer, m_values.at(i),
                                         config::active_widget_color_v);
    }
  }
  else
  {
    m_left_arrow = UniqueTexture{
        load_word(m_font, m_renderer, "<", config::inactive_widget_color_v)};
    m_right_arrow = UniqueTexture{
        load_word(m_font, m_renderer, ">", config::inactive_widget_color_v)};

    for (size_t i{}; i < m_value_textures.size(); ++i)
    {
      m_value_textures.at(i) = load_word(m_font, m_renderer, m_values.at(i),
                                         config::inactive_widget_color_v);
    }
  }
}

std::string_view Selection::get_value() const { return m_value; }

}; // namespace ui
