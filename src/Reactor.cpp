#include "Reactor.h"
#include "Event.h"
#include "EventHandler.h"
#include "ReactorImplementation.h"
#include "Log.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <memory>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <type_traits>
#include <unistd.h>
#include <vector>
#include "ListenHandle.h"
#include "NofityHandler.h"
#include <errno.h>
using namespace std; 
Reactor::Reactor(){    
    this->reactorImp=ReactorImplementation::get_ReactorImplementation();
    // 设置本地通信的文件描述符
    int socketPair[2];
    int ret=socketpair(AF_UNIX, SOCK_STREAM,0,socketPair);
    if(ret==-1){
        LOG_FATAL("socketpair error");
        exit(0);
    }
    // fd[0] 只能写
    shutdown(socketPair[0], SHUT_RD);
    this->nofity_fd=socketPair[0]; 
    // fd[1] 只能读
    shutdown(socketPair[1], SHUT_WR);
    // 监听socketPair[1] 的读事件
    shared_ptr<NofityHandler> notify_handler=NofityHandler::get_NofityHandler(socketPair[1]);
    notify_handler->set_event(Event::ReadEvent); // 监听读
    notify_handler->set_reactor(this);
    ret=this->regist(notify_handler,notify_handler->event());
}
void Reactor::nofity_reactor(){
    // 唤醒reactor 处理任务
    char buf[1]={1};
    int len=write(nofity_fd, buf, 1);
    if (len < 0) {
        LOG_ERROR("Write to notify_fd failed, errno: " + std::to_string(errno) + ", error: " + std::string(strerror(errno)));
    }
}

int Reactor::regist(EventHandler_share_ptr ptr,Event event){
    if(!this->reactorImp){
        LOG_FATAL("reactorImp 必须被设置");
        exit(0);
    }
    return this->reactorImp->regist(ptr, event);
}

bool Reactor::modify(EventHandler_share_ptr ptr,Event event){
    if(!this->reactorImp){
        LOG_FATAL("reactorImp 必须被设置");
        exit(0);
    }
    ptr->set_event(event);  // 跟新事件
    return this->reactorImp->modify(ptr,event);
}

void Reactor::remove(EventHandler_share_ptr ptr){
    if(!this->reactorImp){
        LOG_FATAL("reactorImp 必须被设置");
        exit(0);
    }
    (void)this->reactorImp->remove(ptr);
}

void Reactor::addTask(std::function<void()> f){
    // 这里要激活，我等会写
    unique_lock<mutex> lock(task_que_mtx);
    task_que.push_back(f); // 加入任务队列
    // 唤醒
    nofity_reactor();
}


void Reactor::event_loop(int timeout){
    if(!this->reactorImp){
        LOG_FATAL("reactorImp 必须被设置");
        exit(0);
    }
    (void)this->reactorImp->event_loop(timeout);
    //LOG_DEBUG("ASD");
    // 处理任务队列,不要让那个锁一直锁着,参考muduols
    std::vector<std::function<void()>> taskArr;
    {
        unique_lock<mutex> lock(task_que_mtx);
        taskArr.resize(task_que.size());
        copy(task_que.begin(),task_que.end(),taskArr.begin());
        // 任务队列必须被清空
        task_que.clear();
    }
    // 执行任务
    for(auto& f:taskArr){
        f();
    }
}




