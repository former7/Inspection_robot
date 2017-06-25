#include "SerialPort_Sensors.h"
#include "RobotParam.h"
#include "Debug_message.h"

unsigned _stdcall BarriersDetect(void* param)
{
  while(true)
  {
	  
	  //PRINTF_LOG("%d\n",dsmin);
	if(dsmin<360&&dsmin>0)
	{
		PRINTF_LOG("ÕÏ°­Îïbarriers find\n");
		WaitForSingleObject(ControlSemaphore,INFINITE);
		SendMoveCommand(Stop,0,0);
		int count=0;
		while(1)
		{
			if(dsmin==0)count++;
			if(dsmin>550||count>3)
			{
				ReleaseSemaphore(ControlSemaphore, 1, NULL);
				break;
			}
			Sleep(1000);
		}

	}
	Sleep(10);
  }
}