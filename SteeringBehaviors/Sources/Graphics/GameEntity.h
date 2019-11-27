#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "../Input/InputHandling.h"

#include "..\Math\Vector2.h"

#include <memory>

namespace sf
{
class Shape;
} // namespace sf

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class GameEntity : public Updatable, public Renderable, public Input::InputHandling
{
protected:
public:
	GameEntity( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position );
	virtual ~GameEntity();
	virtual void init()		= 0;
	virtual void teardown() = 0;

	virtual Math::Vector2 getPosition() const;
	virtual Math::Vector2 getVelocity() const;
	virtual Math::Vector2 getLookDirection() const;
	virtual Math::Vector2 getSideDirection() const;
	virtual sf::Shape* getGraphicalBody() const;
	virtual float getMaxSpeed() const;
	virtual float getSpeed() const;
	virtual GameWorld* getWorld();
	virtual float getRadius();
	virtual float getMaxForce() const
	{
		return m_maxForce;
	}

	bool isTagged() const;
	void tag();
	void untag();

protected:
	virtual void initGfxPart()		= 0;
	virtual void initPhysicalPart() = 0;

	virtual void wrapScreenPosition() = 0;

protected:
	GameWorld* m_gameWorld;
	std::unique_ptr< sf::Shape > m_graphicalBody;

	Math::Vector2 m_velocity;
	Math::Vector2 m_lookDirection;
	Math::Vector2 m_sideDirection;

	float m_mass{ 1.0f };
	float m_radius;
	float m_maxSpeed;
	float m_maxForce;
	float m_maxTurnRate;

	bool m_tagged{ false };

	Math::Vector2 m_position;
	Math::Vector2 m_targetDirection;
};

} // namespace Graphics
} // namespace SteeringBehaviors
