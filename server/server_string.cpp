/*************************************************************************
	> File Name: server_string.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月02日 星期三 17时16分22秒
 ************************************************************************/

#include<iostream>
#include<cstring>
#include"server_string.h"
using namespace std;

bool c_cmp(char a,char b)
{
    return a == b ? true:false;
}

long unsigned int server_strlen(char *target)
{
    return strlen(target);
}

char *server_strpbrk(char *target,char *temp)
{
    return strpbrk(target,temp);
}

bool server_strcasecmp(char *target,char *temp)
{
    return !strcasecmp(target,temp);
}

int server_strspn(char *target,char *temp)
{
    return strspn(target,temp);
}
/*
bool server_memset(char *target,int temp,long unsigned int n)
{

    cout << "target = " << target << endl; 
    memset(target,temp,n);
    cout << "then target = " << target << endl;
    return true;
}

bool server_strcat(char *target,char *temp)
{
    strcat(target,temp);
    return true;
}

bool server_strncat(char *target,char *temp,int n)
{
    strncat(target,temp,n);
    return true;
}

bool server_strcpy(char *target,char *temp)
{
    strcpy(target,temp);
    return true;
}

bool server_strncpy(char *target,char *temp,int n)
{
    strncpy(target,temp,n);
    return true;
}
*/
