#include "RouteArrayManager.h"
#include "Debug_message.h"

/************************************************************************/
/* ����һ���㵽������                                                    */
/************************************************************************/
ArrayState RouteArrayManager::addRoutePointAtTail(int x , int y, int lift, int dir, int remain,int door) 
{
	RoutePoint * pRoutePoint = NULL ;
	RoutePoint * pRouteIdx   ;
	int          whileLoop ; 
#if 0
	if(m_size <= (m_tail + 1)) 
		return ArrayOutOfIdx ;
#endif

	pRoutePoint = (RoutePoint *)malloc(sizeof(RoutePoint));
	if(NULL == pRoutePoint) 
	{
		return ArrayOutMem ;
	}

	pRoutePoint->x    = x ;
	pRoutePoint->y    = y ;
	pRoutePoint->lift = lift;
	pRoutePoint->direct = dir;
	pRoutePoint->remain = remain ;
	pRoutePoint->door = door ;
	pRoutePoint->next = NULL ;
	/************************************************************************/
	/* ����һ��Ԫ�غ�size + 1                                                */
	/************************************************************************/
	m_size ++ ;

	if(m_tail == ARRAY_TAIL_INIT_IDX) 
	{
		m_RouteArrayHead = pRoutePoint ;
		m_tail = 0;
		return ArrayOK ;
	}
 
	whileLoop = m_tail ;
	pRouteIdx = getRouteArrayHead() ;
	while(whileLoop != 0)
	{
		whileLoop -- ;
		pRouteIdx = pRouteIdx->next ;
	}
	pRouteIdx->next = pRoutePoint ;

	/************************************************************************/
	/* �µ�·�ɵ���룬tailӦ���Լ�                                            */
	/************************************************************************/
	m_tail ++ ;

	return ArrayOK ;
}

/************************************************************************/
/* ���ӵ����鵽������                                                    */
/************************************************************************/
ArrayState RouteArrayManager::addRouteArrayAtTail(int x , int y, int lift, int dir, int remain,int door, int ArraySize)
{
	int i  ;
	ArrayState returnResult = ArrayOK;

	for(i = 0; i < ArraySize; i++)
	{
		returnResult = addRoutePointAtTail(x, y, lift, dir, remain,door) ;
		if(ArrayOK != returnResult)
		{
			return returnResult ;
		}
	}
	return ArrayOK ;
}

/************************************************************************/
/* ������е�����                                                         */
/************************************************************************/
ArrayState RouteArrayManager::clearAllRouteArray()
{
	memset(m_RouteArrayHead, 0, m_size * sizeof(RoutePoint)); 
	m_size = 0 ;
	m_tail = ARRAY_TAIL_INIT_IDX ;
	return ArrayOK ;
}

/************************************************************************/
/* ����һ��·�ɵ㵽ĳһ��λ��                                               */
/************************************************************************/
ArrayState RouteArrayManager::addRoutePointAtIndex(int x, int y, int lift, int dir, int remain, int ArrayIndex)
{
	RoutePoint * pRoutePoint = NULL ;
	RoutePoint * pRouteIdx   ;
	RoutePoint * pRouteTmp ;
	int          whileLoop ; 
#if 0
	if(m_size <= (m_tail + 1)) 
	{
		return ArrayOutOfIdx ;
	}
#endif
	pRoutePoint = (RoutePoint *)malloc(sizeof(RoutePoint));
	if(NULL == pRoutePoint) 
	{
		return ArrayOutMem ;
	}

	pRoutePoint->x    = x ;
	pRoutePoint->y    = y ;
	pRoutePoint->lift = lift;
	pRoutePoint->direct = dir ;
	pRoutePoint->remain = remain ;
	pRoutePoint->next = NULL ;
	m_size ++ ;

	/************************************************************************/
	/* ����ͷ��                                                              */
	/************************************************************************/
	if(0 == ArrayIndex)
	{
		pRouteIdx = m_RouteArrayHead; 	
		pRoutePoint->next = pRouteIdx ;
		m_RouteArrayHead = pRoutePoint ;
		m_tail ++ ;
		return ArrayOK ;
	}

	/************************************************************************/
	/* ���ں���                                                              */
	/************************************************************************/
	whileLoop = ArrayIndex - 1 ;
	pRouteIdx = getRouteArrayHead() ;
	while(whileLoop != 0)
	{
		whileLoop -- ;
		pRouteIdx = pRouteIdx->next ;
	}
	pRouteTmp = pRouteIdx->next ;
	pRouteIdx->next = pRoutePoint ;
	pRoutePoint->next = pRouteTmp ;

	return ArrayOK ;
}


void RouteArrayManager::printAllRoutePoint()
{
	RoutePoint *pRoutePointIdx = getRouteArrayHead(); 
	int         RecentSize     = m_size ;
	int         i; 

	for(i = 0; i< RecentSize; i++)
	{
		PRINTF_LOG("%d\t%d\t%d\t%d\t%d\n",pRoutePointIdx->x, pRoutePointIdx->y, pRoutePointIdx->lift, pRoutePointIdx->direct, pRoutePointIdx->remain);
		pRoutePointIdx = pRoutePointIdx->next ;
	}
}

/************************************************************************/
/* ��ȡ��Ҫ�ĵ�Ľṹ                                                      */
/************************************************************************/
RoutePoint * RouteArrayManager::getRoutePointIndex(int idx) 
{
	RoutePoint * pRoutePoint = getRouteArrayHead();
	if(ARRAY_TAIL_INIT_IDX == m_tail) 
	{
		return NULL ;
	}
	while(idx) 
	{
		pRoutePoint = pRoutePoint->next ;
		idx--;
	}
	return pRoutePoint ;
}