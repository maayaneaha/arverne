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
    /* ArgOpt argopt = getopt_Parse(argc, argv); */
    
    double deltav = 0;
    double twrmin = 0;
    double twrmax = 0;
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
    printf("%F\n%F\n%F", deltav, twrmin, twrmax);

    return 0;
#endif
}
