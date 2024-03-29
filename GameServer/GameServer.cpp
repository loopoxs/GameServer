// GameServer.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "game.h"
#define PI 3.14159

class EllipseShape : public sf::Shape
{
public:

	explicit EllipseShape(const sf::Vector2f& radius = sf::Vector2f(0.f, 0.f)) :
		m_radius(radius)
	{
		update();
	}

	void setRadius(const sf::Vector2f& radius)
	{
		m_radius = radius;
		update();
	}

	const sf::Vector2f& getRadius() const
	{
		return m_radius;
	}

	virtual std::size_t getPointCount() const
	{
		return 30; // fixed, but could be an attribute of the class if needed
	}

	virtual sf::Vector2f getPoint(std::size_t index) const
	{
		static const float pi = 3.141592654f;

		float angle = index * 2 * pi / getPointCount() - pi / 2;
		float x = std::cos(angle) * m_radius.x;
		float y = std::sin(angle) * m_radius.y;

		return sf::Vector2f(m_radius.x + x, m_radius.y + y);
	}


private:

	sf::Vector2f m_radius;
};
EllipseShape EllipsePublic;
std::vector<EllipseShape> ellipseVector;
std::vector<sf::VertexArray> lineStripVector;
sf::Clock gameClock;
game gamei;
void createSkill1(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId);
void createSkill2(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId);
void createSKill3(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId, sf::Uint16 skillId);
bool letterInfo(string strVal, bool boo, sf::Uint32 uIndex, sf::Uint16 multiple = 1);
void addskillExp(sf::Uint16 skillId, sf::Uint16, sf::Uint32 uIndex);
sf::Int8 getDir(sf::Vector2f start, sf::Vector2f end);
sf::Uint32 createActor(float x, float y, sf::Uint32 uid, sf::Uint8 race, sf::Uint8 ActorType,
	sf::Int32 EnemyId = -1, sf::Uint8 anim = 0, sf::Uint8 dir = 4);
bool getAiPath2(float startX, float startY, float endX, float endY, std::vector<sf::Vector2f> *setVal, float disVal);
void breakAiPath(std::vector<sf::Vector2f> * value, sf::Int8 dis);
bool createSkill(sf::Int32 targetIndexId, sf::Int32 attackerIndexId, sf::Uint8 skillId);
bool AiMoveNext(float posX, float posY);
sf::Uint16 actorIndex = 0;
void ThreadUdpServer();

//角色 资源 保存
struct ActorArr
{
	sf::Uint32 level = 1;	//怪物等级
	std::wstring name = L"001"; //怪物名字
	sf::Uint32 MaxHp = 100;		//MAX HP
	sf::Uint32 MaxMp = 100;		//MAX MP
	sf::Uint32 MaxTili = 100;		//MAX Tili
	sf::Uint32 damage = 4;		//攻击力
	sf::Uint32 defense = 4;		//防御力
	//属性
	std::map<std::string, sf::Uint32> Attributes;

}actorData;
std::vector<ActorArr> actorVector;

struct Player
{
	sf::Int32 uid = -1;	//用户ID
	string playerName;
	float posX, posY, posZ = 0;
	string ip = "";
	unsigned short int ActorAnim, Direction;
	unsigned short int ActorType;	//类型 ,0友方,1敌方,2中立生物(兔子...),3 NPC		
	sf::Int16 race;		//种族, 0人族, 1狼人族		
	sf::Int32 EnemyId = -1;	//敌人组的第几个id
	int AnimEnd = 0;	//上一次移动的时间！
	sf::Text TextName, *TextDamage;
	sf::Int32 *Ttime_TextDamage, *startTimeTextDamage;
	short int damageTextIndex;
	sf::Uint16 textureIndex = 0;
	sf::VertexArray vertex;
	sf::Sprite spriteHP, alertIcon;
	sf::Text talkText;		//聊天气泡
	sf::RectangleShape talkShape;
	sf::Int32 talkShowTime;
	bool isShowTalk = 0;
	bool isJump;		//是否跳跃
	sf::Int32 jumpTime;	//跳跃时间
	bool isDeath = false;	//是否死亡
	sf::Uint8 state = 0;
	sf::Uint8 damageRadius = 15;
	sf::Vector2f damageCenter;
	std::map<sf::Uint16, sf::Int16> buffMap;	//buff技能id, buff CD剩余时间

	sf::Uint32 MaxHp = 100, MaxMp = 100, MaxTili = 100, NowHp = 100, NowMp = 100, NowTili = 100;
	//属性数组<属性字母, 值> // p,20 表示力量20
	std::map<std::string, sf::Uint32> Attributes;
	sf::Uint32 level = 1;
	sf::Uint64 experience = 0;	//经验值
	sf::Uint64 old_experience = 0;	//经验值
}PlayerData;
std::map<sf::Int32, Player> playerMap, drawplayerMap;

struct GroupEmeny
{
	sf::Int8 ActorTypeID = -1;	//友方敌方
	sf::Int16 race;			//种族
	short int bronMapX;	//出生地图X
	short int bronMapY;	//出生地图Y
	int bronX, bronY;	//出生点	相对与现在的地图坐标(0-1920)
	short int bronNum;	//生成数量
	short int MoveRadius;	//可移动半径,垂直距离
	short int MaxStandTime;	//最多等待多久
	short int moveSpeed;	//移动速度
	sf::Uint8 AnimSpeed = 100;	//序列帧播放速度
	sf::Int8 moveDistanceX;	//移动的距离X
	sf::Int8 moveDistanceY;	//移动的距离Y
	sf::Int8 attackType;	//攻击类型	0被攻击后 才发动攻击 1在攻击范围内就发动攻击
	sf::Int16 attackRadius = 400;	//主动攻击范围(视野范围内主动攻击)
	sf::Int8 attackCall;	//被攻击后 是否触发旁边的怪物一起攻击 0否1是,2恐慌逃走
	sf::Uint16 attackCallRadius = 200; //被攻击后,	触发半径
	string skillarr = "";	//能放哪些技能
	string drop;	//掉落物品最多10个，千分之一几率 id:type:10.5%, 
	sf::Uint32 exp = 40;	//杀死怪物获得的经验
	sf::Uint32 maxHp = 1000;
}GroupEmenyArray[10];

struct Enemy
{
	sf::Uint32 indexID;
	bool isDeath = false;
	sf::Int32 deathTime = -1;	//死亡时间
	float nowPosX, nowPosY;
	float bronX, bronY;	//每个怪物的出生点
	sf::Uint32 nowHp;
	sf::Int8 state;		//-1表示上一个动作结束，0站立等待，1行走，2攻击，3....
	sf::Int32 standTime;			//站立等待多少时间		
	sf::Uint8 groupId;
	short int TypeID;	//ActorType 角色类型		
	int standStartTime;		//站立等待开始的时间
	sf::Uint32 standAnimTime;	//站立序列帧播放
	sf::Int32 moveStartTime;	//移动序列帧
	sf::Int32 AnimTime;	//移动序列帧
	float targetPosX, targetPosY;	//移动到目标坐标
	int targetIndexId;	//攻击者id
	sf::Uint8 anim = 0;		//当前序列帧
	sf::Int8 dir;		//方向
	sf::Int8 walkNum;		//目标方向移动的步数
	string hatred;	//仇恨列表

	std::map<sf::Uint16, sf::Uint32> skillCDMap;	//<技能ID, CD时间>	技能CD
	sf::Uint32 lastSkillTime = 0;	//上次施放技能的时间
	std::vector<sf::Vector2f> path;	//寻路算法保存的路径点

}EnemyData;
std::vector<Enemy> EnemyVector;

sf::VertexArray createRoundLine(sf::Vector2f point, sf::Int16 radius = 100, sf::Uint16 pointSum = 60)
{
	double a = ceil(radius / 2.f);
	double b = radius;
	int s = pointSum + 1;
	sf::VertexArray triangle(sf::LineStrip, s);
	int m = 0;
	int i = 0;
	while (true)
	{
		double _tan = tan(i * PI / 180);
		sf::Vector2f data;
		data.x = sqrt(1.0 / ((1.0 / pow(_tan, 2))*(1.0 / pow(a, 2)) + (1.0 / pow(b, 2))));
		data.y = sqrt(1.0 / (1.0 / (pow(a, 2)) + pow(_tan, 2)*(1.0 / pow(b, 2))));
		if (i > 90 && i < 270)
		{
			data.y *= -1;
		}
		if (i > 180 && i < 360)
		{
			data.x *= -1;
		}
		data.x *= -1;
		data.y *= -1;

		data.x += point.x;
		data.y += point.y;

		triangle[m].position = data;
		triangle[m].color = sf::Color::Black;

		m++;
		i += 3;
		if (i > 360)
		{
			break;
		}
	}

	return triangle;
}

void addskill(sf::Uint16 skillId, string s)
{
	std::cout << "添加技能" << std::endl;
}

void setOtherAnim(sf::Uint32 attackerIndexId, sf::Int8 anim, sf::Int8 dir, sf::Int8 state)
{
	
}

sf::Uint64 getLvExp(sf::Uint32 level)
{
	sf::Uint64 lvExp = 50 + pow(level, 3);
	return lvExp;
}

