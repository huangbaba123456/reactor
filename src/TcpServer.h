#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include<memory>
#include "SockHandler.h"
#include "ThreadPool.h"
typedef int Handle;
struct sockaddr_in;
class ListenHandle;

class TcpServer{
public:
    TcpServer(){};     
    int registListenPort(unsigned short port);
    void set_threadNum(int threadnum);
    void start();
    void set_timeout(int timeout){timeout_=timeout;};
    int timout();
    // 工厂模式生成实例化对象
    virtual std::shared_ptr<SockHandler> CreateServiceHandle()=0;
    virtual void connection_establish(unsigned short,const char* ip)=0;
private:
    bool add_cilent(Handle,sockaddr_in);
    int timeout_=-1;
    ThreadPool pool;
    
};

#endif // !TCPSERVER_H_
