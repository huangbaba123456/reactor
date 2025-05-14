#include "TcpServer.h"
#include "Event.h"
#include "EventHandler.h"
#include "ListenHandle.h"
#include "Reactor.h"
#include <arpa/inet.h>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <unistd.h>
#include <vector>
#include "Log.h"
#include "ThreadPool.h"
#include <sys/epoll.h>
int TcpServer::registListenPort(unsigned short port){
    // 设置
    std::shared_ptr<ListenHandle> ptr=ListenHandle::get_ListenHandle();
    ptr->set_connectionEstablish_callback(
        std::bind(&TcpServer::add_cilent,this,std::placeholders::_1, std::placeholders::_2)
    );
    ptr->listenOn(port);
    // 监听读事件
    ptr->set_event(Event::ReadEvent);
    ptr->set_reactor(Reactor::get_instance());
    return Reactor::get_instance()->regist(ptr, ptr->event());
}
void TcpServer::set_threadNum(int threadnum){
    this->pool.set_threadNum(threadnum); // 设置线程池的数量
}



bool TcpServer::add_cilent(Handle client_fd,sockaddr_in client_addr){
    // 有新的连接需要被建立
    uint16_t port = ntohs(client_addr.sin_port);
    char* ip = inet_ntoa(client_addr.sin_addr); // 不安全，可能被其他线程覆盖
    this->connection_establish(port, ip);
    // char buf[128]={0};
    // sprintf(buf, "客户端: %s:%d 建立连接", ip,port);
    // LOG_INFO(buf);
    // 创建一个Handle 建立通讯
    //close(client_fd);
    std::shared_ptr<SockHandler> clientHandler=CreateServiceHandle();
    // 监听读事件
    clientHandler->set_sock_fd(client_fd); // 设置fd
    clientHandler->set_event( (Event)((EPOLL_EVENTS)Event::ReadEvent | (EPOLL_EVENTS)Event::ErrorEvent)); // 监听读事件
    // 加入任务
    //LOG_DEBUG(std::to_string(clientHandler.use_count()));
    //Reactor::get_instance()->addTask(std::bind(&Reactor::regist,Reactor::get_instance(),clientHandler,Event::ReadEvent));
    // 这里采坑了，clientHandler 必须直接传递，不然会自动释放
    // Reactor::get_instance()->addTask(
    //     [&,clientHandler](){
    //         Reactor::get_instance()->regist(clientHandler,clientHandler->event());
    //         clientHandler->connection_call_back(); // 连接回调函数
    //     }
    // );
    // 重线程池选择一个线程出来
    std::shared_ptr<Reactor> reator=pool.getNext_WorkerThreadReactor();
    clientHandler->set_reactor(reator.get());
    // clientHandler->set_reactor(Reactor::get_instance());
    clientHandler->reactor()->addTask(
        [&,clientHandler](){
            clientHandler->reactor()->regist(clientHandler,clientHandler->event());
            clientHandler->connection_call_back(); // 连接回调函数
        }
    );
    //clientHandler->reactor()->regist(clientHandler,clientHandler->event());
    return true;
}
void TcpServer::start(){
    this->pool.run(timeout_); // 开始跑
    while(1){
        Reactor::get_instance()->event_loop(timeout_);
    }
}
