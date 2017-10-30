/*************************************************************************
	> File Name: parajudge.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时22分37秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<cstdlib>
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
    int flag = 1;

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
    ifm.open("../conf/version.v",ios_base::in);
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
    ifm.open("../conf/help.txt",ios_base::in);
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
        if(flag) {
            bs.rootpath = temp.substr(9,strlen(read_buf) - 9);
        }
        return 1;
    }
    if(temp.substr(0,5) == "agent"){
        bs.agent = 1;
        return 1;
    }
    return -1;
}
