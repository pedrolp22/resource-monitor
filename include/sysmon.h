#ifndef SYSMON_H
#define SYSMON_H
#include <stddef.h>
#include <termbox2/termbox2.h>

//render configs for termbox2
//todo: make cpu render different colors for usage %
enum cpuRender {
    bgcpu = TB_256_BLACK,
    fgcpuneat = 12,
    fgcpuok = 10,
    fgcpumod = 11,
    fgcpumodhigh = 202,
    fgcpucrit = 9
};

enum memRender {
    bgmem = TB_256_BLACK,
    fgmem = 13,
};

enum procRender {
    bgproc = TB_256_BLACK,
    fgproc = 220
};

enum clrRender{
    bgclr = TB_256_BLACK,
    fgclr = 231
};

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
