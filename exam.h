#ifndef _EXAM_H
#define _JSS_H
#include "server_epoll.h"
#include<arpa/inet.h>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
class exam
{
public:
    static epoll_class *epoll_object;
    struct sockaddr_in client_addr;
    int sockfd;
    
    void init(epoll_class *epoll_ex,int sockfd,const sockaddr_in * clientaddr){
       // epoll_object = new epoll_class;
        //memcpy(epoll_object,&epoll_ex,sizeof(epoll_class));
        epoll_object = epoll_ex;
        this -> sockfd = sockfd;
        client_addr = *clientaddr ;
    }
    void process()
    {
        while(1){
            char readbuf[1024];
            memset(readbuf,0,1024);
            int ret = recv(sockfd,readbuf,1024,0);
            if(ret < 0){
                if(errno != EAGAIN){
                    fprintf(stderr,"319:ret < 0 close\n");
                    //continue;
                    epoll_object -> server_delfd(sockfd);
                }
                break;
            }
            if(ret == 0){
                fprintf(stderr,"326:ret = 0 close\n");
                epoll_object->server_delfd(sockfd);
                break;
            }else{

                fprintf(stderr,"331:sockfd %d :%s",sockfd,readbuf);
                epoll_object -> server_modfd(sockfd,EPOLLOUT);
                if(send(sockfd,(char*)"This is from server",32,0) < 0){
                    perror("334:服务器发送消息失败");
                    continue;;
                 }
                epoll_object -> server_modfd(sockfd,EPOLLIN);
            }
        }
    }

};
#endif
