#pragma once

#include "vec2.hpp"
#include <SDL2/SDL_rect.h>
#include <functional>
#include <memory>
#include <string>

struct SDL_Renderer;
struct SDL_Texture;
union SDL_Event;
typedef struct _TTF_Font TTF_Font;

namespace ui
{
using UniqueTexture =
    std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>>;

UniqueTexture load_word(TTF_Font *font, SDL_Renderer *renderer,
                        const std::string &text, const SDL_Color &color);

constexpr int32_t letter_width_v{16};

class TextInput
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
  SDL_Renderer *m_renderer{};
  TTF_Font *m_font{};
  Vec2<int32_t> m_position{};
  int32_t m_height{};
  std::string m_value{};
  UniqueTexture m_lbracket_texture{};
  UniqueTexture m_rbracket_texture{};
  SDL_Rect m_lbracket_rect{};
  SDL_Rect m_rbracket_rect{};
  UniqueTexture m_font_texture{};
};

}; // namespace ui
