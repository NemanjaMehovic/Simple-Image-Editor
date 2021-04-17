#pragma once
#include<exception>


class ExceptionTransparency :public std::exception
{
public:
	const char* what()
	{
		return "Couldn\'t change transparency value of the Layer.";
	}
};

class ExceptionPictureDoesntExist :public std::exception
{
public:
	const char* what()
	{
		return "Picture doesn\'t exist.";
	}
};

class ExceptionSelectionAlreadyExists :public std::exception
{
public:
	const char* what()
	{
		return "Selection with that name already exists.";
	}
};

class ExceptionSelectionDoesntExists :public std::exception
{
public:
	const char* what()
	{
		return "Selection with that name doesn\'t exists.";
	}
};

class ExceptionLayerDoesntExists :public std::exception
{
public:
	const char* what()
	{
		return "That layer doesn\'t exists.";
	}
};

class ExceptionFileOpen :public std::exception
{
public:
	const char* what()
	{
		return "Couldn\'t open file.";
	}
};

class ExceptionFileType :public std::exception
{
public:
	const char* what()
	{
		return "Wrong type of file.";
	}
};
