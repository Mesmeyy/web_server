/*************************************************************************
	> File Name: http_conn.h
	> Author: zhuziyu
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月09日 星期三 10时14分22秒
 ************************************************************************/
#ifndef _HTTP_CONN_H
#define _HTTP_CONN_H
#include<iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include"server_epoll.h"
using namespace std;
class http_conn
{
public:
    static const int FILENAME_LEN = 200;                              /*文件名的最大长度*/
    static const int READ_BUFFER_SIZE = 2048;                         /*读缓冲区的大小*/
    static const int WRITE_BUFFER_SIZE = 2048;                        /*写缓冲区的大小*/
    enum METHOD                                                       /*HTTP请求方法*/
    {    GET = 0,POST,HEAD,PUT,DELETE,TRACE,OPTIONS,CONNECT,PATCH};
    enum CHECK_STATE {                                                /*解析客户请求时，主状态机所处的状态*/ 
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONNECT};
    enum HTTP_CODE                                                    /*服务器处理HTTP请求的可能结果*/
    {NO_REQUEST,GET_REQUEST,BAD_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST,
     FILE_REQUEST,INTERNAL_ERROR,CLOSED_CONNECTION};
    enum LINE_STATUS                                                  /*行的读取状态*/
    {LINE_OK = 0,LINE_BAD,LINE_OPEN};
    //static epoll_class *m_epoll;                                              /*子进程注册事件表*/
    //static int m_usr_count;                                           /*客户总量*/
public:
//private:
    int m_linger;                                                     /*HTTP请求是否要求保持连接*/
    char* m_url;                                                      /*客户请求的目标的文件名*/
    char* m_version;                                                  /*HTTP协议版本号*/
    int m_content_length;                                           /*HTTP请求的消息体的长度*/
    char* m_host;                                                     /*主机名*/
    int m_start_line;                                                 /*当前正在解析的行的起始位置*/
    int m_checked_idx;                                                /*当前正在分析的字符在读缓冲区中的位置*/
    int m_read_idx;                                                   /*标识读缓冲区中已读入的客户数据的最后一个字节的下一个位置*/
    int m_write_idx;                                                  /*写缓冲区中待发送的字节数*/
    char m_read_buf[1024];                                                 /*读缓冲区*/
    char m_write_buf[1024];                                                /*写缓冲区*/
    char m_read_file[1024];                                                /*客户请求的目标文件的完整路径*/
    CHECK_STATE m_check_state;//主机状态
    METHOD m_method;//请求方法(GET)
    int m_sockfd;//客户端socket
    sockaddr_in m_address;//客户端要连接的目标地址
    char* m_URL;//保存目标URL
    char m_real_file[FILENAME_LEN];//目标文件名
    char* m_file_address;//客户请求的目标文件被映射到内存的起始位置
    struct stat m_file_stat;//目标文件的状态
    struct iovec m_iv[2];
    int m_iv_count;

  //  private:
    public:
    void init(int s);//初始化
    LINE_STATUS parse_line();//检测一行是否完整
    int read();//服务器接收数据
    int m_epollfd;///////////////

    public:
    void init(epoll_class &epolltemp,int sockfd,const struct sockaddr_in& addr);//初始化
    HTTP_CODE parse_request_line(char*text);//解析请求行
    HTTP_CODE parse_headers(char*text);//解析头部
    HTTP_CODE process_read();//主状态机
    char * get_line()//读取一行
    {
        return m_read_buf+m_start_line;
    }
    HTTP_CODE parse_content(char* text);//处理消息体 
    HTTP_CODE do_request();//解析出目标文件的绝对路径
    void unmap();//释放映射的内存
    bool write();//---------------------
    bool add_response(const char* format,...);
    bool add_statues_line(int status,const char* title);
    bool add_content_length(int content_len);
    bool add_headers(int content_len);
    bool add_linger();
    bool add_blank_line();
    bool add_content(const char* content);
    bool process_write(HTTP_CODE ret);
    void process();
    void removefd(int epollfd,int fd);
    void close_conn();

};

#endif
