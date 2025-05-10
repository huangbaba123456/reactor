#ifndef  REACTORIMPLEMENTATION_H_
#define REACTORIMPLEMENTATION_H_
#include<unordered_map>
#include "Event.h"
// #include "EventHandler.h"

typedef int Handle;

class EventDemultiplexer;
class EventHandler;


class ReactorImplementation{
private:
    EventDemultiplexer* demultiplexer;
    std::unordered_map<Handle,EventHandler*> handlers;
public:
    int regist(EventHandler*,Event);
    void remove(EventHandler*);
    void event_loop(int timeout);
};



#endif // ! REACTORIMPLEMENTATION_H_
