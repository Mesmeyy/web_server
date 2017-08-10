/*************************************************************************
	> File Name: server_processpool.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月07日 星期一 16时10分32秒
 ************************************************************************/

#ifndef _SERVER_PROCESSPOOL_H
#define _SERVER_PROCESSPOOL_H
#include<sys/types.h>
#include"server_epoll.h"

class process
{
public:process():m_pid(-1){}
public:
    pid_t m_pid;
    int m_pipefd[2];
};
template<typename T>
class processpool
{
public:
    processpool(int listenfd);/*进程池构造函数*/

public:
    static processpool<T>* create(int listenfd)
    {
        if(!m_instance){
            m_instance = new processpool<T>(listenfd);
        }
        return m_instance;
    }
    /*线程池创建函数，返回进程池实例的指针*/;
    ~processpool();
    /*进程池析构函数,销毁父进程和子进程*/
    void run();
    /*进程池运行的根函数,由主函数调度*/
private:
    void setup_sig_pipe();
    /*把相关信号加入handler中，创建信号管道,父子共用这一对信号管道*/
    void run_father();
    /*父进程运行函数，监听listenfd,处理客户请求，处理信号*/
    void run_child();
    /*子进程运行函数，监听每一个客户，处理客户各种请求,也是和http类的接口函数*/

private:
    epoll_class epoll_object;/*epoll实例，含有epollfd和events*/
    static const int MAX_PROCESS_NUMBER = 8;/*子进程的数量*/
    static const int USER_EVERY_PROCESS = 65535;/*T user的大小*/
    int m_process_number;//当前子进程数量
    int m_idx;//子进程在进程池里面的序号
    int m_listenfd;//父进程的监听描述符
    bool m_stop;//是否运行
    process *m_sub_process;//存放所有子进程实例的指针
    static processpool<T> *m_instance;//存放进程池的实例
};


template <typename T>
processpool<T> * processpool<T>::m_instance = NULL;
static int sig_pipefd[2];

#endif
