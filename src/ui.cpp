#include "ui/ui.hpp"

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

Widget::Widget()
    : m_type{Widget::Type::None}, m_renderer{}, m_font{}, m_position{},
      m_height{}, m_font_texture{}

{
}

Widget::Widget(Type type, SDL_Renderer *renderer, TTF_Font *font,
               Vec2<int32_t> position, int32_t height, bool active)
    : m_type{type}, m_renderer{renderer}, m_font{font}, m_position{position},
      m_height{height}, m_font_texture{}, m_active{active}
{
}

Widget::Type Widget::get_type() const { return m_type; }

}; // namespace ui