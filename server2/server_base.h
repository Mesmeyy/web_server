/*************************************************************************
	> File Name: server_base.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月04日 星期三 13时19分27秒
 ************************************************************************/

#ifndef _SERVER_BASE_H
#define _SERVER_BASE_H

#include<string>
#include<netinet/in.h>
#include<arpa/inet.h>

using namespace std;
class server_base
{
public:
    string user;/*谁能使用这个nginx*/
    string listenip;/*监听哪一个ip*/
    int listenport;/*监听哪一个端口*/
    int workerprocess;/*工作线程个数*/
    int listenfd;/*监听描述符*/
    struct sockaddr_in server_addr;/*相关选项*/
    server_base(){
        user = "";
        listenip = "";
        listenport = 0;
        int workerprocess = 0;
    }
    int set_base(server_base &s,char *);
    
private:
    int get_base(); 
};
#endif
