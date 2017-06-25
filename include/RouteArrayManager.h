#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
/************************************************************************/
/*  RoutePoint 用于表示实际的路由点,目前用单向链表处理                        */
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
/* 用来处理成员函数的返回值                                                 */
/************************************************************************/
enum ArrayState {
				 ArrayOK       = 0, 
				 ArrayOutOfIdx = 1,
				 ArrayOutMem   = 2, 
				};

/************************************************************************/
/* 尾节点的初始位置                                                       */
/************************************************************************/
#define ARRAY_TAIL_INIT_IDX -1 


/************************************************************************/
/* 类设计                                                                */
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

	/*  这里有点问题
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