#ifndef _TCP_TRANS_INFO_H_
#define _TCP_TRANS_INFO_H_

// #include <string>
// using namespace  std;


//void socket_send(const int fd , struct sockaddr_in *pserv_addr);
void socket_send(const int fd , const char *path_ptr);

char * read_file(const char *pathpic, int &pic_len);
void sendPic(const int sHost ,char* addr, char * host, char *pathpic) ;

#endif