#pragma once

#include "Renderable.h"
#include "Updatable.h"
#include "../Input/InputHandling.h"

#include "..\Math\Vector2.h"

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
	GameEntity( GameWorld* gameWorld, const Math::Vector2& position, float radius );
	virtual ~GameEntity();

	virtual void init()		= 0;
	virtual void teardown() = 0;

	virtual Vector2 getPosition() const;
	virtual sf::Shape* getGraphicalBody() const;
	virtual GameWorld* getWorld();
	virtual float getRadius();

	bool isTagged() const;
	void tag();
	void untag();

protected:
	virtual void initGfxPart()		  = 0;
	virtual void initPhysicalPart()	  = 0;
	virtual void wrapScreenPosition() = 0;

protected:
	GameWorld* m_gameWorld;
	std::unique_ptr< sf::Shape > m_graphicalBody;

	float m_radius;
	bool m_tagged{ false };

	Vector2 m_position;
	Vector2 m_targetDirection;
};

} // namespace Graphics
} // namespace SteeringBehaviors
