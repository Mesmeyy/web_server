/*************************************************************************
	> File Name: main.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月05日 星期四 16时42分15秒
 ************************************************************************/

#include<iostream>
#include"server_base.h"
#include"parameter.h"
using namespace std;
int main(int argc,char*argv[])
{
    server_base s;
    if((argc == 1)||(argc==3)){
        if(s.set_base(s,(char*)"../conf/server.conf") != 0){
            cout << "初始化server_base失败"<<endl;
            return -1;
        }
    }
    /*
    if(argc == 2){
        if(argv[1] == "-V" || argv[1] == "-v") {show_version();return 0;}
        if(argv[1] == "-h" || argv[1] == "-?") {show_help();return 0;}
        cout << "server使用不正确"<<endl;
        return -1;
    }*/
    if(argc == 3){
        if(argv[1] == "-S" || argv[1] == "-s"){show_break();return 0;}
        else if(argv[1] == "-C" || argv[1] == "-c"){set_conf();}
        else {
            cout << "server使用不正确"<<endl;
            return -1;
        }
    }

}
