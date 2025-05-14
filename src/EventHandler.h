#ifndef  EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "Event.h"

typedef int Handle;
class Reactor;
class EventHandler{
private:
    Reactor* reactor_=nullptr;
    Event event_;
public:
    virtual ~EventHandler(){};
    virtual Handle get_handle() const=0;
    virtual void handle_read()=0;
    virtual void handle_write()=0;
    virtual void handle_error()=0;
    Event event(){return event_;};
    void set_event(Event event){this->event_=event;};
    Reactor* reactor(){return this->reactor_;};
    void set_reactor(Reactor* reactor){this->reactor_=reactor;};
};




#endif // ! E