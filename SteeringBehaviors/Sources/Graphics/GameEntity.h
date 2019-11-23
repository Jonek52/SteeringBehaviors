#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "../Input/InputHandling.h"

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
public:
	GameEntity( GameWorld* gameWorld, float maxForceValue );
	virtual ~GameEntity();
	virtual void init()		= 0;
	virtual void teardown() = 0;

protected:
	virtual void initGfxPart()		= 0;
	virtual void initPhysicalPart() = 0;

	virtual void wrapScreenPosition() = 0;

protected:
	GameWorld* m_gameWorld;
	std::unique_ptr< sf::Shape > m_graphicalBody;

	const float m_maxForceValue;
};

} // namespace Graphics
} // namespace SteeringBehaviors
