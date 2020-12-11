//�����ļ��� 
#include <WinSock2.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h> 
#include"packet.h"
#pragma comment(lib,"ws2_32.lib")
#define DEST_PORT 3000    
#define MAX_DATA 51200  //50kb    
#define DEST_IP_ADDRESS "127.0.0.1"

int contrans(SOCKET sock, Packet* pk, SOCKADDR* addr, int len);

int main(int argc, char* argv[])
{
	WORD ver;
	WSADATA WSAData;
	ver = MAKEWORD(1, 1);
	if (WSAStartup(ver, &WSAData))
	{
		return 0;
	}
	if (LOBYTE(WSAData.wVersion) != 1 ||
		HIBYTE(WSAData.wVersion) != 1)
	{

		WSACleanup();
		return 0;
	}
	//�뱾��IP��

	
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	//unsigned long ul = 1;
	//int ret = ioctlsocket(sock, FIONBIO, (unsigned long*)&ul);//���óɷ�����ģʽ�� 

	//if (ret == SOCKET_ERROR)//����ʧ�ܡ�  
	//{
	//	printf("error/n");
	//}
	//�ͻ��˵�ַ
	SOCKADDR_IN addrS;
	addrS.sin_addr.s_addr = htonl(INADDR_ANY);
	addrS.sin_family = AF_INET;
	addrS.sin_port = htons(8899);
	bind(sock, (SOCKADDR*)&addrS, sizeof(SOCKADDR));
	
	//�������˵�IP��ַ
	SOCKADDR_IN addrR;
	addrR.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrR.sin_family = AF_INET;
	addrR.sin_port = htons(9988);
	int len = sizeof(SOCKADDR);
	//��Ҫ�����ļ�
	BOOL    bSuccess;
	HANDLE  hFile;
	DWORD   dwFileSize, dwHighSize, dwBytesRead;
	hFile = CreateFile("d:\\x.txt", GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("hfle");
		getchar();
		return 0;
	}
	dwFileSize = GetFileSize(hFile, &dwHighSize);//����ļ��Ĵ�С
	if (dwHighSize)
	{
		CloseHandle(hFile);
		printf("hfle2");
		getchar();
		return 0;
	}

	//����ȷ�����ӵ����ݰ�
	int num;
	int rstime;
	int seq;
	int ack;

	num = 1;
	rstime = 0;
	seq = 1;
	ack = 0;
	BYTE* data = new BYTE[4];
	Packet *packet=new Packet(num,rstime,seq,ack,data);
	DWORD retval;

	////��һ������
	sendto(sock, (char*)packet, 20, 0, (SOCKADDR*)&addrR, len);
	//
	//�ڶ�������
	Packet* recvpk = new Packet(num, rstime, seq, ack, data);
	retval = recvfrom(sock, (char*)recvpk, 20, 0, (SOCKADDR*)&addrR, &len);
	recvpk->pkprint();

	packet->ack = recvpk->seq + 1;
	packet->number++;
	packet->seq++;
	//����������

	sendto(sock, (char*)packet, 20, 0, (SOCKADDR*)&addrR, len);

	//contrans(sock, packet, (SOCKADDR*)&addrR, len);
	//�����ļ���С
	char strFileSize[20];
	itoa(dwFileSize, strFileSize, 10);
	sendto(sock, strFileSize, 20, 0, (SOCKADDR*)&addrR, len);
	//���ļ����ݵ�readfile��
	BYTE* readfile;
	readfile = (BYTE*)malloc(dwFileSize);
	bSuccess = ReadFile(hFile, readfile, dwFileSize, &dwBytesRead, NULL);
	CloseHandle(hFile);
	if (!bSuccess || (dwBytesRead != dwFileSize))
	{
		free(readfile);
		printf("hfle3");
		getchar();
		return 0;
	}

	//eachSend��ʾÿ�η����ַ��Ĵ�С
	DWORD eachSend = 50 * 1024, n, yu, i, j; 
	BYTE  eachBuf[50 * 1024];
	n = dwFileSize / eachSend;  //����Ҫ����ȫ���
	yu = dwFileSize % eachSend; //���ʣ�µ��ַ���С
	   //�ȷ���n��ȫ������
	for (i = 0; i < n; i++)
	{
		for (j = 0; j < eachSend; j++)
		{
			eachBuf[j] = readfile[i * eachSend + j];

		}
		retval = sendto(sock, (char*)eachBuf, eachSend, 0, (SOCKADDR*)&addrR, len);
		if (retval != eachSend)
		{
			printf("adsf");
			getchar();
			return 0;
		}
		memset(eachBuf, 0, 50 * 1024);
	}
	//�ٷ���ʣ�µ��ַ�
	for (i = 0; i < yu; i++)
	{
		eachBuf[i] = readfile[n * eachSend + i];
	}
	retval = sendto(sock, (char*)eachBuf, yu, 0, (SOCKADDR*)&addrR, len);
	if (retval != yu)
	{

		return 0;
	}
	//�ж�winsocket��
	closesocket(sock);
	WSACleanup();
	printf("over");
	getchar();
	return 0;
}
