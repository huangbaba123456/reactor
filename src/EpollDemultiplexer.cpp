#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <stdexcept>
#include <string>
#include <sys/epoll.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <vector>


#include "EpollDemultiplexer.h"
#include "EventDemultiplexer.h"
#include "EventHandler.h"
#include "Log.h"

using  namespace std;

EpollDemultiplexer::EpollDemultiplexer(){
    epoll_fd=epoll_create1(0);
    if(epoll_fd<0){
        char buf[128]={0};
        sprintf(buf, "epoll_create1 failed: %s",strerror(errno) );
        LOG_FATAL(buf);
        exit(0);
    }
    evs.resize(MAX_PROCESS_EVENT); // 一次最多处理10个事件
}

EpollDemultiplexer::~EpollDemultiplexer(){
    close(epoll_fd);
}



int EpollDemultiplexer::regist(EventHandler_share_ptr eventhandler,Event event) {
    Handle socketfd=eventhandler->get_handle();
    // 加入epoll的监听中
    epoll_event ev;
    ev.events=(decltype(EPOLLIN))event | EPOLLET;
    //ev.data.ptr=eventhandler;
    ev.data.fd=eventhandler->get_handle();
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD, socketfd, &ev)<0){
        char buf[128]={0};
        sprintf(buf, "epoll_ctl ADD: %s",strerror(errno) );
        LOG_ERROR(buf);
        return 0;    
    }
    return 1;
}

int EpollDemultiplexer::modify(EventHandler_share_ptr handler,Event event){
    epoll_event ev={0};
    ev.events=(EPOLL_EVENTS)event | EPOLLET; // 边缘触发
    ev.data.fd=handler->get_handle();
    if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, handler->get_handle(), &ev)<0){
        char buf[128]={0};
        sprintf(buf, "epoll_ctl MOD: %s",strerror(errno) );
        LOG_ERROR(buf);
        return 0;  
    }
    return 1;
}

int EpollDemultiplexer::remove(Handle handler){
    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, handler, nullptr)<0){
        char buf[128]={0};
        sprintf(buf, "epoll_ctl DEL: %s",strerror(errno) );
        LOG_ERROR(buf);
        return 0;
    }
    return 1;
}
int EpollDemultiplexer::wait_event(std::unordered_map<Handle, EventHandler_share_ptr>& handlerMap,int timeout){
    int nfds=epoll_wait(epoll_fd,evs.data(),MAX_PROCESS_EVENT,timeout ); // timeout 的单位为毫秒
    if(nfds<=0){
        LOG_ERROR("epoll_wait timeout");
    }else{
        // 我们处理事件
        for(int i=0;i<nfds;i++){
            // 取出对应的Eventhandler
            EventHandler_share_ptr ptr=handlerMap[evs[i].data.fd];
            if(evs[i].events & EPOLLIN){
                // 读
                ptr->handle_read();
            }
            if(evs[i].events & EPOLLOUT){
                // 写
                ptr->handle_write();
            }
            if(evs[i].events & (EPOLLERR | EPOLLHUP)){
                // 错误
                ptr->handle_error();
            }
        }
    }
    return nfds;
}
