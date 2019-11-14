#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Dynamics\b2Fixture.h"
#include "SFML\Graphics\Vertex.hpp"
#include "Box2D\Collision\Shapes\b2CircleShape.h"

#include "GameWorld.h"
#include "Player.h"
#include "../Math/MathFunctions.h"

#include <iostream>

namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( GameWorld* gameWorld, float maxSpeed )
	: GameEntity{ gameWorld, maxSpeed, GameWorld::PLAYER, GameWorld::OBSTACLE }
{
	init();
}

Player::~Player() = default;

void Player::init()
{

	initGfxPart();
	initPhysicalPart();
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );

	m_ball.graphicalBody = std::make_unique< sf::CircleShape >( 10.0f, 12 );
	m_ball.graphicalBody->setFillColor( sf::Color::Green );
	m_ball.graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );

	b2BodyDef ballDef;
	ballDef.type = b2_dynamicBody;
	ballDef.position.Set( 400.f, 300.f );
	m_ball.physicalBody = m_gameWorld->getPhysicalWorld()->CreateBody( &ballDef );

	b2CircleShape physicalBody;
	physicalBody.m_radius = 10.0f;

	m_ball.physicalBody->CreateFixture( &physicalBody, 0.0f );
}

void Player::teardown() {}

void Player::update( std::chrono::milliseconds delta )
{

	applyForce();
	handleShooting( delta );
	// wrapScreenPosition();
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

	Vec forceApplied = m_targetDirection;
	forceApplied.Normalize();

	forceApplied *= m_ball.forceApplied;
	m_ball.physicalBody->ApplyLinearImpulseToCenter( forceApplied, true );
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
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );
	window->draw( *m_graphicalBody );

	m_ball.graphicalBody->setPosition( Math::toSFMLVector( m_ball.physicalBody->GetPosition() ) );
	window->draw( *m_ball.graphicalBody );
}

void Player::move( float deltaTime )
{
	m_physicalBody->SetTransform( m_physicalBody->GetPosition() + m_physicalBody->GetLinearVelocity(),
								  m_physicalBody->GetAngle() );
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );
}

void Player::applyForce()
{
	m_physicalBody->SetLinearVelocity( Vec{ 0.0f, 0.0f } );
	Vec force = Vec{ 0.0f, 0.0f };
	if( m_moveLeft )
		force += Vec{ -1.0f, 0.0f };

	if( m_moveRight )
		force += Vec{ 1.0f, 0.0f };

	if( m_moveLeft && m_moveRight )
		force += Vec{ 0.0f, 0.0f };

	if( m_moveUp )
		force += Vec{ 0.0f, 1.0f };

	if( m_moveDown )
		force += Vec{ 0.0f, -1.0f };

	if( m_moveUp && m_moveDown )
		force += Vec{ 0.0f, 0.0f };

	force.Normalize();
	force *= m_maxForceValue;

	m_physicalBody->ApplyLinearImpulseToCenter( force, true );
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
	m_mousePosition			   = Math::toBox2DVector( mousePosition );
}

void Player::rotate()
{
	m_targetDirection = m_mousePosition - m_physicalBody->GetPosition();
	m_targetDirection.Normalize();

	float cos				   = m_lookDirection.x * m_targetDirection.x + m_lookDirection.y * m_targetDirection.y;
	float sin				   = m_lookDirection.x * m_targetDirection.y - m_lookDirection.y * m_targetDirection.x;
	float rotationAngle		   = atan2( sin, cos );
	float rotationAngleDegrees = Math::toDegrees( rotationAngle );

	m_lookDirection		= m_targetDirection;
	m_sideDirection		= Vec{ -m_lookDirection.y, m_lookDirection.x };
	float absoluteAngle = atan2( m_sideDirection.y, m_sideDirection.x );

	m_physicalBody->SetTransform( m_physicalBody->GetWorldCenter(), absoluteAngle );
	m_graphicalBody->rotate( rotationAngleDegrees );
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

void Player::initPhysicalPart()
{

	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set( 400.0f, 300.0f );
	m_physicalBody = m_gameWorld->getPhysicalWorld()->CreateBody( &playerBodyDef );

	b2PolygonShape playerShape;
	Vec playerShapePoints[ 3 ];

	for( int i = 0; i < 3; ++i )
	{
		sf::Vector2f transformed = m_graphicalBody->getTransform().transformPoint( m_graphicalBody->getPoint( i ) );
		playerShapePoints[ i ]	 = Math::toBox2DVector( transformed );
	}

	playerShape.Set( playerShapePoints, 3 );

	b2FixtureDef fixture;
	fixture.filter.categoryBits = m_collisionCategory;
	fixture.filter.maskBits		= m_collisionMask;
	fixture.density				= 0.0f;
	fixture.shape				= &playerShape;

	m_physicalBody->CreateFixture( &fixture );
}

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

	m_physicalBody->SetTransform( Vec{ currentPosX, currentPosY }, m_physicalBody->GetAngle() );
}

} // namespace Graphics
} // namespace SteeringBehaviors
