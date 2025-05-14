#ifndef SELECTDEMULTIPLEXER_H_
#define SELECTDEMULTIPLEXER_H_

#include "EventDemultiplexer.h"
#include <sys/select.h>

class SelectDemultiplexer: public EventDemultiplexer{
private:
    fd_set read_set;
    fd_set write_set;
    fd_set err_set;
public:
    SelectDemultiplexer();
    virtual ~SelectDemultiplexer();
    int regist(EventHandler_share_ptr,Event) final override;
    int remove(Handle) final override;
    int modify(EventHandler_share_ptr handler,Event event) final override;
    int wait_event(std::unordered_map<Handle, EventHandler_share_ptr>&,int) final override;
};


#endif // !SELECTDEMULTIPLEXER_H_