#include "Player.h"
#include <SFML\Graphics\Shape.hpp>
#include <SFML\Graphics.hpp>

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player() = default;

Player::~Player() = default;

void Player::init()
{
	m_playerShape = std::make_unique< CircleShape >( 30.0f, 3 );
	m_playerShape->setFillColor( sf::Color::Green );
}

void Player::teardown() {}

void Player::render( RenderWindow* window )
{
	if( m_isDirty )
	{
		window->draw( *m_playerShape );
	}
}

void Player::update( float deltaTime )
{
	m_isDirty = true;
}
void Player::processInput( Event& event ) {}
} // namespace Graphics
}; // namespace SteeringBehaviors
