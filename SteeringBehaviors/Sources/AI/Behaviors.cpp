#include "Behaviors.h"

#include "..\Graphics\Enemy.h"
#include "..\Graphics\Player.h"
#include "..\Graphics\GameWorld.h"
#include "..\Graphics\GameEntity.h"
#include "..\Graphics\Wall.h"

#include "..\Math\MathFunctions.h"
#include "..\Math\Matrix.h"
#include "..\Math\Transformations.h"

#include <math.h>
#include <algorithm>
namespace SteeringBehaviors::AI
{

Behaviors::Behaviors( Graphics::Enemy* enemy ) : m_enemy{ enemy }
{
	m_wanderTarget = Math::Vector2{ enemy->getPosition() };
}
Math::Vector2 Behaviors::calculate()
{
	m_steeringForce.zero();
	m_steeringForce += seek( m_enemy->getWorld()->getPlayer()->getPosition() );
	m_steeringForce += obstacleAvoidance( m_enemy->getWorld()->getObstacles() );
	return m_steeringForce;
}

Math::Vector2 Behaviors::seek( const Math::Vector2& targetPosition )
{
	Math::Vector2 desiredVelocity = Math::normalize( targetPosition - m_enemy->getPosition() ) * m_enemy->getMaxSpeed();
	return desiredVelocity - m_enemy->getVelocity();
}

Math::Vector2 Behaviors::flee( const Math::Vector2& targetPosition )
{
	const float fleeDistanceSquared = 200.0f * 200.0f;
	if( float distance = Math::vecDistanceSquared( m_enemy->getPosition(), targetPosition );
		distance > fleeDistanceSquared )
	{
		return Math::Vector2{};
	}

	Math::Vector2 desiredVelocity = Math::normalize( m_enemy->getPosition() - targetPosition ) * m_enemy->getMaxSpeed();
	return desiredVelocity - m_enemy->getVelocity();
}

Math::Vector2 Behaviors::arrive( const Math::Vector2& targetPos, Deceleration deceleration )
{

	Math::Vector2 toTarget = targetPos - m_enemy->getPosition();
	float distance		   = toTarget.length();

	if( distance > 0.0f )
	{
		const float decelarationTweaker = 0.3f;

		float speed = distance / ( static_cast< float >( deceleration ) * decelarationTweaker );
		speed		= std::min( speed, m_enemy->getMaxSpeed() );

		Math::Vector2 desiredVelocity = toTarget * speed / distance;
		return desiredVelocity - m_enemy->getVelocity();
	}

	return Math::Vector2{};
}

Math::Vector2 Behaviors::pursuit( const Graphics::Player* evader )
{
	Math::Vector2 toEvader = evader->getPosition() - m_enemy->getPosition();

	float relativeHeading = m_enemy->getLookDirection().dot( evader->getLookDirection() );

	if( ( toEvader.dot( m_enemy->getLookDirection() ) > 0 ) && ( relativeHeading < -0.95f ) )
	{
		return seek( evader->getPosition() );
	}

	float lookAheadTime = toEvader.length() / ( m_enemy->getMaxSpeed() + evader->getSpeed() );

	return seek( evader->getPosition() + evader->getVelocity() * lookAheadTime );
}

Math::Vector2 Behaviors::evade( const Graphics::Player* pursuer )
{
	Math::Vector2 toPursuer = pursuer->getPosition() - m_enemy->getPosition();

	float lookAheadTime = toPursuer.length() / ( m_enemy->getMaxSpeed() + pursuer->getMaxSpeed() );
	return flee( pursuer->getPosition() + pursuer->getVelocity() * lookAheadTime );
}

Math::Vector2 Behaviors::wander()
{
	m_wanderTarget += Math::Vector2{ Math::randomClamped() * m_wanderJitter, Math::randomClamped() * m_wanderJitter };
	m_wanderTarget.normalize();
	m_wanderTarget *= m_wanderRadius;

	Math::Vector2 targetLocal = m_wanderTarget + Math::Vector2{ m_wanderDistance, 0.f };

	Math::Vector2 targetWorld = Math::PointToWorldSpace(
		targetLocal, m_enemy->getLookDirection(), m_enemy->getSideDirection(), m_enemy->getPosition() );

	return targetWorld - m_enemy->getPosition();
}

SteeringBehaviors::Math::Vector2 Behaviors::obstacleAvoidance( const std::vector< Graphics::GameEntity* >& obstacles )
{
	m_boxLenght = m_minDetectionBoxLenght + ( m_enemy->getSpeed() / m_enemy->getMaxSpeed() ) * m_minDetectionBoxLenght;

	m_enemy->getWorld()->tagObstaclesWithinRange( m_enemy, m_boxLenght );

	Graphics::GameEntity* closestIntersectionObstacle = nullptr;
	float distanceToClosestIP						  = std::numeric_limits< float >::max();
	Math::Vector2 localPosOfClosestObstacle{};

	for( auto& obstacle : obstacles )
	{
		if( obstacle->isTagged() )
		{
			Math::Vector2 localPos = Math::PointToLocalSpace( obstacle->getPosition(),
															  m_enemy->getLookDirection(),
															  m_enemy->getSideDirection(),
															  m_enemy->getPosition() );

			if( localPos.x >= 0 )
			{
				float expandedRadius = obstacle->getRadius() + m_enemy->getRadius();

				if( fabs( localPos.y ) < expandedRadius )
				{
					float cX = localPos.x;
					float cY = localPos.y;

					float sqrtPart = sqrt( expandedRadius * expandedRadius - cY * cY );
					float ip	   = cX - sqrtPart;

					if( ip <= 0 )
					{
						ip = cX + sqrtPart;
					}

					if( ip < distanceToClosestIP )
					{
						distanceToClosestIP			= ip;
						closestIntersectionObstacle = obstacle;
						localPosOfClosestObstacle	= localPos;
					}
				}
			}
		}
	}

	Math::Vector2 steeringForce{};

	if( closestIntersectionObstacle )
	{
		float multiplier = 1.0f + ( m_boxLenght - localPosOfClosestObstacle.x ) / m_boxLenght;

		steeringForce.y = ( closestIntersectionObstacle->getRadius() - localPosOfClosestObstacle.y ) * multiplier;
		const float brakingWeight = 0.2f;

		steeringForce.x = ( closestIntersectionObstacle->getRadius() - localPosOfClosestObstacle.x ) * brakingWeight;
	}

	return Math::VectorToWorldSpace( steeringForce, m_enemy->getLookDirection(), m_enemy->getSideDirection() );
}

Math::Vector2 Behaviors::wallAvoidance( const std::vector< Graphics::Wall* >& walls )
{
	createFeelers();

	float distToThisIP	  = 0.0f;
	float distToClosestIP = std::numeric_limits< float >::max();

	int closestWall = -1;

	Math::Vector2 steeringForce{};
	Math::Vector2 point{};
	Math::Vector2 closestPoint{};

	for( size_t flr = 0; flr < m_feelers.size(); ++flr )
	{
		for( size_t w = 0; w < walls.size(); ++w )
		{
			if( Math::LineIntersection2D( m_enemy->getPosition(),
										  m_feelers[ flr ],
										  walls[ w ].from(),
										  walls[ w ].to(),
										  distToThisIP,
										  point ) )
			{
				if( distToThisIP < distToClosestIP )
				{
					distToClosestIP = distToThisIP;
					closestWall		= w;
					closestPoint	= point;
				}
			}
		}

		if( closestWall >= 0 )
		{
			Math::Vector2 overShoot = m_feelers[ flr ] - closestPoint;

			steeringForce = walls[ closestWall ].normal() * overShoot.length();
		}
	}

	return steeringForce;
}

void Behaviors::createFeelers()
{
	m_feelers[ 0 ] = m_enemy->getPosition() + m_wallDetectionFeelersLen * m_enemy->getLookDirection();

	Math::Vector2 temp = m_enemy->getLookDirection();
	Math::Vec2DRotateAroundOrigin( temp, static_cast< float >( M_PI_2 ) * 3.5f );
	m_feelers[ 1 ] = m_enemy->getPosition() + m_wallDetectionFeelersLen / 2.0f * temp;

	temp = m_enemy->getLookDirection();
	Math::Vec2DRotateAroundOrigin( temp, static_cast< float >( M_PI_2 ) * 0.5f );
	m_feelers[ 2 ] = m_enemy->getPosition() + m_wallDetectionFeelersLen / 2.0f * temp;
}
} // namespace SteeringBehaviors::AI
