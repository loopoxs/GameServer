#pragma once
#include "Network.h"
using namespace std;

class List {
public:
	List() { create_List(); }
	~List() { clear(); }
	//创建头结点
	void create_List();
	//插入函数
	void insert(const int& , int, int,string, unsigned short int);
	//在指定位置插入
	void insert_pos(const int& d, const int& d1);
	//删除指定数据的节点
	void erase(const int& d);
	//修改指定数据
	void updataPos(const int& , const int&, const int&);
	//反转链表函数
	void reverse();
	//打印
	void print();
	//获取位置信息

	sf::UdpSocket socket;
	void openServer();
	void runUdpServer(char *in,sf::IpAddress,unsigned short);

private:
	Network net;
	int connectSum = 1;		//当前有多少链接数量
	int listAdd = 0;		//到现在为止总共链接数量

	//节点结构
	struct Node {
		int uid;	//
		string playerName;	
		unsigned short int	mapid = 0;
		int posX, posY;
		string ip;
		unsigned short	port;


		Node * next;
		Node(const int& d) :uid(d), next(NULL) {}
	};
	Node * head;//头节点

	//清理链表函数
	void clear() {
		Node * p = head;
		//从头节点开始循环删除
		while (p) {
			Node * q = p->next;
			delete p;
			p = q;
		}
	}
	//查找数据d的上一个节点位置的函数
	//为了方便后面删除操作
	Node* find(const int& d) {
		Node * p = head;
		for (; p; p = p->next) {
			if (p->next->uid == d)
				break;
		}
		return p;
	}
};
