#include "particles.hpp"
#include "random.hpp"

#include <iostream>

void Particle::update(std::chrono::milliseconds delta)
{
  if (!is_dead())
  {
    velocity.x += acceleration.x * delta.count();
    velocity.y += acceleration.y * delta.count();
    position.x += velocity.x * delta.count();
    position.y += velocity.y * delta.count();
    lifespan -= decay_rate * delta.count();
  }
}

void Particle::render(SDL_Renderer *renderer)
{
  if (!is_dead())
  {
    SDL_Rect tmp{static_cast<int32_t>(position.x),
                 static_cast<int32_t>(position.y), size.x, size.y};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, lifespan);
    SDL_RenderFillRect(renderer, &tmp);
  }
}

bool Particle::is_dead() const { return lifespan < 0.0f; }

ParticleSystem::ParticleSystem()
    : m_position{Vec2<int32_t>{0, 0}}, m_particles{}
{
}

ParticleSystem::ParticleSystem(Vec2<int32_t> position, size_t num_particles)
    : m_position{position}, m_particles{}, m_active{false}
{
  for (size_t i = 0; i < num_particles; ++i)
  {
    m_particles.emplace_back(Particle(
        {static_cast<float>(m_position.x), static_cast<float>(m_position.y)},
        {rng::f32(-0.3f, 0.3f), rng::f32(0.0f, 0.5f)}, {0, 1e-5}, {3, 3},
        {static_cast<uint8_t>(rng::i32(0, 255)),
         static_cast<uint8_t>(rng::i32(0, 255)),
         static_cast<uint8_t>(rng::i32(0, 255)), 0},
        255.0f, 0.1f));
  }
}

void ParticleSystem::update(std::chrono::milliseconds dt)
{
  for (auto &particle : m_particles)
    particle.update(dt);
}

void ParticleSystem::render(SDL_Renderer *renderer)
{
  for (auto &particle : m_particles)
    particle.render(renderer);
}

void ParticleSystem::set_position(Vec2<int32_t> position)
{
  m_position = position;
}

bool ParticleSystem::is_dead() const
{
  return static_cast<size_t>(
             std::count_if(m_particles.cbegin(), m_particles.cend(),
                           [](const Particle &p) { return p.is_dead(); })) ==
         m_particles.size();
}