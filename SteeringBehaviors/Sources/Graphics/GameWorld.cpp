#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Window\Window.hpp"

#include "GameWorld.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"

namespace SteeringBehaviors
{
namespace Graphics
{

GameWorld::GameWorld( sf::Window* window ) : m_mainWindow{ window } {}

void GameWorld::init()
{
	m_player = new Player( this, 50.0f );
	m_gameEntities.push_back( m_player );

	auto enemy = new Enemy( this, 15.0f );
	m_gameEntities.push_back( enemy );

	// auto leftWall = std::make_unique< Wall >( this, 0.0f, Wall::Orientation::VERTICAL, Wall::Side::LEFT );
	// m_gameEntities.push_back( std::move( leftWall ) );

	// auto rightWall = std::make_unique< Wall >( this, 0.0f, Wall::Orientation::VERTICAL, Wall::Side::RIGHT );
	// m_gameEntities.push_back( std::move( rightWall ) );

	// auto upperWall = std::make_unique< Wall >( this, 0.0f, Wall::Orientation::HORIZONTAL, Wall::Side::UP );
	// m_gameEntities.push_back( std::move( upperWall ) );

	// auto downWall = std::make_unique< Wall >( this, 0.0f, Wall::Orientation::HORIZONTAL, Wall::Side::DOWN );
	// m_gameEntities.push_back( std::move( downWall ) );
}

void GameWorld::teardown() {}

void GameWorld::render( sf::RenderWindow* window )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->render( window );
	}
}

void GameWorld::update( float delta )
{
	for( const auto& gameEntity : m_gameEntities )
	{
		gameEntity->update( delta );
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

Player* GameWorld::getPlayer()
{
	return m_player;
}

sf::Window* GameWorld::getWindow() const
{
	return m_mainWindow;
}

} // namespace Graphics
} // namespace SteeringBehaviors
