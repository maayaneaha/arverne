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
    printf("    ISP (atm, vac) = %d, %d\n    thrust (atm, vac) = %f, %f\n",
           e->ISP_atm, e->ISP_vac, e->thrust_atm, e->thrust_vac);
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
    for (size_t j = 0; j < NBR_DIAMS; j++) {
        for (size_t i = 0; i < d->nbr_tanks[j]; i++) {
            print_tank(d->tanks[j][i]);
        }
        printf("Engines:\n");
        for (size_t i = 0; i < d->nbr_engines[j]; i++) {
            print_engine(d->engines[j][i]);
        }
        printf("Decouplers:\n");
        for (size_t i = 0; i < d->nbr_decouplers[j]; i++) {
            print_decoupler(d->decouplers[j][i]);
        }
    }
    return 1;
}