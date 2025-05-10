#ifndef EVENT_H_
#define EVENT_H_


#include <sys/epoll.h>
enum class Event { 
    ReadEvent=EPOLLIN,
    WriteEvent=EPOLLOUT,
    ErrorEvent=EPOLLERR
};



#endif

