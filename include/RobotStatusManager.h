#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#pragma once
typedef struct    
{
	int x;
	int y;
}Location; 
typedef struct{
	int x;
	int y;
	int GyroInfo;
}Direction, *pDirection;
class RobotStatusManager
{
public:
	RobotStatusManager()
	{
	m_RobotLocation.x=0;
	m_RobotLocation.y=0;
	}
	void UpdateLocation(int x,int y){
		m_RobotLocation.x=x;
        m_RobotLocation.y=y;
		
	}
	void UpdateDirection(int x,int y){
		m_RobotDirection.x=x;
		m_RobotDirection.y=y;

	}
	void UpdateDirection(int angle){
		m_RobotDirection.GyroInfo=angle;
	}
	Location getRobotLocation()
	{
		return m_RobotLocation;
	}
	Direction getDirection()
	{
		return m_RobotDirection;
	}
	int getUltrasonicInfo(int i)
	{  
		return m_UltrasonicInfo[i];
	}
	int getBatteryInfo(int i)
		{  
			return m_BatteryInfo[i];
		}
private:
	Location m_RobotLocation;
	Direction m_RobotDirection;
	int m_UltrasonicInfo[16];
	int m_BatteryInfo[3];       //电池信息，输出电压，输出电流，电池温度
	short m_LaserInfo;         //激光测距距离

};