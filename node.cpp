#include "node.h"
#include <iostream>

node::node()
{
	name = "blank";
	x = 0;
	y = 0;
	dist = 0;
}

node::node(string n, float x, float y, float d)
{
	this->name = n;
	this->x = x;
	this->y = y;
	this->dist = d;
}

void node::toggleSearch()
{
	if (this->isSearched == 0)
	{
		this->isSearched = 1;
	}
	else
	{
		this->isSearched = 0;
	}
}

void node::setData(string n, float x, float y, float d)
{
	this->name = n;
	this->x = x;
	this->y = y;
	this->dist = d;

}

void node::displayState()
{
	cout << "Current Node:" << this->name << endl;
	cout << "X: " << x << "Y: " << y << endl;
	cout << "Dist to Goal: " << dist << endl << endl;
}
