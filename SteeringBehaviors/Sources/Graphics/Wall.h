#pragma once
#include "GameEntity.h"

namespace sf
{
class Shape;
}

namespace SteeringBehaviors
{
namespace Graphics
{
class GameWorld;
class Wall : public GameEntity
{
public:
	enum class Orientation
	{
		HORIZONTAL,
		VERTICAL
	};

	enum class Side
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

public:
	Wall( GameWorld* gameWorld, float maxSpeed, Orientation orientation, Side side );
	~Wall();

	virtual void init() override;
	virtual void teardown() override;
	virtual void render( RenderWindow* window ) override;
	virtual void update() override;

	virtual void processInput() override;
	virtual void processEvents( sf::Event& event ) override;
	virtual void wrapScreenPosition() override;

private:
	virtual void initPhysicalPart() override;
	virtual void initGfxPart() override;
	void calculateWallSize();
	void calculateWallPosition();

private:
	Orientation m_orientation;
	Side m_side;

	inline static constexpr float width = 10.0f;
};
} // namespace Graphics

} // namespace SteeringBehaviors
