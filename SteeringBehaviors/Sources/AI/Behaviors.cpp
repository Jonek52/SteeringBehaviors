#include "Behaviors.h"

#include "..\Graphics\Enemy.h"
#include "..\Graphics\Player.h"
#include "..\Graphics\GameWorld.h"
#include "..\Graphics\GameEntity.h"
#include "..\Graphics\Wall.h"
#include "..\Graphics\Obstacle.h"

#include "..\Math\MathFunctions.h"
#include "..\Math\Matrix.h"
#include "..\Math\Transformations.h"
#include "..\Math\Geometry.h"

#include <math.h>
#include <algorithm>
namespace SteeringBehaviors::AI
{

Behaviors::Behaviors( Graphics::Enemy* enemy ) : m_enemy{ enemy }
{
	m_wanderTarget = Math::Vector2{ enemy->getPosition() };
	m_feelers.emplace_back();
	m_feelers.emplace_back();
	m_feelers.emplace_back();
}
Math::Vector2 Behaviors::calculate()
{
	m_steeringForce.zero();

	if( isOn( Behavior::WALL_AVOIDANCE ) )
	{
		Math::Vector2 force = wallAvoidance( m_enemy->getWorld()->getWalls() ) * m_wallAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::OBSTACLE_AVOIDANCE ) )
	{
		Math::Vector2 force = obstacleAvoidance( m_enemy->getWorld()->getObstacles() ) * m_obstacleAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::EVADE ) )
	{
		Math::Vector2 force = evade( m_enemy->getWorld()->getPlayer() ) * m_evadeWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}
	if( isOn( Behavior::FLEE ) )
	{
		Math::Vector2 force = flee( m_enemy->getWorld()->getPlayer()->getPosition() ) * m_fleeWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::SEPARATION ) || isOn( Behavior::ALIGNMENT ) || isOn( Behavior::COHESION ) )
	{
		m_enemy->getWorld()->tagFriendsWithinRange( m_enemy, m_viewDistance );

		if( isOn( Behavior::SEPARATION ) )
		{
			Math::Vector2 force = separation( m_enemy->getWorld()->getEnemies() ) * m_separationWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}

		if( isOn( Behavior::ALIGNMENT ) )
		{
			Math::Vector2 force = alignment( m_enemy->getWorld()->getEnemies() ) * m_alignmentWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}

		if( isOn( Behavior::COHESION ) )
		{
			Math::Vector2 force = cohension( m_enemy->getWorld()->getEnemies() ) * m_cohensionWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}
	}

	if( isOn( Behavior::SEEK ) )
	{
		Math::Vector2 force = seek( m_enemy->getWorld()->getPlayer()->getPosition() ) * m_seekWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::HIDE ) )
	{
		Math::Vector2 force = hide( m_hideTarget, m_enemy->getWorld()->getObstacles() );
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::ARRIVE ) )
	{
		Math::Vector2 force = arrive( m_enemy->getWorld()->getPlayer()->getPosition(), Deceleration::FAST );
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::WANDER ) )
	{
		Math::Vector2 force = wander() * m_wallAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	return m_steeringForce;
}

Math::Vector2 Behaviors::seek( const Math::Vector2& targetPosition )
{
	Math::Vector2 desiredVelocity = Math::normalize( targetPosition - m_enemy->getPosition() ) * m_enemy->getMaxSpeed();
	return desiredVelocity - m_enemy->getVelocity();
}

