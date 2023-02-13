#include <stdio.h>
#include "utils.h"
#include "arguments.h"

#if DEBUG
#include "test.h"
#endif

int main(int argc, char* argv[])
{
#if DEBUG
    // For test functions only
    test_brutforce();
    //coni();
    return 0;
#else
    ArgOpt argopt = getopt_Parse(argc, argv);
    
    double deltav = 0;
    double twrmin = 0;
    double twrmax = 0;
    if (argopt.deltav)
        deltav = atof(argopt.deltav);
    else if (argopt.twrmin)
        twrmin = atof(argopt.twrmin);
    else if (argopt.twrmax)
        twrmax = atof(argopt.twrmax);

    return 0;
#endif
}
