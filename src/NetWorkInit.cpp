#include<iostream>
#include"NetWorkInit.h"
#include "Debug_message.h"
int sock_fd = 0;
int http_socket = 0;
using namespace std;
void NetWorkInit()
{
	struct sockaddr_in serv_addr;
    while((http_socket = sock_init(SERVER_ADDR , SERVER_PORT ,&serv_addr)) < 0)
    {
		PRINTF_LOG("[%s]:connect server fail .\n",__FUNCTIONW__);
		Sleep(1000);
    }
}
void NetWorkClose(){
	closesocket(http_socket);
	WSACleanup();
	http_socket = 0;
}