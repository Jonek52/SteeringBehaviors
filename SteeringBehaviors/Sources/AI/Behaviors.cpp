#include "stdafx.h"

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

Behaviors::Behaviors( Graphics::Enemy& enemy ) : m_enemy{ enemy }, m_wanderTarget{ enemy.getPosition() }
{
	m_feelers.emplace_back();
	m_feelers.emplace_back();
	m_feelers.emplace_back();
}
Vector2 Behaviors::calculate()
{
	m_steeringForce.zero();

	checkForBehaviorsChanges( m_enemy.getWorld()->getEnemies() );

	if( isOn( Behavior::WALL_AVOIDANCE ) )
	{
		Vector2 force = wallAvoidance( m_enemy.getWorld()->getWalls() ) * m_wallAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::OBSTACLE_AVOIDANCE ) )
	{
		Vector2 force = obstacleAvoidance( m_enemy.getWorld()->getObstacles() ) * m_obstacleAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::EVADE ) )
	{
		Vector2 force = evade( m_enemy.getWorld()->getPlayer() ) * m_evadeWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}
	if( isOn( Behavior::FLEE ) )
	{
		Vector2 force = flee( m_enemy.getWorld()->getPlayer()->getPosition() ) * m_fleeWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::SEPARATION ) || isOn( Behavior::ALIGNMENT ) || isOn( Behavior::COHESION ) )
	{

		if( isOn( Behavior::SEPARATION ) )
		{
			Vector2 force = separation( m_enemy.getWorld()->getEnemies() ) * m_separationWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}

		if( isOn( Behavior::ALIGNMENT ) )
		{
			Vector2 force = alignment( m_enemy.getWorld()->getEnemies() ) * m_alignmentWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}

		if( isOn( Behavior::COHESION ) )
		{
			Vector2 force = cohension( m_enemy.getWorld()->getEnemies() ) * m_cohensionWeight;
			if( !accumulateForce( m_steeringForce, force ) )
				return m_steeringForce;
		}
	}

	if( isOn( Behavior::SEEK ) )
	{
		Vector2 force = seek( m_enemy.getWorld()->getPlayer()->getPosition() ) * m_seekWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::PURSUIT ) )
	{
		Vector2 force = pursuit( m_enemy.getWorld()->getPlayer() ) * m_pursuitWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::HIDE ) )
	{
		Vector2 force = hide( m_enemy.getWorld()->getPlayer(), m_enemy.getWorld()->getObstacles() ) * m_hideWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::ARRIVE ) )
	{
		Vector2 force = arrive( m_enemy.getWorld()->getPlayer()->getPosition(), Deceleration::FAST );
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	if( isOn( Behavior::WANDER ) )
	{
		Vector2 force = wander() * m_wallAvoidanceWeight;
		if( !accumulateForce( m_steeringForce, force ) )
			return m_steeringForce;
	}

	return m_steeringForce;
}

Vector2 Behaviors::seek( const Vector2& targetPosition )
{
	Vector2 desiredVelocity = Math::normalize( targetPosition - m_enemy.getPosition() ) * m_enemy.getMaxSpeed();
	return desiredVelocity - m_enemy.getVelocity();
}

Vector2 Behaviors::flee( const Vector2& targetPosition )
{
	const float fleeDistanceSquared = 200.0f * 200.0f;
	if( float distance = Math::distanceSquared( m_enemy.getPosition(), targetPosition );
		distance > fleeDistanceSquared )
	{
		return Vector2{};
	}

	Vector2 desiredVelocity = Math::normalize( m_enemy.getPosition() - targetPosition ) * m_enemy.getMaxSpeed();
	return desiredVelocity - m_enemy.getVelocity();
}

Vector2 Behaviors::arrive( const Vector2& targetPos, Deceleration deceleration )
{

	Vector2 toTarget = targetPos - m_enemy.getPosition();
	float distance	 = toTarget.length();

	if( distance > 0.0f )
	{
		const float decelarationTweaker = 0.3f;

		float speed = distance / ( static_cast< float >( deceleration ) * decelarationTweaker );
		speed		= std::min( speed, m_enemy.getMaxSpeed() );

		Vector2 desiredVelocity = toTarget * speed / distance;
		return desiredVelocity - m_enemy.getVelocity();
	}

	return Vector2{};
}

