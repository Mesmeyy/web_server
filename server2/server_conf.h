/*************************************************************************
	> File Name: server_conf.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月07日 星期六 15时49分30秒
 ************************************************************************/

#ifndef _SERVER_CONF_H
#define _SERVER_CONF_H
#include<hash_map>
#include<iostream>
#include "server_conf.h"
#define FZJH_SERVERS 64
#define ALL 10000 //优先使用这个服务器直到这个服务器挂机
#define CNU 0     //本轮负载均衡这个服务器资源已经使用完
#define DEFAULT 1 //默认执行R-R算法初始化服务器权值
#define BACK -1   //默认当前这个机器不可用除非其他服务起挂机
#define DIE -2    //服务器挂机状态,永远不允许使用
typedef int ARRTI;

using namespace std;
class server_conf
{

private:
    hash_map<struct sockaddr_in*,ARRTI> hmall;//所有服务器
    hash_map<struct sockaddr_in*,ARRTI> hming;//在运行服务器
public:
    server_conf();//构造函数
    int add_server(struct sockaddr_in*,ARRTI);//添加代理服务器
    void del_server();//删除代理服务器
    int default_server();//恢复下一轮循环
};
int  get_agency_conf(server_conf**  sc,char* a);//去实例代理服务器的数据结构
int read_conf(server_conf**,char *a);//读取指定配置文件
#endif
