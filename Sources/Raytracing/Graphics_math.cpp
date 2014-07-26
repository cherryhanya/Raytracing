#include "Graphics_math.h"
#include <cassert>
#include <math.h>


Point3::Point3(double x, double y, double z)
{
	P[0] = x;
	P[1] = y;
	P[2] = z;
}
Point3::Point3(const Point3 &cpy)
{
	for(int i=0; i <3; i++)
		P[i] = cpy.P[i];
}
Point3::~Point3()
{
}

Point3 &Point3::operator =(const Point3 &rhs)
{
	for(int i = 0; i<3; i++)
		P[i] = rhs.P[i];
	return *this;
}
double &Point3::operator [](const int &idx)
{
	assert( idx>=0 && idx<=2);
	return P[idx];
}
const double &Point3::operator [](const int &idx) const
{
	assert( idx>=0 && idx<=2);
	return P[idx];
}

Point3 Point3::Set(const double &x, const double &y, const double &z)
{
	P[0] = x;
	P[1] = y;
	P[2] = z;
	return *this;
}

Vector3::Vector3(double x, double y, double z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
}
Vector3::Vector3(const Vector3 &cpy)
{
	for (int i = 0; i < 3; i++)
		V[i] = cpy.V[i];
}
Vector3::~Vector3()
{
}

Vector3 &Vector3::operator =(const Vector3 &rhs)
{
	for(int i = 0; i <3; i++)
		V[i] = rhs.V[i];
	return *this;
}
Vector3 &Vector3::operator +=(const Vector3 &rhs)
{
	for(int i = 0; i <3; i++)
		V[i] += rhs.V[i];
	return *this;
}
Vector3 &Vector3::operator -=(const Vector3 &rhs)
{
	for(int i = 0; i <3; i++)
		V[i] -= rhs.V[i];
	return *this;
}
Vector3 &Vector3::operator *=(const double &k)
{
	for(int i =0; i <3; i++)
		V[i] *= k;
	return *this;
}
Vector3 &Vector3::operator /=(const double &k)
{
	for(int i =0; i <3; i++)
		V[i] /= k;
	return *this;
}
// ¿ÜÀû
Vector3 &Vector3::operator ^=(const Vector3 &rhs)
{
	Vector3 tmp = *this;
	V[0] = tmp.V[1]*rhs.V[2] - tmp.V[2]*rhs.V[1];
	V[1] = tmp.V[2]*rhs.V[0] - tmp.V[0]*rhs.V[2];
	V[2] = tmp.V[0]*rhs.V[1] - tmp.V[1]*rhs.V[0];
	return *this;
}

Vector3 Vector3::operator +() const
{
	return *this;
}
Vector3 Vector3::operator -() const
{
	return (*this * -1);
}

double &Vector3::operator[](const int &idx)
{
	assert(idx>=0  && idx<=2);
	return V[idx];
}
const double &Vector3::operator[](const int &idx) const
{
	assert(idx>=0  && idx<=2);
	return V[idx];
}

Vector3 Vector3::Set(const double &x, const double &y, const double &z)
{
	V[0] = x;
	V[1] = y;
	V[2] = z;
	return *this;
}
Vector3 &Vector3::Normalize()
{
	double length = this->norm();
	if(length!=0.0)
	{
		for(int i=0; i <3; i++)
			V[i] /= length;
	}
	return *this;
}
double Vector3::norm()
{
	return sqrt(V[0]*V[0] + V[1]*V[1] + V[2]*V[2]);
}

void Color::SetColor(double _r, double _g, double _b, double _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
	MAXCOLOR(r, g, b, 1.0f);
}

Color Color::GetColor()
{
	return Color(r, g, b, a);
}

void Color::GetColor(double *_r, double *_g, double *_b, double *_a)
{
	*_r = r;
	*_g = g;
	*_b = b;
	*_a = a;
}

Color Color::operator +(const Color &rhs)
{
	Color tmp;
	tmp.r = r + rhs.r;
	tmp.g = g + rhs.g;
	tmp.b = b + rhs.b;
	tmp.a = a + rhs.a;
	MAXCOLOR(tmp.r, tmp.g, tmp.b, 1.0f);	
	MINCOLOR(tmp.r, tmp.g, tmp.b, 0.0f);
	return tmp;
}

Color Color::operator -(const Color &rhs)
{
	Color tmp;
	tmp.r = r - rhs.r;
	tmp.g = g - rhs.g;
	tmp.b = b - rhs.b;
	tmp.a = a - rhs.a;
	MAXCOLOR(tmp.r, tmp.g, tmp.b, 1.0f);	
	MINCOLOR(tmp.r, tmp.g, tmp.b, 0.0f);
	return tmp;
}

Color Color::operator *(const Color &rhs)
{
	Color tmp;
	tmp.r = r * rhs.r;
	tmp.g = g * rhs.g;
	tmp.b = b * rhs.b;
	tmp.a = a * rhs.a;
	MAXCOLOR(tmp.r, tmp.g, tmp.b, 1.0f);	
	MINCOLOR(tmp.r, tmp.g, tmp.b, 0.0f);
	return tmp;
}