#pragma once

#include "..\Math\Vector2.h"

namespace SteeringBehaviors::Graphics
{
class Enemy;
class Player;
} // namespace SteeringBehaviors::Graphics

namespace SteeringBehaviors::AI
{
class Behaviors
{
public:
	enum class Deceleration
	{
		SLOW   = 3,
		NORMAL = 2,
		FAST   = 1
	};

	Behaviors( Graphics::Enemy* enemy );

	Math::Vector2 calculate();
	Math::Vector2 seek( const Math::Vector2& targetPos );
	Math::Vector2 flee( const Math::Vector2& targetPos );
	Math::Vector2 arrive( const Math::Vector2& targetPos, Deceleration deceleration );
	Math::Vector2 pursuit( const Graphics::Player* evader );
	Math::Vector2 evade( const Graphics::Player* pursuer );
	Math::Vector2 wander();

private:
	Graphics::Enemy* m_enemy{};
	Math::Vector2 m_steeringForce{};
	Math::Vector2 m_target{};

	Math::Vector2 m_wanderTarget;

	float m_wanderRadius{ 200.0f };
	float m_wanderDistance{ 300.f };
	float m_wanderJitter{ 1000.0f };
};
} // namespace SteeringBehaviors::AI
