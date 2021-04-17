#include<iostream>
#include<utility>
#include "Rectangle.h"
#include"Errors.h"

void Rectangle::Check(std::pair<int, int>& Coordinate)
{
	if (Coordinate.first >= Layer::width)
		Coordinate.first = Layer::width - 1;
	if (Coordinate.second >= Layer::height)
		Coordinate.second = Layer::height - 1;
	if (Coordinate.first < 0)
		Coordinate.first = 0;
	if (Coordinate.second < 0)
		Coordinate.second = 0;
}

Rectangle::Rectangle(const std::pair<int, int>& TopLeft, const int& width, const int& height)
{
	try
	{
		if (Layer::width == 0 || Layer::height == 0)
			throw ExceptionPictureDoesntExist();
		this->TopLeft = TopLeft;
		Width = width;
		Height = height;
		Check(this->TopLeft);
		BottomRight.first = this->TopLeft.first + width;
		BottomRight.second = this->TopLeft.second - height;
		Check(BottomRight);
	}
	catch (ExceptionPictureDoesntExist & e)
	{
		std::cout << e.what() << std::endl;
	}
}

std::pair<int, int> Rectangle::GetTopLeft()const
{
	return TopLeft;
}

std::pair<int, int> Rectangle::GetBottomRight()const
{
	return BottomRight;
}

int Rectangle::GetWidth()const
{
	return Width;
}

int Rectangle::GetHeight()const
{
	return Height;
}