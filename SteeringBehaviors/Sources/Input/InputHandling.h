#pragma once

namespace sf
{
class Window;
} // namespace sf
namespace SteeringBehaviors
{
namespace Input
{
class InputHandling
{
public:
	virtual ~InputHandling()				 = default;
	virtual void processInput( sf::Window* ) = 0;
};
} // namespace Input
} // namespace SteeringBehaviors
