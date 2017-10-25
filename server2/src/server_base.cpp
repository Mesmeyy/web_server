/*************************************************************************
	> File Name: server_base.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 19时48分08秒
 ************************************************************************/

#include<iostream>
#include<netinet/in.h>
#include<cstring>
#include<sys/stat.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string>
#include<unistd.h>
#include<pwd.h>
#include"server_base.h"
#define Default_listen 5
using namespace std;
using std::string;

server_base::server_base()
{
    user = "";
    listenip = "";
    listenport = -1;
    workerprocess = 1;
    server_name = "";
    rootpath = "";
    agent = 0;
    sockfd = 0;
}

server_base::~server_base(){}
int server_base::start_server_base()
{
    //开始判断当前用户
    struct passwd *pwd = NULL;
    pwd = getpwuid(getuid());
    string pwuser(pwd -> pw_name);
    if(pwuser != user) return 0;

    //设定ip和端口
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        cout << "创建监听套接字失败"<<endl;
        return 0;
    }
    int optival = 1;
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(void*)&optival,sizeof(int)) < 0){
        cout << "设置套接字属性失败"<<endl;
        return 0;
    }
    memset(&address,0,sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(listenport);
    if(inet_aton(listenip.c_str(),&address.sin_addr) == 0) {
        cout << "设置ip失败"<<endl;
        return 0;
    }

    if(bind(sockfd,(struct sockaddr*)&address,sizeof(struct sockaddr_in)) < 0){
        cout << "绑定套接字失败"<<endl;
        return 0;
    }
    if(listen(sockfd,Default_listen) < 0){
        cout << "监听失败"<<endl;
        return 0;
    }
    
    if(rootpath != ""){
        if(server_name == "") server_name = "admin";
        userrootpath = rootpath + server_name;   
        if(mkdir(userrootpath.c_str(),S_IRWXU|S_IRWXG) != 0){
            cout << "用户目录不正确或目录已经存在"<<endl;
            return 0;
        }
    }
    return sockfd;
}
