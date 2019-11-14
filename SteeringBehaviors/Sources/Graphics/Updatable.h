#pragma once

#include <chrono>

namespace SteeringBehaviors
{
namespace Graphics
{
class Updatable
{
public:
	virtual ~Updatable()  = default;
	virtual void update(std::chrono::milliseconds delta) = 0;
};

} // namespace Graphics
} // namespace SteeringBehaviors
