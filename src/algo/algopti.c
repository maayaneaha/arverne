#include "algopti.h"

// on itere le nombre d'etage de 0 a 100
// on cherche a repartir equitablement de dv
// pour choisir un moteur, on prend celui qui permet d'avoir le plus de carburant
// si le dv n'est pas satisfait, on le reparti sur les autres etages

// utiliser calculate_mass_fuel pour trouver la masse totale et prendre la plus faible
Engine search_engine()
{
    
}


double search_stage(Datas* d, Rocket* r, double dv_needed)
{
    
}

int search_rocket(Datas* d, size_t nbr_stages)
{
    Rocket *r = create_rocket(datas);
    r->cost = 999999999;
    int ret = 0;
    for (size_t i = nbr_stages; i > 0; i--)
    {
        if ((int dv_remaining = search_stage(d, r, d->deltaV_min / i)) != 0)
        { // The stage can't have enough DV, we share it between the other stages
#if DEBUG
            printf("search_rcoket: dv_remaining = %f\n", dv_remaining);
#endif
        }
    }
    return ret;
}

int linear_algo(Datas* d)
{
    for (size_t nbr_stages = 1; nbr_stages < NBR_SEARCH_STAGES; nbr_stages++)
    {
        if (!search_rocket(d, nbr_stages))
            return 0;
    }
    return 1;
}