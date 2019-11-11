#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Dynamics\b2World.h"

#include "GameWorld.h"
#include "Player.h"
#include "../Math/MathFunctions.h"
#include <iostream>
namespace SteeringBehaviors
{
namespace Graphics
{
Player::Player( GameWorld* gameWorld, float maxSpeed, const Vec& position ) : GameEntity( gameWorld, maxSpeed )
{
	b2BodyDef playerBodyDef;
	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set( 400.0f, 300.0f );
	m_physicalBody = m_gameWorld->getPhysicalWorld()->CreateBody( &playerBodyDef );
}

Player::~Player() = default;

void Player::init()
{
	m_graphicalBody = std::make_unique< sf::ConvexShape >();
	m_graphicalBody->setFillColor( sf::Color::Green );
	sf::ConvexShape* playerGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	playerGfx->setPointCount( 3 );

	std::vector< sf::Vector2f > points;
	points.push_back( sf::Vector2f{ 0.0f, -100.0f } );
	points.push_back( sf::Vector2f{ -50.0f, 0.0f } );
	points.push_back( sf::Vector2f{ 50.0f, 0.0f } );

	float playerCenterX{ 0.0f };
	float playerCenterY{ 0.0f };

	for( int i = 0; i < points.size(); ++i )
	{
		playerCenterX += points[ i ].x;
		playerCenterY += points[ i ].y;
	}

	playerCenterX /= 3.0f;
	playerCenterY /= 3.0f;

	playerGfx->setPoint( 0, sf::Vector2f{ 0.0f, -100.0f } - sf::Vector2f{ playerCenterX, playerCenterY } );
	playerGfx->setPoint( 1, sf::Vector2f{ -50.0f, 0.0f } - sf::Vector2f{ playerCenterX, playerCenterY } );
	playerGfx->setPoint( 2, sf::Vector2f{ 50.0f, 0.0f } - sf::Vector2f{ playerCenterX, playerCenterY } );

	m_graphicalBody->setOrigin( sf::Vector2f{ playerCenterX, playerCenterX } );
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );

	b2PolygonShape playerShape;
	Vec playerShapePoints[ 3 ];

	for( int i = 0; i < 3; ++i )
	{
		playerShapePoints[ i ] = Math::toBox2DVector( m_graphicalBody->getPoint( i ) );
	}

	playerShape.Set( playerShapePoints, 3 );
	m_physicalBody->CreateFixture( &playerShape, 0.0f );
}

void Player::teardown() {}

void Player::render( RenderWindow* window )
{
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );
	window->draw( *m_graphicalBody );
}

void Player::update()
{

	applyForce();
	// wrapScreenPosition();
	rotate();
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

void Player::processEvents( sf::Event& event ) {}

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
	Vec b2mousePosition		   = Math::toBox2DVector( mousePosition );
	m_targetDirection		   = b2mousePosition - m_physicalBody->GetPosition();
	m_targetDirection.Normalize();
}

void Player::rotate()
{
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
