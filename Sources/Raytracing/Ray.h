//Tracing : 레이 추적 시작 화면 픽셀 수만큼 반복문 돌려 Tracing을 수행하여 픽셀 색깔 받아들임
//Shade : 물체와 첫 광선이 만날 때, 만나는 물체, 지점, 광선의 방향을 받아, 그 지점의 색깔을 계산해 리턴
//ShadowRay : 그림자인지 아닌지 판단하는 함수
//LocalIllumination : 그 점에서 각 광원에 의해 나타나는 색깔 계산

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






//헤더끼리 꼬일때는, 선언할 헤더 파일 만들고
//거기다가 Class A; 이런식으로 선언만 해준당
//그리고 구현은 cpp파일에

//그러면.. 그 클래스가 있다는 사실은 알게되므로
//cpp파일에서는 이용해서 정의를 할 수 있징
//하지만 헤더파일에서는 아직 존재 여부만 알고
//정의를 모르기 때문에.. 구현을 할 수 없당?


/* 씐나는 클래스 상속 공부

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