
#include "NofityHandler.h"
#include "EventHandler.h"
#include <string>
#include <unistd.h>
#include "Log.h"
// 内部通信的Handler,用于唤醒reactor 处理内部的通信

NofityHandler::NofityHandler(Handle fd){
    this->fd=fd;
}

std::shared_ptr<NofityHandler> NofityHandler::get_NofityHandler(Handle fd){
    std::shared_ptr<NofityHandler> ptr(new NofityHandler(fd));
    return ptr;
}
NofityHandler::~NofityHandler(){
    close(this->fd);
}
Handle NofityHandler::get_handle() const{
    return this->fd;
}
void NofityHandler::handle_read(){
    // 全部读出来
    char buf[1024]={0};
    read(fd, buf, sizeof(buf)); // 读出来啥都不用做
}
void NofityHandler::handle_write(){

}
void NofityHandler::handle_error(){

}





