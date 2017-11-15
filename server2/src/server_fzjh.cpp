/*************************************************************************
	> File Name: server_fzjh.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月13日 星期一 19时56分54秒
 ************************************************************************/

#include<iostream>
#include<string>
#include"server_fzjh.h"
using namespace std;
using std::string;


fzjhs::fzjh()//负载均衡类的构造函数
{
    //...
}
bool fzjh::addip(string ip,int weight)//添加监控ip
{
    //...
}
bool fzjh::delip(string ip)//删除无效ip
{
    //...
}
bool fzjh::reset()//重置ip队列
{
    //...
}
(ip_homes) fzjh::get_ip()//获取此次应该选择的ip_home
{
    
}
(ip_homes) fzjh::get_ok_ips()//获取当前可用的ip队列
{
    //...
}
(ip_homes) fzjh::get_bad_ips()//获取当前不可用ip的队列
{
    //...
}
int fzjh::get_ip(string ip)//获取对应ip的属性值
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

bool fzjh::set_ip_attr(string ip)//改变ip的属性
{
    //...
}


