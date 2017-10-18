/*************************************************************************
	> File Name: client.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月12日 星期六 08时40分45秒
 ************************************************************************/

#include<iostream>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#define PORT 38002
using namespace std;
int main()
{   
    struct sockaddr_in address;
    memset(&address,'0',sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    int ret = inet_aton((char*)"127.0.0.1",&address.sin_addr);
    if(ret == 0){
        cout << "地址失败"<<endl;
        return 0;
    }
 
    int connfd = socket(AF_INET,SOCK_STREAM,0);
    if(connfd < 0){
        cout <<"客户端创建套接字失败"<<endl;
        return 0;
    }
    ret = connect(connfd,(struct sockaddr*)&address,sizeof(struct sockaddr));
    if(ret < 0){
        
        //cout <<"连接失败"<<endl;
        perror("连接失败");
        return 0;
    }
    char readbuf[1024];
    while(1){
        sleep(3);
        memset(readbuf,0,1024);
        int ret = send(connfd,(char*)"This is from client",32,0);//将来就是发送http请求包
        if(ret < 0){
            cout << "客户端发送信息失败"<<endl;
            continue;
        }else{
            cout << "客户端发送信息成功"<<endl;

        }
        ret = recv(connfd,readbuf,1024,0);//将来就是http响应包
        if(ret < 0){
            cout << "读取服务器信息失败"<<endl;
            continue;
        }else {
            cout <<"client:recvbuf = "<< readbuf << endl;
        }
    }
}
