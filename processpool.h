/*************************************************************************
	> File Name: processpool.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月11日 星期五 08时17分53秒
 ************************************************************************/

#ifndef _PROCESSPOOL_H
#define _PROCESSPOOL_H
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/epoll.h>
#include<signal.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include"server_epoll.h"

class Process
{
public:
    Process(){
        m_pid = -1;
    }
    pid_t m_pid;
    int m_pipefd[2];
};
template <typename T>
class Processpool
{
private:
    Processpool(int listenfd,int process_number = 8);
public:
    static Processpool<T>* create(int listenfd,int process_number=8){
        if(!m_instance){
            m_instance = new Processpool<T>(listenfd,process_number);
            return m_instance;
        }
    }
    ~Processpool(){
        delete [] m_sub_process;
    }
    void run();//入口函数
private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();

    static const int MAX_PROCESS_NUMBER = 8;//进程池的最大的进程数
    static const int USER_PER_PROCESS = 65535;//将来一个进程的最大连接数，但不同与最大的监听数
    static Processpool<T>* m_instance ;//将来的进程池静态实例

    int m_process_number;//京城池当前拥有的进程数目
    int m_idx;//每个进程的进程序号
    epoll_class *epoll_object;//epoll实例，包含epollfd和event
    int m_listenfd;//进程池的监听套接字
    int m_stop;//进程是否执行
    Process * m_sub_process;//管理进程的数组
};

template <typename T>
Processpool<T>* Processpool<T>::m_instance = NULL;
static int sig_pipefd[2];


#endif
