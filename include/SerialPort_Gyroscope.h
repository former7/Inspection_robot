#ifndef _SERIAPORT_GYROSCOPE_H_
#define _SERIAPORT_GYROSCOPE_H_
#include<windows.h>
bool Serial_Gyroscope();
int ReceiveGyro();
int SendGyro(BYTE* m_szWriteBuffer,DWORD m_nToSend);
bool GyroInit();
bool GetGyro();
#endif