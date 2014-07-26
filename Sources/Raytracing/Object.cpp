#include "header.h"
#include "Object.h"
#include <math.h>

ObjMng* ObjMng::m_pInst = NULL;

#define x_parallel 1
#define y_parallel 2
#define z_parallel 3

bool ObjSphere::ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist)
{
	Point3 point_on_ray;
	Vector3 direction_ray;
	ray.GetRay(&point_on_ray, &direction_ray);
	Vector3 center_to_ray = center - point_on_ray;
	double t_c = center_to_ray * direction_ray;

	if(t_c<0) return false;

	double b = (center_to_ray.norm()*center_to_ray.norm()) - t_c*t_c;
	if(b > radius*radius) return false;
	
	double t_u_square = radius*radius - b;

	double t = t_c - sqrt(t_u_square);
	*pnt = point_on_ray + t*direction_ray;
	*intersect_dist = (*pnt - point_on_ray).norm();
	
	return true;
}

bool ObjCube::ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist)
{
	bool is_intersect = false;
	Point3 intersect_point;
	Point3 position;
	Vector3 direction;
	ray.GetRay(&position, &direction);
	
	double t_x1 = 0.f, t_x2 = 0.f;
	double t_y1 = 0.f, t_y2 = 0.f;
	double t_z1 = 0.f, t_z2 = 0.f;
	double t_1 = 0.f, t_2 = 0.f;
	int parallel_axis = 0;

	// x에 대하여
	if(direction[0] > 0)
	{
		t_x1 = (x_min - position[0])/direction[0];
		t_x2 = (x_max - position[0])/direction[0];
	}
	else
	{
		t_x1 = (x_max - position[0])/direction[0];
		t_x2 = (x_min - position[0])/direction[0];
	}

	// y에 대하여
	if(direction[1] > 0)
	{
		t_y1 = (y_min - position[1])/direction[1];
		t_y2 = (y_max - position[1])/direction[1];
	}
	else
	{
		t_y1 = (y_max - position[1])/direction[1];
		t_y2 = (y_min - position[1])/direction[1];
	}

	// z에 대하여
	if(direction[2] > 0)
	{
		t_z1 = (z_min - position[2])/direction[2];
		t_z2 = (z_max - position[2])/direction[2];
	}
	else
	{
		t_z1 = (z_max - position[2])/direction[2];
		t_z2 = (z_min - position[2])/direction[2];
	}

	// 공통범위 최소 t값
	if(t_x1 > t_y1) 
	{
		t_1 = t_x1;
		parallel_axis = x_parallel;
	}
	else 
	{
		t_1 = t_y1;
		parallel_axis = y_parallel;
	}
	if(t_z1 > t_1) 
	{
		t_1 = t_z1;
		parallel_axis = z_parallel;
	}
	// 공통범위 최대 t값
	if(t_x2 < t_y2)
		t_2 = t_x2;
	else 
		t_2 = t_y2;
	if(t_z2 < t_2) 
		t_2 = t_z2;

	if(t_1 <= t_2 && t_1 > 0)
	{
		is_intersect = true;
		*pnt = position + t_1*direction;
		*intersect_dist = (*pnt - position).norm();

		//normal  계산
		if(parallel_axis == x_parallel)
		{
			Vector3 x(1.f, 0.f, 0.f);
			if( x*direction < 0)
				this->normal = x;			
			else this->normal = -1.0*x;
		}
		else if(parallel_axis == y_parallel)
		{
			Vector3 y(0.f, 1.f, 0.f);
			if( y*direction < 0)
				this->normal = y;
			else this->normal = -1.0*y;
		}
		else if(parallel_axis == z_parallel)
		{
			Vector3 z(0.f, 0.f, 1.f);
			if( z*direction < 0)
				this->normal = z;
			else this->normal = -1.0*z;
		}
	}
	return is_intersect;
}

bool ObjPlane::ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist)
{
	bool is_intersect = false;
	Vector3 d;
	Point3 eye;
	ray.GetRay(&eye, &d);

	double t_n = normal * d;

	if(t_n == 0) is_intersect = false;
	else
	{
		double n_dot_eye = normal[0]*eye[0] + normal[1]*eye[1] + normal[2]*eye[2];
		double t = -(n_dot_eye + D)/t_n;

		if(t<=0) is_intersect = false;
		else
		{
			is_intersect = true;
			*pnt = eye + t*d;
			*intersect_dist = (*pnt - eye).norm();
		}	
	}

	return is_intersect;
}


