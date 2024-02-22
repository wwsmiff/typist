#include "ui/text.hpp"
#include "config.hpp"

namespace ui
{

Text::Text() : Widget{}, m_value{} {}

Text::Text(const std::string &value, SDL_Renderer *renderer, TTF_Font *font,
           Vec2<int32_t> position, int32_t height, bool active)
    : Widget{Widget::Type::Text, renderer, font, position, height, active},
      m_value{value}
{
  m_font_rect = SDL_Rect{m_position.x, m_position.y,
                         m_value.size() * ui::letter_width_v, height};

  if (m_active)
  {
    m_font_texture =
        load_word(m_font, m_renderer, m_value, config::active_widget_color_v);
  }
  else
  {
    m_font_texture =
        load_word(m_font, m_renderer, m_value, config::inactive_widget_color_v);
  }
}

void Text::handle_events(SDL_Event &event) {}

void Text::render()
{
  SDL_RenderCopy(m_renderer, m_font_texture.get(), nullptr, &m_font_rect);
}

void Text::enable(bool active)
{
  m_active = active;

  if (m_active)
  {
    m_font_texture =
        load_word(m_font, m_renderer, m_value, config::active_widget_color_v);
  }
  else
  {
    m_font_texture =
        load_word(m_font, m_renderer, m_value, config::inactive_widget_color_v);
  }
}

std::string_view Text::get_value() const { return m_value; }

}; // namespace ui