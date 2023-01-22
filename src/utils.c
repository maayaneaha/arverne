double calculate_mass_dry_tank(Tank t)
{
    return t->full_mass - t->empty_mass;
}


int calculate_stage_masses(Stage s)
{
    s->mass_fuel = 0;
    s->mass_dry = s->engine->mass * s->nbr_engine;
    s->cost = s->engine->cost * s->nbr_engine;
    Part tank = s->first_tank;
    s->fuel = tank->part_type->fuel;
    while(fuel != NULL)
    {
        s->mass_fuel += tank->part_type->empty_mass;
        s->mass_dry += calculate_mass_dry_tank(tank->part_type);
        s->cost += tank->cost;
        tank = tank->next;
    }
}
