/*************************************************************************
	> File Name: parajudge.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时22分37秒
 ************************************************************************/

#include<iostream>
#include<string>
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
    if(a == 1){
        if(make_server_base(bs,default_conf) == 1)return Default_start;
        else return -1;
    }
    if(a == 2){
        string V(b[1]);
        string S(b[1]);
        if(V == "-v") {
            version();
            return Version;
        }
        else if(S == "-s") {
            return Stop;
        }
        else if(strcmp(b[1],"-?")){
            help();
            return Help;
        }
        else if(strcmp(b[1],"-h")){
            help();
            return Help;
        }
        else{
            return -1;
        }
    }
    if(a == 3){
        if(strcmp(b[1],"-t")){
            if(test_conf(b[2]) == 1) return Conf;
            else return -1;
        }
        else if(strcmp(b[1],"-c")){
            if(make_server_base(bs,b[2]) == 1)return Conf_change;
            else return -1;
        }
    }
}

int make_server_base(server_base&bs,string filename){}
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
int test_conf(string filename){}
