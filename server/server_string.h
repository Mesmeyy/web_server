/*************************************************************************
	> File Name: server_string.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月02日 星期三 16时58分36秒
 ************************************************************************/

#ifndef _SERVER_STRING_H
#define _SERVER_STRING_H
bool c_cmp(char a,char b);/*比较两个字母*/

long unsigned int server_strlen(char *target);/*求目标串长度*/

char* server_strpbrk(char* target,char* temp);
/*比较target与temp中是否有相同的字符,有则返回在target中该字母的指针，反之返回NULL*/

bool server_strcasecmp(char *a,char *b);/*忽略大小写比较字符串是否相等*/

int server_strspn(char*a,char*b);
/*返回a中第一个不在b中出现的字符的下标*/


//bool server_memset(char *a,int  b,long unsigned int n);
/*字符串a从0开始到n-1为止的n个字符全部转化为字符串b*/

//bool server_strcat(char *a,char *b);/*把b接到a后面*/

//bool server_strncat(char *a,char *b,int n);/*b的前n字符接到a后面*/

//bool server_strcpy(char *a,char *b);/*b覆盖到a*/

//bool server_strncpy(char *a,char *b,int n);/*b的前n个字符覆盖到a*/
#endif
