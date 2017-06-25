#ifndef _MSGQUEUE_H_
#define  _MSGQUEUE_H_
#include<queue>
#include<windows.h>
#include<process.h>
#include<iostream>
using namespace std;
typedef struct{
	int msgtype;
	int msgvalue1;
	int msgvalue2;
}Command, *pCommand;
class MsgQueue
{
private:
	queue<Command> RobotQueue;
	HANDLE	m_mutex;

public:
	MsgQueue()
	{
		m_mutex=CreateMutex(NULL,FALSE,"QueueMutex");		

	};
	~MsgQueue()
	{

	};
	int getQueueSize()
	{
		return RobotQueue.size();
	}
	void sendMsg(pCommand pmsg);
	bool getMsg(pCommand pmsg);
	
};
#endif