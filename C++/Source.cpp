#include<iostream>
#include<vector>
#include<memory>
#include<regex>
#include"Errors.h"
#include"Pixel.h"
#include"Layer.h"
#include"Rectangle.h"
#include"Selection.h"
#include"Operations.h"
#include"Formatter.h"
#include"Picture.h"

void ClearScreen()
{
	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
	std::system("cls");
}

void PrintOperations()
{
	std::cout << "1.Inverse" << std::endl;
	std::cout << "2.Grey" << std::endl;
	std::cout << "3.Blacn and white" << std::endl;
	std::cout << "4.Abs" << std::endl;
	std::cout << "5.Blur" << std::endl;
	std::cout << "6.Log" << std::endl;
	std::cout << "7.Add" << std::endl;
	std::cout << "8.Sub" << std::endl;
	std::cout << "9.Inverse sub" << std::endl;
	std::cout << "10.Mul" << std::endl;
	std::cout << "11.Div" << std::endl;
	std::cout << "12.Inverse div" << std::endl;
	std::cout << "13.Min" << std::endl;
	std::cout << "14.Max" << std::endl;
	std::cout << "15.Fill" << std::endl;
	std::cout << "16.Pow" << std::endl;
	std::cout << "17.Composite function" << std::endl;
}

std::shared_ptr<Operation> OptionOperations(int Option,bool flag)
{
	std::string str;
	std::shared_ptr<Operation> Op;
	int tmpOption;
	double tmpOptionD;
	switch (Option)
	{
	case 1:
		Op = std::make_shared<OperationInverse>();
		break;
	case 2:
		Op = std::make_shared<OperationGrey>();
		break;
	case 3:
		Op = std::make_shared<OperationBlackAndWhite>();
		break;
	case 4:
		Op = std::make_shared<OperationAbs>();
		break;
	case 5:
		Op = std::make_shared<OperationBlur>();
		break;
	case 6:
		std::cout << "Enter log base:";
		std::cin >> tmpOptionD;
		Op = std::make_shared<OperationLog>(tmpOptionD);
		ClearScreen();
		break;
	case 7:
		std::cout << "Enter number that you wish to add:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationAdd>(tmpOption);
		ClearScreen();
		break;
	case 8:
		std::cout << "Enter number that you wish to subtract:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationSub>(tmpOption);
		ClearScreen();
		break;
	case 9:
		std::cout << "Enter number that you wish to subtract from:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationInverseSub>(tmpOption);
		ClearScreen();
		break;
	case 10:
		std::cout << "Enter number that you wish to multiply:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationMul>(tmpOption);
		ClearScreen();
		break;
	case 11:
		std::cout << "Enter number for division:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationDiv>(tmpOption);
		ClearScreen();
		break;
	case 12:
		std::cout << "Enter number that you wish divide from:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationInverseDiv>(tmpOption);
		ClearScreen();
		break;
	case 13:
		std::cout << "Enter min:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationMin>(tmpOption);
		ClearScreen();
		break;
	case 14:
		std::cout << "Enter max:";
		std::cin >> tmpOption;
		Op = std::make_shared<OperationMax>(tmpOption);
		ClearScreen();
		break;
	case 15:
		std::cout << "Enter selection name:";
		std::getline(std::cin, str);
		std::cout << "Enter red,green and blue." << std::endl;
		int tmpR, tmpG, tmpB;
		std::cout << "R:";
		std::cin >> tmpR;
		std::cout << "G:";
		std::cin >> tmpG;
		std::cout << "B:";
		std::cin >> tmpB;
		Op = std::make_shared<OperationFill>(str, tmpR, tmpG, tmpB);
		ClearScreen();
		break;
	case 16:
		std::cout << "Enter power:";
		std::cin >> tmpOptionD;
		Op = std::make_shared<OperationPow>(tmpOptionD);
		ClearScreen();
		break;
	case 17:
		if (flag)
		{
			std::cout << "Enter composite number:";
			std::cin >> tmpOption;
			if (tmpOption >= 1 && tmpOption <= CompositeOperation::CompositeOperations.size())
				Op = CompositeOperation::CompositeOperations[tmpOption - 1];
			else
			{
				Op = nullptr;
				std::cout << "That composite function doesn't exist." << std::endl;
				std::system("pause");
			}
			ClearScreen();
		}
		else
		{
			std::cout << "Enter location:";
			std::getline(std::cin, str);
			std::system("cls");
			if (str.find_first_of('.') == std::string::npos)
			{
				Op = nullptr;
				std::cout << "File with no extansion name given aborting." << std::endl;
				std::system("pause");
				std::system("cls");
				break;
			}
			if (str.substr(str.find_first_of('.')).compare(".fun") != 0)
			{
				Op = nullptr;
				std::cout << "File with wrong extansion given aborting." << std::endl;
				std::system("pause");
				std::system("cls");
				break;
			}
			Op = std::make_shared<CompositeOperation>(str);
		}
		break;
	default:
		std::cout << "Operation doesn't exist." << std::endl;
		Op = nullptr;
		std::system("cls");
		break;
	}
	return Op;
}

