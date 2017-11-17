/*************************************************************************
	> File Name: hong.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月23日 星期一 20时15分08秒
 ************************************************************************/

#ifndef _HONG_H
#define _HONG_H
#define Default_start 1 //以默认的形式开始
#define Help 2     //输入是help参数
#define Version 3   //进行的是版本查询
#define Conf 4     //进行的是配置检查
#define Stop 5    //进行的平滑重启或者关闭
#define Conf_change 6   //进行的是-c 改变配置文件
#define server_bad_attr  -4 //本身服务器就是坏的
#define server_use_bad  -3  //服务器在使用过程中坏了，如果不人为修护并重启，则不使用它
#define server_last     -2  //除非其他服务起都坏，否则不使用它
#define server_user_exhaust 0 //服务器的名额用完，不再往这个服务器发送请求，但是服务器没有坏
#define server_default  1   //默认的R-R轮寻值
#define server_weight "^[1-9]\d*$" //匹配正整数
#define server_always -1 //新用户的链接一直给他，用到它坏
#endif
