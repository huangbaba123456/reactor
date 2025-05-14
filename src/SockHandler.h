#ifndef SockHandler_H_
#define SockHandler_H_

#include "EventHandler.h"
#include <memory>
#include <string>
#define BUFFER_INIT_SIZE 10240
typedef int Handle;
class Buffer;
class Reactor;
class SockHandler:public EventHandler,public std::enable_shared_from_this<SockHandler>{
private:
    Handle sock_fd=-1;
    std::shared_ptr<Buffer> writeBuffer;
    std::shared_ptr<Buffer> readBuffer;    
public:
    SockHandler(int size=BUFFER_INIT_SIZE);
    virtual ~SockHandler(); // 要注意释放sock_fd
    virtual Handle get_handle() const override final;
    virtual void handle_read()  override final;
    virtual void handle_write()  override final;
    virtual void handle_error()  override final;
    int write(const char* cs,int size) ;
    int write(const std::string& s) ;
    void set_sock_fd(Handle sock_fd);


    // 业务重写的方法
    virtual void connection_call_back()=0;
    virtual void read_call_back(const char*data,int size)=0;
};





#endif