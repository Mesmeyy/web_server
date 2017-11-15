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
#include <ptypes.h>
using namespace std;
class ip_homes
{
public:
    string ip;//存放ip
    int weight;//也算是性质标志
    int now_weight;
    ip_home *next;
    bool att ;//当前服务器是否可用 ,0不可用1可用
    void * endpoint;//用作结尾标识符,为以后的hash作
};

class fzjh
{
private:
    ip_homes *home_use;//当前服役的服务器头头
    ip_homes *home_use_end;//当前服役的服务器尾巴
    ip_homes *home_now;//当前该选它作为负载均衡的服务器
    ip_homes *homw_lazy;//目前gg服务器
    ip_homes **buckets;//为以后ip hash用
    int ip_number;//ip数量
    int ip_ok_number;
    int ip_bad_number;
    int over;//是否轮寻完一圈,1轮寻完一圈，要重置了,0没有轮寻完一圈
public:
    fzjh();//构造函数
    bool addip(string ip,int weight);//添加新的ip
    bool delip(string ip);//删除旧的ip
    bool reset();//重置ip队列
    (ip_homes) get_ip();//获取负载均衡的ip  
    (ip_homes) get_ok_ips();//列出当前的可用ip和属性们
    (ip_homes) get_bad_ips();//列出当前不可用的ip和属性们
    int get_ip(string ip);//获取所需要的ip属性
    int get_ok_ip_number();//获取可用ip数量
    int get_ip_number();//获取总ip数量
    int get_bad_ip_number();//获取不可用ip数量
    bool ip_weight_hash();//进行ip到weight的哈希
    bool set_ip_attr(string ip);//本质上用hash比较好
};
#endif
