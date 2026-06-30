#include <sysmon.h>
#include <render.h>
#include <string.h>

static void renderBox(size_t x, size_t y, size_t w, size_t h) {
    // Box has to be at least 2x2 to draw corners
    if (w < 2 || h < 2) return;

    // Unicode codepoints for heavy box drawing characters
    uint32_t tl = 0x250F; // ┏
    uint32_t tr = 0x2513; // ┓
    uint32_t bl = 0x2517; // ┗
    uint32_t br = 0x251B; // ┛
    uint32_t hline = 0x2501; // ━
    uint32_t vline = 0x2503; // ┃

    // Top edge
    tb_set_cell((int)x, (int)y, tl, fgline, bgline);
    for (size_t i = 1; i + 1 < w; ++i)
        tb_set_cell((int)(x + i), (int)y, hline, fgline, bgline);
    tb_set_cell((int)(x + w - 1), (int)y, tr, fgline, bgline);

    // Sides
    for (size_t j = 1; j + 1 < h; ++j) {
        tb_set_cell((int)x, (int)(y + j), vline, fgline, bgline);
        tb_set_cell((int)(x + w - 1), (int)(y + j), vline, fgline, bgline);
    }

    // Bottom edge
    tb_set_cell((int)x, (int)(y + h - 1), bl, fgline, bgline);
    for (size_t i = 1; i + 1 < w; ++i)
        tb_set_cell((int)(x + i), (int)(y + h - 1), hline, fgline, bgline);
    tb_set_cell((int)(x + w - 1), (int)(y + h - 1), br, fgline, bgline);
}

void renderLabel(int x, int y, const char *label, int color) {
    for (int i = 0; label[i] != '\0' && i < 20; i++) {
        tb_set_cell(x + i, y, label[i], color | TB_BOLD, bgline);
    }
}

void renderCpuBar(int x, int y, double cpu) {
    int barSize = 0;
    if (cpu <= 25) { barSize = 1; }
    else if (cpu <= 50) { barSize = 2; }
    else if (cpu <= 75) { barSize = 4; }
    else { barSize = 8; }
    
    for (int i = 0; i < 8; i++) {
        int color = TB_256_BLACK;
        if (i < barSize) {
            if (i < 2) { color = fgbarok; }
            else if (i < 4) { color = fgbarmod; }
            else if (i < 6) { color = fgbarmodhigh; }
            else { color = fgbarcrit; }
        }
        tb_set_cell(x + i, y, 0x2588, color, bgcpu);
    }
}

void renderMemBar(int x, int y, double mem) {
    int barSize = 0;
    if (mem <= 25) { barSize = 1; }
    else if (mem <= 50) { barSize = 2; }
    else if (mem <= 75) { barSize = 4; }
    else { barSize = 8; }
    
    for (int i = 0; i < 8; i++) {
        int color = TB_256_BLACK;
        if (i < barSize) {
            if (i < 2) { color = fgbarok; }
            else if (i < 4) { color = fgbarmod; }
            else if (i < 6) { color = fgbarmodhigh; }
            else { color = fgbarcrit; }
        }
        tb_set_cell(x + i, y, 0x2588, color, bgcpu);
    }
}

/*todo: add multiple pages for running processes
        improve process visibility */
void renderSystem(double cpu, struct memData *mem, int memUnit) {
    // Draw info box at top-left
    int term_w = tb_width();
    size_t box_x = 0;
    size_t box_y = 0;
    size_t box_w = (term_w > 30) ? 30 : (size_t)term_w;
    size_t box_h = 4;

    if (box_w >= 2) renderBox(box_x, box_y, box_w, box_h);

    // Calculate memory percentage
    double memPercent = (mem->memTotal > 0) ? ((mem->memTotal - mem->memAvailable) * 100.0 / mem->memTotal) : 0.0;

    // Print CPU info with percentage
    tb_printf((int)(box_x + 1), (int)(box_y + 1), fgcpu, bgcpu, "CPU: %.2f%% ", cpu);

    // Print memory info with percentage (tighter layout)
    tb_printf((int)(box_x + 9), (int)(box_y + 1), fgmem, bgcpu, " Memory: %.2f%%", memPercent);

    // Print memory absolute values
    long memUsed = mem->memTotal - mem->memAvailable;
    if (memUnit == MEGABYTES) {
        tb_printf((int)(box_x + 11), (int)(box_y + 2), fgmem, bgcpu, " %ld / %ld MB", memUsed / 1024, mem->memTotal / 1024);
    } else {
        tb_printf((int)(box_x + 11), (int)(box_y + 2), fgmem, bgcpu, "%.2f / %.2f GB", memUsed / 1024.0 / 1024.0, mem->memTotal / 1024.0 / 1024.0);
    }

    // Draw CPU bar box
    size_t cpu_box_x = 0;
    size_t cpu_box_y = box_y + box_h;
    size_t cpu_box_w = box_w;
    size_t cpu_box_h = 3;

    if (cpu_box_w >= 2) renderBox(cpu_box_x, cpu_box_y, cpu_box_w, cpu_box_h);
    renderLabel((int)cpu_box_x + 1, (int)cpu_box_y, "CPU", fgline);
    renderCpuBar((int)(cpu_box_x + 6), (int)(cpu_box_y + 1), cpu);

    // Draw Memory bar box
    size_t mem_box_x = 0;
    size_t mem_box_y = cpu_box_y + cpu_box_h;
    size_t mem_box_w = box_w;
    size_t mem_box_h = 3;

    if (mem_box_w >= 2) renderBox(mem_box_x, mem_box_y, mem_box_w, mem_box_h);
    renderLabel((int)mem_box_x + 1, (int)mem_box_y, "MEM", fgline);
    renderMemBar((int)(mem_box_x + 6), (int)(mem_box_y + 1), memPercent);
}

void renderProcesses(struct procList *list, unsigned long int procLimit) {
    int pageNum = 1;

    // Calculate box that will contain the processes table
    int term_w = tb_width();
    size_t box_x = (PROCSPACING >= 2) ? (size_t)(PROCSPACING - 2) : 0; // give a small left padding
    size_t box_y = 0;
    size_t count = procLimit < list->nprocs ? procLimit : list->nprocs;
    size_t box_h = (count + 2) < 2 ? 2 : (count + 2); // header + rows + borders
    size_t box_w = (term_w > (int)box_x) ? (size_t)(term_w - (int)box_x) : 2;

    if (box_w >= 2 && box_h >= 2) renderBox(box_x, box_y, box_w, box_h);

    // Offset content to be inside the box: header on row box_y+1, rows start at box_y+2
    tb_printf(PROCSPACING, (int)(box_y + 1), bgproc, fgproc, "PID");
    tb_printf((int)(PROCSPACING * 1.25), (int)(box_y + 1), bgproc, fgproc, "STATE");
    tb_printf((int)(PROCSPACING * 1.5), (int)(box_y + 1), bgproc, fgproc, "NAME");
    tb_printf((int)(PROCSPACING * 2), (int)(box_y + 1), bgproc, fgproc, "Page %d (Showing %zu of %zu)",
        pageNum, procLimit, list->nprocs);

    for (size_t i = 0; i < count; i++) {
        tb_printf(PROCSPACING, (int)(box_y + 2 + i), fgproc, bgproc, "%d", list->procs[i].pid);
        tb_printf((int)(PROCSPACING * 1.25), (int)(box_y + 2 + i), fgproc, bgproc, "%c", list->procs[i].pstate);
        tb_printf((int)(PROCSPACING * 1.5), (int)(box_y + 2 + i), fgproc, bgproc, "%s", list->procs[i].pname);
    }
}
