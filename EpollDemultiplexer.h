#ifndef EPOLLDEMULTIPLEXER_H_
#define EPOLLDEMULTIPLEXER_H_

#include "EventDemultiplexer.h"
#include <sys/epoll.h>
#include <vector>
class EpollDemultiplexer: public EventDemultiplexer{
private:
    int epoll_fd;
    std::vector<epoll_event> evs;
public:
    EpollDemultiplexer();
    virtual ~EpollDemultiplexer(){};
    int regist(EventHandler*,Event) final override;
    int remove(Handle) final override;
    int wait_event(std::unordered_map<Handle, EventHandler>,Event) final override;
};


#endif // !EPOLLDEMULTIPLEXER_H_
