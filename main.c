#include <stdio.h>
#include "utils.h"
#include "arguments.h"
#include "basic_display/basic_display.h"
#include "algo/brutforce.h"

#if DEBUG
#include "test/test.h"
#endif

int main(int argc, char* argv[])
{
#if DEBUG
    // For test functions only
    physic_tests();
    //coni();
    return 0;
#else
    /* ArgOpt argopt = getopt_Parse(argc, argv); */
    
    double deltav = 2100;
    double twrmin = 1.4;
    double twrmax = 1.8;
    if (argc > 1)
        deltav = atof(argv[1]);
    if (argc > 2)
    {
        twrmin = atof(argv[2]);
    }
    if (argc > 3)
    { 
        twrmax = atof(argv[3]);
    }

    Datas *d = create_datas();
    d->deltaV_min = deltav;
    d->TWR_min = twrmin;
    d->TWR_max = twrmax;
    int r = brut_force(d);
    basic_display(d->best_rocket);

    return 0;
#endif
}
