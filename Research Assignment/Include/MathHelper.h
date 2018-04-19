//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//             Maths Helper
//             Version: 1.0
//             Author: Christopher Diamond
//             Date: March 20, 2013    (last updated September 10, 2013)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Description:
//
//    Math helpers including 2D/4D vectors and 4D matrix
//
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//========================================================================
#ifndef __MATHHELPER_H_
#define __MATHHELPER_H_
//========================================================================
#include <math.h>
 






// Disable warning for unnamed unions/structs
#pragma warning( disable : 4201 )

namespace Maths
{

const float PI					= 3.14159265359f;
const float	TWOPI     			= 6.28318530717958657692f;
const float HALFPI				= 1.57079632679489661923f;
const float THREEHALFPI			= 4.71238898038468985769f;

const float	EPSILON				= 0.00000000001f;
const float	DEG2RAD             = 0.01745329251994329577f;
const float	RAD2DEG             = 57.2957795130823208768f;

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Reciprocal function
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
inline float Recipf( float x )
{ 
	return (1.0f/x); 
}

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Min & Max functions
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
inline int  Max( int x, int y ){ return (x > y) ? x : y; }
inline int  Min( int x, int y ){ return (x < y) ? x : y; }
inline float  Maxf( float x, float y ){ return (x > y) ? x : y; }
inline float  Minf( float x, float y ){ return (x < y) ? x : y; }

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Clamp function (between 0.0f and 1.0f)
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
inline float  Clampf( float x )
{ 
	return (x > 1.0f) ? 1.0f : ((x < 0.0f) ? 0.0f : x ); 
}

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Clamp function (between MIN and MAX)
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
inline float  Clampf( float x, float minx, float maxx )
{ 
	return (x > maxx) ? maxx : ((x < minx) ? minx : x ); 
}
inline int  Clamp( int x, int minx, int maxx )
{ 
	return (x > maxx) ? maxx : ((x < minx) ? minx : x ); 
}

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Linearly interpolate a float
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
inline float  Lerp( float fA, float fB, float fT ) 
{ 
	return ( fB - fA ) * fT + fA;  
}

inline float  SmoothStep( float fValue ) 
{ 
	return ( fValue * fValue * ( 3.0f - 2.0f * fValue ) ); 
}

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// 2D vector class
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
class Vector2
{
public:

	float x, y;

	Vector2() {}
	Vector2(float a, float b) : x(a), y(b) {}
	~Vector2() {}

	void Normalise()
	{
		float fMag = sqrtf(x*x + y*y);
		x /= fMag;
		y /= fMag;
	}

	Vector2 Normalised()
	{
		Vector2 a = *this;
		a.Normalise();
		return a;
	}

	float Magnitude() const
	{
		return sqrtf(x*x + y*y);
	}

	float MagnitudeSqr() const
	{
		return x * x + y * y;
	}

	void Invert()
	{
		x *= -1;
		y *= -1;
	}

	float Dot(const Vector2& v) const
	{
		return x * v.x + y * v.y;
	}

	Vector2 operator + (const Vector2& v) const
	{
		return Vector2(x + v.x, y + v.y);
	}

	Vector2 operator - (const Vector2& v) const
	{
		return Vector2(x - v.x, y - v.y);
	}

	Vector2 operator * (float f) const
	{
		return Vector2(x * f, y * f);
	}

	Vector2 operator + (float f) const
	{
		return Vector2(x + f, y + f);
	}

	Vector2& operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2& operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2& operator *= (float f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	operator float* ()
	{
		return &x;
	}

	Vector2 operator - ()
	{
		return Vector2( -this->x, -this->y );
	}

	Vector2& operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
};
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// 4D vector
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
class Vector4
{
public:

	float x, y, z, w;

	Vector4() {}
	Vector4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
	~Vector4() {}

	Vector4 Normalise()
	{
		float fMag = Magnitude();
		fMag = (fMag != 0.f) ? fMag : 0.001f;
		x /= fMag;
		y /= fMag;
		z /= fMag;
		w = 0;
		return *this;
	}

