/*************************************************************************
	> File Name: client.cpp
	> Author: liangmengdi
	> Mail: llmmdd1997130@126.com
	> Created Time: 2017年09月19日 星期二 19时41分24秒
 ************************************************************************/

#include<iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
using namespace std;

int main()
{
    struct sockaddr_in server_address;
    memset(&server_address,0,sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(38002);
    inet_aton("127.0.0.1",&server_address.sin_addr);

    int connfd = socket(AF_INET,SOCK_STREAM,0);
    if(connect(connfd,(struct sockaddr *)&server_address,sizeof(server_address)) < 0){
        cout << "Connection fail"<<endl;
        return 0;
    }

    char m_read_buf[2048];
    memset(m_read_buf,0,2048);
    strcat(m_read_buf,"GET / HTTP/1.0\r\n");
    strcat(m_read_buf,"Host: 127.0.0.1\r\n");
    strcat(m_read_buf,"Connection: keep alive\r\n");
    strcat(m_read_buf,"Content-Length:10\r\n");
    strcat(m_read_buf,"\r\n\0");
    strcat(m_read_buf,"hello world");

    while(1){
        int ret;
        if(ret = send(connfd,m_read_buf,2048,0)){
            //cout << "data send ok"<<endl;
            cout << "ret = " << ret <<endl;
        }else{continue;}
        
        char m_recv_buff[2048];
        memset(m_recv_buff,0,2048);
        if(recv(connfd,m_recv_buff,2048,0) < 0){
            cout << "recv fail"<<endl;
            continue;
        }else{
            cout << "==============================="<<endl;
            cout << "recv :"<<endl;
            cout << m_recv_buff<<endl;
            cout << "==============================="<<endl<<endl;
        }
        sleep(2);
    }
}

