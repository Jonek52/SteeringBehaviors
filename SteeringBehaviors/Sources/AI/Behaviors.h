#pragma once

#include "..\Math\Vector2.h"

#include <vector>

namespace SteeringBehaviors::Graphics
{
class Enemy;
class Player;
class GameEntity;
class Wall;
class Obstacle;
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

	enum class Behavior
	{
		SEEK,
		FLEE,
		ARRIVE,
		PURSUIT,
		EVADE,
		WANDER,
		OBSTACLE_AVOIDANCE,
		WALL_AVOIDANCE,
		HIDE,
		SEPARATION,
		ALIGNMENT,
		COHESION
	};

	Behaviors( Graphics::Enemy* enemy );

	Math::Vector2 calculate();
	Math::Vector2 seek( const Math::Vector2& targetPos );
	Math::Vector2 flee( const Math::Vector2& targetPos );
	Math::Vector2 arrive( const Math::Vector2& targetPos, Deceleration deceleration );
	Math::Vector2 pursuit( const Graphics::Player* evader );
	Math::Vector2 evade( const Graphics::Player* pursuer );
	Math::Vector2 wander();
	Math::Vector2 obstacleAvoidance( const std::vector< Graphics::Obstacle* >& obstacles );
	Math::Vector2 wallAvoidance( const std::vector< Graphics::Wall* >& walls );
	Math::Vector2 hide( const Graphics::Player* targetPos, std::vector< Graphics::Obstacle* >& obstacles );
	Math::Vector2 separation( const std::vector< Graphics::Enemy* >& neighbors );
	Math::Vector2 alignment( const std::vector< Graphics::Enemy* >& neightbors );
	Math::Vector2 cohension( const std::vector< Graphics::Enemy* >& neighbors );
	bool accumulateForce( Math::Vector2& runningTot, Math::Vector2 forceToAdd );

	void turnBehaviorOn( Behavior behavior );
	void turnBehaviorOff( Behavior behavior );
	bool isOn( Behavior behavior );

private:
	void createFeelers();

protected:
	Math::Vector2 getHidingPosition( const Math::Vector2& obstaclePosition,
									 float obstacleRadius,
									 const Math::Vector2& targetPos );

private:
	Graphics::Enemy* m_enemy{};
	Math::Vector2 m_steeringForce{};
	Math::Vector2 m_target{};

	Math::Vector2 m_wanderTarget{};

	const float m_minDetectionBoxLenght{ 40.0f };
	float m_boxLenght;
	float m_wanderRadius{ 1.2f };
	float m_wanderDistance{ 2.f };
	float m_wanderJitter{ 40.0f };

	std::vector< Math::Vector2 > m_feelers;
	float m_wallDetectionFeelersLen{ 40.0f };
	float m_viewDistance{ 50.0f };

	Math::Vector2 m_seekTarget;
	Math::Vector2 m_fleeTarget;
	Math::Vector2 m_arriveTarget;
	const Graphics::Player* m_evader;
	const Graphics::Player* m_pursuer;
	const Graphics::Player* m_hideTarget;

	bool m_seekOn{ false };
	bool m_fleeOn{ false };
	bool m_arriveOn{ false };
	bool m_pursuitOn{ false };
	bool m_evadeOn{ false };
	bool m_wanderOn{ false };
	bool m_obstacleAvoidanceOn{ false };
	bool m_wallAvoidanceOn{ false };
	bool m_hideOn{ false };
	bool m_separationOn{ false };
	bool m_alignmentOn{ false };
	bool m_cohesionOn{ false };

	float m_separationWeight		= 200.0f * 3.0f;
	float m_alignmentWeight			= 200.0f * 1.0f;
	float m_cohensionWeight			= 200.0f * 1.0f;
	float m_wanderWeight			= 200.0f * 1.0f;
	float m_fleeWeight				= 200.0f * 1.0f;
	float m_evadeWeight				= 200.0f * 0.01f;
	float m_obstacleAvoidanceWeight = 200.0f * 10.0f;
	float m_wallAvoidanceWeight		= 200.0f * 10.0f;
	float m_seekWeight				= 200.0f * 1.0f;
};
} // namespace SteeringBehaviors::AI
