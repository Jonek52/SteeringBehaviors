#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Dynamics\b2World.h"

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
Wall::Wall( GameWorld* gameWorld, float maxSpeed, const Vec& position, Orientation orientation, Side side )
	: GameEntity{ gameWorld, maxSpeed }, m_orientation{ orientation }, m_side{ side }
{
	b2BodyDef wallBodyDef;
	wallBodyDef.type	 = b2_staticBody;
	wallBodyDef.position = position;
	m_physicalBody		 = m_gameWorld->getPhysicalWorld()->CreateBody( &wallBodyDef );
}

Wall::~Wall() = default;

void Wall::init()
{
	calculateWallPosition();
	calculateWallSize();

	m_graphicalBody->setFillColor( sf::Color::Blue );
	// b2PolygonShape wallShape;

	// Vec wallShapeVertices[ 4 ];

	// for( size_t i = 0; i < 4; i++ )
	//{
	//	wallShapeVertices[ i ] = Math::toBox2DVector( m_graphicalBody->getPoint( i ) );
	//}

	// wallShape.Set( wallShapeVertices, 4 );
	// m_physicalBody->CreateFixture( &wallShape, 1.0f );
}

void Wall::teardown() {}
void Wall::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Wall::update() {}

void Wall::processInput() {}
void Wall::processEvents( sf::Event& event ) {}

void Wall::calculateWallSize()
{
	b2PolygonShape wallShape;

	switch( m_orientation )
	{
	case Orientation::HORIZONTAL:
		wallShape.SetAsBox( m_gameWorld->getWindow()->getSize().x / 2, 40.0f );
		break;
	case Orientation::VERTICAL:
		wallShape.SetAsBox( 20.0f, m_gameWorld->getWindow()->getSize().y / 2 );
		break;
	}

	m_physicalBody->CreateFixture( &wallShape, 0.0f );
}

void Wall::calculateWallPosition()
{
	m_graphicalBody			 = std::make_unique< sf::ConvexShape >();
	sf::ConvexShape* wallGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	wallGfx->setPointCount( 4 );
	switch( m_side )
	{
	case Side::LEFT:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, 0.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2, sf::Vector2f{ 40.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ 40.0f, 0.0f } );
		break;
	case Side::RIGHT:
		wallGfx->setPoint( 0, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - 40.0f, 0.0f } );
		wallGfx->setPoint( 1,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x - 40.0f,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );
		break;
	case Side::UP:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, 0.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, 40.0f } );
		wallGfx->setPoint( 2, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 40.0f } );
		wallGfx->setPoint( 3, sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x, 0.0f } );
		break;
	case Side::DOWN:
		wallGfx->setPoint( 0, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y - 40.0f } );
		wallGfx->setPoint( 1, sf::Vector2f{ 0.0f, ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 2,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y } );
		wallGfx->setPoint( 3,
						   sf::Vector2f{ ( float )m_gameWorld->getWindow()->getSize().x,
										 ( float )m_gameWorld->getWindow()->getSize().y - 40.0f } );
		break;
	default:
		break;
	}
}

void Wall::wrapScreenPosition() {}
} // namespace Graphics
} // namespace SteeringBehaviors
