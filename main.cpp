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
    server_base serobject;
    if((argc == 1)){
        if(serobject.set_base(serobject,(char*)"../conf/server.conf") != 0){
            cout << "初始化server_base失败"<<endl;
            return -1;
        }
    }
    
    if(argc == 2){
        string V("-V"),v("-v"),h("-h"),Q("-?");
        if((argv[1] == V) || (argv[1] == v)) {show_version();return 0;}
        if((argv[1] == h )|| (argv[1] == Q)) {show_help();return 0;}
        cout << "帮助文件:"<<endl;
        show_help();
        return -1;
    }
    if(argc == 3){
        string s("-s"),S("-S"),c("-c");
        if(argv[1] == S || argv[1] == s){show_break();return 0;}
        else if(argv[1] == "-c") {
            if(serobject.set_base(serobject,(char*)"../conf/server.conf") != 0){
                cout << "初始化server_base失败"<<endl;
                return -1;
            }
            get_other_conf();
        }    
        else {
            cout << "帮助文件:"<<endl;
            show_help();
            return -1;
        }
        
    }
    /*初始化全部完成,开始正式工作*/
}
