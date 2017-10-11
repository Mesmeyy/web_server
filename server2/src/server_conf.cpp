/*************************************************************************
	> File Name: server_conf.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月07日 星期六 15时49分36秒
 ************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<fstream>
#include<string>
#include<regex>
#include"server_conf.h"
using namespace std;
int read_conf(server_conf*sc,char *a)
{
    ifstream ifm;
    ifm.open(a);
    if(!ifm.is_open()){
        cout << "找不到 "<< a << "文件"<<endl;
        return -1;
    }
    char *rbuf;
    rbuf = (char*)malloc(512);
    while(ifm.getline(rbuf,sizeof(rbuf))){
        string  rsf = rbuf;
        smatch resultstr;
        regex sonstr("server:");
        regex_match(rsf,resultstr,sonstr);
        if(resultstr != "server:") return -1;
        
    }
}
int  get_agency_conf(server_conf** sc,char* conffilename)
{
    *sc = new server_conf ;
    if (read_conf(*sc,conffilename) == -1);
        return -1;
    return 0;
}

