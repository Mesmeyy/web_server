/*************************************************************************
	> File Name: processpool.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月11日 星期五 08时45分08秒
 ************************************************************************/

#include<iostream>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include"server_epoll.h"
#include"processpool.h"
#define SER_PORT 38002
//#include"exam.h"
#include"http_conn.h"
using namespace std;

/*template <typename T>
Processpool<T>* Processpool<T>::m_instance = NULL;
static int sig_pipefd[2];
*/
//static epoll_class this_epoll;
static void exitout(int ret,string msg)
{
    cout << msg << endl;
    return ;
}

static void sig_handler(int sig)
{
    int save_errno=errno;
    int msg = sig;
    send(sig_pipefd[1],(char*)&msg,1,0);//往信号1发信号
    errno=save_errno;
}

static void addsig(int sig,void(handler)(int),bool restart = true)
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = handler;
    if(restart){
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset(&sa.sa_mask);
    if(sigaction(sig,&sa,NULL) == -1){
        exitout(-1,"sigaction error");
    }
}

template<typename T>
Processpool<T>::Processpool(int listenfd,int process_number)
{
    m_listenfd = listenfd;
    m_sub_process = new Process[process_number];//一开始初始化进程池
    m_process_number = process_number;
    m_idx = -1;//父进程是-1
    m_stop = false;
    if(process_number > MAX_PROCESS_NUMBER){
        exitout(-1,"申请创建的子进程数过大");
    }
    for(int i = 0;i < process_number;i++){
        int ret = socketpair(PF_UNIX,SOCK_STREAM,0,m_sub_process[i].m_pipefd);//单个子进程和父进程之间私有的管道
        if(ret<0){
            exitout(-1,"父子一对管道创建失败");
        }
        m_sub_process[i].m_pid = fork();
        if(m_sub_process[i].m_pid<0){
            exitout(-1,"fork error");
        }
        if(m_sub_process[i].m_pid > 0){
            close(m_sub_process[i].m_pipefd[1]);//父进程关1用0
        }
        if(m_sub_process[i].m_pid == 0){
            m_idx = i;
            close(m_sub_process[i].m_pipefd[0]);//子进程关0用1
            break;
        }
    }
}

template<typename T>
void Processpool<T>::setup_sig_pipe()
{
    epoll_object = new epoll_class;
    if(epoll_object == NULL ){
        exitout(1,"87:进程自己的epoll_object创建失败");
    }
    int ret = socketpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
    if(ret < 0){
        exitout(-1,"91:信号管道创建失败");
    }
    epoll_object -> server_nonblocking(sig_pipefd[1]);//信号往[1]写
    epoll_object -> server_addfd(sig_pipefd[0]);//信号从[0]读

    addsig(SIGCHLD,sig_handler);
    addsig(SIGTERM,sig_handler);
    addsig(SIGINT,sig_handler);
    addsig(SIGPIPE,SIG_IGN);
}


template<typename T>
void Processpool<T>::run()
{
    if(m_idx != -1){
        run_child();
        return ;
    }
    run_parent();
}

template<typename T>
void Processpool<T>::run_parent()
{
    printf("116:这是父进程在运行\n");
    setup_sig_pipe();
    epoll_object -> server_addfd(m_listenfd);//父进程负责监听就行
    int ok_number = 0;//就绪文件描述符个数
    int new_conn = 1;//信号来临的标志,发送给子进程
    int m_sub_counter = 0;

    while(!m_stop){
        ok_number = epoll_object -> server_epoll_wait();
        cout << "125:父进程ok_number = " << ok_number << endl;
        /*如果当前出错,没有有效信号*/
        
        if((ok_number < 0)&& (errno != EINTR)){
            printf("129:epoll failure\n");
            break;
        }
        for(int i = 0;i < ok_number;i++){
            int sockfd = epoll_object -> events[i].data.fd;
            if(sockfd == m_listenfd){
                int i = m_sub_counter;
                do{
                    if(m_sub_process[i].m_pid != -1){break;}
                    i = (i+1)%m_process_number;
                }while(i != m_sub_counter);
                if(m_sub_process[i].m_pid == -1){
                    /*进程任务分配出错*/
                    m_stop = true;
                    break;
                }
                m_sub_counter = (i+1)%m_process_number;
                int ret = send(m_sub_process[i].m_pipefd[0],(char*)&new_conn,4,0);
                if(ret < 0){
                    perror("148:向子进程发送任务失败");
                    continue;
                }
                cout << "151:send request to "<< i<<" child"<<endl;
            }
            else if((sig_pipefd[0] == sockfd) && (epoll_object -> events[i].events &EPOLLIN)){
                printf("154:有子进程死了\n");
                int sig;
                char signals[1024];
                memset(signals,0,1024);
                int ret = recv(sig_pipefd[0],signals,sizeof(signals),0);
                if(ret <= 0){
                    continue;
                }else{
                    for(int i = 0;i < ret;i++){
                        /*处理多个信号*/
                        switch(signals[i]){
                            case SIGCHLD:{
                                /*子进程终止时候会产生这个信号,父进程监测到底是哪一个子进程gg了*/
                                pid_t pid;
                                int stat;
                                while((pid == waitpid(-1,&stat,WNOHANG)) > 0){
                                    for(int i = 0;i < m_process_number;i++){
                                        if(m_sub_process[i].m_pid == pid){
                                            cout << "子进程"<< i <<" 终止了"<<endl;
                                            close(m_sub_process[i].m_pipefd[0]);
                                            m_sub_process[i].m_pid = -1;
                                            m_process_number --;
                                        }
                                    }
                                }
                                /*查看当前是否所有子进程都退出*/
                                m_stop = true;
                                for(int i = 0;i < m_process_number;i++){
                                    if(m_sub_process[i].m_pid != -1){
                                        m_stop = false;
                                        break;
                                    }
                                }
                                break;
                            }
                            case SIGTERM:/*这是一个普通的结束该进程的终止信号*/
                            case SIGINT:{
                                /*这个信号告诉父进程要杀死所有子进程,并等待他们全部结束*/
                                cout << "父进程现在要杀死所有子进程"<<endl;
                                for(int i = 0;i < m_process_number;i++){
                                    int pid = m_sub_process[i].m_pid;
                                    if(pid != -1){
                                        kill(pid,SIGTERM);
                                    }
                                }
                                break;
                                return ;
                            }
                            defalut:break;
                        }
                    }
                }
            }
            else{continue;}
        }
    }
    
}

