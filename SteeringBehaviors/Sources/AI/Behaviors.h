#pragma once

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

	Behaviors( Graphics::Enemy& enemy );

	Vector2 calculate();
	Vector2 seek( const Vector2& targetPos );
	Vector2 flee( const Vector2& targetPos );
	Vector2 arrive( const Vector2& targetPos, Deceleration deceleration );
	Vector2 pursuit( const shared_ptr< const Graphics::Player >& evader );
	Vector2 evade( const shared_ptr< const Graphics::Player >& pursuer );
	Vector2 wander();
	Vector2 obstacleAvoidance( const vector< shared_ptr< Graphics::Obstacle > >& obstacles );
	Vector2 wallAvoidance( const vector< shared_ptr< Graphics::Wall > >& walls );
	Vector2 hide( const shared_ptr< const Graphics::Player >& targetPos,
				  const vector< shared_ptr< Graphics::Obstacle > >& obstacles );
	Vector2 separation( const vector< shared_ptr< Graphics::Enemy > >& neighbors );
	Vector2 alignment( const vector< shared_ptr< Graphics::Enemy > >& neightbors );
	Vector2 cohension( const vector< shared_ptr< Graphics::Enemy > >& neighbors );
	bool accumulateForce( Vector2& runningTot, Vector2 forceToAdd );

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
	Graphics::Enemy& m_enemy;
	Vector2 m_steeringForce;
	Vector2 m_wanderTarget;
	
	vector< Vector2 > m_feelers;

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

	constexpr static inline float m_minDetectionBoxLenght{ 40.0f };
	constexpr static inline float m_wanderRadius{ 1.2f };
	constexpr static inline float m_wanderDistance{ 2.f };
	constexpr static inline float m_wanderJitter{ 40.0f };
	constexpr static inline float m_wallDetectionFeelersLen{ 40.0f };
	constexpr static inline float m_viewDistance{ 50.0f };

	constexpr static inline float m_separationWeight		= 200.0f * 2.0f;
	constexpr static inline float m_alignmentWeight			= 200.0f * 1.0f;
	constexpr static inline float m_cohensionWeight			= 200.0f * 1.0f;
	constexpr static inline float m_wanderWeight			= 200.0f * 1.0f;
	constexpr static inline float m_fleeWeight				= 200.0f * 1.0f;
	constexpr static inline float m_evadeWeight				= 200.0f * 0.01f;
	constexpr static inline float m_obstacleAvoidanceWeight = 200.0f * 10.0f;
	constexpr static inline float m_wallAvoidanceWeight		= 200.0f * 10.0f;
	constexpr static inline float m_seekWeight				= 200.0f * 1.0f;
};
} // namespace SteeringBehaviors::AI
