#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Enemy.h"
#include "GameWorld.h"
#include "..\Math\MathFunctions.h"
#include "..\AI\Bahaviors.h"

namespace SteeringBehaviors
{
namespace Graphics
{

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed ) : GameEntity{ gameWorld, maxSpeed }
{
	m_steeringBehaviors = new AI::Behaviors( this );
	init();
}

Enemy::~Enemy() = default;

void Enemy::init()
{
	m_lookDirection = Math::Vector2{ 1.0f, 0.0f };
	m_sideDirection = m_lookDirection.perp();

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
	m_position = Math::Vector2{ 600.0f, 500.f };

	// auto* enemyGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	/*enemyGfx->setPointCount( 3 );

	std::vector< sf::Vector2f > points;
	points.emplace_back( 0.0f, -100.0f );
	points.emplace_back( -50.0f, 0.0f );
	points.emplace_back( 50.0f, 0.0f );

	sf::Vector2f center{ 0.0f, 0.0f };

	for( const auto& p : points )
	{
		center += p;
	}

	center /= 3.0f;

	enemyGfx->setPoint( 0, points[ 0 ] - center );
	enemyGfx->setPoint( 1, points[ 1 ] - center );
	enemyGfx->setPoint( 2, points[ 2 ] - center );*/
}

void Enemy::initPhysicalPart() {}

void Enemy::wrapScreenPosition() {}

} // namespace Graphics
} // namespace SteeringBehaviors
