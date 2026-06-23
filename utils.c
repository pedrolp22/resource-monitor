// General utils module
#include <ctype.h>
#include "include/sysmon.h"

bool stringIsNumeric(char *str) {
    if (str == NULL || *str == '\0') {
        return false;
    }

    while (*str != '\0') {
        if (!isdigit((unsigned char)*str)) {
            return false;
        }
        str++;
    }

    return true;
}