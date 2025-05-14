
#include "ListenHandle.h"
#include "Log.h"
#include "Reactor.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
using namespace std;
void setReusePort(int fd) {
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        LOG_ERROR(string(" setsockopt(SO_REUSEPORT) failed:  ")+strerror(errno));
    }
}
std::shared_ptr<ListenHandle> ListenHandle::get_ListenHandle(){
    //return make_shared<ListenHandle>();
    shared_ptr<ListenHandle> ptr(new ListenHandle());
    return ptr;
}

ListenHandle::ListenHandle(){
    listenfd=socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0){
        char buf[128]={0};
        sprintf(buf, "create socket failed: %s", strerror(errno));
        LOG_FATAL(buf);
        exit(0);
    }
    setReusePort(listenfd);
}
ListenHandle::~ListenHandle(){
    close(listenfd);
}
Handle ListenHandle::get_handle() const{
    return listenfd;
}
void ListenHandle::set_connectionEstablish_callback(ConnectionEstablish_callback connectionEstablish_callback){
    this->connectionEstablish_callback=connectionEstablish_callback;
}


void ListenHandle::handle_read(){
    sockaddr_in client_addr={0};
    socklen_t client_len=sizeof(client_addr);
    int client_fd=accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
    if(client_fd<0){
        LOG_ERROR("accept error");
        return;
    }else{
        if(this->connectionEstablish_callback){
            this->setNonBlocking(client_fd); // 设置这个非堵塞模式
            this->connectionEstablish_callback(client_fd,client_addr);
        }else{
            LOG_ERROR("connectionEstablish_callback is null, close connection");
            close(client_fd);
        }
    }
}
void ListenHandle::handle_write(){

}
void ListenHandle::handle_error(){
    // 可以结束监听了
    LOG_FATAL("void SockHandler::handle_write()");
    reactor()->addTask(
        [&](){
                this->reactor()->remove(shared_from_this());
    });
    exit(0);
}
bool ListenHandle::listenOn(int port){
    sockaddr_in server_addr={0};
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY; // 本地的任何地址
    server_addr.sin_port=htons(port);
    if(bind(listenfd, (const struct sockaddr *)&server_addr, sizeof(server_addr))<0){
        char buf[128]={0};
        sprintf(buf, "bind failed: %s", strerror(errno));
        LOG_FATAL(buf);
        close(listenfd);
        return false;    
    }
    listen(listenfd,10);
    return true;

}
void ListenHandle::setNonBlocking(int fd){
    int flags=fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL,flags|O_NONBLOCK);
}