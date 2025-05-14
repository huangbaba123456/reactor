#ifndef REACTOR_H_
#define REACTOR_H_
#include <memory>
#include <netinet/in.h>
#include "Event.h"
#include <unistd.h>
#include <vector>
#include <functional>
#include <mutex>
class ReactorImplementation;
class EventHandler;
class ListenHandle;
typedef int Handle;
using EventHandler_share_ptr=std::shared_ptr<EventHandler>;

class Reactor{
private:
    std::shared_ptr<ReactorImplementation> reactorImp=nullptr;
        // 添加任务队列
    std::vector<std::function<void()>> task_que;
    std::mutex task_que_mtx; // 互斥访问
    int nofity_fd=-1;
    void nofity_reactor();
    Reactor();
public:
    friend class WorkerThread;
    ~Reactor(){
        if(nofity_fd!=-1){
            close(nofity_fd);
        }
    };

    // 删除那几个构造函数
    Reactor(const Reactor&)=delete;
    Reactor(Reactor&&)=delete;
    // 删除赋值函数
    Reactor& operator=(const Reactor&)=delete;
    Reactor& operator=(Reactor&&)=delete;    
    void set_reactorImp(std::shared_ptr<ReactorImplementation> reactorImp){
        this->reactorImp=reactorImp;
    }
    // int regist(EventHandler*,Event);
    // void remove(EventHandler*);
    // 我改一下接口类型，如果按照文档那样没法写
    int regist(EventHandler_share_ptr,Event);
    void remove(EventHandler_share_ptr);
    void event_loop(int timeout);
    // 我自己增加的接口,修改监听事件，为了实现读缓存
    bool modify(EventHandler_share_ptr,Event);
    static Reactor* get_instance(){
        //静态变量 保证 线程安全 
        static Reactor reator;
        return &reator;
    }
    void addTask(std::function<void()>);
};




#endif