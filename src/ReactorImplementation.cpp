#include "ReactorImplementation.h"
#include "EpollDemultiplexer.h"
#include "EventDemultiplexer.h"
#include "EventHandler.h"
#include "SelectDemultiplexer.h"
#include <memory>
#include <sys/epoll.h>
#include <tuple>
#include <sys/epoll.h>
#include "Log.h"
std::shared_ptr<ReactorImplementation> ReactorImplementation::get_ReactorImplementation(){
    //std::shared_ptr<ReactorImplementation> ptr=std::make_shared<ReactorImplementation>();
    std::shared_ptr<ReactorImplementation> ptr(new ReactorImplementation());
    #ifndef USE_SELECT
        ptr->demultiplexer=std::make_shared<EpollDemultiplexer>();
    #else
        LOG_DEBUG("use select");
        ptr->demultiplexer=std::make_shared<SelectDemultiplexer>();
    #endif
    return ptr;
}


int ReactorImplementation::regist(EventHandler_share_ptr handler,Event event){
    // 同时我们把这个存入表中
    handlers[handler->get_handle()]=handler;
    // 代理模式
    return this->demultiplexer->regist(handler,event);
}
int ReactorImplementation::modify(EventHandler_share_ptr handler,Event event){
    // 修改监听的事件
    if(
        !((EPOLL_EVENTS)event & EPOLLIN) 
    &&  !((EPOLL_EVENTS)event & EPOLLOUT)
    &&  !((EPOLL_EVENTS)event & EPOLLERR)
    ){
        // 如果啥事件都不监听，就没什么意义了
        this->remove(handler); // 删除这个憨逼
        return 1;
    }else {
        return this->demultiplexer->modify(handler,event);
    }
}
void ReactorImplementation::remove(EventHandler_share_ptr handler){
    // 从hash表里面删除
    handlers.erase(handler->get_handle());
    (void)this->demultiplexer->remove(handler->get_handle());
}
void ReactorImplementation::event_loop(int timeout){
    // 监听
    (void)this->demultiplexer->wait_event(handlers, timeout);
}