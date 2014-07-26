#ifndef _GRAPHICS_MATH_H_
#define _GRAPHICS_MATH_H_

#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

class Vector3;
class Point3;
class Color;

#define PI 3.14159265358979323846
#define SQRT(X)			sqrt((X))
#define MAX(x, y)		( (x)>(y) ? (x) : (y) )
#define MIN(x, y)		( (x)>(y) ? (y) : (x) )
#define ABS(X)			(  ( (X)>0.0 ) ? (X) : ( -(X) ) )

#define MAXCOLOR(R, G, B, max_value)\
{\
	if(R > max_value) R = max_value;\
	if(G > max_value) G = max_value;\
	if(B > max_value) B = max_value;\
}\

#define MINCOLOR(R, G, B, min_value)\
{\
	if(R < min_value) R = min_value;\
	if(G < min_value) G = min_value;\
	if(B < min_value) B = min_value;\
}\
//friend, inline, & // const??
class Vector3
{
public:
	//생성자 및 소멸자
	Vector3(double x  = 0.0, double y = 0.0, double z = 0.0);
	Vector3(const Vector3 &cpy);
	virtual ~Vector3();

	//대입, 복합대입 연산자
	Vector3 &operator =(const Vector3 &rhs);
	Vector3 &operator +=(const Vector3 &rhs);
	Vector3 &operator -=(const Vector3 &rhs);
	Vector3 &operator *=(const double &k);
	Vector3 &operator /=(const double &k);
	Vector3 &operator ^=(const Vector3 &rhs);
	
	//단항연산자
	Vector3 operator +() const;
	Vector3 operator -() const;

	//첨자연산자
	double &operator[](const int &idx);
	const double &operator[](const int &idx) const;

	//멤버함수
	Vector3 Set(const double &x, const double &y, const double &z);
	Vector3 &Normalize();
	double norm();

public:
	//프렌드 함수
	friend Vector3 operator +(const Vector3 &lhs, const Vector3 &rhs);
	friend Vector3 operator -(const Vector3 &lhs, const Vector3 &rhs);
	friend Vector3 operator *(const double &k, const Vector3 &rhs);
	friend Vector3 operator *(const Vector3 &lhs, const double &k);
	friend double operator *(const Vector3 &lhs, const Vector3 & rhs);
	friend Vector3 operator /(const Vector3 &lhs, const double &k);
	friend Vector3 operator ^(const Vector3 &lhs, const Vector3 &rhs);
	friend bool operator ==(const Vector3 &lhs, const Vector3 &rhs);
	friend bool operator !=(const Vector3 &lhs, const Vector3 &rhs);
	friend ostream &operator <<(ostream &os, const Vector3 &rhs);
	friend istream &operator >>(istream &is, Vector3 &rhs);
	friend double dist(const Vector3 &v1, const Vector3 &v2);

protected:
	double V[3];
};

class Point3
{
public:
	//생성자 및 소멸자
	Point3(double x = 0.0, double y = 0.0, double z = 0.0);
	Point3(const Point3 &cpy);
	virtual ~Point3();

	//대입연산자,  첨자연산자
	Point3 &operator =(const Point3 &rhs);
	double &operator [](const int &idx);
	const double &operator [](const int &idx) const;

	//멤버함수
	Point3 Set(const double &x, const double &y, const double &z);
	
public:
	//프렌드 함수
	friend Vector3 operator -(const Point3 &lhs, const Point3 &rhs);
	friend Point3 operator -(const Point3 &lhs, const Vector3 &rhs);
	friend Point3 operator +(const Point3 &lhs, const Vector3 &rhs);
	friend Point3 operator +(const Vector3 &lhs, const Point3 &rhs);
	friend bool operator ==(const Point3 &lhs, const Point3 &rhs);
	friend bool operator != (const Point3 &lhs, const Point3 &rhs);
	friend ostream &operator <<(ostream &os, const Point3 &rhs);
	friend istream &operator >>(istream &is, Point3 &rhs);
	friend double dist(const Point3 &p1, const Point3 &p2);

protected:
	double P[3];
};

class Color
{
public:
	Color()
		: r(0.f), g(0.f), b(0.f), a(1.f)
	{}
	Color(double _r, double _g, double _b, double _a)
		: r(_r), g(_g), b(_b), a(_a)
	{
		MAXCOLOR(r, g, b, 1.0f);
		MINCOLOR(r, g, b, 0.0f);
	}
	~Color() {}

public:
	void SetColor(double _r, double _g, double _b, double _a);
	Color GetColor();
	void GetColor(double *_r, double *_g, double *_b, double *_a);
	Color operator +(const Color &rhs);
	Color operator -(const Color &rhs);
	Color operator *(const Color &rhs);

public:
	friend Color operator *(const double k, const Color &rhs);
	friend Color operator *(const Color &lhs, const double k);

public:
	double r, g, b, a;
};



inline Vector3 operator -(const Point3 &lhs, const Point3 &rhs)
{
	return Vector3(lhs.P[0] - rhs.P[0], lhs.P[1] - rhs.P[1], lhs.P[2] - rhs.P[2]);
}

inline Point3 operator -(const Point3 &lhs, const Vector3 &rhs)
{
	return Point3(lhs.P[0] - rhs[0], lhs.P[1] - rhs[1], lhs.P[2] - rhs[2]);
}

inline Point3 operator +(const Point3 &lhs, const Vector3 &rhs)
{
	return Point3(lhs.P[0] + rhs[0], lhs.P[1] + rhs[1], lhs.P[2] + rhs[2]);
}

