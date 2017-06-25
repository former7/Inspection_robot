#include"NetWorkInit.h"
#include"SerialPort_Location.h"
#include"SerialPort_Robot.h"
#include"SerialPort_Lifter.h"
#include"SerialPort_Sensors.h"
#include"SerialPort_Gyroscope.h"
#include"RobotStatusManager.h"
#include"MsgQueue.h"
#include"RobotDataProcess.h"
#include"RouteArrayManager.h"
#include"HKCamera.h"
#include<process.h>
#include "Debug_message.h"


//获取超宽带定位线程
unsigned _stdcall GetPositon(void* param)
{

	//ReceiveLocation(route->getRouteArrayHead()->x,route->getRouteArrayHead()->y);
	ReceiveLocation2(75,97);
	return 0;
} 
 //获取升降杆信息线程
unsigned _stdcall ReceiveLifter(void* param)
{

	ReceiveLifter();

	return 0;
} 
//获取传感器信息线程
unsigned _stdcall ReceiveSensors(void* param)
{

	ReceiveSensors2();

	return 0;
} 
//获取陀螺仪朝向线程
unsigned _stdcall ReceiveGyro(void* param)
{

	while(1)
	{
	GetGyro();
	Sleep(1500);
	ReceiveGyro();
    }
	return 0;
}  
//发送机器人位置线程
unsigned _stdcall SendLocation(void* param){

	while(1){
		//NetWorkInit();
		Sleep(2000);
		NetWorkInit();
		send_location(http_socket);
		NetWorkClose();
	}
	return 0;}
unsigned _stdcall SocketConnect(void* param)
{
	Sleep(100000);
	while(1)
	{
	//NetWorkInit();
	Sleep(50000);
	}
}
HANDLE LifterEvent;
HANDLE LifterCheck;
HANDLE NavigateOver;
HANDLE SendSuccess;
HANDLE MoveStartEvent;
HANDLE ControlSemaphore;
//HANDLE GetRouteSuccess; // 不需要进行等待验证了
BOOL AfterManual=0;
MsgQueue RMqueue;
RobotStatusManager RobotState;
//RobotDataProcess*RobotData=new RobotDataProcess();
unsigned _stdcall MoveControl(void* param);
unsigned _stdcall PathNavigation(void* param);
unsigned _stdcall ManualControl(void* param);
unsigned _stdcall OrbDataInterface(void* param);
unsigned _stdcall BarriersDetect(void* param);

void main()
{



	Serial_Robot();//开启机器人控制串口
	Serial_Location();//开启超宽带串口
	Serial_Lifter();//开启升降杆串口
	//Serial_Gyroscope();//开启陀螺仪串口
	Serial_Sensors();//开启传感器串口
//	NetWorkInit();//开启网络连接 //网络连接修改为 连接一次断开一次，在调用的时候创建
	HKcameraInit2();



	LifterEvent  =  CreateEvent(NULL, FALSE, FALSE, "lifterheght");
	LifterCheck  =  CreateEvent(NULL, FALSE, FALSE, "liftercheck");
	NavigateOver =  CreateEvent(NULL, FALSE, FALSE, "navigateover");
	SendSuccess  =  CreateEvent(NULL, FALSE, FALSE, "sendsuccess");
	//GetRouteSuccess  =  CreateEvent(NULL, FALSE, FALSE, "GetRouteSuccess");

	ControlSemaphore=CreateSemaphore(NULL, 1, 1, NULL);//运动控制资源,当前1个资源，最大允许1个同时访问


	//开启获取升降杆信息线程
	HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, ReceiveLifter, NULL, NULL,NULL); 
	//开启获取超宽带定位坐标线程
	HANDLE handle2 = (HANDLE)_beginthreadex(NULL, 0, GetPositon, NULL, NULL,NULL); 
	//开启获取陀螺仪角度线程
	////HANDLE handle3 = (HANDLE)_beginthreadex(NULL, 0, ReceiveGyro, NULL, NULL,NULL); 
	//开启运动控制模块
	HANDLE handle4 = (HANDLE)_beginthreadex(NULL, 0, MoveControl, NULL, NULL,NULL); 
	//开启路径导航
    HANDLE handle5 = (HANDLE)_beginthreadex(NULL, 0, PathNavigation, NULL, NULL,NULL);
	//开启手动控制模块
	HANDLE handle6 = (HANDLE)_beginthreadex(NULL, 0, ManualControl, NULL, NULL,NULL); 
	//ORB接口
	//HANDLE handle8 = (HANDLE)_beginthreadex(NULL, 0, OrbDataInterface, NULL, NULL,NULL);
	//传感器接口
	HANDLE handle9 = (HANDLE)_beginthreadex(NULL, 0, ReceiveSensors, NULL, NULL,NULL); 
	Sleep(8000);
	//超声波避障接口
	HANDLE handle10 = (HANDLE)_beginthreadex(NULL, 0, BarriersDetect, NULL, NULL,NULL); 
	//开启发送机器人位置线程
	HANDLE handle7 = (HANDLE)_beginthreadex(NULL, 0, SendLocation, NULL, NULL,NULL); 
	SetThreadPriority(handle6,THREAD_PRIORITY_ABOVE_NORMAL);
   
	WaitForSingleObject(NavigateOver,INFINITE);
	CloseHandle(LifterEvent);
	CloseHandle(LifterCheck);
	CloseHandle(NavigateOver);
	CloseHandle(handle1);
	CloseHandle(handle2);
//	CloseHandle(handle3);
//	CloseHandle(handle4);
	CloseHandle(handle5);
//	CloseHandle(handle6);
	//CloseHandle(handle7);
	CloseHandle(handle9);
	
	
	
}