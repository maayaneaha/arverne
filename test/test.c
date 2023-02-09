#include <stdlib.h>

#include "algo/algo.h"
#include "basic_display/basic_display.h"
#include "utils.h"


Datas *create_datas()
{
    Datas *d = malloc(sizeof(Datas));
    return d;
}


Rocket *build_rocket()
{
    Rocket *r = malloc(sizeof(Rocket));
    return r;
}

