#pragma once

namespace sf
{
class Event;
}
namespace SteeringBehaviors
{
using sf::Event;
namespace Input
{
class InputHandling
{
public:
	virtual ~InputHandling()			= default;
	virtual void processInput( Event& ) = 0;
};
} // namespace Input
} // namespace SteeringBehaviors
