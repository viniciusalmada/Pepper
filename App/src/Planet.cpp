#include "Planet.hpp"

#include "Random.hpp"

#include <utility>

void Planet::Update(float newXPosition, Pepper::Ref<Pepper::Texture2D> newTexture)
{
  PP_PROFILE_FUNCTION()
  m_position.x = newXPosition;
  m_texture = std::move(newTexture);
  m_size.y = Random::Float(1.0f, 10.0f);
  m_size.x = m_size.y;
  m_position.y = Random::Float(-5.0f, 5.0f);
}

const glm::vec3& Planet::GetPosition() const
{
  return m_position;
}

void Planet::OnRendererCall() const
{
  Pepper::Renderer2D::DrawPixelateQuad(m_position, m_size, m_texture, m_size.x * 20.0f);
}

float Planet::GetRadius() const
{
  return m_size.x / 2.0f;
}

bool Planet::operator==(const Planet& other) const
{
  return m_size == other.m_size && m_texture == other.m_texture && m_position == other.m_position;
}
