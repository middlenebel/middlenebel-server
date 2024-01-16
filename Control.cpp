

#include <iostream>
#include <fstream>
#include <unistd.h>
#include "sys/types.h"
#include "sys/sysinfo.h"

#include "inc/Control.hpp"

using namespace std;

bool ctr_debug = false;

Control::Control(){
    init();
}

void Control::init(){
    pages = sysconf(_SC_PHYS_PAGES);
    page_size = sysconf(_SC_PAGE_SIZE);
}

long Control::getTotalSystemMemory(){
    getValues();
    return pages * page_size;
}

long Control::getFreeMemory(){
    getValues();
    return freeMemory;
}

long Control::getAvailableMemory(){
    getValues();
    return availableMemory;
}

long Control::getUsedMemory(){
    getValues();
    return usedMemory;
}

void Control::getValues(){
    struct sysinfo memInfo;

    sysinfo (&memInfo);
    long long totalVirtualMem = memInfo.totalram;
    //Add other values in next statement to avoid int overflow on right hand side...
    totalVirtualMem += memInfo.totalswap;
    totalVirtualMem *= memInfo.mem_unit;

    //usedMemory = totalVirtualMem; https://stackoverflow.com/questions/8085830/how-to-remove-deleted-files-from-showing-up-in-my-local-git-status
    //long long physMemUsed = memInfo.totalram - memInfo.freeram;

    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    usedMemory = physMemUsed;

    /*
    int tSize = 0, resident = 0, share = 0;
    ifstream buffer("/proc/self/statm");
    buffer >> tSize >> resident >> share;
    buffer.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    double rss = resident * page_size_kb;
    //cout << "RSS - " << rss << " kB\n";

    double shared_mem = share * page_size_kb;
    //cout << "Shared Memory - " << shared_mem << " kB\n";

    //cout << "Private Memory - " << rss - shared_mem << "kB\n";
    */
}