
#include<stdio.h>
#include<stdlib.h>
#include"Serialport.h"
#include "Debug_message.h"
bool openport(char*portname,HANDLE &hComm)//打开一个串口
{
	hComm=CreateFile(portname,
		GENERIC_READ|GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
/*	hComm=CreateFile(portname,
		GENERIC_READ|GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0);*/
	if(hComm==INVALID_HANDLE_VALUE)
		return FALSE;
	else
		return true;

}

bool setupdcb(int rate_arg,HANDLE &hcomm)
{
	DCB dcb;
	int rate=rate_arg;
	memset(&dcb,0,sizeof(dcb));
	if(!GetCommState(hcomm,&dcb))//获取当前 DCB 配置
	{
		return FALSE;
	}
	/*-------------------------------------------------------------------- */
	//setDCBtoconfigure the serialport
	dcb.DCBlength=sizeof(dcb);
	/*----------Serial PortConfig -------*/
	dcb.BaudRate =rate;
	dcb.Parity=NOPARITY;
	dcb.fParity=0;
	dcb.StopBits=ONESTOPBIT;
	dcb.ByteSize =8;
	dcb.fOutxCtsFlow =0;
	dcb.fOutxDsrFlow =0;
	dcb.fDtrControl=DTR_CONTROL_DISABLE;
	dcb.fDsrSensitivity=0;
	dcb.fRtsControl=RTS_CONTROL_DISABLE;
	dcb.fOutX=0;
	dcb.fInX=0;
	/*-----------------misc parameters----- */
	dcb.fErrorChar =0;
	dcb.fBinary =1;
	dcb.fNull=0;
	dcb.fAbortOnError =0;
	dcb.wReserved=0;
	dcb.XonLim=2;
	dcb.XoffLim =4;
	dcb.XonChar =0x13;
	dcb.XoffChar =0x19;
	dcb.EvtChar =0;
	/*-------------------------------------------------------------------- */
	//setDCB
	if(!SetCommState(hcomm,&dcb))
	{
		return false;
	}
	else
		return true;
}
bool setuptimeout(DWORD ReadInterval,
				  DWORD ReadTotalMultiplier,
				  DWORD ReadTotalconstant,
				  DWORD WriteTotalMultiplier,
				  DWORD WriteTotalconstant,
				  HANDLE &hComm)
{
	COMMTIMEOUTS timeouts;timeouts.ReadIntervalTimeout=ReadInterval;
	timeouts.ReadTotalTimeoutConstant=ReadTotalconstant;
	timeouts.ReadTotalTimeoutMultiplier=ReadTotalMultiplier;
	timeouts.WriteTotalTimeoutConstant=WriteTotalconstant;
	timeouts.WriteTotalTimeoutMultiplier=WriteTotalMultiplier;
	if(!SetCommTimeouts(hComm,&timeouts))
	{
		return false;
	}
	else
		return true;
}
