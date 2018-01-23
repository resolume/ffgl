#pragma once

#include <math.h>

#ifdef _OFX_VEC3f
typedef ofxVec3f Vec3;
#else

/// Stripped down vector class just for this, so we don't depend on ofxVectorMath
struct Vec3
{
	Vec3()
	{
	}
	
	Vec3( float x, float y=0, float z=0 )
	:
	x(x),y(y),z(z)
	{
	}
	
	Vec3( const Vec3 & v )
	:
	x(v.x),y(v.y),z(v.z)
	{
	}
	
	void normalize()
	{
		float invlen = 1.0 / sqrt( x*x + y*y + z*z );
		x*=invlen;
		y*=invlen;
		z*=invlen;
	}
	
	Vec3  operator - () const { return Vec3(-x,-y,-z); }
	
	
	Vec3& operator += ( const Vec3& v)	{ x+=v.x; y+=v.y; z+=v.z; return *this; }
	Vec3& operator -= ( const Vec3& v)	{ x-=v.x; y-=v.y; z-=v.z;return *this; }
	Vec3& operator *= ( float f)		{ x*=f; y*=f; z*=f;return *this;  }
	Vec3& operator *= ( const Vec3& v)	{ x*=v.x; y*=v.y; z*=v.z; return *this; }
	Vec3& operator /= ( float f)		{ x/=f; y/=f; z/=f;return *this; }
	
	Vec3 operator + ( const Vec3& v) const		{ return Vec3(x+v.x, y+v.y, z+v.z); }
	Vec3 operator - ( const Vec3& v ) const		{ return Vec3(x-v.x, y-v.y, z-v.z); }
	Vec3 operator * ( float f) const			{ return Vec3(x*f, y*f, z*f); }
	Vec3 operator * ( const Vec3& v) const		{ return Vec3(x*v.x, y*v.y, z*v.z); }
	Vec3 operator / ( float f) const			{ return Vec3(x/f, y/f, z/f); }
	
	float x;
	float y;
	float z;
};



#endif

