/*************************************************************************
	> File Name: http_conn.cpp
	> Author: zhuziyu
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年08月10日 星期四 16时13分17秒
 ************************************************************************/
#include<iostream>
#include<string.h>
#include"http_conn.h"
#include"server_epoll.h"

static epoll_class  *m_epoll ;
static int m_usr_count  = 0 ;
void removefd(int epollfd,int fd)
{
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}
void http_conn::close_conn()
{
    if(m_sockfd != -1)
    {
        //removefd(m_epollfd,m_sockfd);
        m_epoll -> server_delfd(m_sockfd);
        m_sockfd = -1;
         m_usr_count--;
    }
}
void http_conn::init(int s)
{
   // m_usr_count = 0;
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = 0;
    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    
    memset(m_read_buf,'\0',READ_BUFFER_SIZE);
    memset(m_write_buf,'\0',WRITE_BUFFER_SIZE);
    memset(m_real_file,'\0',FILENAME_LEN); 
    
    //m_sockfd = s;
}

bool server_nonblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

bool server_addfd(int epollfd,int fd)
{
    struct epoll_event events;
    events.data.fd = fd;
    events.events = EPOLLIN|EPOLLET|EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&events);
    server_nonblocking(fd); 
}

void http_conn::init(epoll_class &epolltemp,int  sockfd,const struct sockaddr_in& addr)
{


    //m_epoll = new epoll_class;
    //*m_epoll = *epolltemp; 
    m_epoll = &epolltemp;
    m_sockfd = sockfd;
    m_address = addr;////////////////////////////////
    m_usr_count++;
    init(m_sockfd);
}

