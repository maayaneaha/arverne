#include <math.h>
#include "utils.h"


double calculate_DeltaV(int ISP, double mass_total, double mass_dry, double g)
{
    return ISP * g * log(mass_total / mass_dry);
}

double calculate_TWR(double mass, double thrust, double g)
{
    return thrust / (mass * g);
}

double calculate_g()
{
    return 9.81;
}

double calculate_max_mass(double TWR_min, double thrust, double g)
{
    return thrust/(TWR_min * g);
}

double calculate_mass_fuel(double deltaV, int ISP, double g, double m_tank, double  m_fuel, double m_engines, double m_payload)
{
    //return exp(deltaV / ISP * g) * mass_dry - mass_dry;
    double ve = ISP * g;
    double beta = m_tank / m_fuel;
    return ((m_payload + m_engines) * (exp(deltaV / ve) - 1))/( 1 - beta * (exp(deltaV / ve) - 1));
}
