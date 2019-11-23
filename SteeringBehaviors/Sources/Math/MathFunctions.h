#pragma once

#include "SFML/System/Vector2.hpp"

#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>

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
	assert( lenght > 0.0f );
	return v / vectorLenght( v );
}

inline float toDegrees( float radians )
{
	float inversePI = 1.0f / static_cast< float >( M_PI );
	return radians * ( 180.0f * inversePI );
}

inline float toRadians( float degrees )
{
	float inverse180 = 1.0f / 180.0f;
	return degrees * ( static_cast< float >( M_PI ) * inverse180 );
}

} // namespace Math
} // namespace SteeringBehaviors
