#pragma once
#include<utility>
#include"Layer.h"


class Rectangle
{
	std::pair<int, int> TopLeft;
	std::pair<int, int> BottomRight;
	int Width = 0, Height = 0;
	void Check(std::pair<int, int>&);
public:
	Rectangle(const std::pair<int, int>&, const int&, const int&);
	std::pair<int, int> GetTopLeft()const;
	std::pair<int, int> GetBottomRight()const;
	int GetWidth()const;
	int GetHeight()const;
};

