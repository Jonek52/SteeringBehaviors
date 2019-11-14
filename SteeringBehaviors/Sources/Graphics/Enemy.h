#pragma once

#include "GameEntity.h"

namespace sf
{
class Shape;
}
namespace SteeringBehaviors
{
class GameWorld;
namespace Graphics
{
class Enemy : public GameEntity
{
public:
	Enemy( GameWorld* gameWorld, float maxSpeed );
	virtual ~Enemy();

	virtual void init() override;
	virtual void teardown() override;
	virtual void update() override;
	virtual void render( RenderWindow* ) override;
	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;

protected:
	virtual void initGfxPart() override;

	virtual void initPhysicalPart() override;

	virtual void wrapScreenPosition() override;
};
} // namespace Graphics
} // namespace SteeringBehaviors
