#include "Box2D\Common\b2Math.h"
#include "Level.h"
#include "Player.h"

namespace SteeringBehaviors
{
namespace Graphics
{

void Level::init()
{
	auto player = std::make_unique< Player >( b2Vec2{ 400.0f, 300.0f } );
	m_gameEntities.emplace_back( std::move( player ) );

	initGameEntities();
}

void Level::teardown() {}

void Level::render( sf::RenderWindow* window )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->render( window );
	}
}

void Level::update( float deltaTime )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->update( deltaTime );
	}
}

void Level::processEvents( Event& event )
{
	for( const auto& gameEnity : m_gameEntities )
	{
		gameEnity->processEvents( event );
	}
}

void Level::initGameEntities()
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->init();
	}
}

void Level::processInput( sf::Window* window )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->processInput( window );
	}
}

} // namespace Graphics
} // namespace SteeringBehaviors
