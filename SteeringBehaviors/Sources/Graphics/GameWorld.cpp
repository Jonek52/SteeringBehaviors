#include "stdafx.h"

#include "SFML\Graphics\CircleShape.hpp"
#include "SFML\Window\Window.hpp"

#include "GameWorld.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "Obstacle.h"

#include "..\Math\MathFunctions.h"
#include "..\Math\Geometry.h"

namespace SteeringBehaviors
{
namespace Graphics
{

GameWorld::GameWorld( sf::Window* window ) : m_mainWindow{ window } {}

void GameWorld::init()
{
	m_player = make_shared< Player >( this, 400.0f, Vector2{ 100.f, 100.f } );
	m_gameEntities.push_back( m_player );

	shared_ptr< Enemy > enemy;
	for( int i = 0; i < 50; ++i )
	{
		enemy = make_shared< Enemy >(
			this, 150.0f, Vector2{ Math::randInRange( 0.0f, 800.f ), Math::randInRange( 0.f, 600.f ) } );
		m_gameEntities.push_back( enemy );
		m_enemies.push_back( enemy );
	}

	auto obstacle1 = make_shared< Obstacle >( this, Vector2{ 700.f, 300.f }, 30.0f );
	m_gameEntities.push_back( obstacle1 );
	m_obstacles.push_back( obstacle1 );

	auto obstacle2 = make_shared< Obstacle >( this, Vector2{ 400.f, 400.f }, 20.0f );
	m_gameEntities.push_back( obstacle2 );
	m_obstacles.push_back( obstacle2 );

	auto obstacle3 = make_shared< Obstacle >( this, Vector2{ 500.f, 200.f }, 40.0f );
	m_gameEntities.push_back( obstacle3 );
	m_obstacles.push_back( obstacle3 );

	auto obstacle4 = make_shared< Obstacle >( this, Vector2{ 100.f, 500.f }, 30.0f );
	m_gameEntities.push_back( obstacle4 );
	m_obstacles.push_back( obstacle4 );

	auto obstacle5 = make_shared< Obstacle >( this, Vector2{ 200.f, 200.f }, 30.0f );
	m_gameEntities.push_back( obstacle5 );
	m_obstacles.push_back( obstacle5 );

	auto obstacle6 = make_shared< Obstacle > (this, Vector2 { 650.f, 200.f }, 10.0f);
	m_gameEntities.push_back (obstacle6);
	m_obstacles.push_back (obstacle6);

	// auto leftWall = make_shared< Wall >( this, Wall::Orientation::VERTICAL, Wall::Side::LEFT );
	// m_gameEntities.push_back( leftWall );
	// m_walls.push_back( leftWall );

	// auto rightWall = make_shared< Wall >( this, Wall::Orientation::VERTICAL, Wall::Side::RIGHT );
	// m_gameEntities.push_back( rightWall );
	// m_walls.push_back( rightWall );

	// auto upperWall = make_shared< Wall >( this, Wall::Orientation::HORIZONTAL, Wall::Side::UP );
	// m_gameEntities.push_back( upperWall );
	// m_walls.push_back( upperWall );

	// auto downWall = make_shared< Wall >( this, Wall::Orientation::HORIZONTAL, Wall::Side::DOWN );
	// m_gameEntities.push_back( downWall );
	// m_walls.push_back( downWall );
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
	auto lastEnemyIter = std::remove_if (
		m_enemies.begin (), m_enemies.end (), [](shared_ptr< Enemy >& enemy) { return enemy->toDelete (); });
	m_enemies.erase (lastEnemyIter, m_enemies.end ());

	auto lastEntetiesIter = std::remove_if( m_gameEntities.begin(),
											m_gameEntities.end(),
											[]( shared_ptr< GameEntity >& entity ) { return entity->toDelete(); } );

	m_gameEntities.erase( lastEntetiesIter, m_gameEntities.end() );
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

void GameWorld::tagObstaclesWithinRange( const GameEntity& object, float range )
{
	tagNeightbors( object, m_obstacles, range );
}

void GameWorld::tagFriendsWithinRange( const Enemy& object, float range )
{
	tagNeightbors( object, m_enemies, range );
}

void GameWorld::markIntersectedEnemies( const Player& player )
{
	Vector2 rayOrigin			   = player.getPosition();
	Vector2 rayDirection		   = player.getLookDirection();
	float distSqrtToClosesObstacle = std::numeric_limits< float >::max();

	for( auto& enemy : m_enemies )
	{
		if( !Math::DoRayCircleIntersect( rayOrigin, rayDirection, enemy->getPosition(), enemy->getRadius() ) )
		{
			for( const auto& obstacle : m_obstacles )
			{
				if( !Math::DoRayCircleIntersect(
						rayOrigin, rayDirection, obstacle->getPosition(), obstacle->getRadius() ) )
				{
					Vector2 toObstacle = obstacle->getPosition() - player.getPosition();

					if( toObstacle.lengthSquared() < distSqrtToClosesObstacle )
					{
						distSqrtToClosesObstacle = toObstacle.lengthSquared();
					}
				}
			}

			Vector2 toEnemy = enemy->getPosition() - player.getPosition();

			if( toEnemy.lengthSquared() < distSqrtToClosesObstacle )
			{
				enemy->markAsDeleted();
			}
		}
	}
}

shared_ptr< Player >& GameWorld::getPlayer()
{
	return m_player;
}

const vector< shared_ptr< Obstacle > >& GameWorld::getObstacles()
{
	return m_obstacles;
}

const vector< shared_ptr< Wall > >& GameWorld::getWalls()
{
	return m_walls;
}

const vector< shared_ptr< Enemy > >& GameWorld::getEnemies()
{
	return m_enemies;
}

sf::Window* GameWorld::getWindow() const
{
	return m_mainWindow;
}

} // namespace Graphics
} // namespace SteeringBehaviors
