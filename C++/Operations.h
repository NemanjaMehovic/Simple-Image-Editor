#pragma once
#include<vector>
#include<memory>
#include<map>
#include<string>
#include"Selection.h"

struct tmpPixel
{
	int R;
	int G;
	int B;
	tmpPixel(const int& Red, const int& Green, const int& Blue)
	{
		R = Red;
		G = Green;
		B = Blue;
	}
};

class Operation
{
	static std::vector<std::pair<std::string, std::shared_ptr<Operation>>> InitilalizerOperations();
	virtual void Op(int&, int&, int&)const=0;
	static std::vector<std::pair<std::string, std::shared_ptr<Operation>>> Operations;
	virtual std::shared_ptr<Operation> GetCopy() { return nullptr; }
public:
	static std::shared_ptr<Operation> Factory(const std::string&);
	virtual std::string GetXml()const { return "Operatio"; }
	virtual void Set(const std::pair<std::vector<double>, std::string>&) {}
	virtual std::shared_ptr<std::pair<std::vector<double>, std::string>> Get() { return nullptr; }
	virtual std::vector<std::shared_ptr<tmpPixel>> Execute(std::map<std::string, std::shared_ptr<Selection>>&, std::vector<std::shared_ptr<tmpPixel>>&)const;
};

class OperationInverse:public Operation
{
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationInverse>(); }
public:
	std::string GetXml()const { return "Inverse"; }
};

class OperationGrey :public Operation
{
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationGrey>(); }
public:
	std::string GetXml()const { return "Grey"; }
};

class OperationBlackAndWhite :public Operation
{
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationBlackAndWhite>(); }
public:
	std::string GetXml()const { return "BlackAndWhite"; }
};

class OperationAbs :public Operation
{
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationAbs>(); }
public:
	std::string GetXml()const { return "Abs"; }
};

class OperationBlur :public Operation
{
	void Op(int& R, int& G, int& B)const {}
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationBlur>(); }
public:
	std::string GetXml()const { return "Blur"; }
	std::vector<std::shared_ptr<tmpPixel>> Execute(std::map<std::string, std::shared_ptr<Selection>>&, std::vector<std::shared_ptr<tmpPixel>>&)const;
};

class OperationLog :public Operation
{
	double base;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationLog>(base); }
public:
	std::string GetXml()const { return "Log"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		base = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(base);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationLog(const double&);
};

class OperationAdd:public Operation
{
	int Sum;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationAdd>(Sum); }
public:
	std::string GetXml()const { return "Add"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Sum = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Sum);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationAdd(const int&);
};

class OperationSub :public Operation
{
	int Sub;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationSub>(Sub); }
public:
	std::string GetXml()const { return "Sub"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Sub = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Sub);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationSub(const int&);
};

class OperationInverseSub :public Operation
{
	int Sub;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationInverseSub>(Sub); }
public:
	std::string GetXml()const { return "InverseSub"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Sub = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Sub);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationInverseSub(const int&);
};

class OperationMul :public Operation
{
	int Mul;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationMul>(Mul); }
public:
	std::string GetXml()const { return "Mul"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Mul = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Mul);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationMul(const int&);
};

class OperationDiv :public Operation
{
	int Div;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationDiv>(Div); }
public:
	std::string GetXml()const { return "Div"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Div = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Div);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationDiv(const int&);
};

class OperationInverseDiv :public Operation
{
	int Div;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationInverseDiv>(Div); }
public:
	std::string GetXml()const { return "InverseDiv"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Div = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Div);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationInverseDiv(const int&);
};

class OperationPow :public Operation
{
	double Pow;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationPow>(Pow); }
public:
	std::string GetXml()const { return "Pow"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Pow = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Pow);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationPow(const double&);
};

class OperationMin :public Operation
{
	int Min;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationMin>(Min); }
public:
	std::string GetXml()const { return "Min"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Min = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Min);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationMin(const int&);
};

class OperationMax :public Operation
{
	int Max;
	void Op(int&, int&, int&)const;
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationMax>(Max); }
public:
	std::string GetXml()const { return "Max"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		Max = x.first[0];
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(Max);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, "");
		return x;
	}
	OperationMax(const int&);
};

class OperationFill :public Operation
{
	std::string name;
	int R;
	int G;
	int B;
	void Op(int& R, int& G, int& B)const {}
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<OperationFill>(name,R,G,B); }
public:
	std::string GetXml()const { return "Fill"; }
	void Set(const std::pair<std::vector<double>, std::string>& x)
	{
		R = x.first[0];
		G = x.first[1];
		B = x.first[2];
		name=x.second;
	}
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get()
	{
		std::vector<double> y;
		y.push_back(R);
		y.push_back(G);
		y.push_back(B);
		std::shared_ptr<std::pair<std::vector<double>, std::string>> x = std::make_shared<std::pair<std::vector<double>, std::string>>(y, name);
		return x;
	}
	OperationFill(const std::string&, const int&, const int&, const int&);
	std::vector<std::shared_ptr<tmpPixel>> Execute(std::map<std::string, std::shared_ptr<Selection>>&, std::vector<std::shared_ptr<tmpPixel>>&)const;
};

class CompositeOperation :public Operation
{
	static std::vector<std::string> UsedLocations;
	std::string Location;
	std::vector<std::shared_ptr<Operation>> Operations;
	void Op(int& R, int& G, int& B)const {}
	std::shared_ptr<Operation> GetCopy() { return std::make_shared<CompositeOperation>(Location); }
	void LoadXml();
public:
	static std::vector<std::shared_ptr<CompositeOperation>> CompositeOperations;
	void Set(const std::pair<std::vector<double>, std::string>&);
	std::shared_ptr<std::pair<std::vector<double>, std::string>> Get();
	std::string GetXml()const { return "Composite"; }
	CompositeOperation(const std::string&);
	CompositeOperation(const std::vector<std::shared_ptr<Operation>>&);
	std::vector<std::shared_ptr<tmpPixel>> Execute(std::map<std::string, std::shared_ptr<Selection>>&, std::vector<std::shared_ptr<tmpPixel>>&)const;
	void SaveXml(const std::string&);
};