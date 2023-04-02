#include <math.h>
#include "physics/physics.h"

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
    return 10;
}

double calculate_max_mass(double TWR_min, double thrust, double g)
{
    return thrust/(TWR_min * g);
}

int calculate_isp(Engine* e)
{ //TODO
#if DEBUG
    printf("calculate_isp(%zu)\n", (size_t) e);
#endif
    return e->ISP_atm;
}

    int nbr_engines_needed()
{ //TODO
    return 1;
}

//double calculate_mass_fuel(double deltaV, int ISP, double g, double m_tank, double  m_fuel, double m_engines, double m_payload)
double calculate_mass_fuel(double deltaV, int ISP, double g, double beta, double m_engines, double m_payload)
{ //res = calculate_mass_fuel(1000, 100, 10, 100, 1000, 1000, 1000);
#if DEBUG
    printf("calculate_mass_fuel(deltaV = %f, ISP = %u, g = %f, beta = %f,  m_engines = %f, m_payload = %f)\n",
           deltaV, ISP, g, beta, m_engines, m_payload);
#endif
    //return exp(deltaV / ISP * g) * mass_dry - mass_dry;
    double ve = ISP * g;
    //double beta = m_tank / m_fuel;
    return ((m_payload + m_engines) * (exp(deltaV / ve) - 1))/( 1 - beta * (exp(deltaV / ve) - 1));
}
