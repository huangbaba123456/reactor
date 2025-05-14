#ifndef NofityHandler_H_
#define NofityHandler_H_

#include "EventHandler.h"
#include <memory>
// 内部通信的Handler,用于唤醒reactor 处理内部的通信

typedef int Handle;
class NofityHandler:public EventHandler,public std::enable_shared_from_this<NofityHandler>{
private:
    Handle fd;
    NofityHandler(Handle fd); // 不希望直接创建对象
    // 连接建立的回调函数，这个函数会把新的连接加入reactor中 ，在reactor中被赋值，会被指向一个reactor的一个成员函数
public:
    static std::shared_ptr<NofityHandler> get_NofityHandler(Handle fd);
    virtual ~NofityHandler();
    virtual Handle get_handle() const override;
    virtual void handle_read() override;
    virtual void handle_write() override;
    virtual void handle_error() override;
    void set_fd(Handle fd){this->fd=fd;};
};



#endif // !NofityHandler_H_