void initGame()
{
	actorData.level = 1;
	actorData.name = L"人类族1";
	actorData.MaxHp = 100 + pow(actorData.level, 3);
	actorData.damage = 4;
	actorData.defense = 4;
	actorData.Attributes["p"] = 4;
	actorData.Attributes["b"] = 2;
	actorVector.push_back(actorData);

	actorData.name = L"人类族2";
	actorData.damage = 4;
	actorData.defense = 4;
	actorData.level = 20;
	actorData.MaxHp = 100 + pow(actorData.level, 3);
	actorData.Attributes["a"] = 4;
	actorData.Attributes["b"] = 2;
	actorVector.push_back(actorData);

	actorData.name = L"小白兔";
	actorData.damage = 4;
	actorData.defense = 4;
	actorData.level = 30;
	actorData.MaxHp = 100 + pow(actorData.level, 3);//HP
	actorData.Attributes["p"] = 4;
	actorData.Attributes["m"] = 2;
	actorVector.push_back(actorData);

	//实例化怪物组
	GroupEmenyArray[0].ActorTypeID = 1;
	GroupEmenyArray[0].race = 0;
	GroupEmenyArray[0].bronNum = 1;
	GroupEmenyArray[0].bronMapX = 0;
	GroupEmenyArray[0].bronMapY = 0;
	GroupEmenyArray[0].bronX = 1480;
	GroupEmenyArray[0].bronY = 1420;
	GroupEmenyArray[0].moveSpeed = 10;
	GroupEmenyArray[0].moveDistanceX = 2;
	GroupEmenyArray[0].moveDistanceY = 2;
	GroupEmenyArray[0].MaxStandTime = 10;
	GroupEmenyArray[0].MoveRadius = 1000;
	GroupEmenyArray[0].attackType = 1;
	GroupEmenyArray[0].attackCall = 1;
	GroupEmenyArray[0].skillarr = "0";
	GroupEmenyArray[0].drop = "0:1:400,1:1:200";

	GroupEmenyArray[1].ActorTypeID = 1;
	GroupEmenyArray[1].race = 1;
	GroupEmenyArray[1].bronNum = 10;
	GroupEmenyArray[1].bronMapX = 0;
	GroupEmenyArray[1].bronMapY = 0;
	GroupEmenyArray[1].bronX = 2000;
	GroupEmenyArray[1].bronY = 2000;
	GroupEmenyArray[1].moveSpeed = 10;
	GroupEmenyArray[1].moveDistanceX = 2;
	GroupEmenyArray[1].moveDistanceY = 2;
	GroupEmenyArray[1].MaxStandTime = 10;
	GroupEmenyArray[1].MoveRadius = 1000;
	GroupEmenyArray[1].attackType = 0;
	GroupEmenyArray[1].attackCall = 0;
	GroupEmenyArray[1].skillarr = "1";
	GroupEmenyArray[1].drop = "0:1:400,1:1:200";

	GroupEmenyArray[2].ActorTypeID = 2;
	GroupEmenyArray[2].race = 2;
	GroupEmenyArray[2].bronNum = 10;
	GroupEmenyArray[2].bronMapX = 0;
	GroupEmenyArray[2].bronMapY = 0;
	GroupEmenyArray[2].bronX = 2500;
	GroupEmenyArray[2].bronY = 1700;
	GroupEmenyArray[2].moveSpeed = 130;
	GroupEmenyArray[2].moveDistanceX = 5;
	GroupEmenyArray[2].moveDistanceY = 5;
	GroupEmenyArray[2].MaxStandTime = 10;
	GroupEmenyArray[2].MoveRadius = 500;
	GroupEmenyArray[2].attackType = 0;
	GroupEmenyArray[2].attackCall = 1;
	GroupEmenyArray[2].drop = "0:1:400,1:1:200";
	GroupEmenyArray[2].attackCallRadius = 300;

	//生成怪物群组ID
	std::string GroupEnemyStr = "0";	//0,1,2

	sf::Uint8 GroupEnemySum = 1;
	for (size_t j = 0; j < GroupEnemyStr.length(); j++)
	{
		if (GroupEnemyStr[j] == ',')
		{
			GroupEnemySum++;
		}
	}
	//实例化怪物数组
	std::string enemyGroupId;
	sf::Uint16 Enemyid = 0;

	sf::VertexArray triangle;
	for (size_t k = 0; k < GroupEnemyStr.length(); k++)
	{
		if (GroupEnemyStr[k] == ',' || k >= GroupEnemyStr.length() - 1)
		{
			if (k >= GroupEnemyStr.length() - 1)
			{
				enemyGroupId += GroupEnemyStr[k];
			}
			int i = std::stoi(enemyGroupId);
			for (size_t j = 0; j < GroupEmenyArray[i].bronNum; j++)
			{
				sf::Int8 num = 1;
				if (rand() % 2 == 0)
				{
					num = -1;
				}
				num *= rand() % GroupEmenyArray[i].MoveRadius;
				EnemyData.bronX = EnemyData.nowPosX = GroupEmenyArray[i].bronMapX * gamei.MapWidthHeight + GroupEmenyArray[i].bronX + num;

				num = 1;
				if (rand() % 2 == 0)
				{
					num = -1;
				}
				sf::Uint16 radius = round(GroupEmenyArray[i].MoveRadius / 2.f);
				num *= rand() % radius;
				EnemyData.bronY = EnemyData.nowPosY = GroupEmenyArray[i].bronMapY * gamei.MapWidthHeight + GroupEmenyArray[i].bronY + num;

				EllipsePublic.setRadius(sf::Vector2f(8, 8));
				EllipsePublic.setPosition({ (float)(GroupEmenyArray[i].bronMapX * gamei.MapWidthHeight + GroupEmenyArray[i].bronX),
					(float)(GroupEmenyArray[i].bronMapY * gamei.MapWidthHeight + GroupEmenyArray[i].bronY) });
				EllipsePublic.setFillColor(sf::Color::White);
				ellipseVector.push_back(EllipsePublic);

				triangle = createRoundLine({ (float)(GroupEmenyArray[i].bronMapX * gamei.MapWidthHeight + GroupEmenyArray[i].bronX) ,
					(float)(GroupEmenyArray[i].bronMapY * gamei.MapWidthHeight + GroupEmenyArray[i].bronY) }, GroupEmenyArray[i].MoveRadius, 120);
				lineStripVector.push_back(triangle);

				EnemyData.groupId = i;
				EnemyData.state = -1;
				EnemyData.nowHp = GroupEmenyArray[i].maxHp;
				EnemyData.targetIndexId = -1;
				EnemyData.indexID = createActor(EnemyData.nowPosX, EnemyData.nowPosY, Enemyid, GroupEmenyArray[i].race, GroupEmenyArray[i].ActorTypeID, Enemyid);

				//根据技能字符串添加到Map里
				EnemyData.skillCDMap.clear();
				std::string cdstr = "";
				for (size_t p = 0; p < GroupEmenyArray[i].skillarr.length(); p++)
				{
					if (GroupEmenyArray[i].skillarr[p] == ',' || p >= GroupEmenyArray[i].skillarr.length() - 1)
					{
						if (p >= GroupEmenyArray[i].skillarr.length() - 1)
						{
							cdstr += GroupEmenyArray[i].skillarr[p];
						}

						EnemyData.skillCDMap[std::stoi(cdstr)] = 0;

						cdstr = "";
					}
					else
					{
						cdstr += GroupEmenyArray[i].skillarr[p];
					}
				}
				EnemyVector.push_back(EnemyData);
				Enemyid++;
			}
			enemyGroupId = "";
		}
		else
		{
			enemyGroupId += GroupEnemyStr[k];
		}
	}

}

sf::Uint32 createActor(float x, float y, sf::Uint32 uid, sf::Uint8 race, sf::Uint8 ActorType,
	sf::Int32 EnemyId, sf::Uint8 anim, sf::Uint8 dir)
{
	PlayerData.level = actorVector.at(race).level;
	PlayerData.playerName = gamei.wstringTostring(actorVector.at(race).name);
	PlayerData.NowHp = PlayerData.MaxHp = actorVector.at(race).MaxHp;
	PlayerData.NowMp = PlayerData.MaxMp = actorVector.at(race).MaxMp;
	PlayerData.NowTili = PlayerData.MaxTili = actorVector.at(race).MaxTili;
	PlayerData.EnemyId = EnemyId;
	PlayerData.Attributes = actorVector.at(race).Attributes;

	PlayerData.uid = uid;
	PlayerData.posX = x;
	PlayerData.posY = y;
	PlayerData.ActorAnim = anim;
	PlayerData.Direction = dir;
	PlayerData.ActorType = ActorType;
	PlayerData.race = race;

	sf::Uint32 returnVal = actorIndex;
	playerMap[actorIndex] = PlayerData;
	actorIndex++;

	return returnVal;
}

void setActorPos(sf::Uint32 uIndex, sf::Vector2f pos, sf::Uint8 anim, sf::Uint8 dir)
{
	playerMap[uIndex].posX = pos.x;
	playerMap[uIndex].posY = pos.y;
	playerMap[uIndex].ActorAnim = anim;
	playerMap[uIndex].Direction = dir;

}

void setActorAnim(sf::Uint32 uIndex, sf::Uint8 anim, sf::Uint8 dir)
{
	playerMap[uIndex].ActorAnim = anim;
	playerMap[uIndex].Direction = dir;

	std::cout << "设置角色动画帧" << std::endl;
}

