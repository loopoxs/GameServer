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

	//1装备，2材料，3技能，4可使用道具
	struct public_prop
	{
		sf::Uint32 id;
		sf::Uint16 iconPosX, iconPosY;
		wstring name;
		wstring explanation;
		sf::Uint8 type;			//propTypeMap类型
		sf::Vector2f dropImgPos;
		sf::Uint32 buyMoney = 100;
		sf::Uint32 level = 1;		//使用等级
		string madeMaterial = "0:2:1";	//制作该道具所使用的配方 id:type:num, id:type:num
		sf::Uint32 madeTime = 10;	//制作该道具所需的时间（秒）
		sf::Uint8 madeNum = 1;	//制作一次可以获得的数量
		bool canMade = true;	//是否可以制作
		sf::Uint8 colorLevel = 0;	//颜色等级
		sf::Uint16 cdSum = 0;	//总CD时间
	};

	//装备
	struct equipment : public_prop
	{
		//wstring name; //装备的名字			
		sf::Uint8 bodyParts = 0;	//重甲，轻甲
		//short int type; //0--0长剑,1弓箭,2魔杖
		//wstring explanation;
		//sf::Vector2f dropImgPos;	//掉落品 的图片位置
		sf::Uint16 durable = 1000;	//耐久度	
		string repairMaterial = "0:2:1";	//id:type:num, 修理装备的材料id，类型，数量		
		sf::Uint16 mateId = 0;
		sf::Uint16 coreId = 0;
		string level0Material = "0:1:2,0:2:2";	// 升级到蓝色装备需要的材料id,num,type
		string level1Material = "0:2:2,0:3:2";	//...
		string level2Material = "0:3:2,0:4:2";	//...

	};
	std::map<sf::Uint16, equipment> equipArray;

	//可使用道具,宝石
	struct prop : public_prop
	{
		string val = "";		//字符串增加值

	}propdata;
	std::map<sf::Uint16, prop> propVector;

	//材料
	struct material : public_prop
	{


	};
	std::map<sf::Uint16, material>materialArray;

	//技能
	struct skill
	{
		sf::Uint16 iconPosX, iconPosY;	//图标位置
		wstring name;
		wstring explanation;
		sf::Int8 bodyParts;	//施放技能依赖的装备
		float cdSum;	//总CD时间
		float dis;	//攻击距离

		bool isDurable = false;		//发动该技能是否消耗耐久度
		sf::Uint16 durable = 10;	//每释放一次技能消耗的耐久度
		sf::Uint8 type = 0; //0无类型攻击, 1物理攻击, 2魔法攻击, 3功能类技能没有伤害的
		sf::Uint16 mingzhonglv = 900;	//技能命中率，总值1/1000
		sf::Uint16 baojilv = 300;		//暴击率,1/1000
		bool Active = true;		//主动技能，还是被动技能
		sf::Uint16 hurtVal = 10;	//威力值
		bool isTarget = false;		//发动技能需不需要目标
		std::string needVal = "mo:10";	//施放一次需要什么属性值
		sf::Uint16 addexp = 10;		//施放一次增加的技能经验值

		bool isBuff = false;	//是buff类技能
		bool isdamage = false;	//ture增加， false减少
		std::string buffVal = "h:50";	//(如果是"s",则表示是技能伤害)
		sf::Uint16 keepTime = 20;	//buff持续时间
		sf::Uint8 hurtTime = 1;		//每多少秒伤害一次
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

