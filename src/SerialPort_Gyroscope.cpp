/*****************************************************************************
Éý½µ¸Ë½Ó¿Ú
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"Serialport.h"
#include "Debug_message.h"

HANDLE hCommgyro; //´®¿Úhandle
OVERLAPPED m_ovgyro;
COMSTAT comstatgyro;
DWORD m_dwCommEventsgyro;
int robotAngle;
int ReceiveGyro()
{
	BOOL bRead =TRUE;
	BOOL bResult=TRUE;
	DWORD dwError=0;
	DWORD BytesRead =0;
	
	int readbyte=1;
	unsigned char RXBuff[10];
	float yaw;
	bool flag=0;
	int i=0;
	BYTE temp[3]={-1,-1,-1};
	bool flag1=0,flag2=0;
	
	for(;;){
		bResult=ClearCommError(hCommgyro, &dwError,&comstatgyro);
		if(comstatgyro.cbInQue== 0)
			return 0;
		if(bRead){
			bResult=ReadFile(hCommgyro,//HandletoCOMMport
				&RXBuff,//RXBufferPointer
				readbyte, //Readonebyte
				&BytesRead,//Stores numberofbytes read
				&m_ovgyro);//pointertothem_ov structure


			if(!bResult){
				switch(dwError=GetLastError()){
case ERROR_IO_PENDING:
				{
					bRead=FALSE;
					break;
				}
				default: {
					break;
						 }
				}
			}
			else {
				bRead =TRUE;
			} // close if (bRead)
			if (!bRead) {
				bRead = TRUE;
				bResult = GetOverlappedResult(hCommgyro, //Handle to COMM port
					&m_ovgyro,// Overlapped structure
					&BytesRead, // Stores number of bytes read
					TRUE); //Wait flag
			}
		}
		if(!flag)temp[i++]=RXBuff[0];
		if(flag){
		//	PRINTF_LOG("%d\n",RXBuff[1]*256+RXBuff[2]);
			yaw = yaw = (RXBuff[1]<<8)|RXBuff[2];
			if(yaw > 32768) yaw = 32768 - yaw;
			robotAngle = yaw/10;
			PRINTF_LOG("%d\n",robotAngle);
			readbyte=1;
			temp[0]=0;
			temp[1]=0;
			flag=0;
			i=0;
		}
		if(temp[0]==0xA5&&temp[1]==0x5A){
			readbyte=8;
			flag=1;
		}
		else if(temp[1]>=0) {
			temp[0]=RXBuff[0];
		
			i=1;

		}
	}
}
int SendGyro(BYTE* m_szWriteBuffer,DWORD m_nToSend)
{
	BOOL bWrite =TRUE;
	BOOL bResult =TRUE;
	DWORD BytesSent = 0;

	if (bWrite) {
		m_ovgyro.Offset =0;
		m_ovgyro.OffsetHigh = 0;
		// Clear buffer
		bResult = WriteFile(hCommgyro, // Handle to COMM Port
			m_szWriteBuffer, // Pointer to message buffer in calling finction
			m_nToSend, // Length of message to send
			&BytesSent, //Where to store the number of bytes sent
			&m_ovgyro );//Overlapped structure
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
			; bResult = GetOverlappedResult(hCommgyro, //Handle to COMM port
			&m_ovgyro,// Overlapped structure
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
bool GyroInit()
{  char command[]="INIT#";
   SendGyro((BYTE*)command,strlen(command));
   return true;
}
bool GetGyro()
{  char command[]="IMU#";
   SendGyro((BYTE*)command,strlen(command));
   return true;
}
bool Serial_Gyroscope()
{

	if(openport("com9",hCommgyro))
		PRINTF_LOG("open Gyro-port success\n");
	else return FALSE;
	if(setupdcb(115200,hCommgyro))
		PRINTF_LOG("setupdcb success\n");
	else return FALSE;
	if(setuptimeout(0,0,0,0,0,hCommgyro))
		PRINTF_LOG("setuptime sucucess\n");
	else return FALSE;
	PurgeComm(hCommgyro,
		PURGE_RXCLEAR|PURGE_TXCLEAR
		|PURGE_RXABORT|PURGE_TXABORT);
	return true;
}
