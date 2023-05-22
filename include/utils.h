#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include "cjson/cJSON.h"

#if DEBUG
#include "debug.h"
#endif

#define NBR_SEARCH_STAGES 1 // number of stages for the algopti search
#define INF 999999999
#define NBR_MAX_ENGINES 8
#define MAX_TANK 40
#define OXFUEL_DENSITY 5
#define FUEL_COST 0.8
#define OX_COST 0.18
#define NBR_DIAMS 8

enum diameter {TINY = 0, SMALL = 1, MEDIUM = 2, LARGE = 3, EXTRALARGE = 4, MK2 = 5, MK3 = 6, X = 7}; // MK1 = SMALL
enum fuel_type {FUELOX, LIQUIDFUEL, MONOPROPELLANT, SOLIDFUEL, XENON, ORE, ELETRIC};


typedef struct tank Tank;
typedef struct decoupler Decoupler;
typedef struct rocket Rocket;
typedef struct part Part;
typedef struct datas Datas;
typedef struct stage Stage;
typedef struct engine Engine;

struct tank
{
    char *name;
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

struct engine
{
    char *name;
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
    enum diameter diam;
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
	int nbr_engines;
    Part *first_tank;
    Part *engine;
    Part *decoupler;
    enum diameter top_diam;
    enum diameter down_diam;
    Stage *prev;
    Stage *next;
};
/*
datas->deltaV_min
datas->TWR_min
datas->TWR_max
datas->nbr_engines
datas->mass_payload

// et pour stage
stage[i]->total_mass
stage[i]->DeltaV
stage[i]->cost
stage[i]->ISP_vac
stage[i]->ISP_atm
stages[i]->TWR_min
stages[i]->TWR_max

//et des liste chainées dans la struct de Stage
engines[i]->name
tanks[i]->name
decouplers[i]->name
*/

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
    Tank ***tanks;
    size_t nbr_tanks[NBR_DIAMS];
    Engine ***engines;
    size_t nbr_engines[NBR_DIAMS];
    Decoupler ***decouplers;
    size_t nbr_decouplers[NBR_DIAMS];
    Rocket *best_rocket;
    double beta;
};


Datas *create_datas();
double calculate_mass_fuel_tank(Tank *t);
int calculate_stage_infos(Stage *s, Rocket* r);
int calculate_rocket_infos(Rocket *r);
Part *create_tank(Tank *t);
Part *create_engine(Engine *e);
Part *create_decoupler(Decoupler *d);
Stage *create_stage(Datas *d, enum diameter top_diam);
Rocket *create_rocket(Datas *d);
Rocket *copy_rocket(Rocket *r);
int create_tank_stack(Datas *d, Stage *s, double mass_fuel);
int append_stage(Rocket *r, Stage *s);


#endif

Decoupler* load_Decoupler(char* filename);
cJSON* json_ParseFile(char* filename);
