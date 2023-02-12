#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "utils.h"
#include "physics/physics.h"

double calculate_mass_fuel_tank(Tank *t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_infos(Stage *s)
{
#if DEBUG
    printf("calculate_stages_infos(s = %zu)\n", (size_t) s);
#endif
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
    Engine *e = s->engine->part_type;
    s->DeltaV = calculate_DeltaV(e->ISP_atm, s->mass_full, s->mass_dry, calculate_g());
    s->TWR_min = calculate_TWR(s->mass_full, e->thrust_atm * s->nbr_engines, calculate_g());
    return 1;
}


int calculate_rocket_infos(Rocket *r)
{
#if DEBUG
        printf("calculate_rocket_infos(r)\n");
#endif
    r->total_mass = r->mass_payload;
    Stage *s = r->first_stage;
#if DEBUG
    printf("  begin: stage = %zu\n", (size_t) s);
#endif
    Stage *prev = s;
    for(; s != NULL; s = s->next)
    {
        calculate_stage_infos(s);
        if (s->prev == NULL)
        {
            s->mass_dry += r->mass_payload;
            s->mass_full += r->mass_payload;
        }
        r->DeltaV += s->DeltaV;
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


Part *copy_part(Part *p)
{
    Part *np = malloc(sizeof(Part));
    np->part_type = p->part_type;
    np->name = malloc(sizeof(char) * (strlen(p->name) + 1));
    strcpy(np->name, p->name);
    np->mass = p->mass;
    np->cost = p->cost;
    np->prev = NULL;
    np->next = NULL;
    return np;
}


Stage *create_stage(Datas *d)
{
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
    printf("create_tank_stack(d, s, diam, mass_fuel = %f)\n", mass_fuel);
#endif
    Part *prev = create_tank(d->tanks[0]);
    double mass_total = calculate_mass_fuel_tank(prev->part_type);
    Tank *t = prev->part_type;
    if (diam != t->top_diam) // not the correct diameter
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


int append_stage(Rocket *r, Stage *s)
{
#if DEBUG
    printf("append_stage(r = %zu, s = %zu)\n", (size_t) r, (size_t) s);
#endif
    if (r->first_stage == NULL)
    {
        r->first_stage = s;
        return 1;
    }
    Stage *cur = r->first_stage;
    while(cur->next != NULL)
        cur = cur->next;
    cur->next = s;
    cur->prev = cur;
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
void free_part (Part* p);
{
   free(p->part_type);
   free(p->name);
   free(p);
}
Decoupler* load_Decoupler(char* filename)
{
    Decoupler* obj = malloc(sizeof(Decoupler));
    cJSON* file = json_ParseFile(filename);
    cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
    cJSON* tmp = NULL;
    
    if (!part)
        errx(1, "Incorrect decoupler file");

    tmp = cJSON_GetObjectItemCaseSensitive(part, "name");
    if (tmp)
        asprintf(&obj->name, "%s", tmp->valuestring);

    tmp = cJSON_GetObjectItemCaseSensitive(part, "mass");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->mass = tmp->valuedouble;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->cost = tmp->valuedouble;
        else
            obj->cost = (double) tmp->valueint;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "maxTemp");
    if (tmp)
    {
        if (tmp->valueint)
            obj->max_temp = tmp->valueint;
    }
    
    tmp = cJSON_GetObjectItemCaseSensitive(part, "crashTolerance");
    if (tmp)
    {
        if (tmp->valueint)
            obj->tolerance_ms = tmp->valueint;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "MODULE");
    if (tmp)
    {
        if (cJSON_IsArray(tmp))
        {
            cJSON* i = NULL;
            cJSON* j = NULL;
            cJSON_ArrayForEach(i, tmp)
            {
                /* j = cJSON_GetObjectItemCaseSensitive(i, "name"); */
                /* if (cJSON_IsString(j)) */
                /* { */
                j = cJSON_GetObjectItemCaseSensitive(i, "ejectionForce");
                if (j)
                {
                    if (j->valueint)
                        obj->ejection = j->valueint;
                    break;
                }
                /* } */
            }
        }
    }

    cJSON_Delete(file);
    return obj;
}
*/
