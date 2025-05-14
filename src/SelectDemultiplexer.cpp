#include "SelectDemultiplexer.h"
#include "EventDemultiplexer.h"
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/select.h>
#include "EventHandler.h"
#include "Log.h"
SelectDemultiplexer::SelectDemultiplexer(){
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&err_set);
}
SelectDemultiplexer::~SelectDemultiplexer(){
}

int SelectDemultiplexer::regist(EventHandler_share_ptr ptr,Event event){
    Handle fd=ptr->get_handle();
    if((uint32_t)event & EPOLLIN){
        FD_SET(fd, &read_set);
    }
    if((uint32_t)event & EPOLLOUT){
        FD_SET(fd, &write_set);
    }
    if((uint32_t)event & EPOLLERR){
        FD_SET(fd, &err_set);
    }
    return 1;
}
int SelectDemultiplexer::remove(Handle fd){
    if(FD_ISSET(fd, &read_set)){
        FD_CLR(fd, &read_set);
    }
    if(FD_ISSET(fd, &write_set)){
        FD_CLR(fd, &write_set);
    }
    if(FD_ISSET(fd, &err_set)){
        FD_CLR(fd, &err_set);
    }
    return 1;
}
int SelectDemultiplexer::modify(EventHandler_share_ptr handler,Event event){
    // 先删除再注册
    Handle fd=handler->get_handle();
    remove(fd);
    return regist(handler, event);
}
int SelectDemultiplexer::wait_event(std::unordered_map<Handle, EventHandler_share_ptr>& handlerMap ,int timeout){
    // 下面我们开始监听
    timeval tv={0};
    if(timeout>0){
        // 查了我好久，
        tv.tv_usec=timeout*1000;
    }
    int maxFd=0;
    for(auto& p:handlerMap){
        maxFd=std::max(p.first,maxFd);
    }
    std::cout<<std::endl;
    fd_set tmp_read_set;
    fd_set tmp_write_set;
    fd_set tmp_err_set;
    // 复制过去
    memcpy(&tmp_read_set, &read_set, sizeof(fd_set));
    memcpy(&tmp_write_set, &write_set, sizeof(fd_set));
    memcpy(&tmp_err_set, &err_set, sizeof(fd_set));
    int ret;
    if(timeout>0)
        ret=select(maxFd+1, &tmp_read_set, &tmp_write_set, &tmp_err_set, &tv);
    else
        ret=select(maxFd+1, &tmp_read_set, &tmp_write_set, &tmp_err_set, nullptr);
    if(ret==-1){
        LOG_ERROR(std::string("SELECT ERROR: ")+strerror(errno));
        return 0;        
    }else if(ret==0){
        LOG_DEBUG("SELECT timeout");
        return 1;
    }
    // 下面我们开始便利
    for(auto& p:handlerMap){
        Handle curFd=p.first;
        if(FD_ISSET(curFd, &tmp_read_set)){
            p.second->handle_read();
        }
        if(FD_ISSET(curFd, &tmp_write_set)){
            p.second->handle_write();
        }
        if(FD_ISSET(curFd, &tmp_err_set)){
            p.second->handle_error();
        }
    }
    return ret;
}


