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


//��ȡ�������λ�߳�
unsigned _stdcall GetPositon(void* param)
{

	//ReceiveLocation(route->getRouteArrayHead()->x,route->getRouteArrayHead()->y);
	ReceiveLocation2(75,97);
	return 0;
} 
 //��ȡ��������Ϣ�߳�
unsigned _stdcall ReceiveLifter(void* param)
{

	ReceiveLifter();

	return 0;
} 
//��ȡ��������Ϣ�߳�
unsigned _stdcall ReceiveSensors(void* param)
{

	ReceiveSensors2();

	return 0;
} 
//��ȡ�����ǳ����߳�
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
//���ͻ�����λ���߳�
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
//HANDLE GetRouteSuccess; // ����Ҫ���еȴ���֤��
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



	Serial_Robot();//���������˿��ƴ���
	Serial_Location();//�������������
	Serial_Lifter();//���������˴���
	//Serial_Gyroscope();//���������Ǵ���
	Serial_Sensors();//��������������
//	NetWorkInit();//������������ //���������޸�Ϊ ����һ�ζϿ�һ�Σ��ڵ��õ�ʱ�򴴽�
	HKcameraInit2();



	LifterEvent  =  CreateEvent(NULL, FALSE, FALSE, "lifterheght");
	LifterCheck  =  CreateEvent(NULL, FALSE, FALSE, "liftercheck");
	NavigateOver =  CreateEvent(NULL, FALSE, FALSE, "navigateover");
	SendSuccess  =  CreateEvent(NULL, FALSE, FALSE, "sendsuccess");
	//GetRouteSuccess  =  CreateEvent(NULL, FALSE, FALSE, "GetRouteSuccess");

	ControlSemaphore=CreateSemaphore(NULL, 1, 1, NULL);//�˶�������Դ,��ǰ1����Դ���������1��ͬʱ����


	//������ȡ��������Ϣ�߳�
	HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, ReceiveLifter, NULL, NULL,NULL); 
	//������ȡ�������λ�����߳�
	HANDLE handle2 = (HANDLE)_beginthreadex(NULL, 0, GetPositon, NULL, NULL,NULL); 
	//������ȡ�����ǽǶ��߳�
	////HANDLE handle3 = (HANDLE)_beginthreadex(NULL, 0, ReceiveGyro, NULL, NULL,NULL); 
	//�����˶�����ģ��
	HANDLE handle4 = (HANDLE)_beginthreadex(NULL, 0, MoveControl, NULL, NULL,NULL); 
	//����·������
    HANDLE handle5 = (HANDLE)_beginthreadex(NULL, 0, PathNavigation, NULL, NULL,NULL);
	//�����ֶ�����ģ��
	HANDLE handle6 = (HANDLE)_beginthreadex(NULL, 0, ManualControl, NULL, NULL,NULL); 
	//ORB�ӿ�
	//HANDLE handle8 = (HANDLE)_beginthreadex(NULL, 0, OrbDataInterface, NULL, NULL,NULL);
	//�������ӿ�
	HANDLE handle9 = (HANDLE)_beginthreadex(NULL, 0, ReceiveSensors, NULL, NULL,NULL); 
	Sleep(8000);
	//���������Ͻӿ�
	HANDLE handle10 = (HANDLE)_beginthreadex(NULL, 0, BarriersDetect, NULL, NULL,NULL); 
	//�������ͻ�����λ���߳�
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