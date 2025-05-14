#ifndef WORKERTHREAD_H_
#define WORKERTHREAD_H_
#include <memory>
#include <string>
#include <thread>
class Reactor;
class WorkerThread{
public:
    WorkerThread(const std::string& name,int timeout);
    ~WorkerThread();
    // 启动这个线程
    void WorkerThreadRun();
    std::shared_ptr<Reactor> get_reactor(){return this->reactorPtr;}
    void set_isExit(bool isExit){this->isExit_=isExit;}
    void notify_thread();
private:
    void ThreadFun();
    int timeout_;
    std::string threadName;
    std::thread::id threadId;
    std::shared_ptr<std::thread> threadPtr=nullptr; // 线程指针
    std::shared_ptr<Reactor> reactorPtr=nullptr; // 指向reactor的指针
    bool isExit_=false;
};

#endif // !WORKERTHREAD_

