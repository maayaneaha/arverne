#include <stdlib.h>
#include <stdio.h>

#include "algo/algo.h"
#include "algo/brutforce.h"
#include "basic_display/basic_display.h"
#include "utils.h"





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
        if (prev_s != NULL)
        {
            prev_s->next = s;
        }
        else
        {
            r->first_stage = s;
        }
        prev_s = s;
    }
    calculate_rocket_infos(r);
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

void test_brutforce()
{
#if DEBUG
        printf("test_brutforce()\n");
#endif
    Datas *d = create_datas();
    int r = brut_force(d);
    basic_display(d->best_rocket);
}

void coni()
{
    Decoupler* pute = load_Decoupler("Decoupler_0.json");    
    printf("%s\n", pute->name);
    printf("%f\n", pute->mass);
    printf("%f\n", pute->cost);
    /* printf("%d\n", pute->max_temp); */
    /* printf("%d\n", pute->ejection); */
    return;
}
