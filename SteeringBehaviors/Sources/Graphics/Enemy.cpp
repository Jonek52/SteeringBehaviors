#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Enemy.h"
#include "GameWorld.h"
#include "../Math/MathFunctions.h"

namespace SteeringBehaviors
{
namespace Graphics
{

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed ) : GameEntity{ gameWorld, maxSpeed }
{
	init();
}

Enemy::~Enemy() = default;

void Enemy::init()
{
	initGfxPart();
	initPhysicalPart();
	m_graphicalBody->setPosition( sf::Vector2f{ 430.0f, 330.0f } );
}

void Enemy::teardown() {}

void Enemy::update( std::chrono::milliseconds delta ) {}

void Enemy::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Enemy::processInput() {}

void Enemy::processEvents( sf::Event& event ) {}

void Enemy::initGfxPart()
{
	m_graphicalBody = std::make_unique< sf::ConvexShape >();
	m_graphicalBody->setFillColor( sf::Color::Red );
	auto* enemyGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	enemyGfx->setPointCount( 3 );

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
	enemyGfx->setPoint( 2, points[ 2 ] - center );

	m_graphicalBody->setScale( { 0.5f, 0.5f } );
}

void Enemy::initPhysicalPart() {}

void Enemy::wrapScreenPosition() {}
} // namespace Graphics
} // namespace SteeringBehaviors
