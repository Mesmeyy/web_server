/*************************************************************************
	> File Name: server_processpool.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月07日 星期一 16时10分44秒
 ************************************************************************/

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

#include"server_processpool.h"
using namespace std;

/*信号处理函数*/
static void sig_handler(int sig)
{
    cout << "检测到信号值为"<< sig <<  ", 正在处理..."<<endl;
    int msg = sig;
    send(sig_pipefd[1],(char*)&msg,1,0);
    cout << "信号处理完毕..."<<endl;
    return ;
}

/*添加信号到监测集合*/
static void add_signal(int sig,void (handler)(int),bool restart = true)
{
    struct sigaction sa;
    memset(&sa,0,sizeof(sa));
    cout << "add_signal这里没有段错误" << endl;
    sa.sa_handler = handler;
    if(restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    int ret = sigaction(sig,&sa,NULL);//给信号sig设置信号处理函数
    if(ret == -1){
        cout << "设置信号不成功"<<endl;
        return ;
    }else{
        cout << sig <<" 信号设置成功"<<endl;
        return ;
    }
}

/*实例化一个进程池*/
template<typename T>
processpool<T>::processpool(int listenfd):m_listenfd(listenfd),m_idx(-1),m_stop(false)
{
    int m_process_number = 8;
    cout << "创建子进程中..."<<endl;
    m_sub_process = new process[m_process_number];

    for(int i = 0;i < m_process_number;i++){
        int ret = socketpair(PF_UNIX,SOCK_STREAM,0,m_sub_process[i].m_pipefd);
        m_sub_process[i].m_pid = fork();
        
        assert(m_sub_process[i].m_pid >= 0);
        /*有fork() < 0程序中断*/
        if(m_sub_process[i].m_pid){
            //当前是父进程
            close(m_sub_process[i].m_pipefd[1]);//父进程关1用0
            cout << "现在是父进程在运行"<<endl;
        }else{
            close(m_sub_process[i].m_pipefd[0]);//子进程关0用1
            m_idx = i;
            cout << "现在是子进程"<<i<<"在运行"<<endl;
            break;
        }
    }
}
/*
template<typename T>
processpool<T>::~processpool()
{
    delete [] m_sub_process;//销毁子进程
    delete m_instance;//进程池
}

template<typename T>
void processpool<T>:: run()
{
    if(m_idx != -1){
        run_child();
    }else{
        run_father();
    }
}
template <typename T>
void setup_sig_pipe()
{
    int ret = sockerpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
    assert()
}

template<typename T>
void run_father()
{
    //父进程有epoll;

}
*/