Vector2 Behaviors::pursuit( const shared_ptr< const Graphics::Player >& evader )
{
	Vector2 toEvader = evader->getPosition() - m_enemy.getPosition();

	float relativeHeading = m_enemy.getLookDirection().dot( evader->getLookDirection() );

	if( ( toEvader.dot( m_enemy.getLookDirection() ) > 0 ) && ( relativeHeading < -0.95f ) )
	{
		return seek( evader->getPosition() );
	}

	float lookAheadTime = toEvader.length() / ( m_enemy.getMaxSpeed() + evader->getSpeed() );

	return seek( evader->getPosition() + evader->getVelocity() * lookAheadTime );
}

Vector2 Behaviors::evade( const shared_ptr< const Graphics::Player >& pursuer )
{
	Vector2 toPursuer = pursuer->getPosition() - m_enemy.getPosition();

	const float ThreatRange = 200.0f;
	if( toPursuer.lengthSquared() > ThreatRange * ThreatRange )
		return Vector2();

	float lookAheadTime = toPursuer.length() / ( m_enemy.getMaxSpeed() + pursuer->getSpeed() );
	return flee( pursuer->getPosition() + pursuer->getVelocity() * lookAheadTime );
}

Vector2 Behaviors::wander()
{
	float jitter = m_wanderJitter * m_enemy.getDeltaTime();
	m_wanderTarget += Vector2{ Math::randomClamped() * jitter, Math::randomClamped() * jitter };
	m_wanderTarget.normalize();
	m_wanderTarget *= m_wanderRadius;

	Vector2 targetLocal = m_wanderTarget + Vector2{ m_wanderDistance, 0.f };

	Vector2 targetWorld = Math::PointToWorldSpace(
		targetLocal, m_enemy.getLookDirection(), m_enemy.getSideDirection(), m_enemy.getPosition() );

	return targetWorld - m_enemy.getPosition();
}

