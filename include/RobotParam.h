#ifndef  __ROBOTPARAM_H__
#define  __ROBOTPARAM_H__
#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>


//************数据接口*************//
#define RobotPort    "com5" // 机器人
#define LocationPort "com3"	// 超宽带定位
#define SensorsPort  "com4" // 传感器 超声波
#define LifterPort   "com2"	// 升降杆 串口
#define avenum       5      //超宽带滤波平均值个数
#define errnum       200     //超宽带滤波误差坐标范围
//************机器人运动数据********//
#define INISPEED   12
#define FASTSPEED  30
#define  WalkAngel   0
//************路径导航************//
#define PI           3.1415926
//#define threshold    450     //阈值
#define InitialX     0       //初始朝向X方向
#define InitialY     -100     //初始朝向Y方向
#define InitialAngle 0       //初始偏转度
#define DisThreshold 400     //第二档提速阈值
#define DisThreshold2   200   //第一档提速阈值
//#define Safethreshold  12000 //降速距离阈值
//************运动控制************//
#define Forward 0
#define Turn    1
#define Stop    2
#define Back    3
#define GO10    4
#define BACK10  5
#define LEFT10  6
#define RIGHT10 7
#define CROSS   8
#define CHARGE  9
#define BUFSIZE 255

//ORBSLAM DATA 
typedef struct send_info
{
	float x;
	float y;
	float angle;
	int barriers;
}; 
//Navigation turnmode
typedef struct MoveMode
{
	int TurnAngle;//转角
	int turn;//转向，-1为左转，0为不转，1为右转
	int GoWithxy;//是否在同一根轴上运动，1为X轴，2为y轴
	int LThreshold;
};
extern HANDLE LifterEvent;
extern HANDLE LifterCheck;
extern HANDLE NavigateOver;
extern HANDLE SendSuccess;
extern HANDLE MoveStartEvent;
extern HANDLE ControlSemaphore;
extern BOOL AfterManual;
extern int dsmin;
void SendMoveCommand(int param1,int param2,int param3);
#endif




