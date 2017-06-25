/*****************************************************************************
							服务器通信接口
	
	//http://121.42.59.34:8088/admin.php/MapEstablish/ReceiveFile

******************************************************************************/


#ifndef _NETWORKINIT_H_
#define _NETWORKINIT_H_
#include <WINSOCK2.H>
#include <stdio.h>
#include<string.h>
#define SERVER_ADDR				"192.168.2.129" //"192.168.3.5"
#define SERVER_PORT				 8090 //8090
#define PATH1 "/SDrobotNavigate2/RoutineXML/"
#define PATH2 "map7/robotRoutine.xml"
#define PATH3 "./conf/receive.xml"
#define PATH4 "/SDrobotNavigate2/admin.php/AutoDrive/ReceivePhoto"
#define PATH5 "/SDrobotNavigate2/admin.php/"
#define PATH6 "AutoDrive/controlMove"
#define PATH7 "/SDrobotNavigate2/admin.php/"
#define PATH8 "AutoDrive/feedback"
#define BUFSIZE 10240
#define BUFSIZE2 1024
//#define BUFSIZE					1024
#define PIC_LEN					200
#define BOUND_LEN				20
int XMLinit(); 
void NetWorkInit();
void NetWorkClose();
int Get_Route(int sock_fd);
int Get_Route_SUCCESS(int sock_fd);
int Get_Command(int sock_fd);
int sock_init(const char* ip,int port , struct sockaddr_in *pserv_addr );
int send_location(int http_socket);
extern int sock_fd;
extern int http_socket;
//int Sendpicture(int sock_fd,char *picname);
#endif
