#include <WinSock2.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h> 
#include <time.h>
#include<ctime>
#include"packet.h"
#pragma comment(lib,"ws2_32.lib")

//addr�������˵�ַ
int contrans(SOCKET sock, Packet* pk,SOCKADDR* addr,int len)
{
	clock_t now = clock();
	sendto(sock, (char*)pk, 20, 0, (SOCKADDR*)&addr, len);
	if (recvfrom(sock, (char*)pk, 20, 0, (SOCKADDR*)&addr, &len)!=-1) {
		now = clock();
		pk->ack = ++pk->seq + 1;
		return 0;
	}
	else {
		if (clock() - now >= 3) {
			pk->reSendTimes++;
			if (pk->reSendTimes > 3)
				return 1;
			contrans(sock, pk, addr, len);
		}
	}
}

void sev_hands3(SOCKET sock, SOCKADDR* addrS, Packet* recvpk, DWORD retval, int len) {
	//��һ������
	retval = recvfrom(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, &len);
	recvpk->pkprint();
	recvpk->ack = recvpk->seq + 1;
	recvpk->seq = 5;
	//�ڶ�������
	sendto(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, len);

	//����������
	recvfrom(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, &len);
	recvpk->pkprint();
}