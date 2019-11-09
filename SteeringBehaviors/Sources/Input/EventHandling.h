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
class EventHandling
{
public:
	virtual ~EventHandling()			= default;
	virtual void processEvents( Event& ) = 0;
};
} // namespace Input
} // namespace SteeringBehaviors
