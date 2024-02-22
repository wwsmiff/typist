#include "ui/textinput.hpp"
#include "config.hpp"
#include <iostream>

namespace ui
{
TextInput::TextInput(SDL_Renderer *renderer, TTF_Font *font,
                     Vec2<int32_t> position, int32_t height)
    : Widget{Widget::Type::TextInput, renderer, font, position, height}
{
  m_lbracket_texture = UniqueTexture{
      load_word(m_font, m_renderer, "[", SDL_Color{255, 255, 255, 255})};
  m_rbracket_texture = UniqueTexture{
      load_word(m_font, m_renderer, "]", SDL_Color{255, 255, 255, 255})};

  m_lbracket_rect = SDL_Rect{m_position.x - ui::letter_width_v, m_position.y,
                             ui::letter_width_v, m_height};

  m_rbracket_rect =
      SDL_Rect{m_position.x + (config::word_limit_v * ui::letter_width_v),
               m_position.y, ui::letter_width_v, m_height};
}

TextInput::TextInput() : Widget{} {}

void TextInput::handle_events(SDL_Event &event)
{
  if (event.type == SDL_TEXTINPUT)
  {
    if (strcmp(event.text.text, " ") != 0 &&
        m_value.size() < config::word_limit_v)
    {
      m_value.append(event.text.text);
      m_font_texture =
          load_word(m_font, m_renderer, m_value, SDL_Color{255, 255, 255, 255});
    }
  }
  if (event.type == SDL_KEYDOWN)
  {
    if (event.key.keysym.sym == SDLK_BACKSPACE)
      if (m_value.size() > 0)
        m_value.pop_back();
    if (event.key.keysym.sym == SDLK_DELETE)
      clear();
    m_font_texture =
        load_word(m_font, m_renderer, m_value, SDL_Color{255, 255, 255, 255});
  }
}

void TextInput::render()
{
  SDL_RenderCopy(m_renderer, m_lbracket_texture.get(), nullptr,
                 &m_lbracket_rect);

  SDL_RenderCopy(m_renderer, m_rbracket_texture.get(), nullptr,
                 &m_rbracket_rect);

  SDL_Rect tmp{m_position.x, m_position.y,
               static_cast<int32_t>(m_value.size() * ui::letter_width_v),
               m_height};
  SDL_RenderCopy(m_renderer, m_font_texture.get(), nullptr, &tmp);
}

std::string_view TextInput::get_value() const { return m_value; }

void TextInput::clear() { m_value.clear(); }

void TextInput::enable(bool active) { m_active = active; }

}; // namespace ui
