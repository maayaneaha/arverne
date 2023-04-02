#include <stdio.h>
#include "utils.h"
#include "arguments.h"
#include "basic_display/basic_display.h"
#include "algo/algopti.h"
#include "basic_display/pretty_print.h"

#if DEBUG
#include "test/test.h"
#endif

int main(int argc, char* argv[])
{
#if DEBUG
    printf("debug\n");
	// For test functions only
	ArgOpt argopt = getopt_Parse(argc, argv);
    physic_tests();
	//printf("%s\n", argopt.version);
    // return 0;
#endif
    printf("debug2\n");
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
    print_parts(d);
    d->deltaV_min = deltav;
    d->TWR_min = twrmin;
    d->TWR_max = twrmax;
    int r = linear_algo(d);
    basic_display(d->best_rocket);

    return 0;
}
