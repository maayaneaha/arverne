#include <stdlib.h>
#include <stdio.h>

#include "algo/algo.h"
#include "basic_display/basic_display.h"
#include "utils.h"


Datas *create_datas()
{
    #if DEBUG
        printf("create_datas\n");
    #endif
    Datas *d = malloc(sizeof(Datas));
    d->delta_v_min = 2000;
    d->mass_payload = 2000;
    d->mass_max = 200000; // 200 000
    d->TWR_min = 1.4;
    d->TWR_max = 2;
    d->diameter_payload = SMALL;

    #if DEBUG
        printf("  begin\n");
    #endif
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
    e->name = "LV-T30 \"Reliant\" Liquid Fuel Engine\0";
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
    d->nbr_decouplers = 1;

    d->best_rocket = NULL;
    return d;
}


Rocket *build_rocket(Datas *datas, size_t nbr_stages)
{
    #if DEBUG
        printf("build_rocket(datas, %zu)\n", (size_t) datas->tanks[0]);
    #endif

    Rocket *r = malloc(sizeof(Rocket));
    r->mass_payload = datas->mass_payload;
    Stage *prev_s = NULL;
    #if DEBUG
        printf("  begin\n");
    #endif
    printf("3 %zu\n", (size_t) datas->tanks[0]);
    for(size_t i = 0; i < nbr_stages; i++) {
        Stage *s = malloc(sizeof(Stage));
        s->fuel = FUELOX;
        Part *prev = create_tank(datas->tanks[0]);
        s->first_tank = prev;

#if DEBUG
        printf("  new stage\n");
#endif
        for (size_t j = 0; j < 5; j++) {
            Part *tank = create_tank(*datas->tanks);
            tank->prev = prev;
            prev->next = tank;
            prev = tank;
        }
        s->engine = create_engine(*datas->engines);
        s->decoupler = create_decoupler(*datas->decouplers);
        s->nbr_engines = 1;
        s->prev = prev_s;
        if (prev_s != NULL) {
            prev_s->next = s;
        } else {
            r->first_stage = s;
        }
        prev_s = s;
    }
    calculate_rocket(r);
    return r;
}


void test_basic_display()
{
    #if DEBUG
        printf("test_basic_display\n");
    #endif
    Datas *d = create_datas();
    Rocket *r = build_rocket(d, 2);
    basic_display(r);
}
