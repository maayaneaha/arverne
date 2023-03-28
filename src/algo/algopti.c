#include "algopti.h"

// on itere le nombre d'etage de 0 a 100
// on cherche a repartir equitablement de dv
// pour choisir un moteur, on prend celui qui permet d'avoir le plus de carburant
// si le dv n'est pas satisfait, on le reparti sur les autres etages

// utiliser calculate_mass_fuel pour trouver la masse totale et prendre la plus faible
Engine* search_engine(Datas* d, Rocket* r, double dv_needed, int* nbr_engines)
{
}


int search_stage(Datas* d, Rocket* r, double dv_needed)
{
    int nbr_engines;
    Engine* optimal_engine = search_engine(d, r, dv_needed, &nbr_engines);
    // int create_tank_stack(Datas *d, Stage *s, enum diameter diam, double mass_fuel)
    Stage* s = create_stage(d);
    s->engine = create_engine(optiam_engine);
    s->nbr_engines = nbr_engines;
    append_stage(r, s);
    return 1;
}

int search_rocket(Datas* d, size_t nbr_stages)
{
    Rocket *r = create_rocket(datas);
    r->cost = 999999999;
    int ret = 0;
    for (size_t i = nbr_stages; i > 0; i--)
    {
        if (!search_stage(d, r, d->deltaV_min / i))
        { // The stage can't have enough DV, we share it between the other stages
#if DEBUG
            printf("search_rocket: search_stage failed\n");
#endif
            return 0;
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