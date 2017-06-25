#include <stdio.h>
#include <iostream>
#include "HCNetSDK.h"
#include <time.h>
#include"HKCamera.h"
#include"NetWorkInit.h"
#include"tcp_trans_info.h"
#include "Windows.h"
#include "Debug_message.h"
using namespace std;
LONG lRealPlayHandle;
LONG lUserID;

typedef HWND (WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindow1;

void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
    char tempbuf[256] = {0};
    switch(dwType) 
    {
    case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
        PRINTF_LOG("----------reconnect--------%d\n", time(NULL));
    break;
	default:
    break;
    }
}

//ץͼ
bool CapturePicture(int direction,int idx){
	time_t  timer;

	CameraStatus cs=north;
	
	char sPicFileName[100] ="./conf/";
	double a,b;
	//��ת����ͷ
	if(direction==1)
	{
		// ����Ԥ�õ�λ��
		do
		{
			bool val=NET_DVR_PTZPreset_Other(lUserID,1,GOTO_PRESET,1);
			if(val == true){
				a=clock();
				
				Sleep(5000);
				b=clock();
				PRINTF_LOG("time:%f",b-a);
				break;
			}
			//Sleep(2000);
		}while (true);
	}
	else if(direction==2)
	{
		do
		{
			bool val=NET_DVR_PTZPreset_Other(lUserID,1,GOTO_PRESET,2);
			if(val == true){
				Sleep(5000);
				break;
			}
			//Sleep(2000);
		}while (true);
	}
	else
	{

	}
	a=clock();
	Sleep(3000);
	b=clock();
	PRINTF_LOG("time:%f",b-a);
	time(&timer);	
	//_snprintf(sPicFileName+strlen(sPicFileName) , 100 , "%d_%d_%d_%d.jpg" ,localtime(&timer)->tm_mon,localtime(&timer)->tm_mday,localtime(&timer)->tm_hour , localtime(&timer)->tm_min);
	_snprintf(sPicFileName+strlen(sPicFileName) , 100 , "%d_%d_%d_%d_%d_%d.jpg" ,idx,1900+localtime(&timer)->tm_year,1+localtime(&timer)->tm_mon,localtime(&timer)->tm_mday,localtime(&timer)->tm_hour, localtime(&timer)->tm_min);
	PRINTF_LOG("%s",sPicFileName);
/*	if(!NET_DVR_SetCapturePictureMode( JPEG_MODE  ))
		NET_DVR_GetLastError();
	NET_DVR_CapturePicture(
 lRealPlayHandle,
  sPicFileName
);*/
	

	NET_DVR_JPEGPARA picmode/*={a,b}*/;
	picmode.wPicSize = 5;
	picmode.wPicQuality =1;
	NET_DVR_CaptureJPEGPicture(
		lUserID,
		1,
		&picmode,
		sPicFileName	
		);
	do
		{
			bool val=NET_DVR_PTZPreset_Other(lUserID,1,GOTO_PRESET,3);
			if(val == true){
				Sleep(5000);
				break;
			}
			Sleep(2000);
		}while (true);
	
	Sleep(1000);
	//FILE *a=fopen(sPicFileName,"w");
	//fclose(a);
	//NetWorkInit();
	sendPic(sock_fd ,PATH4, SERVER_ADDR, sPicFileName);
	//��ת����ͷ
  
	return 1;
}

void HKcameraInit() {
	
	//---------------------------------------
	// ��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//�����쳣��Ϣ�ص�����
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	// ��ȡ����̨���ھ��
	// HMODULE hKernel32 = GetModuleHandle("kernel32");
//	GetConsoleWindow1=(PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");
//
	//---------------------------------------
	// ע���豸
	//LONG lUserID;

	//��¼�����������豸��ַ����¼�û��������
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	struLoginInfo.bUseAsynLogin = 0; //ͬ����¼��ʽ
	strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); //�豸IP��ַ
	struLoginInfo.wPort = 8000; //�豸����˿�
	strcpy(struLoginInfo.sUserName, "admin"); //�豸��¼�û���
	strcpy(struLoginInfo.sPassword, "cvlab720"); //�豸��¼����

	//�豸��Ϣ, �������
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (lUserID < 0)
	{
		PRINTF_LOG("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}

	//---------------------------------------
	//����Ԥ�������ûص�������
	//LONG lRealPlayHandle;
	HWND hWnd = GetConsoleWindow();     //��ȡ���ھ��
	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = hWnd;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	//struPlayInfo.hPlayWnd = 0; 
	struPlayInfo.lChannel     = 1;       //Ԥ��ͨ����
	struPlayInfo.dwStreamType = 0;       //0-��������1-��������2-����3��3-����4���Դ�����
	struPlayInfo.dwLinkMode   = 0;       //0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, NULL, NULL);
	if (lRealPlayHandle < 0)
	{
		PRINTF_LOG("NET_DVR_RealPlay_V40 error\n");
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return;
	}
		
		//CapturePicture(lRealPlayHandle);
	//Sleep(10000);
	//---------------------------------------
	

  return ;
}
void HKcameraInit2() {
	
	//---------------------------------------
	// ��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//�����쳣��Ϣ�ص�����
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	// ��ȡ����̨���ھ��
	// HMODULE hKernel32 = GetModuleHandle("kernel32");
//	GetConsoleWindow1=(PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");
//
	//---------------------------------------
	// ע���豸
	//LONG lUserID;

	//��¼�����������豸��ַ����¼�û��������
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	struLoginInfo.bUseAsynLogin = 0; //ͬ����¼��ʽ
	strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); //�豸IP��ַ
	struLoginInfo.wPort = 8000; //�豸����˿�
	strcpy(struLoginInfo.sUserName, "admin"); //�豸��¼�û���
	strcpy(struLoginInfo.sPassword, "cvlab720"); //�豸��¼����

	//�豸��Ϣ, �������
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (lUserID < 0)
	{
		PRINTF_LOG("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}
	do
		{
			bool val=NET_DVR_PTZPreset_Other(lUserID,1,GOTO_PRESET,3);
			if(val == true){
				Sleep(5000);
				break;
			}
			Sleep(2000);
		}while (true);
  return ;
}


