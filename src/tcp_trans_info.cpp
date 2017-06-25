#include <iostream>
#include "tcp_trans_info.h"
#include "NetWorkInit.h"
#include "Debug_message.h"
using namespace std;

#pragma  comment(lib,"ws2_32.lib")

/*
socket客户端
*/


char * read_file(const char *pathpic, int &pic_len){
	//将图片读取出来
	FILE *fp = fopen(pathpic, "rb");     //打开文件
	if (!fp){
		MessageBoxA(NULL, "没有找到文件位置", 0, 0);
		return NULL;
	}
	fseek(fp, 0, SEEK_END);  //一直寻找到文件尾部
	pic_len = ftell(fp);  //得到图片的长度
	rewind(fp);  //rewind将文件指针指向开头
	char *pic_buf = new char[pic_len + 1];  //开辟一个空间在堆上
	memset(pic_buf, 0, pic_len + 1);  //清空文件指针
	//读取文件内容
	fread(pic_buf,sizeof(char),pic_len,fp);
	//测试将文件再保存于D:中
	/*
	MessageBoxA(NULL, "文件开始", 0, 0);
	FILE *fpw = fopen("C:\\AA.jpg","wb");
	fwrite(pic_buf,sizeof(char), pic_len, fpw);
	fclose(fpw); //关闭文件流
	MessageBoxA(NULL, "文件结束", 0, 0);
	*/
	fclose(fp);  

	return pic_buf;
}

/* 接收 ip 地址 */
//void socket_send(const int fd , struct sockaddr_in *pserv_addr){
//
//void socket_send(const int fd , const char *path_ptr){
//	string header_buf ,pic_headr_buf ,str_pic_name  ;
//	string bound_buf , beg_bound,end_bound;
//	char pic_name[PIC_LEN] ,pic_buf[BUFSIZE+1];
//	char ret_buf[BUFSIZE+1];
//	char temp[PIC_LEN]; 
//	int read_num;
//	unsigned long pic_len , sum_len;
//	FILE *fp;
//
//	/* open picture , if picture's size too big , we should loop to send , 
//	* so firstly we get picture"s size and send entire length to server,
//	* then we loop to send picture's data
//	*/
//	if(  ( fp =fopen(path_ptr , "rb") ) ==NULL )
//		oops( "fopen error:");
//
//	/* get picture's size	*/
//	fseek( fp , 0 , SEEK_END );
//	sum_len = pic_len = ftell(fp);
//
//	//pic_ptr = read_file(path_ptr , pic_len);
//	
//	/* init begin and end boundary */
//	bound_buf = "71b23e4066ed";
//	beg_bound = "--" +bound_buf;
//	end_bound = "--" +bound_buf+ "--";
//
//	sum_len += end_bound.length();
//	
//	//snprintf( header_buf , sizeof(header_buf) , "POST http://%s:%d%s HTTP/1.0\r\n" , SERVER_ADDR,
//	//		SERVER_PORT,PATH1);
//
//	/*init html header */
//	header_buf ="POST";
//	header_buf +=PATH1;
//	header_buf += " HTTP/1.1\r\n" ;
//	header_buf +="Host:";
//	header_buf +=SERVER_ADDR;
//	header_buf +=":";
//	header_buf +=SERVER_PORT;
//	header_buf += "\r\n" ;
//	header_buf +="Accept:  */*\r\n" ;
//	//header_buf +="Accept-Language: zh-CN \r\n" ;
//	header_buf +="Pragma: no-cache\r\n" ;
//	header_buf +="Connection:Keep-Alive\r\n\r\n" ;
//	header_buf +="Content-Type: multipart/form-data;boundary=";
//	header_buf +=bound_buf;
//	header_buf +="\r\n" ;
//	sum_len += header_buf.length();
//
//	///PRINTF_LOG( "header_buf :%s\n" , header_buf);
//
//	/*	init pic content  */  
//	pic_headr_buf = beg_bound;  
//	pic_headr_buf +="\r\n" ;
//	get_pic_name( path_ptr , pic_name , '\\');
//	//str_pic_name = pic_name;
//	pic_headr_buf +="Content-Disposition: form-data; name=\"file\"; filename=\"";
//	pic_headr_buf += pic_name;
//	pic_headr_buf += "\"\r\n\r\n"; 
//	pic_headr_buf +="Content-Type: image/jpeg \r\n\r\n"; 
//	sum_len += pic_headr_buf.length();
//
//	//PRINTF_LOG( "pic_headr_buf :%s\n" , pic_headr_buf);
//	////cout << pic_headr_buf << endl;
//
//	/*  Content-Length */
//	header_buf += "Content-Length: ";
//
//	//注意下面这个参数Content-Length，这个参数值是：http请求头长度+请求尾长度+文件总长度
//	// 就分块传送 
//	sprintf(temp, "%d\r\n\r\n",sum_len);
//	header_buf +=temp;
//	header_buf +="\r\n\r\n" ;
//	////PRINTF_LOG( "header_buf :%s\n" , header_buf);
//
//	/* send header */
//	send(fd, header_buf.c_str(), header_buf.length(), 0);
//	Sleep(0.2);
//
//	/* send pic_headr_buf */
//	send(fd, pic_headr_buf.c_str(), pic_headr_buf.length(), 0);
//	Sleep(0.2);
//
//	/* read pic data and send data to server */
//	fseek( fp , 0 , SEEK_SET);
//	memset( (void *)pic_buf , 0 , sizeof(pic_buf));
//	while ( !feof(fp)  ){
//		read_num  = fread( pic_buf ,sizeof(char),BUFSIZE ,fp );
//		send(fd, pic_buf, strlen(pic_buf), 0);
//		Sleep(0.2);
//	}
//
//	/* send data end boundary*/
//	send(fd, end_bound.c_str(), end_bound.length(), 0);
//	Sleep(0.2);
//
//	while( (read_num = recv( fd , ret_buf , sizeof(ret_buf) ,0) ) > 0  ){
//		ret_buf[read_num] = '\0';
//		PRINTF_LOG( "%s", ret_buf );
//	}
//	//}
//}