inline Point3 operator +(const Vector3 &lhs, const Point3 &rhs)
{
	return Point3(lhs[0] + rhs.P[0], lhs[1] + rhs.P[1], lhs[2] + rhs.P[2]);
}

inline bool operator ==(const Point3 &lhs, const Point3 &rhs)
{
	bool is_equal = false;
	if( (lhs.P[0]==rhs.P[0]) && (lhs.P[1]==rhs.P[1]) && (lhs.P[2]==rhs.P[2]) ) is_equal = true;
	return is_equal;
}

inline bool operator != (const Point3 &lhs, const Point3 &rhs)
{
	bool is_equal = false;
	if( (lhs.P[0]!=rhs.P[0]) || (lhs.P[1]!=rhs.P[1]) || (lhs.P[2]==rhs.P[2]) ) is_equal = true;
	return is_equal;
}

inline ostream &operator <<(ostream &os, const Point3 &rhs)
{
	os << "(" << setw(10) << rhs.P[0] << ", " << setw(10) << rhs.P[1] << ", " << setw(10) << rhs.P[2] << ")";
	return os;
}

inline istream &operator >>(istream &is, Point3 &rhs)
{
	is >> rhs.P[0] >> rhs.P[1] >> rhs.P[2];
	return is;
}

inline double dist(const Point3 &p1, const Point3 &p2)
{
	return SQRT( (p1.P[0]-p2.P[0])*(p1.P[0]-p2.P[0]) + (p1.P[1]-p2.P[1])*(p1.P[1]-p2.P[1]) + (p1.P[2]-p2.P[2])*(p1.P[2]-p2.P[2]) );
}

inline Vector3 operator +(const Vector3 &lhs, const Vector3 &rhs)
{
	return Vector3(lhs.V[0]+rhs.V[0], lhs.V[1]+rhs.V[1], lhs.V[2]+rhs.V[2]);
}

inline Vector3 operator -(const Vector3 &lhs, const Vector3 &rhs)
{
	return Vector3(lhs.V[0]-rhs.V[0], lhs.V[1]-rhs.V[1], lhs.V[2]-rhs.V[2]);
}

inline Vector3 operator *(const double &k, const Vector3 &rhs)
{
	return Vector3(k*rhs.V[0], k*rhs.V[1], k*rhs.V[2]);
}

inline Vector3 operator *(const Vector3 &lhs, const double &k)
{
	return Vector3(k*lhs.V[0], k*lhs.V[1], k*lhs.V[2]);
}

inline double operator *(const Vector3 &lhs, const Vector3 & rhs)
{
	double tmp = 0;
	for (int i = 0; i <3; ++i)
		tmp += lhs.V[i] * rhs.V[i];
	return tmp;
}

inline Vector3 operator /(const Vector3 &lhs, const double &k)
{
	return Vector3( lhs.V[0]/k, lhs.V[1]/k, lhs.V[2]/k );
}

inline Vector3 operator ^(const Vector3 &lhs, const Vector3 &rhs)
{
	return Vector3(lhs.V[1] * rhs.V[2] - lhs.V[2] * rhs.V[1], lhs.V[2] * rhs.V[0] - lhs.V[0] * rhs.V[2], lhs.V[0] * rhs.V[1] - lhs.V[1] * rhs.V[0]);
}

inline bool operator ==(const Vector3 &lhs, const Vector3 &rhs)
{
	bool is_equal = false;
	if( (lhs.V[0]==rhs.V[0]) && (lhs.V[1]==rhs.V[1]) && (lhs.V[2]==rhs.V[2]) ) is_equal = true;
	return is_equal;
}

inline bool operator !=(const Vector3 &lhs, const Vector3 &rhs)
{	
	bool is_equal = false;
	if( (lhs.V[0]!=rhs.V[0]) || (lhs.V[1]!=rhs.V[1]) || (lhs.V[2]==rhs.V[2]) ) is_equal = true;
	return is_equal;
}

inline ostream &operator <<(ostream &os, const Vector3 &rhs)
{
	os << "(" << setw(10) << rhs.V[0] << ", " << setw(10) << rhs.V[1] << ", " << setw(10) << rhs.V[2] << ")";
	return os;
}

inline istream &operator >>(istream &is, Vector3 &rhs)
{
	is >> rhs.V[0] >> rhs.V[1] >> rhs.V[2];
	return is;
}

inline double dist(const Vector3 &v1, const Vector3 &v2)
{
	return SQRT( (v1.V[0]-v2.V[0])*(v1.V[0]-v2.V[0]) + (v1.V[1]-v2.V[1])*(v1.V[1]-v2.V[1]) + (v1.V[2]-v2.V[2])*(v1.V[2]-v2.V[2]) );
}

inline Color operator *(const double k, const Color &rhs)
{
	Color tmp;
	tmp.r = k*rhs.r;
	tmp.g = k*rhs.g;
	tmp.b = k*rhs.b;
	tmp. a = k*rhs.a;
	MAXCOLOR(tmp.r, tmp.g, tmp.b, 1.0f);
	MINCOLOR(tmp.r, tmp.g, tmp.b, 0.0f);
	return tmp;
}

inline Color operator *(const Color &lhs, const double k)
{
	Color tmp;
	tmp.r = k*lhs.r;
	tmp.g = k*lhs.g;
	tmp.b = k*lhs.b;
	tmp. a = k*lhs.a;
	MAXCOLOR(tmp.r, tmp.g, tmp.b, 1.0f);		
	MINCOLOR(tmp.r, tmp.g, tmp.b, 0.0f);
	return tmp;
}	

#endif