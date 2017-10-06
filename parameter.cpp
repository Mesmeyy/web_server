/*************************************************************************
	> File Name: parameter.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月05日 星期四 20时35分51秒
 ************************************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;

void show_version();/*查看版本*/
void show_help();/*查看帮助*/
void show_break();/*停止运行*/

void show_version(){
    ifstream ifm;
    ifm.open("../conf/version.v",ios::in);
    if(!ifm.is_open()){
        cout << "找不到版本文件"<<endl;
        return ;
    }
    char read_buf[256];
    while(ifm.getline(read_buf,sizeof(read_buf))){
        cout << read_buf<<endl;
        memset(read_buf,0,sizeof(read_buf));
    }
}

void show_help(){
    ifstream ifm;
    ifm.open("../conf/help.txt",ios::in | ios::binary);
    if(!ifm.is_open()){
        cout << "找不到帮助文件"<<endl;
        return ;
    }
    char read_buf[256];
    while(ifm.getline(read_buf,sizeof(read_buf))){
        cout << read_buf<<endl;
        memset(read_buf,0,sizeof(read_buf));
    }
}
void show_break(){}

