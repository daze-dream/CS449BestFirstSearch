#pragma once

#include <string>

using namespace std;
class node
{
public:
	node();
	node(string n, float x, float y, float d);
	void toggleSearch();
	void setData(string n, float x, float y , float d);
	float x =0, y=0;
	float dist;
	string name;
	bool isSearched = 0;
	void displayState();
private:
	

};
