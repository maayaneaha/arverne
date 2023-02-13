#pragma once
#include <stddef.h>

struct ArgOpt_s {
    // char* == type 1 (with value)
    // int == type 2 (flag)
    char* deltav;
    char* twrmin;
    char* twrmax;
}; 

typedef struct ArgOpt_s ArgOpt;

typedef struct {
    void* value;
    char* shortname;
    char* longname;
    int type; // 0 == "with value"; 1 == "flag"
    int required;
    char* desc;
} Opt;

ArgOpt getopt_Parse(int argc, char* argv[]);
void getopt_CheckRequired(ArgOpt argopt);
