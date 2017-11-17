/*************************************************************************
	> File Name: server_base.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 19时39分49秒
 ************************************************************************/

#ifndef _SERVER_BASE_H
#define _SERVER_BASE_H
/*这是有关服务器的基础信息*/
#include <string>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include"server_fzjh.h"
using namespace std;

class server_base
{
public:
    string user;
    string listenip;
    string server_name;
    int listenport;
    int workerprocess;
    string rootpath;
    string userrootpath;
    int agent;
    string confname;
    fzjh servers;//负责负载均衡的总类
   
    struct sockaddr_in address;
    int sockfd;
public:
    server_base();
    ~server_base();
    int start_server_base();
};

#endif
