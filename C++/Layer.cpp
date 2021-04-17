#include "Layer.h"
#include"Errors.h"

int Layer::width = 0;
int Layer::height = 0;

Layer::Layer(const std::vector<std::shared_ptr<Pixel>>& Pixels)
{
	this->Pixels = Pixels;
}

std::vector<std::shared_ptr<Pixel>>& Layer::GetPixels()
{
	return Pixels;
}

double Layer::GetTransparency()const
{
	return Transparency;
}

Layer& Layer::SetTransparency(const double& Transparency)
{
	try 
	{
		if (Transparency > 100 || Transparency < 0)
			throw ExceptionTransparency();
		this->Transparency = Transparency;
		return *this;
	}
	catch (ExceptionTransparency & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

bool Layer::GetVisible()const
{
	return Visible;
}

Layer& Layer::SetVisible(const bool& Visible)
{
	this->Visible = Visible;
	return *this;
}

bool Layer::GetActive()const
{
	return Active;
}

Layer& Layer::SetActive(const bool& Active)
{
	this->Active = Active;
	return *this;
}

Layer& Layer::Resize(const int& OldWidth)
{
	std::vector<std::shared_ptr<Pixel>> tmp;
	int i = 0, j = 0;
	for (auto x : Pixels)
	{
		tmp.push_back(x);
		j++;
		if (j >= OldWidth)
		{
			while (j < Layer::width)
			{
				tmp.push_back(std::make_shared<Pixel>(0, 0, 0, 0));
				j++;
			}
			j = 0;
			i++;
		}
	}
	while (i < Layer::height)
	{
		tmp.push_back(std::make_shared<Pixel>(0, 0, 0, 0));
		j++;
		if (j >= Layer::width)
		{
			i++;
			j = 0;
		}
	}
	Pixels = tmp;
	return *this;
}