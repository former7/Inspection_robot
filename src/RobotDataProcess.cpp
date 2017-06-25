#include "RobotDataProcess.h"
#include "Debug_message.h"

RobotDataProcess::RobotDataProcess(void)
{
	m_llq.first_point_idx     = 0; ;
	m_llq.last_point_idx      = -1 ;
	m_llq.locationPointSize   = 0  ;
	m_llq.start_save_location = false ;

	m_restart_compute_location = false ;
}


RobotDataProcess::~RobotDataProcess(void)
{
}


/************************************************************************/
/* 计算一个点阵的方差                                                      */
/************************************************************************/
int RobotDataProcess::computeVariance(int * data , int lenth)
{
	double       meanValue = 0;
	int			 thelta = 0; 
	int			 i ; 
	double       differValue = 0.0 ;
	double       PowerValue = 0.0;
	double       powerMeanValue = 0.0; 
	double       PowerThelta = 0.0;
	double       tmpValue =  0.0 ;
	double       resultValue = 0.0;
	int		     resultLenth = 0;
	
	for(i = 0; i< lenth; i++)
		meanValue = meanValue + data[i] ;

	meanValue = (double) (meanValue/lenth);

	for (i = 0; i<lenth; i++)
	{
		differValue = data[i] - meanValue ;
		PowerValue = differValue * differValue ;
		powerMeanValue += PowerValue;
	}

	PowerThelta  = (double) (powerMeanValue/lenth);
	PowerThelta  = sqrt(PowerThelta) ;

	/************************************************************************/
	/* 获得了thealta之后，开始去掉大于门限的值                                   */
	/************************************************************************/
	resultValue = 0.0 ;
	resultLenth = 0 ;
	for(i = 0; i< lenth; i++)
	{
		tmpValue = data[i] - meanValue ;
		if(abs(tmpValue) <= PowerThelta)
		{
			resultValue += data[i] ;
			resultLenth ++ ;
		}
	}


	if(resultLenth != 0)
		return (int)(resultValue/resultLenth) ;
	else
		return (-1) ;
}


/************************************************************************/
/* 用于补偿locaton                                                       */
/************************************************************************/
void RobotDataProcess::compensateLocation(Location &location)
{

}


/************************************************************************/
/* 用于处理当前超宽带数据                                                  */
/************************************************************************/
void RobotDataProcess::processLocationData(void)
{
	Location location ;

	while(1) 
	{
		WaitForSingleObject(m_LocationDataEvent, INFINITE) ; //等待获取坐标线程通知

		location = pSensorManager->getRobotLocation() ;

		if(m_restart_compute_location == true) 
		{
			m_restart_compute_location = false ;
			m_llq.first_point_idx     = 0; ;
			m_llq.last_point_idx      = -1 ;
			m_llq.locationPointSize   = 0  ;
			m_llq.start_save_location = false ;
		}


		if(m_llq.locationPointSize < avenum ) //当队列没有放满的时候,end_idx， size增加
		{
			m_llq.last_point_idx ++ ;
			m_llq.locationPointSize ++ ;

		}
		else                                 //队列放满了，将改变start_idx和end_idx，size不变
		{
			m_llq.last_point_idx  = (m_llq.last_point_idx  + 1) % avenum ;
			m_llq.first_point_idx = (m_llq.first_point_idx + 1) & avenum ;
			m_llq.start_save_location = true ;
		}

		m_llq.location_x[m_llq.last_point_idx] =  location.x ;
		m_llq.location_y[m_llq.last_point_idx] =  location.y ;

		
		if(m_llq.start_save_location)
		{
			location.x = computeVariance(m_llq.location_x, avenum) ;
			location.y = computeVariance(m_llq.location_y, avenum) ;
			if((-1 == location.x) || (-1 == location.y))	
			{
				//当取得的位置无效的时候，需要根据当前速度做补偿
				compensateLocation(location) ;
			}

			m_lastLocation = location ;
		}
		m_LocationAfterCalibration = location ;
	}
}


void RobotDataProcess::processGyroscopeData()
{

}