#pragma once
#include "GameEntity.h"

#include <vector>
#include <memory>

namespace sf
{
class RenderWindow;
class Window;
} // namespace sf
class b2World;

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld : public Updatable, public Renderable, public Input::InputHandling
{
public:
	enum CollisionCategory : uint16
	{
		PLAYER	 = 0x0001,
		ENEMY	 = 0x0002,
		OBSTACLE = 0x0004
	};

	explicit GameWorld( sf::Window* window );
	~GameWorld() = default;

	virtual void init();
	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update(std::chrono::milliseconds delta) override;
	virtual void step( float deltaTime );
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;
	virtual sf::Window* getWindow() const;
	virtual std::shared_ptr< b2World >& getPhysicalWorld();

protected:
	virtual void initGameEntities();

private:
	sf::Window* m_mainWindow;
	std::shared_ptr< b2World > m_world;
	std::vector< std::unique_ptr< GameEntity > > m_gameEntities;
};

} // namespace Graphics
} // namespace SteeringBehaviors
