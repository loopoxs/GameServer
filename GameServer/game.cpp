#include "stdafx.h"
#include "game.h"

game::game()
{
	std::string str = "../../Project1/Maps/World1/map17.jpg";
	if (!mapdata.texture.loadFromFile(str))
	{
		std::cout << "����ʧ�ܣ�" << std::endl;
	}
	mapdata.MapSprite.setTexture(mapdata.texture);
	mapdata.mapX = 0;
	mapdata.mapY = 0;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapdata.blackMap.loadFromFile("../../Project1/Maps/World1/map16.jpg");
	mapVector.push_back(mapdata);

	mapdata.mapX = 1;
	mapdata.mapY = 0;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 2;
	mapdata.mapY = 0;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 0;
	mapdata.mapY = 1;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 1;
	mapdata.mapY = 1;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 2;
	mapdata.mapY = 1;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 0;
	mapdata.mapY = 2;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 1;
	mapdata.mapY = 2;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

	mapdata.mapX = 2;
	mapdata.mapY = 2;
	mapdata.MapSprite.setPosition(mapdata.mapX * MapWidthHeight, mapdata.mapY * MapWidthHeight);
	mapVector.push_back(mapdata);

}

game::~game()
{

}

//��wstringת����string  
string game::wstringTostring(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}

wstring game::stringTowstring(string str) 
{
	wstring result;      //��ȡ��������С��������ռ䣬��������С���ַ�����      
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];      //���ֽڱ���ת���ɿ��ֽڱ���      
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β      //ɾ��������������ֵ      
	result.append(buffer);
	delete[] buffer;
	return result;
}

