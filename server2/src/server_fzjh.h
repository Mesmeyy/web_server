/*************************************************************************
	> File Name: server_fzjh.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月13日 星期一 19时56分37秒
 ************************************************************************/

#ifndef _SERVER_FZJH_H
#define _SERVER_FZJH_H
#include <string>
#include <iostream>
#define Bucket_size   128
//#include <ptypes.h>
using namespace std;
class ip_homes
{
private:
    string ip;//存放ip
    unsigned int port;
    int weight;//也算是性质标志
    int now_weight;
    ip_homes *next;
    bool att ;//当前服务器是否可用 ,0不可用1可用
public:
    void * endpoint;//用作结尾标识符,为以后的hash作
public:
    ip_homes();//ip_homes的构造函数
    ~ip_homes();//ip_homes的析构函数
    
    bool set_ip(string& ip);//设置ip
    string& get_ip() const;//获取ip
    
    bool set_weight();//设置权重
    int get_weight() const;//获取权重
    
    bool set_nowweight(int& w);//设置当前权重
    bool set_des_weight();//权重-1表示使用
    int get_nowweight() const;//获取当前权重
    
    bool set_attr(bool att);//设置服务器是否使用的标记
    bool get_attr() const;//获取服务器的是否可以使用的状态
 
    //bool set_point((ip_homes*) p);//设置尾巴指针
    //ip_homes* get_point() const;//获取尾巴指针
};

class fzjh
{
private:
    ip_homes *home_use;//当前服役的服务器头头
    ip_homes *home_use_end;//当前服役的服务器尾巴
    ip_homes *home_now;//当前该选它作为负载均衡的服务器
    ip_homes *homw_lazy;//目前gg服务器
    ip_homes **buckets;//为以后ip hash用
    const int buckets_default_size = Bucket_size;//一个桶的大小撑死128,里面可以有上限的ip_homes个数
    int ip_number;//ip数量
    int ip_ok_number;
    int ip_bad_number;
    int over;//是否轮寻完一圈,1轮寻完一圈，要重置了,0没有轮寻完一圈
public:
    fzjh();//构造函数
    bool addip(string& ip,unsigned int& port,int& weight);//添加新的ip,需要解析好ip和端口和权重
    bool delip(string& ip);//删除旧的ip
    bool reset();//重置ip队列
    
    ip_homes& get_ip();//获取负载均衡的ip  
    ip_homes& get_ok_ips();//列出当前的可用ip和属性们
    ip_homes& get_bad_ips();//列出当前不可用的ip和属性们
    
    int get_ip(string& ip);//获取所需要的ip属性
    int get_ok_ip_number();//获取可用ip数量
    int get_ip_number();//获取总ip数量
    int get_bad_ip_number();//获取不可用ip数量
    
    bool ip_weight_hash();//进行ip到weight的哈希
    bool set_ip_attr(string& ip);//本质上用hash比较好
};
#endif
