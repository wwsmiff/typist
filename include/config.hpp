#pragma once

#include "vec2.hpp"
#include <cstdint>
#include <string_view>

namespace config
{
constexpr std::string_view window_title{"Typist"};
constexpr int32_t window_width_v{1280};
constexpr int32_t window_height_v{720};
constexpr std::string_view font_name_v{"JetBrainsMonoNerdFont-Bold.ttf"};
constexpr int32_t font_size_v{32};
constexpr Vec2<int32_t> text_input_position_vec2_v{50, window_height_v - 100};
constexpr int32_t word_limit_v{45};
constexpr float text_velocity_v{0.03f};
constexpr int32_t max_words_on_screen_v{13};
constexpr Vec2<int32_t> score_position_vec2_v{
    50 + (text_input_position_vec2_v.x + 50 * 16), window_height_v - 100};
constexpr Vec2<int32_t> life_position_vec2_v{score_position_vec2_v.x,
                                             score_position_vec2_v.y + 50};
constexpr SDL_Color text_color_v{255, 255, 255, 255};
constexpr SDL_Color warning_color_v{252, 186, 3, 255};
constexpr SDL_Color critical_color_v{191, 80, 80, 255};
constexpr SDL_Color reverse_color_v{0, 204, 197, 255};
}; // namespace config
