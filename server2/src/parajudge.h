/*************************************************************************
	> File Name: parajudge.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时09分34秒
 ************************************************************************/

#ifndef _PARAJUDGE_H
#define _PARAJUDGE_H
#include"server_base.h"
#include<string>
using namespace std;
int parajudge(int a,char *b[],server_base & bs);
int make_server_base(server_base& bs,string filename);
void version();
void help();
int test_conf(string filename);
int read_conf(server_base& bs,char* read_buf,int flag = 1);
#endif
