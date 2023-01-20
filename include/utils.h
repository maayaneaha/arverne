enum diameter {TINY, SMALL, MEDIUM, LARGE, EXTRALARGE, MK2, MK3, X}; // MK1 = SMALL
enum fuel_type {FUELOX, LIQUIDFUEL, MONOPROPELLANT, SOLIDFUEL, XENON, ORE};

struct Tank
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
};

struct Engine
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
    float consumption;
    float gimbal;
};