/* get picture name from picture path , ch present symbol which we chunk*/
void get_pic_name(const char *path_ptr , char *ret_ptr, const char ch){
	int pic_len , index;

	memset( ret_ptr , 0 , sizeof(ret_ptr) );
	pic_len = strlen(path_ptr);
	for (index = pic_len ; index >= 0; --index ){
		/*		find ch	in path ptr		*/
		if ( path_ptr[index] == ch ){
			break;
		}
	}
	/*	we have a deal , if path do not have ch , e.g  path = xxx.jpg or 
	*  path = ./jpg or path = c:/jpg , we copy the entire path to retptr
	*/
	// 	if(index == -1 ){
	// 		strncpy( ret_ptr , path_ptr , pic_len);
	// 		ret_ptr[pic_len] = '\0';
	// 	}
	// 	else{
	// 		strncpy( ret_ptr , path_ptr+index+1 , pic_len-index-1 );
	// 		ret_ptr[pic_len-index] = '\0';
	// 	}

	/* up body equal to the same condition */
	strncpy( ret_ptr , path_ptr+index+1 , pic_len-index-1 );
	ret_ptr[pic_len-index-1] = '\0';
}

void sendPic(const int sHost ,char* addr, char * host, char *pathpic) {

	//先读取文件流
	//实名图片读取
	int Spic_len;
	char *Spic_data=NULL, *Dpic_data=NULL;

	Spic_data=read_file(pathpic, Spic_len);

	string header("");
	string content("");        //实名文件

	//----------------------post头开始--------------------------------  
	header += "POST ";
	header += addr;
	header += " HTTP/1.1\r\n";
	header += "Host: ";
	header += host;
	header += "\r\n";
	header += "Connection: Keep-Alive\r\n";
	header += "Accept: */*\r\n";
	header += "Pragma: no-cache\r\n";
	header += "Content-Type: multipart/form-data;boundary=71b23e4066ed\r\n";
	//cout << header << endl;


	//发送文件数据
	content += "--71b23e4066ed\r\n";
	content += "Content-Disposition: form-data; name=\"file\"; filename=\"";
	content += pathpic;
	content += "\"\r\n";
	content += "Content-Type: image/jpeg \r\n\r\n";
	//cout << content << endl;


	//post尾时间戳  
	std::string strContent("\r\n--71b23e4066ed--\r\n");
	char temp[64] = { 0 };
	//注意下面这个参数Content-Length，这个参数值是：http请求头长度+请求尾长度+文件总长度
	// 就分块传送 
	sprintf(temp, "Content-Length: %d\r\n\r\n",
		content.length()  + Spic_len + strContent.length());
	header += temp;

	std::string str_http_request;
	str_http_request.append(header);

	//----------------------post头结束-----------------------------------
	//发送post头  
	//cout << str_http_request  << endl ;
	send(sHost, str_http_request.c_str(), str_http_request.length(), 0);
	//char fBuff[1024];
	//int buffsize = 1024; // 每个数据包存放文件的buffer大小  
	//int nStart;//记录post初始位置  
	//int nSize;//记录剩余文件大小  
	Sleep(0.2);
	//发送尾部
	//发送格式
	send(sHost, content.c_str(), content.length(), 0);
	Sleep(0.2);
	send(sHost, Spic_data, Spic_len, 0);
	Sleep(0.2);
	//如果数据是在够大，需要作调整，可以使用如下的方式，切割文件发送数据
	/*
	for (int i = 0; i < Spic_len; i += bufsize)
	{
	nStart = i;
	if (i + bufsize + 1> Spic_len){
	nSize = Spic_len - i;
	}
	else{
	nSize = bufsize;
	}

	memcpy(fBuff, Spic_data + nStart, nSize);
	::send(sHost, fBuff, nSize, 0);
	Sleep(0.2);   //防止毡包
	}*/

	send(sHost, strContent.c_str(), strContent.length(), 0);
	Sleep(0.2);
	
	char buf1[BUFSIZE2+1];
	int read_num;
	if( (read_num = recv( sHost , buf1 , sizeof(buf1) ,0) ) > 0  ){
		buf1[read_num] = '\0';
		PRINTF_LOG( "%s", buf1 );
	}
	if (Spic_data == NULL){
		PRINTF_LOG("file content is null\n");
	}
	//释放内存
	delete Spic_data;
}



