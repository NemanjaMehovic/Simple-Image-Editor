#pragma once
#include<iostream>

using color = unsigned char;
const color MaxValue = 255;
const color MinValue = 0;

class Pixel
{
	color R;
	color G;
	color B;
	color A;
public:
	Pixel(const color&, const color&, const color&, const color&);
	const color GetRed()const;
	const color GetGreen()const;
	const color GetBlue()const;
	const color GetAlpha()const;
	Pixel& SetRed(const color&);
	Pixel& SetGreen(const color&);
	Pixel& SetBlue(const color&);
	Pixel& SetAlpha(const color&);
	friend std::ostream& operator<<(std::ostream&, const Pixel&);
};