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
/* ����һ������ķ���                                                      */
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
	/* �����thealta֮�󣬿�ʼȥ���������޵�ֵ                                   */
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
/* ���ڲ���locaton                                                       */
/************************************************************************/
void RobotDataProcess::compensateLocation(Location &location)
{

}


/************************************************************************/
/* ���ڴ���ǰ���������                                                  */
/************************************************************************/
void RobotDataProcess::processLocationData(void)
{
	Location location ;

	while(1) 
	{
		WaitForSingleObject(m_LocationDataEvent, INFINITE) ; //�ȴ���ȡ�����߳�֪ͨ

		location = pSensorManager->getRobotLocation() ;

		if(m_restart_compute_location == true) 
		{
			m_restart_compute_location = false ;
			m_llq.first_point_idx     = 0; ;
			m_llq.last_point_idx      = -1 ;
			m_llq.locationPointSize   = 0  ;
			m_llq.start_save_location = false ;
		}


		if(m_llq.locationPointSize < avenum ) //������û�з�����ʱ��,end_idx�� size����
		{
			m_llq.last_point_idx ++ ;
			m_llq.locationPointSize ++ ;

		}
		else                                 //���з����ˣ����ı�start_idx��end_idx��size����
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
				//��ȡ�õ�λ����Ч��ʱ����Ҫ���ݵ�ǰ�ٶ�������
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