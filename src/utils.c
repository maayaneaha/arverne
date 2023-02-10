#include <string.h>
#include <stdio.h>
#include "utils.h"


double calculate_mass_dry_tank(Tank *t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_masses(Stage *s)
{
    s->mass_dry = s->engine->mass * s->nbr_engines;
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
    return 1;
}


int calculate_rocket_masses(Rocket *r)
{
    r->total_mass = r->mass_payload;
    for(Stage *s = r->first_stage; s != NULL; s = s->next)
    {
        calculate_stage_masses(s);
        r->total_mass += s->mass_full;
    }
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

/*Decoupler loadfile_Decoupler(char* filename)
{

}
*/