void playerDeath(sf::Uint32 uIndex)
{
	sf::Vertex *quad4 = &playerMap[uIndex].vertex[0];
	quad4[0].position.x = 0, quad4[0].position.y = 0;
	quad4[1].position.x = 0, quad4[1].position.y = 0;
	quad4[2].position.x = 0, quad4[2].position.y = 0;
	quad4[3].position.x = 0, quad4[3].position.y = 0;

	playerMap[uIndex].spriteHP.setPosition(0, 0);
	playerMap[uIndex].alertIcon.setPosition(0, 0);
	playerMap[uIndex].TextName.setPosition(0, 0);

}

//复活怪物
void reviveEnemy(sf::Uint16 enemyVectorIndex)
{
	EnemyVector.at(enemyVectorIndex).nowPosX = EnemyVector.at(enemyVectorIndex).bronX;
	EnemyVector.at(enemyVectorIndex).nowPosY = EnemyVector.at(enemyVectorIndex).bronY;
	sf::Uint8 groupId = EnemyVector.at(enemyVectorIndex).groupId;
	EnemyVector.at(enemyVectorIndex).nowHp = GroupEmenyArray[groupId].maxHp;
	EnemyVector.at(enemyVectorIndex).targetIndexId = -1;
	EnemyVector.at(enemyVectorIndex).state = -1;
	EnemyVector.at(enemyVectorIndex).isDeath = false;

	//gui->letterInfo("h:" + std::to_string(EnemyVector.at(enemyVectorIndex).nowHp), true, EnemyVector.at(enemyVectorIndex).indexID);
	playerMap[EnemyVector.at(enemyVectorIndex).indexID].NowHp = EnemyVector.at(enemyVectorIndex).nowHp;

	playerMap[EnemyVector.at(enemyVectorIndex).indexID].isDeath = false;

	//复活后设置坐标
	setActorPos(EnemyVector.at(enemyVectorIndex).indexID, { EnemyVector.at(enemyVectorIndex).bronX, EnemyVector.at(enemyVectorIndex).bronY },
		EnemyVector.at(enemyVectorIndex).anim, EnemyVector.at(enemyVectorIndex).dir);

}

sf::Int8 IsPixelInEllipse2(
	sf::Int64  ri_X,
	sf::Int64  ri_Y,
	sf::Int64 ri_CentreX,
	sf::Int64 ri_CentreY,
	sf::Int64 ri_HRadius,
	sf::Int64 ri_VRadius)
{
	sf::Int64 i_X;
	sf::Int64 i_Y;

	i_X = ri_X - ri_CentreX;
	i_Y = ri_Y - ri_CentreY;

	sf::Int64 i_SX = ((i_X << 8) / ri_HRadius);
	sf::Int64 i_SY = ((i_Y << 8) / ri_VRadius);

	if (i_SX > (1 << 8) || i_SY > (1 << 8))
	{
		return false;	//0
	}
	else
	{
		return  (i_SX * i_SX) + (i_SY * i_SY) <= (1 << 16);
	}
}

sf::Int32 checkEnemyTarget(sf::Vector2f point, float radius)
{
	for (map<int, Player>::iterator iter = playerMap.begin(); iter != playerMap.end(); iter++)
	{
		//目标是玩家
		if (playerMap[iter->first].ActorType == 0)
		{
			//玩家在目标圈内
			if (IsPixelInEllipse2(playerMap[iter->first].posX, playerMap[iter->first].posY, point.x, point.y, radius, round(radius / 2.f)) > 0)
			{
				return iter->first;
			}
		}
	}
	return -1;
}

void checkEvent()
{
	sf::Int32 nowTime = gameClock.getElapsedTime().asMilliseconds();

	//AI
	sf::Uint16 index = 0;
	for (auto &data : EnemyVector)
	{
		if (data.isDeath == true)
		{
			if (gameClock.getElapsedTime().asMilliseconds() > data.deathTime + 5000)
			{
				reviveEnemy(index);
			}
			else
				continue;
		}
		//命令执行完毕 并且 没有攻击
		if (data.state == -1 && data.targetIndexId < 0)
		{
			sf::Int8 state = rand() % 2;
			if (state == 1)
			{
				state = 3;
			}
			data.state = state;

			//设置 站立等待
			if (state == 0)
			{
				data.standTime = rand() % GroupEmenyArray[data.groupId].MaxStandTime;
				data.standAnimTime = data.standStartTime = nowTime;

				playerMap[data.indexID].state = 0;

				//std::cout << "AI设置站立" << std::endl;
			}
			//在自己出生点范围内，随机找个点，无障碍模式走过去
			else if (state == 1)
			{

			}
			//上下左右哪个方向能移动
			else if (state == 3)
			{
				//无论朝哪个方向移动，都要走满随机步数？
				data.walkNum = 2 + rand() % 20;
				data.dir = rand() % 8;

				//std::cout << "AI设置行走" << std::endl;
				playerMap[data.indexID].state = 1;

			}
		}
		//判断怪物是主动攻击还是被动攻击，主动攻击则判断目标圈内是否有玩家
		if (GroupEmenyArray[data.groupId].attackType == 1 && data.targetIndexId < 0)
		{
			sf::Int32 index = checkEnemyTarget({ data.nowPosX, data.nowPosY }, GroupEmenyArray[data.groupId].attackRadius);

			//玩家是否在怪物的可视攻击范围内
			if (index >= 0)
			{
				//std::cout<< "发现敌人" <<std::endl;
				if (IsPixelInEllipse2(
					floor(playerMap[index].posX), floor(playerMap[index].posY), data.bronX, data.bronY,
					GroupEmenyArray[data.groupId].MoveRadius, round(GroupEmenyArray[data.groupId].MoveRadius / 2.f)) > 0)
				{
					//std::cout << "设置敌人攻击状态" << std::endl;
					data.state = 2;
					data.targetIndexId = index;
				}
			}
			else
			{
				//std::cout << "没有发现敌人" << std::endl;
			}
		}
		//执行 站立等待
		if (data.state == 0)
		{
			if (nowTime >= data.standStartTime + data.standTime * 1000)
			{
				data.state = -1;
			}
		}
		//攻击
		else if (data.state == 2)
		{
			//判断是否有敌人目标
			if (data.targetIndexId < 0)
			{
				data.state = -1;
				data.targetIndexId = -1;
				continue;
			}
			bool isClear = false;

			//玩家是否在敌人的活动范围内
			if (IsPixelInEllipse2(
				floor(playerMap[data.targetIndexId].posX), floor(playerMap[data.targetIndexId].posY), data.bronX, data.bronY,
				GroupEmenyArray[data.groupId].MoveRadius, round(GroupEmenyArray[data.groupId].MoveRadius / 2.f)) <= 0)
			{
				//std::cout << "走出范围" << floor(playerMap[data.targetIndexId].posX) <<","<< floor(playerMap[data.targetIndexId].posY)
					//<< ","<< data.bronX<< ","<< data.bronY<<","<< GroupEmenyArray[data.groupId].MoveRadius << std::endl;
				isClear = true;
			}
			//z轴高低距离检测
			if (abs(playerMap[data.targetIndexId].posZ - playerMap[data.indexID].posZ) >= 200)
			{
				//std::cout << "z轴高低距离检测" << std::endl;
				isClear = true;
			}
			if (isClear == true)
			{
				data.state = -1;
				data.targetIndexId = -1;
				data.hatred = "";	//清空仇恨列表
			}
			else
			{
				//技能cd时间检测
				//技能数组里，哪个技能的cd好了，就施放哪个技能
				for (auto &data2 : data.skillCDMap)
				{
					//技能的cd
					if (nowTime >= data2.second + gamei.skillMap[data2.first].cdSum * 1000.f
						&& nowTime - data.lastSkillTime >= 1000)
					{
						float EnemyAttackDis = gamei.skillMap[data2.first].dis;
						bool isInDis = false;
						//判断敌人是否在自己坐标的攻击距离内
						if (IsPixelInEllipse2(floor(playerMap[data.targetIndexId].posX), floor(playerMap[data.targetIndexId].posY),
							data.nowPosX, data.nowPosY, EnemyAttackDis, round(EnemyAttackDis / 2.f)) > 0)
						{
							isInDis = true;
						}
						//不在范围内就开始寻路
						if (isInDis == false)
						{
							data.path.clear();
							bool isVal = false;
							isVal = getAiPath2(data.nowPosX, data.nowPosY,
								playerMap[data.targetIndexId].posX, playerMap[data.targetIndexId].posY,
								&data.path, EnemyAttackDis / 2);	//距离取一半

							breakAiPath(&data.path, GroupEmenyArray[data.groupId].moveDistanceX);
							
							//寻路成功
							if (isVal == true && !data.path.empty())
							{
								if (!data.path.empty())
								{
									data.path.pop_back();
								}

								//设置AI模式
								data.state = 4;
								//行走模式
								playerMap[data.indexID].state = 1;
							}
							//寻路失败
							else
							{
								data.state = -1;
								data.targetIndexId = -1;
							}
						}
						//创建技能
						else
						{
							createSkill(data.targetIndexId, data.indexID, data2.first);
							data2.second = nowTime;

							//技能施放间隔最少1秒
							data.lastSkillTime = nowTime;
						}

						break;
					}
				}
			}
		}
		//执行 行走2,在自己出生点范围内，随机一个方向行走0-10步数
		else if (data.state == 3)
		{
			/*if (nowTime - data.AnimTime >= GroupEmenyArray[data.groupId].AnimSpeed)
			{
				data.AnimTime = nowTime;
				data.anim += 1;
				if (data.anim >= actorVector.at(GroupEmenyArray[data.groupId].race).walkNum)
				{
					data.anim = 0;
				}
			}*/
			if (nowTime - data.moveStartTime >= GroupEmenyArray[data.groupId].moveSpeed)
			{
				data.moveStartTime = nowTime;
				sf::Vector2f point;
				point.x = data.nowPosX;
				point.y = data.nowPosY;

				//判断下一步是否可以移动
				sf::Int8 dirSum = 0;
				bool isSuccess = false;
				while (true)
				{
					if (dirSum >= 8)
					{
						//八个方向都走不了
						data.state = -1;
						data.targetIndexId = -1;
						break;
					}
					if (data.dir == 0)
					{
						point.y += GroupEmenyArray[data.groupId].moveDistanceY;
					}
					if (data.dir == 4)
					{
						point.y -= GroupEmenyArray[data.groupId].moveDistanceY;
					}
					if (data.dir == 2)
					{
						point.x -= GroupEmenyArray[data.groupId].moveDistanceX;
					}
					if (data.dir == 6)
					{
						point.x += GroupEmenyArray[data.groupId].moveDistanceX;
					}
					//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
					if (data.dir == 3)
					{
						point.x -= GroupEmenyArray[data.groupId].moveDistanceX;
						point.y -= GroupEmenyArray[data.groupId].moveDistanceY;
					}
					if (data.dir == 1)
					{
						point.x -= GroupEmenyArray[data.groupId].moveDistanceX;
						point.y += GroupEmenyArray[data.groupId].moveDistanceY;
					}
					if (data.dir == 5)
					{
						point.x += GroupEmenyArray[data.groupId].moveDistanceX;
						point.y -= GroupEmenyArray[data.groupId].moveDistanceY;
					}
					if (data.dir == 7)
					{
						point.x += GroupEmenyArray[data.groupId].moveDistanceX;
						point.y += GroupEmenyArray[data.groupId].moveDistanceY;
					}
					//是否走出圈外了
					if (IsPixelInEllipse2(ceil(point.x), ceil(point.y), data.bronX, data.bronY, GroupEmenyArray[data.groupId].MoveRadius, GroupEmenyArray[data.groupId].MoveRadius / 2) <= 0
						//或者不能走
						|| AiMoveNext(point.x, point.y) == true)
					{
						//如果这个方向走不了, 就dir + 1个方向判断
						data.dir++;
						if (data.dir >= 8)
						{
							data.dir = 0;
						}
					}
					else
					{
						data.nowPosX = point.x;
						data.nowPosY = point.y;
						isSuccess = true;
						break;
					}
					dirSum++;
				}

				if (isSuccess == true)
				{
					setActorPos(data.indexID, { data.nowPosX, data.nowPosY }, data.anim, data.dir);

					data.walkNum--;
					if (data.walkNum < 0)
					{
						data.state = -1;
						data.targetIndexId = -1;
					}
				}
			}
		}
		//根据寻路算法行走，上面攻击施放技能检测距离，所以不能加else,距离不够就执行寻路算法接近目标地点
		if (data.state == 4)
		{
			/*if (nowTime - data.AnimTime >= GroupEmenyArray[data.groupId].AnimSpeed)
			{
				data.AnimTime = nowTime;
				data.anim += 1;
				if (data.anim >= actorVector.at(GroupEmenyArray[data.groupId].race).walkNum)
				{
					data.anim = 0;
				}
			}*/
			if (nowTime - data.moveStartTime >= GroupEmenyArray[data.groupId].moveSpeed)
			{
				data.moveStartTime = nowTime;

				if (!data.path.empty())
				{
					data.dir = getDir({ data.nowPosX, data.nowPosY }, { data.path.back().x, data.path.back().y });

					data.nowPosX = data.path.back().x;
					data.nowPosY = data.path.back().y;

					setActorPos(data.indexID, { data.nowPosX, data.nowPosY }, data.anim, data.dir);

					data.path.pop_back();
				}

				//寻路结束,执行攻击状态
				if (data.path.empty())
				{
					data.state = 2;
				}
			}
		}

		index++;
	}
}

