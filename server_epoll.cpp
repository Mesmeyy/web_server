/*************************************************************************
	> File Name: server_epoll.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月03日 星期四 19时32分00秒
 ************************************************************************/

#include<iostream>
#include<sys/epoll.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include"server_epoll.h"
using namespace std;

epoll_class::epoll_class()
{
    epollfd = server_epoll_create(5);
    now_listen_fds = 0;
    events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * MAX_LISTEN_FD);
    memset(events,0,sizeof(struct epoll_event) * 1024);
}

epoll_class::~epoll_class()
{
    server_epoll_kill();
}

bool epoll_class::server_addfd(int fd)
{
    if(now_listen_fds >= (MAX_LISTEN_FD - 2)) return false;
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    server_nonblocking(fd);
    
    now_listen_fds ++;
}

bool epoll_class::server_delfd(int fd)
{
    if(now_listen_fds <= 0) return false;
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
    now_listen_fds --;
}

bool epoll_class::server_modfd(int fd,int ev)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}

int epoll_class::server_epoll_wait()
{
    return epoll_wait(epollfd,events,MAX_LISTEN_FD,-1);
}

int epoll_class::server_epoll_create(int first_size)
{
    return epoll_create(first_size);
}

bool epoll_class::server_epoll_kill()
{
    if(now_listen_fds > 0) {
        for(int i = 0;i < MAX_LISTEN_FD;i++){
            if(events[i].data.fd >= 0){
                close(events[i].data.fd);
            }else{
                continue;
            }
        }
    }
    close(epollfd);
    return true; 
}

bool epoll_class::server_nonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return true;
}


