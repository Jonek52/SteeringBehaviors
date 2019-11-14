#include "Box2D\Common\b2Math.h"
#include "SFML\Graphics\CircleShape.hpp"

#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Common\b2Math.h"
#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"

#include "SFML\Window\Window.hpp"
#include "GameWorld.h"
#include "Player.h"
#include "Wall.h"

namespace SteeringBehaviors
{
namespace Graphics
{

GameWorld::GameWorld( sf::Window* window )
	: m_mainWindow{ window }, m_world{ std::make_unique< b2World >( b2Vec2{ 0.0f, 0.0f } ) }
{
}

void GameWorld::init()
{
	auto player = std::make_unique< Player >( this, 50.0f );
	m_gameEntities.emplace_back( std::move( player ) );

	auto leftWall = std::make_unique< Wall >( this, 0.0f, Wall::Orientation::VERTICAL, Wall::Side::LEFT );
	m_gameEntities.emplace_back( std::move( leftWall ) );
	// initGameEntities();
}

void GameWorld::teardown() {}

void GameWorld::render( sf::RenderWindow* window )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->render( window );
	}
}

void GameWorld::update()
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->update();
	}
}

void GameWorld::step( float deltaTime )
{
	m_world->Step( deltaTime, 8, 10 );
}

void GameWorld::initGameEntities()
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->init();
	}
}

void GameWorld::processInput()
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->processInput();
	}
}

void GameWorld::processEvents( sf::Event& event )
{

	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->processEvents( event );
	}
}
sf::Window* GameWorld::getWindow() const
{
	return m_mainWindow;
}

std::shared_ptr< b2World >& GameWorld::getPhysicalWorld()
{
	return m_world;
}
} // namespace Graphics
} // namespace SteeringBehaviors
