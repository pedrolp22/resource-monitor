// Process data parsing and list management module
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sysmon.h"

void initProcList(struct procList *list) {
    if (list == NULL) {
        return;
    }
    list->nprocs = 0;
    list->listsize = 64;
    list->procs = malloc(list->listsize * sizeof(struct procData));
    if (list->procs == NULL) {
        printf("ERROR: malloc failed\n");
        list->listsize = 0;
    }
}

int updateProcList(struct procList *list) {
    DIR *d = opendir("/proc");
    if (d == NULL) {
        printf("ERROR: Could not open /proc\n");
        return -1;
    }
    struct dirent *entry;
    while ((entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        if (stringIsNumeric(entry->d_name)) {
            if (list->nprocs == list->listsize) {
                size_t newsize = list->listsize * 2;
                struct procData *tmp = realloc(list->procs, newsize * sizeof(struct procData));
                if (tmp == NULL) {
                    printf("ERROR: realloc failed\n");
                    closedir(d);
                    return -1;
                }
                list->procs = tmp;
                list->listsize = newsize;
            }
            size_t id = list->nprocs;

            list->procs[id].pid = atoi(entry->d_name);
            char path[256];
            snprintf(path, sizeof(path), "/proc/%d/stat", list->procs[id].pid);
            FILE *f = fopen(path, "r");
            if (f == NULL) {
                continue;
            }
            char buffer[1024];
            if ((fgets(buffer, sizeof(buffer), f) == NULL)) {
                continue;
            }
            int i = 0;
            while (buffer[i] != '\0' && buffer[i] != '(') {
                i++;
            }
            if (buffer[i] == '\0') {
                fclose(f);
                continue;
            }
            i++;
            int j = 0;
            while (buffer[i] != '\0' && buffer[i] != ')' && j < 99) {
                list->procs[id].pname[j] = buffer[i];
                i++;
                j++;
            }
            list->procs[id].pname[j] = '\0';
            if (buffer[i] == ')') {
                if (buffer[i + 2] != '\0') {
                    list->procs[id].pstate = buffer[i + 2];
                } else {
                    list->procs[id].pstate = '?';
                }
            } else {
                list->procs[id].pstate = '?';
            }
            list->nprocs++;
        }
    }
    closedir(d);
    return 0;
}

void freeProcList(struct procList *list) {
    if (list == NULL) {
        return;
    }
    free(list->procs);
    list->procs = NULL;
    list->nprocs = 0;
    list->listsize = 0;
}