void game::readData()
{
	FILE * fp;
	char charStr[255];
	int number = 0;
	int first;

	//��ȡװ���ʵ�
	if ((fp = fopen("..\\Data\\equipBook.txt", "rb")) == NULL)
	{
		std::cout << "Cannot open output file. sound.txt\n" << std::endl;
	}
	else
	{
		while (true)
		{
			first = -1;
			fscanf(fp, "%d", &first);
			if (first < 0)
			{
				break;
			}
			fscanf(fp, "%d", &number);
			equipArray[first].id = number;
			fscanf(fp, "%d", &number);
			equipArray[first].iconPosX = number;
			fscanf(fp, "%d", &number);
			equipArray[first].iconPosY = number;
			fscanf(fp, "%s", &charStr);
			std::string name = charStr;
			equipArray[first].name = stringTowstring(name);
			fscanf(fp, "%s", &charStr);
			std::string explanation = charStr;
			equipArray[first].explanation = stringTowstring(explanation);
			fscanf(fp, "%d", &number);
			equipArray[first].type = number;
			fscanf(fp, "%d", &number);
			equipArray[first].dropImgPos.x = number;
			fscanf(fp, "%d", &number);
			equipArray[first].dropImgPos.y = number;
			fscanf(fp, "%d", &number);
			equipArray[first].buyMoney = number;

			fscanf(fp, "%d", &number);
			equipArray[first].level = number;
			fscanf(fp, "%s", &charStr);
			equipArray[first].madeMaterial = charStr;
			fscanf(fp, "%d", &number);
			equipArray[first].madeTime = number;
			fscanf(fp, "%d", &number);
			equipArray[first].madeNum = number;
			fscanf(fp, "%d", &number);
			equipArray[first].canMade = number;
			fscanf(fp, "%d", &number);
			equipArray[first].colorLevel = number;
			fscanf(fp, "%d", &number);
			equipArray[first].cdSum = number;
			fscanf(fp, "%d", &number);
			equipArray[first].bodyParts = number;
			fscanf(fp, "%d", &number);
			equipArray[first].durable = number;
			fscanf(fp, "%s", &charStr);
			equipArray[first].repairMaterial = charStr;
			fscanf(fp, "%d", &number);
			equipArray[first].mateId = number;
			fscanf(fp, "%d", &number);
			equipArray[first].coreId = number;
			fscanf(fp, "%s", &charStr);
			equipArray[first].level0Material = charStr;
			fscanf(fp, "%s", &charStr);
			equipArray[first].level1Material = charStr;
			fscanf(fp, "%s", &charStr);
			equipArray[first].level2Material = charStr;
		}
	}

	//��ȡ���ߴʵ�
	if ((fp = fopen("..\\Data\\propBook.txt", "rb")) == NULL)
	{
		std::cout << "Cannot open output file. sound.txt\n" << std::endl;
	}
	else
	{
		while (true)
		{
			first = -1;
			fscanf(fp, "%d", &first);
			if (first < 0)
			{
				break;
			}
			fscanf(fp, "%d", &number);
			propVector[first].id = number;
			fscanf(fp, "%d", &number);
			propVector[first].iconPosX = number;
			fscanf(fp, "%d", &number);
			propVector[first].iconPosY = number;
			fscanf(fp, "%s", &charStr);
			std::string name = charStr;
			propVector[first].name = stringTowstring(name);
			fscanf(fp, "%s", &charStr);
			std::string explanation = charStr;
			propVector[first].explanation = stringTowstring(explanation);
			fscanf(fp, "%d", &number);
			propVector[first].type = number;
			fscanf(fp, "%d", &number);
			propVector[first].dropImgPos.x = number;
			fscanf(fp, "%d", &number);
			propVector[first].dropImgPos.y = number;
			fscanf(fp, "%d", &number);
			propVector[first].buyMoney = number;
			fscanf(fp, "%d", &number);
			propVector[first].level = number;
			fscanf(fp, "%s", &charStr);
			propVector[first].madeMaterial = charStr;
			fscanf(fp, "%d", &number);
			propVector[first].madeTime = number;
			fscanf(fp, "%d", &number);
			propVector[first].madeNum = number;
			fscanf(fp, "%d", &number);
			propVector[first].canMade = number;
			fscanf(fp, "%d", &number);
			propVector[first].colorLevel = number;
			fscanf(fp, "%d", &number);
			propVector[first].cdSum = number;

			fscanf(fp, "%s", &number);
			propVector[first].val = number;

		}
	}

	//��ȡ���ϴʵ�
	if ((fp = fopen("..\\Data\\materialBook.txt", "rb")) == NULL)
	{
		std::cout << "Cannot open output file. sound.txt\n" << std::endl;
	}
	else
	{
		while (true)
		{
			first = -1;
			fscanf(fp, "%d", &first);
			if (first < 0)
			{
				break;
			}
			fscanf(fp, "%d", &number);
			materialArray[first].id = number;
			fscanf(fp, "%d", &number);
			materialArray[first].iconPosX = number;
			fscanf(fp, "%d", &number);
			materialArray[first].iconPosY = number;
			fscanf(fp, "%s", &charStr);
			std::string name = charStr;
			materialArray[first].name = stringTowstring(name);
			fscanf(fp, "%s", &charStr);
			std::string explanation = charStr;
			materialArray[first].explanation = stringTowstring(explanation);
			fscanf(fp, "%d", &number);
			materialArray[first].type = number;
			fscanf(fp, "%d", &number);
			materialArray[first].dropImgPos.x = number;
			fscanf(fp, "%d", &number);
			materialArray[first].dropImgPos.y = number;
			fscanf(fp, "%d", &number);
			materialArray[first].buyMoney = number;
			fscanf(fp, "%d", &number);
			materialArray[first].level = number;
			fscanf(fp, "%s", &charStr);
			materialArray[first].madeMaterial = charStr;
			fscanf(fp, "%d", &number);
			materialArray[first].madeTime = number;
			fscanf(fp, "%d", &number);
			materialArray[first].madeNum = number;
			fscanf(fp, "%d", &number);
			materialArray[first].canMade = number;
			fscanf(fp, "%d", &number);
			materialArray[first].colorLevel = number;
			fscanf(fp, "%d", &number);
			materialArray[first].cdSum = number;
		}
	}

	//��ȡ���ܴʵ�
	if ((fp = fopen("..\\Data\\skillBook.txt", "rb")) == NULL)
	{
		std::cout << "Cannot open output file. sound.txt\n" << std::endl;
	}
	else
	{
		while (true)
		{
			first = -1;
			fscanf(fp, "%d", &first);
			if (first < 0)
			{
				break;
			}

			fscanf(fp, "%d", &number);
			skillMap[first].iconPosX = number;
			fscanf(fp, "%d", &number);
			skillMap[first].iconPosY = number;
			fscanf(fp, "%s", &charStr);
			std::string name = charStr;
			skillMap[first].name = stringTowstring(name);
			fscanf(fp, "%s", &charStr);
			std::string explanation = charStr;
			skillMap[first].explanation = stringTowstring(explanation);
			fscanf(fp, "%d", &number);
			skillMap[first].bodyParts = number;
			fscanf(fp, "%d", &number);
			skillMap[first].cdSum = number;
			fscanf(fp, "%d", &number);
			skillMap[first].dis = number;

			fscanf(fp, "%d", &number);
			skillMap[first].isDurable = number;
			fscanf(fp, "%d", &number);
			skillMap[first].durable = number;
			fscanf(fp, "%d", &number);
			skillMap[first].type = number;
			fscanf(fp, "%d", &number);
			skillMap[first].mingzhonglv = number;
			fscanf(fp, "%d", &number);
			skillMap[first].baojilv = number;
			fscanf(fp, "%d", &number);
			skillMap[first].Active = number;
			fscanf(fp, "%d", &number);
			skillMap[first].hurtVal = number;
			fscanf(fp, "%d", &number);
			skillMap[first].isTarget = number;
			fscanf(fp, "%s", &charStr);
			skillMap[first].needVal = charStr;
			fscanf(fp, "%d", &number);
			skillMap[first].addexp = number;

			fscanf(fp, "%d", &number);
			skillMap[first].isBuff = number;
			fscanf(fp, "%d", &number);
			skillMap[first].isdamage = number;
			fscanf(fp, "%s", &charStr);
			skillMap[first].buffVal = charStr;
			fscanf(fp, "%d", &number);
			skillMap[first].keepTime = number;
			fscanf(fp, "%d", &number);
			skillMap[first].hurtTime = number;
		}
	}

	fclose(fp);
}
