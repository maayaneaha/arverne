#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "utils.h"
#include "physics/physics.h"

Datas *create_datas()
{
#if DEBUG
    printf("create_datas\n");
#endif
    Datas *d = malloc(sizeof(Datas));
    d->deltaV_min = 2000;
    d->mass_payload = 2000;
    d->mass_max = 200000; // 200 000
    d->TWR_min = 1.4;
    d->TWR_max = 2;
    d->diameter_payload = SMALL;

#if DEBUG
    printf("  begin\n");
#endif
    /*
    Tank *t = malloc(sizeof(Tank));
    t->name = "FL-T100 Fuel Tank";
    t->empty_mass = 62.5;
    t->full_mass = 560;
    t->empty_cost = 104.1;
    t->full_cost = 150;
    t->top_diam = SMALL;
    t->down_diam = SMALL;
    t->fuel = FUELOX;
    t->quantity_fuel1 = 45;
    t->quantity_fuel2 = 55;

    d->tanks = malloc(sizeof(Tank*));
    d->tanks[0] = t;
    d->nbr_tanks = 1;

#if DEBUG
    printf("  tank (name = \"%s\" %s)\n", t->name, d->tanks[0]->name);
#endif
    Engine *e = malloc(sizeof(Engine));
    e->name = malloc(sizeof(char) * 36);
    const char* name = "LV-T30 \"Reliant\" Liquid Fuel Engine\0";
    e->name = strdup(name);
    e->mass = 1250;
    e->cost = 1100;
    e->fuel = FUELOX;
    e->diam = SMALL;
    e->ISP_atm = 265;
    e->ISP_vac = 310;
    e->thrust_atm = 205160;
    e->thrust_vac = 240000;
    e->consumption = 15.79;
    e->gimbal = 0;

    d->engines = malloc(sizeof(Engine*));
    d->engines[0] = e;
    d->nbr_engines = 1;

    Decoupler *s = malloc(sizeof(Decoupler));
    s->name = malloc(sizeof(char) * 16);
    s->name = "TD-12 Decoupler\0";
    s->mass = 40;
    s->cost = 200;

    d->decouplers = malloc(sizeof(Decoupler*));
    d->decouplers[0] = s;
    d->nbr_decouplers = 1;*/

    d->best_rocket = NULL;
    d->beta = 1/8;
    return d;
}