int main()
{
	//线程侦听端口
	//sf::Thread thread(&ThreadUdpServer);
	//thread.launch();

	initGame();
		
	while (true)
	{
		checkEvent();
	}

	return 0;

}

//type0玩家本人对敌人发动，1敌人对玩家本人
bool createSkill(sf::Int32 targetIndexId, sf::Int32 attackerIndexId, sf::Uint8 skillId)
{
	if (gamei.skillMap[skillId].isTarget == true && targetIndexId < 0)
	{
		std::cout << "未选中敌人" << std::endl;
		return false;
	}
	//攻击之前调整 面向敌人
	//如果技能需要目标, 并且目标不是自己
	if (gamei.skillMap[skillId].isTarget == true && targetIndexId != attackerIndexId)
	{
		sf::Vector2f attackPos, targetPos;
		attackPos.x = playerMap[attackerIndexId].posX;
		attackPos.y = playerMap[attackerIndexId].posY;
		targetPos.x = playerMap[targetIndexId].posX;
		targetPos.y = playerMap[targetIndexId].posY;

		sf::Uint8 absdis = 10;	//...

		sf::Int8 runDir = -1;
		//左上方
		if (attackPos.x < targetPos.x && attackPos.y < targetPos.y && abs(attackPos.x - targetPos.x) >= absdis && abs(attackPos.y - targetPos.y) >= absdis)
		{
			runDir = 7;
		}
		//左下方
		else if (attackPos.x < targetPos.x && attackPos.y > targetPos.y && abs(attackPos.x - targetPos.x) >= absdis && abs(attackPos.y - targetPos.y) >= absdis)
		{
			runDir = 5;
		}
		//右下方
		else if (attackPos.x > targetPos.x && attackPos.y > targetPos.y && abs(attackPos.x - targetPos.x) >= absdis && abs(attackPos.y - targetPos.y) >= absdis)
		{
			runDir = 3;
		}
		//右上方
		else if (attackPos.x > targetPos.x && attackPos.y < targetPos.y && abs(attackPos.x - targetPos.x) >= absdis && abs(attackPos.y - targetPos.y) >= absdis)
		{
			runDir = 1;
		}
		//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
		//正上方
		else if (abs(attackPos.x - targetPos.x) < absdis && attackPos.y < targetPos.y)
		{
			runDir = 0;
		}
		//正下方
		else if (abs(attackPos.x - targetPos.x) < absdis && attackPos.y > targetPos.y)
		{
			runDir = 4;
		}
		//正左方
		else if (abs(attackPos.y - targetPos.y) < absdis && attackPos.x < targetPos.x)
		{
			runDir = 6;
		}
		//正右方
		else if (abs(attackPos.y - targetPos.y) < absdis && attackPos.x > targetPos.x)
		{
			runDir = 2;
		}

		if (runDir >= 0)
			setOtherAnim(attackerIndexId, playerMap[attackerIndexId].ActorAnim, runDir, 0);
	}


	bool isSuccess = false;
	if (gamei.skillMap[skillId].isBuff == true)
	{
		createSKill3(targetIndexId, attackerIndexId, skillId);

		isSuccess = true;
	}
	else
	{
		//技能特效
		if (skillId == 0)
		{
			isSuccess = true;
			createSkill1(targetIndexId, attackerIndexId);
		}
		else if (skillId == 1)
		{
			isSuccess = true;
			createSkill2(targetIndexId, attackerIndexId);
		}
	}
	//怪物和中立生物 不用减少， 只有玩家施放才减少
	if (isSuccess == true && playerMap[attackerIndexId].ActorType == 0)
	{
		//减少技能需要的属性值(使用第一次技能需要, 比如需要消耗10魔力施放)
		//减少施放者的属性
		letterInfo(gamei.skillMap[skillId].needVal, false, attackerIndexId);
	}

	//不是功能类技能，就增加经验, 并且施放者是玩家
	if (gamei.skillMap[skillId].type != 3 && playerMap[attackerIndexId].ActorType == 0 && gamei.skillMap.find(skillId) != gamei.skillMap.end())
	{
		//增加技能经验值
		addskillExp(skillId, gamei.skillMap[skillId].addexp, attackerIndexId);
	}

	return true;
}

void addskillExp(sf::Uint16 skillId, sf::Uint16, sf::Uint32 uIndex)
{
	std::cout<< "增加uIndex的技能经验值" <<std::endl;
}

