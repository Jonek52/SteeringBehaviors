#include "Game.h"
#include "GameWorld.h"

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

namespace SteeringBehaviors
{
namespace Graphics
{
Game::Game( sf::Window* window ) : m_mainWindow{ window }, m_gameWorld{ nullptr }
{
	init();
};

Game::~Game() = default;

Game::Game( Game&& ) noexcept = default;

Game& Game::operator=( Game&& ) noexcept = default;

void Game::init()
{
	m_gameWorld = std::make_unique< GameWorld >( m_mainWindow );
	m_gameWorld->init();
}

void Game::teardown(){};

void Game::render( sf::RenderWindow* window )
{
	window->clear();
	m_gameWorld->render( window );
	window->display();
}

void Game::update()
{
	m_gameWorld->update();
}

void Game::step( float deltaTime )
{
	m_gameWorld->step( deltaTime );
}
void Game::processInput()
{
	m_gameWorld->processInput();
}

void Game::processEvents( sf::Event& event )
{
	m_gameWorld->processEvents( event );
}

} // namespace Graphics
} // namespace SteeringBehaviors
