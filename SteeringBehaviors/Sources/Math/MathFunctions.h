#pragma once

#include "SFML/System/Vector2.hpp"
#include <math.h>
#include <assert.h>

namespace SteeringBehaviors
{
namespace Math
{
template< typename T >
T vectorLenght( sf::Vector2< T > v )
{
	return sqrt( v.x * v.x + v.y * v.y );
}

template< typename T >
T vectorLenghtSquared( sf::Vector2< T > v )
{
	return v.x * v.x + v.y * v.y;
}

template< typename T >
sf::Vector2< T > normalizeVector( sf::Vector2< T > v )
{
	float lenght = vectorLenght( v );
	assert( lenght > 0.0f, "Can't divide by zero!" );
	return v / vectorLenght( v );
}
} // namespace Math
} // namespace SteeringBehaviors
