#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Dynamics\b2World.h"

#include "SFML\Window.hpp"
#include "SFML\Graphics\ConvexShape.hpp"
#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\RenderWindow.hpp"
#include "Box2D\Dynamics\b2Fixture.h"

#include "Wall.h"
#include "GameWorld.h"
#include "../Math/MathFunctions.h"

#include <iostream>
namespace SteeringBehaviors
{
namespace Graphics
{
Wall::Wall( GameWorld* gameWorld, float maxSpeed, Orientation orientation, Side side )
	: GameEntity{ gameWorld, maxSpeed, GameWorld::OBSTACLE, GameWorld::PLAYER | GameWorld::ENEMY },
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

void Wall::update(std::chrono::milliseconds delta)
{}

void Wall::processInput() {}
void Wall::processEvents( sf::Event& event ) {}

void Wall::initPhysicalPart()
{
	b2BodyDef wallBodyDef;
	wallBodyDef.type = b2_staticBody;

	switch( m_side )
	{
	case Side::LEFT:
		wallBodyDef.position.Set( width / 2.f, m_gameWorld->getWindow()->getSize().y / 2 );
		break;
	case Side::RIGHT:
		wallBodyDef.position.Set( m_gameWorld->getWindow()->getSize().x - width / 2.f,
								  m_gameWorld->getWindow()->getSize().y / 2 );
		break;
	case Side::UP:
		wallBodyDef.position.Set( m_gameWorld->getWindow()->getSize().x / 2.f, width / 2.f );
		break;
	case Side::DOWN:
		wallBodyDef.position.Set( m_gameWorld->getWindow()->getSize().x / 2.f,
								  m_gameWorld->getWindow()->getSize().y - width / 2.f );
		break;
	default:
		break;
	}

	m_physicalBody = m_gameWorld->getPhysicalWorld()->CreateBody( &wallBodyDef );

	b2PolygonShape wallShape;

	switch( m_orientation )
	{
	case Orientation::HORIZONTAL:
		wallShape.SetAsBox( m_gameWorld->getWindow()->getSize().x / 2, width / 2.f );
		break;
	case Orientation::VERTICAL:
		wallShape.SetAsBox( width / 2.f, m_gameWorld->getWindow()->getSize().y / 2 );
		break;
	}

	b2FixtureDef fixture;
	fixture.filter.categoryBits = m_collisionCategory;
	fixture.filter.maskBits		= m_collisionMask;
	fixture.density				= 0.0f;
	fixture.shape				= &wallShape;

	m_physicalBody->CreateFixture( &fixture );
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
