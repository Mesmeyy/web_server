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

ip_homes::ip_homes()//ip_homes的构造函数
{
    
}

ip_homes::~ip_homes()//ip_homes的析构函数
{
    
}

bool ip_homes::set_ip(string ip)//设置ip
{
    
}
string& ip_homes::get_ip() const
//得到ip对应的ip
{
    
}

bool ip_homes::set_weight();//设置权重
{
    
}
int ip_homes::get_weight() const
//获取权重
{
    
}

bool ip_homes::set_nowweight(int w)
//设置当前的权重
{
    
}

bool ip_homes::set_des_weight()
//权重自减
{
    
}

int get_nowweight() const
//获取当前的权重
{
    
}

bool set_attr(book att)
//设置服务器是否使用的标记
{
    
}
bool get_attr() const
//获取服务器是否可以使用的状态
{
    
}

bool set_point(ip_homes* p)
//设置指针尾巴
{
    
}
ip_homes* get_point() const
//获取指针尾巴
{
    
}
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
(ip_homes) fzjh::get_ip()//获取此次应该选择的ip_homes
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


