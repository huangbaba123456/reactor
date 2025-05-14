
#include "TcpServer.h"
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include "Log.h"
#include <thread>
using namespace std;

class myHandler : public SockHandler{
public:
    virtual void connection_call_back() override{
        LOG_INFO("virtual void connection_call_back()");
    }

    virtual void read_call_back(const char*data,int size) override{
        std::cout << string(data,size);
        write(data,size);
    }
};
class myServer:public TcpServer{
public:
    virtual std::shared_ptr<SockHandler> CreateServiceHandle() override{
        return make_shared<myHandler>();
    };
    virtual void connection_establish(unsigned short port,const char* ip) override{
        char buf[128]={0};
        sprintf(buf, "客户端: %s:%d 建立连接", ip,port);
        LOG_INFO(buf);
    }    

};
int main(){
    
    // TcpServer server;
    // server.set_timeout(5000);
    // server.registListenPort(10020);
    // server.start();
    myServer server;
    server.set_timeout(-1);
    server.set_threadNum(thread::hardware_concurrency()*2+1);
    server.registListenPort(10020);
    server.start();
    return 0;
}