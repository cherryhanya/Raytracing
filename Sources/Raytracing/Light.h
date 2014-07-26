//필요한 함수는 뭘까? 파일에서 읽어들이는 함수? 함수는 필요 없겠당...
//shade함수넣어야겠다. ray->shade에서 호출해서 실제 계산은 여기서하게

//PhongShade : 각 광원의 종류에 따라, 색깔 계산, Ray::LocalIllumination()에서 호출해서 쓸거당

//camera는? 첫번째레이를 쏘는 순간에 필요하다. 왜냐?? u,v를 계산해야 하니깐!


#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Graphics_math.h"
#include "Ray.h"
#include <vector>

using namespace std;


class Light
{
public:
	enum TYPE
	{
		LIGHT_POINT,
		LIGHT_DIRECTIONAL,
	};
	TYPE type;

public:
	Light(Color _color, TYPE _type) 
		: color(_color), type(_type)
	{}
	~Light() {}
	virtual Color PhongShading(Object* obj, Point3 pnt, Ray ray) = 0;
	virtual bool ShadowIntersection(Point3 intersect_pnt, Object* obj) = 0;

protected:
	Color color;
};


class LightPoint : public Light
{
public:
	LightPoint(Color _color, TYPE _type, Point3 _position)
		: Light(_color, _type)
	{
		position = _position;
	}

public:
	virtual Color PhongShading(Object* obj, Point3 pnt, Ray ray);
	virtual bool ShadowIntersection(Point3 intersect_pnt, Object* obj);

private:
	Point3 position;
};


class LightDirect : public Light
{
public:
	LightDirect(Color _color, TYPE _type, Vector3 _direction)
		: Light(_color, _type)
	{
		direction = _direction;
	}

public:
	virtual Color PhongShading(Object* obj, Point3 pnt, Ray ray);
	virtual bool ShadowIntersection(Point3 intersect_pnt, Object* obj);

private:
	Vector3 direction;
};


class Camera
{
public:
	Camera() {}
	~Camera() {}

public:
	void InitCamera(Point3 _eye, Vector3 _look, Vector3 _up, double _fovx)
	{
		eye = _eye; look = _look; up = _up; fovx = _fovx;
	}
	void UpdateUVDirect(int width, int height);
	void GetUVDirect(Vector3* _u, Vector3* _v, Vector3* _direct);

public:
	Point3 eye;
	Vector3 look;
	Vector3 up;
	double fovx;

public:
	Vector3 u;
	Vector3 v;
	Vector3 direct; // o
};


class LightMng
{
public:
	typedef vector<Light*> LIGHTLIST;

public:
	static LightMng* GetInstance()
	{
		if(m_pInst == NULL) m_pInst = new LightMng();
		return m_pInst;
	}
	void PushBack(Light* pLight) { light_list.push_back(pLight); }
	LIGHTLIST& GetList() { return light_list; }
	Camera& GetCamera() { return camera; }

private:
	static LightMng* m_pInst;

private:
	LIGHTLIST light_list;
	Camera camera;
};

#endif