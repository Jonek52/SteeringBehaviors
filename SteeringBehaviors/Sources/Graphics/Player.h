#pragma once

#include "MovingEntity.h"

#include "SFML/Graphics/Vertex.hpp"

#include <array>

namespace sf
{
class Shape;
class CircleShape;
class Text;
class Font;
} // namespace sf

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class Obstacle;
class Player final : public MovingEntity
{
public:
	Player( GameWorld* gameWorld, float maxSpeed, const Math::Vector2& position );
	virtual ~Player();

	void init() override;
	void teardown() override;
	void render( RenderWindow* window ) override;
	void update( float delta ) override;

	void processInput() override;
	void processEvents( sf::Event& event ) override;

protected:
	void initGfxPart() override;
	void initText();
	void initPhysicalPart() override;

	void wrapScreenPosition() override;
	void handleKeyboard();
	void handleMouse();
	void handleShooting( float delta );
	void move( float delta );
	void rotate();
	Vector2 calculateForce();

private:
	void wait();
	void handleCollisions( const vector< shared_ptr< Obstacle > >& obstacles );
	void handleLineDrawing( float deltaTime );
	void checkIntersectionWithEnemies() const;
	void handleCollisionWithEnemy();
	void updateHealth();
	void shouldDie();

private:
	bool m_moveUp{ false };
	bool m_moveDown{ false };
	bool m_moveRight{ false };
	bool m_moveLeft{ false };

	bool canShoot{ true };
	bool wantsToShoot{ false };
	bool onCooldown{ false };

	bool m_drawLine{ false };

	bool m_createLine{ false };
	bool m_hideLine{ false };
	std::array< sf::Vertex, 2 > m_line;

	std::chrono::steady_clock::time_point m_shotCooldown;
	float m_createLineParam{ 0.0f };
	float m_hideLineParam{ 0.0f };

	constexpr static inline float s_createLineTime{ 0.2f };
	constexpr static inline float s_hideLineTime{ 0.2f };

	float m_healtCounter{ 100.0f };
	int m_hpPoints{ 100 };
	unique_ptr< sf::Text > m_healthText;
	unique_ptr< sf::Font > m_font;
};

} // namespace Graphics
} // namespace SteeringBehaviors
