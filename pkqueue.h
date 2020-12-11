#pragma once
#ifndef _PKQUEUE_H_
#define _PKQUEUE_H_

#include <stdio.h>    
#include <stdlib.h>
#include<string>
#include <WinSock2.h>    
#include <string.h> 
#include<ctime>
#include"packet.h"

class pknode {
public:
	Packet *packet;
	pknode* next;
	pknode() {}
	pknode(Packet* packet) {
		this->packet = packet;
		next = NULL;
	}
};

class pkqueue {
public:
	pknode* head;
	int num;
	pkqueue() {
		this->num = 0;
	}
	pkqueue(pknode* head, int num) {
		this->head = head;
		this->num = num;
	}
	void insert(pknode* newnode) {
		if (!head) {
			head = newnode;
			num++;
		}
	}
	void del() {
		pknode* p;
		pknode* temp = head;
		for (temp; temp->next; temp = temp->next) {
			p = temp;
		}
		p->next = NULL;
		delete(temp);
	}
};
#endif