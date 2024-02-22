#pragma once

#include "vec2.hpp"
#include <SDL2/SDL.h>
#include <chrono>
#include <vector>

struct Particle
{
  Vec2<float> position{};
  Vec2<float> velocity{};
  Vec2<float> acceleration{};
  Vec2<float> size{};
  SDL_Color color{};
  float lifespan{};
  float decay_rate{};

  void update(std::chrono::milliseconds delta);
  void render(SDL_Renderer *renderer);
  bool is_dead() const;
};

class ParticleSystem
{
public:
  ParticleSystem();
  ParticleSystem(Vec2<int32_t> position, size_t num_particles);
  void update(std::chrono::milliseconds dt);
  void render(SDL_Renderer *renderer);
  void set_position(Vec2<int32_t> position);
  bool is_dead() const;

protected:
  Vec2<int32_t> m_position{};
  std::vector<Particle> m_particles{};
  bool m_active{};
};
