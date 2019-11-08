#pragma once

#include "GameEntity.h"

#include <memory>

namespace sf
{
class CircleShape;
}
namespace SteeringBehaviors
{
namespace Graphics
{
using sf::CircleShape;
class Player : public GameEntity
{
public:
	Player ();
	virtual ~Player();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update( float deltaTime ) override;
	virtual void processInput( Event& event ) override;

private:
	std::unique_ptr< CircleShape > m_playerShape;
	bool m_isDirty;
};

} // namespace Graphics
} // namespace SteeringBehaviors
