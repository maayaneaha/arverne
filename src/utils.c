#include "utils.h"

double calculate_mass_dry_tank(Tank *t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_masses(Stage *s)
{
    s->mass_fuel = 0;
    s->mass_dry = s->engine->mass * s->nbr_engines;
    s->cost = s->engine->cost * s->nbr_engines;
    Part *tank = s->first_tank;
    Tank *tmp_tank = tank->part_type;
    s->fuel = tmp_tank->fuel;
    while(tank != NULL)
    {
        tmp_tank = tank->part_type;
        s->mass_fuel += tmp_tank->empty_mass;
        s->mass_dry += calculate_mass_dry_tank(tank->part_type);
        s->cost += tank->cost;
        tank = tank->next;
    }
    return 1;
}
#include "utils.h"

/*Decoupler loadfile_Decoupler(char* filename)
{

}
*/
