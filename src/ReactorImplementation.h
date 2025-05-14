#ifndef  REACTORIMPLEMENTATION_H_
#define REACTORIMPLEMENTATION_H_
#include<unordered_map>
#include <memory>

#include "Event.h"

// #include "EventHandler.h"

typedef int Handle;

class EventDemultiplexer;
class EventHandler;

using EventHandler_share_ptr=std::shared_ptr<EventHandler>;
class ReactorImplementation{
private:
    std::shared_ptr<EventDemultiplexer> demultiplexer=nullptr;
    std::unordered_map<Handle,EventHandler_share_ptr> handlers;
    ReactorImplementation(){};
public:

    static std::shared_ptr<ReactorImplementation> get_ReactorImplementation();
    void set_demultiplexer(std::shared_ptr<EventDemultiplexer> demultiplexer){
        this->demultiplexer=demultiplexer;
    }
    int regist(EventHandler_share_ptr,Event);
    void remove(EventHandler_share_ptr);
    void event_loop(int timeout);
    int modify(EventHandler_share_ptr,Event);
};



#endif // ! REACTORIMPLEMENTATION_H_
