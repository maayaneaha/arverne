#include <stdlib.h>
#include <stdio.h>

#include "algo/algo.h"
#include "algo/brutforce.h"
#include "basic_display/basic_display.h"
#include "utils.h"


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

int multivers()
{
   Tank* t = malloc(sizeof(Tank));
   t->name = "HAHA";
   t->empty_mass = 43;
   t->full_mass = 23;
   t->top_diam = SMALL;
   t->fuel = FUELOX;
   t->quantity_fuel1 = 123;
   t->quantity_fuel2 = 124;
   t->radial_fitting = 12;
   Part* tankp = create_tank(t);


   printf("Tank -> Part :\n");
   if (tankp->part_type != t)
      {
      printf("Part's type incorrect !!");
      return 0;
      }
  /* if (tankp->name != t->name)
      {
      printf("Part's name incorrect !!");
      return 0;
      }
   */
   if (tankp->mass != t->full_mass)
      {
      printf("Part's mass incorrect !!");
      return 0;
      }
   if (tankp->cost != t->full_cost)
      {
      printf("Part's cost incorrect !!");
      return 0;
      }
   printf("Tank -> Part : Valid \n\n");
   


   Engine* e = malloc(sizeof(Engine));
   e->name = "OUOH";
   e->mass = 128;
   e->cost = 234;
   e->fuel = XENON;
   e->diam = TINY;
   e->ISP_atm = 349423;
   e->ISP_vac = 9392;
   e->thrust_atm = 2;
   e->thrust_atm = 342;
   e->consumption = 111;
   e->gimbal = 243;
   
   Part* ep = create_engine(e);
   printf("Engine -> Part :\n");
   if (ep->part_type != e)
      {
      printf("Part's type incorrect !!");
      return 0;
      }
  
   /* if (ep->name != e->name)
      {
      printf("Part's name incorrect !!");
      return 0;
      }
   */
   if (ep->mass != e->mass)
      {
      printf("Part's mass incorrect !!");
      return 0;
      }
   if (ep->cost != e->cost)
      {
      printf("Part's cost incorrect !!");
      return 0;
      }
   printf("Engine -> Part : Valid \n\n");


   

   
   Decoupler* d = malloc(sizeof(Decoupler));
   d->name = "BONJOUR";
   d->mass = 444;
   d->cost = 541;
   
   Part* dp = create_decoupler(d);
   printf("Decoupler -> Part :\n");
   if (dp->part_type != d)
      {
      printf("Part's type incorrect !!");
      return 0;
      }
 /*  if (dp->name != d->name)
      {
      printf("Part's name incorrect !!");
      return 0;
      }
 */
   if (dp->mass != d->mass)
      {
      printf("Part's mass incorrect !!");
      return 0;
      }
   if (dp->cost != d->cost)
      {
      printf("Part's cost incorrect !!");
      return 0;
      }
   printf("Decoupler -> Part : Valid \n\n");


   printf("Part copying test\n");
   Part* cp = copy_part(tankp);
   if (cp->part_type != tankp->part_type)
      {
      printf("Part's type incorrect !!");
      return 0;
      }
 /*  if (cp->name != cp->name)
      {
      printf("Part's name incorrect !!");
      return 0;
      }
 */
   if (cp->mass != tankp->mass)
      {
      printf("Part's mass incorrect !!");
      return 0;
      }
   if (cp->cost != tankp->cost)
      {
      printf("Part's cost incorrect !!");
      return 0;
      }
   printf("Part copying : Valid \n\n");
   return 0;
}
/*void coni()
{
    Decoupler* pute = load_Decoupler("Decoupler_0.json");    
    printf("%s\n", pute->name);
    printf("%f\n", pute->mass);
    printf("%f\n", pute->cost);
    printf("%d\n", pute->max_temp);
    printf("%d\n", pute->ejection);
    return;
}*/
