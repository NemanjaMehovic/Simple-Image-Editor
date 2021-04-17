#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include"Formatter.h"
#include"Layer.h"
#include"Pixel.h"
#include"Errors.h"
#include"Picture.h"
#include"Selection.h"
#include"Rectangle.h"
#include"Operations.h"
#include<fstream>
#include<memory>
#include<vector>
#include<string>
#include<cstdlib>
#include<algorithm>
#include<experimental/filesystem>
#include<regex>
#include"rapidxml.hpp"
#include"rapidxml_iterators.hpp"
#include"rapidxml_print.hpp"
#include"rapidxml_utils.hpp"


std::vector<std::pair<std::string, std::shared_ptr<Formatter>>> Formatter::InitilalizerFormats()
{
	std::vector<std::pair<std::string, std::shared_ptr<Formatter>>> tmp;
	std::pair<std::string, std::shared_ptr<Formatter>> tmpPair;
	tmpPair.first = ".bmp";
	tmpPair.second = std::make_shared<BMP>();
	tmp.push_back(tmpPair);
	tmpPair.first = ".pam";
	tmpPair.second = std::make_shared<PAM>();
	tmp.push_back(tmpPair);
	tmpPair.first = ".mn";
	tmpPair.second = std::make_shared<MyFormat>();
	tmp.push_back(tmpPair);
	return tmp;
}

std::vector<std::pair<std::string, std::shared_ptr<Formatter>>> Formatter::Formats = InitilalizerFormats();

std::shared_ptr<Formatter> Formatter::Factory(const std::string& extension)
{
	try
	{
		for (auto x : Formats)
			if (x.first.compare(extension) == 0)
				return x.second;
		throw ExceptionFileType();
	}
	catch (ExceptionFileType & e)
	{
		std::cout << e.what() << std::endl;
		return nullptr;
	}
}

std::shared_ptr<Layer> BMP::Read(const std::string& Location)const
{
	std::shared_ptr<Layer> tmpLayer;
	std::ifstream file(Location, std::ios::in | std::ios::binary);
	if (!file)
		throw ExceptionFileOpen();
	bool flag1 = true, flag2 = true;
	int tmpWidth = 0, tmpHeight = 0, offset = 0;
	char checkB, checkM;
	file.read(&checkB, sizeof(char));
	file.read(&checkM, sizeof(char));
	if (checkB != 'B' || checkM != 'M')
		throw ExceptionFileType();
	file.seekg(10, std::ios::beg);
	file.read((char*)&offset, sizeof(int));
	file.seekg(18, std::ios::beg);
	file.read((char*)&tmpWidth, sizeof(int));
	file.read((char*)&tmpHeight, sizeof(int));
	file.seekg(28, std::ios::beg);
	char BitsPerPixel = 0;
	file.read((char*)&BitsPerPixel, sizeof(char));
	if (BitsPerPixel > 24)
		tmpLayer = std::make_shared<Layer>(BMP32(file, tmpWidth, tmpHeight, offset));
	else
		tmpLayer = std::make_shared<Layer>(BMP24(file, tmpWidth, tmpHeight, offset));
	if (tmpWidth >= Layer::width)
	{
		Layer::width = tmpWidth;
		flag1 = false;
	}
	if (tmpHeight >= Layer::height)
	{
		Layer::height = tmpHeight;
		flag2 = false;
	}
	if (flag1 || flag2)
		tmpLayer->Resize(tmpWidth);
	file.close();
	return tmpLayer;
}

std::vector<std::shared_ptr<Pixel>> BMP::BMP24(std::ifstream& file, const int& width, const int& height, const int& offset)const
{
	file.seekg(offset, std::ios::beg);
	std::vector<std::shared_ptr<Pixel>> tmpPixels;
	int padding = 0;
	while ((width * 3 + padding) % 4 != 0)
		padding++;
	for (int i = 0; i < height; i++)
	{
		char tmpR, tmpG, tmpB;
		for (int j = 0; j < width; j++)
		{
			file.read(&tmpB, sizeof(char));
			file.read(&tmpG, sizeof(char));
			file.read(&tmpR, sizeof(char));
			tmpPixels.push_back(std::make_shared<Pixel>(tmpR, tmpG, tmpB, 255));
		}
		for (tmpG = 0; tmpG < padding; tmpG++)
			file.read(&tmpR, sizeof(char));
	}
	return tmpPixels;
}

