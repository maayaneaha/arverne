#include "algo/algopti.h"
#include <stdio.h>

// on itere le nombre d'etage de 0 a 100
// on cherche a repartir equitablement de dv
// pour choisir un moteur, on prend celui qui permet d'avoir le plus de carburant
// si le dv n'est pas satisfait, on le reparti sur les autres etages

// utiliser calculate_mass_fuel pour trouver la masse totale et prendre la plus faible
Engine* search_engine(Datas* d, Rocket* r, double dv_needed, int* ne, double* ret_mass)
{
#if DEBUG
    printf("search_engine(%zu)\n{\n", (size_t) d);
#endif
    Engine* optimal_engine = NULL;
    double minimal_mass = INF;
#if DEBUG
    printf("nbr_engines %zu\n", d->nbr_engines);
#endif
    for(size_t i = 0; i < d->nbr_engines; i++)
    {
        int nbr_engines = 1;
        Engine* e = d->engines[i];
        double TWR = -1;
        double mass_fuel, mass_total;
        double beta = d->beta;

#if DEBUG
        printf("engine %zu (%zu)\n", i, (size_t) e);
#endif
        while (TWR < d->TWR_min) // To be sure that the TWR is ok
        {
#if DEBUG
            printf("d->TWR_min = %f\n", d->TWR_min);
#endif
            mass_fuel = calculate_mass_fuel(dv_needed, calculate_isp(e),
                                                   calculate_g(), beta, e->mass * nbr_engines,
                                                   r->total_mass);
#if DEBUG
            printf("mass_fuel = %f\n", mass_fuel);
#endif
            mass_total = r->total_mass + mass_fuel * (1 + beta) + e->mass * nbr_engines + d->decouplers[0]->mass;
            TWR = calculate_TWR(mass_total, e->thrust_atm * nbr_engines, calculate_g());
#if DEBUG
            printf("mass_total = %f\nTWR %f\nnbr_engines %u\n", mass_total, TWR, nbr_engines);
#endif
            if (TWR < d->TWR_min)
            {
                int prev_nbr_engines = nbr_engines;
                nbr_engines *= (int) ceil(d->TWR_min / TWR);
                if (prev_nbr_engines >= nbr_engines)
                    nbr_engines = prev_nbr_engines + 1;

            }
        }
        if (mass_fuel < 0)
        {
            continue;
        }
#if DEBUG
        printf("minimal_mass = %f, mass_total = %f\n", minimal_mass, mass_total);
#endif
        if (mass_total < minimal_mass)
        {
            optimal_engine = e;
            minimal_mass = mass_total;
            *ne = nbr_engines;
        }
    }
    *ret_mass = minimal_mass;
#if DEBUG
    printf("}\n");
#endif
    return optimal_engine;
}


int search_stage(Datas* d, Rocket* r, double dv_needed)
{
#if DEBUG
    printf("search_stage(d, r, dv_needed = %f)\n{\nd->deltaV_min = %f\n", dv_needed, d->deltaV_min);
#endif
    int nbr_engines;
    double minimal_mass;
    Engine* optimal_engine = search_engine(d, r, dv_needed, &nbr_engines, &minimal_mass);
    if (optimal_engine == NULL)
    {
#if DEBUG
        printf("}\n");
#endif
        return 0;
    }
#if DEBUG
    printf("optimal_engine = %zu\n", (size_t) optimal_engine);
#endif
    // int create_tank_stack(Datas *d, Stage *s, enum diameter diam, double mass_fuel)
    Stage* s = create_stage(d);
    s->engine = create_engine(optimal_engine);
    create_tank_stack(d, s, optimal_engine->diam, minimal_mass);
    s->nbr_engines = nbr_engines;
    append_stage(r, s);
    //todo gerer le DV en trop
    calculate_rocket_infos(r);
#if DEBUG
    printf("DeltaV = %f\n", calculate_DeltaV(calculate_isp(s->engine->part_type), s->mass_full, s->mass_dry,calculate_g()));
    // double calculate_DeltaV(int ISP, double mass_total, double mass_dry, double g)
    basic_display(r);
    printf("}\n");
#endif
    return 1;
}

int search_rocket(Datas* d, size_t nbr_stages)
{
#if DEBUG
    printf("search_rocket()\n{\n");
#endif
    Rocket *r = create_rocket(d);
    r->cost = 0;
    int ret = 1;
    for (size_t i = nbr_stages; i > 0; i--)
    {
        if (!search_stage(d, r, d->deltaV_min / i))
        { // The stage can't have enough DV, we share it between the other stages
#if DEBUG
            printf("search_rocket: search_stage failed\n}\n");
#endif
            return 0;
        }
    }
#if DEBUG
    printf("r->cost = %f\n", r->cost);
#endif
    if (r->cost < d->best_rocket->cost)
    {
        d->best_rocket = r;
    }
#if DEBUG
    printf("ret = %d\n}\n", ret);
#endif
    return ret;
}

int linear_algo(Datas* d)
{
#if DEBUG
    printf("linear_algo()\n");
#endif
    Rocket *r = create_rocket(d);
    r->cost = INF;
    d->best_rocket = r;
    for (size_t nbr_stages = 1; nbr_stages <= NBR_SEARCH_STAGES; nbr_stages++)
    {
#if DEBUG
        printf("rocket %zu\n", nbr_stages);
#endif
        if (!search_rocket(d, nbr_stages))
        {
            return 0;
        }
    }
    return 1;
}