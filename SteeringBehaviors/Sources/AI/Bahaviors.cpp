#include "Bahaviors.h"
#include "..\Graphics\Enemy.h"
#include "..\Graphics\Player.h"
#include "..\Graphics\GameWorld.h"
#include "..\Math\MathFunctions.h"
#include "..\Math\Matrix.h"
#include "..\Math\Transformations.h"

#include <algorithm>
namespace SteeringBehaviors::AI

{

Behaviors::Behaviors( Graphics::Enemy* enemy ) : m_enemy{ enemy } {}

Math::Vector2 Behaviors::calculate()
{
	m_steeringForce.zero();
	m_steeringForce += wander();
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

	float lookAheadTime = toEvader.length() / ( m_enemy->getMaxSpeed() + evader->getMaxSpeed() );

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

} // namespace SteeringBehaviors::AI
