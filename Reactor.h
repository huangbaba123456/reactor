#ifndef REACTOR_H_
#define REACTOR_H_


#include "Event.h"
class ReactorImplementation;
class EventHandler;


class Reactor{
private:
    Reactor(){};
    ReactorImplementation* imp_point; // 具体实现的指针
public:
    ~Reactor(){};
    // 删除那几个构造函数
    Reactor(const Reactor&)=delete;
    Reactor(Reactor&&)=delete;
    // 删除赋值函数
    Reactor& operator=(const Reactor&)=delete;
    Reactor& operator=(Reactor&&)=delete;
    
    int regist(EventHandler*,Event);
    void remove(EventHandler*);
    void event_loop(int timeout);
  
    Reactor* get_instance(){
        //静态变量 保证 线程安全 
        static Reactor reator;
        return &reator;
    }
};




#endif