int main(int argc, char** argv)
{
	std::shared_ptr<Picture>Pic = nullptr;
	if (argc >= 3)
	{
		std::string picloc = argv[1];
		std::string compoloc = argv[2];
		if (picloc.find_first_of('.') == std::string::npos || compoloc.find_first_of('.') == std::string::npos)
			return -1;
		std::string savelocation;
		std::smatch match;
		std::regex reg("((?:[^\\\\]*\\\\)*)(.*)");
		std::regex_search(picloc, match, reg);
		savelocation = match[1].str() + "finished.mn";
		std::shared_ptr<Formatter> format = Formatter::Factory(picloc.substr(picloc.find_first_of('.')));
		if (format == nullptr)
			return -2;
		Pic = Picture::GetInstance(*format, picloc);
		std::shared_ptr<CompositeOperation>Co = std::make_shared<CompositeOperation>(compoloc);
		Pic->Execute(*Co);
		Pic->Save(*Formatter::Factory(".mn"), savelocation);
		return 0;
	}
	while (true)
	{
		std::string str;
		std::vector<std::shared_ptr<Rectangle>> Rectangles;
		std::shared_ptr<Formatter> format;
		std::shared_ptr<Operation> Op;
		std::vector<std::shared_ptr<Operation>> OpV;
		std::shared_ptr<CompositeOperation>Co;
		int Option;
		std::cout << "1.Creat a new picture(project)." << std::endl;
		std::cout << "2.Add a new layer to project." << std::endl;
		std::cout << "3.Add a new selection to project." << std::endl;
		std::cout << "4.Remove a layer from project." << std::endl;
		std::cout << "5.Remove a selection from project." << std::endl;
		std::cout << "6.Activate/disable a layer(operations don't work on disabled layers)." << std::endl;
		std::cout << "7.Activate/disable a selection." << std::endl;
		std::cout << "8.Set a layer to visible/invisible(if a layer is invisible it won't affect the end result)." << std::endl;
		std::cout << "9.Set a layers transparency(0-100 a higher number means the layer is less transparent)." << std::endl;
		std::cout << "10.Execute an operation." << std::endl;
		std::cout << "11.Creat a composite function." << std::endl;
		std::cout << "12.Save a composite function." << std::endl;
		std::cout << "13.Finish project." << std::endl;
		std::cout << "14.Load an existing project(will erase everything from current project if not saved)." << std::endl;
		std::cout << "15.Save current project." << std::endl;
		std::cout << "0.Exit" << std::endl;
		std::cout << "Input:";
		std::cin >> Option;
		ClearScreen();
		switch (Option)
		{
		case 0:
			if (Pic == nullptr || Pic->GetSaved())
				return 0;
			std::cout << "Are you sure you want to exit without saving Y/N." << std::endl;
			std::cout << "Input:";
			char tmpOption;
			std::cin >> tmpOption;
			if (tmpOption == 'Y' || tmpOption == 'y')
				return 0;
			ClearScreen();
			break;
		case 1:
			if (Pic != nullptr)
			{
				std::cout << "Creating a new picture(project) will erase the current project do you wish to continue Y/N?" << std::endl;
				std::cout << "Input:";
				char tmpOption;
				std::cin >> tmpOption;
				if (tmpOption != 'Y' && tmpOption != 'y')
				{
					ClearScreen();
					break;
				}
				Picture::DeleteInstance();
				ClearScreen();
			}
			std::cout << "1.Creat a new project using width and height." << std::endl;
			std::cout << "2.Create a new project using an existing picture." << std::endl;
			std::cout << "Input:";
			std::cin >> Option;
			ClearScreen();
			switch (Option)
			{
			case 1:
				int tmpWidth;
				int tmpHeight;
				std::cout << "Width:";
				std::cin >> tmpWidth;
				std::cout << "Height:";
				std::cin >> tmpHeight;
				Pic = Picture::GetInstance(tmpWidth, tmpHeight);
				ClearScreen();
				break;
			case 2:
				std::cout << "Location:";
				std::getline(std::cin, str);
				if (str.find_first_of('.') != std::string::npos)
				{
					format = Formatter::Factory(str.substr(str.find_last_of('.')));
					if(format!=nullptr)
						Pic = Picture::GetInstance(*format, str);
				}
				else
				{
					std::cout << "Fajl doesn't have extansion." << std::endl;
					std::system("pause");
				}
				system("cls");
				break;
			default:
				std::cout << "Option doesn't exist a picture was created with width=800 and hight=600." << std::endl;
				Pic = Picture::GetInstance(800, 600);
				system("cls");
				break;
			}
			break;
		case 2:
			if (Pic != nullptr)
			{
				std::cout << "1.Add a new enmpty layer." << std::endl;
				std::cout << "2.Add a picture as layer." << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				switch (Option)
				{
				case 1:
					Pic->CreateNewLayer();
					break;
				case 2:
					std::cout << "Location:";
					std::getline(std::cin, str);
					if (str.find_first_of('.') != std::string::npos)
					{
						format = Formatter::Factory(str.substr(str.find_last_of('.')));
						if (format != nullptr)
							Pic->CreateNewLayer(*format, str);
					}
					else
					{
						std::cout << "Fajl doesn't have extansion." << std::endl;
						std::system("pause");
					}
					system("cls");
					break;
				}
			}
			else
			{
				std::cout << "Layer can't be created.Project doesn't exist." << std::endl;
				system("pause");
			}
			system("cls");
			break;
		case 3:
			if (Pic != nullptr)
			{
				std::cout << "Enter selection name." << std::endl;
				std::cout << "Input:";
				std::getline(std::cin, str);
				system("cls");
				bool flag = true;
				do
				{
					int x, y;
					std::cout << "Enter the coordinates of the top left corner of the rectangle." << std::endl;
					std::cout << "x:";
					std::cin >> x;
					std::cout << "y:";
					std::cin >> y;
					ClearScreen();
					std::pair<int, int>coordinates;
					coordinates.first = x;
					coordinates.second = y;
					std::cout << "Enter the width and height of the rectangle." << std::endl;
					std::cout << "Width:";
					std::cin >> x;
					std::cout << "Height:";
					std::cin >> y;
					Rectangles.push_back(std::make_shared<Rectangle>(coordinates, x, y));
					ClearScreen();
					std::cout << "Do you wish to add more rectangles Y/N?" << std::endl;
					char tmpOption;
					std::cout << "Input:";
					std::cin >> tmpOption;
					if (tmpOption != 'Y' && tmpOption != 'y')
						flag = false;
					ClearScreen();
				} while (flag);
				Pic->CreateNewSelection(str, Rectangles);
				Rectangles.clear();
			}
			else
			{
				std::cout << "Selection can't be created.Project doesn't exist." << std::endl;
				system("pause");
			}
			system("cls");
			break;
		case 4:
			if (Pic != nullptr)
			{
				std::cout << "Enter the number of the layer you wish to remove." << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				Pic->DeleteLayer(Option);
				ClearScreen();
			}
			else
			{
				std::cout << "Layer can't be removed.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 5:
			if (Pic != nullptr)
			{
				std::cout << "Enter selection name that you wish to remove." << std::endl;
				std::cout << "Input:";
				std::getline(std::cin, str);
				Pic->DeleteSelection(str);
				system("cls");
			}
			else
			{
				std::cout << "Selection can't be removed.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 6:
			if (Pic != nullptr)
			{
				std::cout << "Enter the number of the layer that you wish to change the active status of." << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				std::cout << "Should the layer be active Y/N?" << std::endl;
				char tmpOption;
				std::cout << "Input:";
				std::cin >> tmpOption;
				if (tmpOption == 'Y' || tmpOption == 'y')
					Pic->SetActiveLayer(Option, true);
				else
					Pic->SetActiveLayer(Option, false);
				ClearScreen();
			}
			else
			{
				std::cout << "Can't change layer active status.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 7:
			if (Pic != nullptr)
			{
				std::cout << "Enter the name of the selection that you wish to change the active status of." << std::endl;
				std::cout << "Input:";
				std::getline(std::cin, str);
				system("cls");
				std::cout << "Should the selection be active Y/N?" << std::endl;
				char tmpOption;
				std::cout << "Input:";
				std::cin >> tmpOption;
				if (tmpOption == 'Y' || tmpOption == 'y')
					Pic->SetSelectionActive(str, true);
				else
					Pic->SetSelectionActive(str, false);
				ClearScreen();
			}
			else
			{
				std::cout << "Can't change layer activity.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 8:
			if (Pic != nullptr)
			{
				std::cout << "Enter the number of the layer that you wish to change the visibility of." << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				std::cout << "Should the layer be visible Y/N?" << std::endl;
				char tmpOption;
				std::cout << "Input:";
				std::cin >> tmpOption;
				if (tmpOption == 'Y' || tmpOption == 'y')
					Pic->SetVisibleLayer(Option, true);
				else
					Pic->SetVisibleLayer(Option, false);
				ClearScreen();
			}
			else
			{
				std::cout << "Can't change layer visibility.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 9:
			if (Pic != nullptr)
			{
				std::cout << "Enter the number of the layer that you wish to change the transparency of." << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				std::cout << "Enter a new transparency 0-100." << std::endl;
				double tmpOption;
				std::cout << "Input:";
				std::cin >> tmpOption;
				Pic->SetTransparencyLayer(Option, tmpOption);
				ClearScreen();
			}
			else
			{
				std::cout << "Can't change layer transparency.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 10:
			if (Pic != nullptr)
			{
				PrintOperations();
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				Op = OptionOperations(Option,true);
				if (Op != nullptr)
					Pic->Execute(*Op);
			}
			else
			{
				std::cout << "Can't execute an operation.Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 11:
			std::cout << "1.Load an existing function." << std::endl;
			std::cout << "2.Create a new function" << std::endl;
			std::cout << "Input:";
			std::cin >> Option;
			ClearScreen();
			switch (Option)
			{
			case 1:
				std::cout << "Enter location:";
				std::getline(std::cin, str);
				std::system("cls");
				if (str.find_first_of('.') == std::string::npos)
				{
					std::cout << "File with no extansion name given aborting." << std::endl;
					std::system("pause");
					std::system("cls");
					break;
				}
				if (str.substr(str.find_first_of('.')).compare(".fun") != 0)
				{
					std::cout << "File with wrong extansion given aborting." << std::endl;
					std::system("pause");
					std::system("cls");
					break;
				}
				Co = std::make_shared<CompositeOperation>(str);
				CompositeOperation::CompositeOperations.push_back(Co);
				break;
			case 2:
				while (true)
				{
					std::cout << "Select the operations that you wish to be a part of the composite function." << std::endl;
					PrintOperations();
					std::cout << "18.Stop" << std::endl;
					std::cin >> Option;
					ClearScreen();
					if (Option == 18)
						break;
					Op = OptionOperations(Option,false);
					if (Op != nullptr)
						OpV.push_back(Op);
				}
				CompositeOperation::CompositeOperations.push_back(std::make_shared<CompositeOperation>(OpV));
				OpV.clear();
				break;
			default:
				std::cout << "Option doesn't exist." << std::endl;
				std::system("pause");
				break;
			}
			break;
		case 12:
			std::cout << "Enter number of composite function that you wish to save." << std::endl;
			std::cout << "Input:";
			std::cin >> Option;
			ClearScreen();
			if (Option<1 || Option>CompositeOperation::CompositeOperations.size())
			{
				std::cout << "That composite function doesn't exist." << std::endl;
				std::system("pause");
				std::system("cls");
				break;
			}
			std::cout << "Enter where you wish to save the composite function also include how you wish to call the composite function." << std::endl;
			std::cout << "Input:";
			std::getline(std::cin, str);
			try
			{
				CompositeOperation::CompositeOperations[Option-1]->SaveXml(str);
			}
			catch (ExceptionFileOpen & e)
			{
				std::cout << e.what() << std::endl;
				std::system("pause");
			}
			std::system("cls");
			break;
		case 13:
			if (Pic != nullptr)
			{
				std::cout << "1.BMP" << std::endl;
				std::cout << "2.PAM" << std::endl;
				std::cout << "Input:";
				std::cin >> Option;
				ClearScreen();
				std::cout << "Enter name and location." << std::endl;
				std::cout << "Input:";
				std::getline(std::cin, str);
				switch (Option)
				{
				case 1:
					format = Formatter::Factory(".bmp");
					Pic->Save(*format, str);
					break;
				case 2:
					format = Formatter::Factory(".pam");
					Pic->Save(*format, str);
					break;
				default:
					std::cout << "Option doesn't exist." << std::endl;
					std::system("pause");
					break;
				}
				std::system("cls");
			}
			else
			{
				std::cout << "Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		case 14:
			std::cout << "Enter location and name of project that you wish to load." << std::endl;
			std::cout << "Input:";
			std::getline(std::cin, str);
			if (str.find_first_of('.') == std::string::npos)
			{
				std::cout << "File with no extansion name given aborting." << std::endl;
				std::system("pause");
				std::system("cls");
				break;
			}
			format = Formatter::Factory(str.substr(str.find_first_of('.')));
			if (format == nullptr)
			{
				std::system("pause");
				std::system("cls");
				break;
			}
			Picture::DeleteInstance();
			Pic = Picture::GetInstance(*format, str);
			std::system("cls");
			break;
		case 15:
			if (Pic != nullptr)
			{
				std::cout << "Enter location and name of where you wish to save the project." << std::endl;
				std::cout << "Input:";
				std::getline(std::cin, str);
				format = Formatter::Factory(".mn");
				Pic->Save(*format, str);
			}
			else
			{
				std::cout << "Project doesn't exist." << std::endl;
				system("pause");
				system("cls");
			}
			break;
		default:
			std::cout << "Option doesn't exist." << std::endl;
			system("pause");
			system("cls");
			break;
		}
	}
}