#pragma once

#include "GameEntity.h"

namespace sf
{
class Shape;
}

namespace SteeringBehaviors::AI
{
class Behaviors;
}

namespace SteeringBehaviors::Graphics
{
class GameWorld;
class Enemy : public GameEntity
{
public:
	Enemy( GameWorld* gameWorld, float maxSpeed );
	virtual ~Enemy();

	virtual void init() override;
	virtual void teardown() override;
	virtual void update( float delta ) override;
	virtual void render( RenderWindow* ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;
	virtual void wrapScreenPosition() override;

	AI::Behaviors* m_steeringBehaviors;
};
} // namespace SteeringBehaviors::Graphics
