#include <iostream>
#include "header.h"
#include "Ray.h"

const double MAX_DIST = numeric_limits<double>::max();
#define MAXLEVEL 4

void TracingFirstRay(const char* pFileName, int w, int h)
{
	FILE* pFile;
	fopen_s(&pFile, pFileName, "wb");

	if(pFile == nullptr)
		return;

	fprintf(pFile, "P6\n");
	fprintf(pFile, "%d\n", w);
	fprintf(pFile, "%d\n", h);
	fprintf(pFile, "%d\n", 255);

	Camera cam = LightMng::GetInstance()->GetCamera();
	Ray ray;
	Color pixel_color;
	
	cout << "Start >>>> Ray tracing process" << endl << endl;

	for(double y = 0; y< h; y+=1.0f)
	{
		for(double x = 0; x < w; x+=1.0f)
		{
			Object* first_ray_obj = nullptr;

			ray.SetRay(cam.eye, cam.direct + cam.u * x + cam.v * y);
			ray.SetLevel(0);

			pixel_color = ray.Tracing(first_ray_obj);
			WritePPM(pFile, pixel_color);
		}
		cout<<fixed;
		cout.precision(2);
		cout << (y+1)/h*100 << "%" << '\r';
	}
	cout << endl << endl << "End   >>>> Ray tracing process" << endl;

	fclose(pFile);
}

void WritePPM(FILE* pFile, Color _color)
{
	unsigned char color[3];
	color[0] = (unsigned char)(_color.r * 255);
	color[1] = (unsigned char)(_color.g * 255);
	color[2] = (unsigned char)(_color.b * 255);
	fwrite(color, 1, 3, pFile);
}

Color Ray::Tracing(Object* start_obj)
{	
	Color color;
	Color background_color(0.0f, 0.0f, 0.0f, 1.0f);

	double nearest_dist = 0.f;
	Point3 nearest_point;
	Object* closest_obj = nullptr;

	this->ClosestIntersectOfAllObj(&closest_obj, &nearest_point, &nearest_dist, start_obj);

	if(closest_obj == nullptr)
		color = background_color;
	else
		color = this->Shade(closest_obj, nearest_point);

	return color;
}

bool Ray::ClosestIntersectOfAllObj(Object** closest_obj, Point3* point, double* distance, Object* start_obj)
{
	bool is_true = false;
	bool is_intersected = false;

	Object* tmp_closest_obj = nullptr;
	Point3 pnt;
	double intersect_dist = 0.f;

	double nearest_dist = MAX_DIST;
	Point3 nearest_point;

	ObjMng::OBJLIST& obj_list = ObjMng::GetInstance()->GetList();
	ObjMng::OBJLIST::iterator itr = obj_list.begin();

	for(; itr != obj_list.end(); ++itr)
	{
		if(*itr == start_obj) continue; 

		is_intersected = (*itr)->ClosestIntersection(*this, &pnt, &intersect_dist);
	
		if( (is_intersected == true) && (intersect_dist < nearest_dist) )
		{
			nearest_dist = intersect_dist;
			nearest_point = pnt;
			tmp_closest_obj = *itr;
			is_true = true;
		}
	}

	*point = nearest_point;
	*distance = nearest_dist;
	*closest_obj = tmp_closest_obj;

	return is_true;
}

Color Ray::Shade(Object* obj, Point3 intersect_pnt)
{
	Color tmp_color(0.0f, 0.0f, 0.0f, 1.0f);
	Color shadow(0.6f, 0.6f, 0.6f, 1.0f);
	int level = this->GetLevel();
	
	LightMng::LIGHTLIST list = LightMng::GetInstance()->GetList();
	LightMng::LIGHTLIST::iterator itr = list.begin();

	if(level <= MAXLEVEL)
	{
		level++;

		for(; itr != list.end(); ++itr)
		{
			//�Ϸ�̳��̼�
			tmp_color = tmp_color + LocalIllumination(obj, intersect_pnt, *itr);

			//�׸��� ó��
			if( ShadowRay(intersect_pnt, *itr, obj))
			{
				tmp_color = tmp_color - shadow;
			}

			//�ݻ� ó��
			if( obj->reflected_coef != 0 )
			{
				Vector3 N = obj->GetObjNormal(intersect_pnt);
				Vector3 I = -1.0 * this->vec;
				Vector3 direction = 2.f*(N*I)*N - I;

				Ray reflect_ray;
				reflect_ray.SetRay(intersect_pnt, direction);
				reflect_ray.SetLevel(level);

				tmp_color = tmp_color + obj->reflected_coef * reflect_ray.Tracing(obj);
			}
			
			//���� ó��	
			if( obj->transmitted_coef != 0 )
			{
				Vector3 N = obj->GetObjNormal(intersect_pnt).Normalize();
				Vector3 I = this->vec;
				double eta = 1.0003f / obj->refracted_idx;
				//�Ф� �𸣰ڴ� �ФФФФФФФФФФФФФФ���... ���� �߸�����?
								// 0.f, 0, 0.0 �� �ٸ� ���̾�//
				if( N*I > 0.f ) // �̰� �̷��� ��Ȯ�� �����ָ� ���� ���������� ��ȯ�ؼ� ����ϱ� ������ �� �Ҽ��� ©��
				{		 		
					N = -1.f * N;
					eta = obj->refracted_idx / 1.0003f;
				}						  

				double cos_theta_i = (-1.f*I)*N;
				double cos_theta_t = obj->refracted_idx * (1.f - 1.f/(obj->refracted_idx*obj->refracted_idx)) * sqrt(1.f - cos_theta_i*cos_theta_i);
				Vector3 T = eta*I - (cos_theta_t - (eta * cos_theta_i) )*N;

				Ray transmitted_ray;
				transmitted_ray.SetRay(intersect_pnt, T);
				transmitted_ray.SetLevel(level);

				tmp_color = tmp_color + obj->transmitted_coef * transmitted_ray.Tracing(obj);
			}
		}
	}// ���� �̹� ���������
	return tmp_color;
}

bool Ray::ShadowRay(Point3 intersect_pnt, Light* light, Object* obj)
{
	bool is_shadow = false;
	is_shadow = light->ShadowIntersection(intersect_pnt, obj);
	return is_shadow;
}

Color Ray::LocalIllumination(Object* obj, Point3 pnt, Light* light)
{
	Color tmp_color(0.f, 0.f, 0.f, 1.f);
	tmp_color = light->PhongShading(obj, pnt, *this); 
	return tmp_color;
}

// static_cast ���
//		LightDirect* pld = static_cast<LightDirect*>(light);
//		LightPoint* plp = static_cast<LightPoint*>(light);

// enum ���� ���
//	if( light->type == Light::TYPE::LIGHT_DIRECTIONAL )
//	{
//	}
//	else if( light->type == Light::TYPE::LIGHT_POINT ) 
//	{
//	}
