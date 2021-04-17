#pragma once
#include<vector>
#include<memory>
#include<utility>
#include"Rectangle.h"

class Selection
{
	bool Active = true;
	std::vector<std::shared_ptr<Rectangle>> Rectangles;
public:
	Selection(const std::vector<std::shared_ptr<Rectangle>>&);
	bool GetActive()const;
	Selection& SetActive(const bool&);
	bool InSelected(const std::pair<int, int>&);
	std::vector<std::shared_ptr<Rectangle>>& GetRectangles();
};

