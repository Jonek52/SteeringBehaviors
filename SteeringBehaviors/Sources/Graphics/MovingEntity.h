#pragma once

#include "GameEntity.h"

namespace SteeringBehaviors::Graphics
{
class MovingEntity : public GameEntity
{
public:
	MovingEntity( GameWorld* gameWorld, float maxSpeed, float maxForce, float m_radius, const Math::Vector2& position );
	virtual ~MovingEntity() = default;

	virtual Vector2 getLookDirection() const;
	virtual Vector2 getSideDirection() const;

	virtual Vector2 getVelocity() const;
	virtual float getSpeed() const;
	virtual float getMaxSpeed() const;
	virtual float getMaxForce() const;

protected:
	Vector2 m_velocity;
	Vector2 m_lookDirection;
	Vector2 m_sideDirection;

	const float m_mass;
	const float m_maxSpeed;
	const float m_maxForce;
};
} // namespace SteeringBehaviors::Graphics