template <typename T>
void Processpool<T>:: run_child()
{
    cout << "****  这是子进程 "<<m_idx << " 在运行  ****"<<endl;
    setup_sig_pipe();
    static epoll_class &this_epoll = *epoll_object;
    int pipefd = m_sub_process[m_idx].m_pipefd[1];
    epoll_object -> server_addfd(pipefd);

    T* users = NULL;
    users = new T [USER_PER_PROCESS];
    if(!users){
        exitout(-1,"开辟特定处理类失败");
    }

    int ok_number = 0; 
    int ret;
    while(!m_stop){
        ok_number = epoll_object -> server_epoll_wait();
        cout << "\n\n子进程"<< m_idx << "的　ok_number = " << ok_number << endl;
        if((ok_number < 0) && (errno != EINTR)){
            exitout(-1,"232:子进程监听失败了\n");
            cout <<"233:子进程"<< m_idx<<"要退出了"<<endl;
            break;
        }
        for(int i = 0;i < ok_number;i++){
            int sockfd = epoll_object -> events[i].data.fd;
            if((sockfd == pipefd) && (epoll_object -> events[i].events & EPOLLIN)){
                //证明要创建新用户连接了,而且父进程是给子进程发送了一个4个字节的int强转的char类型
                int client;
                ret = recv(sockfd,(char*)&client,4,0);
                if((ret  < 0)&& (errno != EAGAIN)){
                    //这里感觉没有把情况考虑完整
                    continue;
                }else{
                    cout <<"246:子进程 "<<m_idx<< " 已经接到新用户的任务,connecting.."<<endl;
                    struct sockaddr_in client_address;
                    socklen_t len= sizeof(client_address);
                    int connfd = accept(m_listenfd,(struct sockaddr*)&client_address,&len);
                    if(connfd < 0){
                        cout << "251:errno = " <<errno << endl;
                        continue;//感觉一旦失败,这个客户就连接不上了
                    }
                    else{
                        fprintf(stderr,"255:%d    accept new connection",getpid());
                        fprintf(stderr,"\n256:      connfd = %d\n",connfd);
                        epoll_object -> server_addfd(connfd);
                        users[connfd].init(this_epoll,connfd,client_address);
                    }
                }
            }
            else if((sockfd == sig_pipefd[0]) && (epoll_object -> events[i].events & EPOLLIN)){
                int sig;
                char signals[1024];
                memset(signals,'0',1024);
                ret = recv(sig_pipefd[0],signals,1024,0);
                if(ret <= 0)continue;
                else{
                    for(int i = 0;i < ret;i++){
                        switch(signals[i]){
                            case SIGCHLD:{
                                //我感觉作为没有孙子进程的子进程不会收到这个信号
                                pid_t pid;
                                int stat;
                                while((pid = waitpid(-1,&stat,WNOHANG))>0) continue;
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:{m_stop = true;break;}
                            defalut:break;
                        }
                    }
                }
            }
            else if(epoll_object -> events[i].events & EPOLLIN){
                /*正常树立用户请求*/
                users[sockfd].process();//调用类的处理函数,里面包含读和注册写然后写并发送
            }
            else{
                /*其他情况一律不处理*/
            }
        }
    }
    fprintf(stderr,"294:child %d is died\n",m_idx);
}


epoll_class *epoll_object = NULL;
int main(int argc,char*argv[])
{
    struct sockaddr_in server_address;
    server_address.sin_family = PF_INET;
    server_address.sin_port = htons(SER_PORT);
    //inet_pton(PF_INET,"127.0.0.1",&server_address.sin_addr);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    bind(sockfd,(struct sockaddr*)&server_address,sizeof(server_address));
    listen(sockfd,5);
    
    Processpool<http_conn > *pool = Processpool<http_conn>::create(sockfd);
    if(pool){
        cout << "358:创建进程池成功"<<endl;
        pool -> run();
        delete pool;
    }
    close(sockfd);
    return 0;
}










