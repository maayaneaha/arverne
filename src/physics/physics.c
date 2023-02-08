#include <math.h>
#include "utils.h"


double calculate_DeltaV(int ISP, double mass_total, double mass_dry)
{
    return ISP * log(fmod(mass_total, mass_dry));
}

