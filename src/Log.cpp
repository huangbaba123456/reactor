#include "Log.h"
#include <string>
#include <utility>

using LOGPAIR=std::pair<std::string, int>;
std::unordered_map<std::string, int> Log::valMap={LOGPAIR("DEBUG",3),LOGPAIR("INFO",2),LOGPAIR("ERROR",1),LOGPAIR("FATAL",0)};

void Log::LOG_PRINT(const std::string& level,const std::string msg,const char* filename,int line){
    int curSetVal=valMap[CUR_LEVEL];
    int curVal=valMap[level];
    if(curVal<=curSetVal){
        std::cout<<"[ "<<filename<<" ]"<<" [ "<<line<<" ] "<<level<<" : "<<msg<<std::endl; 
    }
}