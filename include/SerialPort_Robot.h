#ifndef _SERIALPORT_ROBOT_H_
#define _SERIALPORT_ROBOT_H_
bool Serial_Robot();
bool GoForward(int speed,int deflection);
bool RobotStop();
bool Turnleft(int angle);
bool Turnright(int angle);
bool leftmove();
bool rightmove();
bool Goback();
bool RecWalk();
bool CrossDoor();
bool RobotCharge();
bool Rotate(int angle);
bool GoForward10();
bool GoBack10();
bool GoLeft10();
bool GoRight10();
#endif