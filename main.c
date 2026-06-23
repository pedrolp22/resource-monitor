#include <stdio.h>
#include <unistd.h>
#include <sysmon.h>

#define TB_IMPL
#include <termbox2/termbox2.h>


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
    tb_printf(0,0,fgcpuneat,bgcpu, "CPU: %.2f%%\n", cpu);
    tb_printf(0, 1,fgmem,bgcpu,"Memory: %ld / %ld kB\n", mem->memAvailable, mem->memTotal);
}

static void renderProcesses(const struct procList *list, unsigned long int procLimit) {
    tb_printf(0, 3, fgproc, bgproc, "PID");
    tb_printf(15, 3, fgproc, bgproc, "STATE");
    tb_printf(30, 3, fgproc, bgproc, "NAME");
    size_t count = procLimit < list->nprocs ? procLimit : list->nprocs;
    tb_printf(45,3,fgproc,bgproc,"(Showing %zu of %zu)\n", procLimit, list->nprocs);


    for (size_t i = 0; i < count; i++) {
        tb_printf(0,4+i,fgproc,bgproc,"\n%d", list->procs[i].pid);
        tb_printf(15, 4+i, fgproc, bgproc, "%c", list->procs[i].pstate);  // <-- pstate is char, not string
        tb_printf(30, 4+i, fgproc, bgproc, "%s\n", list->procs[i].pname);
    }
}

int main(int argc, char **argv) {
    //initializing data structures
    unsigned long int plimit = 20;
    struct procList list;
    initProcList(&list);
    struct cpuData prev, curr;
    readCpu(&prev);
    struct memData mem;

    //initializing termbox2
    if (tb_init() != 0) return 1;  // <-- check tb_init failure
    tb_set_output_mode(TB_OUTPUT_256);
    tb_set_clear_attrs(fgclr,bgclr);

    struct tb_event ev = {0};  // <-- zero-initialize before first loop check

    while (!(ev.key == TB_KEY_CTRL_Q || ev.key == TB_KEY_ESC)){
        tb_clear(); //clear screen at start

        double cpuUsage = updateCpu(&prev, &curr);
        readMem(&mem);
        updateProcesses(&list);

        renderSystem(cpuUsage, &mem);
        renderProcesses(&list, plimit);
        tb_present(); //show render

        tb_peek_event(&ev, 200); //detect input
        if (ev.key == TB_KEY_CTRL_P) {
            plimit += 20;
        }
    }

    //free memory and shutdown termbox2
    freeProcList(&list);
    tb_shutdown();
    return 0;
}