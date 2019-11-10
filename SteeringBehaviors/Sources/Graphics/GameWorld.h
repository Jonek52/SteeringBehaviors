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
class GameWorld : public Updatable, public Renderable, public Input::InputHandling
{
public:
	explicit GameWorld( sf::Window* window );
	~GameWorld() = default;

	virtual void init();
	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;
	virtual sf::Window* getWindow() const;

protected:
	virtual void initGameEntities();

private:
	sf::Window* m_mainWindow;
	std::vector< std::unique_ptr< GameEntity > > m_gameEntities;
};

} // namespace Graphics
} // namespace SteeringBehaviors