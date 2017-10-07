/*************************************************************************
	> File Name: main.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月05日 星期四 16时42分15秒
 ************************************************************************/

#include<iostream>
#include"server_base.h"
#include"para_judge.h"
#include"server_conf.h"

using namespace std;
int main(int argc,char*argv[])
{
    server_base serobject;
    server_conf *sc = NULL ;
    para_judge(&sc,serobject,argc,argv);
    if(sc == NULL) return 0;
    /*服务器配置完成*/
}
