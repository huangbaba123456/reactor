#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <memory>
#include <vector>
class WorkerThread;
class Reactor;
class ThreadPool{
public:
    explicit ThreadPool(int threadNum=4); // 线程池的数量
    ~ThreadPool();
    // 启动线程池里面所有的线程
    void set_threadNum(int threadNum){this->threadNum=threadNum;}
    void run(int timeout); 
    // 轮询选择一个reactor
    std::shared_ptr<Reactor> getNext_WorkerThreadReactor();    
private:
    int index=0;
    int threadNum;
    std::vector<std::shared_ptr<WorkerThread>> workerThreadArr; 
};

#endif // !THREADPOOL_H_
