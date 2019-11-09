#pragma once

#include "Updatable.h"
#include "Renderable.h"
#include "../Input/EventHandling.h"

#include <memory>

namespace SteeringBehaviors
{
namespace Graphics
{

class Level;
class Game : public Updatable, public Renderable, public Input::EventHandling
{
public:
	Game();
	virtual ~Game();

	Game( Game&& );
	Game& operator=( Game&& );

	Game( const Game& ) = delete;

	Game& operator=( const Game& ) = delete;

	virtual void init();
	virtual void teardown();
	virtual void render( sf::RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;
	virtual void processEvents( sf::Event& event ) override;

private:
	std::unique_ptr< Level > m_level;
};

} // namespace Graphics
} // namespace SteeringBehaviors
