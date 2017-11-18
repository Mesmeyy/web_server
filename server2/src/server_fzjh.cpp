/*************************************************************************
	> File Name: server_fzjh.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月13日 星期一 19时56分54秒
 ************************************************************************/

#include<iostream>
#include<cstdlib>
#include<string>
#include "hong.h"
#include"server_fzjh.h"
#define Min_port 0
#define Max_port 65535
using namespace std;
using std::string;

ip_homes::ip_homes()//ip_homes的构造函数
{
    
    ip = "";//ip初始化
    port = Min_port + rand() % Max_port;//端口初始化为0-65535中间的一个随机值
    
    weight = Server_user_exhaust;//初始权重为0
    now_weight = weight;//初始当前权重为本身权重

    next = NULL;//next指针为空
    att = false;//初始服务器不可使用
    endpoint = NULL;//尾巴标志为NULL
}

ip_homes::~ip_homes()//ip_homes的析构函数
{
    ip = "";
    port = 0;
    weight = 0;
    now_weight = weight;
    /*delete next;
    next = NULL;
    */

    while(next){
        ip_homes* af= next -> next;
        delete next;
        next = af;
    }//循环释放数据

    next = NULL;
    att = false;
    endpoint = NULL;
}

bool ip_homes::set_ip(string& ip)//设置ip
{
    this ->  ip = ip;
    return true;
}
string ip_homes::get_ip() const//得到ip对应的ip
{
    return ip;
}

bool ip_homes::set_weight(int w)//设置权重
{
    weight = w;
    return true;
}
int ip_homes::get_weight() const//获取权重
{
    return weight;
}

bool ip_homes::set_nowweight(int& w)//设置当前的权重
{
    now_weight = w;
    return true;
}

bool ip_homes::set_des_weight()//权重自减
{
    now_weight --;
    return true;
}

int ip_homes::get_nowweight() const//获取当前的权重
{
    return now_weight;
}

bool ip_homes::set_attr(bool att)//设置服务器是否使用的标记
{
    this -> att = att;
    return true;
}
bool ip_homes::get_attr() const//获取服务器是否可以使用的状态
{
    return att;
}


/*
bool ip_homes::set_point((ip_homes*) p)
//设置指针尾巴
{
    
}
ip_homes* ip_homes::get_point() const
//获取指针尾巴
{
    
}
*/


fzjh::fzjh()//负载均衡类的构造函数
{
    //...
}
bool fzjh::addip(string& ip,unsigned int& point,int& weight)//添加监控ip
{
    //...
}
bool fzjh::delip(string& ip)//删除无效ip
{
    //...
}
bool fzjh::reset()//重置ip队列
{
    //...
}
ip_homes& fzjh::get_ip()//获取此次应该选择的ip_homes
{
    
}
ip_homes& fzjh::get_ok_ips()//获取当前可用的ip队列
{
    //...
}
ip_homes& fzjh::get_bad_ips()//获取当前不可用ip的队列
{
    //...
}
int fzjh::get_ip(string& ip)//获取对应ip的属性值
{
    //...
}

int fzjh::get_ip_number()//获取总ip数量
{
    //...
}

int fzjh::get_ok_ip_number()//获取可用ip数量
{
    //.,,
}

int fzjh::get_bad_ip_number()//获取不可用ip数量
{
    //...
}

bool fzjh::ip_weight_hash()
//进行ip到weight的哈希
{
    //...
}

bool fzjh::set_ip_attr(string& ip)//改变ip的属性
{
    //...
}


