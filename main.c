#include <locale.h>
#include <render.h>
#include <sysmon.h>

#define TB_IMPL


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

// todo: fix crashes and system slowdown
int main(int argc, char **argv) {
    setlocale(LC_ALL, "");
    //initializing data structures
    struct procList list;
    initProcList(&list);
    struct cpuData prev, curr;
    readCpu(&prev);
    struct memData mem;
    int memDisplayUnit = MEGABYTES;

    //initializing termbox2
    if (tb_init() != 0) return 1;
    tb_set_output_mode(TB_OUTPUT_256);
    tb_set_clear_attrs(fgclr,bgclr);

    struct tb_event ev = {0};

    while (!(ev.key == TB_KEY_CTRL_Q || ev.key == TB_KEY_ESC)){
        unsigned long int plimit = 20;
        tb_clear(); //clear screen at start

        double cpuUsage = updateCpu(&prev, &curr);
        readMem(&mem);
        updateProcesses(&list);

        renderSystem(cpuUsage, &mem, memDisplayUnit);
        renderProcesses(&list, plimit);
        tb_present(); //show render

        tb_peek_event(&ev, 100); //detect input
        
        if (ev.key == 'm' || ev.key == 'M') {
            memDisplayUnit = (memDisplayUnit == MEGABYTES) ? GIGABYTES : MEGABYTES;
        }
    }

    //free memory and shutdown termbox2
    freeProcList(&list);
    tb_shutdown();
    return 0;
}