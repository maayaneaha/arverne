#include <stdio.h>
#include "utils.h"


void basic_display(Rocket *rocket)
{
    printf("Mass (+ payload): %f (%f)\n", rocket->total_mass - rocket->mass_payload, rocket->mass_payload);
    printf("Delta V: %f\n", rocket->DV);
    printf("Cost: %f\n", rocket->cost);
}