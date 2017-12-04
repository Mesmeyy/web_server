/*************************************************************************
	> File Name: server_fzjh.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月13日 星期一 19时56分54秒
 ************************************************************************/

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<vector>
#include<utility>
#include<list>
#include "hong.h"
#include"server_fzjh.h"
#define Min_port 0
#define Max_port 65535
using namespace std;
using std::string;

bool combine_ip_homes(ip_homes& a,ip_homes& b);
ip_homes::~ip_homes()//ip_homes的析构函数
{
    bs.ip = "";
    bs.port = 0;
    weight = 0;
    now_weight = weight;
    att = false;
    endpoint = NULL;
}

bool ip_homes::set_bser(string& ip,unsigned int& port)
{
    bs.ip = ip;
    bs.port = port;
}
bser ip_homes::get_bser() const
{
    return bs;
}
string ip_homes::get_ip() const//得到ip对应的ip
{
    return bs.ip;
}
unsigned int ip_homes::get_port() const //得到对应端口
{
    return bs.port;
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

bool ip_homes::set_nowweight(int w)//设置当前的权重
{
    now_weight = w;
    return true;
}

bool ip_homes::set_deduct_nowweight()//权重自减
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


fzjh::fzjh()//负载均衡类的构造函数
{
    home_now = NULL;
    ip_number = 0;
    home_use_number = 0;
    home_lazy_number = 0;
    turn = 0;
    over = true;//当前轮寻完一圈，每次开始找ip之前都要检查这个over是否满足条件
    buckets = new list<ip_homes>[Bucket_number];
}

//对于vector中有冲突的ip_homes进行合并
bool combine_ip_homes(ip_homes& a,ip_homes& b)
{
    //cout << "a:weight = " << a.get_weight() << "  b.weight = " << b.get_weight()<<endl;
    /*
    *使用规则:在home_use中，根据数字的意义,
    *出现一方是-2，则给服务器属性权重定义为-2,-2代表不可用直到别的都坏掉
    *出现一方是-1，则属性权重一律是-1,-1代表一直用它                                        
    * 出现是0,1,>1的整数的时候，则ab对应的属性权重相加得最终的属性权重
    * */
    pair<ip_homes&,ip_homes&> i(a,b);
    if((i.first.get_weight() == Server_last) || (i.second.get_weight() == Server_last)) {
        i.first.set_weight(Server_last);
        return true;
    }
    if((i.first.get_weight() == Server_always) || (i.second.get_weight() == Server_always)){
        i.first.set_weight(Server_always);
        return true;
    }
    i.first.set_weight(i.first.get_weight() + i.second.get_weight());
    i.first.set_nowweight(i.first.get_nowweight() + i.second.get_nowweight());
    return true;
}

bool fzjh::addip(string& ip,unsigned int& port,int& weight)//添加监控ip
{
    if(home_use_number + home_lazy_number >= max_ip_number){
        //不能再添加负载均衡的ip
        return false;
    }
    ip_homes ipho(ip,port,weight);
    //vector<ip_homes>::size_type in = 0;
    vector<ip_homes>::iterator it = home_lazy.begin();
    for(;it != home_lazy.end();it++){
        if((it -> get_ip() == ipho.get_ip()) && (it ->get_port() ==ipho.get_port())){
            //原来ip在lazy里面，本身就是不能用的，因此本次的ip_homes 无影响，不填加，本次添加程序作废
            //combine_ip_homes(home_lazy[in],ipho);
            return true;
        }
    }
    it = home_use.begin();
    for(;it != home_use.end();it++){
        if((it -> get_ip() == ipho.get_ip()) && (it -> get_port() == ipho.get_port())){
            //代表新添加的ip和可用的ip有重复情况
            combine_ip_homes(*it,ipho);
            return true;
        }
    }
    //如果这个ipho是新的
    ip_number++;
    
    //开始检查ipho的weight，看放入home_lazy还是home_use。
    if((ipho.get_weight() == Server_use_bad) || (ipho.get_weight() == Server_bad_attr)){
        home_lazy.push_back(ipho);
        home_lazy_number++;
    }else {
        home_use.push_back(ipho);
        home_use_number++;
    }
    //添加成功
    return true;
}

//删除无效ip_homes
bool fzjh::delip(ip_homes& ipho)
{
    /*由于home_lazy的存在，是属性Server_bad_attr或者Server_use_bad,不可能通过程序把home_lazy里面的ip_homes转移到home_use里面，因此只能是home_use里面的服务器出错，被程序移动到home_lazy里面，即不使用这个ip_homes,被删除的ip_home到了home_lazy中*/
    vector<ip_homes>::iterator it = home_use.begin();
    for(;it != home_use.end();){
        if((it -> get_ip() == ipho.get_ip())&&(it -> get_port() == ipho.get_port())){
            ip_homes temp_ip_home(*it);
            it = home_use.erase(it);
            home_use_number--;

            home_lazy.push_back(temp_ip_home);
            home_lazy_number ++;
            break;
        }else ++it;
    }
    //***********************************是要把hash中的ip_home也改变属性的
    //要想删除ipho，那这个肯定是被用坏的 
    int tempweight = Server_use_bad;
    set_hash_weight(ipho,tempweight);
   
}
bool fzjh::reset()//重置ip队列
{
    vector<ip_homes>::size_type in = 0;
    for(;in != home_use_number;in++){
        int weight = home_use[in].get_weight();
        home_use[in].set_nowweight(weight);
    }

}

//获取此次应该选择的ip_homes
ip_homes& fzjh::get_turn_ip(unsigned int turn)
{

    if(over) {turn = 0;reset();}//新开始一轮
    
    vector<ip_homes>::size_type in = 0;
    cout << "server_fzjh.cpp:202 get_turn_ip中home_use有"<<endl;
    for(;in < home_use_number;in++){
        cout << home_use[in].get_ip() << " : " << home_use[in].get_port() << " -> " << home_use[in].get_nowweight()<< endl;
    }
    
    for(vector<ip_homes>::size_type in = 0;in < home_use_number;in++){
        if(home_use[in].get_nowweight() == Server_always){
            turn = in;
            if(turn == home_use_number - 1) over = true;
            else over = false;
            return home_use[in];
        }
    }

    if(home_use[turn].get_nowweight() == Server_last){
        //Server_last的情况下查看是否仅有这么一个服务器，有别的的话，优先考虑别的
        if(turn == home_use_number - 1) over = true;
        else over = false;
        if(home_use_number == 1) return home_use[turn];
        //else return get_turn_ip((++turn)% home_use_number);
        else{
            turn = (turn + 1) % home_use_number;
            return get_turn_ip(turn);
        }
    }
    if(home_use[turn].get_nowweight()){
        //1,2,3,...的权重情况
        home_use[turn].set_deduct_nowweight();
        if(turn == home_use_number - 1) over = true;
        else over = false;
        //hash buckets中的nowweight不用更改，因为buckets中nowweight没有意义。你想，我要是有ip_weight，肯定是用户指定了ip，这样就不涉及轮寻，而nowweight是为了轮寻设计的
        //***************************8要改变ip_hash中对应的ip_home
        //set_hash_deduct_nowweight(home_use[turn]);
        ip_homes &temp = (home_use[turn]);
        if(!home_use[turn].get_nowweight()) turn = (++turn) % home_use_number;
        cout << "server_fzjh.cpp:236 本次选择的是:"<<temp.get_ip() << ":" << temp.get_port() <<endl;
        return temp;
    }
    else{
        //权重是0的情况
        if(turn == home_use_number - 1) over = true;
        else over = false;
        turn = (turn + 1) % home_use_number;
        return get_turn_ip(turn);
    }
}
bool fzjh::get_ok_ips()//获取当前可用的ip队列
{
    cout << "ip          " << " port" << endl;
    vector<ip_homes>::size_type in = 0;
    for(;in < home_use_number;in++){
        cout << home_use[in].get_ip() << "    " << home_use[in].get_port()<< endl;
    }
    return true;
}
bool fzjh::get_bad_ips()//获取当前不可用ip的队列
{
    cout << "ip          " << " port" << endl;
    vector<ip_homes>::size_type in = 0;
    for(;in < home_lazy_number;in++){
        cout << home_lazy[in].get_ip() << "    " << home_lazy[in].get_port() << endl;
    }
    return true;
}
int fzjh::get_ip_number() const//获取总ip数量
{
    return ip_number;
}

int fzjh::get_ok_ip_number() const//获取可用ip数量
{
    return home_use_number;
}

int fzjh::get_bad_ip_number() const //获取不可用ip数量
{
    return home_lazy_number;
}
//ip点十进制拆解
vector<int> divide_ip(ip_homes& ipho)
{
    vector<int> ret;
    int vector_index = 0;
    
    char ip_bot_ten[4];//实际上3位够了，但是为了保证最后一个是\0就用了4个
    int bot_index = 0;//数组ip_bot_ten的下标
    for(auto temp : ipho.get_ip()){
        if(temp != '.'){
           ip_bot_ten[bot_index++] = temp;
        }
        else{
            int ten = atoi(ip_bot_ten);
            //cout << "server_fzjh.cpp:293 ten = " << ten << endl;
            ret.push_back(ten);
            vector_index++;
            memset(ip_bot_ten,0,sizeof(ip_bot_ten));
            bot_index = 0;
        }    
    }
    ret.push_back(atoi(ip_bot_ten));
    vector_index++;
    return ret;
}

bool fzjh::ip_weight_hash()
//进行ip到weight的哈希
{
    int hash_start = 89;//哈希的一个初始值，随便给的一个质数
    
    //进行home_use数据的哈希，不考虑局域网
    for(vector<ip_homes>::size_type in = 0;in != home_use_number;in++){
        int hash = hash_start;
        ip_homes &temp = (home_use[in]);
        vector<int> ip_bots(divide_ip(temp));//得到了一个ip_home的点十进制
       int i = 0;
        for(i = 0;i < 3;i++){
            hash = (hash*13 + ip_bots[i] + temp.get_port()) % Bucket_number;//这是范式写法，性能不一定好
        }
        cout << "hash_number = " << hash<<endl;
        //现在已经得到一个ip_home的hash值了
        temp.hashnum = hash; 
        buckets[hash].push_back(temp);
    }

    //进行home_lazy数据的哈希，之所以要进行这一部分的哈希，是因为怕用户请求这部分的ip_homes,然后可以及时查到now——weight,然后给用户返回一个错误标志.
    for(vector<ip_homes>::size_type in = 0;in != home_lazy_number;in++){
        int hash = hash_start;
        ip_homes &temp = (home_lazy[in]);
        vector<int> ip_bots(divide_ip(temp));//得到了一个ip_home的点十进制
        int i = 0;
        for(i = 0;i < 3;i++){
            hash = (hash*13 + ip_bots[i] + temp.get_port()) % Bucket_number;//这是范式写法，性能不一定好
        }
        //现在已经得到一个ip_home的hash值了
        temp.hashnum = hash;
        cout << "hash_number = " << hash << endl;
        buckets[hash].push_back(temp);
    }
    return true;
}
/*当用户请求有明确的服务器或者反响代理有明确的服务器的时候，就要根据ip和端口找到对应的服务器的weight和now——weight，这时候就需要哈希和设定now——weight或者get_now——weight*/
bool fzjh::set_hash_weight(ip_homes& tempiphome,int& weight)
{
    tempiphome.set_weight(weight);
    vector<int> t(divide_ip(tempiphome));
    
    int hash  = 89;
    int i = 0;
    
    for(;i < 3;i++){
        hash = (hash*13 + t[i] + tempiphome.get_port())%Bucket_number;
    }

    int index = hash;
    list<ip_homes>::iterator it = buckets[index].begin();
    for(;it != buckets[index].end();it++){
        if(it -> get_ip() == tempiphome.get_ip()){
            if(it -> get_port() == tempiphome.get_port()){
                it -> set_weight(weight);
                //注意这里的更改不会造成home*中对应ip_homes的weight更改
                //这个函数设计之初是一是为了让delip函数调用，加入有用户指定这个ip,经过connect之后这个ip的weight坏掉了，那么后来的负载均衡客户在home_use中使用，也会被目标服务器挂掉，由此以来，home_use和buckets就同步了
                return true;
            }else continue;
        }
    }
    return false;
}


int fzjh::get_hash_weight(ip_homes& tempiphome)
{
   
    vector<int> t(divide_ip(tempiphome));
    
    int hash  = 89;
    int i = 0;
    
    for(;i < 3;i++){
        hash = (hash*13 + t[i] + tempiphome.get_port())%Bucket_number;
    }

    int index = hash;
    list<ip_homes>::iterator it = buckets[index].begin();
    for(;it != buckets[index].end();it++){
        if(it -> get_ip() == tempiphome.get_ip()){
            if(it -> get_port() == tempiphome.get_port()){
                return it -> get_weight();
            }else continue;
        }
    }
}

bool fzjh::set_hash_nowweight(ip_homes& tempiphome,int& now_weight)
{
    tempiphome.set_nowweight(now_weight);
    vector<int> t(divide_ip(tempiphome));
    
    int hash  = 89;
    int i = 0;
    
    for(;i < 3;i++){
        hash = (hash*13 + t[i] + tempiphome.get_port())%Bucket_number;
    }

    int index = hash;
    list<ip_homes>::iterator it = buckets[index].begin();
    for(;it != buckets[index].end();it++){
        if(it -> get_ip() == tempiphome.get_ip()){
            if(it -> get_port() == tempiphome.get_port()){
                it -> set_nowweight(now_weight);
                //注意这里的更改不会造成home*中ip_homes的nowweight的更改
                return true;
            }else continue;
        }
    }
    return false;
}

int fzjh::get_hash_nowweight(ip_homes& tempiphome)
{
    
    vector<int> t(divide_ip(tempiphome));
    
    int hash  = 89;
    int i = 0;
    
    for(;i < 3;i++){
        hash = (hash*13 + t[i] + tempiphome.get_port())%Bucket_number;
    }

    int index = hash;
    list<ip_homes>::iterator it = buckets[index].begin();
    for(;it != buckets[index].end();it++){
        if(it -> get_ip() == tempiphome.get_ip()){
            if(it -> get_port() == tempiphome.get_port()){
                return it -> get_nowweight();
            }else continue;
        }
    }
}

bool fzjh::set_hash_deduct_nowweight(ip_homes& tempiphome)
{
    vector<int> t(divide_ip(tempiphome));
    
    int hash  = 89;
    int i = 0;
    
    for(;i < 3;i++){
        hash = (hash*13 + t[i] + tempiphome.get_port())%Bucket_number;
    }

    int index = hash;
    list<ip_homes>::iterator it = buckets[index].begin();
    for(;it != buckets[index].end();it++){
        if(it -> get_ip() == tempiphome.get_ip()){
            if(it -> get_port() == tempiphome.get_port()){
                it -> set_deduct_nowweight();
                for(vector<ip_homes>::size_type in = 0;in != home_use_number;in++){
                    if((home_use[in].get_ip() == tempiphome.get_ip()) && (home_use[in].get_port() == tempiphome.get_port())){
                        home_use[in].set_deduct_nowweight();
                    }
                }
               return true;
            }else continue;
        }
    }
    return false;
}







