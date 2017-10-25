/*************************************************************************
	> File Name: server_base.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 19时48分08秒
 ************************************************************************/

#include<iostream>
#include<string>
#include"server_base.h"

using namespace std;
using std::string;
server_base::server_base()
{
    user = "";
    listenip = "";
    listenport = -1;
    workerprocess = 1;
    server_name = "";
    rootpath = "";
    agent = 0;
}

server_base::~server_base(){}
