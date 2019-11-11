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
class b2Body;

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
	explicit GameEntity( GameWorld* gameWorld, float maxSpeed );
	virtual ~GameEntity();
	virtual void init()		= 0;
	virtual void teardown() = 0;

protected:
	virtual void wrapScreenPosition() = 0;

protected:
	GameWorld* m_gameWorld;
	std::unique_ptr< sf::Shape > m_graphicalBody;
	b2Body* m_physicalBody;

	Vec m_lookDirection;
	Vec m_sideDirection;

	const float m_maxForceValue;
};

} // namespace Graphics
} // namespace SteeringBehaviors
