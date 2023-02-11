#include "utils.h"
#include "physic.h"


int rocket_creator(Datas *datas, double deltaV_min, Rocket *r)
{
    if (deltaV_min <= 0)
        return 1;
    double deltaV_min = datas->DeltaV_min;
    for (size_t i = 0; i < datas->nbr_engines; ++i)
    {
        Stage *s = create_stage();
        Engine *e = datas->engines[i];
        s->engine = create_engine(e);
        // Mass fuel max for a minimum TWR
        double mass_fuel_max = calculate_max_mass(datas->TWR_min, e->thrust_atm, calculate_g());
        double tmp = calculate_max_mass(datas->TWR_min, e->thrust_vac, calculate_g());
        if (tmp < mass_fuel_max)
            mass_fuel_max = tmp;
        // Mass fuel needed for th DeltaV
        double mass_fuel_needed = calculate_mass_fuel(deltaV_min, e->ISP_atm, calculate_g(), e->mass);
        tmp = calculate_mass_fuel(deltaV_min, e->ISP_vac, calculate_g(), e->mass);
        if (tmp < mass_fuel_needed)
            mass_fuel_needed = tmp;

        if (mass_fuel_needed > mass_fuel_max)
            mass_fuel_needed = mass_fuel_max;
        create_tank_stack(datas, s, e->diam, mass_fuel_needed);
        calculate_rocket_infos(r);
        DeltaV_min -= s->DeltaV;
        if (deltaV_min > 0)
            r
    }
}

int brut_force(Datas datas)
{

}
