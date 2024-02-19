#include "ui.hpp"
#include "config.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

namespace
{

} // namespace

namespace ui
{
UniqueTexture load_word(TTF_Font *font, SDL_Renderer *renderer,
                        const std::string &text, const SDL_Color &color)
{
  SDL_Surface *font_surface = TTF_RenderText_Blended(font, text.c_str(), color);
  ui::UniqueTexture font_texture{
      SDL_CreateTextureFromSurface(renderer, font_surface), SDL_DestroyTexture};
  SDL_FreeSurface(font_surface);
  return font_texture;
}

TextInput::TextInput(SDL_Renderer *renderer, TTF_Font *font,
                     Vec2<int32_t> position, int32_t height)
    : m_renderer{renderer}, m_font{font}, m_position{position},
      m_height{height}, m_value{}, m_font_texture{}
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

TextInput::TextInput()
    : m_renderer{}, m_font{}, m_position{}, m_height{}, m_value{},
      m_font_texture{}
{
}

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

  SDL_Rect tmp{m_position.x, m_position.y, m_value.size() * ui::letter_width_v,
               m_height};
  SDL_RenderCopy(m_renderer, m_font_texture.get(), nullptr, &tmp);
}

std::string_view TextInput::get_value() const { return m_value; }

void TextInput::clear() { m_value.clear(); }

}; // namespace ui
