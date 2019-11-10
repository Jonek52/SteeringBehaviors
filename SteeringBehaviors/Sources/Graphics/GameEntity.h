#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "../Input/InputHandling.h"

#include "Box2D\Common\b2Math.h"

#include <memory>

namespace sf
{
class Shape;
}
namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class GameEntity : public Updatable, public Renderable, public Input::InputHandling
{
protected:
	using Vec = b2Vec2;

public:
	explicit GameEntity( GameWorld* gameWorld, Vec position, float maxSpeed );
	virtual ~GameEntity();
	virtual void init()		= 0;
	virtual void teardown() = 0;

protected:
	virtual void clampScreenPosition() = 0;

protected:
	GameWorld* m_gameWorld;
	std::unique_ptr< sf::Shape > m_objectGfxShape;

	Vec m_position;
	Vec m_velocity;
	Vec m_lookDirection;
	Vec m_rightDirection;

	const float m_maxSpeed;
};

} // namespace Graphics
} // namespace SteeringBehaviors
