#include "stdafx.h"

#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Graphics\Color.hpp"
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Enemy.h"
#include "GameWorld.h"
#include "..\Math\MathFunctions.h"
#include "..\AI\Behaviors.h"

#include <iostream>

namespace SteeringBehaviors
{
namespace Graphics
{

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position )
	: MovingEntity{ gameWorld, maxSpeed, 200.0f * 4.0f, 10.0f, position },
	  m_behaviorSwitchTime{ static_cast< int >( std::floor( Math::randFloat() * 10.0F + 5.0F ) ) * 1000 }
{

	m_lookDirection = Math::Vector2{ 0.0f, -1.0f };
	m_sideDirection = m_lookDirection.perp();

	init();

	m_steeringBehaviors = new AI::Behaviors( *this );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::SEEK );
	//m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::SEPARATION );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::ALIGNMENT );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::COHESION );

	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WANDER );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::PURSUIT );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::FLEE );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::EVADE );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::OBSTACLE_AVOIDANCE );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WALL_AVOIDANCE );

	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WANDER );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::HIDE );

	/*float randomNumber = std::floor( Math::randFloat() * 10 );
	if( static_cast< int >( randomNumber ) % 2 == 0 )
		m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::HIDE );
	else
		m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WANDER );*/

	m_behaviorCooldown = std::chrono::steady_clock::now();
}

Enemy::~Enemy() = default;

void Enemy::init()
{
	initGfxPart();
}

void Enemy::teardown() {}

void Enemy::update( float delta )
{
	m_deltaTime = delta;

	handleCollisionsWithOtherEnemies( m_gameWorld->getEnemies() );

	Math::Vector2 steeringForce = m_steeringBehaviors->calculate();
	Math::Vector2 acceleration	= steeringForce / m_mass;
	m_velocity += acceleration * delta;

	m_velocity.truncate( m_maxSpeed );
	m_position += m_velocity * delta;
	m_graphicalBody->setPosition( Math::toSFMLVector( m_position ) );

	if( m_velocity.lengthSquared() > 0.000000001 )
	{
		m_lookDirection = Math::normalize( m_velocity );
		m_sideDirection = m_lookDirection.perp();
	}

	/*if( m_shouldCount )
		behaviorCooldownCounter();*/

	WrapAround( m_position, m_gameWorld->getWindow()->getSize().x, m_gameWorld->getWindow()->getSize().y );
}

void Enemy::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Enemy::processInput() {}

void Enemy::processEvents( sf::Event& event ) {}

void Enemy::initGfxPart()
{
	m_graphicalBody = std::make_unique< sf::CircleShape >( m_radius, 10 );
	m_graphicalBody->setFillColor( sf::Color{ 0, 0, 0, 0 } );
	m_graphicalBody->setOutlineColor( sf::Color::Red );
	m_graphicalBody->setOutlineThickness( 1.0f );

	sf::Vector2f center{ 0.f, 0.f };
	size_t numOfPoints = m_graphicalBody->getPointCount();

	for( size_t i = 0; i < numOfPoints; ++i )
	{
		center += m_graphicalBody->getPoint( i );
	}

	center /= static_cast< float >( numOfPoints );
	m_graphicalBody->setOrigin( center );
	m_graphicalBody->setPosition( { 0.0f, 0.0f } );
}

void Enemy::initPhysicalPart() {}

void Enemy::wrapScreenPosition() {}

void Enemy::turnBehaviorOn( AI::Behaviors::Behavior behavior )
{
	m_steeringBehaviors->turnBehaviorOn( behavior );
}

void Enemy::turnBehaviorOff( AI::Behaviors::Behavior behavior )
{
	m_steeringBehaviors->turnBehaviorOff( behavior );
}

float Enemy::getDeltaTime() const
{
	return m_deltaTime;
}

void Enemy::switchBehavior()
{
	if( m_steeringBehaviors->isOn( AI::Behaviors::Behavior::WANDER ) )
	{
		m_steeringBehaviors->turnBehaviorOff( AI::Behaviors::Behavior::WANDER );
		m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::HIDE );
		std::cout << "Switched to hide \n";
	}

	else
	{
		m_steeringBehaviors->turnBehaviorOff( AI::Behaviors::Behavior::HIDE );
		m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WANDER );
		std::cout << "Switched to wander\n";
	}
}

void Enemy::behaviorCooldownCounter()
{
	auto now	 = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( now - m_behaviorCooldown );

	if( elapsed >= std::chrono::milliseconds( m_behaviorSwitchTime ) )
	{
		switchBehavior();
		m_behaviorSwitchTime = static_cast< int >( std::floor( Math::randFloat() * 10.0F + 5.0F ) ) * 1000;
		m_behaviorCooldown	 = std::chrono::steady_clock::now();
	}
} // namespace Graphics

void Enemy::handleCollisionsWithOtherEnemies( const vector< shared_ptr< Enemy > >& enemies )
{
	m_gameWorld->tagFriendsWithinRange( *this, m_radius );

	for( const auto& enemy : enemies )
	{
		if( enemy->isTagged() )
		{
			Vector2 enemytoEnemy = getPosition() - enemy->getPosition();
			Vector2 moveVector	 = Math::normalize( enemytoEnemy ) * ( enemy->getRadius() + m_radius );
			m_position			 = enemy->getPosition() + moveVector;
		}
	}
}
void Enemy::disableCounter()
{
	m_shouldCount = false;
}

} // namespace Graphics
} // namespace SteeringBehaviors
