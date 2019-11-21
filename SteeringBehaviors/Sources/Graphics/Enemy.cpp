#include <SFML\Graphics\ConvexShape.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System\Vector2.hpp>

#include "Box2D\Dynamics\b2Body.h"
#include "Box2D\Collision\Shapes\b2PolygonShape.h"
#include "Box2D\Dynamics\b2World.h"
#include "Box2D\Dynamics\b2Fixture.h"

#include "Enemy.h"
#include "GameWorld.h"
#include "../Math/MathFunctions.h"

namespace SteeringBehaviors
{
namespace Graphics
{

Enemy::Enemy( GameWorld* gameWorld, float maxSpeed )
	: GameEntity{ gameWorld, maxSpeed, GameWorld::ENEMY, GameWorld::OBSTACLE | GameWorld::PLAYER | GameWorld::BALL }
{
	init();
}

Enemy::~Enemy() = default;

void Enemy::init()
{
	initGfxPart();
	initPhysicalPart();
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );
}

void Enemy::teardown() {}

void Enemy::update( std::chrono::milliseconds delta ) {}

void Enemy::render( RenderWindow* window )
{
	m_graphicalBody->setPosition( Math::toSFMLVector( m_physicalBody->GetPosition() ) );
	window->draw( *m_graphicalBody );
}

void Enemy::processInput() {}

void Enemy::processEvents( sf::Event& event ) {}

void Enemy::initGfxPart()
{
	m_graphicalBody = std::make_unique< sf::ConvexShape >();
	m_graphicalBody->setFillColor( sf::Color::Red );
	auto* enemyGfx = dynamic_cast< sf::ConvexShape* >( m_graphicalBody.get() );

	enemyGfx->setPointCount( 3 );

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

	enemyGfx->setPoint( 0, points[ 0 ] - center );
	enemyGfx->setPoint( 1, points[ 1 ] - center );
	enemyGfx->setPoint( 2, points[ 2 ] - center );

	m_graphicalBody->setScale( { 0.5f, 0.5f } );
}

void Enemy::initPhysicalPart()
{
	b2BodyDef enemyBodyDef;
	enemyBodyDef.type = b2_staticBody;
	enemyBodyDef.position.Set( 600.f, 600.0f );
	m_physicalBody = m_gameWorld->getPhysicalWorld()->CreateBody( &enemyBodyDef );

	b2PolygonShape enemyShape;
	Vec enemyShapePoints[ 3 ];

	for( int i = 0; i < 3; ++i )
	{
		sf::Vector2f transformed = m_graphicalBody->getTransform().transformPoint( m_graphicalBody->getPoint( i ) );
		enemyShapePoints[ i ]	 = Math::toBox2DVector( transformed );
	}

	enemyShape.Set( enemyShapePoints, 3 );

	b2FixtureDef fixture;
	fixture.filter.categoryBits = m_collisionCategory;
	fixture.filter.maskBits		= m_collisionMask;
	fixture.density				= 0.0f;
	fixture.shape				= &enemyShape;

	m_physicalBody->CreateFixture( &fixture );
	m_physicalBody->SetUserData( ( void* )GameWorld::CollisionCategory::ENEMY );
}

void Enemy::wrapScreenPosition() {}
} // namespace Graphics
} // namespace SteeringBehaviors
