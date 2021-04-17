#include"Pixel.h"
#include<iostream>

Pixel::Pixel(const color& R, const color& G, const color& B, const color& A)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->A = A;
}

const color Pixel::GetRed()const
{
	return R;
}
const color Pixel::GetGreen()const
{
	return G;
}
const color Pixel::GetBlue()const
{
	return B;
}
const color Pixel::GetAlpha()const
{
	return A;
}

Pixel& Pixel::SetRed(const color& R)
{
	if (R > MaxValue || R < MinValue)
		return *this;
	this->R = R;
	return *this;
}
Pixel& Pixel::SetGreen(const color& G)
{
	if (G > MaxValue || G < MinValue)
		return *this;
	this->G = G;
	return *this;
}
Pixel& Pixel::SetBlue(const color& B)
{
	if (B > MaxValue || B < MinValue)
		return *this;
	this->B = B;
	return *this;
}
Pixel& Pixel::SetAlpha(const color& A)
{
	if (A > MaxValue || A < MinValue)
		return *this;
	this->A = A;
	return *this;
}

std::ostream& operator<<(std::ostream& out, const Pixel& P)
{
	out <<static_cast<int>(P.R )<< " " << static_cast<int>(P.G) << " " << static_cast<int>(P.B) << " " << static_cast<int>(P.A);
	return out;
}