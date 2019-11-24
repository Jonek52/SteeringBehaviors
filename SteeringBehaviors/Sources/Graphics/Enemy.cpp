#include "SFML\Graphics\CircleShape.hpp"
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

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed ) : GameEntity{ gameWorld, maxSpeed }
{

	m_position		= Math::Vector2{ 600.0f, 500.f };
	m_radius		= 20.0f;
	m_lookDirection = Math::Vector2{ 1.0f, 0.0f };
	m_sideDirection = m_lookDirection.perp();

	init();
	m_steeringBehaviors = new AI::Behaviors( this );
}

Enemy::~Enemy() = default;

void Enemy::init()
{
	initGfxPart();
}

void Enemy::teardown() {}

void Enemy::update( float delta )
{
	Math::Vector2 steeringForce = m_steeringBehaviors->calculate();
	Math::Vector2 acceleration	= steeringForce / m_mass;
	m_velocity += acceleration * delta;

	m_velocity.truncate( m_maxSpeed );
	m_position += m_velocity * delta;
	m_graphicalBody->setPosition( Math::toSFMLVector( m_position ) );

	if( m_velocity.lengthSquared() > 0.00001 )
	{
		m_lookDirection = Math::normalize( m_velocity );
		m_sideDirection = m_lookDirection.perp();
	}
}

void Enemy::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Enemy::processInput() {}

void Enemy::processEvents( sf::Event& event ) {}

void Enemy::initGfxPart()
{
	m_graphicalBody = std::make_unique< sf::CircleShape >( 20.0f, 10 );
	m_graphicalBody->setFillColor( sf::Color::Red );

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

} // namespace Graphics
} // namespace SteeringBehaviors
