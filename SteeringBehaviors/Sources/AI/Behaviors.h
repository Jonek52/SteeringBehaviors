#pragma once

#include "..\Math\Vector2.h"

#include <vector>

namespace SteeringBehaviors::Graphics
{
class Enemy;
class Player;
class GameEntity;
class Wall;
} // namespace SteeringBehaviors::Graphics

namespace SteeringBehaviors::AI
{
class Behaviors final
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
	Math::Vector2 obstacleAvoidance( const std::vector< Graphics::GameEntity* >& obstacles );
	Math::Vector2 wallAvoidance( const std::vector< Graphics::Wall* >& walls );
	Math::Vector2 getHidingPosition( const Math::Vector2& obstaclePosition,
									 float obstacleRadius,
									 const Math::Vector2& targetPos );
	Math::Vector2 hide( const Graphics::Player* targetPos, std::vector< Graphics::GameEntity* >& obstacles );

private:
	void createFeelers();

private:
	Graphics::Enemy* m_enemy{};
	Math::Vector2 m_steeringForce{};
	Math::Vector2 m_target{};

	Math::Vector2 m_wanderTarget{};

	const float m_minDetectionBoxLenght{ 50.0f };
	float m_boxLenght;
	float m_wanderRadius{ 30.0f };
	float m_wanderDistance{ 200.f };
	float m_wanderJitter{ 40.0f };

	std::vector< Math::Vector2 > m_feelers;
	float m_wallDetectionFeelersLen{ 50.0f };
};
} // namespace SteeringBehaviors::AI
