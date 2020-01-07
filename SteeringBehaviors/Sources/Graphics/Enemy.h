#pragma once

#include "MovingEntity.h"

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
class Enemy : public MovingEntity
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
	virtual float getDeltaTime() const;
	void disableCounter();

protected:
	virtual void initGfxPart() override;
	virtual void initPhysicalPart() override;
	virtual void wrapScreenPosition() override;
	virtual void switchBehavior();
	void behaviorCooldownCounter();
	void handleCollisionsWithOtherEnemies( const vector< shared_ptr< Enemy > >& enemies );

	AI::Behaviors* m_steeringBehaviors;
	float m_deltaTime;
	int m_behaviorSwitchTime;
	std::chrono::steady_clock::time_point m_behaviorCooldown;
	bool m_shouldCount{ true };
};
} // namespace SteeringBehaviors::Graphics
