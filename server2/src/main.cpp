/*************************************************************************
	> File Name: main.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月05日 星期四 16时42分15秒
 ************************************************************************/

#include<iostream>
#include"server_base.h"

using namespace std;
int main()
{
    server_base s;
    if(s.set_base(s,(char*)"../conf/server.conf") != 0){
        cout << "初始化server_base失败"<<endl;
        return -1;
    }

}
