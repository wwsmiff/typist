#pragma once
#include "vec2.hpp"
#include <string>

enum class WordType : uint8_t
{
  Normal = 0x0,
  Reverse
};

struct Word
{
  WordType type{};
  std::string_view value{};
  Vec2<float> position{};
};