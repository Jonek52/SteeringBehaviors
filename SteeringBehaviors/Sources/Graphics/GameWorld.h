#pragma once
#include "GameEntity.h"

#include <vector>
#include <memory>

namespace sf
{
class RenderWindow;
class Window;
} // namespace sf

namespace SteeringBehaviors
{
namespace Graphics
{
class Player;
class Enemy;
class GameEntity;
class Obstacle;
class Wall;

class GameWorld : public Updatable, public Renderable, public Input::InputHandling
{
public:
	explicit GameWorld( sf::Window* window );
	~GameWorld() = default;

	virtual void init();

	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update( float delta ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

	virtual void tagObstaclesWithinRange( const GameEntity& object, float range );
	virtual void tagFriendsWithinRange( const Enemy& object, float range );
	virtual void markIntersectedEnemies( const Player& object );

	virtual sf::Window* getWindow() const;

	virtual shared_ptr< Player >& getPlayer();
	virtual const vector< shared_ptr< Obstacle > >& getObstacles();
	virtual const vector< shared_ptr< Wall > >& getWalls();
	virtual const vector< shared_ptr< Enemy > >& getEnemies();

protected:
	virtual void initGameEntities();
	template< typename T, typename U >
	void tagNeightbors( const U& object, const vector< shared_ptr< T > >& objects, float radius )
	{
		for( auto& obj : objects )
		{
			obj->untag();

			Math::Vector2 to = object.getPosition() - obj->getPosition();
			float range		 = radius + obj->getRadius();

			if( ( obj.get() != &object ) && to.lengthSquared() < range * range )
			{
				obj->tag();
			}
		}
	}

private:
	sf::Window* m_mainWindow;
	vector< shared_ptr< GameEntity > > m_gameEntities;
	vector< shared_ptr< Obstacle > > m_obstacles;
	vector< shared_ptr< Enemy > > m_enemies;
	vector< shared_ptr< Wall > > m_walls;
	shared_ptr< Player > m_player;
};

} // namespace Graphics
} // namespace SteeringBehaviors
