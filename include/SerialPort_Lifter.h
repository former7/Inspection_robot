#ifndef _SERIALPORT_LIFTER_H_
#define _SERIALPORT_LIFTER_H_
#include<windows.h>
int SendLifter(BYTE* m_szWriteBuffer,DWORD m_nToSend);
int ReceiveLifter();
bool LiftOption(int height);
int Serial_Lifter();
bool SendCheck();
bool LifterCheckSelf();
#endif