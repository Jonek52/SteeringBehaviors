#include "stdafx.h"

#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Graphics.hpp"
#include "SFML\System\Vector2.hpp"

#include "Obstacle.h"
#include "GameWorld.h"
#include "..\Math\MathFunctions.h"
namespace SteeringBehaviors::Graphics
{

Obstacle::Obstacle( GameWorld* gameWorld, Math::Vector2 position, float radius )
	: GameEntity{ gameWorld, position, radius }
{
	init();
}

Obstacle::~Obstacle() = default;

void Obstacle::init()
{
	initGfxPart();
}

void Obstacle::teardown() {}

void Obstacle::update( float delta ) {}

void Obstacle::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Obstacle::processInput() {}

void Obstacle::processEvents( sf::Event& event ) {}

void Obstacle::initGfxPart()
{
	auto shape = std::make_unique< sf::CircleShape >( m_radius, 20 );
	shape->setFillColor( sf::Color::Blue );

	sf::Vector2f center{ 0.f, 0.f };
	size_t numOfPoints = shape->getPointCount();

	for( size_t i = 0; i < numOfPoints; ++i )
	{
		center += shape->getPoint( i );
	}

	center /= static_cast< float >( numOfPoints );

	shape->setOrigin( center );
	shape->setPosition( 0.f, 0.f );

	shape->setPosition( Math::toSFMLVector( m_position ) );
	m_graphicalBody = std::move( shape );
}

void Obstacle::initPhysicalPart() {}

void Obstacle::wrapScreenPosition() {}

} // namespace SteeringBehaviors::Graphics
