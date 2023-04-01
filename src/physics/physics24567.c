#include <math.h>
#include "physics/physics.h"
#include "utils.h"

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




