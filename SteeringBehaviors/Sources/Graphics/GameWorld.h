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
	enum CollisionCategory : int
	{
		PLAYER	 = 0x0001,
		ENEMY	 = 0x0002,
		OBSTACLE = 0x0004,
		BALL	 = 0x0008
	};

	explicit GameWorld( sf::Window* window );
	~GameWorld() = default;

	virtual void init();

	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update( float delta ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

	virtual void tagObstaclesWithinRange( GameEntity* object, float range );
	virtual void tagFriendsWithinRange( Enemy* object, float range );

	virtual sf::Window* getWindow() const;
	virtual Player* getPlayer() const;
	virtual std::vector< Obstacle* >& getObstacles();
	virtual std::vector< Wall* >& getWalls();
	virtual std::vector< Enemy* >& getEnemies();

protected:
	virtual void initGameEntities();
	template< typename T, typename U >
	void tagNeightbors( const U* object, const std::vector< T* >& objects, float radius )
	{
		for( auto& obj : objects )
		{
			obj->untag();

			Math::Vector2 to = object->getPosition() - obj->getPosition();
			float range		 = radius + obj->getRadius();

			if( ( obj != object ) && to.lengthSquared() < range * range )
			{
				obj->tag();
			}
		}
	}

private:
	sf::Window* m_mainWindow;
	std::vector< GameEntity* > m_gameEntities;
	std::vector< Obstacle* > m_obstacles;
	std::vector< Enemy* > m_enemies;
	std::vector< Wall* > m_walls;
	Player* m_player;
};

} // namespace Graphics
} // namespace SteeringBehaviors
