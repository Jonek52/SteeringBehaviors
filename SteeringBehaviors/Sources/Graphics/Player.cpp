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
Player::Player( GameWorld* gameWorld, float maxSpeed ) : GameEntity{ gameWorld, maxSpeed }
{
	init();
}

Player::~Player() = default;

void Player::init()
{

	initGfxPart();
	initPhysicalPart();
}

void Player::teardown() {}

void Player::update( std::chrono::milliseconds delta )
{

	applyForce();
	handleShooting( delta );
	rotate();
}

void Player::handleShooting( std::chrono::milliseconds delta )
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
	m_ball.graphicalBody = std::make_unique< sf::CircleShape >( m_ball.radius, 12 );

	sf::Vector2f origin{ 0.f, 0.f };
	for( int i = 0; i < m_ball.graphicalBody->getPointCount(); ++i )
	{
		origin += m_ball.graphicalBody->getPoint( i );
	}

	origin /= static_cast< float >( m_ball.graphicalBody->getPointCount() );

	m_ball.graphicalBody->setOrigin( origin );
	m_ball.graphicalBody->setFillColor( sf::Color::Yellow );
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

	if( m_ball.graphicalBody )
	{
		window->draw( *m_ball.graphicalBody );
	}
}

void Player::move( float deltaTime ) {}

void Player::applyForce()
{
	// if( m_moveLeft )

	//	if( m_moveRight )

	//		if( m_moveLeft && m_moveRight )

	//			if( m_moveUp )

	//				if( m_moveDown )

	//					if( m_moveUp && m_moveDown )
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
}

void Player::rotate()
{

	// m_graphicalBody->rotate( rotationAngleDegrees );
}

void Player::initGfxPart()
{
	m_graphicalBody = std::make_unique< sf::ConvexShape >();
	m_graphicalBody->setFillColor( sf::Color::Green );
	auto* playerGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	playerGfx->setPointCount( 3 );

	std::vector< sf::Vector2f > points;
	points.emplace_back( 0.0f, -100.0f );
	points.emplace_back( -50.0f, 0.0f );
	points.emplace_back( 50.0f, 0.0f );

	sf::Vector2f center{ 0.0f, 0.0f };

	for( const auto& p : points )
	{
		center += p;
	}

	center /= 3.0f;

	playerGfx->setPoint( 0, points[ 0 ] - center );
	playerGfx->setPoint( 1, points[ 1 ] - center );
	playerGfx->setPoint( 2, points[ 2 ] - center );

	m_graphicalBody->setScale( { 0.7f, 0.7f } );
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
