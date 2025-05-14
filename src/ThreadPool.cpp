#include "ThreadPool.h"
#include "Reactor.h"
#include "WorkerThread.h"
#include <cstdio>
#include <memory>


ThreadPool::ThreadPool(int threadNum):threadNum(threadNum){

}
ThreadPool::~ThreadPool(){
    // 这里我们退出所有的线程
    for(auto& p:this->workerThreadArr){
        p->set_isExit(true);
        p->notify_thread(); // 唤醒线程退出
    }
}
// 启动线程池里面所有的线程
void ThreadPool::run(int timeout){
    // 启动所有的线程
    for (int i=0; i<this->threadNum; i++) {
        char buf[64]={0};
        sprintf(buf, "thread_%d",i+1);
        this->workerThreadArr.emplace_back(new WorkerThread(buf,timeout));
        this->workerThreadArr.back()->WorkerThreadRun();
    }
} 
// 轮询选择一个reactor
std::shared_ptr<Reactor> ThreadPool::getNext_WorkerThreadReactor(){
    std::shared_ptr<Reactor> ans=this->workerThreadArr[this->index]->get_reactor();
    this->index++;
    this->index%=this->threadNum;
    return ans;
} 
