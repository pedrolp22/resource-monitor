//Memory data parsing module
#include <stdio.h>
#include <stdlib.h>
#include <sysmon.h>
#include <termbox2/termbox2.h>

void readMem(struct memData *data) {
    FILE *f1 = fopen("/proc/meminfo", "r");
    if(f1 == NULL) {
        tb_clear();
        tb_printf(0,0,TB_RED,TB_256_BLACK,"ERROR: Could not open /proc/meminfo\n");
        return;
    }
    char buffer[256];
    while (fgets(buffer, 256, f1) != NULL) {
        if(sscanf(buffer, "MemTotal: %ld kB", &data->memTotal) == 1) {
        }else if (sscanf(buffer, "MemAvailable: %ld kB", &data->memAvailable) == 1) {
        }else if (sscanf(buffer, "SwapTotal: %ld kB", &data->swapTotal) == 1) {
        }else if (sscanf(buffer, "SwapFree: %ld kB", &data->swapFree) == 1){
        }
    }
    fclose(f1);
}