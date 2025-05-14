#ifndef BUFFER_H_
#define BUFFER_H_
#include <string>
// 缓冲区

class Buffer{
public:
    Buffer(int size);
    ~Buffer();
    // 还能够写多少字节
    int ableWriteSize();
    // 还能读多少字节
    int ableReadSize();
    // 扩容函数 .保证剩余可写的字节数大于等于size
    bool bufferExtendRoom(int size);
    // 写数据到缓存区
    bool bufferAppend(const char* data,int size);
    // 从fd中读数据出来进入缓存区
    int bufferSocketRead(int fd);
    // 把缓存区里面的数据读出来，然后写入fd中
    int bufferSocketWrite(int fd);
    // 重置函数 
    void bufferReset();
    std::string getBufferData();
private:
    char* data;
    int capacity;
    int readPos;
    int writePos;
};


#endif // !BUFFER_H_