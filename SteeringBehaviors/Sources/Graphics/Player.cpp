#include "stdafx.h"

#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "SFML\Graphics\Vertex.hpp"

#include "GameWorld.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Player.h"
#include "../Math/MathFunctions.h"

#include <iostream>

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position )
	: MovingEntity{ gameWorld, maxSpeed, 2000.0f, 30.0f, position }
{
	init();
}

Player::~Player() = default;

void Player::init()
{
	m_lookDirection = Math::Vector2{ 0.f, -1.f };
	m_sideDirection = m_lookDirection.perp();

	initText();
	initGfxPart();
	initPhysicalPart();
}

void Player::teardown() {}

void Player::update( float delta )
{
	handleCollisions( m_gameWorld->getObstacles() );
	move( delta );
	rotate();
	handleShooting( delta );
	handleLineDrawing( delta );
	handleCollisionWithEnemy();
	updateHealth();
	shouldDie();
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
			m_shotCooldown = std::chrono::steady_clock::now();
			onCooldown	   = true;
			m_drawLine	   = true;
			m_createLine   = true;
			checkIntersectionWithEnemies();
		}
	}
}

void Player::wait()
{
	auto now	 = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast< std::chrono::milliseconds >( now - m_shotCooldown );

	if( elapsed >= std::chrono::milliseconds( 1500 ) )
	{
		onCooldown = false;
	}
}

void Player::render( RenderWindow* window )
{
	window->draw( *m_graphicalBody );

	if( m_drawLine )
	{
		window->draw( m_line.data(), 2, sf::Lines );
	}

	window->draw( *m_healthText );
}

void Player::move( float delta )
{
	Vector2 moveForce	 = calculateForce();
	Vector2 acceleration = moveForce / m_mass;

	m_velocity += acceleration * delta;
	m_velocity.truncate( m_maxSpeed );

	m_position += m_velocity * delta;

	if( acceleration.length() <= 0.0f )
	{
		m_velocity *= 0.9f;
	}
	m_graphicalBody->setPosition( Math::toSFMLVector( m_position ) );
	WrapAround( m_position, m_gameWorld->getWindow()->getSize().x, m_gameWorld->getWindow()->getSize().y );
}

Vector2 Player::calculateForce()
{
	Math::Vector2 force{ 0.f, 0.f };

	if( m_moveLeft )
		force += Math::Vector2{ -1.f, 0.f };

	if( m_moveRight )
		force += Math::Vector2{ 1.f, 0.f };

	if( m_moveUp )
		force += Math::Vector2{ 0.f, 1.f };

	if( m_moveDown )
		force += Math::Vector2{ 0.f, -1.f };

	return Math::Vec2DNormalize( force ) * m_maxForce;
}

void Player::processInput()
{
	handleKeyboard();
	handleMouse();
}

void Player::processEvents( sf::Event& event )
{
	if( event.type == sf::Event::MouseButtonPressed )
	{
		if( event.key.code == sf::Mouse::Left )
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

void Player::handleCollisions( const vector< shared_ptr< Obstacle > >& obstacles )
{
	m_gameWorld->tagObstaclesWithinRange( *this, m_radius );

	for( const auto& obstacle : obstacles )
	{
		if( obstacle->isTagged() )
		{
			Vector2 playerToObstacle = getPosition() - obstacle->getPosition();
			Vector2 moveVector		 = Math::Vec2DNormalize( playerToObstacle ) * ( obstacle->getRadius() + m_radius );
			m_position				 = obstacle->getPosition() + moveVector;
		}
	}
}

void Player::handleLineDrawing( float deltaTime )
{
	if( m_drawLine )
	{
		if( m_createLine )
		{
			m_line[ 0 ] = sf::Vertex{ Math::toSFMLVector( m_position + m_lookDirection * m_radius ) };
			m_line[ 1 ] = sf::Vertex{ Math::toSFMLVector( m_position + m_lookDirection * 800.0f ) };
			m_createLineParam += deltaTime;
			if( m_createLineParam / s_createLineTime <= 1.0f )
			{
				sf::Uint8 alpha = static_cast< sf::Uint8 >(
					Math::lerp< float >( 0, 255, ( m_createLineParam / s_createLineTime ) ) );
				m_line[ 0 ].color = sf::Color( sf::Color{ 255, 0, 0, alpha } );
				m_line[ 1 ].color = sf::Color( sf::Color{ 255, 0, 0, alpha } );
			}
			else
			{
				m_createLine	  = false;
				m_hideLine		  = true;
				m_createLineParam = 0.0f;
			}
		}

		if( m_hideLine )
		{
			m_line[ 0 ] = sf::Vertex{ Math::toSFMLVector( m_position + m_lookDirection * m_radius ) };
			m_line[ 1 ] = sf::Vertex{ Math::toSFMLVector( m_position + m_lookDirection * 800.0f ) };
			m_hideLineParam += deltaTime;

			if( m_hideLineParam / s_hideLineTime <= 1.0f )
			{
				sf::Uint8 alpha
					= static_cast< sf::Uint8 >( Math::lerp< float >( 255, 0, ( m_hideLineParam / s_hideLineTime ) ) );
				sf::Uint8 oneMinusAlpha = static_cast< sf::Uint8 >( 1 - alpha );
				m_line[ 0 ].color		= sf::Color( sf::Color{ 255, oneMinusAlpha, oneMinusAlpha, alpha } );
				m_line[ 1 ].color		= sf::Color( sf::Color{ 255, oneMinusAlpha, oneMinusAlpha, alpha } );
			}

			else
			{
				m_drawLine		= false;
				m_hideLine		= false;
				m_hideLineParam = 0.0f;
			}
		}
	}
}

void Player::checkIntersectionWithEnemies() const
{
	m_gameWorld->markIntersectedEnemies( *this );
}

void Player::initText()
{
	m_font = make_unique< sf::Font >();
	if( !m_font->loadFromFile( "C:\\Users\\Jonasz\\Documents\\Visual Studio "
							   "2019\\Projects\\SteeringBehaviors\\arial.ttf" ) )
	{
		std::cout << "Error loading font" << std::endl;
	}

	m_healthText = make_unique< sf::Text >();
	m_healthText->setFont( *m_font );

	m_healthText->setString( std::to_string( m_hpPoints ) );

	m_healthText->setCharacterSize( 24 );

	m_healthText->setFillColor( sf::Color::Red );

	m_healthText->setStyle( sf::Text::Bold );
}

void Player::handleCollisionWithEnemy()
{
	const vector< shared_ptr< Enemy > >& enemies = m_gameWorld->getEnemies();

	for( const auto& enemy : enemies )
	{
		Vector2 toEnemy	  = enemy->getPosition() - m_position;
		float totalRadius = enemy->getRadius() + m_radius;

		if( toEnemy.lengthSquared() < totalRadius * totalRadius )
		{
			m_healtCounter -= 0.1f;
			if( m_healtCounter < 0 )
			{
				m_healtCounter = 0.0f;
			}
		}
	}
}

void Player::updateHealth()
{
	m_hpPoints = static_cast< int >( m_healtCounter );
	m_healthText->setString( std::to_string( m_hpPoints ) );
}

void Player::shouldDie()
{
	if( m_hpPoints == 0 )
	{
		m_toDelete = true;
	}
}

} // namespace Graphics
} // namespace SteeringBehaviors
