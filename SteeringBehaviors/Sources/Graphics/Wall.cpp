#include "SFML\Window.hpp"
#include "SFML\Graphics\ConvexShape.hpp"
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\RenderWindow.hpp"

#include "Wall.h"
#include "GameWorld.h"
#include "../Math/MathFunctions.h"

#include <iostream>
namespace SteeringBehaviors
{
namespace Graphics
{
Wall::Wall( GameWorld* gameWorld, float maxSpeed, Orientation orientation, Side side )
	: GameEntity{ gameWorld, maxSpeed},
	  m_orientation{ orientation },
	  m_side{ side }
{
	init();
}

Wall::~Wall() = default;

void Wall::init()
{

	initPhysicalPart();
	initGfxPart();
}

void Wall::teardown() {}

void Wall::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Wall::update( std::chrono::milliseconds delta ) {}

void Wall::processInput() {}
void Wall::processEvents( sf::Event& event ) {}

void Wall::initPhysicalPart()
{

	switch( m_side )
	{
	case Side::LEFT:
		break;
	case Side::RIGHT:
		break;
	case Side::UP:
		break;
	case Side::DOWN:
		break;
	default:
		break;
	}

	switch( m_orientation )
	{
	case Orientation::HORIZONTAL:
		break;
	case Orientation::VERTICAL:
		break;
	}
}

void Wall::initGfxPart()
{

	m_graphicalBody			 = std::make_unique< sf::ConvexShape >();
	sf::ConvexShape* wallGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );
	wallGfx->setPointCount( 4 );

	switch( m_side )
	{
	case Side::LEFT:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, 0.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2, sf::Vector2f{ width, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ width, 0.0f } );
		break;
	case Side::RIGHT:
		wallGfx->setPoint( 0, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - width, 0.0f } );
		wallGfx->setPoint( 1,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - width,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );
		break;
	case Side::UP:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, 0.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, width } );
		wallGfx->setPoint( 2, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, width } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );
		break;
	case Side::DOWN:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y - width } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y - width } );
		break;
	default:
		break;
	}
	m_graphicalBody->setFillColor( sf::Color::Blue );
}

void Wall::wrapScreenPosition() {}
} // namespace Graphics
} // namespace SteeringBehaviors
