#pragma once
#include "Network.h"
using namespace std;

class List {
public:
	List() { create_List(); }
	~List() { clear(); }
	//����ͷ���
	void create_List();
	//���뺯��
	void insert(const int& , int, int,string, unsigned short int);
	//��ָ��λ�ò���
	void insert_pos(const int& d, const int& d1);
	//ɾ��ָ�����ݵĽڵ�
	void erase(const int& d);
	//�޸�ָ������
	void updataPos(const int& , const int&, const int&);
	//��ת������
	void reverse();
	//��ӡ
	void print();
	//��ȡλ����Ϣ

	sf::UdpSocket socket;
	void openServer();
	void runUdpServer(char *in,sf::IpAddress,unsigned short);

private:
	Network net;
	int connectSum = 1;		//��ǰ�ж�����������
	int listAdd = 0;		//������Ϊֹ�ܹ���������

	//�ڵ�ṹ
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
	Node * head;//ͷ�ڵ�

	//����������
	void clear() {
		Node * p = head;
		//��ͷ�ڵ㿪ʼѭ��ɾ��
		while (p) {
			Node * q = p->next;
			delete p;
			p = q;
		}
	}
	//��������d����һ���ڵ�λ�õĺ���
	//Ϊ�˷������ɾ������
	Node* find(const int& d) {
		Node * p = head;
		for (; p; p = p->next) {
			if (p->next->uid == d)
				break;
		}
		return p;
	}
};
