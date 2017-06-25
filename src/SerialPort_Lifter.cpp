/*****************************************************************************
升降杆接口
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"Serialport.h"
#include"RobotParam.h"
#include<iostream>
#include "Debug_message.h"
HANDLE hCommlifter; //串口handle
OVERLAPPED m_ovlifter;
COMSTAT comstatlifter;
DWORD m_dwCommEventslifter;
extern HANDLE  LifterEvent;
extern HANDLE  LifterCheck;


int ReceiveLifter()
{ 
	DWORD BytesRead;
	DWORD dwError;
	DWORD ReadError;
	int recvx,recvy;
	for(;;){
	BYTE bufferRecv[1024];
	Sleep(100);
	ClearCommError(hCommlifter, &dwError,&comstatlifter);
	if(comstatlifter.cbInQue== 0)
	{
			//Sleep(10);
			continue;
	}
	//PRINTF_LOG("cin %d\n",comstat.cbInQue);
	ReadFile(hCommlifter,//Handle to COMM port
		bufferRecv,//RXBuffer Pointer
		comstatlifter.cbInQue, //Read m_ComStat.cbInQue bytes
		&BytesRead,//Stores number of bytes read
		&m_ovlifter);//pointer to the m_ov structure
	//bufferRecv[comstat.cbInQue]='\0';
	//PRINTF_LOG("buffer%x\n",bufferRecv[0]);
	//PRINTF_LOG("byteread %d\n",comstat.cbInQue);
	for(int i=0;(i<comstatlifter.cbInQue)&&(i<1024);i++)
	{
		//if(bufferRecv[i]==0x57)
			//PRINTF_LOG("yes");
	   if((bufferRecv[i]==0xaa))
	   {   
		   //PRINTF_LOG("ok\n");
		   if(bufferRecv[i+1]==0xff&&((i+1)<=comstatlifter.cbInQue))
		   {
			    PRINTF_LOG("高度达到\n");
				SetEvent(LifterEvent);
			    i+=1;
			   
		       //PRINTF_LOG("%d,%d \n",recvx,recvy);
		   }
	
	   }
	}
	//PRINTF_LOG("%s\n",bufferRecv);
	}
	

}
int SendLifter(BYTE* m_szWriteBuffer,DWORD m_nToSend)
{
	BOOL bWrite =TRUE;
	BOOL bResult =TRUE;
	DWORD BytesSent = 0;

	if (bWrite) {
		m_ovlifter.Offset =0;
		m_ovlifter.OffsetHigh = 0;
		// Clear buffer
		bResult = WriteFile(hCommlifter, // Handle to COMM Port
			m_szWriteBuffer, // Pointer to message buffer in calling finction
			m_nToSend, // Length of message to send
			&BytesSent, //Where to store the number of bytes sent
			&m_ovlifter );//Overlapped structure
		if (!bResult) {
			DWORD dwError = GetLastError();
			switch (dwError) {
			case ERROR_IO_PENDING: {
				//continue to GetOverlappedResults()
				BytesSent =0;
				bWrite = FALSE;
				break;
								   }
			default:
				{
					//all other error codes
					break;
				}
			}
		}
	} // end if(bWrite)
	if (!bWrite) {
		bWrite = TRUE
			; bResult = GetOverlappedResult(hCommlifter, //Handle to COMM port
			&m_ovlifter,// Overlapped structure
			&BytesSent, //Stores number of bytes sent
			TRUE); //Waitflag
		// deal with the error code
		if (!bResult) {
			PRINTF_LOG("GetOverlappedResults() in WriteFile()");
		}
	} // end if (!bWrite)
	//Verify that the data size send equals what we tried to send
	if (BytesSent != m_nToSend) {
		PRINTF_LOG("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length:%d\n", BytesSent, strlen((char*)m_szWriteBuffer));
	}
	return true;
}
bool LiftOption(int height)
{   char heightcommand[20];
    itoa(height,heightcommand,10);
	_snprintf(heightcommand+strlen(heightcommand),20,"%s","##");
	//strcat(heightcommand,"##");
	SendLifter((BYTE*)heightcommand,strlen(heightcommand));
	return true;
}
bool SendCheck()
{   char check[]="check";
SendLifter((BYTE*)check,sizeof(check));
return true;
}
bool LifterCheckSelf()
{   bool flag=1;
    DWORD dw;
	while(flag){
		SendCheck();
		dw = WaitForSingleObject(LifterCheck, 2000);
		switch(dw)
		{
		case WAIT_OBJECT_0:
			// The process terminated.
			{  std::cout<<"升降杆复位成功"<<std::endl;
			flag=0;
			break;}

		case WAIT_TIMEOUT:
			{// The process did not terminate within 5000 milliseconds.
				std::cout<<"超时"<<std::endl;
				break;
			}
		case WAIT_FAILED:
			// Bad call to function (invalid handle?)
			break;
		}	
	}
	ResetEvent(LifterCheck);
	return true;
}
int Serial_Lifter()
{
	if(openport(LifterPort,hCommlifter))
		PRINTF_LOG("open Lifter-port success\n");
	else return FALSE;
	if(setupdcb(115200,hCommlifter))
		PRINTF_LOG("setupdcb success\n");
	else return FALSE;
	if(setuptimeout(0,0,0,0,0,hCommlifter))
		PRINTF_LOG("setuptime sucucess\n");
	else return FALSE;
	PurgeComm(hCommlifter,
		PURGE_RXCLEAR|PURGE_TXCLEAR
		|PURGE_RXABORT|PURGE_TXABORT);
	return true;
	/*char message[]="im SS";
	byte *b=(byte*)message;
	WriteChar(b,sizeof(message));PRINTF_LOG("received data:\n");*/
}
	

	

