#include <SFML\Graphics\Shape.hpp>
#include <SFML\Graphics.hpp>

#include "Player.h"
#include "..\Math\MathFunctions.h"

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( sf::Vector2< float > position )
	: m_playerShape{ nullptr }, m_position{ position }, m_velocity{ 0.0f, 0.0f }, m_maxSpeed{ 5.0f }
{
}

Player::~Player() = default;

void Player::init()
{
	m_playerShape = std::make_unique< CircleShape >( 20.0f, 3 );
	m_playerShape->setPosition( m_position );
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
	processInput();
	calculateVelocity();

	m_position = m_position + m_velocity;
	m_playerShape->setPosition( m_position );
	m_isDirty = true;

}

void Player::calculateVelocity()
{
	m_velocity = sf::Vector2< float >{ 0.0f, 0.0f };

	if( m_moveLeft )
		m_velocity += sf::Vector2< float >{ -1.0f, 0.0f };

	if( m_moveRight )
		m_velocity += sf::Vector2< float >{ 1.0f, 0.0f };

	if( m_moveLeft && m_moveRight )
		m_velocity += sf::Vector2< float >{ 0.0f, 0.0f };

	if( m_moveUp )
		m_velocity += sf::Vector2< float >{ 0.0f, 1.0f };

	if( m_moveDown )
		m_velocity += sf::Vector2< float >{ 0.0f, -1.0f };

	if( m_moveUp && m_moveDown )
		m_velocity += sf::Vector2< float >{ 0.0f, 0.0f };

	if( Math::vectorLenght( m_velocity ) > 0.0f )
	{
		m_velocity = Math::normalizeVector( m_velocity );
		m_velocity = m_velocity * m_maxSpeed;
	}
}

void Player::processEvents( Event& event ) {}

void Player::processInput()
{
	handleKeyboard();
	handleMouse();
}
void Player::handleKeyboard()
{
	using sf::Keyboard;

	if( Keyboard::isKeyPressed( Keyboard::A ) )
		m_moveLeft = true;
	else
		m_moveLeft = false;

	if( Keyboard::isKeyPressed( Keyboard::D ) )
		m_moveRight = true;
	else
		m_moveRight = false;

	if( Keyboard::isKeyPressed( Keyboard::W ) )
		m_moveDown = true;
	else
		m_moveDown = false;

	if( Keyboard::isKeyPressed( Keyboard::S ) )
		m_moveUp = true;
	else
		m_moveUp = false;
}
void Player::handleMouse() {}

} // namespace Graphics
} // namespace SteeringBehaviors