bool ObjTriangle::ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist)
{
	bool is_intersect = false;

	Vector3 ray_dir;
	Point3 ray_pos;
	ray.GetRay(&ray_pos, &ray_dir);

	// check if ray intersects with the plane whicch includes this triangle
	Vector3 normal = (p2-p1)^(p3-p1);
	//if( normal*ray_dir > 0.0) normal *= -1.0;
	Color black(0.0, 0.0, 0.0, 1.0);
	ObjPlane plane(black, black, 0.0, 0.0, 0.0, 0.0, normal, p1);

	Point3 temp_intersect_p;
	double temp_intersect_d;
	bool is_hit_temp = false;
	is_hit_temp = plane.ClosestIntersection(ray, &temp_intersect_p, &temp_intersect_d);

	// if it hits with plane
	if(is_hit_temp /*&& temp_intersect_d < 10000.0*/)
	{
		//for edge1(p1p2)
		Vector3 triangle_edge = p2 - p1;
		Vector3 point_edge = temp_intersect_p - p1;
		Vector3 cross_vec = triangle_edge ^ point_edge;
		if(normal*cross_vec < 0)
			is_hit_temp = false;
		//for edge2(p2p3)
		triangle_edge = p3 - p2;
		point_edge = temp_intersect_p - p2;
		cross_vec = triangle_edge ^ point_edge;
		if(normal*cross_vec < 0)
			is_hit_temp = false;

		//for edge3(p3p1)
		triangle_edge = p1 - p3;
		point_edge = temp_intersect_p - p3;
		cross_vec = triangle_edge ^ point_edge;
		if(normal*cross_vec < 0)
			is_hit_temp = false;

		if(is_hit_temp)
		{		
			is_intersect = true;
			*pnt = temp_intersect_p;
			*intersect_dist = temp_intersect_d;
			if(normal * ray_dir) 
				normal *= -1.0;
			this->normal = normal;
		}
	}

	return is_intersect;
}


bool ObjFromFile::ClosestIntersection(Ray ray, Point3* pnt, double* intersect_dist)
{	
	bool is_intersect = false;
	Point3 p1, p2, p3;
	Vector3 n1, n2, n3;
	int idx1, idx2, idx3;
	
	Vector3 ray_dir;
	Point3 ray_pos;
	ray.GetRay(&ray_pos, &ray_dir);

	Point3 temp_intersect_p;
	double temp_intersect_d;
	//받아와서 세 점에 넣기!
	
	vector<Point3>::iterator itr = this->l_face_vertex.begin();
	vector<Vector3>::iterator itr_normal = this->l_face_vertex_normal.begin();

	for( ; itr != this->l_face_vertex.end(); ++itr, ++itr_normal)
	{
		idx1 = (int)(*itr)[0];
		idx2 = (int)(*itr)[1];
		idx3 = (int)(*itr)[2];
		p1 = this->l_vertex[idx1];
		p2 = this->l_vertex[idx2];
		p3 = this->l_vertex[idx3];

		Color black(0.0, 0.0, 0.0, 1.0);
		ObjTriangle triangle(black, black, 0.0, 0.0, 0.0, 0.0, p1, p2, p3);

		bool is_intersect_triangle = triangle.ClosestIntersection(ray, &temp_intersect_p, &temp_intersect_d);

		if(is_intersect_triangle)
		{
			n1 = this->l_vertex_normal[ (int)(*itr_normal)[0] ];
			n2 = this->l_vertex_normal[ (int)(*itr_normal)[1] ];
			n3 = this->l_vertex_normal[ (int)(*itr_normal)[2] ];

			*pnt = temp_intersect_p;
			*intersect_dist = temp_intersect_d;
			this->normal = ((n1 + n2 + n3)/3).Normalize();
//			if(this->normal * ray_dir) 
//				this->normal *= -1.0;

			is_intersect = true;
			break;
		}
	}

	return is_intersect;
}
