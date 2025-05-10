#ifndef LISTENHANDLE_H_
#define LISTENHANDLE_H_

#include "EventHandler.h"
typedef int Handle;

class ListenHandle:public EventHandler{
private:
    Handle listenfd;
public:
    ListenHandle();
    virtual ~ListenHandle();
    virtual Handle get_handle() const override;
    virtual void handle_read() override;
    virtual void handle_write() override;
    virtual void handle_error() override;
    bool listenOn(int port);
    void setNonBlocking(int fd);
};



#endif // !LISTENHANDLE_H_
