#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "physics/physics.h"


double calculate_mass_fuel_tank(Tank *t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_infos(Stage *s)
{
    s->mass_dry = s->engine->mass * s->nbr_engines;
    if (s->prev != NULL)
        s->mass_dry += s->mass_full;
    s->mass_dry += s->decoupler->mass;
    s->cost = s->engine->cost * s->nbr_engines;
    s->cost += s->decoupler->cost;
    s->mass_full = s->mass_dry;
    Part *tank = s->first_tank;
    Tank *tmp_tank = tank->part_type;
    s->fuel = tmp_tank->fuel;
    while(tank != NULL)
    {
        tmp_tank = tank->part_type;
        s->mass_full += tmp_tank->full_mass;
        s->mass_dry += tmp_tank->empty_mass;
        s->cost += tank->cost;
        tank = tank->next;
    }
    s->DeltaV = calculate_DeltaV(s->engine->part_type->ISP_atm, s->mass_full, s->mass_dry, calculate_g());
    s->TWR_min = calculate_TWR(s->mass_full, s->engine->part_type->thrust_atm * s->nbr_engines, calculate_g());
    return 1;
}


int calculate_rocket_infos(Rocket *r)
{
    r->total_mass = r->mass_payload;
    Stage *s = r->first_stage
    for(; s != NULL; s = s->next)
    {
        calculate_stage_infos(s);
        if (s->prev == NULL)
        {
            s->mass_dry += mass_payload;
            s->mass_full += mass_payload;
        }
        r->DeltaV += s->DeltaV;
    }
    r->total_mass = s->mass_full;
    return 1;
}


int calculate_rocket(Rocket *r)
{
    calculate_rocket_masses(r);
    return 1;
}


Part *create_tank(Tank *t)
{

    #if DEBUG
        printf("create_tank(tank) (name = %s)\n", t->name);
    #endif
    Part *p = malloc(sizeof(Part));
    p->part_type = t;
    p->name = malloc(sizeof(char) * (strlen(t->name) + 1));
    strcpy(p->name, t->name);
    p->mass = t->full_mass;
    p->cost = t->full_cost;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

Part *create_engine(Engine *e)
{
    Part *p = malloc(sizeof(Part));
    p->part_type = e;
    p->name = malloc(sizeof(char) * (strlen(e->name) + 1));
    strcpy(p->name, e->name);
    p->mass = e->mass;
    p->cost = e->cost;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

Part *create_decoupler(Decoupler *d)
{
    Part *p = malloc(sizeof(Part));
    p->part_type = d;
    p->name = malloc(sizeof(char) * (strlen(d->name) + 1));
    strcpy(p->name, d->name);
    p->mass = d->mass;
    p->cost = d->cost;
    p->prev = NULL;
    p->next = NULL;
    return p;
}

Stage *create_stage()
{
    Stage *s = malloc(sizeof(Stage));
    s->mass_full = 0;
    s->mass_dry = 0;
    s->cost = 0;
    s->DeltaV = 0;
    s->prev = NULL;
    s->next = NULL;
    return s;
}

Rocket *create_rocket(Datas *d)
{
    Rocket *r = malloc(sizeof(Rocket));
    r->mass_payload = d->mass_payload;
    r->DeltaV = 0;
    r->cost = 0;
    r->first_stage = NULL;
}

int create_tank_stack(Datas *d, Stage *s, diameter diam, double mass_fuel)
{
    Part *prev = create_tank(d->tanks[0]);
    double mass_total = calculate_mass_fuel_tank(prev->part_type);
    if (diam != prev->part_type->top_diam) // not the corrrct diameter
        return 0;
    s->first_tank = prev;
    while (mass_total < mass_fuel)
    {
        Part *tank = create_tank(d->tanks[0]);
        mass_total += calculate_mass_fuel_tank(tank->part_type);
        tank->prev = prev;
        prev->next = tank;
        prev = tank;
    }
    return 1;
}

/*Decoupler loadfile_Decoupler(char* filename)
{

}
*/