//boo true 增加, false 减少 
bool letterInfo(string strVal, bool boo, sf::Uint32 uIndex, sf::Uint16 multiple)
{
	if (strVal == "")
	{
		return false;
	}
	sf::Int32 value;
	string data[2];
	for (size_t i = 0, n = 0; i < strVal.length(); i++)
	{
		if (strVal[i] != ':' && strVal[i] != ',')
		{
			data[n] += strVal[i];
		}
		if (strVal[i] == ':')
		{
			n++;
		}
		if (strVal[i] == ',' || i == strVal.length() - 1)
		{
			sf::Int64 setVal = 0;

			if (data[1] != "")
			{
				value = std::stoi(data[1]);
			}
			else
			{
				return false;
			}

			//技能
			if (data[0] == "s")
			{
				if (boo == true)
				{
					addskill(value, "addtion");
				}
				else
				{
					gamei.skillMap.erase(value);
				}
			}

			if (boo == false)
			{
				value *= -1;
			}
			value *= multiple;

			//血量
			if (data[0] == "h")
			{
				if (value > playerMap[uIndex].NowHp)
				{
					playerMap[uIndex].NowHp = 0;
				}
				else if (playerMap[uIndex].NowHp + value > playerMap[uIndex].MaxHp)
				{
					playerMap[uIndex].NowHp = playerMap[uIndex].MaxHp;
				}
				else
				{
					playerMap[uIndex].NowHp += value;
				}
				if (playerMap[uIndex].NowHp <= 0)
				{
					playerMap[uIndex].isDeath = true;

					playerDeath(uIndex);

					return true;
				}
			}
			//血量最大值
			if (data[0] == "hm")
			{
				playerMap[uIndex].MaxHp += value;
				if (playerMap[uIndex].NowHp > playerMap[uIndex].MaxHp)
				{
					playerMap[uIndex].NowHp = playerMap[uIndex].MaxHp;
				}
			}
			//体力
			if (data[0] == "t")
			{
				setVal = playerMap[uIndex].NowTili + value;
				if (setVal < 0)
				{
					playerMap[uIndex].NowTili = 0;
				}
				else if (setVal > playerMap[uIndex].MaxTili)
				{
					playerMap[uIndex].NowTili = playerMap[uIndex].MaxTili;
				}
				else
				{
					playerMap[uIndex].NowTili += value;
				}
			}
			//体力最大值
			if (data[0] == "tm")
			{
				playerMap[uIndex].MaxTili += value;
				if (playerMap[uIndex].NowTili > playerMap[uIndex].MaxTili)
				{
					playerMap[uIndex].NowTili = playerMap[uIndex].MaxTili;
				}
			}
			//魔力
			if (data[0] == "mo")
			{
				setVal = playerMap[uIndex].NowMp + value;
				if (setVal < 0)
				{
					playerMap[uIndex].NowMp = 0;
				}
				else if (setVal > playerMap[uIndex].MaxMp)
				{
					playerMap[uIndex].NowMp = playerMap[uIndex].MaxMp;
				}
				else
				{
					playerMap[uIndex].NowMp += value;
				}
			}
			//魔力最大值
			if (data[0] == "mom")
			{
				playerMap[uIndex].MaxMp += value;
				if (playerMap[uIndex].NowMp > playerMap[uIndex].MaxMp)
				{
					playerMap[uIndex].NowMp = playerMap[uIndex].MaxMp;
				}
			}

			//经验值
			if (data[0] == "exp")
			{
				//一直增加或减少到 值 为0为止
				while (value != 0)
				{
					if (value > 0)
					{
						sf::Uint64 exp = getLvExp(playerMap[uIndex].level);
						if (playerMap[uIndex].experience + value >= exp)
						{
							playerMap[uIndex].level++;
							value = value - (exp - playerMap[uIndex].experience);

							//减完之后判断是否还需要循环
							if (value >= getLvExp(playerMap[uIndex].level))
							{
								playerMap[uIndex].experience = 0;
								continue;
							}
							else
							{
								playerMap[uIndex].experience = value;
								value = 0;
							}
						}
						else
						{
							playerMap[uIndex].experience += value;
							value = 0;
						}
					}

					else if (value < 0)
					{
						sf::Int64 exp;
						if (playerMap[uIndex].level > 1)
						{
							if (playerMap[uIndex].experience < abs(value))
							{
								playerMap[uIndex].level -= 1;
								value += playerMap[uIndex].experience;

								exp = getLvExp(playerMap[uIndex].level);

								if (exp <= abs(value))
								{
									playerMap[uIndex].experience = exp;
									continue;
								}
								else
								{
									playerMap[uIndex].experience = abs(value);
									value = 0;
								}
							}
							else
							{
								playerMap[uIndex].experience += value;
								value = 0;
							}
						}
						else if (playerMap[uIndex].level == 1)
						{
							if (playerMap[uIndex].experience <= abs(value))
							{
								playerMap[uIndex].experience = 0;
							}
							else
							{
								playerMap[uIndex].experience += value;
							}
							value = 0;
						}
					}

				}
			}

			//力量
			if (data[0] == "p")
			{
				setVal = playerMap[uIndex].Attributes["p"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["p"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["p"] += value;
				}

			}
			//智力
			if (data[0] == "z")
			{
				setVal = playerMap[uIndex].Attributes["z"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["z"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["z"] += value;
				}

			}
			//命中率
			if (data[0] == "m")
			{
				setVal = playerMap[uIndex].Attributes["m"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["m"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["m"] += value;
				}

			}
			//暴击率
			if (data[0] == "b")
			{
				setVal = playerMap[uIndex].Attributes["b"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["b"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["b"] += value;
				}

			}
			//金属性
			if (data[0] == "j")
			{
				setVal = playerMap[uIndex].Attributes["j"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["j"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["j"] += value;
				}

			}
			//木属性
			if (data[0] == "mu")
			{
				setVal = playerMap[uIndex].Attributes["mu"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["mu"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["mu"] += value;
				}

			}
			//水属性
			if (data[0] == "shui")
			{
				setVal = playerMap[uIndex].Attributes["shui"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["shui"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["shui"] += value;
				}

			}
			//火属性
			if (data[0] == "huo")
			{
				setVal = playerMap[uIndex].Attributes["huo"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["huo"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["huo"] += value;
				}

			}
			//土属性
			if (data[0] == "tu")
			{
				setVal = playerMap[uIndex].Attributes["tu"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["tu"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["tu"] += value;
				}

			}
			//光属性
			if (data[0] == "g")
			{
				setVal = playerMap[uIndex].Attributes["g"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["g"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["g"] += value;
				}
			}
			//暗属性
			if (data[0] == "a")
			{
				setVal = playerMap[uIndex].Attributes["a"] + value;
				if (setVal < 0)
				{
					playerMap[uIndex].Attributes["a"] = 0;
				}
				else
				{
					playerMap[uIndex].Attributes["a"] += value;
				}
			}

			n = 0;
			data[0] = data[1] = "";
		}
	}

	return false;
}

void createSkill1(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId)
{
	std::cout<< "创建技能1" <<std::endl;
}

void createSkill2(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId)
{
	std::cout << "创建技能2" << std::endl;
}

void createSKill3(sf::Uint32 targetIndexId, sf::Uint32 attackerIndexId, sf::Uint16 skillId)
{
	std::cout << "创建技能3" << std::endl;
}

bool AiMoveNext(float posX, float posY)
{
	if (posX < 0 || posX >= gamei.MapDataX * gamei.MapWidthHeight || posY < 0 || posY >= gamei.MapDataY * gamei.MapWidthHeight)
	{
		return true;
	}

	sf::Uint8 checkMapX = floor(posX / gamei.MapWidthHeight);
	sf::Uint8 checkMapY = floor(posY / gamei.MapWidthHeight);
	sf::Uint16 checkX = ceil(posX - checkMapX * gamei.MapWidthHeight);
	sf::Uint16 checkY = ceil(posY - checkMapY * gamei.MapWidthHeight);

	sf::Color color{ 255, 255, 255, 255 };
	for (auto &data : gamei.mapVector)
	{
		if (data.mapX == checkMapX && data.mapY == checkMapY)
		{
			color = data.blackMap.getPixel(checkX, checkY);
			break;
		}
	}
	//颜色
	if (color.r <= 3 && color.g <= 3 && color.b <= 3)
	{
		return true;
	}

	return false;
}

//disVal抵达终点的半径, 
bool getAiPath2(float startX, float startY, float endX, float endY, std::vector<sf::Vector2f> *setVal, float disVal = 45)
{
	//判断起点和终点是否在可移动区域
	if (AiMoveNext(startX, startY) == true || AiMoveNext(endX, endY) == true)
	{
		std::cout << "自身的位置无法移动或者目标地点不可到达！" << std::endl;
		return false;
	}

	struct _path
	{
		sf::Uint8 dir;
		sf::Uint32 id;
		sf::Vector2f point;
		sf::Uint16 sum = 0;
	}path, path2;

	float distance = 40;

	//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds

	//起点判断上下左右四个方向，哪个方向的距离最短，就首先往那边走，如果走不了就选择第二个距离近的方向
	float minDis = 0, kdis;
	sf::Int8 kDir = -1;
	//上
	if (AiMoveNext(startX, startY - distance) == false)
	{
		kDir = 4;
		minDis = sqrt(pow(abs(startX - endX), 2) + pow(abs(startY - distance - endY), 2));
	}
	//下
	if (AiMoveNext(startX, startY + distance) == false)
	{
		kdis = sqrt(pow(abs(startX - endX), 2) + pow(abs(startY + distance - endY), 2));
		if (kdis < minDis || kDir == -1)
		{
			minDis = kdis;
			kDir = 0;
		}
	}
	//左
	if (AiMoveNext(startX - distance, startY) == false)
	{
		kdis = sqrt(pow(abs(startX - distance - endX), 2) + pow(abs(startY - endY), 2));
		if (kdis < minDis || kDir == -1)
		{
			minDis = kdis;
			kDir = 2;
		}
	}
	//右
	if (AiMoveNext(startX + distance, startY) == false)
	{
		kdis = sqrt(pow(abs(startX + distance - endX), 2) + pow(abs(startY - endY), 2));
		if (kdis < minDis || kDir == -1)
		{
			minDis = kdis;
			kDir = 6;
		}
	}
	if (kDir == -1)
	{
		std::cout << "没有找到路径" << std::endl;
		return false;
	}

	path.dir = kDir;
	path.id = 0;
	path.point.x = startX;
	path.point.y = startY;

	struct _point
	{
		sf::Vector2f point;
		sf::Int8 dir;
		float dis;
		sf::Int32 pid, id;
		bool isVal = true;
	}point, point2, point3, point4, point5, point6;
	std::map<sf::Uint32, _point> pathMap;

	struct _pathxb
	{
		sf::Vector2f point;
		sf::Uint8 type;
	}pathxb;
	std::vector<_pathxb> pathVector;
	std::vector<sf::Vector2f> pathPoint;

	point.dir = path.dir;
	point.point = path.point;
	point.dis = sqrt(pow(abs(startX - endX), 2) + pow(abs(startY - endY), 2));
	point.pid = -1;
	int addid = 0;
	pathMap.insert(std::pair<sf::Uint32, _point>(addid++, point));

	float checkX = startX, checkY = startY;
	int a = 0;

	std::vector<sf::Vector2f> xbVec;
	while (true)
	{
		a++;
		//开始从终点回到起点 找到最近的路
		if (IsPixelInEllipse2(floor(path.point.x), floor(path.point.y), endX, endY, disVal, round(disVal / 2.f)) > 0)
		{
			//终点，最后一个点
			point.point = path.point;
			point.dis = 0;
			point.pid = path.id;
			point.id = addid;
			pathMap.insert(std::pair<sf::Uint32, _point>(addid++, point));
			pathPoint.push_back(path.point);
			xbVec.push_back(path.point);
			int pid = path.id = point.id;

			//直线路径，直接返回
			if (point.pid == 0)
			{
				pathPoint.clear();
				int l = pathMap.size() - 1;
				sf::Vector2f lastPoint;
				lastPoint.x = 0;
				lastPoint.y = 0;
				while (l >= 0)
				{
					if (abs(lastPoint.x - pathMap[l].point.x) > 1 || abs(lastPoint.y - pathMap[l].point.y) > 1)
					{
						pathPoint.push_back(pathMap[l].point);
					}
					lastPoint = pathMap[l].point;
					l--;
				}

				if (!pathPoint.empty())
				{
					*setVal = pathPoint;

					return true;
				}
				else
				{
					return false;
				}
			}
			//画线
			bool isGo = false;
			sf::Vector2f lastPoint2;
			std::vector<sf::Vector2f> lastPath;
			bool isdrawball = false;
			int s = 0;
			while (true)
			{
				if (pid == -1)
				{
					break;
				}

				sf::Int32 p0 = pid;
				sf::Int32 p1 = pathMap[p0].pid;
				sf::Int32 p2 = pathMap[p1].pid;

				//垂直 或者 水平 线段上的 三个点
				if (p0 >= 0 && p1 >= 0 && p2 >= 0)
				{
					//判断对面的线条是否可以直接到达 ，例如凹字形
					sf::Int8 fx1 = pathMap[p0].dir, fx2 = pathMap[p2].dir;

					if ((fx1 == 0 && fx2 == 4 || fx1 == 4 && fx2 == 0 || fx1 == 2 && fx2 == 6 || fx1 == 6 && fx2 == 2) && !pathPoint.empty() && 3 < 2)
					{
						sf::Int8 targetDir = pathMap[p1].dir;
						sf::Int8 moveDir = pathMap[p2].dir;
						sf::Vector2f movePos = pathMap[p2].point;
						sf::Vector2f pos = movePos;
						bool isSuccess = false, first = false;
						bool isNext = false;
						static int colorNum = 1;
						sf::Uint8 loopTime = 0;
						while (true)
						{
							isSuccess = false;
							for (auto &data : pathPoint)
							{
								if (abs(data.x - pos.x < 0.1) && abs(data.y - pos.y) < 0.1)
								{
									isSuccess = true;
									break;
								}
							}
							if (isSuccess == true)
							{
								break;
							}
							//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
							if (AiMoveNext(pos.x, pos.y) == true || loopTime >= 10)
							{
								isNext = false;
								first = true;
								if (moveDir == 0)
								{
									movePos.y += distance;
								}
								if (moveDir == 4)
								{
									movePos.y -= distance;
								}
								if (moveDir == 2)
								{
									movePos.x -= distance;
								}
								if (moveDir == 6)
								{
									movePos.x += distance;
								}
								pos = movePos;
								if (movePos == pathMap[p1].point)
								{
									break;
								}
							}

							loopTime++;
						}
						//colorNum++;

						//成功到达对面
						if (isSuccess == true)
						{
							//重新赋值中点和末尾点
							pathMap[p1].point = movePos;
							pathMap[p0].point = pos;

							//if (!lastPath.empty())
							{
								//std::vector<sf::Vector2f>::iterator iter;
								//iter = lastPath.end() - 1;

								//上一次的多余出来的点，删除掉
								int h = 0;
								bool isdelete = false;
								for (auto &data : pathPoint)
								{
									if (isdelete == false)
									{
										if (abs(pathMap[p0].point.x - data.x) < 0.1 && abs(pathMap[p0].point.y - data.y) < 0.1)
										{
											isdelete = true;

											EllipsePublic.setRadius(sf::Vector2f(8, 8));
											EllipsePublic.setPosition({ pathMap[p0].point.x - 10, pathMap[p0].point.y });
											EllipsePublic.setFillColor(sf::Color::Blue);
											ellipseVector.push_back(EllipsePublic);

										}
									}
									else
									{
										lastPath.pop_back();
										pathPoint.pop_back();
									}
								}
							}
						}

					}

					lastPath.clear();

					sf::Vector2f target0 = pathMap[p0].point;
					sf::Vector2f target1 = pathMap[p1].point;
					pathxb.point = target0;
					pathxb.type = 0;
					pathVector.push_back(pathxb);

					bool isTurn = false;
					bool isFind = false;
					int dir4 = getDir(pathMap[p0].point, pathMap[p1].point);

					//isGo = false;
					//把水平垂直线上的点放入数组
					while (true)
					{
						pathxb.type = 0;
						if (dir4 == 6)
						{
							target0.x += distance;
							if (target0.x > target1.x)
							{
								if (isTurn == false)
								{
									dir4 = getDir(pathMap[p1].point, pathMap[p2].point);
									target0 = pathMap[p1].point;
									target1 = pathMap[p2].point;
									isTurn = true;
									continue;
								}
								else
								{
									break;
								}
							}
							else
							{
								pathxb.point = target0;
								if (isTurn == true)
								{
									pathxb.type = 1;
								}
								if (target0.x != target1.x)
								{
									pathVector.push_back(pathxb);
								}
								if (isTurn == false)
								{
									pathPoint.push_back(target0);
									lastPath.push_back(target0);
								}
								else
								{
									//起点
									if (pathMap[p2].point.x == startX && pathMap[p2].point.y == startY)
									{
										pathPoint.push_back(target0);
									}
								}
							}
						}
						if (dir4 == 0)
						{
							target0.y += distance;
							if (target0.y > target1.y)
							{
								if (isTurn == false)
								{
									dir4 = getDir(pathMap[p1].point, pathMap[p2].point);
									target0 = pathMap[p1].point;
									target1 = pathMap[p2].point;

									isTurn = true;
									continue;
								}
								else
								{
									break;
								}
							}
							else
							{
								pathxb.point = target0;
								if (isTurn == true)
								{
									pathxb.type = 1;
								}
								if (target0.y != target1.y)
								{
									pathVector.push_back(pathxb);
								}
								else
								{
									/*ellipse.ellipse.setRadius(sf::Vector2f(10, 10));
									ellipse.ellipse.setPosition(target0);
									ellipse.ellipse.setFillColor(sf::Color::Blue);
									ellipseVector.push_back(ellipse);*/
								}

								if (isTurn == false)
								{
									pathPoint.push_back(target0);
									lastPath.push_back(target0);
								}
								else
								{
									if (pathMap[p2].point.x == startX && pathMap[p2].point.y == startY)
									{
										pathPoint.push_back(target0);
									}
								}
							}
						}
						if (dir4 == 2)
						{
							target0.x -= distance;
							if (target0.x < target1.x)
							{
								if (isTurn == false)
								{
									dir4 = getDir(pathMap[p1].point, pathMap[p2].point);
									target0 = pathMap[p1].point;
									target1 = pathMap[p2].point;
									isTurn = true;
									continue;
								}
								else
								{
									break;
								}
							}
							else
							{
								pathxb.point = target0;
								if (isTurn == true)
								{
									pathxb.type = 1;
								}
								if (target0.x != target1.x)
								{
									pathVector.push_back(pathxb);
								}
								if (isTurn == false)
								{
									pathPoint.push_back(target0);
									lastPath.push_back(target0);
								}
								else
								{
									if (pathMap[p2].point.x == startX && pathMap[p2].point.y == startY)
									{
										pathPoint.push_back(target0);
									}
								}
							}
						}
						if (dir4 == 4)
						{
							target0.y -= distance;
							if (target0.y < target1.y)
							{
								if (isTurn == false)
								{
									dir4 = getDir(pathMap[p1].point, pathMap[p2].point);
									target0 = pathMap[p1].point;
									target1 = pathMap[p2].point;

									isTurn = true;
									continue;
								}
								else
								{
									break;
								}
							}
							else
							{
								pathxb.point = target0;
								if (isTurn == true)
								{
									pathxb.type = 1;
								}
								if (target0.y != target1.y)
								{
									pathVector.push_back(pathxb);
								}
								if (isTurn == false)
								{
									pathPoint.push_back(target0);
									lastPath.push_back(target0);
								}
								else
								{
									if (pathMap[p2].point.x == startX && pathMap[p2].point.y == startY)
									{
										pathPoint.push_back(target0);
									}
								}
							}
						}
					}

					int i = 0;
					sf::Vector2f target;
					dir4 = getDir(pathMap[p0].point, pathMap[p2].point);

					pathVector.clear();

				}

				pid = pathMap[pid].pid;
				s++;
			}

			sf::Vector2f start, end;
			bool isStart = false, isEnd = false;
			std::vector<sf::Vector2f>::iterator iter;
			int i = 0, startId, c = 0;
			std::vector<sf::Vector2f> path3;
			std::vector<sf::Vector2f> path4;
			bool isxb = false;
			sf::Vector2f lastPoint;
			bool isbreak = false;

			lastPoint.x = -1;
			lastPoint.y = -1;

			sf::Int8 val = -1, mode = -1;
			sf::Vector2f firstPoint, centerPoint, endPoint;
			bool isCenter = false, isTurn = false;
			std::vector<sf::Vector2f> Hpoint, Vpoint, shortPath;
			int e = 0, lastCenter = -1;
			bool isfirst = true;

			i = 0;
			sf::Int8 lastDir = -1;
			while (i < pathPoint.size() - 1)
			{
				i++;
			}

			i = 0;
			isfirst = true;
			sf::Int8 dir1 = -1, dir2 = -1, dir3 = -1, dir4 = -1;
			lastDir = -1;
			bool isBegin = false;
			Hpoint.clear();
			firstPoint.x = firstPoint.y = -1;
			endPoint.x = endPoint.y = -1;
			std::vector<sf::Uint8> dirVector;
			while (i < pathPoint.size() - 1 && 1 > 2)
			{
				dir4 = getDir(pathPoint[i], pathPoint[i + 1]);
				dirVector.push_back(dir4);
				if (isBegin == false)
				{
					for (auto &data : dirVector)
					{
						if (data == 0 && dir4 == 4)
						{
							isBegin = true;
							break;
						}
						if (data == 4 && dir4 == 0)
						{
							isBegin = true;
							break;
						}
						if (data == 2 && dir4 == 6)
						{
							isBegin = true;
							break;
						}
						if (data == 6 && dir4 == 2)
						{
							isBegin = true;
							break;
						}
					}
					if (isBegin == true)
					{
						lastDir = dir4;
					}

				}
				else
				{

				}

				if (isBegin == true)
				{
					dir2 = getDir(pathPoint[i - 1], pathPoint[i]);

					EllipsePublic.setRadius(sf::Vector2f(4, 4));
					EllipsePublic.setPosition(pathPoint[i]);
					EllipsePublic.setFillColor(sf::Color::Green);
					ellipseVector.push_back(EllipsePublic);

					EllipsePublic.setRadius(sf::Vector2f(4, 4));
					EllipsePublic.setPosition(pathPoint[i - 1]);
					EllipsePublic.setFillColor(sf::Color::Blue);
					ellipseVector.push_back(EllipsePublic);

					//std::cout << "dir1" << (int)dir1 << ",dir2" << (int)dir2 << ",dir3" << (int)dir3 << std::endl;
					//break;
					sf::Int8 targetDir = -1;
					if (dir2 == 2)
					{
						targetDir = 6;
					}
					if (dir2 == 6)
					{
						targetDir = 2;
					}
					if (dir2 == 0)
					{
						targetDir = 4;
					}
					if (dir2 == 4)
					{
						targetDir = 0;
					}
					//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds

					//std::cout << ",dir2" << (int)dir2 << std::endl;
					sf::Vector2f point = pathPoint[i];
					bool isSuccess = false;
					sf::Uint8 sum = 0;
					while (true)
					{
						if (targetDir == 0)
						{
							point.y += distance;
						}
						if (targetDir == 4)
						{
							point.y -= distance;
						}
						if (targetDir == 2)
						{
							point.x -= distance;
						}
						if (targetDir == 6)
						{
							point.x += distance;
						}
						if (AiMoveNext(point.x, point.y) == true)
						{
							break;
						}

						for (auto &data : pathPoint)
						{
							if (point == data)
							{
								isSuccess = true;
								break;
							}
							if (data == endPoint)
							{
								break;
							}
						}

						if (isSuccess == true)
						{
							break;
						}
						if (sum >= 10)
						{
							dirVector.clear();
							break;
						}
						sum++;
					}
				}

				i++;
			}

			//到达对面的捷径
			std::vector<sf::Vector2f> shortlj;
			shortPath.clear();
			i = 0;
			while (i < pathPoint.size() - 1)
			{
				shortPath.push_back(pathPoint[i]);
				sf::Uint8 dir1 = getDir(pathPoint[i], pathPoint[i + 1]);
				if (dir1 == 0 || dir1 == 4)
				{
					dir2 = 2;
					dir3 = 6;
				}
				if (dir1 == 2 || dir1 == 6)
				{
					dir2 = 0;
					dir3 = 4;
				}
				sf::Vector2f tpoint = pathPoint[i];
				bool isSuccess = false, first = false;
				shortlj.clear();
				sf::Uint8 tDir = dir2;
				int sum = 0, j = 0;
				while (true)
				{
					if (tDir == 0)
					{
						tpoint.y += distance;
					}
					if (tDir == 4)
					{
						tpoint.y -= distance;
					}
					if (tDir == 2)
					{
						tpoint.x -= distance;
					}
					if (tDir == 6)
					{
						tpoint.x += distance;
					}
					//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds

					shortlj.push_back(tpoint);
					j = i;
					while (j < pathPoint.size())
					{
						if (abs(tpoint.x - pathPoint[j].x) < 0.1 && abs(tpoint.y - pathPoint[j].y) < 0.1)
						{
							isSuccess = true;
							break;
						}
						j++;
					}
					if (isSuccess == true)
					{
						break;
					}

					bool isborder = false;
					if (tpoint.x < 0 || tpoint.x >= gamei.MapDataX * gamei.MapWidthHeight || tpoint.y < 0 || tpoint.y >= gamei.MapDataY * gamei.MapWidthHeight)
					{
						isborder = true;
					}
					else if (AiMoveNext(tpoint.x, tpoint.y) == true || sum >= 20)
					{
						isborder = true;
					}

					if (isborder == true)
					{
						if (first == false)
						{
							first = true;
							sum = 0;
							tDir = dir3;
							tpoint = pathPoint[i];
							shortlj.clear();
						}
						else
						{
							break;
						}

					}

					sum++;
				}

				if (isSuccess == true)
				{
					shortlj.pop_back();
					for (auto &data : shortlj)
					{
						shortPath.push_back(data);
					}

					i = j;
				}
				else
				{
					i++;
				}
				if (i == pathPoint.size() - 1)
				{
					shortPath.push_back(pathPoint[i]);
				}
				//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds

			}

			i = 0;
			//垂直和水平的点, pathPoint是最短的垂直水平路径
			while (i < shortPath.size() - 1 && !shortPath.empty())
			{			
				i++;
			}

			pathPoint.clear();

			i = 0;
			//判断斜边是否可以到达2
			while (i < shortPath.size() - 1 && !shortPath.empty())
			{
				pathPoint.push_back(shortPath[i]);
				sf::Int8 dir = getDir(shortPath[i], shortPath[i + 1]);
				sf::Int8 dir1 = -1, dir2 = -1;
				if (dir == 0)
				{
					dir1 = 1;
					dir2 = 7;
				}
				if (dir == 4)
				{
					dir1 = 3;
					dir2 = 5;
				}
				if (dir == 2)
				{
					dir1 = 3;
					dir2 = 1;
				}
				if (dir == 6)
				{
					dir1 = 5;
					dir2 = 7;
				}

				//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
				sf::Vector2f point = shortPath[i];
				int j = 0;
				int sum = 0;
				bool isSuccess = false;
				bool isfirst = false;
				std::vector<sf::Vector2f> xbVector;
				sf::Uint8 dis = 10, disSum = 0;
				while (true)
				{
					if (dir1 == 1)
					{
						point.x -= dis;
						point.y += dis;
					}
					if (dir1 == 3)
					{
						point.x -= dis;
						point.y -= dis;
					}
					if (dir1 == 5)
					{
						point.x += dis;
						point.y -= dis;
					}
					if (dir1 == 7)
					{
						point.x += dis;
						point.y += dis;
					}
					disSum += dis;

					if (disSum >= distance)
					{
						xbVector.push_back(point);
						j = i;
						while (j < shortPath.size())
						{
							if (abs(shortPath[j].x - point.x) < 0.1 && abs(shortPath[j].y - point.y) < 0.1)
							{
								isSuccess = true;
								break;
							}
							j++;
						}
					}
					if (isSuccess == true)
					{
						break;
					}
					bool isVal = true;
					if (point.x < 0 || point.x >= gamei.MapDataX * gamei.MapWidthHeight || point.y < 0 || point.y >= gamei.MapDataY * gamei.MapWidthHeight || sum >= 20)
					{
						isVal = false;
					}
					else if (AiMoveNext(point.x, point.y) == true)
					{
						isVal = false;
					}

					if (isVal == false)
					{
						if (isfirst == false)
						{
							point = shortPath[i];
							dir1 = dir2;
							sum = 0;
							isfirst = true;
							xbVector.clear();
							disSum = 0;
						}
						else
						{
							break;
						}
					}

					if (disSum >= distance)
					{
						disSum = 0;
						sum++;
					}

				}

				if (isSuccess == true)
				{
					xbVector.pop_back();
					for (auto &data : xbVector)
					{
						pathPoint.push_back(data);
					}
					i = j;
				}
				else
				{
					i++;
				}

				if (i == shortPath.size() - 1)
				{
					pathPoint.push_back(shortPath[i]);
				}

			}

			if (!pathPoint.empty())
			{
				*setVal = pathPoint;

				return true;

			}
			else
			{

			}
			return false;

			break;
		}

		checkX = path.point.x;
		checkY = path.point.y;

		if (path.dir == 0)
		{
			checkY += distance;
			point.dir = 2;
			point2.dir = 6;
			point3.dir = 1;
			point4.dir = 7;
			point5.dir = 3;
			point6.dir = 5;
		}
		if (path.dir == 2)
		{
			checkX -= distance;
			point.dir = 4;
			point2.dir = 0;
			point3.dir = 1;
			point4.dir = 3;
			point5.dir = 5;
			point6.dir = 7;
		}
		if (path.dir == 4)
		{
			checkY -= distance;
			point.dir = 2;
			point2.dir = 6;
			point3.dir = 3;
			point4.dir = 5;
			point5.dir = 1;
			point6.dir = 7;
		}
		if (path.dir == 6)
		{
			checkX += distance;
			point.dir = 4;
			point2.dir = 0;
			point3.dir = 5;
			point4.dir = 7;
			point5.dir = 3;
			point6.dir = 1;
		}

		//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
		if (path.dir == 1)
		{
			checkY += distance;
			checkX -= distance;
			point.dir = 2;
			point2.dir = 0;
			point3.dir = 3;
			point4.dir = 7;
			point5.dir = 4;
			point6.dir = 6;
		}
		if (path.dir == 3)
		{
			checkX -= distance;
			checkY -= distance;
			point.dir = 2;
			point2.dir = 4;
			point3.dir = 1;
			point4.dir = 5;
			point5.dir = 0;
			point6.dir = 6;
		}
		if (path.dir == 5)
		{
			checkY -= distance;
			checkX += distance;
			point.dir = 4;
			point2.dir = 6;
			point3.dir = 3;
			point4.dir = 7;
			point5.dir = 2;
			point6.dir = 0;
		}
		if (path.dir == 7)
		{
			checkX += distance;
			checkY += distance;
			point.dir = 6;
			point2.dir = 0;
			point3.dir = 5;
			point4.dir = 1;
			point5.dir = 2;
			point6.dir = 4;
		}

		//移动到的点是否已经走过了
		bool findit = false;
		for (auto &data : pathMap)
		{
			if (data.second.point.x == checkX && data.second.point.y == checkY)
			{
				findit = true;
				break;
			}
		}

		bool isFindPoint = false;
		if (checkX <= 0 || checkY <= 0 || checkX >= gamei.MapDataX * gamei.MapWidthHeight || checkY >= gamei.MapDataY * gamei.MapWidthHeight || path.sum >= 25)
		{
			isFindPoint = true;
		}
		else if (AiMoveNext(checkX, checkY) == true || findit == true)
		{
			isFindPoint = true;
		}

		if (isFindPoint == true)
		{
			pathMap[path.id].isVal = false;
			bool isval = false, hasVal = false;
			float min = -1;
			sf::Int32 minId = -1, targetId = -1;
			for (auto &data : pathMap)
			{
				if (data.second.isVal == true)
				{
					if (data.second.dis < min || minId == -1)
					{
						min = data.second.dis;
						minId = data.first;
						isval = true;

						if (data.second.dir == getDir(data.second.point, { endX, endY }))
						{
							//targetId = data.first;
							isval = true;
						}
					}
					hasVal = true;
				}
			}

			//没有找到点
			if (isval == false || hasVal == false)
			{
				std::cout << "没有找到点" << std::endl;
				break;
			}
			if (targetId >= 0)
			{
				path.id = targetId;
			}
			else
			{
				path.id = minId;
			}

			path.id = minId;
			path.dir = pathMap[path.id].dir;
			path.point = pathMap[path.id].point;
			path.sum = 0;

		}
		else
		{
			//赋值
			path.point.x = checkX;
			path.point.y = checkY;
			path.sum++;
			findit = false;
			//增加节点
			if (findit == false)
			{
				int xb = sqrt(pow(abs(checkX - endX), 2) + pow(abs(checkY - endY), 2));
				point.point.x = checkX;
				point.point.y = checkY;
				point.dis = xb;
				point.pid = path.id;
				point.id = addid;
				pathMap.insert(std::pair<sf::Uint32, _point>(addid++, point));
				point2.point = point.point;
				point2.dis = xb;
				point2.pid = path.id;
				point2.id = addid;
				pathMap.insert(std::pair<sf::Uint32, _point>(addid++, point2));
				point3.point = point.point;
				point3.dis = xb;
				point3.pid = path.id;
				point3.id = addid;
				point4.point = point.point;
				point4.dis = xb;
				point4.pid = path.id;
				point4.id = addid;
				point5.point = point.point;
				point5.dis = xb;
				point5.pid = path.id;
				point5.id = addid;
				point6.point = point.point;
				point6.dis = xb;
				point6.pid = path.id;
				point6.id = addid;

			}
		}
	}

	std::cout << "while sum:" << a << std::endl;
}

//40,20,10,5,2
void breakAiPath(std::vector<sf::Vector2f> * value, sf::Int8 dis)
{
	sf::Int8 loopTime = 40 / dis;
	std::vector<sf::Vector2f> path;
	path = *value;
	sf::Vector2f pos;
	std::vector<sf::Vector2f> lujingVector;
	for (size_t i = 0; i < path.size() - 1; i++)
	{
		//右上方
		if (path[i + 1].x > path[i].x && path[i + 1].y < path[i].y)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x += dis;
				pos.y -= dis;
				lujingVector.push_back(pos);
			}
		}
		//右下方
		else if (path[i + 1].x > path[i].x && path[i + 1].y > path[i].y)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x += dis;
				pos.y += dis;
				lujingVector.push_back(pos);
			}
		}
		//左下方
		else if (path[i + 1].x < path[i].x && path[i + 1].y > path[i].y)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x -= dis;
				pos.y += dis;
				lujingVector.push_back(pos);
			}
		}
		//左上方
		else if (path[i + 1].x < path[i].x && path[i + 1].y < path[i].y)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x -= dis;
				pos.y -= dis;
				lujingVector.push_back(pos);
			}
		}
		//正右方
		else if (path[i + 1].x > path[i].x && abs(path[i + 1].y - path[i].y) < 0.1)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x += dis;
				lujingVector.push_back(pos);
			}
		}
		//正下方
		else if (path[i + 1].y > path[i].y && abs(path[i + 1].x - path[i].x) < 0.1)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.y += dis;
				lujingVector.push_back(pos);
			}
		}
		//正左方
		else if (path[i + 1].x < path[i].x && abs(path[i + 1].y - path[i].y) < 0.1)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.x -= dis;
				lujingVector.push_back(pos);
			}
		}
		//正上方
		else if (path[i + 1].y < path[i].y && abs(path[i + 1].x - path[i].x) < 0.1)
		{
			lujingVector.push_back(path[i]);
			pos = path[i];
			for (size_t j = 0; j < loopTime - 1; j++)
			{
				pos.y -= dis;
				lujingVector.push_back(pos);
			}
		}

	}

	*value = lujingVector;

}

