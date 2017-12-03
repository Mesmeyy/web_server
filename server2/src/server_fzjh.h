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
#include<vector>
#include<list>
#include"hong.h"
#define Bucket_size   128
#define Bucket_number 255
#define Ip_number    255
//#include <ptypes.h>

using namespace std;
class bser
{
public:
    string ip;
    unsigned int port;
    //bser的构造函数1
    bser(){
        ip = "";
        port=(rand()%101);//0-100随机整数
    }
    //bser的构造函数2
    bser(string& ip,unsigned int& port){
        this -> ip = ip;
        this -> port = port;
    }
};
class ip_homes
{
private:
    //string ip;//存放ip
    //unsigned int port;
    bser bs;
    int weight;//也算是性质标志
    int now_weight;
    bool att ;//当前服务器是否可用 ,false不可用 true可用
    //int hashnum; 
public:
    void * endpoint;//用作结尾标识符,为以后的hash作,这个先保留
    int hashnum;
    //unsigned int inde ;//这个元素在vector中的位置
public:
    //ip_homes的构造函数1
    ip_homes(){
        bser *bsp = new bser;
        bs = *bsp;
        weight = Server_user_exhaust;//初始权重为0
        now_weight = weight;//初始当前权重是本身权重
        att = false;
        endpoint = NULL;
        hashnum = 0;
    }
    //ip_homes的构造函数2
    ip_homes(string& ip,unsigned int& port,int& weight){
        bser *bsp = new bser(ip,port);
        bs = *bsp;
        this -> weight = weight;
        now_weight = weight;
        att = false;
        endpoint = NULL;
        hashnum = 0;
    }
    ~ip_homes();//ip_homes的析构函数
    
    bool set_bser(string& ip,unsigned int& port);//设置bser

    bser get_bser() const ;//获取bser

    //设置ip和端口 
    bool set_ip_port(string& ip,unsigned int& port){
        bs.ip = ip;
        bs.port = port;
    }
    string get_ip() const;//获取ip
    unsigned int get_port() const;//获取端口
    
    bool set_weight(int w);//设置权重
    int get_weight() const;//获取权重
    
    bool set_nowweight(int w);//设置当前权重
    bool set_deduct_nowweight();//权重减少一个表示它被选中且被使用
    int get_nowweight() const;//获取当前权重
    
    bool set_attr(bool att);//设置服务器是否使用的标记
    bool get_attr() const;//获取服务器的是否可以使用的状态
 
};


class fzjh
{
public:
    vector<ip_homes > home_use;//当前服役的服务器头头
    //ip_homes* home_use_end;//当前服役的服务器尾巴
    
    ip_homes *home_now;//当前该选它作为负载均衡的服务器
    
    vector<ip_homes > home_lazy;//目前gg服务器
    list<ip_homes> * buckets;//为以后ip hash用,如果这是引用的话，当weight，nowweight发生改变，ip_weight的hash中也会发生改变
    
    const int buckets_default_size = Bucket_size;//一个桶的大小撑死128,里面可以有上限的ip_homes个数
    const int max_buckets_number = Bucket_number;//桶的个数最多是255个
    const int max_ip_number = Ip_number;//负载均衡管理的ip最多是255个
    
    int ip_number;//ip数量
    int home_use_number;
    int home_lazy_number;
    int turn;//代表上次被选择的iphome所在home_use的位置
    bool over;//是否轮寻完一圈,true轮寻完一圈，要重置了,false没有轮寻完一圈
public:
    fzjh();//构造函数
    bool addip(string& ip,unsigned int& port,int& weight);//添加新的ip,需要解析好ip和端口和权重
    bool delip(ip_homes& ipho);//删除旧的ip
    bool reset();//重置ip队列,即让当前权重为属性权重
    
    ip_homes& get_turn_ip(unsigned int turn);//获取负载均衡的ip  
    bool get_ok_ips();//列出当前的可用ip和属性们
    bool get_bad_ips();//列出当前不可用的ip和属性们
    
    int get_ok_ip_number()const;//获取可用ip数量
    int get_ip_number() const;//获取总ip数量
    int get_bad_ip_number() const;//获取不可用ip数量
    
    bool ip_weight_hash();//进行ip到weight的哈希

    bool set_hash_weight(ip_homes& ipho,int& weight);
    int get_hash_weight(ip_homes& ipho);
    
    bool set_hash_nowweight(ip_homes& ipho,int& nowweight);
    int get_hash_nowweight(ip_homes& ipho);
    
    bool set_hash_deduct_nowweight(ip_homes& ipho);//对指定的ip的nowweight进行自减操作
    /*为什么要用hash，假设服务器必须要往一个有特定功能的服务器上发送请求，也就是服务器维护者很多目标服务器的状态，ip_weight _hash能告诉服务器当前目标的那个特定功能的服务器可不可能接受你这个请求，这个目标服务器是否处于压力状态。那么我需要的是根据ip和端口找当前权重的。*/

};
vector<int> divide_ip(ip_homes& ipho);
bool combine_ip_homes(ip_homes& a,ip_homes& b);
#endif
