#include <stdio.h>
#include "utils.h"


void basic_display(Rocket *rocket)
{
    printf("Mass (payload): %f (%f)\n", rocket->total_mass - rocket->mass_payload, rocket->mass_payload);
    printf("Delta V: %f\n", rocket->DV);
    printf("Cost: %f\n", rocket->cost);
    int i = 0;
    for(Stage *s = rocket->first_stage; s != NULL; s = s->next)
    {
        printf("Stage %i:\n", i);
        printf("  Mass (empty): %f (%f)\n", s->mass_full, s->mass_dry);
        printf("  Cost: %f\n", s->cost);
        printf("  Parts (name (mass, cost)):\n");
        printf("    %s (%f, %f)\n", s->decoupler->name, s->decoupler->mass, s->decoupler->cost);
        for(Part *p = s->first_tank; p != NULL; p = p->next)
        {
            printf("    %s (%f, %f)\n", p->name, p->mass, p->cost);
        }
        printf("    %s (%f, %f)\n", s->engine->name, s->engine->mass, s->engine->cost);
        i++;
    }
}