std::vector<std::shared_ptr<Pixel>> BMP::BMP32(std::ifstream& file, const int& width, const int& height, const int& offset)const
{
	char x[4];
	//mask starts at position 54
	file.seekg(54, std::ios::beg);
	//i=0 => R i=1 => G i=2 => B i=3 => A
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			char tmp;
			file.read(&tmp, sizeof(char));
			if (tmp != 0)
				x[j] = i;
		}
	file.seekg(offset, std::ios::beg);
	std::vector<std::shared_ptr<Pixel>> tmpPixels;
	for (int i = 0; i < height; i++)
	{
		char tmpR, tmpG, tmpB, tmpA;
		for (int j = 0; j < width; j++)
		{
			for (int z = 0; z < 4; z++)
				switch (x[z])
				{
				case 0:file.read(&tmpR, sizeof(char)); break;
				case 1:file.read(&tmpG, sizeof(char)); break;
				case 2:file.read(&tmpB, sizeof(char)); break;
				case 3:file.read(&tmpA, sizeof(char)); break;
				}
			tmpPixels.push_back(std::make_shared<Pixel>(tmpR, tmpG, tmpB, tmpA));
		}
	}
	return tmpPixels;
}

void BMP::Save(const std::string& Location, std::shared_ptr<Layer>& Finished)const
{
	std::string Loc;
	if (Location.find_first_of('.') == std::string::npos)
		Loc = Location + ".bmp";
	else if (Location.substr(Location.find_last_of('.')).compare(".bmp") != 0)
		Loc = Location + ".bmp";
	else
		Loc = Location;
	std::ofstream file(Loc, std::ios::out | std::ios::binary);
	if (!file)
		throw ExceptionFileOpen();
	Header(file);
	for (auto x : Finished->GetPixels())
	{
		file.put(x->GetRed());
		file.put(x->GetGreen());
		file.put(x->GetBlue());
		file.put(x->GetAlpha());
	}
}

void BMP::Header(std::ofstream& file)const
{
	const std::string BM = "BM";
	const int Zero = 0;
	const int SizeData = Layer::width * Layer::height * 4;
	const int Size = SizeData + 122;
	const int Offset = 122;
	const int HeaderSize = 108;
	const int BitColor = 0x200001;
	const int BiBitFields = 3;
	const int PrintResolution = 2835;
	const int MaskA = 0xff000000;
	const int MaskB = 0x00ff0000;
	const int MaskG = 0x0000ff00;
	const int MaskR = 0x000000ff;
	const int LittleEndian = 0x57696e20;
	file.write(BM.c_str(), BM.size());
	file.write((char*)&Size, sizeof(int));
	file.write((char*)&Zero, sizeof(int));
	file.write((char*)&Offset, sizeof(int));
	file.write((char*)&HeaderSize, sizeof(int));
	file.write((char*)&Layer::width, sizeof(int));
	file.write((char*)&Layer::height, sizeof(int));
	file.write((char*)&BitColor, sizeof(int));
	file.write((char*)&BiBitFields, sizeof(int));
	file.write((char*)&SizeData, sizeof(int));
	file.write((char*)&PrintResolution, sizeof(int));
	file.write((char*)&PrintResolution, sizeof(int));
	for (int i = 0; i < 2; i++)
		file.write((char*)&Zero, sizeof(int));
	file.write((char*)&MaskR, sizeof(int));
	file.write((char*)&MaskG, sizeof(int));
	file.write((char*)&MaskB, sizeof(int));
	file.write((char*)&MaskA, sizeof(int));
	file.write((char*)&LittleEndian, sizeof(int));
	for (int i = 0; i < 12; i++)
		file.write((char*)&Zero, sizeof(int));
}

std::shared_ptr<Layer> PAM::Read(const std::string& Location)const
{
	std::shared_ptr<Layer> tmpLayer;
	std::ifstream file(Location);
	if (!file)
		throw ExceptionFileOpen();
	std::string tmpString[7];
	int offset = 0;
	for (int i = 0; i < 7; i++)
	{
		std::getline(file, tmpString[i]);
		offset += tmpString[i].size() + 1;
	}
	if (tmpString[0].compare("P7") != 0)
		throw ExceptionFileType();
	file.close();
	file.open(Location, std::ios::in | std::ios::binary);
	int width = std::atoi(tmpString[1].substr(tmpString[1].find(" ")).c_str());
	int height = std::atoi(tmpString[2].substr(tmpString[2].find(" ")).c_str());
	int depth = std::atoi(tmpString[3].substr(tmpString[3].find(" ")).c_str());
	if (depth > 3)
		tmpLayer = std::make_shared<Layer>(RGBA(file, width, height, offset));
	else
		tmpLayer = std::make_shared<Layer>(RGB(file, width, height, offset));
	bool flag1 = true, flag2 = true;
	if (width >= Layer::width)
	{
		Layer::width = width;
		flag1 = false;
	}
	if (height >= Layer::height)
	{
		Layer::height = height;
		flag2 = false;
	}
	if (flag1 || flag2)
		tmpLayer->Resize(width);
	file.close();
	return tmpLayer;
}

