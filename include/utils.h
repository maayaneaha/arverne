#ifndef UTILS
#define UTILS

#include <stdlib.h>

enum diameter {TINY, SMALL, MEDIUM, LARGE, EXTRALARGE, MK2, MK3, X}; // MK1 = SMALL
enum fuel_type {FUELOX, LIQUIDFUEL, MONOPROPELLANT, SOLIDFUEL, XENON, ORE};

struct tank
{
    const char *name;
    double empty_mass;
    double full_mass;  // full_mass = empty_mass + fuel_mass
    double empty_cost;
    double full_cost;
    enum diameter top_diam;
    enum diameter down_diam;
    enum fuel_type fuel;
    double quantity_fuel1;
    double quantity_fuel2;
    int radial_fitting; // Parts can be fit on radial
    int radial_part; // Is a radial tank
};

typedef struct tank Tank;
typedef struct decoupler Decoupler;
typedef struct rocket Rocket;
typedef struct part Part;
typedef struct datas Datas;
typedef struct stage Stage;
typedef struct engine Engine;

struct engine
{
    const char *name;
    double mass;
    double cost;
    enum fuel_type fuel;
    enum diameter diam;
    int ISP_atm;
    int ISP_vac;
    double thrust_atm;
    double thrust_vac;
    double consumption;
    double gimbal;
};


struct decoupler
{
    char *name;
    double mass;
    double cost;
};

struct part
{
    void *part_type;
    char *name;
    double mass;
    double cost;
    Part *prev;
    Part *next;
};

struct stage
{
    double mass_full;
    double mass_dry;
    double cost;
    enum fuel_type fuel;
    double quantity_fuel1;
    double quantity_fuel2;
    double DeltaV;
    double total_thrust_atm_min;
    double total_thrust_atm_max;
    double total_thrust_vac_min;
    double total_thrust_vac_max;
    double ISP_atm;
    double ISP_vac;
    double TWR_min;
    double TWR_max;
    double consumption;
    Part *first_tank;
    Part *engine;
    int nbr_engines;
    Part *decoupler;
    Stage *prev;
    Stage *next;
};

struct rocket
{
    double mass_payload;
    double total_mass;
    double DeltaV;
    double cost;
    Stage *first_stage;
};

struct datas
{
    double deltaV_min;
    double mass_payload;
    double mass_max;
    double TWR_min;
    double TWR_max;
    enum diameter diameter_payload;
    Tank **tanks;
    size_t nbr_tanks;
    Engine **engines;
    size_t nbr_engines;
    Decoupler **decouplers;
    size_t nbr_decouplers;
    Rocket *best_rocket;
};


double calculate_mass_fuel_tank(Tank *t);
int calculate_stage_infos(Stage *s);
int calculate_rocket_infos(Rocket *r);
Part *create_tank(Tank *t);
Part *create_engine(Engine *e);
Part *create_decoupler(Decoupler *d);
Stage *create_stage();
Rocket *create_rocket(Datas *d);
Rocket *copy_rocket(Rocket *r);
int create_tank_stack(Datas *d, Stage *s, enum diameter diam, double mass_fuel);
int append_stage(Rocket *r, Stage *s);

#endif

Decoupler* load_Decoupler(char* filename);
