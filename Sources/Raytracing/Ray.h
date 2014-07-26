//Tracing : ���� ���� ���� ȭ�� �ȼ� ����ŭ �ݺ��� ���� Tracing�� �����Ͽ� �ȼ� ���� �޾Ƶ���
//Shade : ��ü�� ù ������ ���� ��, ������ ��ü, ����, ������ ������ �޾�, �� ������ ������ ����� ����
//ShadowRay : �׸������� �ƴ��� �Ǵ��ϴ� �Լ�
//LocalIllumination : �� ������ �� ������ ���� ��Ÿ���� ���� ���

#ifndef _RAY_H_
#define _RAY_H_

#include "Graphics_math.h"
#include "Object.h"
#include "Light.h"

void TracingFirstRay(const char* pFileName, int w, int h);
void WritePPM(FILE* pFile, Color _color);

class Ray
{ 
public:
	Ray() {}
	~Ray() {}

public:
	//Ray setting & get info. of ray
	void GetRay(Point3* _pnt, Vector3* _vec) { *_pnt = pnt; *_vec = vec;}
	void SetRay(Point3 _pnt, Vector3 _vec) { pnt = _pnt; vec = _vec.Normalize();}
	
	//when recursive level of reflection is not zero.
	int GetLevel() { return level;}
	void SetLevel(int _level) { level = _level; }

	//Get the color
	Color Tracing(Object* start_obj);
	Color Shade(Object* obj, Point3 intersect_pnt);
	Color LocalIllumination(Object* obj, Point3 pnt, Light* light);

	//Get the intersection point among all the objects
	bool ClosestIntersectOfAllObj(Object** closest_obj, Point3* point, double* distance, Object* start_obj);
	//Find out if this point is shadow or not
	bool ShadowRay(Point3 intersect_pnt, Light* light, Object* obj);
	

private:
	Point3 pnt;
	Vector3 vec;
	int level;
};

#endif






//������� ���϶���, ������ ��� ���� �����
//�ű�ٰ� Class A; �̷������� ���� ���ش�
//�׸��� ������ cpp���Ͽ�

//�׷���.. �� Ŭ������ �ִٴ� ����� �˰ԵǹǷ�
//cpp���Ͽ����� �̿��ؼ� ���Ǹ� �� �� ��¡
//������ ������Ͽ����� ���� ���� ���θ� �˰�
//���Ǹ� �𸣱� ������.. ������ �� �� ����?


/* ������ Ŭ���� ��� ����

class Monster
{
public:
	virtual void Fly() = 0;
};

Bird* bird = new Bird();
Bird* bird1 = new Dulgi();
Bird* bird2 = new Penguin();
Bird* bird3 = new Bird();

bird2.Fly();
((Penguin*)bird2)->jackjack();

class Bulsajo : public Bird
{

};

class Dulgi : public Bird
{
public:
	virtual void Fly() {}
	void Gugu() {}
};

class DakDulgi : public Dulgi
{
public:
 	virtual void Fly() {}
};

class Penguin : public Bird
{
public:
	virtual void Fly() {}
	void JackJack() {}
};


 */