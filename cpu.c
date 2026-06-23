//CPU data parsing module
#include <stdio.h>
#include <sysmon.h>
#include <termbox2/termbox2.h>

void readCpu(struct cpuData *data){
    FILE *f1 = fopen("/proc/stat", "r");
    if(f1 == NULL) {
        tb_clear();
        tb_printf(0,0,TB_RED,TB_256_BLACK,"ERROR: Could not open /proc/stat\n");
        return;
    }
    int read = fscanf(f1,"cpu %ld %ld %ld %ld %ld %ld %ld", &data->usrTime, &data->niceTime, &data->sysTime,
                                                            &data->idleTime, &data->ioWait, &data->irq, &data->softirq);
    if (read != 7) {
        printf("ERROR: Failed to read /proc/stat\n");
        fclose(f1);
        return;
    }
    fclose(f1);
}

double calculateCpuUsage(const struct cpuData *prev, const struct cpuData *curr){
    unsigned long prevTotal = prev->usrTime + prev->niceTime + prev->sysTime + prev->idleTime
                     + prev->ioWait + prev->irq + prev->softirq;
    unsigned long prevIdle = prev->idleTime + prev->ioWait;

    unsigned long currTotal = curr->usrTime + curr->niceTime + curr->sysTime + curr->idleTime
                     + curr->ioWait + curr->irq + curr->softirq;
    unsigned long currIdle = curr->idleTime + curr->ioWait;

    unsigned long deltaTotal = currTotal - prevTotal;
    if (deltaTotal == 0) {
        return 0.0;
    }
    unsigned long deltaIdle = currIdle - prevIdle;

    unsigned long activeTime = deltaTotal - deltaIdle;

    double usage = (double) activeTime / (double) deltaTotal;

    return usage*100;
}