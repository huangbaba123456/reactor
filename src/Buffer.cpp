#include "Buffer.h"
#include <cstdlib>
#include <cstring>
#include <ios>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include "Log.h"
#include <sys/uio.h>
#include <sys/socket.h>

using namespace  std;

Buffer::Buffer(int size){
    this->data=(char*)malloc(size);
    this->capacity=size;
    this->bufferReset();
}
Buffer::~Buffer(){
    free(this->data);
}
// 还能够写多少字节
int Buffer::ableWriteSize(){
    return this->capacity-this->writePos;
}
// 还能读多少字节
int Buffer::ableReadSize(){
    return this->writePos-this->readPos;
}
// 扩容函数 .保证剩余可写的字节数大于等于size
bool Buffer::bufferExtendRoom(int size){
    if(this->ableWriteSize()>=size){
        return true;
    }
    if(this->readPos+this->ableWriteSize()>=size){
        int readAble=this->ableReadSize();
        memcpy(this->data, this->data+this->readPos, readAble);
        this->readPos=0;
        this->writePos=readAble;
        return true;
    }else{
        char *tmp=(char*)realloc(this->data, this->capacity+size);
        if(!tmp){
            return false;
        }
        this->data=tmp;
        this->capacity=this->capacity+size;
        return true;
    }
}
// 写数据到缓存区
bool Buffer::bufferAppend(const char* data,int size){
    if(!bufferExtendRoom(size)){
        LOG_ERROR("扩容失败");
        return false; // 扩容失败
    }
    memcpy(this->data+this->writePos, data, size); // 写进去
    this->writePos+=size;
    return true;
}
// 从fd中读数据出来进入缓存区
int Buffer::bufferSocketRead(int fd){
    struct iovec vec[2];
    memset(&vec, 0, sizeof(vec));
    int writeAble=this->ableWriteSize(); // 还能够写入缓冲区多少
    vec[0].iov_base=this->data+this->writePos;
    vec[0].iov_len=writeAble;
    char* tmpBuf=(char*)malloc(40960);
    vec[1].iov_base=tmpBuf;
    vec[1].iov_len=40960;
    int readLen=readv(fd,vec,2);
    if(readLen==-1){
        free(tmpBuf);
        return -1;
    }else if(readLen<=writeAble){
        this->writePos+=readLen; // 没有溢出
    }else{
        this->writePos=this->capacity; // 读满啦
        bool ok=bufferAppend(tmpBuf, readLen-writeAble);
        if(!ok){
            LOG_ERROR("被迫丢弃了一些数据");
            return writeAble;
        }
    }
    return readLen;
}

int Buffer::bufferSocketWrite(int fd){
    int readAble=this->ableReadSize();
    if(readAble>0){
        // 发送数据
        int writeLen=send(fd,this->data+this->readPos,readAble,0);
        if(writeLen>0){
            this->readPos+=writeLen;
        }
        return writeLen;
    }
    return 0; // 没有要写的了
}

// 重置函数 
void Buffer::bufferReset(){
    this->writePos=this->readPos=0;
}
std::string Buffer::getBufferData(){
    std::string msg=std::string(data+readPos,ableReadSize());
    bufferReset();
    return msg;
}