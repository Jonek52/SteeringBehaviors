#include "Vector2.h"

namespace SteeringBehaviors::Math
{

Vector2::Vector2() : x{ 0.f }, y( 0.f ) {}

Vector2::Vector2( float x, float y ) : x{ x }, y{ y } {}

Vector2& Vector2::truncate( float val )
{
	float vectorLenght = this->length();

	if( vectorLenght > val )
	{
		this->normalize();
		*this *= val;
	}
	return *this;
}

float Vector2::dot( const Vector2& v ) const
{
	return x * v.x + y * v.y;
}

float Vector2::lengthSquared() const
{
	return x * x + y * y;
}

float Vector2::length() const
{
	return sqrtf( x * x + y * y );
}

Vector2& Vector2::normalize()
{
	float vectorLenght = this->length();

	if( vectorLenght > std::numeric_limits< float >::epsilon() )
	{
		x /= vectorLenght;
		y /= vectorLenght;
	}

	return *this;
}

Vector2 Vector2::perp() const
{
	return Vector2{ -y, x };
}

Vector2 Vector2::operator/=( float val )
{
	x /= val;
	y /= val;

	return *this;
}

Vector2& Vector2::operator*=( float val )
{
	x *= val;
	y *= val;

	return *this;
}

Vector2& Vector2::operator-=( const Vector2& v )
{
	x -= v.x;
	y -= v.y;

	return *this;
}

Vector2& Vector2::operator+=( const Vector2& v )
{
	x += v.x;
	y += v.y;

	return *this;
}

Vector2 Vector2::operator/( float value ) const
{
	float inverse = 1.f / value;
	return *this * inverse;
}

Vector2 Vector2::operator*( float value ) const
{
	return Vector2{ x * value, y * value };
}

Vector2 Vector2::operator-( const Vector2& v ) const
{
	return Vector2{ x - v.x, y - v.y };
}

Vector2 Vector2::operator+( const Vector2& v ) const
{
	return Vector2{ x + v.x, y + v.y };
}

void Vector2::zero ()
{
	*this = Vector2 {};
}

Vector2 normalize( const Vector2& v )
{
	Vector2 normalized = v;
	float vectorLenght = v.length();

	if( vectorLenght > std::numeric_limits< float >::epsilon() )
	{
		normalized /= vectorLenght;
	}

	return normalized;
}

float vecDistance( const Vector2& v0, const Vector2& v1 )
{
	Math::Vector2 distanceVector = v0 - v1;
	return distanceVector.length();
}

float vecDistanceSquared( const Vector2& v0, const Vector2& v1 )
{
	Math::Vector2 distanceVector = v0 - v1;
	return distanceVector.lengthSquared();
}

} // namespace SteeringBehaviors::Math
