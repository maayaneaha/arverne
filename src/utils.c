enum diameter = {TINY, SMALL, LARGE, EXTRALARGE, MK2, MK3, X}; // MK1 = SMALL and no MEDIUM
enum fuel_type = {FUELOX, LIQUIDFUEL, MONOPROPELLANT, SOLIDFUEL, XENON, ORE};

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
    float quantity_fuel1;
    float quantity_fuel2;
    int radial_fitting; // Parts can be fit on radial
    int radial_part; // Is a radial tank
}
