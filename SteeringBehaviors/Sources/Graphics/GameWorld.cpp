#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Window\Window.hpp"

#include "GameWorld.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Obstacle.h"

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

	auto obstacle1 = new Obstacle( this, 0.0f, Math::Vector2{ 700.f, 300.f }, 30.0f );
	m_gameEntities.push_back( obstacle1 );
	m_obstacles.push_back( obstacle1 );

	auto obstacle2 = new Obstacle( this, 0.0f, Math::Vector2{ 400.f, 600.f }, 20.0f );
	m_gameEntities.push_back( obstacle2 );
	m_obstacles.push_back( obstacle2 );

	auto obstacle3 = new Obstacle( this, 0.0f, Math::Vector2{ 700.f, 700.f }, 40.0f );
	m_gameEntities.push_back( obstacle3 );
	m_obstacles.push_back( obstacle3 );

	auto obstacle4 = new Obstacle( this, 0.0f, Math::Vector2{ 100.f, 500.f }, 30.0f );
	m_gameEntities.push_back( obstacle4 );
	m_obstacles.push_back( obstacle4 );

	auto leftWall = new Wall( this, 0.0f, Wall::Orientation::VERTICAL, Wall::Side::LEFT );
	m_gameEntities.push_back( leftWall );
	m_walls.push_back( leftWall );

	auto rightWall = new Wall( this, 0.0f, Wall::Orientation::VERTICAL, Wall::Side::RIGHT );
	m_gameEntities.push_back( rightWall );
	m_walls.push_back( rightWall );

	auto upperWall = new Wall( this, 0.0f, Wall::Orientation::HORIZONTAL, Wall::Side::UP );
	m_gameEntities.push_back( upperWall );
	m_walls.push_back( upperWall );

	auto downWall = new Wall( this, 0.0f, Wall::Orientation::HORIZONTAL, Wall::Side::DOWN );
	m_gameEntities.push_back( downWall );
	m_walls.push_back( downWall );
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

void GameWorld::tagObstaclesWithinRange( GameEntity* object, float range )
{
	tagNeightbors( object, m_obstacles, range );
}

void GameWorld::tagNeightbors( GameEntity* object, std::vector< GameEntity* >& obstacles, float radius )
{

	for( auto& obstacle : obstacles )
	{
		obstacle->untag();

		Math::Vector2 to = object->getPosition() - obstacle->getPosition();
		float range		 = radius + obstacle->getRadius();

		if( ( obstacle != object ) && to.lengthSquared() < range * range )
		{
			obstacle->tag();
		}
	}
}

Player* GameWorld::getPlayer() const
{
	return m_player;
}

std::vector< GameEntity* >& GameWorld::getObstacles()
{
	return m_obstacles;
}

std::vector< Wall* >& GameWorld::getWalls()
{
	return m_walls;
}

sf::Window* GameWorld::getWindow() const
{
	return m_mainWindow;
}

} // namespace Graphics
} // namespace SteeringBehaviors
