#include "header.h"
#include "Light.h"
#include <math.h>

LightMng* LightMng::m_pInst = NULL;


Color LightPoint::PhongShading(Object* obj, Point3 pnt, Ray ray)
{
	Color tmp_color(0.f, 0.f, 0.f, 1.f);

	Vector3 ray_dir;
	Point3 ray_position;
	ray.GetRay(&ray_position, &ray_dir);

	Vector3 N = obj->GetObjNormal(pnt);
	Vector3 L = this->position - pnt;				L.Normalize();
	Vector3 V = ray_position - pnt;					V.Normalize();
	Vector3 H = (L+V)/2.f; 

	double dist = L.norm();
	double k = 1.0f / (1.0f + 0.045f*dist +  0.0075f*dist*dist); //°¨¼â»ó¼ö
	
	Color diff, spec;
	double shininess = 0.f, reflect_coef = 0.f, trans_coef = 0.f, refract_idx = 0.f;
	obj->Get(&diff, &spec, &shininess, &reflect_coef, &trans_coef, &refract_idx);

	//diffuse
	if(N*L>0)
		tmp_color = tmp_color  + k * (N*L) * (diff  * this->color);
	//specular
	if(N*H>0)
		tmp_color = tmp_color + k * pow(N*H, shininess) * (spec * this->color);
	
	return tmp_color;
}

bool LightPoint::ShadowIntersection(Point3 intersect_pnt, Object* obj)
{
	Object* intersect_closest_obj;
	bool is_shadow = false;
	bool is_intersect = false;

	Vector3 ray_direction;
	Point3 ray_position = intersect_pnt;
	Ray new_ray;

	Point3 tmp_point;
	double tmp_dist = 0.f;

	ray_direction = this->position - intersect_pnt;
	new_ray.SetRay(ray_position, ray_direction);

	is_shadow = new_ray.ClosestIntersectOfAllObj(&intersect_closest_obj, &tmp_point, &tmp_dist, obj);

	return is_shadow;
}

Color LightDirect::PhongShading(Object* obj, Point3 pnt, Ray ray)
{	
	Color tmp;
	return tmp;
}

bool LightDirect::ShadowIntersection(Point3 intersect_pnt, Object* obj)
{
	Object* intersect_closest_obj;
	bool is_shadow = false;
	bool is_intersect = false;

	Vector3 ray_direction;
	Point3 ray_position = intersect_pnt;
	Ray new_ray;

	Point3 tmp_point;
	double tmp_dist = 0.f;

	ray_direction = -1.0f *this->direction;
	new_ray.SetRay(ray_position, ray_direction);

	is_shadow = new_ray.ClosestIntersectOfAllObj(&intersect_closest_obj, &tmp_point, &tmp_dist, obj);

	return is_shadow;
}

void Camera::UpdateUVDirect(int width, int height)
{
	Vector3 cross_look_up = look^up;
	u = cross_look_up.Normalize();
	Vector3 cross_look_u = look^u;
	v = cross_look_u.Normalize();
	direct = (width / (2*tan(fovx*0.5f))) * look.Normalize() - (width/2.0f)*u - (height/2.0f)*v;
}

void Camera::GetUVDirect(Vector3* _u, Vector3* _v, Vector3* _direct)
{
	*_u = u;
	*_v = v;
	*_direct = direct;
}