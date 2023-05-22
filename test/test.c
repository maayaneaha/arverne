#include <stdlib.h>
#include <stdio.h>

#include "algo/algo.h"
#include "algo/brutforce.h"
#include "basic_display/basic_display.h"
#include "physics/physics.h"
#include "utils.h"
#include "loader.h"

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
    calculate_rocket_infos(datas, r);
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
	Tank** tanks = load_Tanks("./bdd/FuelTank");
	/*Engine** engines = load_Engines("./bdd/Engine");*/
	/*Decoupler** decouplers = load_Decouplers("./bdd/Coupling");*/
}

void physic_tests()
{
    // calculate_DeltaV(int ISP, double mass_total, double mass_dry, double g)
    double res = calculate_DeltaV(100, 2000, 1000, 10);
    double res_sup = 693.14718056;
    if (res < res_sup * 0.98 || res > res_sup * 1.02)
    {
        printf("ERROR: calculate_DeltaV, res = %f (693.14718056)\n", res);
    }
    printf("OK: calculate_DeltaV\n");
    res = calculate_TWR(1000, 1000, 10);
    res_sup = 0.1;
    if (res < res_sup * 0.98 || res > res_sup * 1.02)
    {
        printf("ERROR: calculate_TWR, res = %f (0.1)\n", res);
    }
    printf("OK: calculate_TWR %f\n", res);
    res = calculate_g();
    res_sup = 9.81;
    if (res < res_sup * 0.98 || res > res_sup * 1.02)
    {
        printf("ERROR: calculate_g, res = %f (9.81)\n", res);
    }
    printf("OK: calculate_g\n");
    res = calculate_max_mass(1, 1000, 10);
    res_sup = 100;
    if (res < res_sup * 0.98 || res > res_sup * 1.02)
    {
        printf("ERROR: calculate_max_mass, res = %f (100m)\n", res);
    }
    printf("OK: calculate_max_mass %f\n", res);
    res = calculate_mass_fuel(1000, 100, 10, 0.1, 1000, 1000);
    res_sup = 4149.578;
    if (res < res_sup * 0.98 || res > res_sup * 1.02)
    {
        printf("ERROR: calculate_mass_fuel, res = %f (100m)\n", res);
    }
    printf("OK: calculate_mass_fuel %f\n", res);

}
