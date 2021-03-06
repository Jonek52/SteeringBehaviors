#pragma once

#include <math.h>
#include <limits>

namespace SteeringBehaviors::Math
{
inline bool isEqual( float a, float b )
{
	if( fabs( a - b ) < 1E-12 )
	{
		return true;
	}

	return false;
}

struct Vector2
{
	float x;
	float y;

	Vector2() : x{ 0.0f }, y{ 0.0f } {}
	Vector2( float a, float b ) : x{ a }, y{ b } {}

	void zero()
	{
		x = 0.0;
		y = 0.0;
	}

	inline float length() const;

	inline float lengthSquared() const;

	inline void normalize();

	inline float dot( const Vector2& v2 ) const;

	inline Vector2 perp() const;

	inline void truncate( float max );

	inline float distance( const Vector2& v2 ) const;

	inline float distanceSquared( const Vector2& v2 ) const;

	const Vector2& operator+=( const Vector2& rhs )
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2& operator-=( const Vector2& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2& operator*=( const float& rhs )
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}

	const Vector2& operator/=( const float& rhs )
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==( const Vector2& rhs ) const
	{
		return ( isEqual( x, rhs.x ) && isEqual( y, rhs.y ) );
	}

	bool operator!=( const Vector2& rhs ) const
	{
		return ( x != rhs.x ) || ( y != rhs.y );
	}
};

inline Vector2 operator*( const Vector2& lhs, float rhs );
inline Vector2 operator*( float lhs, const Vector2& rhs );
inline Vector2 operator-( const Vector2& lhs, const Vector2& rhs );
inline Vector2 operator+( const Vector2& lhs, const Vector2& rhs );
inline Vector2 operator/( const Vector2& lhs, float val );

inline float Vector2::length() const
{
	return sqrtf( x * x + y * y );
}

inline float Vector2::lengthSquared() const
{
	return ( x * x + y * y );
}

inline float Vector2::dot( const Vector2& v2 ) const
{
	return x * v2.x + y * v2.y;
}

inline Vector2 Vector2::perp() const
{
	return Vector2( -y, x );
}

inline float Vector2::distance( const Vector2& v2 ) const
{
	float ySeparation = v2.y - y;
	float xSeparation = v2.x - x;

	return sqrtf( ySeparation * ySeparation + xSeparation * xSeparation );
}

inline float Vector2::distanceSquared( const Vector2& v2 ) const
{
	float ySeparation = v2.y - y;
	float xSeparation = v2.x - x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline void Vector2::truncate( float max )
{
	if( this->length() > max )
	{
		this->normalize();

		*this *= max;
	}
}

inline void Vector2::normalize()
{
	float vector_length = this->length();

	if( vector_length > std::numeric_limits< float >::epsilon() )
	{
		this->x /= vector_length;
		this->y /= vector_length;
	}
}

inline Vector2 normalize( const Vector2& v )
{
	Vector2 vec = v;

	float vector_length = vec.length();

	if( vector_length > std::numeric_limits< float >::epsilon() )
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}

inline float distance( const Vector2& v1, const Vector2& v2 )
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return sqrtf( ySeparation * ySeparation + xSeparation * xSeparation );
}

inline float distanceSquared( const Vector2& v1, const Vector2& v2 )
{

	float ySeparation = v2.y - v1.y;
	float xSeparation = v2.x - v1.x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline float Vec2DLength( const Vector2& v )
{
	return sqrtf( v.x * v.x + v.y * v.y );
}

inline float Vec2DLengthSq( const Vector2& v )
{
	return ( v.x * v.x + v.y * v.y );
}

//------------------------------------------------------------------------operator overloads
inline Vector2 operator*( const Vector2& lhs, float rhs )
{
	Vector2 result( lhs );
	result *= rhs;
	return result;
}

inline Vector2 operator*( float lhs, const Vector2& rhs )
{
	Vector2 result( rhs );
	result *= lhs;
	return result;
}

// overload the - operator
inline Vector2 operator-( const Vector2& lhs, const Vector2& rhs )
{
	Vector2 result( lhs );
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

// overload the + operator
inline Vector2 operator+( const Vector2& lhs, const Vector2& rhs )
{
	Vector2 result( lhs );
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

// overload the / operator
inline Vector2 operator/( const Vector2& lhs, float val )
{
	Vector2 result( lhs );
	result.x /= val;
	result.y /= val;

	return result;
}

///////////////////////////////////////////////////////////////////////////////

// treats a window as a toroid
inline void WrapAround( Vector2& pos, int MaxX, int MaxY )
{
	if( pos.x > MaxX )
	{
		pos.x = 0.0;
	}

	if( pos.x < 0 )
	{
		pos.x = ( float )MaxX;
	}

	if( pos.y < 0 )
	{
		pos.y = ( float )MaxY;
	}

	if( pos.y > MaxY )
	{
		pos.y = 0.0;
	}
}

} // namespace SteeringBehaviors::Math
