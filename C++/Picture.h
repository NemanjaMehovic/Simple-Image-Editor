#pragma once
#include"Pixel.h"
#include"Layer.h"
#include"Selection.h"
#include"Rectangle.h"
#include"Operations.h"
#include"Formatter.h"
#include<memory>
#include<vector>
#include<map>
#include<string>


class Picture
{
	void Check(tmpPixel&)const;
	std::shared_ptr<Layer> Finished;
	std::vector<std::shared_ptr<Layer>> Layers;
	std::map<std::string,std::shared_ptr<Selection>> Selections;
	bool Saved = false;
	void CreateFinished();
	Picture(const int&, const int&);
	static std::shared_ptr<Picture> Pic;
public:
	static std::shared_ptr<Picture> GetInstance();
	static std::shared_ptr<Picture> GetInstance(const int&, const int&);
	static std::shared_ptr<Picture> GetInstance(const Formatter&, const std::string&);
	static void DeleteInstance();
	Picture& SetVisibleLayer(const int&, const bool&);
	Picture& SetActiveLayer(const int&, const bool&);
	Picture& SetTransparencyLayer(const int&,const double&);
	Picture& CreateNewLayer();
	Picture& CreateNewLayer(const Formatter&, const std::string&);
	Picture& DeleteLayer(const int&);
	Picture& CreateNewSelection(const std::string&, const std::vector<std::shared_ptr<Rectangle>>&);
	Picture& SetSelectionActive(const std::string&, const bool&);
	Picture& DeleteSelection(const std::string&);
	std::vector<std::shared_ptr<Layer>>& GetLayers();
	std::map<std::string, std::shared_ptr<Selection>>& GetSelections();
	Layer GetFinished()const;
	bool GetSaved()const;
	Picture& Execute(const Operation&);
	Picture& Save(const Formatter&, const std::string&);
};

