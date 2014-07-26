//Object, Light는 몇개가 될지 모르니깐...중요한 것들 만들어놓고, 매니져를 둬서 관리한다
//싱글턴으로 어디서든 접근할 수 있게 (Ray속의 함수 안에서 사용할거니깐)
//근데 오브젝트는 어떻게 할까? 상속?
//object에서 필요한 건 : 읽어들여서 저장하는거, 충돌체크, 색깔

//필요한 함수들? -> 파일에서 읽어들여 저장하는 함수, 충돌체크하는 함수 (레이, 카메라, obejct정보)
//카메라가 필요하겠네?

//ClosestIntersection : object list를 돌면서 가장 가까운 교점 찾아냄 Ray::Trace()함수 안에서 쓸거

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Graphics_math.h"
#include "Ray.h"
#include <vector>

using namespace std;


class Object
{
public:
	Object(Color _diffuse, Color _specular, double _shininess, 
		double _reflected_coef, double _transmitted_coef, double _refracted_idx)
		: diffuse(_diffuse), specular(_specular), shininess(_shininess),
		reflected_coef(_reflected_coef), transmitted_coef(_transmitted_coef),
		refracted_idx(_refracted_idx)
	{}
	~Object() {}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist) = 0;
	virtual Vector3 GetObjNormal(Point3 intersect_pnt) = 0;

public:	
	void Get(Color* pDiffuse, Color* pSpecular, double* pShininess, double* pReflected_coef, double* pTransmitted_coef, double* pRefracted_idx) 
	{ 
		*pDiffuse = diffuse;
		*pSpecular = specular;
		*pShininess = shininess;
		*pReflected_coef = reflected_coef;
		*pTransmitted_coef = transmitted_coef;
		*pRefracted_idx = refracted_idx;
	}

public:
	Color diffuse;
	Color specular;
	double shininess;
	double reflected_coef;
	double transmitted_coef;
	double refracted_idx;
};


class ObjSphere : public Object
{
public:	
	ObjSphere(Color _diffuse, Color _specular, double _shininess, 
		double _reflected_coef, double _transmitted_coef, double _refracted_idx,
		Point3 _center, double _radius)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		center = _center;
		radius = _radius;
	}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist);
	virtual Vector3 GetObjNormal(Point3 intersect_pnt)
	{
		Vector3 normal;
		normal = intersect_pnt - center;
		normal.Normalize();
		return normal;
	}

private:
	Point3 center;
	double radius;
};


class ObjCube : public Object
{
public:
	ObjCube(Color _diffuse, Color _specular, double _shininess, 
		double _reflected_coef, double _transmitted_coef, double _refracted_idx,
		Point3 _vertex[8])
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		for(int i = 0; i < 8; i++)
		{
			vertex[i] = _vertex[i];

			if(i ==0)
			{
				x_min = (vertex[0])[0];		x_max = (vertex[0])[0];
				y_min = (vertex[0])[1];		y_max = (vertex[0])[1];
				z_min = (vertex[0])[2];		z_max = (vertex[0])[2];
			}

			if((vertex[i])[0] > x_max) 
				x_max = (vertex[i])[0];			
			if((vertex[i])[0] < x_min)
				x_min = (vertex[i])[0];

			if((vertex[i])[1] > y_max) 
				y_max = (vertex[i])[1];			
			if((vertex[i])[1] < y_min)
				y_min = (vertex[i])[1];

			if((vertex[i])[2] > z_max) 
				z_max = (vertex[i])[2];			
			if((vertex[i])[2] < z_min)
				z_min = (vertex[i])[2];
		}
	}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist);
	virtual Vector3 GetObjNormal(Point3 intersect_pnt) { return this->normal; }

private:
	Point3 vertex[8];
	double x_min, x_max;
	double y_min, y_max;
	double z_min, z_max;
	Vector3 normal;
};


class ObjPlane : public Object
{
public:
	ObjPlane(Color _diffuse, Color _specular, double _shininess, 
		double _reflected_coef, double _transmitted_coef, double _refracted_idx,
		Vector3 _normal, Point3 _point)
		: Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		normal = _normal.Normalize();
		point = _point;
		D = -(normal[0]*point[0] + normal[1]*point[1] + normal[2]*point[2]);
	}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist);
	virtual Vector3 GetObjNormal(Point3 intersect_pnt) 	{ return this->normal; }

private:
	Vector3 normal;
	Point3 point;
	double D;
};


class ObjTriangle : public Object
{
public:
	ObjTriangle(Color _diffuse, Color _specular, double _shininess,
		double _reflected_coef, double _transmitted_coef, double _refracted_idx,
		Point3 _p1, Point3 _p2, Point3 _p3)
		:Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		p1 = _p1;
		p2 = _p2;
		p3 = _p3;
		normal = Vector3(0.0, 0.0, 0.0);
	}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist);
	virtual Vector3 GetObjNormal(Point3 intersect_pnt) 	{ return this->normal; }

private:
	Point3 p1;
	Point3 p2;
	Point3 p3;
	Vector3 normal;
};


class ObjFromFile : public Object
{
public:
	ObjFromFile(Color _diffuse, Color _specular, double _shininess,
		double _reflected_coef, double _transmitted_coef, double _refracted_idx)
		:Object(_diffuse, _specular, _shininess, _reflected_coef, _transmitted_coef, _refracted_idx)
	{
		normal = Vector3(0.0, 0.0, 0.0);
	}

public:
	virtual bool ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist);
	virtual Vector3 GetObjNormal(Point3 intersect_pnt) 	{ return this->normal; }

public:
	void PutPointToListV(Point3 pnt) { l_vertex.push_back(pnt); }
	void PutPointToListVT(Point3 pnt) { l_vertex_texture.push_back(pnt); }
	void PutPointToListVN(Vector3 vec) { l_vertex_normal.push_back(vec); }
	void PutPointToListFaceV(Point3 pnt) { l_face_vertex.push_back(pnt); }
	void PutPointToListFaceVT(Point3 pnt) { l_face_vertex_texture.push_back(pnt); }
	void PutPointToListFaceVN(Vector3 vec) { l_face_vertex_normal.push_back(vec); }

private:
	vector<Point3> l_vertex;
	vector<Point3> l_vertex_texture;
	vector<Vector3> l_vertex_normal;
	vector<Point3> l_face_vertex;
	vector<Point3> l_face_vertex_texture;
	vector<Vector3> l_face_vertex_normal;
	Vector3 normal;
};


class ObjMng
{
public:
	typedef vector<Object*> OBJLIST;
	
public:
	static ObjMng* GetInstance()
	{
		if(m_pInst == NULL) m_pInst = new ObjMng();
		return m_pInst;
	}

public:
	OBJLIST& GetList() { return object_list; }
	void PushBack(Object* pObj) { object_list.push_back(pObj); }

private:
	static ObjMng* m_pInst;

private:
	OBJLIST object_list;
};

#endif