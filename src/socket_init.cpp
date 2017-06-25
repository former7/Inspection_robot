#include "NetWorkInit.h"
#include "Debug_message.h"
#include <stdlib.h>

/* socket initialize , only call once */
void oops(void *msg){
	perror( (char*)msg );
	exit(EXIT_FAILURE);
}
int sock_init(const char* ip,int port , struct sockaddr_in *pserv_addr ){
	int addrlen ;

	WORD sockVersion = MAKEWORD(2,2);
	WSADATA data; 
	SOCKET conn_fd;

	if(WSAStartup(sockVersion, &data) != 0)
		oops("WSAStartup:");

	//init socket
	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd == INVALID_SOCKET)
		{
			oops("invalid socket:");
			return -1;

	}
	//init adddr
	pserv_addr->sin_family = AF_INET;
	pserv_addr->sin_port = htons(SERVER_PORT);
	pserv_addr->sin_addr.S_un.S_addr = inet_addr(SERVER_ADDR); 

	// set sock opt 
	//int keepalive = 1; // ����keepalive����
	//int keepidle = 60; // ���������60����û���κ���������,�����̽��
	//int keepinterval = 5; // ̽��ʱ������ʱ����Ϊ5 ��
	//int keepcount = 3; // ̽�Ⳣ�ԵĴ���.�����1��̽������յ���Ӧ��,���2�εĲ��ٷ�.
	//setsockopt(conn_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive , sizeof(keepalive ));
	//setsockopt(conn_fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepidle , sizeof(keepidle ));
	//setsockopt(conn_fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepinterval , sizeof(keepinterval ));
	//setsockopt(conn_fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcount , sizeof(keepcount ));



	//connect
	addrlen = sizeof( struct sockaddr );
	if (connect(conn_fd, (sockaddr *)pserv_addr, addrlen) == SOCKET_ERROR){
		PRINTF_LOG("connect error !");
		closesocket(conn_fd);
		return -1;
	}

	PRINTF_LOG("We have connect the server ok sockefd = %d .\n",conn_fd);  

	return conn_fd;
}
