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
using namespace std;

class server_base
{
public:
    string user;
    string listenip;
    int listenport;
    int workerprocess;
    string rootpath;
    int agent;

public:
    server_base();
    ~server_base();
};

#endif
