//
//  List.cpp
//  List
//
//  Created by scandy_yuan on 13-1-6.
//  Copyright (c) 2013�� Sam. All rights reserved.
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
	
	char sendmsg[128];	//���͵���Ϣ

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
				//command create player uid at posx posy anim dir �����û�
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
					//�Լ�����Ϣ���͸�������
					for (Node * p = head->next; p; p = p->next) {
						//�����Լ������Լ�
						if (p->uid != listAdd) {
							if (socket.send(sendmsg, sizeof(sendmsg), p->ip, p->port) == sf::Socket::Done)
							{

							}
						}
					}
					std::cout << "send to every client:"<< sendmsg << std::endl;

					//�������˵���Ϣ���͸��Լ�
					for (Node * p = head->next; p; p = p->next) {
						//�����Լ������Լ�
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
								//command set player id pos x y anim dir; ���ý�ɫ����,���߶�������֡,�泯����
								//��������
								updataPos(uid, posX, posY);

								//�����귢�͸�������
								for (Node * p = head->next; p; p = p->next) 
								{	
									//�����Լ������Լ�
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
//����ͷ���
void List::create_List()
{
	head = new Node(0);
}
//��ͷ����һ���ڵ�
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
//��ӡ����
void List::print()
{
	for (Node * p = head->next; p; p = p->next) {
		std::cout << p->uid <<","<< p->posX << "," << p->posY << std::endl;
	}
	
}



//��dλ��֮ǰ����d1
void List::insert_pos(const int& d, const int& d1)
{
	Node * p = find(d);
	Node * q = new Node(d1);
	q->next = p->next;
	p->next = q;
}

//ɾ��
void List::erase(const int& d)
{
	Node * p = find(d);
	//��Ϊp����һ���ڵ��λ�ã���q������
	//Ҫɾ���Ľڵ�ĵ�ַ
	Node *q = p->next;
	//ͨ������һ���ڵ��nextָ��ָ��Ҫɾ���ڵ��nextָ
	//��־��Ľڵ�ʵ�ֶϿ�Ҫɾ���ڵ��Ŀ��
	p->next = p->next->next;
	//ɾ��Ҫɾ���Ľڵ�q
	delete q;
}

//�޸�ָ������
void List::updataPos(const int& uid, const int& posX, const int& posY)
{
	Node * p = find(uid);
	p->next->posX = posX;
	p->next->posY = posY;
}

//��ת����
void List::reverse()
{
	Node * p = head->next;//ͷ���֮��ĵ�1���ڵ�
	Node * q = head->next->next;//ͷ���֮��ĵ�2�ڵ�
	Node * m = head->next->next->next;//ͷ���֮��ĵ�3���ڵ�
	p->next = NULL;//��ͷ�ӵ�֮��ĵ�1���ڵ��nextָ����Ϊ��
				   //����m�Ƿ�Ϊ�����ж� �Դ�����ÿһ���ڵ�
	while (m) {
		q->next = p;
		p = q;
		q = m;
		m = m->next;
	}
	//�����һ���ڵ�����
	q->next = p;
	//��ͷ����ָ���µĵĵ�1���ڵ�(֮ǰ�����һ���ڵ�)
	head->next = q;
}
