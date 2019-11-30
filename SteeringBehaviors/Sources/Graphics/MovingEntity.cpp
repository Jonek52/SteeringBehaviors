#include "stdafx.h"

#include "MovingEntity.h"

namespace SteeringBehaviors::Graphics
{

MovingEntity::MovingEntity(
	GameWorld* gameWorld, float maxSpeed, float maxForce, float radius, const Math::Vector2& position )
	: GameEntity( gameWorld, position, radius ), m_mass{ 1.0f }, m_maxSpeed{ maxSpeed }, m_maxForce{ maxForce }
{
}

Vector2 MovingEntity::getLookDirection() const
{
	return m_lookDirection;
}

Vector2 MovingEntity::getSideDirection() const
{
	return m_sideDirection;
}

Vector2 MovingEntity::getVelocity() const
{
	return m_velocity;
}

float MovingEntity::getSpeed() const
{
	return m_velocity.length();
}

float MovingEntity::getMaxSpeed() const
{
	return m_maxSpeed;
}

float MovingEntity::getMaxForce() const
{
	return m_maxForce;
}

} // namespace SteeringBehaviors::Graphics
