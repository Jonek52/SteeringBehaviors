#pragma once

namespace sf
{
class RenderWindow;
}
namespace SteeringBehaviors
{
using sf::RenderWindow;
namespace Graphics
{
class Renderable
{
public:
	virtual ~Renderable()				 = default;
	virtual void render( RenderWindow* ) = 0;
};

} // namespace Graphics
} // namespace SteeringBehaviors
