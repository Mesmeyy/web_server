/*************************************************************************
	> File Name: para_judge.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月07日 星期六 15时38分37秒
 ************************************************************************/

#include<iostream>
#include<unistd.h>
#include<cstring>
#include<string>
#include<stdlib.h>
#include"server_base.h"
#include"server_conf.h"
#include"parameter.h"
#include"para_judge.h"
using namespace std;
int para_judge(server_conf** sc,server_base& serobject,int argc,char *argv[])
{
    if((argc == 1)){
        if(serobject.set_base(serobject,(char*)"../conf/server.conf") != 0){
            cout << "初始化server_base失败"<<endl;
            return -1;
        }
        *sc = (server_conf*)malloc(sizeof(server_conf));
        memset(*sc,0,sizeof(server_conf));
        return 0;
    }
    
    if(argc == 2){
        string V("-V"),v("-v"),h("-h"),Q("-?");
        if((argv[1] == V) || (argv[1] == v)) {show_version(); return -1;}
        if((argv[1] == h )|| (argv[1] == Q)) {show_help();return -1;}
        cout << "帮助文件:"<<endl;
        show_help();
        return -1;
    }
    if(argc == 3){
        string s("-s"),S("-S"),c("-c");
        if(argv[1] == S || argv[1] == s){show_break();return -1;}
        else if(argv[1] == c) {
            if(serobject.set_base(serobject,(char*)"../conf/server.conf") != 0){
                cout << "初始化server_base失败"<<endl;
                return -1;
            }
            int ret = get_agency_conf(sc,argv[2]);
            return 2;
        }    
        else {
            cout << "帮助文件:"<<endl;
            show_help();
            return -1;
        }
    }
}
