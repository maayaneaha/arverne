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
    char *name;
    double mass;
    double cost;
    int max_temp;
    int tolerance_ms;
    int tolerance_g;
    int ejection;
} Decoupler;

typedef struct stage Stage;
typedef struct part Part;

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
    double mass;
    enum fuel_type fuel;
    double quantity_fuel1;
    double quantity_fuel2;
    double delta_v;
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
};

typedef struct
{
    double mass_payload;
    double total_mass;
    double total_DV;
    Stage *first_stage;
} Rocket;


Decoupler* load_Decoupler(char* filename);
