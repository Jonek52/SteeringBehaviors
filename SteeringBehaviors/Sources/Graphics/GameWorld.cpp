#include "Box2D\Common\b2Math.h"
#include "SFML\Graphics\CircleShape.hpp"
#include "GameWorld.h"
#include "Player.h"

namespace SteeringBehaviors
{
namespace Graphics
{

GameWorld::GameWorld( sf::Window* window ) : m_mainWindow{ window } {}

void GameWorld::init()
{
	auto player = std::make_unique< Player >( this, b2Vec2{ 400.0f, 300.0f }, 5.0f );
	m_gameEntities.emplace_back( std::move( player ) );

	initGameEntities();
}

void GameWorld::teardown() {}

void GameWorld::render( sf::RenderWindow* window )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->render( window );
	}
}

void GameWorld::update( float deltaTime )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->update( deltaTime );
	}
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

} // namespace Graphics
} // namespace SteeringBehaviors
