/*****************************************************************************
传感器接口
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"SerialPort.h"
#include"RobotParam.h"

#include "Debug_message.h"
HANDLE hCommsensors; //串口handle
OVERLAPPED m_ov2;
COMSTAT comstat2;
DWORD m_dwCommEvents2;
int dsmin=5000,dsleftmin=5000,dsrightmin=5000;  //车体前左右障碍物最小测距
int ds[16]; //超声波数据


int ReceiveSensors()
{
	BOOL bRead =TRUE;
	BOOL bResult=TRUE;
	DWORD dwError=0;
	DWORD BytesRead =0;
	ds[0]=5000;
	ds[1]=5000;
	dsmin=5000;
	int readbyte=1,flag=0,i=0;
	unsigned char RXBuff[36],temp[3]={-1,-1,-1};
	bool flag1=0,flag2=0,flag3=0,flag4=0;	
	int count=1,avecount=0;//count为命令字符计数，18个字符为一次定位数据，avecount为计算平均值的计数
	for(;;){
	bResult=ClearCommError(hCommsensors, &dwError,&comstat2);
	if(comstat2.cbInQue== 0)
	continue;
	if(bRead)
	{
		bResult=ReadFile(hCommsensors,//HandletoCOMMport
		&RXBuff,//RXBufferPointer
		readbyte, //Readonebyte
		&BytesRead,//Stores numberofbytes read
		&m_ov2);//pointertothem_ov structure

	//PRINTF_LOG("%x ",RXBuff[0]);
	 if((!flag)&&(!flag1)&&(!flag2))temp[i++]=RXBuff[0];
	 if(flag)
	 {
	  readbyte=1;
	  temp[0]=-1;
	  temp[1]=-1;
	  temp[2]=-1;
  
	  /*for(int i=1,k=0;k<16;i+=2,k++)
	  {
	   ds[k]=RXBuff[i+1]*256+RXBuff[i];
	   PRINTF_LOG("%d ",ds[k]);
	  }*/
	 // PRINTF_LOG("1 %d",RXBuff[0]*256+RXBuff[1]);
 
	 // PRINTF_LOG("\n");
	  ds[0]=RXBuff[0]*256+RXBuff[1];
	  if(ds[0]<=ds[1])dsmin=ds[0];
	  PRINTF_LOG("dsmin %d\n",dsmin);
	  flag=0;
	  i=0;
	}
if(flag1){
  readbyte=1;
  temp[0]=-1;
  temp[1]=-1;
  temp[2]=-1; 
  //PRINTF_LOG("8and5 %d",RXBuff[0]*256+RXBuff[1]);
 // PRINTF_LOG("8 %d",RXBuff[2]*256+RXBuff[3]);
 // PRINTF_LOG("\n");
  ds[1]=RXBuff[2]*256+RXBuff[3];
  if(ds[1]<=ds[0])dsmin=ds[1];
  PRINTF_LOG("dsmin %d\n",dsmin);
  flag1=0;
  i=0;
}
if(flag2){
  readbyte=1;
  temp[0]=-1;
  temp[1]=-1;
  temp[2]=-1;
  flag2=0;
  i=0;
}
/*if(temp[0]==0xAA&&temp[1]==0xAA){
   if(temp[2]==0x00){
	readbyte=36;   //16路
	//readbyte=20;     //8路
	flag=1;}
   if(temp[2]==0x01){
	readbyte=6;
	flag1=1;}
   if(temp[2]==0x02){
	readbyte=9;
	flag2=1;}
}*/
if(temp[0]==0xAA&&temp[1]==0xAA){
   if(temp[2]==0x01)
   {
	readbyte=6;   //16路
	//readbyte=20;     //8路
	flag=1;
   }
   if(temp[2]==0x04)
   {
	readbyte=6;
	flag1=1;}
   
}
else if(temp[1]>=0&&temp[2]>=0) {
temp[0]=temp[1];
temp[1]=RXBuff[0];

i=2;

}


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
bResult = GetOverlappedResult(hCommsensors, //Handle to COMM port
&m_ov2,// Overlapped structure
&BytesRead, // Stores number of bytes read
TRUE); //Wait flag
   }
 }

}
}
int ReceiveSensors2()
{ 
	DWORD BytesRead;
	DWORD dwError;
	DWORD ReadError;
	int recvx,recvy;
	for(;;){
		Sleep(50);
		BYTE bufferRecv[1024];
		ClearCommError(hCommsensors, &dwError,&comstat2);
		//PRINTF_LOG("%d\n",comstat2.cbInQue);
		if(comstat2.cbInQue== 0)
		{
			//Sleep(10);
			continue;
		}
		ReadFile(hCommsensors,//Handle to COMM port
			bufferRecv,//RXBuffer Pointer
			comstat2.cbInQue, //Read m_ComStat.cbInQue bytes
			&BytesRead,//Stores number of bytes read
			&m_ov2);//pointer to the m_ov structure
		//bufferRecv[comstat2.cbInQue]='\0';
		
		//PRINTF_LOG("%x ",bufferRecv[0]);
		//PRINTF_LOG("%x",bufferRecv[1]);
		for(int i=0;(i<comstat2.cbInQue)&&(i<1024);i++)
		{
			//PRINTF_LOG("%x ",bufferRecv[i]);
			if((bufferRecv[i]==(0xAA))&&((i+38)<=comstat2.cbInQue))
			{

				if(bufferRecv[i+1]==0xAA)
				{  
					if(bufferRecv[i+2]==0x00)
					{
						ds[0]=(bufferRecv[i+5]<<8)+bufferRecv[i+4];
						if(ds[0]==0)ds[0]=5000;
						ds[1]=(bufferRecv[i+35]<<8)+bufferRecv[i+34];
						if(ds[1]==0)ds[1]=5000;
						if(ds[0]<ds[1])
							dsmin=ds[0];
						else
							dsmin=ds[1];
						PRINTF_LOG("ds0 %d ds1 %d dsmin %d\n",ds[0],ds[1],dsmin);
					}
					
					i+=38;

				}
			}
		}

	}


}
bool Serial_Sensors()
{
if(openport(SensorsPort,hCommsensors))
	PRINTF_LOG("open Sensors-port success\n");
else return FALSE;
if(setupdcb(115200,hCommsensors))
	PRINTF_LOG("setupdcb success\n");
else return FALSE;
if(setuptimeout(0,0,0,0,0,hCommsensors))
	PRINTF_LOG("setuptime sucucess\n");
else return FALSE;
PurgeComm(hCommsensors,
		  PURGE_RXCLEAR|PURGE_TXCLEAR
		  |PURGE_RXABORT|PURGE_TXABORT);
return true;




}
