/*****************************************************************************
超宽带接口
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"Serialport.h"
#include"RobotParam.h"
#include"RobotStatusManager.h"
#include"RobotDataProcess.h"
#include "Debug_message.h"
HANDLE hCommlocation; //串口handle
OVERLAPPED m_ov;
COMSTAT comstat;
DWORD m_dwCommEvents;
extern RobotStatusManager RobotState;
//extern RobotDataProcess*RobotData;
int ReceiveLocation(int startx,int starty)
{   
	HANDLE LocationReceived;
	BOOL bRead =TRUE;
	BOOL bResult=TRUE;
	DWORD dwError=0;
	DWORD BytesRead =0;
	int readbyte=1;
	unsigned char RXBuff[18],tempx1=0,tempy1=0,tempz1=0;
	int sumx=0,sumy=0,tempx3=0,tempy3=0,tempx2=startx,tempy2=starty;
	int ave[2][5],j=0,n=0;
	bool flag=0;
	int i=0;
	BYTE temp[3]={-1,-1,-1};
	bool flag1=0,flag2=0;
	int count=1,avecount=0;//count为命令字符计数，18个字符为一次定位数据，avecount为计算平均值的计数
	LocationReceived=CreateEvent(NULL,FALSE,FALSE,"locationreceived");
	for(;;){
		bResult=ClearCommError(hCommlocation, &dwError,&comstat);
		if(comstat.cbInQue== 0)
		{
			  //Sleep(10);
			  continue;
		}
		if(bRead){
			bResult=ReadFile(hCommlocation,//HandletoCOMMport
				&RXBuff,//RXBufferPointer
				readbyte, //Readonebyte
				&BytesRead,//Stores numberofbytes read
				&m_ov);//pointertothem_ov structure


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
				bResult = GetOverlappedResult(hCommlocation, //Handle to COMM port
					&m_ov,// Overlapped structure
					&BytesRead, // Stores number of bytes read
					TRUE); //Wait flag
			}
		}
		if(!flag)temp[i++]=RXBuff[0];
		if(flag){
	        tempx3=RXBuff[7]*256+RXBuff[6];
			tempy3=RXBuff[9]*256+RXBuff[8];
			ave[0][n]=tempx3;
			ave[1][n]=tempy3;
			avecount++;
			n++;
			
			if(((tempx3-tempx2)>errnum)||((tempx3-tempx2)<-errnum)||((tempy3-tempy2)>errnum)||((tempy3-tempy2)<-errnum))
				{  
					PRINTF_LOG("错误坐标,舍弃\n");
					avecount--;
					n--;
					continue;
				}	
			if(avecount==5)
			{
				while(j<5)
				{
					sumx+=ave[0][j];
					sumy+=ave[1][j];
					j++;
				}
				j=0;
				tempx2=sumx/avenum;
				tempy2=sumy/avenum;
				//PRINTF_LOG("%d,%d\n",tempx2,tempy2);
				RobotState.UpdateLocation(tempx2,tempy2);
				//SetEvent(LocationReceived);
			    //RobotData->setLocationEventHandle(&LocationReceived);
				sumx=0;
				sumy=0;
				for(int k=0;k<4;k++)
				{
				ave[0][k]=ave[0][k+1];
				ave[1][k]=ave[1][k+1];
				}
				n=4;
				avecount=4;

			}

			
			readbyte=1;
			temp[0]=0;
			temp[1]=0;
			flag=0;
			i=0;
		}
		if(temp[0]==0x57&&temp[1]==0x58){
			readbyte=16;
			flag=1;
		}
		else if(temp[1]>0) {
			temp[0]=RXBuff[0];
		
			i=1;

		}
	}
}
int ReceiveLocation2(int startx,int starty)
{ 
	DWORD BytesRead;
	DWORD dwError;
	DWORD ReadError;
	int recvx,recvy;
	SYSTEMTIME sys; // GetLocalTime( &sys ); 定义时间通过调用来获取当前时间
	
	BYTE bufferRecv[1280];
	Location RobotLocation; // 坐标信息
	while(1)
	{
		
		ClearCommError(hCommlocation, &dwError,&comstat);
		if(comstat.cbInQue== 0)
		{
			Sleep(60);
			continue;
		}
		Sleep(20);
		ReadFile(hCommlocation,bufferRecv,comstat.cbInQue, &BytesRead,&m_ov);
		for(int i=0;(i<comstat.cbInQue)&&(i<1280);i++)
		{
		   if((bufferRecv[i]==0x57))
		   {   
			   if(bufferRecv[i+1]==0x58&&((i+17)<=comstat.cbInQue))
			   {			   			   
					recvx=(bufferRecv[i+9]<<8)+bufferRecv[i+8];
					recvy=(bufferRecv[i+11]<<8)+bufferRecv[i+10];
					RobotState.UpdateLocation(recvx,recvy);
					i+=17;
				//	GetLocalTime( &sys ); 
				//	PRINTF_LOG( "%4d/%02d/%02d %02d:%02d:%02d.%03d 星期%1d\n",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond,sys.wMilliseconds,sys.wDayOfWeek); 
				//	PRINTF_LOG("location x = %d ,y = %d .\n",recvx,recvy);					
				//	RobotLocation=RobotState.getRobotLocation();
				//	PRINTF_LOG("当前坐标：%d,%d \n",RobotLocation.x,RobotLocation.y);
			   }
		   }
		}
	}
	

}
bool Serial_Location()
{

	if(openport(LocationPort,hCommlocation))
		PRINTF_LOG("open Location-port success\n");
	else return FALSE;
	if(setupdcb(115200,hCommlocation))
		PRINTF_LOG("setupdcb success\n");
	else return FALSE;
	if(setuptimeout(0,0,0,0,0,hCommlocation))
		PRINTF_LOG("setuptime sucucess\n");
	else return FALSE;
	PurgeComm(hCommlocation,
		PURGE_RXCLEAR|PURGE_TXCLEAR
		|PURGE_RXABORT|PURGE_TXABORT);
	return true;
}
