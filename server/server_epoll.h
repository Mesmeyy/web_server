/*************************************************************************
	> File Name: server_epoll.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月03日 星期四 17时37分04秒
 ************************************************************************/

#ifndef _SERVER_EPOLL_H
#define _SERVER_EPOLL_H
#include<sys/epoll.h>
class epoll_class
{
public:
    epoll_class();
    /*构造函数,创建epoll_class类的实例*/
    
    ~epoll_class();
    /*析构函数,销毁epoll_class类的实例*/
    
    bool server_addfd(int fd);
    /*向epoll_class中的epollfd添加监听的套接字描述符*/

    bool server_delfd(int fd);
    /*向epoll_class中的epollfd删除监听的套接字描述符*/

    bool server_modfd(int fd,int ev);
    /*更改epollfd所监听的fd套接字描述符的监听事件,ev是新监听的事件*/
    int server_epoll_wait();
    /*epoll_wait的封装函数,返回就绪的描述符个数*/
    bool server_nonblocking(int fd);
    /*把fd更改为非阻塞fd*/
    
private:
    int server_epoll_create(int first_size);
    /*被构造函数调用，用来创建epollfd内核事件监听符*/

    bool server_epoll_kill();
    /*被析构函数调用,销毁epollfd*/

public:
    int epollfd;/*epoll_class核心变量,用来监听epoll事件的描述符*/
    const int MAX_LISTEN_FD = 1024;/*一个进程最多监听MAX_LISTEN_FD个套接字描述符*/
    struct epoll_event *events;//[MAX_LISTEN_FD];/*用来存放监听的描述符们的*/
    int now_listen_fds;/*当前被epollfd监听的套接字个数*/
};
#endif
