enum diameter {TINY, SMALL, MEDIUM, LARGE, EXTRALARGE, MK2, MK3, X}; // MK1 = SMALL
enum fuel_type {FUELOX, LIQUIDFUEL, MONOPROPELLANT, SOLIDFUEL, XENON, ORE};

typedef struct
{
    char *name;
    double empty_mass;
    double full_mass;
    double empty_cost;
    double full_cost;
    enum diameter top_diam;
    enum diameter down_diam;
    enum fuel_type fuel;
    double quantity_fuel1;
    double quantity_fuel2;
    int radial_fitting; // Parts can be fit on radial
    int radial_part; // Is a radial tank
} Tank;

typedef struct
{
    char *name;
    double mass;
    double cost;
    enum fuel_type fuel;
    int ISP_atm;
    int ISP_vac;
    double thrust_atm;
    double thrust_vac;
    double TWR_atm;
    double TWR_vac;
    double consumption;
    double gimbal;
} Engine;

typedef struct
{
    char *name
    double mass;
    double cost;
    int max_temp;
    int toterance_ms;
    int tolerance_g;
    int ejection;
} Decoupler;

typedef struct
{
    void *part_type;
    char *name;
    double mass;
    double cost;
    Part *prev;
    Part *next;
} Part;

typedef struct
{
    double mass_fuel;
    double mass_dry;
    enum fuel_type fuel;
    double quantity_fuel1;
    double quantity_fuel2;
    double DV;
    double total_thurst_atm_min;
    double total_thurst_atm_max;
    double total_thrust_vac_min;
    double total_thurst_vac_max;
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
} Stage;

typedef struct
{
    double mass_payload;
    double mass;
    double DV;
    double cost;
    Stage *first_stage;
} Rocket;

typedef struct
{
    double delva_v_min;
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
} Datas;
