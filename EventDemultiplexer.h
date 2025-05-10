#ifndef EVENTDEMULTIPLEXER_H_
#define EVENTDEMULTIPLEXER_H_
#include "Event.h"
#include <unordered_map>
typedef int Handle;
class EventHandler;


class EventDemultiplexer{
public:
    EventDemultiplexer(){};
    virtual ~EventDemultiplexer(){};
    virtual int regist(EventHandler*,Event)=0;
    virtual int remove(Handle)=0;
    virtual int wait_event(std::unordered_map<Handle, EventHandler>,Event)=0;
};

#endif // !EVENTDEMULTIPLEXER_H_
