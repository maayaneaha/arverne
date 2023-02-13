#include <stdio.h>
#include "utils.h"
#include "algo/brutforce.h"
#include "basic_display/basic_display.h"
#include "arguments.h"
#include "test/test.h"

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
    
    double deltav = 2100;
    double twrmin = 1.4;
    double twrmax = 1.8;
    if (argopt.deltav)
        deltav = atof(argopt.deltav);
    else if (argopt.twrmin)
        twrmin = atof(argopt.twrmin);
    else if (argopt.twrmax)
        twrmax = atof(argopt.twrmax);
    Datas *d = create_datas();
    d->deltaV_min = deltav;
    d->TWR_min = twrmin;
    d->TWR_max = twrmax;
    int r = brut_force(d);
    basic_display(d->best_rocket);

    return 0;
#endif
}
