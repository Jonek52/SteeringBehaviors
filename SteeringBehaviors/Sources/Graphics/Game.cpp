#include "Game.h"
#include "Level.h"

#include <SFML\Graphics.hpp>

namespace SteeringBehaviors
{
namespace Graphics
{
Game::Game() : m_level{ nullptr }
{
	init();
};

Game::~Game() = default;

Game::Game( Game&& ) = default;

Game& Game::operator=( Game&& ) = default;

void Game::init()
{
	m_level = std::make_unique< Level >();
	m_level->init();
}

void Game::teardown(){};

void Game::render( sf::RenderWindow* window )
{
	window->clear();
	m_level->render( window );
	window->display();
}

void Game::update( float deltaTime )
{
	m_level->update( deltaTime );
}

void Game::processEvents( sf::Event& event )
{
	m_level->processEvents( event );
}

void Game::processInput( sf::Window* window )
{
	m_level->processInput( window );
}

} // namespace Graphics
} // namespace SteeringBehaviors
