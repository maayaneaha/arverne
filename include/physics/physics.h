#ifndef PHYSIC
#define PHYSIC

double calculate_DeltaV(int ISP, double mass_total, double mass_dry, double g);
double calculate_TWR(double mass, double thrust, double g);
double calculate_g();
double calculate_max_mass(double TWR_min, double thrust, double g);
int calculate_isp(Engine* e);
//double calculate_mass_fuel(double deltaV, int ISP, double g, double m_tank, double  m_fuel, double m_engines, double m_payload);
double calculate_mass_fuel(double deltaV, int ISP, double g, double beta, double m_engines, double m_payload);
#endif