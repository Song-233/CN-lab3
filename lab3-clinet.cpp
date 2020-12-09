//发送文件； 
#include <WinSock2.h>
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h> 
#pragma comment(lib,"ws2_32.lib")
#define DEST_PORT 3000    
#define MAX_DATA 51200  //50kb    
#define DEST_IP_ADDRESS "127.0.0.1" //"192.168.1.113"//"10.42.0.1" //"192.168.1.19"//"169.254.9.36"    


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
	//与本地IP绑定
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN addrS;
	addrS.sin_addr.s_addr = htonl(INADDR_ANY);
	addrS.sin_family = AF_INET;
	addrS.sin_port = htons(8899);
	bind(sock, (SOCKADDR*)&addrS, sizeof(SOCKADDR));
	//接收端的IP地址
	SOCKADDR_IN addrR;
	addrR.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrR.sin_family = AF_INET;
	addrR.sin_port = htons(9988);
	int len = sizeof(SOCKADDR);
	//打开要发送文件
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
	dwFileSize = GetFileSize(hFile, &dwHighSize);//获得文件的大小
	if (dwHighSize)
	{
		CloseHandle(hFile);
		printf("hfle2");
		getchar();
		return 0;
	}
	//发送文件大小
	char strFileSize[20];
	itoa(dwFileSize, strFileSize, 10);
	sendto(sock, strFileSize, 20, 0, (SOCKADDR*)&addrR, len);
	//读文件内容到readfile中
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

	//eachSend表示每次发送字符的大小
	DWORD eachSend = 50 * 1024, retval, n, yu, i, j;
	BYTE  eachBuf[50 * 1024];
	n = dwFileSize / eachSend;  //共需要几次全额发送
	yu = dwFileSize % eachSend; //最后剩下的字符大小
	   //先发送n次全额数据
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
	//再发送剩下的字符
	for (i = 0; i < yu; i++)
	{
		eachBuf[i] = readfile[n * eachSend + i];
	}
	retval = sendto(sock, (char*)eachBuf, yu, 0, (SOCKADDR*)&addrR, len);
	if (retval != yu)
	{

		return 0;
	}
	//中断winsocket库
	closesocket(sock);
	WSACleanup();
	printf("over");
	getchar();
	return 0;
}