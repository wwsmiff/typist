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
    SDL_Rect tmp{position.x, position.y, size.x, size.y};
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
        {rng::f32(-0.3f, 0.3f), rng::f32(0.0f, 0.05f)}, {0, 0.0000000001f},
        {3, 3}, {rng::i32(0, 255), rng::i32(0, 255), rng::i32(0, 255), 0},
        {255.0f}, {0.01f}));
  }
}

void ParticleSystem::update(std::chrono::milliseconds dt)
{
  if (m_active)
    for (auto &particle : m_particles)
      particle.update(dt);
}

void ParticleSystem::render(SDL_Renderer *renderer)
{
  if (m_active)
    for (auto &particle : m_particles)
      particle.render(renderer);
}

void ParticleSystem::set_position(Vec2<int32_t> position)
{
  m_position = position;
}

void ParticleSystem::emit()
{
  m_active = true;
  for (size_t i = 0; i < m_particles.size(); ++i)
  {
    m_particles.at(i) = Particle(
        {static_cast<float>(m_position.x), static_cast<float>(m_position.y)},
        {rng::f32(-0.3f, 0.3f), rng::f32(0.0f, 0.7f)}, {0, 0.0001f}, {3, 3},
        {rng::i32(0, 255), rng::i32(0, 255), rng::i32(0, 255), 0}, {255.0f},
        {0.01f});
  }
}
