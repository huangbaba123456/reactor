#include "WorkerThread.h"
#include "Reactor.h"
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>
#include "Log.h"
using namespace std;

WorkerThread::WorkerThread(const std::string& name,int timeout_)
:threadName(name),timeout_(timeout_){

}
WorkerThread::~WorkerThread(){
    if(this->threadPtr && this->threadPtr->joinable()){
        this->threadPtr->join();
    }
}
// 启动这个线程
void WorkerThread::WorkerThreadRun(){
    //this->reactorPtr=std::shared_ptr<Reactor>(new Reactor());
    this->reactorPtr=std::shared_ptr<Reactor>(new Reactor());
    this->threadPtr=make_shared<thread>(&WorkerThread::ThreadFun,this);
}
void WorkerThread::notify_thread(){
    if(this->reactorPtr){
        this->reactorPtr->nofity_reactor();
    }
}

void WorkerThread::ThreadFun(){
    threadId=this_thread::get_id();
    while(!isExit_){
        this->reactorPtr->event_loop(timeout_);
    }
}