sf::Int8 getDir(sf::Vector2f start, sf::Vector2f end)
{
	//方向 下0=s,左2=a,上4=w,右6=d,左下1=as,左上3=aw,右上5=dw,右下7=ds
	float dis = 0.1;
	//上
	if (end.y <= start.y && abs(end.x - start.x) <= dis)
	{
		return 4;
	}
	//下
	if (end.y >= start.y && abs(end.x - start.x) <= dis)
	{
		return 0;
	}
	//左
	if (end.x <= start.x && abs(end.y - start.y) <= dis)
	{
		return 2;
	}
	//右
	if (end.x >= start.x && abs(end.y - start.y) <= dis)
	{
		return 6;
	}
	//第一象限
	if (end.x < start.x && end.y < start.y)
	{
		return 3;
	}
	//第二象限
	if (end.x > start.x && end.y < start.y)
	{
		return 5;
	}
	//第三象限
	if (end.x < start.x && end.y > start.y)
	{
		return 1;
	}
	//第四象限
	if (end.x > start.x && end.y > start.y)
	{
		return 7;
	}
	return 0;
}

void ThreadUdpServer()
{
	/*List list;
	char in[128];	//接收的信息
	
	std::size_t received;
	sf::IpAddress sender;
	unsigned short senderPort;

	list.openServer();
	while (1)
	{
		if (list.socket.receive(in, sizeof(in), received, sender, senderPort) == sf::Socket::Done)
		{
			list.runUdpServer(in, sender, senderPort);			
		}
	}*/
}

