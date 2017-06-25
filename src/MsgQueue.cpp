#include"MsgQueue.h"
#include "Debug_message.h"
void MsgQueue::sendMsg(pCommand pmsg)
{
	Command msg;
	msg.msgtype = pmsg->msgtype;
	msg.msgvalue1 = pmsg->msgvalue1;
	msg.msgvalue2 = pmsg->msgvalue2;

	WaitForSingleObject(m_mutex,INFINITE);

	RobotQueue.push(msg);

	ReleaseMutex(m_mutex);


};
bool MsgQueue::getMsg(pCommand pmsg)
{      

	if(!RobotQueue.empty())
	{
		WaitForSingleObject(m_mutex,INFINITE);
		*pmsg = RobotQueue.front();
		RobotQueue.pop();
		ReleaseMutex(m_mutex);

		return true;
	}
	else
	{
		return false;
	}


}