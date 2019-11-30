#include "stdafx.h"

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
	: GameEntity{ gameWorld, maxSpeed, Math::Vector2{} }, m_orientation{ orientation }, m_side{ side }
{
	init();
}

Wall::~Wall() = default;

void Wall::init()
{

	initGfxPart();
}

void Wall::teardown() {}

void Wall::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Wall::update( float delta ) {}

void Wall::processInput() {}
void Wall::processEvents( sf::Event& event ) {}

void Wall::initPhysicalPart() {}

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
		wallGfx->setPoint( 2, sf::Vector2f{ m_width, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ m_width, 0.0f } );

		m_from	 = Math::toMathVector( wallGfx->getPoint( 3 ) );
		m_to	 = Math::toMathVector( wallGfx->getPoint( 2 ) );
		m_normal = Math::Vector2{ 1.0f, 0.0f };
		break;

	case Side::RIGHT:
		wallGfx->setPoint( 0, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - m_width, 0.0f } );
		wallGfx->setPoint( 1,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - m_width,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );

		m_from	 = Math::toMathVector( wallGfx->getPoint( 0 ) );
		m_to	 = Math::toMathVector( wallGfx->getPoint( 1 ) );
		m_normal = Math::Vector2{ -1.0f, 0.0f };
		break;

	case Side::UP:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, 0.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, m_width } );
		wallGfx->setPoint( 2, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, m_width } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );

		m_from	 = Math::toMathVector( wallGfx->getPoint( 1 ) );
		m_to	 = Math::toMathVector( wallGfx->getPoint( 2 ) );
		m_normal = Math::Vector2{ 0.0f, 1.0f };
		break;

	case Side::DOWN:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y - m_width } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y - m_width } );

		m_from	 = Math::toMathVector( wallGfx->getPoint( 0 ) );
		m_to	 = Math::toMathVector( wallGfx->getPoint( 3 ) );
		m_normal = Math::Vector2{ 0.0f, -1.0f };
		break;

	default:
		break;
	}
	m_graphicalBody->setFillColor( sf::Color::Blue );
}

void Wall::wrapScreenPosition() {}

SteeringBehaviors::Math::Vector2 Wall::getFrom() const
{
	return m_from;
}

SteeringBehaviors::Math::Vector2 Wall::getTo() const
{
	return m_to;
}

SteeringBehaviors::Math::Vector2 Wall::getNormal() const
{
	return m_normal;
}

} // namespace Graphics
} // namespace SteeringBehaviors
