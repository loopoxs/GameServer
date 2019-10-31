//
//  List.cpp
//  List
//
//  Created by scandy_yuan on 13-1-6.
//  Copyright (c) 2013年 Sam. All rights reserved.
//

#include "stdafx.h"
#include "List.h"
#include "Network.h"

void List::openServer()
{
	const unsigned short port = 50001;
	// Listen to messages on the specified port
	if (socket.bind(port) != sf::Socket::Done)
		return;
	std::cout << "Server is listening to port " << port << ", waiting for a message... " << std::endl;
}

void List::runUdpServer(char * in,sf::IpAddress sender, unsigned short senderPort)
{
	vector<string> vec;
	string strcom;
	
	char sendmsg[128];	//发送的信息

	string command[10];
	for (int i = 0, j = 0, n = 0; i < strlen(in); ++i)
	{
		if (in[i] == ' ')
		{
			string strTemp(in + j, i - j);
			vec.push_back(strTemp);
			j = i + 1;
			command[n] = strTemp;

			n++;
		}
	}
	for (size_t i = 0; i < sizeof(command) / sizeof(command[0]) - 1; i++)
	{
		if (command[i] != "")
		{
			command[9] += command[i] + " ";
		}
	}
	strcom = command[9];

	if (command[0] == "command")
	{
		if (command[1] == "create")
		{
			if (command[2] == "player")
			{
				//command create player uid at posx posy anim dir 创建用户
				strcom += std::to_string(listAdd) + " at " + std::to_string(50) + " " + std::to_string(50) + " 0 0 ";
				strcpy_s(sendmsg, strcom.c_str());
				if (socket.send(sendmsg, sizeof(sendmsg), sender, senderPort) == sf::Socket::Done)
				{
					std::cout << "send to "<< sender << ": " << strcom.data() << "'" << std::endl;					
					insert(listAdd, 50, 50, sender.toString(), senderPort);
					//sf::IpAddress::toString();
				}

				
				if (connectSum > 1)
				{
					//自己的信息发送给其他人
					for (Node * p = head->next; p; p = p->next) {
						//不用自己发给自己
						if (p->uid != listAdd) {
							if (socket.send(sendmsg, sizeof(sendmsg), p->ip, p->port) == sf::Socket::Done)
							{

							}
						}
					}
					std::cout << "send to every client:"<< sendmsg << std::endl;

					//把其他人的信息发送给自己
					for (Node * p = head->next; p; p = p->next) {
						//不用自己发给自己
						if (p->uid != listAdd) {
							strcom = command[9];
							strcom += std::to_string(p->uid) + " at " + std::to_string(p->posX) + " " + std::to_string(p->posY) + " ";
							strcpy_s(sendmsg, strcom.c_str());

							if (socket.send(sendmsg, sizeof(sendmsg), sender, senderPort) == sf::Socket::Done)
							{
								//std::cout << "send to " << sender << ":" << iter->nNumber << "," << iter->posX << "," << iter->posY << std::endl;
							}
						}
					}
					
				}


				listAdd++;
				connectSum++;
			}
		}

		char msg[128];
		strcpy_s(msg, in);
		
		
		

		int posY = atoi(command[6].c_str());
		//strcpy_s(sendmsg, command[9].data());	//
		if (command[1] == "set")
		{
			if (command[2] == "player")
			{
				int uid = atoi(command[3].c_str());
				if (uid >= 0)
				{
					if (command[4] == "pos")
					{
						int posX = atoi(command[5].c_str());
						int posY = atoi(command[6].c_str());
						if (posX >= 0 && posY >= 0)
						{
							unsigned short int anim = atoi(command[7].c_str());
							unsigned short int dir = atoi(command[8].c_str());
							if (anim >= 0 && dir >= 0)
							{
								//command set player id pos x y anim dir; 设置角色坐标,行走动画序列帧,面朝方向
								//更新坐标
								updataPos(uid, posX, posY);

								//把坐标发送给所有人
								for (Node * p = head->next; p; p = p->next) 
								{	
									//不用自己发给自己
									if(uid != p->uid)
										if (socket.send(msg, sizeof(msg), p->ip, p->port) == sf::Socket::Done)
										{
											//std::cout << "send to " << sender << ":" << iter->nNumber << "," << iter->posX << "," << iter->posY << std::endl;
										}									
								}		
								std::cout << "send pos to every client:" << in << std::endl;
							}
						}
					}
				}
			}
		}






	}


}
//创建头结点
void List::create_List()
{
	head = new Node(0);
}
//从头插入一个节点
void List::insert(const int& id,int x,int y,string ip,unsigned short int port)
{
	Node *p = new Node(id);
	p->posX = x;
	p->posY = y;
	p->ip = ip;
	p->port = port;

	p->next = head->next;
	head->next = p;
}
//打印函数
void List::print()
{
	for (Node * p = head->next; p; p = p->next) {
		std::cout << p->uid <<","<< p->posX << "," << p->posY << std::endl;
	}
	
}



//在d位置之前插入d1
void List::insert_pos(const int& d, const int& d1)
{
	Node * p = find(d);
	Node * q = new Node(d1);
	q->next = p->next;
	p->next = q;
}

//删除
void List::erase(const int& d)
{
	Node * p = find(d);
	//因为p是上一个节点的位置，用q来保存
	//要删除的节点的地址
	Node *q = p->next;
	//通过将上一个节点的next指针指向要删除节点的next指
	//针志向的节点实现断开要删除节点的目的
	p->next = p->next->next;
	//删除要删除的节点q
	delete q;
}

//修改指定数据
void List::updataPos(const int& uid, const int& posX, const int& posY)
{
	Node * p = find(uid);
	p->next->posX = posX;
	p->next->posY = posY;
}

//反转链表
void List::reverse()
{
	Node * p = head->next;//头结点之后的第1个节点
	Node * q = head->next->next;//头结点之后的第2节点
	Node * m = head->next->next->next;//头结点之后的第3个节点
	p->next = NULL;//将头接点之后的第1个节点的next指针置为空
				   //根据m是否为空来判断 以此逆序每一个节点
	while (m) {
		q->next = p;
		p = q;
		q = m;
		m = m->next;
	}
	//将最后一个节点逆序
	q->next = p;
	//将头从新指向新的的第1个节点(之前的最后一个节点)
	head->next = q;
}
