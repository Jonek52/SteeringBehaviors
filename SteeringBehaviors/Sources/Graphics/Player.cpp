#include <SFML\Graphics\Shape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Player.h"
#include "../Math/MathFunctions.h"

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( Vec position )
	: m_playerShape{ nullptr },
	  m_position{ position },
	  m_velocity{ 0.0f, 0.0f },
	  m_lookDirection{ 0.0f, -1.0f },
	  m_maxSpeed{ 5.0f }
{
}

Player::~Player() = default;

void Player::init()
{
	m_playerShape = std::make_unique< CircleShape >( 20.0f, 3 );
	m_playerShape->setPosition( sf::Vector2< float >{ m_position.x, m_position.y } );
	m_playerShape->setFillColor( sf::Color::Green );

	float playerCenterX{ 0.0f };
	float playerCenterY{ 0.0f };

	for( int i = 0; i < m_playerShape->getPointCount(); ++i )
	{
		playerCenterX += m_playerShape->getPoint( i ).x;
		playerCenterY += m_playerShape->getPoint( i ).y;
	}

	m_playerShape->setOrigin( sf::Vector2< float >{ playerCenterX / 3.0f, playerCenterY / 3.0f } );
}

void Player::teardown() {}

void Player::render( RenderWindow* window )
{
	window->draw( *m_playerShape );
}

void Player::update( float deltaTime )
{
	calculateVelocity();

	if( m_velocity.LengthSquared() > 0.0f )
	{
		move( deltaTime );
	}
}

void Player::move( float deltaTime )
{
	m_velocity *= deltaTime;
	m_position = m_position + m_velocity;
	m_playerShape->setPosition( sf::Vector2< float >{ m_position.x, m_position.y } );
}

void Player::calculateVelocity()
{
	m_velocity = Vec{ 0.0f, 0.0f };

	if( m_moveLeft )
		m_velocity += Vec{ -1.0f, 0.0f };

	if( m_moveRight )
		m_velocity += Vec{ 1.0f, 0.0f };

	if( m_moveLeft && m_moveRight )
		m_velocity += Vec{ 0.0f, 0.0f };

	if( m_moveUp )
		m_velocity += Vec{ 0.0f, 1.0f };

	if( m_moveDown )
		m_velocity += Vec{ 0.0f, -1.0f };

	if( m_moveUp && m_moveDown )
		m_velocity += Vec{ 0.0f, 0.0f };

	m_velocity.Normalize();
	m_velocity *= m_maxSpeed;
}

void Player::processEvents( Event& event ) {}

void Player::processInput( sf::Window* window )
{
	handleKeyboard( window );
	handleMouse( window );
}
void Player::handleKeyboard( sf::Window* window )
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
void Player::handleMouse( sf::Window* window )
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition( *window );
	Vec b2mousePosition		   = Math::toBox2DVector( mousePosition );
	Vec targetDirection		   = b2mousePosition - m_position;
	targetDirection.Normalize();

	rotate( targetDirection );
}

void Player::rotate( const Vec& targetDirection )
{
	float cos				   = m_lookDirection.x * targetDirection.x + m_lookDirection.y * targetDirection.y;
	float sin				   = m_lookDirection.x * targetDirection.y - m_lookDirection.y * targetDirection.x;
	float rotationAngle		   = atan2( sin, cos );
	float rotationAngleDegrees = Math::toDegrees( rotationAngle );

	m_lookDirection = targetDirection;
	m_playerShape->rotate( rotationAngleDegrees );
}

} // namespace Graphics
} // namespace SteeringBehaviors
