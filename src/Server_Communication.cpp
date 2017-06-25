#include <iostream>
#include "tcp_trans_info.h"
#include "NetWorkInit.h"
#include <WINSOCK2.H>
#include <stdio.h>
#include<string.h>
#include "Debug_message.h"
#if _MSC_VER
#define snprintf _snprintf
#endif
#pragma  comment(lib,"ws2_32.lib")
//��ȡ�滮·��
int string_find( char str[], char substr[] )
{
    int i, j, check ,count = 0;
    int len = strlen( str );        /*ȡ���ַ������ȣ�������'\0'*/
    int sublen = strlen( substr );
    for( i = 0; i < len; i++ )
    {
        check = 1;        /*�����*/
        for( j = 0; j + i < len && j < sublen; j++ )        /*����ַ����м�⣬��sublen�����ڣ�һ�����ֲ�ͬ�ַ��㽫check��Ϊ0*/
        {
            if( str[i + j] != substr[j] )
            {
                check = 0;
                break;
            }
        }
        if( check == 1 )        /*��sublen�����ڵ��ַ������*/
        {
        
            i = i + sublen;        /*���������ʼλ��*/
			return i;
        }
    }
    
}
int Get_Route_SUCCESS(int sock_fd)
{
	char tmp[BUFSIZE+1];
	snprintf( tmp , sizeof(tmp) , "GET %s%s HTTP/1.1\r\n" , PATH7,PATH8);
	snprintf(tmp+strlen(tmp),BUFSIZE,"Host:%s:%d\r\n",SERVER_ADDR,SERVER_PORT);  
   	snprintf(tmp+strlen(tmp),BUFSIZE,"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Accept-Language:zh-cn\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Connection::Keep-Alive\r\n\r\n"); 
	NetWorkInit();
	//send(sock_fd, tmp, strlen(tmp), 0);
	PRINTF_LOG("[%s]: buf = %s \n",__FUNCTION__,tmp);
	send(http_socket, tmp, strlen(tmp), 0);
	NetWorkClose();
    return 1;
}
int Get_Route(int sock_fd)
{
	
	int  read_num;
	int  command=0;
	char buf[BUFSIZE+1];
	char tmp[BUFSIZE+1];

	snprintf( tmp , sizeof(tmp) , "GET %s%s HTTP/1.1\r\n" , PATH1,PATH2);
	snprintf(tmp+strlen(tmp),BUFSIZE,"Host:%s:%d\r\n",SERVER_ADDR,SERVER_PORT);  
   	snprintf(tmp+strlen(tmp),BUFSIZE,"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Accept-Language:zh-cn\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Connection::Keep-Alive\r\n\r\n"); 
	PRINTF_LOG( "[%s]:tmp :%s\n" ,__FUNCTION__, tmp);
	
	// http �����ӣ�����һ�ζϿ�һ�� ʹ��http_socket
	NetWorkInit();
	send(http_socket, tmp, strlen(tmp), 0);
	//read_num = recv(sock_fd,buf,sizeof(buf),0);
	read_num = recv(http_socket,buf,sizeof(buf),0);
	NetWorkClose();
	if (read_num>0)
	{
		PRINTF_LOG("[%s] recv from server :\nbuf = %s \n",__FUNCTION__,buf);
		buf[read_num] = '\0';
		
		char *p;
		p=buf;
		bool flag=0;
		int buflength=sizeof(buf);
		while(*p!=EOF)
		{
		   
			if(*p=='"')
			{
				if(*(p+1)=='?')
				{
					if(*(p+2)=='>')
					{
						p+=4;
						flag=1;
						break;
					}
				}
			}
					p++;
		}
		if(!flag)
		{
			PRINTF_LOG("[%s]: flag is Error .\n",__FUNCTION__);
			return 0;
		}
		FILE *route=fopen(PATH3,"w");
		//if(*p==EOF)return 0;
		fprintf(route,"%s",p);
		fclose(route);		
	}
	else {
		PRINTF_LOG("[%s] :recv error .read_num = %d .\n",__FUNCTION__,read_num);
		return 0;
	}
	Sleep(50);
	XMLinit();//����XML 
	return 1;
}
//��ȡ�ֶ�����ָ��
int Get_Command(int sock_fd){

	int back_val = 0 ;
	int  read_num;
	int command=0;
	char buf[1024];
	char tmp[4096];

	snprintf( tmp , sizeof(tmp) , "GET %s%s HTTP/1.1\r\n" , PATH5,PATH6);
	snprintf(tmp+strlen(tmp),BUFSIZE,"Host:%s:%d\r\n",SERVER_ADDR,SERVER_PORT);  
	snprintf(tmp+strlen(tmp),BUFSIZE,"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Accept-Language:zh-cn\r\n");  
	snprintf(tmp+strlen(tmp),BUFSIZE,"Connection::Keep-Alive\r\n\r\n"); 
	//PRINTF_LOG( "tmp :%s\n" , tmp);

	NetWorkInit();
	//send(sock_fd, tmp, strlen(tmp), 0);
	back_val = send(http_socket, tmp, strlen(tmp), 0);
	if (back_val<0)
	{
		PRINTF_LOG("[%s]:back_val = %d .\n",__FUNCTION__,back_val);
	}
	
	read_num = recv( http_socket , buf , sizeof(buf) ,0);
	NetWorkClose();

	if(read_num > 0)
	{
		PRINTF_LOG("[%s]:BUF = %s \n",__FUNCTION__,buf);
		//PRINTF_LOG("[%s]:Ask server Command OK\n",__FUNCTION__);
		char *p;
		int i=0;
		//FILE *route=fopen(PATH3,"w");
		p=buf;
		while(*p!=EOF)
		{
			if(*p=='n'&&(i+5)<=read_num)
			{
				if(*(p+1)=='"')
				{
					if(*(p+2)==':')
					{
						if(*(p+3)=='"')
						{
							p+=4;
							break;
						}
					}
				}
			}
			p++;
			i++;
		}

		if(*p!=EOF)
		{
			PRINTF_LOG("p=%d\n",(*p-48));
		    command=(*p-48);
		}
		else {
			PRINTF_LOG("[%s]: end of file ,can not find command flag .\n",__FUNCTION__);
			return 0;
		}
	}
	else if(read_num == 0)
	{
		PRINTF_LOG("[%s]:server have close the connect .\n",__FUNCTION__);
	}
	else{
		PRINTF_LOG("[%s]:read_num = %d",__FUNCTION__,read_num);
	}

	return command;
}
/*
int Sendpicture(int sock_fd,char *picname){
	char pic_path[PIC_LEN] =".\\conf\\";
	snprintf( pic_path+strlen(pic_path) ,PIC_LEN , "%s",picname);
	sendPic(sock_fd ,PATH4, SERVER_ADDR, pic_path); 
	return 1;
}
*/
