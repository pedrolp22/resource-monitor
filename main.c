#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sysmon.h"

static double updateCpu(struct cpuData *prev, struct cpuData *curr) {
    readCpu(curr);
    double usage = calculateCpuUsage(prev, curr);
    *prev = *curr;
    return usage;
}

static void updateProcesses(struct procList *list) {
    list->nprocs = 0;
    updateProcList(list);
}

static void renderSystem(double cpu, const struct memData *mem) {
    printf("CPU: %.2f%%\n", cpu);
    printf("Memory: %ld / %ld kB\n", mem->memAvailable, mem->memTotal);
}

static void renderProcesses(const struct procList *list, long unsigned int procLimit) {
    printf("\nPID\tSTATE\tNAME\n (%zu of %zu)", procLimit, list->nprocs);
    for (size_t i = 0; i < procLimit; i++) {
        printf("%d\t%c\t%s\n",
               list->procs[i].pid,
               list->procs[i].pstate,
               list->procs[i].pname);
    }
}

int main(void) {
    struct procList list;
    initProcList(&list);

    struct cpuData prev, curr;
    readCpu(&prev);

    struct memData mem;

    while (1) {
        usleep(500000);
        system("clear");

        double cpuUsage = updateCpu(&prev, &curr);
        readMem(&mem);
        updateProcesses(&list);

        renderSystem(cpuUsage, &mem);
        renderProcesses(&list, 20);
    }

    freeProcList(&list);
    return 0;
}