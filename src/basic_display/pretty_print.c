#include "basic_display/pretty_print.h"


int print_tank(Tank* t)
{
    printf("  %s:\n    mass (empty) = %f (%f)\n    cost (empty) = %f (%f)\n",
           t->name, t->full_mass, t->empty_mass, t->full_cost, t->empty_cost);
    return 1;
}

int print_engine(Engine* e)
{
    printf(" %s:\n    mass =  %f\n    cost = %f\n",
           e->name, e->mass, e->cost);
    return 1;
}

int print_decoupler(Decoupler* d)
{
    printf(" %s:\n    mass =  %f\n    cost = %f\n",
           d->name, d->mass, d->cost);
    return 1;
}

int print_parts(Datas* d)
{
    printf("Print_parts\n{\nTanks:\n");
    for(size_t i = 0; i < d->nbr_tanks; i++)
    {
        print_tank(d->tanks[i]);
    }
    printf("Engines:\n");
    for(size_t i = 0; i < d->nbr_engines; i++)
    {
        print_engine(d->engines[i]);
    }
    printf("Decouplers:\n");
    for(size_t i = 0; i < d->nbr_decouplers; i++)
    {
        print_decoupler(d->decouplers[i]);
    }
    return 1;
}