Vector2 Behaviors::obstacleAvoidance( const vector< shared_ptr< Graphics::Obstacle > >& obstacles )
{
	const float boxLength
		= m_minDetectionBoxLenght + ( m_enemy.getSpeed() / m_enemy.getMaxSpeed() ) * m_minDetectionBoxLenght;

	m_enemy.getWorld()->tagObstaclesWithinRange( m_enemy, boxLength );

	shared_ptr< Graphics::Obstacle > closestIntersectionObstacle{ nullptr };
	float distanceToClosestIP{ std::numeric_limits< float >::max() };
	Vector2 localPosOfClosestObstacle{};

	for( const auto& obstacle : obstacles )
	{
		if( obstacle->isTagged() )
		{
			Vector2 localPos = Math::PointToLocalSpace( obstacle->getPosition(),
														m_enemy.getLookDirection(),
														m_enemy.getSideDirection(),
														m_enemy.getPosition() );

			if( localPos.x >= 0 )
			{
				float expandedRadius = obstacle->getRadius() + m_enemy.getRadius();

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

	Vector2 steeringForce{};

	if( closestIntersectionObstacle )
	{
		float multiplier = 1.0f + ( boxLength - localPosOfClosestObstacle.x ) / boxLength;

		steeringForce.y = ( closestIntersectionObstacle->getRadius() - localPosOfClosestObstacle.y ) * multiplier;
		const float brakingWeight = 0.2f;

		steeringForce.x = ( closestIntersectionObstacle->getRadius() - localPosOfClosestObstacle.x ) * brakingWeight;
	}

	return Math::VectorToWorldSpace( steeringForce, m_enemy.getLookDirection(), m_enemy.getSideDirection() );
}

Vector2 Behaviors::wallAvoidance( const vector< shared_ptr< Graphics::Wall > >& walls )
{
	createFeelers();

	float distToThisIP	  = 0.0f;
	float distToClosestIP = std::numeric_limits< float >::max();

	int closestWall = -1;

	Vector2 steeringForce{};
	Vector2 point{};
	Vector2 closestPoint{};

	for( size_t flr = 0; flr < m_feelers.size(); ++flr )
	{
		for( size_t w = 0; w < walls.size(); ++w )
		{
			if( Math::LineIntersection2D( m_enemy.getPosition(),
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
			Vector2 overShoot = m_feelers[ flr ] - closestPoint;

			steeringForce = walls[ closestWall ]->getNormal() * overShoot.length();
		}
	}

	return steeringForce;
}

void Behaviors::createFeelers()
{
	m_feelers[ 0 ] = m_enemy.getPosition() + m_wallDetectionFeelersLen * m_enemy.getLookDirection();

	Vector2 temp = m_enemy.getLookDirection();
	Math::Vec2DRotateAroundOrigin( temp, static_cast< float >( M_PI_2 ) * 3.5f );
	m_feelers[ 1 ] = m_enemy.getPosition() + m_wallDetectionFeelersLen / 2.0f * temp;

	temp = m_enemy.getLookDirection();
	Math::Vec2DRotateAroundOrigin( temp, static_cast< float >( M_PI_2 ) * 0.5f );
	m_feelers[ 2 ] = m_enemy.getPosition() + m_wallDetectionFeelersLen / 2.0f * temp;
}

Vector2 Behaviors::getHidingPosition( const Vector2& obstaclePosition, float obstacleRadius, const Vector2& targetPos )
{
	const float distanceFromBoundary = 50.f;
	float cumulatedDistance			 = obstacleRadius + distanceFromBoundary;

	Vector2 playerToObstacle = Math::normalize( obstaclePosition - targetPos );

	return obstaclePosition + playerToObstacle * cumulatedDistance;
}

Vector2 Behaviors::hide( const shared_ptr< const Graphics::Player >& targetPos,
						 const vector< shared_ptr< Graphics::Obstacle > >& obstacles )
{
	float distToClosestObstacle = std::numeric_limits< float >::max();
	Vector2 bestHidingSpot{};

	for( auto& obstacle : obstacles )
	{
		Vector2 hidingSpot
			= getHidingPosition( obstacle->getPosition(), obstacle->getRadius(), targetPos->getPosition() );
		float distance = Math::distanceSquared( hidingSpot, m_enemy.getPosition() );

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

Vector2 Behaviors::separation( const vector< shared_ptr< Graphics::Enemy > >& neighbors )
{
	Vector2 steeringForce{};
	for( const auto& neighbor : neighbors )
	{
		if( neighbor.get() != &m_enemy && neighbor->isTagged() )
		{
			Vector2 toAgent = m_enemy.getPosition() - neighbor->getPosition();
			steeringForce += Math::normalize( toAgent ) / toAgent.length();
		}
	}

	return steeringForce;
}

Vector2 Behaviors::alignment( const vector< shared_ptr< Graphics::Enemy > >& neightbors )
{
	Vector2 averageHeading;
	int neightborCount{ 0 };

	for( const auto& neightbor : neightbors )
	{
		if( ( neightbor.get() != &m_enemy ) && ( neightbor->isTagged() ) )
		{
			averageHeading += neightbor->getLookDirection();
			neightborCount++;
		}
	}

	if( neightborCount > 0 )
	{
		averageHeading /= static_cast< float >( neightborCount );
		averageHeading -= m_enemy.getLookDirection();
	}

	return averageHeading;
}

Vector2 Behaviors::cohension( const vector< shared_ptr< Graphics::Enemy > >& neighbors )
{
	Vector2 centerOfMass{};
	Vector2 steeringForce{};

	int neightborCount{ 0 };

	for( const auto& neighbor : neighbors )
	{
		if( ( neighbor.get() != &m_enemy ) && ( neighbor->isTagged() ) )
		{
			centerOfMass += neighbor->getPosition();
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

bool Behaviors::accumulateForce( Vector2& runningTot, Vector2 forceToAdd )
{
	float maginitudeSoFar	 = runningTot.length();
	float magnituteRemaining = m_enemy.getMaxForce() - maginitudeSoFar;

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

void Behaviors::checkForBehaviorsChanges( const vector< shared_ptr< Graphics::Enemy > >& enemies )
{
	m_enemy.getWorld()->tagFriendsWithinRange( m_enemy, friendDistance );

	int neighborCounter{ 0 };

	for( const auto& neighbor : enemies )
	{
		if( neighbor->isTagged() )
		{
			neighborCounter++;
		}
	}

	if( neighborCounter >= groupLimit || enemies.size() <= attackLimit )
	{
		m_enemy.disableCounter();
		turnBehaviorOff( Behavior::FLEE );
		turnBehaviorOff( Behavior::EVADE );
		turnBehaviorOn( Behavior::COHESION );
		turnBehaviorOn( Behavior::ALIGNMENT );
		turnBehaviorOn( Behavior::SEPARATION );
		turnBehaviorOff( Behavior::HIDE );
		turnBehaviorOn( Behavior::SEEK );
		turnBehaviorOn( Behavior::PURSUIT );
	}
}

} // namespace SteeringBehaviors::AI
