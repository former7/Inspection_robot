#include"NetWorkInit.h"
#include"RobotStatusManager.h"
#include"RobotParam.h"
#include"RouteArrayManager.h"
#include"SerialPort_Lifter.h"
#include"HKCamera.h"
#include"MsgQueue.h"
#include"OrbDataInterface.h"
#include "Debug_message.h"
extern RobotStatusManager RobotState;
extern MsgQueue RMqueue;
//extern HANDLE GetRouteSuccess;
RouteArrayManager route;
void SendMoveCommand(int param1,int param2,int param3)
{
	Command RobotMsg;
	RobotMsg.msgtype=param1;
	RobotMsg.msgvalue1=param2;
	RobotMsg.msgvalue2=param3;
	RMqueue.sendMsg(&RobotMsg);
	WaitForSingleObject(SendSuccess,INFINITE);
	//cout<<"send success"<<endl;
}

MoveMode CalculateAngle(int Targetx,int Targety,int Lastx,int Lasty )
{       
	Direction RobotDirection=RobotState.getDirection();
    MoveMode movemode;
	int RobotSpeed=INISPEED;
	int tempx=Targetx-Lastx;
    int tempy=Targety-Lasty;//计算运动路线向量
	if(tempx==0)
	{
		movemode.GoWithxy=1;
	}
	else if(tempy==0)
	{
			movemode.GoWithxy=2;
	}
	else movemode.GoWithxy=0;
	PRINTF_LOG("movemode.GoWithxy=%d\n",movemode.GoWithxy);
	double t1=RobotDirection.x*RobotDirection.x+RobotDirection.y*RobotDirection.y;
	double t2=(tempx*tempx)+tempy*tempy;
	double t3=(RobotDirection.x*tempx+RobotDirection.y*tempy)/(sqrt(t1)*sqrt(t2));
	float TurnAngle=acos(t3)*180/PI;             //余弦公式计算转角
	int temp=RobotDirection.x*tempy-RobotDirection.y*tempx;//叉乘计算转向
	int turn;
	if(temp==0)
	{
	  turn=0;
	  if(tempx*RobotDirection.x<0||tempy*RobotDirection.y<0)
	  {
		 turn=1;
		 TurnAngle=180;
	  }
	}
	else if(temp>0)
	{
		turn=1;
	}
	else
	{
		turn=-1;
	}
	PRINTF_LOG("t2 :%f\n",sqrt(t2));
	if(sqrt(t2)>DisThreshold)
	{
		movemode.LThreshold=1;
	}
	else if (sqrt(t2)>DisThreshold2)
	{
		movemode.LThreshold=2;
	}
	else
		movemode.LThreshold=0;
	movemode.TurnAngle=TurnAngle;
	PRINTF_LOG("TurnAngle %f\n",TurnAngle);
	movemode.turn=turn;
	//PRINTF_LOG("turn%d\n",turn);
	RobotState.UpdateDirection(tempx,tempy); //更新机器人朝向
	return movemode;

}
unsigned _stdcall PathNavigation(void* param)
{   
	int log_times = 0;
	int log_times_1 = 0;
	int log_times_2 = 0;
	while(true)//接收规划路径
	{	  
	  if(Get_Route(sock_fd)==1)
	  {	  
		  PRINTF_LOG("get route success\n");
		  Sleep(1000);
		  Get_Route_SUCCESS(sock_fd);
		  //SetEvent(GetRouteSuccess);
		  break;
	  }
	  Sleep(2000);
	}
	
	SendMoveCommand(CROSS,0,0); //  过门命令
	/*
    route.addRoutePointAtTail(70,532,0,0,0,0);
    route.addRoutePointAtTail(70,65,0,0,0,0);
	route.addRoutePointAtTail(235,65,2,1,1,0);
	//route.addRoutePointAtTail(235,300,0,0,0,0);
	route.addRoutePointAtTail(235,520,2,2,1,0);
	route.addRoutePointAtTail(315,520,0,0,0,0);
	route.addRoutePointAtTail(315,65,0,0,0,0);
	route.addRoutePointAtTail(443,65,0,0,0,0);
	route.addRoutePointAtTail(443,405,0,0,0,0);
	*/
	
	
	Location RobotLocation;
	MoveMode movemode={0,0,0,0};
	Direction RobotDirection={InitialX,InitialY,InitialAngle};
	RobotState.UpdateDirection(InitialX,InitialY);
	int Point_x,Point_y;//记录目标点坐标
	int lift,direction,remain,door;//记录是否拍照，拍照方向，高度
	int route_num=route.getRouteArraySize();//路径点个数
	PRINTF_LOG("\nsize:%d\n",route_num);
	int i=0;//路径点索引
	int RobotSpeed=INISPEED;
	int SLOWDOWNFLAG=0;
	int threshold;
	int Safethreshold=12000;
	OrbData.barriers=1;
	while(i++<(route_num-1))
	{
		Point_x=route.getRoutePointIndex(i)->x;//下一点X坐标
	    Point_y=route.getRoutePointIndex(i)->y;//下一点Y坐标
		PRINTF_LOG("下一点坐标%d,%d\n",Point_x,Point_y);
		lift=(route.getRoutePointIndex(i)->lift)*500;
		direction=route.getRoutePointIndex(i)->direct;
		remain=route.getRoutePointIndex(i)->remain;
		door=route.getRoutePointIndex(i)->door;

		movemode=CalculateAngle(Point_x,Point_y,route.getRoutePointIndex(i-1)->x,route.getRoutePointIndex(i-1)->y);
		if(movemode.LThreshold==0)
			RobotSpeed=10;
		if(movemode.LThreshold==1)
		{
				RobotSpeed=30;
				Safethreshold=12000;
		}
		if(movemode.LThreshold==2)
		{
			RobotSpeed=20;
			Safethreshold=8100;
		}
		while(1)
	    {  
		 
		  WaitForSingleObject(ControlSemaphore,INFINITE);
		 /*if(!SLOWDOWNFLAG)
		 {
		    if(OrbData.barriers==0)
		    {   
			//PRINTF_LOG("无障碍物,全速\n");
			RobotSpeed=FASTSPEED;
			PRINTF_LOG("RobotSpeed %d\n",RobotSpeed);
		    }
		    else
		    {   
			//PRINTF_LOG("有障碍物,低速\n");
		    RobotSpeed=INISPEED;
			PRINTF_LOG("RobotSpeed %d\n",RobotSpeed);
		    }
		 }*/
		  /*
		  if(AfterManual==1)
		  {
			 RobotLocation=RobotState->getRobotLocation();
		     movemode=CalculateAngle(Point_x,Point_y,RobotLocation.x,RobotLocation.y);
			 RobotState->UpdateDirection(Point_x-RobotLocation.x,Point_y-RobotLocation.y);
			 AfterManual=0;
		  }
		  */
		  RobotLocation=RobotState.getRobotLocation();
		  int PointDistance;
		  log_times++;
		  if(log_times%20==0){
			PRINTF_LOG("当前坐标：%d,%d \n",RobotLocation.x,RobotLocation.y);
			log_times = 21;
		  }
		  
		  if(movemode.GoWithxy==1)
		  {
			  PointDistance=(RobotLocation.y-Point_y)*(RobotLocation.y-Point_y);
			  threshold=150;
		  }
		  else  if(movemode.GoWithxy==2)
		  {
			  PointDistance=(RobotLocation.x-Point_x)*(RobotLocation.x-Point_x);
			  threshold=150;
		  }
		  else
		  {
			  PointDistance= (RobotLocation.x-Point_x)*(RobotLocation.x-Point_x)+ (RobotLocation.y-Point_y)*(RobotLocation.y-Point_y);	  
		      threshold=430;
		  }
		  if(RobotLocation.x==0&&RobotLocation.y==0)
			  PointDistance=100000;
		/*
		  if(PointDistance<=threshold2)
		  {
		      RobotSpeed=10;
		  }
		  */
		  if(log_times_2%10==0){
				PRINTF_LOG("PointDistance; %d\n",PointDistance);
				log_times_2 = 21;
			}
		 
		  if(PointDistance>=threshold)//如果定位坐标与路径规划点距离大于阈值
		  {
			if(PointDistance<=Safethreshold)
			{
			    RobotSpeed=10;
				SLOWDOWNFLAG=1;
			}
			log_times_1++;
			if(log_times_2%10==0){
				PRINTF_LOG("RPBPSPEED %d\n",RobotSpeed);
				log_times_2 = 21;
			}
			
			if(movemode.turn==1)
			{ 
				PRINTF_LOG("***************\n向右方转角%d\n***************\n",movemode.TurnAngle);
			    SendMoveCommand(Turn,-(movemode.TurnAngle-1),0);			
				movemode.turn=0;

			}
		    if(movemode.turn==-1)
			{  
				PRINTF_LOG("***************\n向左方转角%d\n***************\n",movemode.TurnAngle);
			    SendMoveCommand(Turn,movemode.TurnAngle,0);
			    movemode.turn=0;
			}
			if(movemode.turn==0)
			{
					//PRINTF_LOG("向正前方行走\n");
					SendMoveCommand(Forward,RobotSpeed,WalkAngel); //10CM/s前进
					Sleep(100);
			}	
		  }
		 
		  else 
		  {
			 SendMoveCommand(Stop,0,0);     //机器人停止  // 每次到达预设点位
			 PRINTF_LOG("***************\n到达指定位置。停止，计算路线\n***************\n");
			 //if(door == 1){
			 // SendMoveCommand(CROSS,0,0);
			 //}
			 if(remain>0)
			 {
              LiftOption(lift);  //通知升降杆
			  WaitForSingleObject(LifterEvent, INFINITE);//等待升降杆升高到指定高度
			  //ResetEvent(LifterEvent);	
			  CapturePicture(direction,i);//拍照并上传
			  //Sleep(1000);
			  LiftOption(750);
			  //LifterCheckSelf();
			  WaitForSingleObject(LifterEvent, INFINITE);//等待升降杆升高到指定高度
		     }
				Sleep(300);
				ReleaseSemaphore(ControlSemaphore, 1, NULL);
				break;
		  } 
	      ReleaseSemaphore(ControlSemaphore, 1, NULL);
		  Sleep(20);
	   }
    }
	//delete[]route;
	WaitForSingleObject(ControlSemaphore,INFINITE);
	SendMoveCommand(CHARGE,0,0);
	ReleaseSemaphore(ControlSemaphore, 1, NULL);
	SetEvent(NavigateOver);
	
	return 0;
}
	/*tempx=Point_x-route->getRoutePointIndex(i-1)->x;;
		tempy=Point_y-route->getRoutePointIndex(i-1)->y;//计算运动路线向量	
		t1=RobotDirection.x*RobotDirection.x+RobotDirection.y*RobotDirection.y;
		t2=(tempx*tempx)+tempy*tempy;
		t3=(RobotDirection.x*tempx+RobotDirection.y*tempy)/(sqrt(t1)*sqrt(t2));
		TurnAngle=acos(t3)*180/PI;             //余弦公式计算转角
		int temp=RobotDirection.x*tempy-RobotDirection.y*tempx;//叉乘计算转向
		if(temp==0)
		{
			turn=0;
			if(tempx*RobotDirection.x<0||tempy*RobotDirection.y<0)
			{
				turn=1;
				TurnAngle=180;
			}
		}
			else if(temp>0)
			{
				turn=1;
			}
			else
			{
				turn=-1;
			}
			*/