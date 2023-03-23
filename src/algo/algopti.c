#include "algopti.h"

// on itere le nombre d'etage de 0 a 100
// on cherche a repartir equitablement de dv
// pour choisir un moteur, on prend celui qui permet d'avoir le plus de moteurs
// si le dv n'est pas satisfait, on le reparti sur les autres etages

int search_stage(Datas* d, Rocket* r, double dv_needed)
{
    
}

int search_rocket(Datas* d, size_t nbr_stages)
{
    Rocket *r = create_rocket(datas);
    r->cost = 999999999;
    int ret = 1;
    for (size_t i = nbr_stages; i > 0; i--)
    {
        ret = ret && search_stage(d, r, d->deltaV_min / i);
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