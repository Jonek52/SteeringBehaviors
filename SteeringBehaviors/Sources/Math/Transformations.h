#pragma once

#include <vector>

#include "Vector2.h"
#include "Matrix.h"

namespace SteeringBehaviors::Math
{

inline Vector2 PointToWorldSpace( const Vector2& point,
								  const Vector2& AgentHeading,
								  const Vector2& AgentSide,
								  const Vector2& AgentPosition )
{
	Vector2 TransPoint = point;
	C2DMatrix matTransform;

	matTransform.Rotate( AgentHeading, AgentSide );
	matTransform.Translate( AgentPosition.x, AgentPosition.y );
	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

inline Vector2 VectorToWorldSpace( const Vector2& vec, const Vector2& AgentHeading, const Vector2& AgentSide )
{
	Vector2 TransVec = vec;
	C2DMatrix matTransform;

	matTransform.Rotate( AgentHeading, AgentSide );
	matTransform.TransformVector2s( TransVec );

	return TransVec;
}

inline Vector2 PointToLocalSpace( const Vector2& point,
								  const Vector2& AgentHeading,
								  const Vector2& AgentSide,
								  const Vector2& AgentPosition )
{

	Vector2 TransPoint = point;

	C2DMatrix matTransform;

	float Tx = -AgentPosition.dot( AgentHeading );
	float Ty = -AgentPosition.dot( AgentSide );

	matTransform._11( AgentHeading.x );
	matTransform._12( AgentSide.x );
	matTransform._21( AgentHeading.y );
	matTransform._22( AgentSide.y );
	matTransform._31( Tx );
	matTransform._32( Ty );

	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

inline Vector2 VectorToLocalSpace( const Vector2& vec, const Vector2& AgentHeading, const Vector2& AgentSide )
{

	Vector2 TransPoint = vec;

	C2DMatrix matTransform;

	matTransform._11( AgentHeading.x );
	matTransform._12( AgentSide.x );
	matTransform._21( AgentHeading.y );
	matTransform._22( AgentSide.y );

	matTransform.TransformVector2s( TransPoint );

	return TransPoint;
}

inline void Vec2DRotateAroundOrigin( Vector2& v, float ang )
{
	C2DMatrix mat;
	mat.Rotate( ang );
	mat.TransformVector2s( v );
}

} // namespace SteeringBehaviors::Math
