#include "Operations.h"
#include<vector>
#include<memory>
#include<map>
#include<string>
#include<algorithm>
#include<utility>
#include<cmath>
#include"Selection.h"
#include"Layer.h"
#include"Errors.h""
#include"rapidxml.hpp"
#include"rapidxml_iterators.hpp"
#include"rapidxml_print.hpp"
#include"rapidxml_utils.hpp"



std::vector<std::pair<std::string, std::shared_ptr<Operation>>> Operation::InitilalizerOperations()
{
	std::vector<std::pair<std::string, std::shared_ptr<Operation>>> tmpOperations;
	std::shared_ptr<Operation> Op;
	std::pair<std::string, std::shared_ptr<Operation>> P;
	Op = std::make_shared<OperationInverse>();
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationGrey>();
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationBlackAndWhite>();
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationAbs>();
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationBlur>();
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationLog>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationAdd>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationSub>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationInverseSub>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationMul>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationDiv>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationInverseDiv>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationPow>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationMin>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationMax>(1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<OperationFill>("",1,1,1);
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	Op = std::make_shared<CompositeOperation>("");
	P.first = Op->GetXml();
	P.second = Op;
	tmpOperations.push_back(P);
	return tmpOperations;
}

std::vector<std::pair<std::string, std::shared_ptr<Operation>>> Operation::Operations = Operation::InitilalizerOperations();

std::vector<std::string> CompositeOperation::UsedLocations = [] { std::vector<std::string> x; return x; }();

std::vector<std::shared_ptr<CompositeOperation>> CompositeOperation::CompositeOperations = [] {std::vector<std::shared_ptr<CompositeOperation>> x; return x; }();

std::shared_ptr<Operation> Operation::Factory(const std::string& OperationName)
{
	for (auto x : Operation::Operations)
		if (x.first.compare(OperationName) == 0)
			return x.second->GetCopy();
	return nullptr;
}

std::vector<std::shared_ptr<tmpPixel>> Operation::Execute(std::map<std::string, std::shared_ptr<Selection>>& Selections, std::vector<std::shared_ptr<tmpPixel>>& Pixels)const
{
	std::pair<int, int> Coordinate;
	Coordinate.first = Coordinate.second = 0;
	for (auto PixelPointer : Pixels)
	{
		if (std::any_of(Selections.begin(), Selections.end(), [&Coordinate](auto OneSelection) { return (OneSelection.second->GetActive() ? OneSelection.second->InSelected(Coordinate) : false); }))
		{
			Op(PixelPointer->R, PixelPointer->G, PixelPointer->B);
		}
		if (++Coordinate.first >= Layer::width)
		{
			Coordinate.first = 0;
			Coordinate.second++;
		}
	}
	return Pixels;
}

void OperationInverse::Op(int& R, int& G, int& B)const
{
	R = MaxValue - R;
	G = MaxValue - G;
	B = MaxValue - B;
}

void OperationGrey::Op(int& R, int& G, int& B)const
{
	R = G = B = (R + G + B) / 3;
}

void OperationBlackAndWhite::Op(int& R, int& G, int& B)const
{
	double avg = (R + G + B) / 3;
	if (avg > 127)
		R = G = B = MaxValue;
	else
		R = G = B = 0;
}

void OperationAbs::Op(int& R, int& G, int& B)const
{
	R = std::abs(R);
	G = std::abs(G);
	B = std::abs(B);
}

std::vector<std::shared_ptr<tmpPixel>> OperationBlur::Execute(std::map<std::string, std::shared_ptr<Selection>>& Selections, std::vector<std::shared_ptr<tmpPixel>>& Pixels)const
{
	std::pair<int, int> Coordinate;
	std::vector<std::shared_ptr<tmpPixel>> BluredPixels;
	Coordinate.first = Coordinate.second = 0;
	for (auto PixelPointer : Pixels)
	{
		if (std::any_of(Selections.begin(), Selections.end(), [&Coordinate](auto OneSelection) { return (OneSelection.second->GetActive() ? OneSelection.second->InSelected(Coordinate) : false); }))
		{
			std::vector<std::shared_ptr<tmpPixel>> tmpPixels;
			tmpPixels.push_back(PixelPointer);
			if ((Coordinate.second + 1) < Layer::height)
				tmpPixels.push_back(Pixels[Coordinate.first + (Coordinate.second + 1) * Layer::width]);
			if ((Coordinate.second - 1) >= 0)
				tmpPixels.push_back(Pixels[Coordinate.first + (Coordinate.second - 1) * Layer::width]);
			if ((Coordinate.first + 1) < Layer::width)
			{
				tmpPixels.push_back(Pixels[Coordinate.first + 1 + Coordinate.second * Layer::width]);
				if ((Coordinate.second + 1) < Layer::height)
					tmpPixels.push_back(Pixels[Coordinate.first + 1 + (Coordinate.second + 1) * Layer::width]);
				if ((Coordinate.second - 1) >= 0)
					tmpPixels.push_back(Pixels[Coordinate.first + 1 + (Coordinate.second - 1) * Layer::width]);
			}
			if ((Coordinate.first - 1) >= 0)
			{
				tmpPixels.push_back(Pixels[Coordinate.first - 1 + Coordinate.second * Layer::width]);
				if ((Coordinate.second + 1) < Layer::height)
					tmpPixels.push_back(Pixels[Coordinate.first - 1 + (Coordinate.second + 1) * Layer::width]);
				if ((Coordinate.second - 1) >= 0)
					tmpPixels.push_back(Pixels[Coordinate.first - 1 + (Coordinate.second - 1) * Layer::width]);
			}
			int SumR, SumB, SumG;
			SumB = SumG = SumR = 0;
			for (auto TmpPixelPointer : tmpPixels)
			{
				SumB += TmpPixelPointer->B;
				SumR += TmpPixelPointer->R;
				SumG += TmpPixelPointer->G;
			}
			BluredPixels.push_back(std::make_shared<tmpPixel>(SumR / tmpPixels.size(), SumG / tmpPixels.size(), SumB / tmpPixels.size()));
		}
		else
			BluredPixels.push_back(PixelPointer);
		if (++Coordinate.first >= Layer::width)
		{
			Coordinate.first = 0;
			Coordinate.second++;
		}
	}
	return BluredPixels;
}

OperationLog::OperationLog(const double& x)
{
	if (x > 0)
		base = x;
	else
		base = 1;
}

void OperationLog::Op(int& R, int& G, int& B)const
{
	R = std::log(R) / std::log(base);
	G = std::log(G) / std::log(base);
	B = std::log(B) / std::log(base);
}

OperationAdd::OperationAdd(const int& x)
{
	Sum = x;
}

void OperationAdd::Op(int& R, int& G, int& B)const
{
	R += Sum;
	G += Sum;
	B += Sum;
}

OperationSub::OperationSub(const int& x)
{
	Sub = x;
}

void OperationSub::Op(int& R, int& G, int& B)const
{
	R -= Sub;
	G -= Sub;
	B -= Sub;
}

OperationInverseSub::OperationInverseSub(const int& x)
{
	Sub = x;
}

void OperationInverseSub::Op(int& R, int& G, int& B)const
{
	R = Sub - R;
	G = Sub - G;
	B = Sub - B;
}

OperationMul::OperationMul(const int& x)
{
	Mul = x;
}

void OperationMul::Op(int& R, int& G, int& B)const
{
	R *= Mul;
	G *= Mul;
	B *= Mul;
}

OperationDiv::OperationDiv(const int& x)
{
	if (x != 0)
		Div = x;
	else
		Div = 1;
}

void OperationDiv::Op(int& R, int& G, int& B)const
{
	R /= Div;
	G /= Div;
	B /= Div;
}

OperationInverseDiv::OperationInverseDiv(const int& x)
{
	Div = x;
}

void OperationInverseDiv::Op(int& R, int& G, int& B)const
{
	if (R != 0 && G != 0 && B != 0)
	{
		R = Div / R;
		G = Div / G;
		B = Div / B;
	}
}

OperationPow::OperationPow(const double& x)
{
	Pow = x;
}

void OperationPow::Op(int& R, int& G, int& B)const
{
	R = std::pow(R, Pow);
	G = std::pow(G, Pow);
	B = std::pow(B, Pow);
}

OperationMin::OperationMin(const int& x)
{
	Min = x;
}

void OperationMin::Op(int& R, int& G, int& B)const
{
	if (R > Min)
		R = Min;
	if (G > Min)
		G = Min;
	if (B > Min)
		B = Min;
}

OperationMax::OperationMax(const int& x)
{
	Max = x;
}

void OperationMax::Op(int& R, int& G, int& B)const
{
	if (R < Max)
		R = Max;
	if (G < Max)
		G = Max;
	if (B < Max)
		B = Max;
}

OperationFill::OperationFill(const std::string& name, const int& R, const int& G, const int& B)
{
	this->R = R;
	this->G = G;
	this->B = B;
	this->name = name;
}

std::vector<std::shared_ptr<tmpPixel>> OperationFill::Execute(std::map<std::string, std::shared_ptr<Selection>>& Selections, std::vector<std::shared_ptr<tmpPixel>>& Pixels)const
{
	if (Selections.find(name) != Selections.end())
		if (Selections.find(name)->second->GetActive())
		{
			int i = 0, j = 0;
			for (auto PixelPointer : Pixels)
			{
				std::pair<int, int> x(j, i);
				if (Selections[name]->InSelected(x))
				{
					PixelPointer->R = R;
					PixelPointer->G = G;
					PixelPointer->B = B;
				}
				if (++j >= Layer::width)
				{
					j = 0;
					i++;
				}
			}
		}
	return Pixels;
}

std::vector<std::shared_ptr<tmpPixel>> CompositeOperation::Execute(std::map<std::string, std::shared_ptr<Selection>>& Selections, std::vector<std::shared_ptr<tmpPixel>>& Pixels)const
{
	for (auto x : Operations)
		Pixels = x->Execute(Selections, Pixels);
	return Pixels;
}

CompositeOperation::CompositeOperation(const std::vector<std::shared_ptr<Operation>>& Operations)
{
	this->Operations = Operations;
}

CompositeOperation::CompositeOperation(const std::string& Location)
{
	this->Location = Location;
	if (Location.compare("") != 0)
		LoadXml();
}

void CompositeOperation::Set(const std::pair<std::vector<double>, std::string>& x)
{
	this->Location = x.second;
	if (Location.compare("") != 0)
		LoadXml();
}

std::shared_ptr<std::pair<std::vector<double>, std::string>> CompositeOperation::Get()
{
	std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared < std::pair<std::vector<double>, std::string>>();
	x->second = Location;
	return x;
}

void CompositeOperation::LoadXml()
{
	try
	{
		static int i = 0;
		if (Location.substr(Location.find_last_of('.')).compare(".fun") != 0)
			throw ExceptionFileType();
		for (auto x : UsedLocations)
			if (Location.compare(x) == 0)
				return;
		i++;
		CompositeOperation::UsedLocations.push_back(Location);
		std::ifstream file(Location);
		std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(&buffer[0]);
		rapidxml::xml_node<>* Root = Doc.first_node();
		for (rapidxml::xml_node<>* OperationNode = Root->first_node(); OperationNode; OperationNode = OperationNode->next_sibling())
		{
			std::shared_ptr<Operation> OperationPointer;
			std::string Name;
			std::pair<std::vector<double>, std::string> Var;
			rapidxml::xml_node<>* OperationName = OperationNode->first_node();
			Name = OperationName->value();
			OperationPointer = Operation::Factory(Name);
			for (rapidxml::xml_node<>* OperationVar = OperationName->next_sibling(); OperationVar; OperationVar = OperationVar->next_sibling())
			{
				std::string check = OperationVar->name();
				if (check.compare("StrVar")==0)
					Var.second = OperationVar->value();
				else
					Var.first.push_back(std::stod(OperationVar->value()));
			}
			OperationPointer->Set(Var);
			Operations.push_back(OperationPointer);
		}
		Doc.clear();
		i--;
		if (i == 0)
			CompositeOperation::UsedLocations.clear();
	}
	catch (ExceptionFileType & e)
	{
		std::cout << e.what() << std::endl;
	}
}

void CompositeOperation::SaveXml(const std::string& Location)
{
	std::string Loc;
	if (Location.find_first_of('.') == std::string::npos)
		Loc = Location + ".fun";
	else if (Location.substr(Location.find_last_of('.')).compare(".fun") != 0)
		Loc = Location + ".fun";
	else
		Loc = Location;
	rapidxml::xml_document<> Doc;
	rapidxml::xml_node<>* Root = Doc.allocate_node(rapidxml::node_element, "Function");
	for (auto OperationPointer : Operations)
	{
		rapidxml::xml_node<>* OperationNode = Doc.allocate_node(rapidxml::node_element, "Operation");
		rapidxml::xml_node<>* OperationName = Doc.allocate_node(rapidxml::node_element, "OperationName");
		OperationName->value(Doc.allocate_string(OperationPointer->GetXml().c_str(),OperationPointer->GetXml().size()));
		OperationNode->append_node(OperationName);
		auto Variables = OperationPointer->Get();
		if (Variables != nullptr)
		{
			if (Variables->second.compare("") != 0)
			{
				rapidxml::xml_node<>* OperationString = Doc.allocate_node(rapidxml::node_element, "StrVar");
				OperationString->value(Doc.allocate_string(Variables->second.c_str(), Variables->second.size()));
				OperationNode->append_node(OperationString);
			}
			if (Variables->first.size() != 0)
			{
				for (auto VarInt : Variables->first)
				{
					rapidxml::xml_node<>* OperationVar = Doc.allocate_node(rapidxml::node_element, "Var");
					OperationVar->value(Doc.allocate_string(std::to_string(VarInt).c_str()));
					OperationNode->append_node(OperationVar);
				}
			}
		}
		Root->append_node(OperationNode);
	}
	Doc.append_node(Root);
	std::string xml;
	std::ofstream file(Loc);
	if (!file)
	{
		Doc.clear();
		throw ExceptionFileOpen();
	}
	rapidxml::print(std::back_inserter(xml), Doc);
	file << xml;
	file.close();
	Doc.clear();
}