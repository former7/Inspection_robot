#include <process.h>
#include <windows.h>
#include <math.h>
#include "RobotStatusManager.h"
#include "RobotParam.h"

#pragma once

typedef struct __LocationLoopQueue {
	int      location_x [avenum];
	int      location_y [avenum];
	int      first_point_idx ; 
	int      last_point_idx ; 
	int      locationPointSize ; 
	bool     start_save_location ;
} LocationLoopQueue;


class RobotDataProcess
{
public:
	RobotDataProcess(void);
	virtual ~RobotDataProcess(void);

	void processLocationData() ;

	void processGyroscopeData() ;

	void setLocationEventHandle(HANDLE *pHandle) 
	{
		m_LocationDataEvent = pHandle ;
	}

	void setGyoscopeEventHandle(HANDLE *pHandle) 
	{
		m_GyoscopeDataEvent = pHandle ;
	}

	/************************************************************************/
	/* �ýӿ��������¿�ʼ���㵱ǰ�ĳ��������                                    */
	/************************************************************************/
	void restartComputeLocation () 
	{
		m_restart_compute_location = true ;
	}


	/************************************************************************/
	/* ���ؾ���У׼����������                                                   */
	/************************************************************************/
	Location getLocationAfterCalibration()
	{
		return m_LocationAfterCalibration ;
	}

#if 0
	Direction getGyoscopeAfterCalibration()
	{

	}
#endif


private:

	HANDLE m_GyoscopeDataEvent ;

	int    computeVariance(int * data , int lenth) ;
	void   compensateLocation(Location &location) ;

	RobotStatusManager *pSensorManager ; 

	/************************************************************************/
	/* �볬�����صı���                                                      */
	/************************************************************************/
	HANDLE   m_LocationDataEvent ;
	Location m_lastLocation ; 
	Location m_LocationAfterCalibration ;
	LocationLoopQueue m_llq ;
	bool     m_restart_compute_location ;
};

