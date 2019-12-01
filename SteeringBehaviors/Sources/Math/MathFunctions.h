#pragma once

#include "SFML/System/Vector2.hpp"
#include "Vector2.h"
#include <assert.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdlib>

namespace SteeringBehaviors
{
namespace Math
{

template< typename T >
T lerp( T v0, T v1, float param )
{
	return ( 1 - param ) * v0 + param * v1;
}

template<>
inline float lerp( float v0, float v1, float param )
{
	return ( 1.0f - param ) * v0 + param * v1;
}

inline float randFloat()
{
	return ( ( rand() ) / ( RAND_MAX + 1.0f ) );
}

inline float randInRange( float x, float y )
{
	return x + randFloat() * ( y - x );
}

inline bool randBool()
{
	if( randFloat() > 0.5 )
		return true;

	else
		return false;
}

inline float randomClamped()
{
	return randFloat() - randFloat();
}

inline bool isEqual( double a, double b )
{
	if( fabs( a - b ) < 1E-12 )
	{
		return true;
	}

	return false;
}

inline Vector2 normalize( const Vector2& v )
{
	float vector_length = v.length();
	Vector2 normalized{ v };
	if( vector_length > std::numeric_limits< float >::epsilon() )
	{

		normalized.x /= vector_length;
		normalized.y /= vector_length;
	}

	return normalized;
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

template< typename T = float >
sf::Vector2< T > toSFMLVector( const Vector2& v )
{
	return sf::Vector2< T >( v.x, v.y );
}

template< typename T >
Vector2 toMathVector( const sf::Vector2< T > v )
{
	return Vector2{ static_cast< float >( v.x ), static_cast< float >( v.y ) };
}

} // namespace Math
} // namespace SteeringBehaviors
