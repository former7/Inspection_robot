#include <stdio.h>  
#include <winsock2.h>
#include "OrbDataInterface.h"
#include "Debug_message.h"
#pragma comment(lib,"ws2_32.lib")
send_info OrbData;
unsigned _stdcall  OrbDataInterface(void* param)
{
		WORD sockVersion = MAKEWORD(2,2);  
	WSADATA wsaData;  
	if(WSAStartup(sockVersion, &wsaData)!=0)  
	{  
		return 0;  
	}  

	//创建套接字  
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  
	if(slisten == INVALID_SOCKET)  
	{  
		PRINTF_LOG("socket error !");  
		return 0;  
	}  

	//绑定IP和端口  
	sockaddr_in sin;  
	sin.sin_family = AF_INET;  
	sin.sin_port = htons(8088);  
	sin.sin_addr.S_un.S_addr = INADDR_ANY;   
	if(bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)  
	{  
		PRINTF_LOG("bind error !");  
	}  

	//开始监听  
	if(listen(slisten, 5) == SOCKET_ERROR)  
	{  
		PRINTF_LOG("listen error !");  
		return 0;  
	}  

	//循环接收数据  
	//SOCKET sClient;  
	//sockaddr_in remoteAddr;  
	//int nAddrlen = sizeof(remoteAddr);  
	// char revData[255];   
	PRINTF_LOG("等待连接...\n"); 
	//memset(&OrbData,0,sizeof(OrbData));
	while (true)
	{
		SOCKET clientSocket = accept(slisten, NULL, NULL);
		if (clientSocket == INVALID_SOCKET)
		{
			PRINTF_LOG("accept error !");  
			continue; 
		}
		else
		{   
			PRINTF_LOG("ok\n");
			while(1)
			{   
				char buf[BUFSIZE+1]; //1024*10
				memset(buf, 0, BUFSIZE);
				//接收客户端传输过来的数据
				int n = recv(clientSocket, buf, BUFSIZE, 0);
				buf[BUFSIZE] = '\0';
				PRINTF_LOG("n=%d\n",n);
				if(n>0)
				{   
					
					memset(&OrbData,0,sizeof(OrbData));
					memcpy(&OrbData,buf,sizeof(OrbData));
					PRINTF_LOG("%f %f %f %d\n",OrbData.x,OrbData.y,OrbData.angle,OrbData.barriers);
				   
				}
				else
					break;
				
			}



		}
	}

	//有客户链接请求被成功接收


	closesocket(slisten);  
	WSACleanup();  
	return 0;  
	
}
unsigned _stdcall  OrbDataInterface2(void* param)
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        PRINTF_LOG("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.                                  */
        PRINTF_LOG("Could not find a usable version of Winsock.dll\n");
        WSACleanup();
        return 1;
    }
    else
        PRINTF_LOG("The Winsock 2.2 dll was found okay\n");

    SOCKET sockSrv = socket(AF_INET,SOCK_DGRAM, 0);

    SOCKADDR_IN  addrServ;
    addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrServ.sin_family = AF_INET;
    addrServ.sin_port = htons(8088);

    bind(sockSrv,(SOCKADDR*)&addrServ,sizeof(SOCKADDR));
	PRINTF_LOG("等待连接...\n");
    SOCKADDR_IN  addrClient;
    int length = sizeof(SOCKADDR);
    char recvBuf[BUFSIZE+1];
	/*
	while(1)
	{ 
	  memset(recvBuf, 0, BUFSIZE+1);
      int n=recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&length);
	  PRINTF_LOG("n=%d\n",n);
	  if(n>0)
	  { 
		memset(&OrbData,0,sizeof(OrbData));
		memcpy(&OrbData,recvBuf,sizeof(OrbData));
		PRINTF_LOG("%f %f %f %d\n",OrbData.x,OrbData.y,OrbData.angle,OrbData.barriers);
	  }
	}
	*/
	
	while(1)
	{ 
	  memset(recvBuf, 0, BUFSIZE+1);
      int n=recvfrom(sockSrv,recvBuf,100,0,(SOCKADDR*)&addrClient,&length);
	  PRINTF_LOG("n=%d\n",n);
	  if(n>0)
	  { 
		PRINTF_LOG("%s",recvBuf);
	  }
	}
	
    closesocket(sockSrv);

    WSACleanup();

    system("PAUSE");
    return 0;
}