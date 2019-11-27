#pragma once

#include "GameEntity.h"

#include "..\AI\Behaviors.h"

namespace sf
{
class Shape;
}

namespace SteeringBehaviors::AI
{
class Behaviors;
} // namespace SteeringBehaviors::AI

namespace SteeringBehaviors::Graphics
{
class GameWorld;
class Enemy : public GameEntity
{
public:
	Enemy( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position );
	virtual ~Enemy();

	virtual void init() override;
	virtual void teardown() override;
	virtual void update( float delta ) override;
	virtual void render( RenderWindow* ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

	virtual void turnBehaviorOn( AI::Behaviors::Behavior behavior );
	virtual void turnBehaviorOff( AI::Behaviors::Behavior behavior );

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;
	virtual void wrapScreenPosition() override;

	AI::Behaviors* m_steeringBehaviors;
};
} // namespace SteeringBehaviors::Graphics
