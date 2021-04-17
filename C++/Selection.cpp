#include "Selection.h"
#include"Rectangle.h"
#include<vector>
#include<algorithm>
#include<memory>
#include<utility>


Selection::Selection(const std::vector<std::shared_ptr<Rectangle>>& Rectangles)
{
	this->Rectangles = Rectangles;
}

bool Selection::GetActive()const
{
	return Active;
}

Selection& Selection::SetActive(const bool& Active)
{
	this->Active = Active;
	return *this;
}

bool Selection::InSelected(const std::pair<int, int>& Coordinate)
{
	return std::any_of(Rectangles.begin(), Rectangles.end(), [&Coordinate](auto x)
		{
			return
				(
					Coordinate.first <= x->GetBottomRight().first &&
					Coordinate.first >= x->GetTopLeft().first &&
					Coordinate.second >= x->GetBottomRight().second &&
					Coordinate.second <= x->GetTopLeft().second
					);
		});
}

std::vector<std::shared_ptr<Rectangle>>& Selection::GetRectangles()
{
	return Rectangles;
}