double calculate_mass_fuel_tank(Tank *t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_infos(Stage *s, Rocket *r)
{
#if DEBUG
    printf("calculate_stages_infos(s = %zu)\n{\n", (size_t) s);
#endif
    s->mass_dry = s->engine->mass * s->nbr_engines +  s->decoupler->mass;
    if (s->prev != NULL)
        s->mass_dry += s->mass_full;
    else
        s->mass_dry = r->mass_payload;
    s->cost = s->engine->cost * s->nbr_engines + s->decoupler->cost;
    s->mass_full = s->mass_dry;
    Part *tank = s->first_tank;
    Tank *tmp_tank = tank->part_type;
    s->fuel = tmp_tank->fuel;
    s->quantity_fuel1 = 0;
    s->quantity_fuel2 = 0;
#if DEBUG
    printf("  mass_full before tanks = %f\n  cost before tanks = %f\n", s->mass_full, s->cost);
#endif
    while(tank != NULL)
    {
        tmp_tank = tank->part_type;
        s->mass_full += tmp_tank->full_mass;
        s->mass_dry += tmp_tank->empty_mass;
        s->quantity_fuel1 += tmp_tank->quantity_fuel1;
        s->quantity_fuel2 += tmp_tank->quantity_fuel2;
        s->cost += tank->cost;
        tank = tank->next;
    }
    Engine *e = s->engine->part_type;
    s->DeltaV = calculate_DeltaV(e->ISP_atm, s->mass_full, s->mass_dry, calculate_g());
    s->TWR_min = calculate_TWR(s->mass_full, e->thrust_atm * s->nbr_engines, calculate_g());
#if DEBUG
    printf("  s->DeltaV = %f\n  s->TWR_min = %f\n  s->cost = %f\n  s->mass_full = %f\n}\n",
           s->DeltaV, s->TWR_min, s->cost, s->mass_full);
#endif
    return 1;
}


int calculate_rocket_infos(Rocket* r)
{
#if DEBUG
        printf("calculate_rocket_infos(r)\n");
#endif
    r->total_mass = r->mass_payload;
    r->cost = 0;
    Stage *s = r->first_stage;
#if DEBUG
    printf("  begin: stage = %zu\n", (size_t) s);
#endif
    Stage *prev = s;
    r->DeltaV = 0;
    for(; s != NULL; s = s->next)
    {
#if DEBUG
        printf("  stage = %zu\n", (size_t) s);
#endif
        calculate_stage_infos(s, r);
        r->DeltaV += s->DeltaV;
        r->cost += s->cost;
        prev = s;
    }
    r->total_mass = prev->mass_full;
    return 1;
}


Part *create_tank(Tank *t)
{

    #if DEBUG
        printf("create_tank(tank) (name = %s)\n", t->name);
    #endif
    Part *p = malloc(sizeof(Part));
    p->part_type = t;
    p->name = t->name;
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
    p->name = e->name;
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
    p->name = d->name;
    p->mass = d->mass;
    p->cost = d->cost;
    p->prev = NULL;
    p->next = NULL;
    return p;
}


Part *copy_part(Part *p)
{
    Part *np = malloc(sizeof(Part));
    np->part_type = p->part_type;
    np->name = p->name;
    np->mass = p->mass;
    np->cost = p->cost;
    np->prev = NULL;
    np->next = NULL;
    return np;
}


Stage *create_stage(Datas *d)
{
#if DEBUG
    printf("create_stage()\n");
#endif
    Stage *s = malloc(sizeof(Stage));
    s->mass_full = 0;
    s->mass_dry = 0;
    s->cost = 0;
    s->DeltaV = 0;
    s->decoupler = create_decoupler(d->decouplers[0]);
    s->prev = NULL;
    s->next = NULL;
    return s;
}


Stage *copy_stage(Stage *s)
{
    if (s == NULL)
        return NULL;
    // core copy
    Stage *ns = malloc(sizeof(Stage)); // new stage
    ns->mass_full = s->mass_full;
    ns->mass_dry = s->mass_dry;
    ns->cost = s->cost;
    ns->DeltaV = s->DeltaV;
    ns->prev = NULL;
    ns->next = NULL;
    // Tanks copy
    Part *prev = copy_part(s->first_tank);
    ns->first_tank = prev;
    // Part *t = prev;
    for(Part *t = s->first_tank->next; t != NULL; t = t->next)
    {
        Part *nt = copy_part(t);
        nt->prev = prev;
        prev->next = nt;
        prev = nt;
    }
    // Engine copy
    ns->engine = copy_part(s->engine);
    ns->nbr_engines = s->nbr_engines;
    // Decoupler copy
    ns->decoupler = copy_part(s->decoupler);
    // Values copy
    ns->fuel = s->fuel;
    ns->quantity_fuel1 = s->quantity_fuel1;
    ns->quantity_fuel2 = s->quantity_fuel2;
    ns->total_thrust_atm_min = s->total_thrust_atm_min;
    ns->total_thrust_atm_max = s->total_thrust_atm_max;
    ns->total_thrust_vac_min = s->total_thrust_vac_min;
    ns->total_thrust_vac_max = s->total_thrust_vac_max;
    ns->ISP_atm = s->ISP_atm;
    ns->ISP_vac = s->ISP_vac;
    ns->TWR_min = s->TWR_min;
    ns->TWR_max = s->TWR_max;
    ns->consumption = s->consumption;
    return ns;
}


Rocket *create_rocket(Datas *d)
{
    Rocket *r = malloc(sizeof(Rocket));
    r->mass_payload = d->mass_payload;
    r->total_mass = d->mass_payload;
    r->DeltaV = 0;
    r->cost = 0;
    r->first_stage = NULL;
    return r;
}

Rocket *copy_rocket(Rocket *r)
{
    Rocket *nr = malloc(sizeof(Rocket));
    nr->mass_payload = r->mass_payload;
    nr->DeltaV = r->DeltaV;
    nr->cost = r->cost;
    Stage *prev = copy_stage(r->first_stage);
    nr->first_stage = prev;
    if (r->first_stage == NULL)
        return nr;
    for(Stage *s = r->first_stage->next; s != NULL; s = s->next)
    {
        Stage *ns = copy_stage(s);
        ns->prev = prev;
        prev->next = ns;
        prev = ns;
    }
    return nr;
}


int create_tank_stack(Datas *d, Stage *s, enum diameter diam, double mass_fuel)
{
#if DEBUG
    printf("create_tank_stack(d, s, diam, mass_fuel = %f)\n{\n", mass_fuel);
#endif
    Part *prev = create_tank(d->tanks[0]);
    double mass_total = calculate_mass_fuel_tank(prev->part_type);
    Tank *t = prev->part_type;
    if (diam != t->top_diam) // not the correct diameter
        return 0;
    s->first_tank = prev;
    size_t n = 0;
    while (mass_total < mass_fuel)
    {
        Part *tank = create_tank(d->tanks[0]);
        mass_total += calculate_mass_fuel_tank(tank->part_type);
        tank->prev = prev;
        prev->next = tank;
        prev = tank;
        n++;
        if (n > MAX_TANK)
        {
#if DEBUG
            printf("}\n");
#endif
            return 0;
        }
    }
#if DEBUG
    printf("}\n");
#endif
    return 1;
}


int append_stage(Rocket *r, Stage *s)
{
#if DEBUG
    printf("append_stage(r = %zu, s = %zu)\n{\n", (size_t) r, (size_t) s);
#endif
    if (r->first_stage == NULL)
    {
        r->first_stage = s;
#if DEBUG
        printf("}\n");
#endif
        return 1;
    }
    Stage *cur = r->first_stage;
    while(cur->next != NULL)
        cur = cur->next;
    cur->next = s;
    cur->prev = cur;
#if DEBUG
    printf("}\n");
#endif
    return 1;
}

cJSON* json_ParseFile(char* filename)
{
    cJSON* parsed = NULL;
    int c;
    int i = 0;
    char* content = NULL;
    FILE* fp = fopen(filename, "r");

    while ((c = getc(fp)) != EOF)
        i++;

    fseek(fp, 0, 0);
    content = malloc((i + 1) * sizeof(char));

    i = 0;
    while ((c = getc(fp)) != EOF)
        content[i++] = c;
    content[i] = '\0';
    fclose(fp);
    parsed = cJSON_Parse(content);

    free(content);

    return parsed;
}

void free_part(Part* p)
{
    // Do not free the name or the part_type
    free(p); // So easy
}

void free_stage(Stage* s)
{
    free_part(s->engine);
    free_part(s->decoupler);
    for(Part* t = s->first_tank; t != NULL; t = t->next)
    {
        free_part(t);
    }
    free(s);
}

void free_rocket(Rocket* r)
{
    for(Stage* s = r->first_stage; s != NULL; s = s->next)
    {
        free_stage(s);
    }
    free(r);
}

void free_datas(Datas* d)
{
    for(size_t i = 0; i < d->nbr_tanks; i++)
    {
        free(d->tanks[i]->name);
        free(d->tanks[i]);
    }
    for(size_t i = 0; i < d->nbr_engines; i++)
    {
        free(d->engines[i]->name);
        free(d->engines[i]);
    }
    for(size_t i = 0; i < d->nbr_decouplers; i++)
    {
        free(d->decouplers[i]->name);
        free(d->decouplers[i]);
    }
    if (d->best_rocket != NULL)
        free_rocket(d->best_rocket);
}