/*接收读取客户端发来http请求*/
int http_conn::read()
{
    int m_read_idx = 0;
  
    memset(m_read_buf,0,1024);
    if(m_read_idx >= READ_BUFFER_SIZE)
    {
        return 0;
    }
    int bytes_read = 0;
    while(1)
    {
        bytes_read = recv(m_sockfd,m_read_buf+m_read_idx,READ_BUFFER_SIZE-m_read_idx,0);
        if(bytes_read == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            return 0;
        }
        else if(bytes_read == 0)
        {
            return 0;
        }
        m_read_idx += bytes_read;
    }
    m_read_idx =sizeof(m_read_buf);

    cout << "============================"<<endl;
    cout << "read函数接收到的m_read_buf是:\n"<< m_read_buf<<endl;
    cout << "============================"<<endl<<endl<<endl;
    return 1;
}
/*检测一行是否完整*/
 http_conn::LINE_STATUS http_conn::parse_line()
{
    char temp;
    m_read_idx = sizeof(m_read_buf);
    
    for(;m_checked_idx<m_read_idx;++(m_checked_idx))
    {
        temp = m_read_buf[m_checked_idx];
        if(temp == '\r')
        {
            if(((m_checked_idx)+1) == m_read_idx)
            {
            
                return LINE_OPEN;
            }
            else if(m_read_buf[(m_checked_idx)+1]=='\n')
            {
                m_read_buf[(m_checked_idx)++] = '\0';
                m_read_buf[(m_checked_idx)++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(temp == '\n')
        {
            if(((m_checked_idx)>1)&&(m_read_buf[(m_checked_idx)-1]) == '\r')
            {
                m_read_buf[(m_checked_idx)-1] = '\0';
                m_read_buf[(m_checked_idx)++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

/*检测请求行*/
http_conn::HTTP_CODE http_conn::parse_request_line(char* text)
{
    //memset(m_url,0,)
    m_url = strpbrk(text," \t");
    if(!(m_url))
    {
        printf("1:BAD_REQUEST");
        return BAD_REQUEST;
    }
   * m_url++ = '\0';
    char* method = text;
 
    if(strcasecmp(method,"GET") == 0)
    {
        m_method = GET;
    }

    else if(strcasecmp(method,"POST") == 0)
    {
        m_method = POST;
    }
    else
    {
        printf("2:BAD_REQUEST");
        return BAD_REQUEST;
    }
    m_url += strspn(m_url," \t");
    m_version = strpbrk(m_url," \t");
    if(!(m_version))
    {
        printf("3:BAD_REQUEST");
        return BAD_REQUEST;
    }

    *m_version++='\0';
    m_version +=strspn(m_version," \t");
    if(strcasecmp(m_version,"HTTP/1.1") != 0)
    {       printf("4:BAD_REQUEST");
        return BAD_REQUEST;
    }

    if(strncasecmp(m_url,"http://",7) == 0)
    {
        m_url += 7;
        m_url = strchr(m_url,'/');
    }
    if(!(m_url) || (m_url)[0] !='/')
    {
        printf("5:BAD_REQUEST");
        return BAD_REQUEST;
    }
    m_check_state = CHECK_STATE_HEADER;
    //return NO_REQUEST;


    if(m_url[strlen(m_url) - 1] == '/'){
        strcat(m_url,"index.html");
    }else{
        ;
    }
    cout << "---------"<<endl;
    cout << "在parse_request_line函数中:解析到的参数是:"<<endl;
    cout << "请求:m_method = " << m_method << endl;
    cout << "url:m_url = " << m_url << endl;
    cout << "http版本 m_version = " << m_version << endl;
    cout << "---------"<<endl;


    return NO_REQUEST;
}

/*检测头部信息*/
http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{
    if(text[0] == '\0')
    {
        if(m_content_length != 0)
        {
            m_check_state = CHECK_STATE_CONNECT;
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    else if(strncasecmp(text,"Connection:",11) == 0)
    {
        text += 11;
        text += strspn(text," \t"); // text此时为connection后面的内容
        if(strcasecmp(text,"keep alive") == 0)
        {
            m_linger = 1;
        }
    }
    else if(strncasecmp(text,"Content-Length:",15) == 0)
    {
        text += 15;
        text +=strspn(text," \t");
        m_content_length = atol(text);
    }
    else if(strncasecmp(text,"Host:",5) == 0)
    {
        text += 5;
        text +=strspn(text," \t");
        m_host = text;
    }
    else
    {
        printf("unknow header:%s",text);
    }
    return NO_REQUEST;
    
}

http_conn::HTTP_CODE http_conn::parse_content(char* text)
{
    m_content_length = strlen(text);
    if(m_read_idx >= (m_content_length + m_checked_idx))
    {
        text[m_content_length] = '\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn:: do_request()
{
    memset(m_real_file,0,1024);
    strcpy(m_real_file,"/home/zhuziyu/文档");
    int len = strlen("/home/zhuziyu/文档");
    strncpy(m_real_file + len,m_url,FILENAME_LEN-len-1);
    int ret;
    if((ret = stat(m_real_file,&m_file_stat) )< 0)
    {
        return NO_REQUEST;
    }
    if(S_ISDIR(m_file_stat.st_mode&S_IROTH))
    {
        return FORBIDDEN_REQUEST;
    }
    if(S_ISDIR(m_file_stat.st_mode))
    {
        return BAD_REQUEST;
    }
    int fd = open(m_real_file,O_RDONLY);
    m_file_address = (char*)mmap(0,m_file_stat.st_size,PROT_READ,MAP_PRIVATE,fd,0);
    close(fd);
    return FILE_REQUEST;
}

void http_conn::unmap()
{
    if(m_file_address)
    {
        munmap(m_file_address,m_file_stat.st_size);
        m_file_address = 0;
    }
}


bool http_conn::write()
{
    int temp = 0;
    int bytes_have_send = 0;
    int bytes_to_send = m_write_idx;
    if(bytes_to_send == 0)
    {
       m_epoll -> server_modfd(m_sockfd,EPOLLIN);
       // init();
        return 1;
    }

    while(1)
    {
        temp = writev(m_sockfd,m_iv,m_iv_count);
        if(temp <= -1)
        {
            if(errno == EAGAIN)
            {
                m_epoll -> server_modfd(m_sockfd,EPOLLOUT);
                return 1;
            }
            unmap();
            return 0;
        }

    bytes_to_send -= temp;
    bytes_have_send += temp;

        if(bytes_to_send <= bytes_have_send)
        {
        unmap();
            if(m_linger)
        {
            init(m_sockfd);
            m_epoll -> server_modfd(m_sockfd,EPOLLIN);
            return 1;
        }
        else{
            m_epoll -> server_modfd(m_sockfd,EPOLLIN);
            return 0;
        }
    }
    }
}


bool http_conn::add_response(const char* format,...)
{
    
    if(m_write_idx >= WRITE_BUFFER_SIZE)
    {
        return 0;
    }

    va_list arg_list;
    va_start(arg_list,format);
    int len = vsnprintf(m_write_buf+m_write_idx,WRITE_BUFFER_SIZE-1-m_write_idx,format,arg_list);
    if(len >= (WRITE_BUFFER_SIZE-1-m_write_idx))
    {
        return 0;
    }
    m_write_idx += len;
    va_end(arg_list);
    return 1;
}


bool http_conn::add_statues_line(int status,const char* title)
{
    return add_response("%s %d %s\r\n","HTTP/1.2",status,title);
}

bool http_conn::add_headers(int content_len)
{
    add_content_length(content_len);
    add_linger();
    add_blank_line();
}

bool http_conn::add_content_length(int content_len)
{
    return add_response("Content-LENGTH: %d\r\n",content_len);
}

bool http_conn::add_linger()
{
    return add_response("Connection:%s\r\n",(m_linger == 1)?"keep-alive":"close");

}

bool http_conn::add_blank_line()
{
    return add_response("%s","\r\n");
}

bool http_conn::add_content(const char* content)
{
    return add_response("%s",content);
}

bool http_conn::process_write(HTTP_CODE ret)
{
    switch(ret)
    {
        case INTERNAL_ERROR:
        {
            add_statues_line(500,"Internal Error");
            add_headers(strlen("There was an unusual problem serving the requested file.\n"));
            if(! add_content("There was an unusual problem serving the requested file.\n"))
            {
                return 0;
            }
            break;
        }

        case BAD_REQUEST:
        {
            add_statues_line(400,"Bad Request");
            add_headers(strlen("Your request has bad syntax or is inherently impossible to satisfy.\n"));

            if(!add_content("Your request has bad synax or is inherently impossible to satisfy.\n"))
            {
                return 0;
            }
            break;
        }

        case NO_REQUEST: 
        { 
            add_statues_line(404,"Not Found");
            add_headers(strlen("The requested file was not found on this server.\n"));

            if(!add_content("The requested file was not found on this server.\n"))
            {
                return 0;
            }
            break;
            
        }

        case FORBIDDEN_REQUEST:
        {
            add_statues_line(403,"Forbidden");
            add_headers(strlen("You do not have permission to get file from this server.\n"));

            if(!add_content("You do not have permission to get file from this server.\n"))
            {
                return 0;
            }
            break;
        }

        case FILE_REQUEST:
        {
            add_statues_line(200,"OK");
            if(m_file_stat.st_size != 0)
            {
                add_headers(m_file_stat.st_size);
                m_iv[0].iov_base = m_write_buf;
                m_iv[0].iov_len = m_write_idx;
                m_iv[1].iov_base = m_file_address;
                m_iv[1].iov_len = m_file_stat.st_size;
                m_iv_count = 2;
                return 0;
            }
            else
            {
                const char* ok_string = "<html><body></body></html>";
                add_headers(strlen(ok_string));
                if(!add_content(ok_string))
                {
                    return 0;
                }
            }
        }
default:
    {
        return 0;
    }        
    }
    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    return 1;

}

http_conn::HTTP_CODE http_conn::process_read()
{
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;
    m_start_line = 0;
    while(((m_check_state == CHECK_STATE_CONNECT)&& (line_status == LINE_OK))||((line_status = parse_line()) == LINE_OK))
    {   
        cout << "正在进程process"<<endl;
        text = get_line();
        m_start_line = m_checked_idx;
        switch(m_check_state)
        {
            case CHECK_STATE_REQUESTLINE:
            {
                cout << "即将进入parse_request_line函数"<<endl;
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                break;
            }

            case CHECK_STATE_HEADER:
            {
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                else if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                line_status = LINE_OK;
                break;
            }

            case CHECK_STATE_CONNECT:
            {
                ret = parse_content(text);
                if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default:
            {
                return INTERNAL_ERROR;
            }
        }
    }
}

void http_conn::process()
{
    init(m_sockfd);
    int ret_from_read = read();
    if(ret_from_read == 0){
        cout << "有一个用户下线了"<<endl;
        m_epoll -> server_delfd(m_sockfd);
        close(m_sockfd);
        return ;
    }
    HTTP_CODE read_ret = process_read();
    cout << "\n\n\n本次所请求的文件名称是:" << m_real_file << endl;
    if(read_ret == NO_REQUEST)
    {
        m_epoll -> server_modfd(m_sockfd,EPOLLIN);
        return;
    }
    int write_ret = process_write(read_ret);
    /*if(write_ret)
    {
        close_conn();
    }*/
    if(write_ret){
        //如果出差错了的话,z这次发送就忽略,恢复监听epollin事件,监听下一次
        m_epoll -> server_modfd(m_sockfd,EPOLLIN);
       return ; 
    }
    m_epoll ->server_modfd(m_sockfd,EPOLLOUT);
    write();

}
/*
int main()
{
	
    epoll_class *m_epoll;
    m_epoll = new epoll_class(5);
   
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(2003);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    
    http_conn *users = new http_conn[1024];

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    listen(listenfd,5);
    
    m_epoll -> server_addfd(listenfd);
    epoll_class temp = *m_epoll;

    while(1){
        int ret = m_epoll -> server_epoll_wait();
        if(ret < 0){
            return 0;
        }
        if(ret == 0) continue;
        for(int i = 0;i < ret;i++){
            int sockfd = m_epoll -> events[i].data.fd;
            if(sockfd == listenfd){
                struct sockaddr_in client_address;
                socklen_t client_length = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_length);
                m_epoll -> server_addfd(connfd);
                users[connfd].init(temp,connfd,client_address);
                continue;
            }
            if(m_epoll -> events[i].events & EPOLLIN){
                users[sockfd].process();
                continue;
            }
            else continue;
        }
    }
}*/
