/*****************************************************************************
机器人底盘接口
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include"SerialPort.h"
#include"RobotParam.h"
#include "Debug_message.h"
HANDLE hCommrobot; //串口handle
OVERLAPPED m_ov1;
COMSTAT comstat1;
DWORD m_dwCommEvents1;
unsigned int crc_ta_8[256]={ /* CRC 字节余式表 */
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
unsigned int crc_cal_by_byte(unsigned char* ptr, unsigned char len)
{
	unsigned short crc = 0;

	while(len-- != 0)
	{
		unsigned int high = (unsigned int)(crc/256); //取CRC高8位
		crc <<= 8;
		crc ^= crc_ta_8[high^*ptr];
		ptr++;
	}

	return crc;
}

int SendCommand(BYTE* m_szWriteBuffer,DWORD m_nToSend)
{
	BOOL bWrite =TRUE;
	BOOL bResult =TRUE;
	DWORD BytesSent = 0;

	if (bWrite) {
		m_ov1.Offset =0;
		m_ov1.OffsetHigh = 0;
		// Clear buffer
		bResult = WriteFile(hCommrobot, // Handle to COMM Port
			m_szWriteBuffer, // Pointer to message buffer in calling finction
			m_nToSend, // Length of message to send
			&BytesSent, //Where to store the number of bytes sent
			&m_ov1 );//Overlapped structure
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
			; bResult = GetOverlappedResult(hCommrobot, //Handle to COMM port
			&m_ov1,// Overlapped structure
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
bool GoForward(int speed,int deflection){           //以0.05m/s直走
    speed*=100;
	deflection*=100;
	deflection+=9000;
	BYTE command[14]={0xAA,0x20,0x20,0x36,0x06,0xC4,0x22};
    command[5]=deflection&0x00ff;
    command[6]=(deflection&0xff00)>>8;
	command[7]=speed&0x00ff;
	command[8]=(speed&0xff00)>>8;
	command[9]=0x0;
	command[10]=0x0;
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,11);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[11]=crc1;
	command[12]=crc;
	command[13]=0x0D;
	//PRINTF_LOG("%x,%x",command[11],command[12]);
	SendCommand(command,14);
	return 1;
}
bool RecWalk(){           //以0.05m/s带往右方向矫正的直走
	BYTE command[14]={0xAA,0x20,0x20,0x36,0x06,0xC4,0x22,0xE8,0x03,0x00,0x00};
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,11);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[11]=crc1;
	command[12]=crc;
	command[13]=0x0D;
	//PRINTF_LOG("%x,%x",command[11],command[12]);
	SendCommand(command,14);
	return 1;
}
bool RobotStop(){
	BYTE command[8]={0xAA,0x20,0x20,0x31,0x00,0xE6,0xEB,0x0D};
	SendCommand(command,8);
	return 1;
}
bool CrossDoor(){
	BYTE command[8]={0xAA,0x01,0x01,0x37,0x00,0x4C,0xB1,0x0D};
	SendCommand(command,8);
	return 1;
}
bool RobotCharge(){
	BYTE command[8]={0xAA,0x01,0x01,0x2C,0x00,0xC5,0x6E,0x0D};
	SendCommand(command,8);
	return 1;
}
bool Turnleft(int angle){
	angle=angle*10;
	BYTE command[16]={0xAA,0x20,0x20,0x36,0x08,0x00,0x00,0x00,0x0,0xD0,0x07};
	command[11]=angle&0x00ff;
	command[12]=(angle&0xff00)>>8;
//	PRINTF_LOG("%x,%x",command[11],command[12]);
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,13);
crc1=crc&0x00ff;
crc=crc&0xff00;
crc=crc>>8;
	command[13]=crc1;
command[14]=crc;
command[15]=0x0D;
	if(SendCommand(command,16))
	return 1;
	else 
		return 0;
}
bool Turnright(int angle){
	angle=-angle*10;
	BYTE command[16]={0xAA,0x20,0x20,0x36,0x08,0x00,0x00,0x00,0x00,0xD0,0x07};
	command[11]=(angle&0x00ff);
	command[12]=(angle&0xff00)>>8;
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,13);
crc1=crc&0x00ff;
crc=crc&0xff00;
crc=crc>>8;
	command[13]=crc1;
command[14]=crc;
command[15]=0x0D;
	if(SendCommand(command,16))
	return 1;
	else
		return 0;
}
bool Rotate(int angle){
	DWORD sleeptime=abs(angle)*8000/100;
	angle=angle*10;
	BYTE command[16]={0xAA,0x20,0x20,0x36,0x08,0x00,0x00,0x00,0x00,0xD0,0x07};
	command[11]=(angle&0x00ff);
	command[12]=(angle&0xff00)>>8;
	//	PRINTF_LOG("%x,%x",command[11],command[12]);
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,13);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[13]=crc1;
	command[14]=crc;
	command[15]=0x0D;
	Sleep(50);
	if(SendCommand(command,16))
	{   Sleep(sleeptime);
		return 1;
	}
	else
		return 0;
}



bool leftmove(){BYTE command[14]={0xAA,0x20,0x20,0x36,0x06,0x50,0x46,0xE8,0x03,0x00,0x00};
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,11);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[11]=crc1;
	command[12]=crc;
	command[13]=0x0D;
	//PRINTF_LOG("%x,%x",command[11],command[12]);
	SendCommand(command,14);
	return 1;
}
bool rightmove(){
BYTE command[14]={0xAA,0x20,0x20,0x36,0x06,0x00,0x00,0xE8,0x03,0x00,0x00};
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,11);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[11]=crc1;
	command[12]=crc;
	command[13]=0x0D;
	//PRINTF_LOG("%x,%x",command[11],command[12]);
	SendCommand(command,14);
	return 1;}
bool Goback(){
	BYTE command[14]={0xAA,0x20,0x20,0x36,0x06,0xD8,0xDC,0xE8,0x03,0x00,0x00};
	unsigned short crc,crc1;
	crc=crc_cal_by_byte(command,11);
	crc1=crc&0x00ff;
	crc=crc&0xff00;
	crc=crc>>8;
	command[11]=crc1;
	command[12]=crc;
	command[13]=0x0D;
	//PRINTF_LOG("%x,%x",command[11],command[12]);
	SendCommand(command,14);
	return 1;
}
bool GoForward10(){
	GoForward(10,0);
	Sleep(1500);
	RobotStop();
	return 1;
}
bool GoBack10(){
	Goback();
	Sleep(1500);
	RobotStop();
	return 1;
}
bool GoLeft10(){
	leftmove();
	Sleep(1500);
	RobotStop();
	return 1;
}
bool GoRight10(){
	rightmove();
	Sleep(1500);
	RobotStop();
	return 1;
}
bool Serial_Robot()
{
		if(openport(RobotPort,hCommrobot))
			PRINTF_LOG("open Robot-port success\n");
		else return FALSE;
		if(setupdcb(115200,hCommrobot))
			PRINTF_LOG("setupdcb success\n");
		else return FALSE;
		if(setuptimeout(0,0,0,0,0,hCommrobot))
			PRINTF_LOG("setuptime sucucess\n");
		else return FALSE;
		PurgeComm(hCommrobot,
			PURGE_RXCLEAR|PURGE_TXCLEAR
			|PURGE_RXABORT|PURGE_TXABORT);
		return true;
}