//void socket_send1(const int fd , const char *path_ptr){
//	char header_buf[BUFSIZE+1], pic_buf[BUFSIZE+1] ,pic_headr_buf[BUFSIZE+1]  ;
//	char bound_buf[BOUND_LEN] , beg_bound[BOUND_LEN],end_bound[BOUND_LEN];
//	char pic_name[PIC_LEN];
//	char ret_buf[BUFSIZE+1];
//	int read_num;
//	unsigned long pic_len , sum_len;
//	FILE *fp;
//
//	/* open picture , if picture's size too big , we should loop to send , 
//	* so firstly we get picture"s size and send entire length to server,
//	* then we loop to send picture's data
//	*/
//	if(  ( fp =fopen(path_ptr , "rb") ) ==NULL )
//		oops( "fopen error:");
//
//	/* get picture's size	*/
//	fseek( fp , 0 , SEEK_END );
//	sum_len = pic_len = ftell(fp);
//
//	//pic_ptr = read_file(path_ptr , pic_len);
//	
//	/* init begin and end boundary */
//	snprintf(bound_buf , BOUND_LEN , "%s" , "71b23e4066ed");
//	snprintf(beg_bound , BOUND_LEN , "--%s"  ,bound_buf);
//	//sum_len += strlen(beg_bound);
//	snprintf(end_bound , BOUND_LEN , "--%s--\r\n" ,bound_buf);
//	sum_len += strlen(end_bound);
//	
//	//buf[read_num] = '\0';
//	//snprintf( header_buf , sizeof(header_buf) , "POST http://%s:%d%s HTTP/1.0\r\n" , SERVER_ADDR,
//	//		SERVER_PORT,PATH1);
//
//	/*init html header */
//	memset( (void *)header_buf , 0 , sizeof(header_buf));
//	snprintf( header_buf , BUFSIZE , "POST %s HTTP/1.1\r\n" , PATH1);
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Host:%s\r\n" , SERVER_ADDR  );  
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Accept:  */*\r\n");  
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Accept-Language: zh-CN \r\n");  
//	//snprintf(header_buf+strlen(header_buf),BUFSIZE,"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.7.6)\r\n");  
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Pragma: no-cache\r\n"); 
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Connection:Keep-Alive\r\n\r\n"); 
//	//snprintf(header_buf+strlen(header_buf),BUFSIZE,"Charset=utf-8\r\n\r\n"); 	//Charset=utf-8;
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Content-Type: multipart/form-data;boundary=%s\r\n" ,bound_buf);
//	sum_len += strlen(header_buf);
// 
//	///PRINTF_LOG( "header_buf :%s\n" , header_buf);
//
//	/*	init pic content  */  
//	snprintf(pic_headr_buf,BUFSIZE,"%s\r\n", beg_bound);  
//	get_pic_name( path_ptr , pic_name , '\\');
//	snprintf(pic_headr_buf+strlen(pic_headr_buf),BUFSIZE,
//		"Content-Disposition: form-data; name=\"file\"; filename=\"%s\"\r\n\r\n", pic_name);  
//	snprintf(pic_headr_buf+strlen(pic_headr_buf),BUFSIZE,"Content-Type: image/jpeg\r\n\r\n"); 
//	sum_len += strlen(pic_headr_buf);
//
//	//PRINTF_LOG( "pic_headr_buf :%s\n" , pic_headr_buf);
//	////cout << pic_headr_buf << endl;
//
//	/*  Content-Length */
//	snprintf(header_buf+strlen(header_buf),BUFSIZE,"Content-Length: %lu\r\n\r\n" , sum_len );
//	////PRINTF_LOG( "header_buf :%s\n" , header_buf);
//
//	/* send header */
//	send(fd, header_buf, strlen(header_buf), 0);
//	Sleep(0.2);
//
//	/* send pic_headr_buf */
//	send(fd, pic_headr_buf, strlen(pic_headr_buf), 0);
//	Sleep(0.2);
//
//	/* read pic data and send data to server */
//	fseek( fp , 0 , SEEK_SET);
//	memset( (void *)pic_buf , 0 , sizeof(pic_buf));
//	while ( !feof(fp)  ){
//		read_num  = fread( pic_buf ,sizeof(char),BUFSIZE ,fp );
//		send(fd, pic_buf, strlen(pic_buf), 0);
//		Sleep(0.2);
//	}
//
//	/* send data end boundary*/
//	send(fd, end_bound, strlen(end_bound), 0);
//	Sleep(0.2);
//
//	while( (read_num = recv( fd , ret_buf , sizeof(ret_buf) ,0) ) > 0  ){
//		ret_buf[read_num] = '\0';
//		PRINTF_LOG( "%s", ret_buf );
//	}
//	//}
//}