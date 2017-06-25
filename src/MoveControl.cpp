//#include <windows.h>
#include <process.h>
#include <cstdio>
#include"SerialPort_Robot.h"
#include "RobotParam.h"
#include"MsgQueue.h"
#include<iostream>
#include "Debug_message.h"
using namespace std;
extern HANDLE MoveStartEvent;
extern MsgQueue RMqueue;
extern HANDLE SendSuccess;
unsigned _stdcall MoveControl(void* param)
{   
	Command msg;
	int count=0;
    while(true)
	{   
		if(RMqueue.getMsg(&msg))
		{   
			/*cout<<"command id:"<<count<<endl;
			cout<<"msg.param1:"<<msg.msgtype<<endl;
			cout<<"msg.param2:"<<msg.msgvalue1<<endl;
			cout<<"msg.param3:"<<msg.msgvalue2<<endl;
			count++;*/
			switch(msg.msgtype)
			{
			case Forward:
				{
				    GoForward(msg.msgvalue1,msg.msgvalue2);
					SetEvent(SendSuccess);
				    break;
				}
			case Turn:
				{
					Rotate(msg.msgvalue1);
					SetEvent(SendSuccess);
					break;

				}
			case Stop:
				{
					RobotStop();
					SetEvent(SendSuccess);
					break;

				}
			case Back:
				{
				    Goback();
					SetEvent(SendSuccess);
				    break;
				}
			case CROSS:
				{
				    CrossDoor();
					SetEvent(SendSuccess);
				    break;
				}
			case CHARGE:
				{
				    RobotCharge();
					SetEvent(SendSuccess);
				    break;
				}
			case GO10:
				{
					GoForward10();
					SetEvent(SendSuccess);
				    break;
				}
			case BACK10:
				{
				    GoBack10();
					SetEvent(SendSuccess);
				    break;
				}
			case LEFT10:
				{
				    GoLeft10();
					SetEvent(SendSuccess);
				    break;
				}
			case RIGHT10:
				{
				    GoRight10();
					SetEvent(SendSuccess);
				    break;
				}
			default:
				break;
			}
		}

		Sleep(10);
	}
	return 0;
}