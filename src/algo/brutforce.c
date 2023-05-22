#include <stdio.h>

#include "algo/brutforce.h"
#include "physics/physics.h"


int rocket_creator(Datas *datas, double deltaV_min, Rocket *r)
{
    /*
#if DEBUG
    printf("rocket_creator(datas, deltaV_min, r)\n");
#endif
    if (deltaV_min <= 0)
        return 1;
    for (size_t i = 0; i < datas->nbr_engines[0]; ++i)
    {
        Stage *s = create_stage(datas, 0);
        Engine *e = datas->engines[0][i];
        s->engine = create_engine(e);
        // Mass fuel max for a minimum TWR
        double mass_fuel_max = calculate_max_mass(datas->TWR_min, e->thrust_atm, calculate_g());
        double tmp = calculate_max_mass(datas->TWR_min, e->thrust_vac, calculate_g());
        if (tmp < mass_fuel_max)
            mass_fuel_max = tmp;
        // Mass fuel needed for th DeltaV
        double mass_fuel_needed;// = calculate_mass_fuel(deltaV_min, e->ISP_atm, calculate_g(), e->mass);
        //tmp = calculate_mass_fuel(deltaV_min, e->ISP_vac, calculate_g(), e->mass);
        if (tmp < mass_fuel_needed)
            mass_fuel_needed = tmp;

        if (mass_fuel_needed > mass_fuel_max)
            mass_fuel_needed = mass_fuel_max;
        create_tank_stack(datas, s, mass_fuel_needed);
        append_stage(r, s);
        calculate_rocket_infos(r);
        deltaV_min -= s->DeltaV;
        if (r->cost < datas->best_rocket->cost)
            datas->best_rocket = r;
        if (deltaV_min > 0)
        {
            Rocket *nr = copy_rocket(r);
            rocket_creator(datas, deltaV_min, nr);
        } // TODO: fix free
    }
     */
    return 1;
}

int brut_force(Datas *datas)
{
    Rocket *r = create_rocket(datas);
    Rocket *er = copy_rocket(r);
    er->cost = 999999999;
    datas->best_rocket = er;
    rocket_creator(datas, datas->deltaV_min, r);
    return 1;
}
