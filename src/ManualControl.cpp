//#include "NetWorkInit.h"
#include"NetWorkInit.h"
#include "RobotParam.h"
#include"MsgQueue.h"
#include<stack>
#include "Debug_message.h"
//extern HANDLE GetRouteSuccess;
extern int sock_fd;
extern HANDLE ControlSemaphore;
extern HANDLE SendSuccess;
extern MsgQueue *RMqueue;
extern BOOL AfterManual;
int Get_Command(int sock_fd);
void SendMoveCommand(int param1,int param2,int param3);
void MannulControl()
{   
	WaitForSingleObject(ControlSemaphore,INFINITE);
	PRINTF_LOG("�����ֶ�����\n");
	SendMoveCommand(Stop,0,0);
	stack<int> commandstack;
	while(1)
	{
		
		int command=0;
		while(command==0){
		command=Get_Command(sock_fd);
		if(command!=0)break;
		PRINTF_LOG("��ȡ������\n");
		Sleep(1500);
		}
		if(command==7)
		{
			PRINTF_LOG("���������Զ�����");
			AfterManual=1;
			break;
		}
		switch (command)
		{
		case 1:
			{
			PRINTF_LOG("ǰ��\n");
			SendMoveCommand(GO10,0,0);
			commandstack.push(2);
			break;
			   
			 };
		case 2:
			{
			PRINTF_LOG("����\n");
			SendMoveCommand(BACK10,0,0);
			commandstack.push(1);
			break;
			};
		case 3:
			{
			PRINTF_LOG("����\n");
			SendMoveCommand(LEFT10,0,0);
			commandstack.push(4);
			break;
			};
		case 4:
			{
			PRINTF_LOG("����\n");
			SendMoveCommand(RIGHT10,0,0);
			commandstack.push(3);
			break;
			};
		default:
			break;
		}

	}
	/****��������***/
	while(!commandstack.empty())
	{
		switch(commandstack.top())
		{
		case 1:
			{
				PRINTF_LOG("ǰ��\n");
				SendMoveCommand(GO10,0,0);
				commandstack.pop();
				break;
			};
		case 2:
			{
				PRINTF_LOG("����\n");
				SendMoveCommand(BACK10,0,0);
				commandstack.pop();
				break;
			};
		case 3:
			{
				PRINTF_LOG("����\n");
				SendMoveCommand(LEFT10,0,0);
				commandstack.pop();
				break;
			};
		case 4:
			{
				PRINTF_LOG("����\n");
				SendMoveCommand(RIGHT10,0,0);
				commandstack.pop();
				break;
			};
		default:
			break;
		}

	}
	ReleaseSemaphore(ControlSemaphore, 1, NULL);
	PRINTF_LOG("�˳��ֶ�����\n");
	return;
}

unsigned _stdcall ManualControl(void* param)
{
	 // �ȴ� ����·�������� ִ�д˲�����
	//WaitForSingleObject(GetRouteSuccess,INFINITE);
	int command = 0 ;
	while(1)
	{
		command = Get_Command(sock_fd);
		if(command == 6)
		{   
			PRINTF_LOG("�ֶ������߳����� .\n");
			MannulControl();
		}
		Sleep(2000);
	}



}