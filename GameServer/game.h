#pragma once
#include <string>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include<vector>

using namespace std;

class game
{
public:
	game();
	~game();

	//1װ����2���ϣ�3���ܣ�4��ʹ�õ���
	struct public_prop
	{
		sf::Uint32 id;
		sf::Uint16 iconPosX, iconPosY;
		wstring name;
		wstring explanation;
		sf::Uint8 type;			//propTypeMap����
		sf::Vector2f dropImgPos;
		sf::Uint32 buyMoney = 100;
		sf::Uint32 level = 1;		//ʹ�õȼ�
		string madeMaterial = "0:2:1";	//�����õ�����ʹ�õ��䷽ id:type:num, id:type:num
		sf::Uint32 madeTime = 10;	//�����õ��������ʱ�䣨�룩
		sf::Uint8 madeNum = 1;	//����һ�ο��Ի�õ�����
		bool canMade = true;	//�Ƿ��������
		sf::Uint8 colorLevel = 0;	//��ɫ�ȼ�
		sf::Uint16 cdSum = 0;	//��CDʱ��
	};

	//װ��
	struct equipment : public_prop
	{
		//wstring name; //װ��������			
		sf::Uint8 bodyParts = 0;	//�ؼף����
		//short int type; //0--0����,1����,2ħ��
		//wstring explanation;
		//sf::Vector2f dropImgPos;	//����Ʒ ��ͼƬλ��
		sf::Uint16 durable = 1000;	//�;ö�	
		string repairMaterial = "0:2:1";	//id:type:num, ����װ���Ĳ���id�����ͣ�����		
		sf::Uint16 mateId = 0;
		sf::Uint16 coreId = 0;
		string level0Material = "0:1:2,0:2:2";	// ��������ɫװ����Ҫ�Ĳ���id,num,type
		string level1Material = "0:2:2,0:3:2";	//...
		string level2Material = "0:3:2,0:4:2";	//...

	};
	std::map<sf::Uint16, equipment> equipArray;

	//��ʹ�õ���,��ʯ
	struct prop : public_prop
	{
		string val = "";		//�ַ�������ֵ

	}propdata;
	std::map<sf::Uint16, prop> propVector;

	//����
	struct material : public_prop
	{


	};
	std::map<sf::Uint16, material>materialArray;

	//����
	struct skill
	{
		sf::Uint16 iconPosX, iconPosY;	//ͼ��λ��
		wstring name;
		wstring explanation;
		sf::Int8 bodyParts;	//ʩ�ż���������װ��
		float cdSum;	//��CDʱ��
		float dis;	//��������

		bool isDurable = false;		//�����ü����Ƿ������;ö�
		sf::Uint16 durable = 10;	//ÿ�ͷ�һ�μ������ĵ��;ö�
		sf::Uint8 type = 0; //0�����͹���, 1������, 2ħ������, 3�����༼��û���˺���
		sf::Uint16 mingzhonglv = 900;	//���������ʣ���ֵ1/1000
		sf::Uint16 baojilv = 300;		//������,1/1000
		bool Active = true;		//�������ܣ����Ǳ�������
		sf::Uint16 hurtVal = 10;	//����ֵ
		bool isTarget = false;		//���������費��ҪĿ��
		std::string needVal = "mo:10";	//ʩ��һ����Ҫʲô����ֵ
		sf::Uint16 addexp = 10;		//ʩ��һ�����ӵļ��ܾ���ֵ

		bool isBuff = false;	//��buff�༼��
		bool isdamage = false;	//ture���ӣ� false����
		std::string buffVal = "h:50";	//(�����"s",���ʾ�Ǽ����˺�)
		sf::Uint16 keepTime = 20;	//buff����ʱ��
		sf::Uint8 hurtTime = 1;		//ÿ�������˺�һ��
	};
	std::map<sf::Uint16, skill> skillMap;

	sf::Uint8 MapDataX = 3, MapDataY = 3;
	sf::Uint16 MapWidthHeight = 1920;
	struct Mapdat
	{
		bool isload = false;
		sf::Uint16 mapX, mapY;
		sf::Sprite MapSprite;
		sf::Image blackMap;
		sf::Texture texture;
		std::string bgMusic;
	}mapdata;
	std::vector<Mapdat> mapVector;

	void readData();
	string wstringTostring(wstring wstr);
	wstring stringTowstring(string str);


};

