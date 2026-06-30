//Header for rendering defs and UI colors
#ifndef UNTITLED_RENDER_H
#define UNTITLED_RENDER_H

#include <termbox2/termbox2.h>
#include <sysmon.h>

#define PROCSPACING 35

enum cpuColor { //cpu usage display colors
    bgcpu = TB_256_BLACK,
    fgcpu = 10
};

enum memColor { //memory usage display colors
    bgmem = TB_256_BLACK,
    fgmem = 13,
};

enum procColor { //processes display colors
    bgproc = TB_256_BLACK,
    fgproc = 202
};

enum clrRender{ //clear parameters
    bgclr = TB_256_BLACK,
    fgclr = 231
};

enum boxRender { //box outline color
    fgline = 202,
    bgline = TB_256_BLACK
};

enum usageColor { //render system usage as colorful bars
    fgbarneat = 12,
    fgbarok = 10,
    fgbarmod = 11,
    fgbarmodhigh = 202,
    fgbarcrit = 9
};

enum memUnit {
    MEGABYTES = 0,
    GIGABYTES = 1
};

void renderProcesses(struct procList *list, unsigned long int procLimit); // display running processes
void renderSystem(double cpu, struct memData *mem, int memUnit); // display cpu and memory usage with percentages and bars
void renderCpuBar(int x, int y, double cpu); // render CPU usage as a colored bar
void renderMemBar(int x, int y, double mem); // render memory usage as a colored bar
void renderLabel(int x, int y, const char *label, int color); // render bold label at top-left of box

#endif //UNTITLED_RENDER_H

