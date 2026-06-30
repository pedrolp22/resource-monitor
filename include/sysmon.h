#ifndef SYSMON_H
#define SYSMON_H
#include <stddef.h>
#include <termbox2/termbox2.h>

// CPU
struct cpuData{
    long sysTime;
    long usrTime;
    long idleTime;
    long niceTime;
    long ioWait;
    long irq;
    long softirq;
};

void readCpu(struct cpuData *data);
double calculateCpuUsage(const struct cpuData *prev, const struct cpuData *curr);

// Memory
struct memData {
    long memTotal;
    long memAvailable;
    long swapTotal;
    long swapFree;
};

void readMem(struct memData *data);

// Processes
struct procData{
    int pid;
    char pname[100];
    char pstate;
};

struct procList {
    struct procData *procs;
    size_t nprocs; //number of processes
    size_t listsize; //total list size
};

void initProcList(struct procList *list);
int updateProcList(struct procList *list);
void freeProcList(struct procList *list);

// Utilities
bool stringIsNumeric(char *str);
#endif
