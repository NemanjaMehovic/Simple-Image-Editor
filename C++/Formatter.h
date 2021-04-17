#pragma once
#include<vector>
#include<memory>
#include<string>
#include<fstream>
#include"Layer.h"
#include"Pixel.h"


class Formatter
{
	static std::vector<std::pair<std::string, std::shared_ptr<Formatter>>> Formats;
	static std::vector<std::pair<std::string, std::shared_ptr<Formatter>>> InitilalizerFormats();
public:
	static std::shared_ptr<Formatter> Factory(const std::string&);
	virtual std::shared_ptr<Layer> Read(const std::string&)const = 0;
	virtual void Save(const std::string&, std::shared_ptr<Layer>&)const = 0;
};

class BMP :public Formatter
{
	void Header(std::ofstream&)const;
	std::vector<std::shared_ptr<Pixel>> BMP24(std::ifstream&, const int&, const int&, const int&)const;
	std::vector<std::shared_ptr<Pixel>> BMP32(std::ifstream&, const int&, const int&, const int&)const;
public:
	std::shared_ptr<Layer> Read(const std::string&)const;
	void Save(const std::string&, std::shared_ptr<Layer>&)const;
};

class PAM :public Formatter
{
	std::vector<std::shared_ptr<Pixel>> RGB(std::ifstream&, const int&, const int&, const int&)const;
	std::vector<std::shared_ptr<Pixel>> RGBA(std::ifstream&, const int&, const int&, const int&)const;
	void Reverse(std::vector<std::shared_ptr<Pixel>>&,const int&,const int&)const;
public:
	std::shared_ptr<Layer> Read(const std::string&)const;
	void Save(const std::string&, std::shared_ptr<Layer>&)const;
};

class MyFormat :public Formatter
{
public:
	std::shared_ptr<Layer> Read(const std::string&)const;
	void Save(const std::string&, std::shared_ptr<Layer>&)const;
};