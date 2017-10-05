/*************************************************************************
	> File Name: server_base.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月04日 星期三 13时34分41秒
 ************************************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<cstring>
#include<pwd.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"server_base.h"
using namespace std;

int check_user(server_base &base,string &read);/*解析用户是谁*/
int check_listenip(server_base &base,string &read);/*解析监听的网卡*/
int check_listenport(server_base &base,string &read);/*解析监听的端口*/
int check_workerprocess(server_base &base,string &read);/*解析工作进程个数*/

int server_base::set_base(server_base &base,char * filename)
{
    int line_num = 0;
    ifstream  ifs(filename); 
    string read;
    while(getline(ifs,read)){
        line_num ++;
        if(line_num > 4) {
            if(get_base() != 0) return -1;
            else {
                cout << "服务器基础server_base创建完成..."<<endl;
                return 0;
            }
        }
        switch(line_num){
            case 1: if(check_user(base,read) == 0) break;else return -1;
            case 2: if(check_listenip(base,read) == 0) break;else return -1;
            case 3: if(check_listenport(base,read) == 0) break;else return -1;
            case 4: if(check_workerprocess(base,read) == 0) break;else return -1;
            default :return -1;
        }
    }
}

int check_user(server_base& base,string& read)
{
    if(read.substr(0,5) == "user "){
        struct passwd *pwd;
        /*想得到当前运行server的用户,和conf文件设置用户进行权限比较*/
        pwd = getpwuid(getuid());
        unsigned int str_len = strlen(pwd -> pw_name);
        if(read.substr(5,str_len) == pwd -> pw_name){
            cout << "用户正确..."<<endl;
            base.user = pwd -> pw_name;
            return 0;
        }else{
            cout << "用户无权限..."<<endl;
            return -1;
        }
    }else{
        cout << "server.conf文件错误"<<endl;
        return -1;
    }
}

int check_listenip(server_base& base,string& read)
{
    if(read.substr(0,9) == "listenip "){
        int position_n = read.find('\n',9);
        int str_len = position_n - 9;
        base.listenip = read.substr(9,str_len);
        cout << "base.listenip = " << base.listenip << endl;
        return 0;
    }else{
        cout << "server.conf文件错误"<<endl;
        return -1;
    }
}

int check_listenport(server_base& base,string& read)
{
    if(read.substr(0,11) == "listenport "){
        int position_n = read.find('\n',11);
        int str_len = position_n - 11;
        base.listenport = atoi(read.substr(11,str_len).c_str());
        cout << "base.listenport = " <<base.listenport << endl;
        return 0;
    }else{
        cout << "server.conf文件错误"<<endl;
        return -1;
    }
}

int check_workerprocess(server_base& base,string& read)
{
    if(read.substr(0,14) == "workerprocess "){
        int position_n = read.find('\n',14);
        int str_len = position_n - 14;
        base.workerprocess = atoi(read.substr(14,str_len).c_str());
        cout << "base.workerprocess = "<<base.workerprocess << endl;
        return 0;
    }else{
        cout << "server.conf文件错误"<<endl;
        return -1;
    }
}

int server_base::get_base()
{
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0){
        cout << "创建监听套接字失败"<<endl;
        return -1;
    }
    int optval = 1;
    if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(void*)&optval,sizeof(int)) < 0){
        cout << "重置端口失败"<<endl;
        return -1;
    }
    memset(&server_addr,0,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    if(listenport > 65535) {
        cout << "绑定端口无效"<<endl;
        return -1;
    }
    server_addr.sin_port = htons(listenport);
    if(listenip == "INADDR_ANY"){
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        //return 0;
    }else{
        if(inet_aton(listenip.c_str(),&server_addr.sin_addr) == 0){
            cout << "绑定ip无效"<<endl;
            return -1;
        }
    }
    return 0;
}