	float Magnitude() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	float MagnitudeSqr() const
	{
		return x * x + y * y + z * z;
	}

	void Invert()
	{
		x *= -1;
		y *= -1;
		z *= -1;
	}

	float Dot(const Vector4& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	static float Dot(const Vector4& a, const Vector4& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	float Distance( const Vector4& v )
	{
		return sqrt(	pow(v.x - x, 2) + 
						pow(v.y - y, 2) +
						pow(v.z - z, 2) );
	}

	bool SameSideTriVector( const Vector4& a, const Vector4& b, const Vector4& c )
	{
		Vector4 CrossOne = Cross(c - b, *this - b);
		Vector4 CrossTwo = Cross(c - b, a - b);
		return Dot(CrossOne, CrossTwo) >= 0.f;
	}

	Vector4 Cross(const Vector4& v) const
	{
		return Vector4(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y, 0);
	}

	static Vector4 Cross( const Vector4& a, const Vector4& b )
	{
		return Vector4(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y, 0);
	}

	Vector2 ToVector2()
	{
		return Vector2(this->x, this->y);
	}

	Vector4 operator + (const Vector4& v) const
	{
		return Vector4(x + v.x, y + v.y, z + v.z, w);
	}

	Vector4& operator += (const Vector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector4& operator -= (const Vector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector4 operator - (const Vector4& v) const
	{
		return Vector4(x - v.x, y - v.y, z - v.z, w);
	}

	Vector4 operator * (float f) const
	{
		return Vector4(x * f, y * f, z * f, w);
	}

	Vector4 operator / (float f) const
	{
		return Vector4(x / f, y / f, z / f, w);
	}

	Vector4& operator /= (float f)
	{
		x /= f;
		y /= f; 
		z /= f;
		return *this;
	}

	Vector4& operator *= (float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vector4& operator = (const Vector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	operator float* ()
	{
		return &x;
	}

	operator Vector2 ()
	{
		return ToVector2();
	}

	Vector4 operator - ()
	{
		return Vector4(-this->x, -this->y, -this->z, -this->w);
	}

	bool operator != (const Vector4& v)
	{
		return (x != v.x || y != v.y || z != v.z || w != v.w);
	}

	bool operator == (const Vector4& v)
	{
		return (x == v.x && y == v.y && z == v.z && w == v.w);
	}
};


inline float Dot(const Vector2& v0, const Vector2& v1)
{
	return v0.Dot(v1);
}

inline float Dot(const Vector4& v0, const Vector4& v1)
{
	return v0.Dot(v1);
}

inline Vector4 Cross(const Vector4& v0, const Vector4& v1)
{
	return v0.Cross(v1);
}

inline float DistSqr(const Vector2& v0, const Vector2& v1)
{
	return (v0 - v1).MagnitudeSqr();
}

inline bool EqualWithinTolerance(const Vector2& v0, const Vector2& v1, float tolerance = 0.0001f)
{
	return ((v0 - v1).MagnitudeSqr() < (tolerance * tolerance));
}

inline float DistSqr(const Vector4& v0, const Vector4& v1)
{
	return (v0 - v1).MagnitudeSqr();
}

inline bool EqualWithinTolerance(const Vector4& v0, const Vector4& v1, float tolerance = 0.0001f)
{
	return ((v0 - v1).MagnitudeSqr() < (tolerance * tolerance));
}

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// 4D Matrix class
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
class Matrix4
{
public:
	// Disabling warning 4201 for this purpose
	union
	{
		float mm[4][4];
		float m[16];
		struct  
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		struct  
		{
			Vector4 row0, row1, row2, row3;
		};
	};

	Matrix4() 
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	}
	Matrix4(float a, float b, float c, float d,
		float e, float f, float g, float h,
		float i, float j, float k, float l,
		float m, float n, float o, float p)
		: _11(a), _12(b), _13(c), _14(d),
		_21(e), _22(f), _23(g), _24(h),
		_31(i), _32(j), _33(k), _34(l),
		_41(m), _42(n), _43(o), _44(p)
	{

	}
	~Matrix4() {}
	
	void SetFrame(const Vector4& position, const Vector4& forward, const Vector4& up)
	{
		Vector4 vForward = forward;
		vForward.Normalise();

		Vector4 vRight = up.Cross(vForward);
		vRight.Normalise();

		Vector4 vUp = vForward.Cross(vRight);
		vUp.Normalise();
		
		row0 = vRight;
		row1 = vUp;
		row2 = vForward;
		row3 = position;
	}

	Matrix4 ToViewMatrix()
	{
		return Matrix4(	row0.x,				row1.x,				row2.x,				0.0f,
						row0.y,				row1.y,				row2.y,				0.0f,
						row0.z,				row1.z,				row2.z,				0.0f,
						-(row0.Dot(row3)),  -(row1.Dot(row3)),  -(row2.Dot(row3)),	1.0f );

		Matrix4 view;
		
		view._11 = row0.x; 
		view._21 = row0.y; 
		view._31 = row0.z; 
		view._41 = -(row0.Dot(row3));   

		view._12 = row1.x;
		view._22 = row1.y;
		view._32 = row1.z;
		view._42 = -(row1.Dot(row3));  

		view._13 = row2.x; 
		view._23 = row2.y; 
		view._33 = row2.z; 
		view._43 = -(row2.Dot(row3));   

		view._14 = 0.0f;
		view._24 = 0.0f;
		view._34 = 0.0f;
		view._44 = 1.0f;

		return view;
	}
	
	void ViewLookAt(const Vector4& eye, const Vector4& to, const Vector4& up)
	{
		Vector4 vForward = to - eye;
		vForward.Normalise();

		Vector4 vLeft = up.Cross(vForward);
		vLeft.Normalise();

		Vector4 vUp = vForward.Cross(vLeft);
		
		_11 = vLeft.x; 
		_21 = vLeft.y; 
		_31 = vLeft.z; 
		_41 = -(vLeft.Dot(eye));   

		_12 = vUp.x;
		_22 = vUp.y;
		_32 = vUp.z;
		_42 = -(vUp.Dot(eye));  

		_13 = vForward.x; 
		_23 = vForward.y; 
		_33 = vForward.z; 
		_43 = -(vForward.Dot(eye));   

		_14 = 0.0f;
		_24 = 0.0f;
		_34 = 0.0f;
		_44 = 1.0f;
	}
	
	void Orthographic(float a_fLeft, float a_fRight, float a_fTop, float a_fBottom, float a_fNear, float a_fFar)
	{
		float       deltaX = a_fRight - a_fLeft;
		float       deltaY = a_fTop - a_fBottom;
		float       deltaZ = a_fFar - a_fNear;

		_11 = 2.f / deltaX;
		_12 = 0.f;
		_13 = 0.f;
		_14 = 0.f;

		_21 = 0.f;
		_22 = 2.f / deltaY;
		_23 = 0.f;
		_24 = 0.f;

		_31 = 0.f;
		_32 = 0.f;
		_33 = -2.f / deltaZ;
		_34 = 0.f;

		_41 = -((a_fRight + a_fLeft)/deltaX);
		_42 = -((a_fTop + a_fBottom)/deltaY);
		_43 = -((a_fFar + a_fNear)/deltaZ);
		_44 = 1.f;
	}

	void Orthographic(float a_fWidth, float a_fHeight, float a_fNear, float a_fFar)
	{
		_11 = 2 / a_fWidth;
		_12 = 0;
		_13 = 0;
		_14 = 0;

		_21 = 0;
		_22 = 2 / a_fHeight;
		_23 = 0;
		_24 = 0;

		_31 = 0;
		_32 = 0;
		_33 = 1 / (a_fFar - a_fNear);
		_34 = 0;

		_41 = 0;
		_42 = 0;
		_43 = a_fNear / (a_fNear - a_fFar);
		_44 = 1;
	}

	void Perspective(float a_fUpFOV, float a_fAspectRatio, float a_fNear, float a_fFar)
	{
		float halfAngleRadians = 0.5f * a_fUpFOV;
		float cot = 1 / tanf(halfAngleRadians);

		_11 = cot / a_fAspectRatio;
		_12 = 0;
		_13 = 0;
		_14 = 0;

		_21 = 0;
		_22 = cot;
		_23 = 0;
		_24 = 0;

		_31 = 0;
		_32 = 0;
		_33 = a_fFar / (a_fFar - a_fNear);
		_34 = 1;

		_41 = 0;
		_42 = 0;
		_43 = a_fNear * a_fFar / (a_fNear - a_fFar);
		_44 = 0;
	}

	Matrix4 Transpose() const
	{
		return Matrix4(_11, _21, _31, _41,
					_12, _22, _32, _42,
					_13, _23, _33, _43,
					_14, _24, _34, _44);
	}

	void RotateX(float rad)
	{
		_11 = 1;	_12 = 0;			_13 = 0;			_14 = 0;
		_21 = 0;	_22 = cosf(rad);	_23 = -sinf(rad);	_24 = 0;
		_31 = 0;	_32 = sinf(rad);	_33 = cosf(rad);	_34 = 0;
	}
	void RotateY(float rad)
	{
		_11 = cosf(rad);	_12 = 0;	_13 = -sinf(rad);	_14 = 0;
		_21 = 0;			_22 = 1;	_23 = 0;			_24 = 0;
		_31 = sinf(rad);	_32 = 0;	_33 = cosf(rad);	_34 = 0;
	}
	void RotateZ(float rad)
	{
		_11 = cosf(rad);	_12 = -sinf(rad);	_13 = 0;	_14 = 0;
		_21 = sinf(rad);	_22 = cosf(rad);	_23 = 0;	_24 = 0;
		_31 = 0;			_32 = 0;			_33 = 1;	_34 = 0;
	}
	void RotateAxis(float rad, const Vector4& axis)
	{
		float fSin = sinf(rad), fCos = cosf(rad);

		float a = 1.0f - fCos;
		float ax = a * axis.x;
		float ay = a * axis.y;
		float az = a * axis.z;

		_11 = ax * axis.x + fCos;
		_12 = ax * axis.y + axis.z * fSin;
		_13 = ax * axis.z - axis.y * fSin;
		_14 = 0;

		_21 = ay * axis.x - axis.z * fSin;
		_22 = ay * axis.y + fCos;
		_23 = ay * axis.z + axis.x * fSin;
		_24 = 0;

		_31 = az * axis.x + axis.y * fSin;
		_32 = az * axis.y - axis.x * fSin;
		_33 = az * axis.z + fCos;
		_34 = 0;
	}
	void RotationEuler(float bank, float heading, float attitude)
	{
		Matrix4 x, y, z;
		x.RotateX(bank);
		y.RotateY(heading);
		z.RotateZ(attitude);

		*this = x * y * z;
	}

	void Scale( const Vector2& v2Scale )
	{
		_11 = v2Scale.x;	_12 =  0.0f;		_13 =  0.0f;		_14 = 0.0f;
		_21 =  0.0f;		_22 =  v2Scale.y;	_23 =  0.0f;		_24 = 0.0f;
		_31 =  0.0f;		_32 =  0.0f;		_33 =  1.0f;		_34 = 0.0f;
		_41 =  0.0f;		_42 =  0.0f;		_43 =  0.0f;		_44 = 1.0f;
	}

	void Scale( const Vector4& v4Scale )
	{
		_11 = v4Scale.x;	_12 =  0.0f;		_13 =  0.0f;		_14 = 0.0f;
		_21 =  0.0f;		_22 =  v4Scale.y;	_23 =  0.0f;		_24 = 0.0f;
		_31 =  0.0f;		_32 =  0.0f;		_33 =  v4Scale.z;	_34 = 0.0f;
		_41 =  0.0f;		_42 =  0.0f;		_43 =  0.0f;		_44 = 1.0f;
	}

	Matrix4 operator * (const Matrix4& a_rRHS) const
	{
		return Matrix4(	
			mm[0][0] * a_rRHS.mm[0][0] + mm[0][1] * a_rRHS.mm[1][0] + mm[0][2] * a_rRHS.mm[2][0] + mm[0][3] * a_rRHS.mm[3][0],
			mm[0][0] * a_rRHS.mm[0][1] + mm[0][1] * a_rRHS.mm[1][1] + mm[0][2] * a_rRHS.mm[2][1] + mm[0][3] * a_rRHS.mm[3][1],
			mm[0][0] * a_rRHS.mm[0][2] + mm[0][1] * a_rRHS.mm[1][2] + mm[0][2] * a_rRHS.mm[2][2] + mm[0][3] * a_rRHS.mm[3][2],
			mm[0][0] * a_rRHS.mm[0][3] + mm[0][1] * a_rRHS.mm[1][3] + mm[0][2] * a_rRHS.mm[2][3] + mm[0][3] * a_rRHS.mm[3][3],

			mm[1][0] * a_rRHS.mm[0][0] + mm[1][1] * a_rRHS.mm[1][0] + mm[1][2] * a_rRHS.mm[2][0] + mm[1][3] * a_rRHS.mm[3][0],
			mm[1][0] * a_rRHS.mm[0][1] + mm[1][1] * a_rRHS.mm[1][1] + mm[1][2] * a_rRHS.mm[2][1] + mm[1][3] * a_rRHS.mm[3][1],
			mm[1][0] * a_rRHS.mm[0][2] + mm[1][1] * a_rRHS.mm[1][2] + mm[1][2] * a_rRHS.mm[2][2] + mm[1][3] * a_rRHS.mm[3][2],
			mm[1][0] * a_rRHS.mm[0][3] + mm[1][1] * a_rRHS.mm[1][3] + mm[1][2] * a_rRHS.mm[2][3] + mm[1][3] * a_rRHS.mm[3][3],

			mm[2][0] * a_rRHS.mm[0][0] + mm[2][1] * a_rRHS.mm[1][0] + mm[2][2] * a_rRHS.mm[2][0] + mm[2][3] * a_rRHS.mm[3][0],
			mm[2][0] * a_rRHS.mm[0][1] + mm[2][1] * a_rRHS.mm[1][1] + mm[2][2] * a_rRHS.mm[2][1] + mm[2][3] * a_rRHS.mm[3][1],
			mm[2][0] * a_rRHS.mm[0][2] + mm[2][1] * a_rRHS.mm[1][2] + mm[2][2] * a_rRHS.mm[2][2] + mm[2][3] * a_rRHS.mm[3][2],
			mm[2][0] * a_rRHS.mm[0][3] + mm[2][1] * a_rRHS.mm[1][3] + mm[2][2] * a_rRHS.mm[2][3] + mm[2][3] * a_rRHS.mm[3][3],

			mm[3][0] * a_rRHS.mm[0][0] + mm[3][1] * a_rRHS.mm[1][0] + mm[3][2] * a_rRHS.mm[2][0] + mm[3][3] * a_rRHS.mm[3][0],
			mm[3][0] * a_rRHS.mm[0][1] + mm[3][1] * a_rRHS.mm[1][1] + mm[3][2] * a_rRHS.mm[2][1] + mm[3][3] * a_rRHS.mm[3][1],
			mm[3][0] * a_rRHS.mm[0][2] + mm[3][1] * a_rRHS.mm[1][2] + mm[3][2] * a_rRHS.mm[2][2] + mm[3][3] * a_rRHS.mm[3][2],
			mm[3][0] * a_rRHS.mm[0][3] + mm[3][1] * a_rRHS.mm[1][3] + mm[3][2] * a_rRHS.mm[2][3] + mm[3][3] * a_rRHS.mm[3][3]);
	}

	Vector4 operator * (const Vector4& v) const
	{
		return Vector4(mm[0][0] * v.x + mm[0][1] * v.y + mm[0][2] * v.z + mm[0][3] * v.w,
					mm[1][0] * v.x + mm[1][1] * v.y + mm[1][2] * v.z + mm[1][3] * v.w,
					mm[2][0] * v.x + mm[2][1] * v.y + mm[2][2] * v.z + mm[2][3] * v.w,
					mm[3][0] * v.x + mm[3][1] * v.y + mm[3][2] * v.z + mm[3][3] * v.w);
	}

	operator float* ()
	{
		return &_11;
	}
};



//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// Quaternion class
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
class Quaternion
{
public:
	float x, y, z, w;

	Quaternion(): x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	Quaternion(float a_fX, float a_fY, float a_fZ, float a_fW): x(a_fX), y(a_fY), z(a_fZ), w(a_fW) {}
	Quaternion(const Vector4& a_axis): x(a_axis.x), y(a_axis.y), z(a_axis.z), w(a_axis.w) {};
	Quaternion(const Matrix4 a_mat) { FromMatrix(a_mat); }

	Quaternion& Normalize()
	{
		float fScale = x*x + y*y * z*z + w*w;
		float fRScale = 1.0f / sqrtf(fScale);
		return ((*this) *= fRScale);
	}

	Quaternion GetNormalized() const
	{
		float fScale = x*x + y*y * z*z + w*w;
		Quaternion qRet;
		qRet.x = x / fScale;
		qRet.y = y / fScale;
		qRet.z = z / fScale;
		qRet.w = w / fScale;
		return qRet;
	}

	Quaternion GetConjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

 	float Dot(const Quaternion& a_q1) const
 	{
 		return (x * a_q1.x) + (y * a_q1.y) + (z * a_q1.z) + (w * a_q1.w);
 	}
 
 	Quaternion& FromAxisAngle(float a_fRad, const Vector4& a_rAxis)
 	{
 		float fHalfAngle = 0.5f * a_fRad;
 		float fSin = sinf(fHalfAngle);
 		w = cosf(fHalfAngle);
 		x = fSin * a_rAxis.x;
 		y = fSin * a_rAxis.y;
 		z = fSin * a_rAxis.z;
 		return *this;
 	}

 	void ToAxisAngle(float& a_rfAngle, Vector4& a_rAxis) const
 	{	
		const float fScale = sqrtf(x*x + y*y + z*z); // Dot product of the imaginary part
		if((fScale > -0.000001 && fScale < 0.000001) || w > 1.0f || w < -1.0f)
		{
			a_rfAngle = 0.0f;
			a_rAxis.x = 0.0f;
			a_rAxis.y = 1.0f;
			a_rAxis.z = 0.0f;
			a_rAxis.w = 1.0f;
		}
		else
		{
				float fInvScale = Recipf(fScale);
				a_rfAngle = 2.0f * acosf(w);
				a_rAxis.x = x * fInvScale;
				a_rAxis.y = y * fInvScale;
				a_rAxis.z = z * fInvScale;
				a_rAxis.w = 1.0f;
		}
 	}

	void ToMatrix(Matrix4& a_rDest) const
	{
		a_rDest._11 = 1.0f - 2.0f*y*y - 2.0f*z*z;
		a_rDest._12 = 2.0f*x*y + 2.0f*z*w;
		a_rDest._13 = 2.0f*x*z - 2.0f*y*w;
		a_rDest._14 = 0.0f;

		a_rDest._21 = 2.0f*x*y - 2.0f*z*w;
		a_rDest._22 = 1.0f - 2.0f*x*x - 2.0f*z*z;
		a_rDest._23 = 2.0f*z*y + 2.0f*x*w;
		a_rDest._24 = 0.0f;

		a_rDest._31 = 2.0f*x*z + 2.0f*y*w;
		a_rDest._32 = 2.0f*z*y - 2.0f*x*w;
		a_rDest._33 = 1.0f - 2.0f*x*x - 2.0f*y*y;
		a_rDest._34 = 0.0f;

		// Clear the translate part for now
		a_rDest._41 = 0.0f;
		a_rDest._42 = 0.0f;
		a_rDest._43 = 0.0f;
		a_rDest._44 = 1.0f;
	}

	Quaternion& FromMatrix(const Matrix4 a_rMat)
	{
		float fDiag = a_rMat._11 + a_rMat._22 + a_rMat._33 + 1;
		if(fDiag > 0.0f)
		{
			float scale = sqrtf(fDiag) * 2.0f;
			float frScale = Recipf(scale);
			w = 0.25f * scale;
			x = (a_rMat._23 - a_rMat._32) * frScale;
			y = (a_rMat._31 - a_rMat._13) * frScale;
			z = (a_rMat._12 - a_rMat._21) * frScale;
		}
		else
		{
			if(a_rMat._11 > a_rMat._22 && a_rMat._11 > a_rMat._33)
			{
				// 1st element of diagonal is greatest value
				// Find scale according to 1st element, and double it
				float scale = (float)sqrt(1.0 + a_rMat._11 - a_rMat._22 - a_rMat._33) * 2.0f ;
				float frScale = Recipf(scale);
				x = 0.25f * scale;
				y = (a_rMat._21 + a_rMat._12) * frScale;
				z = (a_rMat._13 + a_rMat._31) * frScale;
				w = (a_rMat._23 - a_rMat._32) * frScale;
			}
			else if(a_rMat._22 > a_rMat._11 && a_rMat._22 > a_rMat._33)
			{
				float scale = (float)sqrt(1.0 + a_rMat._22 - a_rMat._11 - a_rMat._33) * 2.0f;
				float frScale = Recipf(scale);
				x = (a_rMat._21 + a_rMat._12) * frScale;
				y = 0.25f * scale;
				z = (a_rMat._32 + a_rMat._23) * frScale;
				w = (a_rMat._31 - a_rMat._13) * frScale;
			}
			else
			{
				float scale = (float)sqrt(1.0 + a_rMat._33 - a_rMat._11 - a_rMat._22) * 2.0f;
				float frScale = Recipf(scale);
				x = (a_rMat._31 + a_rMat._13) * frScale;
				y = (a_rMat._32 + a_rMat._23) * frScale;
				z = 0.25f * scale;
				w = (a_rMat._12 - a_rMat._21) * frScale;
			}
		}

		return (*this);
	}

	Quaternion& operator*=(const Quaternion& rhs)
	{
		return (*this = rhs * (*this));
	}

	Quaternion& operator*=(float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return (*this);
	}

	Quaternion operator*(const Quaternion& rhs) const
	{
		Quaternion qRet;

		qRet.w = (rhs.w * w) - (rhs.x * x) - (rhs.y * y) - (rhs.z * z);
		qRet.x = (rhs.x * w) + (rhs.w * x) + (rhs.z * y) - (rhs.y * z);
		qRet.y = (rhs.y * w) - (rhs.z * x) + (rhs.w * y) + (rhs.x * z);
		qRet.z = (rhs.z * w) + (rhs.y * x) - (rhs.x * y) + (rhs.w * z);
		
		return qRet;
	}

	Quaternion operator*(float rhs)
	{
		return Quaternion(rhs*x, rhs*y, rhs*z, rhs*w);
	}

	Vector4 operator*(const Vector4& rhs) const
	{
		Quaternion q0(rhs.x, rhs.y, rhs.z, 0.0f);
		Quaternion q1 = (*this) * q0;
		Quaternion qInv = this->GetConjugate();
		Quaternion q2 = q1 * qInv;

		return Vector4(q2.x, q2.y, q2.z, 1.0f);

	}

	Quaternion operator+(const Quaternion& rhs )
	{
		return Quaternion(x + rhs.x, y + rhs.y, rhs.z + z, rhs.w + w);
	}

	Quaternion& operator+=(const Quaternion& rhs)
	{
		*this = (*this) + rhs;
		return (*this);
	}
};

//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
// TransformDetails2D class
//	 --			I would Call this 'Rectangle', but apparently that's already defined with a Windows File, So...
//~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
struct TransformDetails2D
{
	union
	{
		struct { float x, y, width, height; };
		struct { float startx, starty, endx, endy; };
		struct { Vector2 Position, Size; };
		struct { Vector4 Transform; };
	};
};

} // Maths Namespace






typedef Maths::Matrix4				Matrix4;
typedef Maths::Vector2				Vector2;
typedef Maths::Vector4				Vector4;
typedef Maths::Quaternion			Quaternion;
typedef Maths::TransformDetails2D	TransformDetails2D;




//////////////////////////////////////////////////////////////////////////
#endif // __MATHHELPER_H_
//////////////////////////////////////////////////////////////////////////