/*************************************************************************
	> File Name: main.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时11分35秒
 ************************************************************************/

#include<iostream>
#include"server_base.h"
#include"parajudge.h"
#include"hong.h"
#include<unistd.h>
#include"server_fzjh.h"

using namespace std;
int main(int argc,char *argv[])
{
    server_base bs;
    int ret_para;
    switch(argc){
        case 1:
        case 2:
        case 3:ret_para = parajudge(argc,argv,bs);break;
        default:help();return 0;
    }
    switch(ret_para){
        case -1:help();return 0;
        case Default_start: break;
        case Help:return 0;
        case Version:return 0;
        case Conf:return 0;
        case Stop:return 0;
        case Conf_change:break;
    }
    if(bs.start_server_base()){
        cout << "启动基础服务器成功"<< endl;
    }else{
        cout << "服务器基础启动失败,程序中断..."<<endl;
        return 0;
    }

    /*测试add_ip和delip的代码
    cout << "main.cpp: 42当前可用的ip_homes"<<endl;
    vector<ip_homes>::size_type in = 0;
    for(;in < bs.servers.home_use_number;in++){
        cout << bs.servers.home_use[in].get_ip() << " : " << bs.servers.home_use[in].get_port() << " -> " << bs.servers.home_use[in].get_weight() << endl;
    }
    */
    
    
    /*测试get_turn_ip()的代码
    while(1){
        sleep(3);
        cout << "50"<<endl;
        ip_homes t(bs.servers.get_turn_ip(bs.servers.turn));
        cout << t.get_ip() <<":"<< t.get_port()<<"本轮还剩 "<< t.get_nowweight()<< " 次运行机会"<<endl; 
    }
    */

    /*测试divide_ip函数
    vector<int> t = divide_ip(bs.servers.home_use[0]);
    cout << t[0] << " " << t[1] << " " << t[2] << " " << t[3] << endl;
    */

    /*测试ip_weight_hash函数
     *
    bs.servers.ip_weight_hash();
    */
    /*测试set_hash_weight和get_hash_weight
    int a = 5;
    int &weight = a;
    ip_homes &t = bs.servers.home_use[0];
    bs.servers.set_hash_weight(t,weight);
    cout << t.get_weight() << endl;
    cout << bs.servers.get_hash_weight(t) << endl;
    */
    
    /*测试set_hash_nowweight和get_hash_nowweight
    int a = 5;
    int &weight = a;
    ip_homes& t = bs.servers.home_use[0];
    bs.servers.set_hash_nowweight(t,weight);
    cout << t.get_nowweight() << endl;
    cout <<bs.servers.get_hash_nowweight(t) << endl;
    */
    
    /*测试set_hash_deduct_nowweight函数
    ip_homes &t = bs.servers.home_use[0];
    bs.servers.set_hash_deduct_nowweight(t);
    cout << t.get_nowweight()<<endl;
    cout << bs.servers.home_use[0].get_nowweight() << endl;
    */
    
    if(bs.servers.ip_weight_hash()){
        cout << "服务器启动ip_weight哈希成功"<<endl;
    }else{
        cout << "服务器启动ip_weight哈希失败，程序中段..."<<endl;
        return 0;
    }

}
