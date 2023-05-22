#include <stdio.h>
#include "utils.h"
#include "arguments.h"
#include "basic_display/basic_display.h"
#include "algo/algopti.h"
#include "basic_display/pretty_print.h"
#include "loader.h"
#include "interface/interface.h"

#if DEBUG
#include "test/test.h"
#include "debug.h"
#endif

int main(int argc, char* argv[])
{
	return start_interface();
#if DEBUG
    printf("debug\n");
    int init_debug();
	// For test functions only
    debug_write("BEGIN\n");
    //printf("%s\n", argopt.version);
    //return 0;
#endif
    double deltav = 2000;
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
#if DEBUG
    debug_write("Arguments recived\n");
#endif
    Datas *d = create_datas();
    load_parts(d);
#if DEBUG
    print_parts(d);
#endif
    d->deltaV_min = deltav;
    d->TWR_min = twrmin;
    d->TWR_max = twrmax;
    int r = linear_algo(d);
    basic_display(d->best_rocket);
    return 0;
}
