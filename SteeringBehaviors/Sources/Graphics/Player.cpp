#include "stdafx.h"

#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "SFML\Graphics\Vertex.hpp"

#include "GameWorld.h"
#include "Player.h"
#include "../Math/MathFunctions.h"

#include <iostream>

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position )
	: MovingEntity{ gameWorld, maxSpeed, 5.0f, 5.0f, position }
{
	m_radius = 5.0f;
	init();
}

Player::~Player() = default;

void Player::init()
{
	m_lookDirection = Math::Vector2{ 0.f, -1.f };
	m_sideDirection = m_lookDirection.perp();

	initGfxPart();
	initPhysicalPart();
}

void Player::teardown() {}

void Player::update( float delta )
{

	applyForce();
	move( delta );
	rotate();
	handleShooting( delta );
}

void Player::handleShooting( float delta )
{
	if( wantsToShoot )
	{
		if( onCooldown )
		{
			wantsToShoot = false;
			wait();
		}
		else
		{
			m_counter  = std::chrono::steady_clock::now();
			onCooldown = true;
			m_drawLine = true;
			shootBall();
		}
	}
}

void Player::shootBall()
{
	spawnBall();
}

void Player::spawnBall()
{
	// m_ball.graphicalBody = std::make_unique< sf::CircleShape >( m_ball.radius, 12 );

	// sf::Vector2f origin{ 0.f, 0.f };
	// for( int i = 0; i < m_ball.graphicalBody->getPointCount(); ++i )
	//{
	//	origin += m_ball.graphicalBody->getPoint( i );
	//}

	// origin /= static_cast< float >( m_ball.graphicalBody->getPointCount() );

	// m_ball.graphicalBody->setOrigin( origin );
	// m_ball.graphicalBody->setFillColor( sf::Color::Yellow );
}

void Player::wait()
{
	auto now	 = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( now - m_counter );

	if( elapsed >= std::chrono::milliseconds( 1500 ) )
	{
		onCooldown = false;
	}
}

void Player::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );
}

void Player::move( float delta )
{
	m_position += m_velocity * delta;
	m_graphicalBody->setPosition( Math::toSFMLVector( m_position ) );
}

void Player::applyForce()
{
	Math::Vector2 velocity{ 0.f, 0.f };

	if( m_moveLeft )
		velocity += Math::Vector2{ -1.f, 0.f };

	if( m_moveRight )
		velocity += Math::Vector2{ 1.f, 0.f };

	if( m_moveUp )
		velocity += Math::Vector2{ 0.f, 1.f };

	if( m_moveDown )
		velocity += Math::Vector2{ 0.f, -1.f };

	m_velocity = Math::Vec2DNormalize( velocity ) * m_maxSpeed;
}

void Player::processInput()
{
	handleKeyboard();
	handleMouse();
}

void Player::processEvents( sf::Event& event )
{
	if( event.type == sf::Event::KeyPressed )
	{
		if( event.key.code == sf::Keyboard::Space )
		{
			wantsToShoot = true;
		}
	}
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

void Player::handleMouse()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition( *m_gameWorld->getWindow() );
	sf::Vector2f fmousePosition{ static_cast< float >( mousePosition.x ), static_cast< float >( mousePosition.y ) };

	m_targetDirection = Math::toMathVector( fmousePosition - m_graphicalBody->getPosition() );
	m_targetDirection.normalize();
}

void Player::rotate()
{
	float cos				   = m_lookDirection.x * m_targetDirection.x + m_lookDirection.y * m_targetDirection.y;
	float sin				   = m_lookDirection.x * m_targetDirection.y - m_lookDirection.y * m_targetDirection.x;
	float rotationAngle		   = atan2( sin, cos );
	float rotationAngleDegrees = Math::toDegrees( rotationAngle );

	m_lookDirection = m_targetDirection;
	m_sideDirection = m_lookDirection.perp();

	m_graphicalBody->rotate( rotationAngleDegrees );
}

void Player::initGfxPart()
{

	auto playerGfx = std::make_unique< sf::ConvexShape >();
	playerGfx->setFillColor( sf::Color::Green );

	playerGfx->setPointCount( 3 );

	std::vector< sf::Vector2f > points;
	points.emplace_back( 0.0f, -50.0f );
	points.emplace_back( -25.0f, 0.0f );
	points.emplace_back( 25.0f, 0.0f );

	sf::Vector2f center{ 0.0f, 0.0f };

	for( const auto& p : points )
	{
		center += p;
	}

	center /= 3.0f;

	playerGfx->setPoint( 0, points[ 0 ] - center );
	playerGfx->setPoint( 1, points[ 1 ] - center );
	playerGfx->setPoint( 2, points[ 2 ] - center );

	m_graphicalBody = std::move( playerGfx );
}

void Player::initPhysicalPart() {}

void Player::wrapScreenPosition()
{
	auto playerShape = dynamic_cast< sf::CircleShape* >( m_graphicalBody.get() );

	float currentPosX = playerShape->getPosition().x;
	float currentPosY = playerShape->getPosition().y;

	if( currentPosX > m_gameWorld->getWindow()->getSize().x - playerShape->getRadius() )
	{
		currentPosX = 0.0f + playerShape->getRadius();
	}
	else if( currentPosX < 0.0f + playerShape->getRadius() )
	{
		currentPosX = m_gameWorld->getWindow()->getSize().x - playerShape->getRadius();
	}

	if( currentPosY > m_gameWorld->getWindow()->getSize().y - playerShape->getRadius() )
	{
		currentPosY = 0.0f + playerShape->getRadius();
	}
	else if( currentPosY < 0.0f + playerShape->getRadius() )
	{
		currentPosY = m_gameWorld->getWindow()->getSize().y - playerShape->getRadius();
	}
}

} // namespace Graphics
} // namespace SteeringBehaviors
