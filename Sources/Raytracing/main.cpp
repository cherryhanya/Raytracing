#include <iostream>
#include <fstream>
#include <string>
#include "header.h"
#include "Graphics_math.h"
#include "Ray.h"
#include "ObjectObj.h"

using namespace std;

char JumpNewLine(FILE* pFile)
{
	char c = '\0';

	do
	{			
		if (feof(pFile))
			break;

		fread(&c, sizeof(char), 1, pFile);

	} while ((c == '\n') || (c == '\r'));
	
	fseek(pFile, -(int)(sizeof(char)), SEEK_CUR);

	return c;
}
bool LoadData(const char* pFileName, int* Width, int* Height); 

int main()
{
	int Width = 0, Height = 0;
	
	//1. 파일 읽어들이기(모든 정보 저장)
	//1-1. 카메라의 uvdirect vectors  계산해둔당
	LoadData("Data/scene_final.txt", &Width, &Height);
	
	//2. TracingFirstRay 함수로 추적 시작
	//2-1. 각 픽셀의 값을 받아서 ppm파일로 출력
	TracingFirstRay("ResultScene.ppm", Width, Height);

	return 0;
}

bool LoadData(const char* pFileName, int* Width, int* Height)
{
	FILE* pFile;
	fopen_s(&pFile, pFileName, "r");
	if(pFile == nullptr)
	{
		cout << "파일이 오픈되지 않았습니다." << endl;
		return false;
	}

	Point3 camera_eye;
	Vector3 camera_look;
	Vector3 camera_up;
	double camera_fovx;
	
	fscanf_s(pFile, "%lf %lf %lf", &camera_eye[0], &camera_eye[1], &camera_eye[2]);
    fscanf_s(pFile, "%lf %lf %lf", &camera_up[0], &camera_up[1], &camera_up[2]);
    fscanf_s(pFile, "%lf %lf %lf", &camera_look[0], &camera_look[1], &camera_look[2]);
    fscanf_s(pFile, "%lf", &camera_fovx);

	fscanf_s(pFile, "%d", Width);
	fscanf_s(pFile, "%d", Height);
	
	LightMng::GetInstance()->GetCamera().InitCamera(camera_eye, camera_look, camera_up, camera_fovx);
	LightMng::GetInstance()->GetCamera().UpdateUVDirect(*Width, *Height);

	int light_count = 0;
	char light_type = '\0';
	Point3 position;
	Color color;
	Vector3 direction;
	
	fscanf_s(pFile, "%d", &light_count);
	
	for(int i = 0; i < light_count; i++)
	{
		while( 1 )
		{
			fscanf_s(pFile, "%c", &light_type);
			if( light_type != '\n') break;
		}
		switch (light_type)
		{
		case 'p':
			{
				fscanf_s(pFile, "%lf %lf %lf", &position[0], &position[1], &position[2]);
				fscanf_s(pFile, "%lf %lf %lf", &color.r, &color.g, &color.b);
				color.r /= 255.f;
				color.g /= 255.f;
				color.b /= 255.f;
				LightPoint* pLightPoint = new LightPoint(color, Light::TYPE::LIGHT_POINT, position);
				LightMng::GetInstance()->PushBack(pLightPoint);
			}
			break;
		case 'd':
			{
				fscanf_s(pFile, "%lf %lf %lf", &direction[0], &direction[1], &direction[2]);
				fscanf_s(pFile, "%lf %lf %lf", &color.r, &color.g, &color.b);
				color.r /= 255.f;
				color.g /= 255.f;
				color.b /= 255.f;
				LightDirect* pLightDirect = new LightDirect(color, Light::LIGHT_DIRECTIONAL, direction);
				LightMng::GetInstance()->PushBack(pLightDirect);
			}
			break;
		}

	}

	int object_count = 0;
	char object_type = '\0';
	Color diffuse, specular;
	double shininess=0.f, reflected_coef=0.f, transmitted_coef=0.f, refracted_idx=0.f;

	fscanf_s(pFile, "%d", &object_count);
	
	for(int i = 0; i < object_count; i++)
	{
		while( 1 )
		{
			fscanf_s(pFile, "%c", &object_type);
			if( object_type != '\n') break;			
		}

		fscanf_s(pFile, "%lf %lf %lf", &diffuse.r, &diffuse.g, &diffuse.b);
		fscanf_s(pFile, "%lf %lf %lf", &specular.r, &specular.g, &specular.b);
		fscanf_s(pFile, "%lf", &shininess);
		fscanf_s(pFile, "%lf", &reflected_coef);
		fscanf_s(pFile, "%lf", &transmitted_coef);
		fscanf_s(pFile, "%lf", &refracted_idx);

		diffuse.r /= 255.f;		diffuse.g /= 255.f;		diffuse.b /= 255.f;
		specular.r /= 255.f;	specular.g /= 255.f;	specular.b /= 255.f;

		switch(object_type)
			{
			case 'c':
				{
					Point3 vertex[8];
					for(int i = 0; i < 8; i ++)
						fscanf_s(pFile, "%lf %lf %lf", &(vertex[i])[0], &(vertex[i])[1], &(vertex[i])[2]);
					ObjCube *pObjCube = new ObjCube(diffuse, specular, shininess, 
						reflected_coef, transmitted_coef, refracted_idx, vertex);
					ObjMng::GetInstance()->PushBack(pObjCube);
				}
				break;
			case 's':
				{
					Point3 center;
					double radius = 0.f;
					fscanf_s(pFile, "%lf %lf %lf", &center[0], &center[1], &center[2]);
					fscanf_s(pFile, "%lf", &radius);
					ObjSphere *pObjShpere = new ObjSphere(diffuse, specular, shininess,
						reflected_coef, transmitted_coef, refracted_idx, center, radius);
					ObjMng::GetInstance()->PushBack(pObjShpere);
				}
				break;
			case 'p':
				{
					Vector3 normal;
					Point3 point;
					fscanf_s(pFile, "%lf %lf %lf", &normal[0], &normal[1], &normal[2]);
					fscanf_s(pFile, "%lf %lf %lf", &point[0], &point[1], &point[2]);
					ObjPlane *pObjPlane = new ObjPlane(diffuse, specular, shininess,
						reflected_coef, transmitted_coef, refracted_idx, normal, point);
					ObjMng::GetInstance()->PushBack(pObjPlane);
				}
				break;
			case 't':
				{
					Point3 p1, p2, p3;
					fscanf_s(pFile, "%lf %lf %lf", &p1[0], &p1[1], &p1[2]);
					fscanf_s(pFile, "%lf %lf %lf", &p2[0], &p2[1], &p2[2]);
					fscanf_s(pFile, "%lf %lf %lf", &p3[0], &p3[1], &p3[2]);
					ObjTriangle *pObjTriangle = new ObjTriangle(diffuse, specular, shininess,
						reflected_coef, transmitted_coef, refracted_idx, p1, p2, p3);
					ObjMng::GetInstance()->PushBack(pObjTriangle);
				}
				break;
			case 'o':
				{
					ObjFromFile *pObjFromFile = new ObjFromFile(diffuse, specular, shininess,
						reflected_coef, transmitted_coef, refracted_idx);
					
					//고칠곳
					//yogi morgeadda
					JumpNewLine(pFile);
					char temp[256] = "";
					fgets(temp, 256, pFile);
					
					//ObjectObj read_obj(objectDir);
					ObjectObj read_obj(temp); // "Data/Object/Sphere.obj"
					read_obj.SetObject();

					int size = read_obj.GetSizeV();
					for(int i = 0; i < size; i++)
						pObjFromFile->PutPointToListV( read_obj.GetV() );

					size = read_obj.GetSizeVN();
					for(int i = 0; i < size; i++)
						pObjFromFile->PutPointToListVN( read_obj.GetVN() );
					
					size = read_obj.GetSizeFaceIndexV();
					for(int i = 0; i < size; i++)
						pObjFromFile->PutPointToListFaceV( read_obj.GetFaceIndexV() );
					
					size = read_obj.GetSizeFaceIndexVN();
					for(int i = 0; i < size; i++)
						pObjFromFile->PutPointToListFaceVN( read_obj.GetFaceIndexVN() );

					ObjMng::GetInstance()->PushBack(pObjFromFile);
				}
				break;
			}
	}
	return true;
}

//신나는 레퍼런스 공부웅
//	int n = 10;
//	int& rn = n;
//	(a += b)
	//레퍼런스를 쓰는 경우 : 포인터는 널 값을 가질 수 있당. 레퍼런스는 무조건 초기화할 때 유효한 값을 가지고 있어야 해!
	//항상 유효한 값인 게 보장됨. 널 체크 할 필요 없이 항상 쓸 때 필요해?
