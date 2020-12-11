#pragma once
#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>    
#include <stdlib.h>
#include<string>
#include <WinSock2.h>    
#include <string.h> 
#include<ctime>

using namespace std;

class Packet {
public:
	int number;//���ı��
	int reSendTimes;//�ش�����
	int seq;
	int ack;
	BYTE* data;//��ŵ�����
	Packet(int number, int reSendTimes,int seq,int ack,BYTE* data) {
		this->number = number;
		this->reSendTimes = reSendTimes;
		this->seq = seq;
		this->ack = ack;
		this->data = data;
	}
	Packet() {}
	void pkprint() {
		printf("NUM:%d \nRSNUM:%d\nseq:%d\nack:%d\n", number, reSendTimes, seq, ack);
	}

};

#endif
