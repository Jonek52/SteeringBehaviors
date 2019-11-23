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
	virtual Player* getPlayer();

	virtual sf::Window* getWindow() const;

protected:
	virtual void initGameEntities();

private:
	sf::Window* m_mainWindow;
	std::vector< GameEntity* > m_gameEntities;
	Player* m_player;
};

} // namespace Graphics
} // namespace SteeringBehaviors
