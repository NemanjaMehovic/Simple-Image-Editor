#pragma once
#include"Pixel.h"
#include<vector>
#include<memory>


class Layer
{
	std::vector<std::shared_ptr<Pixel>> Pixels;
	double Transparency = 100;
	bool Visible = true;
	bool Active = true;
public:
	static int width;
	static int height;
	Layer(const std::vector<std::shared_ptr<Pixel>>&);
	std::vector<std::shared_ptr<Pixel>>& GetPixels();
	double GetTransparency()const;
	Layer& SetTransparency(const double&);
	bool GetVisible()const;
	Layer& SetVisible(const bool&);
	bool GetActive()const;
	Layer& SetActive(const bool&);
	Layer& Resize(const int&);
};

