#include "Picture.h"
#include"Layer.h"
#include"Selection.h"
#include"Rectangle.h"
#include"Errors.h"
#include"Formatter.h"
#include<memory>
#include<vector>
#include<map>
#include<string>
#include<iostream>
#include<cmath>


std::shared_ptr<Picture> Picture::Pic = nullptr;

Picture::Picture(const int& width, const int& height)
{
	if (width < 1)
		Layer::width = 1;
	else
		Layer::width = width;
	if (height < 1)
		Layer::height = height;
	else
		Layer::height = height;
	CreateNewLayer();
}

std::shared_ptr<Picture> Picture::GetInstance()
{
	return Picture::Pic;
}

std::shared_ptr<Picture> Picture::GetInstance(const int& width, const int& height)
{
	if (Pic == nullptr)
		Pic = std::make_shared<Picture>(Picture(width, height));
	return Pic;
}

std::shared_ptr<Picture> Picture::GetInstance(const Formatter& Format, const std::string& Location)
{
	if (Pic == nullptr)
	{
		Pic = std::make_shared<Picture>(Picture(0, 0));
		Pic->CreateNewLayer(Format, Location);
		Pic->DeleteLayer(1);
	}
	return Pic;
}

void Picture::DeleteInstance()
{
	Pic = nullptr;
}

Picture& Picture::CreateNewLayer()
{
	std::vector<std::shared_ptr<Pixel>> tmp;
	for (int i = 0; i < Layer::height * Layer::width; i++)
		tmp.push_back(std::make_shared<Pixel>(0, 0, 0, 255));
	Layers.push_back(std::make_shared<Layer>(tmp));
	Saved = false;
	return *this;
}

