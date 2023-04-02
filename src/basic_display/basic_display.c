#include <stdio.h>
#include "utils.h"


void basic_display(Rocket *rocket)
{
#if DEBUG
    printf("basic_display(%zu)\n", (size_t) rocket);
#endif
    printf("Mass (payload): %f (%f)\n", rocket->total_mass - rocket->mass_payload, rocket->mass_payload);
    printf("Delta V: %f\n", rocket->DeltaV);
    printf("Cost: %f\n", rocket->cost);
    int i = 0;
    for(Stage *s = rocket->first_stage; s != NULL; s = s->next)
    {
        printf("Stage %i:\n", i);
        printf("  Mass (empty): %f (%f)\n", s->mass_full, s->mass_dry);
        printf("  Delta V %f\n", s->DeltaV);
        printf("  Cost: %f\n", s->cost);
        printf("  Parts (name (mass, cost)):\n");
        printf("    %s (%f, %f)\n", s->decoupler->name, s->decoupler->mass, s->decoupler->cost);
        for(Part *p = s->first_tank; p != NULL; p = p->next)
        {
            printf("    %s (%f, %f)\n", p->name, p->mass, p->cost);
        }
        printf("    %s x %u (%f, %f)\n", s->engine->name, s->nbr_engines, s->engine->mass, s->engine->cost);
        i++;
    }
}
