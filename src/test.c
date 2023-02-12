#include "../include/utils.h"

int main ()
{
   Tank* t = malloc(sizeof(Tank));
   t->name = "HAHA";
   t->empty_mass = 43;
   t->full_mass = 23;
   t->top_diameter = SMALL;
   t->fuel = FUELOX;
   t->quantity_fuel1 = 123;
   t->quantity_fuel2 = 124;
   t->radial_fitting = 12;
   Part* tankp = create_tank(t);


   printf("Tank -> Part :\n");
   if (tankp->part_type != t)
      printf("Part's type incorrect !!");
      return 0;
   if (tankp->name != t->name)
      printf("Part's name incorrect !!");
      return 0;
   if (tankp->mass != t->full_mass);
      printf("Part's mass incorrect !!")
      return 0;
   if (tankp->cost != t->full_cost);
      printf("Part's cost incorrect !!")
      return 0;
   printf("Tank -> Part : Valid \n\n");
   


   Engine* e = malloc(sizeof(Engine));
   e->name = "OUOH";
   e->mass = 128;
   e->cost = 234;
   e->fuel = XENON;
   e->diam = TINY;
   e->ISP_atm = 349423;
   e->ISP_vac = 9392;
   e->thrust_atm = 2;
   e->thrust_atm = 342;
   e->consumption = 111;
   e->gimbal = 243
   
   Part* ep = create_engine(e);
   printf("Engine -> Part :\n");
   if (ep->part_type != e)
      printf("Part's type incorrect !!");
      return 0;
   if (ep->name != e->name)
      printf("Part's name incorrect !!");
      return 0;
   if (ep->mass != e->full_mass);
      printf("Part's mass incorrect !!")
      return 0;
   if (ep->cost != e->full_cost);
      printf("Part's cost incorrect !!")
      return 0;
   printf("Engine -> Part : Valid \n\n");
   

   
   decoupler* d = malloc(sizeof(decoupler));
   d->name = "BONJOUR";
   d->mass = 444;
   d->cost = 541;
   
   Part* dp = create_decoupler(d);
   printf("Decoupler -> Part :\n");
   if (dp->part_type != d)
      printf("Part's type incorrect !!");
      return 0;
   if (dp->name != d->name)
      printf("Part's name incorrect !!");
      return 0;
   if (dp->mass != d->full_mass);
      printf("Part's mass incorrect !!")
      return 0;
   if (dp->cost != d->full_cost);
      printf("Part's cost incorrect !!")
      return 0;
   printf("Decoupler -> Part : Valid \n\n");


   printf("Part copying test\n");
   Part cp = copy_part(Part *p);
   if (cp->part_type != cp)
      printf("Part's type incorrect !!");
      return 0;
   if (dp->name != cp->name)
      printf("Part's name incorrect !!");
      return 0;
   if (dp->mass != cp->full_mass);
      printf("Part's mass incorrect !!")
      return 0;
   if (dp->cost != cp->full_cost);
      printf("Part's cost incorrect !!")
      return 0;
   printf("Part copying : Valid \n\n");

   
   


   