Picture& Picture::CreateNewLayer(const Formatter& Format, const std::string& Location)
{
	try
	{
		int OldWidth = Layer::width;
		int OldHeight = Layer::height;
		std::shared_ptr<Layer> tmpLayer = Format.Read(Location);
		if(Location.substr(Location.find_last_of('.')).compare(".mn") != 0)
			if (OldWidth != Layer::width || OldHeight != Layer::height)
				for (auto x : Layers)
					x->Resize(OldWidth);
		Layers.push_back(tmpLayer);
		Saved = false;
		return *this;
	}
	catch(ExceptionFileOpen& e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
	catch(ExceptionFileType& e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::SetVisibleLayer(const int& x, const bool& y)
{
	try
	{
		if (x < 1 || x > Layers.size())
			throw ExceptionLayerDoesntExists();
		this->Layers[x-1]->SetVisible(y);
		Saved = false;
		return *this;
	}
	catch (ExceptionLayerDoesntExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::SetActiveLayer(const int& x, const bool& y)
{
	try
	{
		if (x < 1 || x > Layers.size())
			throw ExceptionLayerDoesntExists();
		this->Layers[x-1]->SetActive(y);
		Saved = false;
		return *this;
	}
	catch (ExceptionLayerDoesntExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::SetTransparencyLayer(const int& x, const double& y)
{
	try
	{
		if (x < 1 || x > Layers.size())
			throw ExceptionLayerDoesntExists();
		this->Layers[x-1]->SetTransparency(y);
		Saved = false;
		return *this;
	}
	catch (ExceptionLayerDoesntExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::DeleteLayer(const int& x)
{
	try
	{
		if (x < 1 || x > Layers.size())
			throw ExceptionLayerDoesntExists();
		if (Layers.size() > 1)
		{
			int i = 0;
			auto it = Layers.begin();
			while (i != x)
			{
				if (i == (x - 1))
					Layers.erase(it);
				else
					it++;
				i++;
			}
		}
		Saved = false;
		return *this;
	}
	catch (ExceptionLayerDoesntExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::CreateNewSelection(const std::string& name, const std::vector<std::shared_ptr<Rectangle>>& Rectangles)
{
	try
	{
		std::shared_ptr<Selection> x = std::make_shared<Selection>(Rectangles);
		std::pair<std::string, std::shared_ptr<Selection>> z(name,x);
		if (Selections.insert(z).second == false)
			throw ExceptionSelectionAlreadyExists();
		Saved = false;
		return *this;
	}
	catch (ExceptionSelectionAlreadyExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::SetSelectionActive(const std::string& name, const bool& active)
{
	try
	{
		if (Selections.find(name) == Selections.end())
			throw ExceptionSelectionDoesntExists();
		Selections[name]->SetActive(active);
		Saved = false;
		return *this;
	}
	catch (ExceptionSelectionDoesntExists & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}

Picture& Picture::DeleteSelection(const std::string& name)
{
	if (Selections.find(name) != Selections.end())
		Selections.erase(name);
	Saved = false;
	return *this;
}

std::vector<std::shared_ptr<Layer>>& Picture::GetLayers()
{
	return Layers;
}

std::map<std::string, std::shared_ptr<Selection>>& Picture::GetSelections()
{
	return Selections;
}

Layer Picture::GetFinished()const
{
	return *Finished;
}

bool Picture::GetSaved()const
{
	return Saved;
}

void Picture::Check(tmpPixel& x)const
{
	if (x.B > MaxValue)
		x.B = MaxValue;
	else if (x.B < 0)
		x.B = 0;
	if (x.R > MaxValue)
		x.R = MaxValue;
	else if (x.R < 0)
		x.R = 0;
	if (x.G > MaxValue)
		x.G = MaxValue;
	else if (x.G < 0)
		x.G = 0;
}

Picture& Picture::Execute(const Operation& Op)
{
	bool flag=false;
	if (Selections.size() < 1)
	{
		std::vector<std::shared_ptr<Rectangle>> b;
		b.push_back(std::make_shared<Rectangle>(std::make_pair<int, int>(0, Layer::height - 1), Layer::width - 1, Layer::height - 1));
		CreateNewSelection("Picture", b);
		flag = true;
	}
	for (auto LayerPointer : Layers)
	{
		if (LayerPointer->GetActive())
		{
			std::vector<std::shared_ptr<tmpPixel>> x;
			for (auto PixelPointer : LayerPointer->GetPixels())
				x.push_back(std::make_shared<tmpPixel>(PixelPointer->GetRed(), PixelPointer->GetGreen(), PixelPointer->GetBlue()));
			x=Op.Execute(Selections, x);
			int i = 0;
			for (auto tmpPixelPointer : x)
			{
				Check(*tmpPixelPointer);
				LayerPointer->GetPixels()[i++]->SetRed(tmpPixelPointer->R).SetGreen(tmpPixelPointer->G).SetBlue(tmpPixelPointer->B);
			}

		}
	}
	if (flag)
		Selections.clear();
	Saved = false;
	return *this;
}

void Picture::CreateFinished()
{
	std::vector<std::shared_ptr<Pixel>> tmp;
	for (int i = 0; i < Layer::height * Layer::width; i++)
		tmp.push_back(std::make_shared<Pixel>(0, 0, 0, 0));
	Finished = std::make_shared<Layer>(tmp);
	tmp.clear();
	for (int i = 0; i < Layer::height * Layer::width; i++)
	{
		for (auto LayerPointer : Layers)
			if (LayerPointer->GetVisible())
			{
				double tmpA0 = Finished->GetPixels()[i]->GetAlpha() / 255.0;
				double tmpA1 = (LayerPointer->GetTransparency() / 100.0) * (LayerPointer->GetPixels()[i]->GetAlpha() / 255.0);
				double tmpA = tmpA0 + (1 - tmpA0) * tmpA1;
				double tmpR = std::round(Finished->GetPixels()[i]->GetRed() * tmpA0 / tmpA + LayerPointer->GetPixels()[i]->GetRed() * (1 - tmpA0) * tmpA1 / tmpA);
				double tmpG = std::round(Finished->GetPixels()[i]->GetGreen() * tmpA0 / tmpA + LayerPointer->GetPixels()[i]->GetGreen() * (1 - tmpA0) * tmpA1 / tmpA);
				double tmpB = std::round(Finished->GetPixels()[i]->GetBlue() * tmpA0 / tmpA + LayerPointer->GetPixels()[i]->GetBlue() * (1 - tmpA0) * tmpA1 / tmpA);
				Finished->GetPixels()[i]->SetAlpha(std::round(tmpA * 255)).SetRed(tmpR).SetGreen(tmpG).SetBlue(tmpB);
			}
	}
}

Picture& Picture::Save(const Formatter& Format, const std::string& Location)
{
	try
	{
		CreateFinished();
		Format.Save(Location, Finished);
		Saved = true;
		return *this;
	}
	catch (ExceptionFileOpen & e)
	{
		std::cout << e.what() << std::endl;
		return *this;
	}
}