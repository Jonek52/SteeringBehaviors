#include "stdafx.h"

#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Graphics\Color.hpp"
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Enemy.h"
#include "GameWorld.h"
#include "..\Math\MathFunctions.h"
#include "..\AI\Behaviors.h"

namespace SteeringBehaviors
{
namespace Graphics
{

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position )
	: MovingEntity{ gameWorld, maxSpeed, 200.0f * 4.0f, 3.0f, position }
{
	m_lookDirection = Math::Vector2{ 0.0f, -1.0f };
	m_sideDirection = m_lookDirection.perp();

	init();

	m_steeringBehaviors = new AI::Behaviors( *this );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::SEEK );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::SEPARATION );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::ALIGNMENT );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::COHESION );

	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WANDER );
	// m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::FLEE );
	//m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::EVADE );
	m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::OBSTACLE_AVOIDANCE );
	//m_steeringBehaviors->turnBehaviorOn( AI::Behaviors::Behavior::WALL_AVOIDANCE );
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

} // namespace Graphics
} // namespace SteeringBehaviors
