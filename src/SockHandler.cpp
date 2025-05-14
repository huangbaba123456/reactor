#include "SockHandler.h"
#include "Buffer.h"
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <functional>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Event.h"
#include "Log.h"
#include "Reactor.h"
SockHandler::SockHandler(int bufferSize):readBuffer(new Buffer(bufferSize)),writeBuffer(new Buffer(bufferSize)){
}
SockHandler::~SockHandler(){
    LOG_INFO("SockHandler::~SockHandler()");
    if(sock_fd!=-1){
        close(sock_fd);
    }
}
Handle SockHandler::get_handle() const{
    return sock_fd;
}
void SockHandler::handle_read(){
    // 读数据
    if(sock_fd==-1){
        return; // 读个毛
    }
    // 开始读,配合边缘触发
    while(1){
        int readLen=readBuffer->bufferSocketRead(sock_fd); // 数据读进去
        if(readLen<0){
            // 关闭连接
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                // 数据读完啦
                break;
            }else{
                // 出问题了断开连接
                // reactor->addTask(std::bind(
                //     &Reactor::remove,reactor,shared_from_this()
                // ));
                reactor()->addTask([&](){
                    this->reactor()->remove(shared_from_this());
                });
                //reactor()->remove(shared_from_this()); // 直接断开
                break;
            }
        }else if(readLen==0){
            // 
            reactor()->addTask([&](){
                reactor()->remove(shared_from_this());
            });
            //reactor()->remove(shared_from_this()); // 不能直接断开
            LOG_DEBUG("连接断开");
            break;
        }
    }
    // 调用连接回调
    if(readBuffer->ableReadSize()>0){
        // 给用户
        std::string data=readBuffer->getBufferData();
        this->read_call_back(data.c_str(), data.size());
    }
}

void SockHandler::handle_write(){
    // 可以写数据啦，我们的目标是把 writeBuffer 中的数据写入fd中
    LOG_DEBUG("void SockHandler::handle_write()");
    // 非堵塞io
    bool ok=true;
    while (writeBuffer->ableReadSize()>0)
    {
        // 只要还能写就一直循环
        int writeLen=writeBuffer->bufferSocketWrite(this->get_handle());
        if(writeLen<=0){
            // 没法写了
            if(errno==EAGAIN || errno==EWOULDBLOCK){
                // 读缓冲区满了，退出等下次触发
                LOG_DEBUG("读缓冲区满了，退出等下次触发");
                break;
            }else{
                // 出问题啦,这个时候需要断开连接啦
                LOG_DEBUG("出问题啦,这个时候需要断开连接啦");
                reactor()->addTask([&](){
                    reactor()->remove(shared_from_this());
                });
                ok=false;
                //reactor()->remove(shared_from_this()); // 直接断开
                return;
            }
        }
    }
    // 判断是不是写完啦，如果写完了的话，就不需要再监听啦
    if(ok &&writeBuffer->ableReadSize()==0){
        // 取消写监听
        uint32_t curEvent=(EPOLL_EVENTS)this->event();
        // 取消写事件
        curEvent=(curEvent & (~EPOLLOUT));
        // 设置自己的事件
        this->set_event((Event)curEvent);
        reactor()->modify(shared_from_this(), this->event()); // 清除读事件
    }
}
void SockHandler::handle_error(){
    reactor()->addTask([&](){
                this->reactor()->remove(shared_from_this());
    });
    //reactor()->remove(shared_from_this());
}
void SockHandler::set_sock_fd(Handle sock_fd)  {
    this->sock_fd=sock_fd;
}

int SockHandler::write(const char* cs,int size){
    // 写数据
    if(cs==nullptr || size==0){
        LOG_ERROR("cs==nullptr || size==0");
        return 1;
    }
    //send(int __fd, const void *__buf, size_t __n, int __flags)
    this->writeBuffer->bufferAppend(cs, size);
    // 设置写监听
    // reactor()->addTask(
    //     [&](){
    //         this->reactor()->modify(shared_from_this(), Event((EPOLL_EVENTS)this->event()|EPOLLOUT));
    //     }
    // );
    this->set_event(Event((EPOLL_EVENTS)this->event()|EPOLLOUT));
    reactor()->modify(shared_from_this(),this->event());
    return size;
}
int SockHandler::write(const std::string& s){
    return write(s.data(),s.size());
}