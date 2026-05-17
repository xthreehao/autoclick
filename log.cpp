#include <iostream>
#include <ctime>
#include "head.h"

void Log::logERROR(const char* a){
    time_t now = time(0);
    tm *localTime = localtime(&now);
    std::cout<<'['<<localTime->tm_mon<<"."<<localTime->tm_mday<<']'<<localTime->tm_hour<<':'<<localTime->tm_min<<':'<<localTime->tm_sec<<" ERROR:"<<a<<std::endl;
}

void Log::logWARNING(const char* a){
    time_t now = time(0);
    tm *localTime = localtime(&now);
    std::cout<<'['<<localTime->tm_mon<<"."<<localTime->tm_mday<<']'<<localTime->tm_hour<<':'<<localTime->tm_min<<':'<<localTime->tm_sec<<" WARNING:"<<a<<std::endl;
}

void Log::logINFO(const char* a){
    time_t now = time(0);
    tm *localTime = localtime(&now);
    std::cout<<'['<<localTime->tm_mon<<"."<<localTime->tm_mday<<']'<<localTime->tm_hour<<':'<<localTime->tm_min<<':'<<localTime->tm_sec<<" INFO:"<<a<<std::endl;
}
