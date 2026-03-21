// General utils module
#include <ctype.h>
#include "sysmon.h"

bool stringIsNumeric(char *str){
    if (*str == '\0') {
        return false;
    }
    if (!isdigit((unsigned char)*str)){
        return false;
    }
    if(*(str+1) == '\0') {
        return true;
    }
    return stringIsNumeric(str+1);
}