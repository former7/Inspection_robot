#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
#include<windows.h>
bool openport(char*portname,HANDLE &hComm);
bool setupdcb(int rate_arg,HANDLE &hcomm);
bool setuptimeout(DWORD ReadInterval,
				  DWORD ReadTotalMultiplier,
				  DWORD ReadTotalconstant,
				  DWORD WriteTotalMultiplier,
				  DWORD WriteTotalconstant,
				  HANDLE &hComm);
#endif