std::vector<std::shared_ptr<Pixel>> PAM::RGB(std::ifstream& file, const int& width, const int& height, const int& offset)const
{
	file.seekg(offset, std::ios::beg);
	std::vector<std::shared_ptr<Pixel>> tmpPixels;
	for (int i = 0; i < height; i++)
	{
		char tmpR, tmpG, tmpB;
		for (int j = 0; j < width; j++)
		{
			file.read(&tmpR, sizeof(char));
			file.read(&tmpG, sizeof(char));
			file.read(&tmpB, sizeof(char));
			tmpPixels.push_back(std::make_shared<Pixel>(tmpR, tmpG, tmpB, 255));
		}
	}
	Reverse(tmpPixels,width,height);
	return tmpPixels;
}

std::vector<std::shared_ptr<Pixel>> PAM::RGBA(std::ifstream& file, const int& width, const int& height, const int& offset)const
{
	file.seekg(offset, std::ios::beg);
	std::vector<std::shared_ptr<Pixel>> tmpPixels;
	for (int i = 0; i < height; i++)
	{
		char tmpR, tmpG, tmpB,tmpA;
		for (int j = 0; j < width; j++)
		{
			file.read(&tmpR, sizeof(char));
			file.read(&tmpG, sizeof(char));
			file.read(&tmpB, sizeof(char));
			file.read(&tmpA, sizeof(char));
			tmpPixels.push_back(std::make_shared<Pixel>(tmpR, tmpG, tmpB, tmpA));
		}
	}
	Reverse(tmpPixels,width,height);
	return tmpPixels;
}

void PAM::Reverse(std::vector<std::shared_ptr<Pixel>>& Pixels,const int& width,const int& height)const
{
	for (int i = 0, j = Pixels.size() - 1; i < j; i++, j--)
	{
		std::shared_ptr<Pixel> tmp = Pixels[i];
		Pixels[i] = Pixels[j];
		Pixels[j] = tmp;
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0, z = width - 1; j < z; j++, z--)
		{
			std::shared_ptr<Pixel> tmp = Pixels[j + width * i];
			Pixels[j + width * i] = Pixels[z + width * i];
			Pixels[z + width * i] = tmp;
		}
	}
}

void PAM::Save(const std::string& Location, std::shared_ptr<Layer>& Finished)const
{
	std::string Loc;
	if (Location.find_first_of('.') == std::string::npos)
		Loc = Location + ".pam";
	else if (Location.substr(Location.find_last_of('.')).compare(".pam") != 0)
		Loc = Location + ".pam";
	else
		Loc = Location;
	const std::string P7 = "P7\n";
	const std::string width = "WIDTH " + std::to_string(Layer::width) + "\n";
	const std::string height = "HEIGHT " + std::to_string(Layer::height) + "\n";
	const std::string depth = "DEPTH 4\n";
	const std::string maxValue = "MAXVAL 255\n";
	const std::string type = "TUPLTYPE RGB_ALPHA\n";
	const std::string end = "ENDHDR\n";
	Reverse(Finished->GetPixels(),Layer::width,Layer::height);
	std::ofstream file(Loc, std::ios::out | std::ios::binary);
	if (!file)
		throw ExceptionFileOpen();
	file.write(P7.c_str(), P7.size());
	file.write(width.c_str(), width.size());
	file.write(height.c_str(), height.size());
	file.write(depth.c_str(), depth.size());
	file.write(maxValue.c_str(), maxValue.size());
	file.write(type.c_str(), type.size());
	file.write(end.c_str(), end.size());
	for (auto x : Finished->GetPixels())
	{
		file.put(x->GetRed());
		file.put(x->GetGreen());
		file.put(x->GetBlue());
		file.put(x->GetAlpha());
	}
	Reverse(Finished->GetPixels(), Layer::width, Layer::height);
}

