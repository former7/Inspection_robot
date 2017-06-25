#ifndef  __ROBOTPARAM_H__
#define  __ROBOTPARAM_H__
#include<windows.h>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>


//************���ݽӿ�*************//
#define RobotPort    "com5" // ������
#define LocationPort "com3"	// �������λ
#define SensorsPort  "com4" // ������ ������
#define LifterPort   "com2"	// ������ ����
#define avenum       5      //������˲�ƽ��ֵ����
#define errnum       200     //������˲�������귶Χ
//************�������˶�����********//
#define INISPEED   12
#define FASTSPEED  30
#define  WalkAngel   0
//************·������************//
#define PI           3.1415926
//#define threshold    450     //��ֵ
#define InitialX     0       //��ʼ����X����
#define InitialY     -100     //��ʼ����Y����
#define InitialAngle 0       //��ʼƫת��
#define DisThreshold 400     //�ڶ���������ֵ
#define DisThreshold2   200   //��һ��������ֵ
//#define Safethreshold  12000 //���پ�����ֵ
//************�˶�����************//
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
	int TurnAngle;//ת��
	int turn;//ת��-1Ϊ��ת��0Ϊ��ת��1Ϊ��ת
	int GoWithxy;//�Ƿ���ͬһ�������˶���1ΪX�ᣬ2Ϊy��
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




