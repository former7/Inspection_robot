#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
/************************************************************************/
/*  RoutePoint ���ڱ�ʾʵ�ʵ�·�ɵ�,Ŀǰ�õ���������                        */
/************************************************************************/
typedef struct __RoutePoint 
{
	int x ;
	int y ;
	int lift;
	int remain ;
	int direct ;
	int door;
	struct __RoutePoint * next ;
} RoutePoint;

/************************************************************************/
/* ���������Ա�����ķ���ֵ                                                 */
/************************************************************************/
enum ArrayState {
				 ArrayOK       = 0, 
				 ArrayOutOfIdx = 1,
				 ArrayOutMem   = 2, 
				};

/************************************************************************/
/* β�ڵ�ĳ�ʼλ��                                                       */
/************************************************************************/
#define ARRAY_TAIL_INIT_IDX -1 


/************************************************************************/
/* �����                                                                */
/************************************************************************/
class RouteArrayManager 
{
public :
	RouteArrayManager() 
	{
		m_size           = 0 ;
		m_tail           = ARRAY_TAIL_INIT_IDX;
		m_RouteArrayHead = NULL ;
	}

	/*  �����е�����
	RouteArrayManager(int array_size)
	{
		m_size = array_size ;
		m_tail = ARRAY_TAIL_INIT_IDX ; 
		m_RouteArrayHead = NULL ;
	}
	*/

	RouteArrayManager(RoutePoint *pRouteHead, int routeSize) 
	{
		m_size = routeSize ; 
		m_tail = routeSize - 1 ;
		m_RouteArrayHead = pRouteHead ;
	}

	virtual ~RouteArrayManager() 
	{
		RoutePoint *pRoutePointIdx = m_RouteArrayHead ;
		RoutePoint *pRoutePointNext ; 
		while(m_size) 
		{
			if(1 != m_size)
			{
				pRoutePointNext = pRoutePointIdx->next ;
			}
			free(pRoutePointIdx) ;
			if(1 != m_size)
			{
				pRoutePointIdx = pRoutePointNext ;
			}

			m_size -- ;
		}
	}

	RoutePoint * getRouteArrayHead() 
	{
		return m_RouteArrayHead ;
	}

	int getRouteArraySize() 
	{
		return m_size ; 
	}

	ArrayState addRoutePointAtTail(int x , int y, int lift, int dir, int remain,int door) ;

	ArrayState addRouteArrayAtTail(int x , int y, int lift, int dir, int remain,int door, int ArraySize) ;

	ArrayState clearAllRouteArray() ;

	ArrayState addRoutePointAtIndex(int x, int y, int lift, int dir, int remain, int ArrayIndex) ;

	void       printAllRoutePoint() ;

	RoutePoint * getRoutePointIndex(int idx) ;

private:
	RoutePoint * m_RouteArrayHead ;
	int          m_size ;
	int          m_tail ;
};