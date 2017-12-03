/*************************************************************************
	> File Name: parajudge.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时22分37秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
#include<regex>
#include<cstring>
#include<fstream>
#include"hong.h"
#include"server_base.h"
#include"parajudge.h"
using namespace std;
string default_conf;
int parajudge(int a,char*b[],server_base& bs)
{
    default_conf = "server.conf";
    //cout << "parajudge.cpp 21: a = " << a<< endl;
    if(a == 1){
        if(make_server_base(bs,default_conf) == 1)return Default_start;
        else return -1;
    }
    if(a == 2){
        string V(b[1]);
        string S(b[1]);
        string H(b[1]);
        string W(b[1]);
        if(V == "-v") {
            version();
            return Version;
        }
        else if(S == "-s") {
            return Stop;
        }
        else if(W == "-?"){
            help();
            return Help;
        }
        else if(H == "-h"){
            help();
            return Help;
        }
        else{
            return -1;
        }
    }
    if(a == 3){
        string T(b[1]);
        string C(b[1]);
        if(T == "-t"){
            if(test_conf(b[2]) == 1) return Conf;
            else return -1;
        }
        else if(C == "-c"){
            cout << "新的配置文件"<< endl;
            if(make_server_base(bs,b[2]) == 1)return Conf_change;
            else return -1;
        }
    }
}

int make_server_base(server_base& bs,string filename)
{
    string the_end_filename("../conf/");//这是默认路径
    char read_buf[64];
    ifstream ifm;
    int flag = 1;//是监测还是真正的执行

    bs.confname = filename;
    unsigned int buflen = sizeof(read_buf);
    the_end_filename += filename;

    ifm.open(the_end_filename.c_str());
    if(ifm.is_open()){
        while(ifm.getline(read_buf,buflen)){
            if(read_conf(bs,read_buf,flag) == 1) continue;
            else {
                cout << "配置文件错误" <<endl;
                return -1;
            }
        }
        return 1;
    }else  return -1;
}
void version(){
    ifstream ifm;
    char read_buf[32];
    unsigned int buflen = sizeof(read_buf);
    ifm.open("../attr/version.v",ios_base::in);
    if(ifm.is_open()){
        memset(read_buf,0,buflen);
        while(ifm.getline(read_buf,buflen)){
            cout << read_buf<<endl;
        }
    }else{
        cout << "版本文件打开出错"<<endl;
    }
    return ;
}
void help(){
    ifstream ifm;
    char read_buf[32];
    ifm.open("../attr/help.txt",ios_base::in);
    unsigned int buflen = sizeof(read_buf);
    if(ifm.is_open()){
        memset(read_buf,0,buflen);
        while(ifm.getline(read_buf,buflen)){
            cout << read_buf<<endl;
        }
    }else{
        cout << "帮助文件打开出错"<<endl;
    }
    return ;
}
int test_conf(string filename){
    server_base bs_temp;
    string the_end_filename("../conf/");
    ifstream ifm;
    char read_buf[64];
    int flag = 0;

    unsigned int buflen = sizeof(read_buf);
    the_end_filename += filename;
    ifm.open(the_end_filename.c_str(),ios_base::in);
    if(ifm.is_open()){
        while(ifm.getline(read_buf,buflen)){
            if(read_conf(bs_temp,read_buf,flag) == 1) continue;
            else return -1;
        }
        cout << filename << "文件配置正确"<<endl;
        return 1;
    }
    else{
        cout << "配置文件打不开"<<endl;
        return -1;
    }
}
int read_conf(server_base& bs,char* read_buf,int flag){
 
    register string temp = read_buf;
    if(temp.substr(0,5) == "user ") {
        //cout << "user right" << endl;
        if(flag) bs.user =  temp.substr(5,strlen(read_buf) - 5); 
        return 1;
    }
    if(temp.substr(0,9) == "listenip "){
        //cout << "listenip right" << endl; 
        if(flag) bs.listenip = temp.substr(9,strlen(read_buf) - 9);
        return 1;
    }
    if(temp.substr(0,11) == "listenport "){
        //cout << "listenport right" << endl;
        if(flag) bs.listenport = atoi(temp.substr(11,strlen(read_buf) -11 ).c_str());
        return 1;
    }
    if(temp.substr(0,14) == "workerprocess "){
        //cout << "workerprocess right" << endl;
        if(flag) bs.workerprocess = atoi(temp.substr(14,strlen(read_buf) -14).c_str());
        return 1;
    }
    if(temp.substr(0,12) == "server_name "){
        //cout << "server_name right" << endl;
        if(flag) bs.server_name = temp.substr(12,strlen(read_buf) - 12);
        return 1;
    }
    if(temp.substr(0,9) == "rootpath "){
        //cout << "rootpath right" << endl;
        if(flag) {
            bs.rootpath= temp.substr(9,strlen(read_buf) - 9);
        }
        return 1;
    }
    /*
    if(temp.substr(0,5) == "agent"){
        bs.agent = 1;
        return 1;
    }*/
    if(temp.length() == 0 ){
        //cout << "读到了空行"<<endl;
        return 1;
    }
    if(temp.substr(0,7) == "server "){
        //cout << "187:temp "<<temp<<endl;
        //cout << "server right"<<endl;
        string ip;
        unsigned int port;
        int weight;

        //现在要正则表达式确定ip
        string ip_port_weight = temp.substr(7,temp.find(':',8) - 7);
        std::regex ip_reg("^(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])");
        std::smatch result;
        bool valid;
        if((valid = std::regex_match(ip_port_weight,result,ip_reg))){
            ip = result[0];
        }else{return -1;}
        //ip已经确定了
        //现在要确定port
        //cout << "201确定了ip"<<endl;
        ip_port_weight = temp.substr(8,temp.length());
        int assign_posi = ip_port_weight.find(':',0);
        //第一个:的位置
        char cport[6];
        memset(cport,0,sizeof(cport));

        if(assign_posi == ip_port_weight.npos) port = 80;
        else{
            int i = 0;
            int cport_index = 0;
            for(i = assign_posi+1;i != ip_port_weight.length();i++){
                if(ip_port_weight[i] == ' ') break;
                else{
                    cport[cport_index++] = ip_port_weight[i];
                }
            }
            port = atoi(cport);
        }
        //port已经准备好了
        //现在开始准备weight
        
        //unsigned int weight_posi = ip_port_weight.find(':',ip_port_weight.find("weight:",0));
        size_t weight_posi = ip_port_weight.find("weight:",0);
        if(weight_posi == ip_port_weight.npos){
            weight = 1;
        }else{
            
            string w("weight:");
            std::regex w_reg("^(0|[1-9][0-9]*|-[1-9][0-9]*)$");
            string wei = ip_port_weight.substr(ip_port_weight.find("weight:")+w.length(),ip_port_weight.length() - w.length());
            std::smatch result;
            if(std::regex_match(wei,result,w_reg)){
                string temp = result[0];
                weight = atoi(temp.c_str());
            }
            
        }
        //现在已经得到了weight
        //cout << "在parajudge.cpp 229中" <<"ip = " << ip << " port = " << port << " weight =" << weight <<endl;
            bs.servers.addip(ip,port,weight);    
        return 1;
    }
    return -1;
}
