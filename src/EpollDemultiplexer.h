#ifndef EPOLLDEMULTIPLEXER_H_
#define EPOLLDEMULTIPLEXER_H_

#include "EventDemultiplexer.h"
#include <functional>
#include <sys/epoll.h>
#include <vector>
#define MAX_PROCESS_EVENT 10

class EpollDemultiplexer: public EventDemultiplexer{
private:
    int epoll_fd;
    std::vector<epoll_event> evs;
public:
    EpollDemultiplexer();
    virtual ~EpollDemultiplexer();
    int regist(EventHandler_share_ptr,Event) final override;
    int modify(EventHandler_share_ptr handler,Event event) final override;

    int remove(Handle) final override;
    int wait_event(std::unordered_map<Handle, EventHandler_share_ptr>&,int) final override;
};


#endif // !EPOLLDEMULTIPLEXER_H_
