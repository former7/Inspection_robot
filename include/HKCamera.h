#ifndef _HKCAMERA_H_
#define _HKCAMERA_H_
void HKcameraInit();
void HKcameraInit2();
bool CapturePicture(int direction,int idx);
extern LONG lRealPlayHandle;
extern LONG lUserID;
enum CameraStatus{north,west,south,east}; 
#endif