void MyFormat::Save(const std::string& Location, std::shared_ptr<Layer>& Finished)const
{
	std::string Loc;
	if (Location.find_first_of('.') == std::string::npos)
		Loc = Location + ".mn";
	else if (Location.substr(Location.find_last_of('.')).compare(".mn") != 0)
		Loc = Location + ".mn";
	else
		Loc = Location;
	rapidxml::xml_document<> Doc;
	rapidxml::xml_node<>* Root = Doc.allocate_node(rapidxml::node_element, "Project");
	std::shared_ptr<Picture> Pic = Picture::GetInstance();
	
	rapidxml::xml_node<>* LayersNode = Doc.allocate_node(rapidxml::node_element, "Layers");
	rapidxml::xml_node<>* NumberOfLayersNode= Doc.allocate_node(rapidxml::node_element, "NumberOfLayers");
	NumberOfLayersNode->value(Doc.allocate_string(std::to_string(Pic->GetLayers().size()).c_str()));
	LayersNode->append_node(NumberOfLayersNode);
	for (auto LayerPointer : Pic->GetLayers())
	{
		rapidxml::xml_node<>* LayerNode = Doc.allocate_node(rapidxml::node_element, "Layer");
		rapidxml::xml_node<>* LayerActiveNode= Doc.allocate_node(rapidxml::node_element, "Active");
		rapidxml::xml_node<>* LayerVisibleNode= Doc.allocate_node(rapidxml::node_element, "Visibility");
		rapidxml::xml_node<>* LayersTransparencyNode = Doc.allocate_node(rapidxml::node_element, "Transparency");
		LayerActiveNode->value(Doc.allocate_string(std::to_string(LayerPointer->GetActive()).c_str()));
		LayerVisibleNode->value(Doc.allocate_string(std::to_string(LayerPointer->GetVisible()).c_str()));
		LayersTransparencyNode->value(Doc.allocate_string(std::to_string(LayerPointer->GetTransparency()).c_str()));
		LayerNode->append_node(LayerActiveNode);
		LayerNode->append_node(LayerVisibleNode);
		LayerNode->append_node(LayersTransparencyNode);
		LayersNode->append_node(LayerNode);
	}
	Root->append_node(LayersNode);
	
	rapidxml::xml_node<>* CompositeNode= Doc.allocate_node(rapidxml::node_element, "CompositeOperations");
	CompositeNode->value(Doc.allocate_string(std::to_string(CompositeOperation::CompositeOperations.size()).c_str()));
	Root->append_node(CompositeNode);
	
	for (auto SelectionsPointer : Pic->GetSelections())
	{
		rapidxml::xml_node<>* SelectionNode = Doc.allocate_node(rapidxml::node_element, "Selection");
		rapidxml::xml_node<>* SelectionNameNode = Doc.allocate_node(rapidxml::node_element, "Name");
		rapidxml::xml_node<>* SelectionActiveNode = Doc.allocate_node(rapidxml::node_element, "Active");
		SelectionNameNode->value(Doc.allocate_string(SelectionsPointer.first.c_str()));
		SelectionActiveNode->value(Doc.allocate_string(std::to_string(SelectionsPointer.second->GetActive()).c_str()));
		SelectionNode->append_node(SelectionNameNode);
		SelectionNode->append_node(SelectionActiveNode);
		for (auto RectanglePointer : SelectionsPointer.second->GetRectangles())
		{
			rapidxml::xml_node<>* RectangleNode = Doc.allocate_node(rapidxml::node_element, "Rectangle");
			rapidxml::xml_node<>* RectangleCoordinateNode=Doc.allocate_node(rapidxml::node_element, "x");
			RectangleCoordinateNode->value(Doc.allocate_string(std::to_string(RectanglePointer->GetTopLeft().first).c_str()));
			RectangleNode->append_node(RectangleCoordinateNode);
			RectangleCoordinateNode = Doc.allocate_node(rapidxml::node_element, "y");
			RectangleCoordinateNode->value(Doc.allocate_string(std::to_string(RectanglePointer->GetTopLeft().second).c_str()));
			RectangleNode->append_node(RectangleCoordinateNode);
			rapidxml::xml_node<>* RectangleWidthHeightNode = Doc.allocate_node(rapidxml::node_element, "Width");
			RectangleWidthHeightNode->value(Doc.allocate_string(std::to_string(RectanglePointer->GetWidth()).c_str()));
			RectangleNode->append_node(RectangleWidthHeightNode);
			RectangleWidthHeightNode = Doc.allocate_node(rapidxml::node_element, "Height");
			RectangleWidthHeightNode->value(Doc.allocate_string(std::to_string(RectanglePointer->GetHeight()).c_str()));
			RectangleNode->append_node(RectangleWidthHeightNode);
			SelectionNode->append_node(RectangleNode);
		}
		Root->append_node(SelectionNode);
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
	std::smatch match;
	std::regex reg("((?:[^\\\\]*\\\\)*)(.*)");
	std::regex_search(Loc, match, reg);
	std::string str = match[1].str() + "Layers";
	std::experimental::filesystem::create_directory(str);
	int i = 0;
	for (auto x : Pic->GetLayers())
	{
		std::shared_ptr<Formatter> format = Formatter::Factory(".bmp");
		format->Save(str + "\\Layer" + std::to_string(i) + ".bmp", x);
		i++;
	}
	str = match[1].str() + "CompositeOperation";
	std::experimental::filesystem::create_directory(str);
	i = 0;
	for (auto x : CompositeOperation::CompositeOperations)
	{
		x->SaveXml(str + "\\CompositeOperation" + std::to_string(i) + ".fun");
		i++;
	}
}

std::shared_ptr<Layer> MyFormat::Read(const std::string& Location)const
{
	try
	{
		if (Location.find_first_of('.') == std::string::npos)
			throw ExceptionFileType();
		else if (Location.substr(Location.find_last_of('.')).compare(".mn") != 0)
			throw ExceptionFileType();
		std::smatch match;
		std::regex reg("((?:[^\\\\]*\\\\)*)(.*)");
		std::regex_search(Location, match, reg);
		std::string LayerLocation = match[1].str() + "Layers";
		std::string CompositeLocation = match[1].str() + "CompositeOperation";
	
		std::ifstream file(Location);
		if (!file)
			throw ExceptionFileOpen();
		CompositeOperation::CompositeOperations.clear();
		std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		rapidxml::xml_document<> Doc;
		Doc.parse<0>(&buffer[0]);
		rapidxml::xml_node<>* Root = Doc.first_node();

		rapidxml::xml_node<>* NodeDepth1 = Root->first_node();
		rapidxml::xml_node<>* NodeDepth2 = NodeDepth1->first_node();
		int NumberOf = std::atoi(NodeDepth2->value());
		int Active, Visibility;
		double Transparency;
		std::shared_ptr<Formatter> f = Formatter::Factory(".bmp");
		std::shared_ptr<Layer> L;
		rapidxml::xml_node<>* ParNode;
		for (int i = 0; i < NumberOf; i++)
		{
			NodeDepth2 = NodeDepth2->next_sibling();

			ParNode = NodeDepth2->first_node();
			Active = std::atoi(ParNode->value());
			ParNode = ParNode->next_sibling();
			Visibility = std::atoi(ParNode->value());
			ParNode = ParNode->next_sibling();
			Transparency = std::stod(ParNode->value());

			L=f->Read(LayerLocation + "\\Layer" + std::to_string(i) + ".bmp");
			L->SetActive(Active);
			L->SetVisible(Visibility);
			L->SetTransparency(Transparency);
			if (i != (NumberOf - 1))
				Picture::GetInstance()->GetLayers().push_back(L);

		}

		NodeDepth1 = NodeDepth1->next_sibling();
		NumberOf = std::atoi(NodeDepth1->value());
		for (int i = 0; i < NumberOf; i++)
		{
			std::shared_ptr<CompositeOperation> Co = std::make_shared<CompositeOperation>(CompositeLocation + "\\CompositeOperation" + std::to_string(i) + ".fun");
			CompositeOperation::CompositeOperations.push_back(Co);
		}

		for (NodeDepth1 = NodeDepth1->next_sibling(); NodeDepth1; NodeDepth1 = NodeDepth1->next_sibling())
		{
			NodeDepth2 = NodeDepth1->first_node();
			std::string SelectionName = NodeDepth2->value();
			NodeDepth2 = NodeDepth2->next_sibling();
			Active = std::atoi(NodeDepth2->value());
			std::vector<std::shared_ptr<Rectangle>> Rectangles;
			for (NodeDepth2 = NodeDepth2->next_sibling(); NodeDepth2; NodeDepth2 = NodeDepth2->next_sibling())
			{
				ParNode = NodeDepth2->first_node();
				std::pair<int, int> xy;
				xy.first = std::atoi(ParNode->value());
				ParNode = ParNode->next_sibling();
				xy.second = std::atoi(ParNode->value());
				int width, height;
				ParNode = ParNode->next_sibling();
				width = std::atoi(ParNode->value());
				ParNode = ParNode->next_sibling();
				height = std::atoi(ParNode->value());
				Rectangles.push_back(std::make_shared<Rectangle>(xy, width, height));
			}
			Picture::GetInstance()->CreateNewSelection(SelectionName, Rectangles);
			Picture::GetInstance()->GetSelections().find(SelectionName)->second->SetActive(Active);
		}

		Doc.clear();
		return L;
	}
	catch (ExceptionFileType & e)
	{
		std::cout << e.what() << std::endl;
	}
}