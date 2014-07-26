#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <vector>
#include <stdlib.h>
using namespace std;

class Tokenizer
{
public:
	Tokenizer(string& strOrigin, const string& strTok);
	virtual ~Tokenizer() {};

public:
	size_t CountSize() { return tokens.size(); }
	string GetToken();

private:
	void Split(string& strOrigin, const string& strTok);

private:
	vector<string> tokens;
	size_t m_idx;
};


Tokenizer::Tokenizer(string& strOrigin, const string& strTok)
{
	Split(strOrigin, strTok);
	m_idx = 0;
}

void Tokenizer::Split(string& strOrigin, const string& strTok)
{
	// lastPos는 구분자가 나타나지 않는 첫번째 위치
	// Pos는 구분자가 나타나는 첫번째 위치
	// 문자열은 Pos-lastPos까지 해야함!
	//공백을 기준으로 자르고 싶을 때는 stringstream 이란 것이 있당!

	//     hi             1) lastPos = 0, Pos = 2
	//						2) tokens = "hi",   <End>
	//     hi/yu		1) lastPos = 0, Pos = 2
	//                     2) tokens = "hi",  lastPos = 3, Pos = 5(npos)
	//						3) tokens = "yu",  lastPos = 5(npos), Pos = 5(npos) <End>
	//     hi//yu      1) lastPos = 0, Pos = 2
	//						 2) tokens = "hi",  lastPos = 4, Pos = 6(npos)
	//						 3) tokens = "yu", lastPos = 6(npos), Pos = 6(npos) <End>
	string::size_type lastPos = strOrigin.find_first_not_of(strTok, 0);
	string::size_type Pos = strOrigin.find_first_of(strTok, lastPos);

	while(Pos != string::npos || lastPos != string::npos )
	{
		tokens.push_back(strOrigin.substr(lastPos, Pos-lastPos));
		lastPos = strOrigin.find_first_not_of(strTok, Pos);
		if((lastPos != Pos+1) && (lastPos != string::npos)) 
			tokens.push_back("");
		Pos = strOrigin.find_first_of(strTok, lastPos); 
	}
}

string Tokenizer::GetToken()
{
	if(m_idx < tokens.size())
		return tokens[m_idx++];

	return ""; 
}

//"" : NULL : '\0' : 0

#endif