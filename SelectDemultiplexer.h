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
    virtual ~SelectDemultiplexer(){};
    int regist(EventHandler*,Event) final override;
    int remove(Handle) final override;
    int wait_event(std::unordered_map<Handle, EventHandler>,Event) final override;
};


#endif // !SELECTDEMULTIPLEXER_H_