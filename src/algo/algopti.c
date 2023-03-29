#include "algo/algopti.h"

// on itere le nombre d'etage de 0 a 100
// on cherche a repartir equitablement de dv
// pour choisir un moteur, on prend celui qui permet d'avoir le plus de carburant
// si le dv n'est pas satisfait, on le reparti sur les autres etages

// utiliser calculate_mass_fuel pour trouver la masse totale et prendre la plus faible
Engine* search_engine(Datas* d, Rocket* r, double dv_needed, int* nbr_engines, double* ret_mass)
{
    Engine* optimal_engine = NULL;
    double minimal_mass = INF;
    for(size_t i = 0; i < d->nbr_engines; i++)
    {
        int nbr_engines = 1;
        Engine* e = d->engines[i];
        double TWR = -1;
        double mass_fuel, mass_total;
        double beta = BETA;
        while (TWR < d->TWR_min) // To be sure that the TWR is ok
        {
            mass_fuel = calculate_mass_fuel(dv_needed, calculate_isp(optimal_engine),
                                                   calculate_g(), beta, nbr_engines,
                                                   r->total_mass);
            mass_total = r->total_mass + mass_fuel * (1 + beta) + e->mass * nbr_engines;
            TWR = calculate_TWR(mass_total, e->thrust_atm, calculate_g());
            if (TWR < d->TWR_min)
            {
                int prev_nbr_engines = nbr_engines;
                nbr_engines *= (int) ceil(d->TWR_min / TWR);
                if (prev_nbr_engines >= nbr_engines)
                    nbr_engines++;

            }
        }
        if (mass_fuel < 0)
        {
            continue;
        }
        if (mass_total < minimal_mass)
        {
            optimal_engine = e;
            minimal_mass = mass_total;
        }
    }
    *ret_mass = minimal_mass;
    return optimal_engine;
}


int search_stage(Datas* d, Rocket* r, double dv_needed)
{
    int nbr_engines;
    double minimal_mass;
    Engine* optimal_engine = search_engine(d, r, dv_needed, &nbr_engines, &minimal_mass);
    // int create_tank_stack(Datas *d, Stage *s, enum diameter diam, double mass_fuel)
    Stage* s = create_stage(d);
    s->engine = create_engine(optimal_engine);
    create_tank_stack(d, s, optimal_engine->diam, minimal_mass);
    s->nbr_engines = nbr_engines;
    append_stage(r, s);
    //todo
    return 1;
}

int search_rocket(Datas* d, size_t nbr_stages)
{
    Rocket *r = create_rocket(d);
    r->cost = INF;
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