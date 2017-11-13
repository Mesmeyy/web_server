/*************************************************************************
	> File Name: server_fzjh.h
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月13日 星期一 19时56分37秒
 ************************************************************************/

#ifndef _SERVER_FZJH_H
#define _SERVER_FZJH_H
typedef struct
{
    int weight;
    int now_weight;
    ip_home *next;
    int att ;//当前服务器是否可用 ,0不可用1可用
}ip_home;

class fzjh
{
public:
    ip_home *home_use;//当前服役的服务器
    ip_home *home_now;//当前该选它作为负载均衡的服务器
    ip_home *homw_lazy;//目前gg服务器
    int ip_number;
    int over;//是否轮寻完一圈,1轮寻完一圈，要重置了,0没有轮寻完一圈

};
#endif
