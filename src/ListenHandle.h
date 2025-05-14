#ifndef LISTENHANDLE_H_
#define LISTENHANDLE_H_

#include "EventHandler.h"
#include <memory>
#include <functional>
#include <netinet/in.h>

typedef int Handle;
using ConnectionEstablish_callback=std::function<bool(Handle,sockaddr_in)>;
class ListenHandle:public EventHandler,public std::enable_shared_from_this<ListenHandle>{
private:
    Handle listenfd;
    void setNonBlocking(int fd);
    ListenHandle(); // 不希望直接创建对象

    // 连接建立的回调函数，这个函数会把新的连接加入reactor中 ，在reactor中被赋值，会被指向一个reactor的一个成员函数
    ConnectionEstablish_callback connectionEstablish_callback=nullptr; 
public:
    void set_connectionEstablish_callback(ConnectionEstablish_callback connectionEstablish_callback);
    static std::shared_ptr<ListenHandle> get_ListenHandle();
    virtual ~ListenHandle();
    virtual Handle get_handle() const override;
    virtual void handle_read() override;
    virtual void handle_write() override;
    virtual void handle_error() override;
    bool listenOn(int port);
};



#endif // !LISTENHANDLE_H_
