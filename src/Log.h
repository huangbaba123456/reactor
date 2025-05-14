#include<iostream>
#include <string>
#include <unordered_map>
#define LOG_DEBUG(msg) Log::LOG_PRINT("DEBUG",msg,__FILE__,__LINE__)
#define LOG_INFO(msg) Log::LOG_PRINT("INFO",msg,__FILE__,__LINE__)
#define LOG_ERROR(msg) Log::LOG_PRINT("ERROR",msg,__FILE__,__LINE__)
#define LOG_FATAL(msg) Log::LOG_PRINT("FATAL",msg,__FILE__,__LINE__)


// 日志类
#define CUR_LEVEL "DEBUG"
class Log{
private:
    Log(){}
    static std::unordered_map<std::string, int> valMap;
public:
    static void LOG_PRINT(const std::string& level,const std::string msg,const char* filename,int line);
};

