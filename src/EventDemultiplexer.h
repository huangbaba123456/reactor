#ifndef EVENTDEMULTIPLEXER_H_
#define EVENTDEMULTIPLEXER_H_
#include "Event.h"
#include <unordered_map>
#include <memory>

class EventHandler;


typedef int Handle;
using EventHandler_share_ptr=std::shared_ptr<EventHandler>;





class EventDemultiplexer{
public:
    EventDemultiplexer(){};
    virtual ~EventDemultiplexer(){};
    virtual int regist(EventHandler_share_ptr,Event)=0;
    virtual int remove(Handle)=0;
    virtual int modify(EventHandler_share_ptr handler,Event event)=0;
    virtual int wait_event(std::unordered_map<Handle, EventHandler_share_ptr>&,int)=0;
};

#endif // !EVENTDEMULTIPLEXER_H_
