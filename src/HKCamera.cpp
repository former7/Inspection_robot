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
    case EXCEPTION_RECONNECT:    //预览时重连
        PRINTF_LOG("----------reconnect--------%d\n", time(NULL));
    break;
	default:
    break;
    }
}

//抓图
bool CapturePicture(int direction,int idx){
	time_t  timer;

	CameraStatus cs=north;
	
	char sPicFileName[100] ="./conf/";
	double a,b;
	//旋转摄像头
	if(direction==1)
	{
		// 到达预置点位，
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
	//回转摄像头
  
	return 1;
}

void HKcameraInit() {
	
	//---------------------------------------
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	// 获取控制台窗口句柄
	// HMODULE hKernel32 = GetModuleHandle("kernel32");
//	GetConsoleWindow1=(PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");
//
	//---------------------------------------
	// 注册设备
	//LONG lUserID;

	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); //设备IP地址
	struLoginInfo.wPort = 8000; //设备服务端口
	strcpy(struLoginInfo.sUserName, "admin"); //设备登录用户名
	strcpy(struLoginInfo.sPassword, "cvlab720"); //设备登录密码

	//设备信息, 输出参数
	NET_DVR_DEVICEINFO_V40 struDeviceInfoV40 = {0};

	lUserID = NET_DVR_Login_V40(&struLoginInfo, &struDeviceInfoV40);
	if (lUserID < 0)
	{
		PRINTF_LOG("Login failed, error code: %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return;
	}

	//---------------------------------------
	//启动预览并设置回调数据流
	//LONG lRealPlayHandle;
	HWND hWnd = GetConsoleWindow();     //获取窗口句柄
	NET_DVR_PREVIEWINFO struPlayInfo = {0};
	struPlayInfo.hPlayWnd = hWnd;         //需要SDK解码时句柄设为有效值，仅取流不解码时可设为空
	//struPlayInfo.hPlayWnd = 0; 
	struPlayInfo.lChannel     = 1;       //预览通道号
	struPlayInfo.dwStreamType = 0;       //0-主码流，1-子码流，2-码流3，3-码流4，以此类推
	struPlayInfo.dwLinkMode   = 0;       //0- TCP方式，1- UDP方式，2- 多播方式，3- RTP方式，4-RTP/RTSP，5-RSTP/HTTP

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
	// 初始化
	NET_DVR_Init();
	//设置连接时间与重连时间
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);

	//---------------------------------------
	//设置异常消息回调函数
	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);

	//---------------------------------------
	// 获取控制台窗口句柄
	// HMODULE hKernel32 = GetModuleHandle("kernel32");
//	GetConsoleWindow1=(PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");
//
	//---------------------------------------
	// 注册设备
	//LONG lUserID;

	//登录参数，包括设备地址、登录用户、密码等
	NET_DVR_USER_LOGIN_INFO struLoginInfo = {0};
	struLoginInfo.bUseAsynLogin = 0; //同步登录方式
	strcpy(struLoginInfo.sDeviceAddress, "192.168.1.64"); //设备IP地址
	struLoginInfo.wPort = 8000; //设备服务端口
	strcpy(struLoginInfo.sUserName, "admin"); //设备登录用户名
	strcpy(struLoginInfo.sPassword, "cvlab720"); //设备登录密码

	//设备信息, 输出参数
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


