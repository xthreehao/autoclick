#include <iostream>
#include "head.h"

int main (int argc , char *argv[])
{
    Log log;
    log.logINFO("已成功启动");
    ui(argc , argv);
    return 0;
}