Math::Vector2 Behaviors::flee( const Math::Vector2& targetPosition )
{
	const float fleeDistanceSquared = 100.0f * 100.0f;
	if( float distance = Math::Vec2DDistanceSq( m_enemy->getPosition(), targetPosition );
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

	const float ThreatRange = 100.0f;
	if( toPursuer.lengthSquared() > ThreatRange * ThreatRange )
		return Math::Vector2();

	float lookAheadTime = toPursuer.length() / ( m_enemy->getMaxSpeed() + pursuer->getSpeed() );
	return flee( pursuer->getPosition() + pursuer->getVelocity() * lookAheadTime );
}

Math::Vector2 Behaviors::wander()
{
	float jitter = m_wanderJitter * m_enemy->getDeltaTime();
	m_wanderTarget += Math::Vector2{ Math::randomClamped() * jitter, Math::randomClamped() * jitter };
	m_wanderTarget.normalize();
	m_wanderTarget *= m_wanderRadius;

	Math::Vector2 targetLocal = m_wanderTarget + Math::Vector2{ m_wanderDistance, 0.f };

	Math::Vector2 targetWorld = Math::PointToWorldSpace(
		targetLocal, m_enemy->getLookDirection(), m_enemy->getSideDirection(), m_enemy->getPosition() );

	return targetWorld - m_enemy->getPosition();
}

SteeringBehaviors::Math::Vector2 Behaviors::obstacleAvoidance( const std::vector< Graphics::Obstacle* >& obstacles )
{
	m_boxLenght = m_minDetectionBoxLenght + ( m_enemy->getSpeed() / m_enemy->getMaxSpeed() ) * m_minDetectionBoxLenght;

	m_enemy->getWorld()->tagObstaclesWithinRange( m_enemy, m_boxLenght );

	Graphics::Obstacle* closestIntersectionObstacle = nullptr;
	float distanceToClosestIP						= std::numeric_limits< float >::max();
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
										  walls[ w ]->getFrom(),
										  walls[ w ]->getTo(),
										  distToThisIP,
										  point ) )
			{
				if( distToThisIP < distToClosestIP )
				{
					distToClosestIP = distToThisIP;
					closestWall		= static_cast< int >( w );
					closestPoint	= point;
				}
			}
		}

		if( closestWall >= 0 )
		{
			Math::Vector2 overShoot = m_feelers[ flr ] - closestPoint;

			steeringForce = walls[ closestWall ]->getNormal() * overShoot.length();
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

SteeringBehaviors::Math::Vector2 Behaviors::getHidingPosition( const Math::Vector2& obstaclePosition,
															   float obstacleRadius,
															   const Math::Vector2& targetPos )
{
	const float distanceFromBoundary = 30.f;
	float cumulatedDistance			 = obstacleRadius + distanceFromBoundary;

	Math::Vector2 playerToObstacle = Math::Vec2DNormalize( obstaclePosition - targetPos );

	return obstaclePosition + playerToObstacle * cumulatedDistance;
}

SteeringBehaviors::Math::Vector2 Behaviors::hide( const Graphics::Player* targetPos,
												  std::vector< Graphics::Obstacle* >& obstacles )
{
	float distToClosestObstacle = std::numeric_limits< float >::max();
	Math::Vector2 bestHidingSpot{};

	for( auto& obstacle : obstacles )
	{
		Math::Vector2 hidingSpot
			= getHidingPosition( obstacle->getPosition(), obstacle->getRadius(), targetPos->getPosition() );
		float distance = Math::Vec2DDistanceSq( hidingSpot, m_enemy->getPosition() );

		if( distance < distToClosestObstacle )
		{
			distToClosestObstacle = distance;
			bestHidingSpot		  = hidingSpot;
		}
	}

	if( distToClosestObstacle == std::numeric_limits< float >::max() )
	{
		return evade( targetPos );
	}

	return arrive( bestHidingSpot, Deceleration::FAST );
}

SteeringBehaviors::Math::Vector2 Behaviors::separation( const std::vector< Graphics::Enemy* >& neighbors )
{
	Math::Vector2 steeringForce{};

	for( auto neighbor : neighbors )
	{
		if( neighbor != m_enemy && neighbor->isTagged() )
		{
			Math::Vector2 toAgent = m_enemy->getPosition() - neighbor->getPosition();
			steeringForce += Math::normalize( toAgent ) / toAgent.length();
		}
	}

	return steeringForce;
}

void Behaviors::turnBehaviorOn( Behavior behavior )
{
	switch( behavior )
	{
	case Behavior::SEEK:
		m_seekOn = true;
		break;
	case Behavior::FLEE:
		m_fleeOn = true;
		break;
	case Behavior::ARRIVE:
		m_arriveOn = true;
		break;
	case Behavior::PURSUIT:
		m_pursuitOn = true;
		break;
	case Behavior::EVADE:
		m_evadeOn = true;
		break;
	case Behavior::WANDER:
		m_wanderOn = true;
		break;
	case Behavior::OBSTACLE_AVOIDANCE:
		m_obstacleAvoidanceOn = true;
		break;
	case Behavior::WALL_AVOIDANCE:
		m_wallAvoidanceOn = true;
		break;
	case Behavior::HIDE:
		m_hideOn = true;
		break;
	case Behavior::SEPARATION:
		m_separationOn = true;
		break;
	case Behavior::ALIGNMENT:
		m_alignmentOn = true;
		break;
	case Behavior::COHESION:
		m_cohesionOn = true;
		break;
	default:
		break;
	}
}

void Behaviors::turnBehaviorOff( Behavior behavior )
{
	switch( behavior )
	{
	case Behavior::SEEK:
		m_seekOn = false;
		break;
	case Behavior::FLEE:
		m_fleeOn = false;
		break;
	case Behavior::ARRIVE:
		m_arriveOn = false;
		break;
	case Behavior::PURSUIT:
		m_pursuitOn = false;
		break;
	case Behavior::EVADE:
		m_evadeOn = false;
		break;
	case Behavior::WANDER:
		m_wanderOn = false;
		break;
	case Behavior::OBSTACLE_AVOIDANCE:
		m_obstacleAvoidanceOn = false;
		break;
	case Behavior::WALL_AVOIDANCE:
		m_wallAvoidanceOn = false;
		break;
	case Behavior::HIDE:
		m_hideOn = false;
		break;
	case Behavior::SEPARATION:
		m_separationOn = false;
		break;
	case Behavior::ALIGNMENT:
		m_alignmentOn = false;
		break;
	case Behavior::COHESION:
		m_cohesionOn = false;
		break;
	default:
		break;
	}
}

bool Behaviors::isOn( Behavior behavior )
{
	switch( behavior )
	{
	case Behaviors::Behavior::SEEK:
		return m_seekOn;
		break;
	case Behaviors::Behavior::FLEE:
		return m_fleeOn;
		break;
	case Behaviors::Behavior::ARRIVE:
		return m_arriveOn;
		break;
	case Behaviors::Behavior::PURSUIT:
		return m_pursuitOn;
		break;
	case Behaviors::Behavior::EVADE:
		return m_evadeOn;
		break;
	case Behaviors::Behavior::WANDER:
		return m_wanderOn;
		break;
	case Behaviors::Behavior::OBSTACLE_AVOIDANCE:
		return m_obstacleAvoidanceOn;
		break;
	case Behaviors::Behavior::WALL_AVOIDANCE:
		return m_wallAvoidanceOn;
		break;
	case Behaviors::Behavior::HIDE:
		return m_hideOn;
		break;
	case Behaviors::Behavior::SEPARATION:
		return m_separationOn;
		break;
	case Behaviors::Behavior::ALIGNMENT:
		return m_alignmentOn;
		break;
	case Behaviors::Behavior::COHESION:
		return m_cohesionOn;
		break;
	default:
		return false;
		break;
	}
}

SteeringBehaviors::Math::Vector2 Behaviors::alignment( const std::vector< Graphics::Enemy* >& neightbors )
{
	Math::Vector2 averageHeading;
	int neightborCount{ 0 };

	for( int a = 0; a < neightbors.size(); ++a )
	{
		if( ( neightbors[ a ] != m_enemy ) && ( neightbors[ a ]->isTagged() ) )
		{
			averageHeading += neightbors[ a ]->getLookDirection();
			neightborCount++;
		}
	}

	if( neightborCount > 0 )
	{
		averageHeading /= static_cast< float >( neightborCount );
		averageHeading -= m_enemy->getLookDirection();
	}

	return averageHeading;
}

SteeringBehaviors::Math::Vector2 Behaviors::cohension( const std::vector< Graphics::Enemy* >& neighbors )
{
	Math::Vector2 centerOfMass{};
	Math::Vector2 steeringForce{};

	int neightborCount{ 0 };

	for( int a = 0; a < neighbors.size(); ++a )
	{
		if( ( neighbors[ a ] != m_enemy ) && ( neighbors[ a ]->isTagged() ) )
		{
			centerOfMass += neighbors[ a ]->getPosition();
			++neightborCount;
		}
	}

	if( neightborCount > 0 )
	{
		centerOfMass /= static_cast< float >( neightborCount );
		steeringForce = seek( centerOfMass );
	}

	return steeringForce;
}

bool Behaviors::accumulateForce( Math::Vector2& runningTot, Math::Vector2 forceToAdd )
{
	float maginitudeSoFar	 = runningTot.length();
	float magnituteRemaining = m_enemy->getMaxForce() - maginitudeSoFar;

	if( magnituteRemaining <= 0.0f )
	{
		return false;
	}

	float magnituteToAdd = forceToAdd.length();
	if( magnituteToAdd < magnituteRemaining )
	{
		runningTot += forceToAdd;
	}
	else
	{
		runningTot += Math::normalize( forceToAdd ) * magnituteRemaining;
	}

	return true;
}

} // namespace SteeringBehaviors::AI
