//�����ļ���

#include <WinSock2.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h> 
#include <time.h>
#include<ctime>
#include"packet.h"
#pragma comment(lib,"ws2_32.lib")
#define SERV_PORT 3000    
#define MAX_DATA 51200  //50kb    
#define FILE_LENGTH 921654 //ͼƬ��С      

void sev_hands3(SOCKET sock,SOCKADDR* addrS, Packet* recvpk, DWORD retval,int len);

int main()
{
	WORD ver;
	WSADATA WSAData;
	ver = MAKEWORD(1, 1);
	if (WSAStartup(ver, &WSAData))
	{
		printf("adsf2");
		getchar();
		return 0;
	}
	if (LOBYTE(WSAData.wVersion) != 1 ||
		HIBYTE(WSAData.wVersion) != 1)
	{
		printf("adsf3");
		getchar();
		WSACleanup();
		return 0;
	}
	//�뱾��IP��
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN addrR;
	addrR.sin_addr.s_addr = htonl(INADDR_ANY);
	addrR.sin_family = AF_INET;
	addrR.sin_port = htons(9988);
	if (SOCKET_ERROR == bind(sock, (SOCKADDR*)&addrR, sizeof(SOCKADDR)))
	{
		printf("adsf4");
		getchar();
		WSACleanup();
		return 0;
	}


	SOCKADDR_IN addrS;
	addrS.sin_addr.s_addr = htonl(INADDR_ANY);
	addrS.sin_family = AF_INET;
	addrS.sin_port = htons(8899);
	bind(sock, (SOCKADDR*)&addrS, sizeof(SOCKADDR));
	int len = sizeof(SOCKADDR);
	
	//��������

	
	//��һ������
	Packet* recvpk = new Packet;
	DWORD retval;
	retval = recvfrom(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, &len);
	recvpk->pkprint();
	recvpk->ack = recvpk->seq + 1;
	recvpk->seq = 5;
	//�ڶ�������
	sendto(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, len);

	//����������
	recvfrom(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, &len);
	recvpk->pkprint();
	
	sendto(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrS, len);
	//sev_hands3(sock,(SOCKADDR*)&addrS,recvpk,retval, len);


	//��ȡҪ�����ļ��Ĵ�С
	char strFileSize[20];
	recvfrom(sock, strFileSize, 20, 0, (SOCKADDR*)&addrS, &len);
	printf("%s", strFileSize);
	DWORD   dwFileSize;
	dwFileSize = atoi(strFileSize); //�ļ���С
	int a = clock();
	DWORD eachRecv = 50 * 1024, n, yu, i, j;
	n = dwFileSize / eachRecv;
	yu = dwFileSize % eachRecv;
	char  eachBuf[50 * 1024];
	printf("adsf5111\n");

	BYTE* recvbuf = (BYTE*)malloc(dwFileSize);//��ʼ��Ҫ������ݵ��ַ���
	for (i = 0; i < n; i++)
	{
		printf("adsf511133\n");
		retval = recvfrom(sock, eachBuf, eachRecv, 0, (SOCKADDR*)&addrS, &len);
		if (retval != eachRecv)
		{
			printf("adsf5");
			getchar();
			return 0;
		}
		for (j = 0; j < eachRecv; j++)
		{
			recvbuf[i * eachRecv + j] = (BYTE)eachBuf[j];
		}
		memset(eachBuf, 0, 50 * 1024);
	}
	printf("adsf5111\n");
	retval = recvfrom(sock, eachBuf, yu, 0, (SOCKADDR*)&addrR, &len);
	if (retval != yu)
	{
		printf("adsfbbbb");
		return 0;
	}
	for (i = 0; i < yu; i++)
	{
		recvbuf[n * eachRecv + i] = (BYTE)eachBuf[i];
	}
	//д���ļ�
	FILE* fp;
	fp = fopen("d:\\y.txt", "wb");
	fwrite(recvbuf, dwFileSize, 1, fp);
	fclose(fp);
	int b = clock() - a;
	printf("%d", b);
	//�ж�winsocket��
	closesocket(sock);
	WSACleanup();
	getchar();
	return 0;
}


