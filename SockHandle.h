#ifndef SOCKHANDLE_H_
#define SOCKHANDLE_H_

#include "EventHandler.h"
typedef int Handle;

class SockHandle:public EventHandler{
private:
    Handle sock_fd;
    char *buf; // 在堆空间上分配内存
    int MAX_SIZE;
public:
    SockHandle(Handle sock_fd):sock_fd(sock_fd){};
    virtual ~SockHandle(); // 要注意释放sock_fd
    virtual Handle get_handle() const override;
    virtual void handle_read()  override;
    virtual void handle_write()  override;
    virtual void handle_error()  override;
};





#endif