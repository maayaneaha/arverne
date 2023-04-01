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

int calculate_isp(Engine* e)
{ //TODO
    return e->ISP_atm;
}

    int nbr_engines_needed()
{ //TODO
    return 1;
}

//double calculate_mass_fuel(double deltaV, int ISP, double g, double m_tank, double  m_fuel, double m_engines, double m_payload)
double calculate_mass_fuel(double deltaV, int ISP, double g, double beta, double m_engines, double m_payload)
{ //res = calculate_mass_fuel(1000, 100, 10, 100, 1000, 1000, 1000);
    //return exp(deltaV / ISP * g) * mass_dry - mass_dry;
    double ve = ISP * g;
    //double beta = m_tank / m_fuel;
    return ((m_payload + m_engines) * (exp(deltaV / ve) - 1))/( 1 - beta * (exp(deltaV / ve) - 1));
}

// pressure_alt : return the pressure for a given altitude (kPa)
// parameters   : double alt = altitude (m)
double pressure_alt (double alt)
{
   return (101.325 * exp((-alt/5600)));
}

// altitude_isp : return Isp for a given altitude
// parameters   : struct Engine e : the engine
//	          int alt : altitude (m)
int altitude_isp (Engine* e,int alt)
{
   double p = pressure_alt ( (double) alt);
   return (int) (e->ISP_vac+(e->ISP_atm-e->ISP_vac)*p);
}

