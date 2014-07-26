#ifndef _OBJECTOBJ_H_
#define _OBJECTOBJ_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Graphics_math.h"
#include "Tokenizer.h"


using namespace std;

#define GetFromVector(idx, vector_container)\
{\
	if(idx < vector_container.size())\
			return vector_container[idx++];\
	return NULL;\
}\

/*	if(idx_v < m_v.size())
		return m_v[idx_v++];
	return NULL;  */

class ObjectObj
{
public:
	ObjectObj(const char* _pFileName) { pFileName = _pFileName; }
	virtual ~ObjectObj() {}
	void SetObject();

private:
	void LoadDataFromfile();
	void SaveDataFromLine(string& line);

public:
	Point3 GetV() { GetFromVector(idx_v, m_v);}
	Point3 GetVT() { GetFromVector(idx_vt, m_vt);}
	Vector3 GetVN() { GetFromVector(idx_vn, m_vn); }
	Point3 GetFaceIndexV() {	GetFromVector(idx_face_v, m_face_v); }
	Point3 GetFaceIndexVT() {	GetFromVector(idx_face_vt, m_face_vt); }
	Vector3 GetFaceIndexVN() {	GetFromVector(idx_face_vn, m_face_vn); }

	size_t GetSizeV() const { return m_v.size(); }
	size_t GetSizeVT() const { return m_vt.size(); }
	size_t GetSizeVN() const { return m_vn.size(); }
	size_t GetSizeFaceIndexV() const { return m_face_v.size(); }
	size_t GetSizeFaceIndexVT() const { return m_face_vt.size(); }
	size_t GetSizeFaceIndexVN() const { return m_face_vn.size(); }

private:
	// vertex 값 배열
	size_t idx_v;
	vector<Point3> m_v;
	// texture  값 배열
	size_t idx_vt;
	vector<Point3> m_vt;
	// normal 값 배열
	size_t idx_vn;
	vector<Vector3> m_vn;
	// vertex 인덱스 배열
	size_t idx_face_v;
	vector<Point3> m_face_v;
	// texture 인덱스 배열
	size_t idx_face_vt;
	vector<Point3> m_face_vt;
	// normal 인덱스 배열
	size_t idx_face_vn;
	vector<Vector3> m_face_vn;
	const char *pFileName;
};


void ObjectObj::SetObject()
{
	LoadDataFromfile();
	idx_v = 0;			idx_vt = 0;			idx_vn = 0;
	idx_face_v = 0;		idx_face_vt = 0;	idx_face_vn = 0;
}

void ObjectObj::LoadDataFromfile()
{
	ifstream fin(pFileName);
	if(fin.fail()) 
	{
		cout << "Failure : File Read"<< endl;
		return;
	}

	//출력-테스트
	fin.seekg(0, std::ios::end);
	streamoff filesize  = fin.tellg();
	fin.seekg(0, std::ios::beg);
	streamoff readData = 0;
	//

	string line;
	while(getline(fin, line)) 
	{
		if(line.empty())
			continue;

		SaveDataFromLine(line);

		//출력 -테스트
		readData = fin.tellg();
		if (readData > 0)
		{
			printf("\r");
			cout<< "per : "<< ((double)readData / (double)filesize) * 100.f;
		}
		//
	}

	fin.close();
}

void ObjectObj::SaveDataFromLine(string& line)
{
	if(line[0] == 'v' && line[1] == ' ')
	{
		line = &line[2];
		Tokenizer t(line, " ");

		double x = atof(t.GetToken().c_str());
		double y = atof(t.GetToken().c_str());
		double z = atof(t.GetToken().c_str()) - 13.0;
		//고칠곳

		Point3 pnt(x, y, z);
		m_v.push_back(pnt);
	}
	if(line[0] == 'v' && line[1] == 't')
	{
		line = &line[2];
		Tokenizer t(line, " ");

		double x = atof(t.GetToken().c_str());
		double y = atof(t.GetToken().c_str());
		double z = atof(t.GetToken().c_str());

		Point3 pnt(x, y, z);
		m_vt.push_back(pnt);
	}
	if(line[0] == 'v' && line[1] == 'n')
	{
		line = &line[2];
		Tokenizer t(line, " ");

		double x = atof(t.GetToken().c_str());
		double y = atof(t.GetToken().c_str());
		double z = atof(t.GetToken().c_str());

		Vector3 vec(x, y, z);
		m_vn.push_back(vec);
	}
	if(line[0] == 'f' && line[1] == ' ')
	{
		line = &line[2];
		Tokenizer t_xyz(line, " ");

		string s_x = t_xyz.GetToken();
		string s_y = t_xyz.GetToken();
		string s_z = t_xyz.GetToken();

		Tokenizer fx(s_x, "/");
		int x_v = atoi(fx.GetToken().c_str());
		int x_vt = atoi(fx.GetToken().c_str());
		int x_vn = atoi(fx.GetToken().c_str());

		Tokenizer fy(s_y, "/");
		int y_v = atoi(fy.GetToken().c_str());
		int y_vt = atoi(fy.GetToken().c_str());
		int y_vn = atoi(fy.GetToken().c_str());

		Tokenizer fz(s_z, "/");
		int z_v = atoi(fz.GetToken().c_str());
		int z_vt = atoi(fz.GetToken().c_str());
		int z_vn = atoi(fz.GetToken().c_str());

		Point3 pnt_v(x_v - 1.0, y_v - 1.0, z_v - 1.0);
		m_face_v.push_back(pnt_v);

		if(x_vt != 0)
		{		
			Point3 pnt_vt(x_vt - 1.0, y_vt - 1.0, z_vt - 1.0);
			m_face_vt.push_back(pnt_vt);
		}
		if(x_vn != 0)
		{
			Vector3 pnt_vn(x_vn - 1.0, y_vn - 1.0, z_vn - 1.0);
			m_face_vn.push_back(pnt_vn);
		}
	}
}

#endif