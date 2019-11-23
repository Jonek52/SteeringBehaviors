#pragma once

#include "Updatable.h"
#include "Renderable.h"
#include "..\Input\InputHandling.h"

#include <memory>

namespace sf
{
class Window;
}
namespace SteeringBehaviors
{
namespace Graphics
{

class GameWorld;
class Game : public Updatable, public Renderable, public Input::InputHandling
{

public:
	explicit Game( sf::Window* window );
	virtual ~Game();

	Game( Game&& ) noexcept;
	Game& operator=( Game&& ) noexcept;

	Game( const Game& ) = delete;

	Game& operator=( const Game& ) = delete;

	virtual void init();
	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update(std::chrono::milliseconds delta) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

private:
	sf::Window* m_mainWindow;
	std::unique_ptr< GameWorld > m_gameWorld;
};

} // namespace Graphics
} // namespace SteeringBehaviors
