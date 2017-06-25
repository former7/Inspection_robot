
#include <WINSOCK2.H>
#include <stdio.h>
#include<string.h>
#include"RobotStatusManager.h"
#pragma  comment(lib,"ws2_32.lib")
#include "Debug_message.h"
//int Route_Arry[200][2];
/*
	socket¿Í»§¶Ë
 */


#if _MSC_VER
#define snprintf _snprintf
#endif

#define SERVER_ADDR "192.168.0.112"
#define SERVER_PORT 80 
#define PATH1 "/SDrobotNavigate2/admin.php"
//#define PATH2 "/AutoDrive/realLocRobot?

#define BUFSIZE 10240
extern int sock_fd;
extern RobotStatusManager RobotState;
int send_location(int http_socket)
{
	char tmp[BUFSIZE+1];
	Location robotlocation;

	robotlocation=RobotState.getRobotLocation();
	char PATH2[40]= "/AutoDrive/realLocRobot?";
	char pathtemp1[]="x=";
	char pathtemp2[]="&y=";
	char pathtemp3[]="\"";
	char str1[5];
	char str2[5];
	itoa(robotlocation.x,str1,10);
	itoa(robotlocation.y,str2,10);
	strcat(PATH2,pathtemp1);
	strcat(PATH2,str1);
	strcat(PATH2,pathtemp2);
	strcat(PATH2,str2);
	strcat(PATH2,pathtemp3);

	snprintf( tmp , sizeof(tmp) , "GET http://%s:%d%s%s HTTP/1.0\r\n" , SERVER_ADDR,SERVER_PORT,PATH1,PATH2);
   	strcpy(tmp+strlen(tmp),"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n");  
	strcpy(tmp+strlen(tmp),"Accept-Language:zh-cn\r\n");  
	strcpy(tmp+strlen(tmp),"Connection:Keep-Alive\r\n\r\n"); 
	//PRINTF_LOG( "tmp :%s\n" , tmp);


	//send(sock_fd, tmp, strlen(tmp), 0);
	int back_val = 0 ;
	back_val = send(http_socket, tmp, strlen(tmp), 0);
	PRINTF_LOG("[%s]: send back_val = %d .\n",__FUNCTION__,back_val);
	
    return 0;
}