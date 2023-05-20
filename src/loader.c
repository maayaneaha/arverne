#define _GNU_SOURCE

#include <string.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <dirent.h>

#include "cjson/cJSON.h"
#include "utils.h"
#include "loader.h"

#define OXFUEL_DENSITY 5

void listFilesRecursively(char *basePath, char** list, int* size, int* pos)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
			if (dp->d_type == DT_REG)
			{
				if ((*pos) >= (*size))
				{
					*size += 50;
					list = realloc(list, (*size) * sizeof(char*));
				}
				/*char* cur = malloc(sizeof(char) * (strlen(dp->d_name) + 1));*/
				/*strcpy(cur, dp->d_name);*/
				char* cur = NULL;
				asprintf(&cur, "%s/%s", basePath, dp->d_name);
				list[(*pos)++] = cur;
				list[(*pos)] = NULL;
			}
			else
			{

				strcpy(path, basePath);
				strcat(path, "/");
				strcat(path, dp->d_name);
				listFilesRecursively(path, list, size, pos);
			}
        }
    }

    closedir(dir);
}

Engine** load_Engines(char* path)
{
	int default_size = 50;
	int position = 0;
	char** files = malloc(sizeof(char*) * default_size);
	files[0] = NULL;
	listFilesRecursively(path, files, &default_size, &position);
	Engine** engines = malloc(sizeof(Engine*) * (position + 1));
	int cur = 0;

	for (int i = 0; files[i] != NULL; i++)
	{
		Engine* tmp = load_Engine(files[i]);
		if (tmp != NULL)
		{
			engines[cur++] = tmp;
		}
		free(files[i]);
	}
	free(files);
	engines[cur] = NULL;

	return engines;
}

Decoupler** load_Decouplers(char* path)
{
	int default_size = 50;
	int position = 0;
	char** files = malloc(sizeof(char*) * default_size);
	files[0] = NULL;
	listFilesRecursively(path, files, &default_size, &position);
	Decoupler** decouplers = malloc(sizeof(Decoupler*) * (position + 1));
	int cur = 0;

	for (int i = 0; files[i] != NULL; i++)
	{
		Decoupler* tmp = load_Decoupler(files[i]);
		if (tmp != NULL)
		{
			decouplers[cur++] = tmp;
		}
		free(files[i]);
	}
	free(files);
	decouplers[cur] = NULL;

	return decouplers;
}

Tank** load_Tanks(char* path)
{
	int default_size = 50;
	int position = 0;
	char** files = malloc(sizeof(char*) * default_size);
	files[0] = NULL;
	listFilesRecursively(path, files, &default_size, &position);
	Tank** tanks = malloc(sizeof(Tank*) * (position + 1));
	int cur = 0;

	for (int i = 0; files[i] != NULL; i++)
	{
		Tank* tmp = load_Tank(files[i]);
		if (tmp != NULL)
		{
			tanks[cur++] = tmp;
		}
		free(files[i]);
	}
	free(files);
	tanks[cur] = NULL;

	return tanks;
}

Tank* load_Tank(char* filename)
{
   Tank* obj = malloc(sizeof(Tank));
   cJSON* file = json_ParseFile(filename);
   cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
   cJSON* tmp = NULL;
   
   if (!part)
   {
	   return NULL;
      /*errx(1,"Incorrect tank file");*/
   }

   tmp = cJSON_GetObjectItemCaseSensitive(part,"name");
   if (tmp)
   {
      asprintf(&obj->name, "%s",tmp->valuestring);
   }
   else
	   return NULL;
      /*errx(1,"name is not found");*/

   tmp = cJSON_GetObjectItemCaseSensitive(part,"mass");
   if (tmp)
   {
      if (tmp->valuedouble)
         obj->empty_mass = tmp->valuedouble * 1000;
   }
   else
	   return NULL;
      /*errx(1,"mass is not found");*/

   tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
   if (tmp)
   {
      if (tmp->valuedouble)
         obj->full_cost = tmp->valuedouble;
      else
         obj->full_cost = (double) tmp->valueint;
   }
   else
	   return NULL;
      /*errx(1,"cost is not found");*/
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_top");
   if(cJSON_IsArray(tmp))
   {
      tmp = cJSON_GetArrayItem(tmp,5);
      if (tmp->valueint == 0)
		 obj->top_diam = TINY;
      if (tmp->valueint == 1)
		 obj->top_diam = SMALL;
      if (tmp->valueint == 2)
		 obj->top_diam = LARGE;
      if (tmp->valueint == 3)
		 obj->top_diam = EXTRALARGE;
   }
   else
	   return NULL;
      /*errx(1,"node_stack_top is not found");   */
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_bottom");
   if(cJSON_IsArray(tmp))
   {
      tmp = cJSON_GetArrayItem(tmp,5);
      if (tmp->valueint == 0)
	 	obj->down_diam = TINY;
      if (tmp->valueint == 1)
	 	obj->down_diam = SMALL;
      if (tmp->valueint == 2)
	 	obj->down_diam = LARGE;
      if (tmp->valueint == 3)
	 	obj->down_diam = EXTRALARGE;
   }
   else
	   return NULL;
      /*errx(1,"node_stack_bottom is not found");*/

   obj->fuel = FUELOX;
   tmp = cJSON_GetObjectItemCaseSensitive(part, "RESOURCE");
   if(cJSON_IsArray(tmp))
   {
      tmp = cJSON_GetArrayItem(tmp,0);
      obj->quantity_fuel1 =(cJSON_GetObjectItemCaseSensitive(tmp, "maxAmount"))->valueint;
      cJSON* tmp = cJSON_GetObjectItemCaseSensitive(part, "RESOURCE");
      tmp = cJSON_GetArrayItem(tmp,1);
      obj->quantity_fuel2 =(cJSON_GetObjectItemCaseSensitive(tmp, "maxAmount"))->valueint;
   }
   else
	   return NULL;
      /*errx(1,"RESOURCE is not found");*/

   tmp = cJSON_GetObjectItemCaseSensitive(part,"attachRules");
   if(cJSON_IsArray(tmp))
   {
      obj->radial_fitting = (cJSON_GetArrayItem(tmp,3))->valueint; 
      obj->radial_part = (cJSON_GetArrayItem(tmp,0))->valueint;
   }
   obj->full_mass = obj->empty_mass + (obj->quantity_fuel1 + obj->quantity_fuel2) * OXFUEL_DENSITY;
   // empty_cost = full_cost - quantity_fuel1 * FUEL_COST - quantity_fuel2 * OX_COST;
   obj->empty_cost = obj->full_cost - obj->quantity_fuel1 * FUEL_COST - obj->quantity_fuel2 * OX_COST;

   return obj;
}

cJSON* json_findElement(cJSON* file, char** key, int src)
{
	cJSON* obj = NULL;
	cJSON* tmp = NULL;
	size_t key_size;
	if ((tmp = cJSON_GetObjectItemCaseSensitive(file, key[src])) == NULL)
		return NULL;
	else
	{
		src++;
		if (cJSON_IsArray(tmp)) 
		{
			 cJSON* i = NULL; 
			 cJSON_ArrayForEach(i, tmp) 
			 { 
				if ((obj = json_findElement(i, key, src)) != NULL)
					break;
			 }
		}
		else
		{
			obj = json_findElement(tmp, key, src);
		}
	}

	return obj; 
}

Engine* load_Engine(char* filename)
{
	Engine* obj = malloc(sizeof(Engine));
	cJSON* file = json_ParseFile(filename);
	cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
	cJSON* tmp = NULL;
    if (!part)
	{
	   return NULL;
        /*errx(1, "Incorrect decoupler file");*/
	}

    tmp = cJSON_GetObjectItemCaseSensitive(part, "name");
    if (tmp)
	{
        asprintf(&obj->name, "%s", tmp->valuestring);
	}
	else
		return NULL;

    tmp = cJSON_GetObjectItemCaseSensitive(part, "mass");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->mass = tmp->valuedouble * 1000;
    }
	else
		return NULL;

    tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->cost = tmp->valuedouble;
        else
            obj->cost = (double) tmp->valueint;
    }
	else
		return NULL;

    tmp = cJSON_GetObjectItemCaseSensitive(part, "node_stack_top");
    if (tmp)
	{
		tmp = cJSON_GetArrayItem(tmp, 5);
		if (tmp->valuedouble)
			obj->diam = tmp->valuedouble;
	}
	else
		return NULL;

	 int max_thrust = 0;
	 tmp = cJSON_GetObjectItemCaseSensitive(part, "MODULE"); 
	 if (tmp) 
	 { 
		 if (cJSON_IsArray(tmp)) 
		 { 
			 cJSON* i = NULL; 
			 cJSON* j = NULL; 
			 cJSON* l = NULL;

			 cJSON_ArrayForEach(i, tmp) 
			 { 
				  j = cJSON_GetObjectItemCaseSensitive(i, "maxThrust");
				  if (j)
					  max_thrust = j->valueint * 1000;

				  j = cJSON_GetObjectItemCaseSensitive(i, "gimbalRange");
				  if (j)
				  {
					  obj->gimbal = j->valuedouble;
				  }

				  j = cJSON_GetObjectItemCaseSensitive(i, "EngineType");
				  if (j)
				  {
					  if (strcmp(j->valuestring, "LiquidFuel") == 0)
						  obj->fuel = FUELOX;
                      else
                          return NULL;/*
					  else if (strcmp(j->valuestring, "Eletric") == 0)
						  obj->fuel = ELETRIC;
					  else if (strcmp(j->valuestring, "MonoProp") == 0)
						  obj->fuel = MONOPROPELLANT;
					  else if (strcmp(j->valuestring, "SolidBooster") == 0)
						  obj->fuel = SOLIDFUEL;
                      else
                          obj->fuel = ELETRIC;*/
				  }

				  j = cJSON_GetObjectItemCaseSensitive(i, "atmosphereCurve");
				  if (j)
				  {
				  	j = cJSON_GetObjectItemCaseSensitive(j, "key");
			 		cJSON_ArrayForEach(l, j) 
					{
						cJSON* k = cJSON_GetArrayItem(l, 0);
						if (cJSON_IsNumber(k))
						{

							if (((double)k->valueint) != k->valuedouble)
								continue;

							if (k->valueint == 0)
							{
								k = cJSON_GetArrayItem(l, 1);
								obj->ISP_vac = k->valueint;
							}
							else if (k->valueint == 1)
							{
								k = cJSON_GetArrayItem(l, 1);
								obj->ISP_atm = k->valueint;
							}
						}
					}	
				} 
		 	} 
		 } 
	 }
	else
		return NULL;

	if (obj->ISP_atm > obj->ISP_vac)
	{
		obj->thrust_atm = max_thrust;
		obj->thrust_vac = obj->thrust_atm * obj->ISP_vac / obj->ISP_atm;
	}
	else
	{
		obj->thrust_vac = max_thrust;
		obj->thrust_atm = obj->thrust_vac * obj->ISP_vac / obj->ISP_atm;
	}


	return obj;
}

Decoupler* load_Decoupler(char* filename)
{
    Decoupler* obj = malloc(sizeof(Decoupler));
    cJSON* file = json_ParseFile(filename);
    cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
    cJSON* tmp = NULL;
    
    if (part == NULL)
	{
	   return NULL;
        /*errx(1, "Incorrect decoupler file");*/
	}

	tmp = cJSON_GetObjectItemCaseSensitive(part, "name");
    if (tmp)
        asprintf(&obj->name, "%s", tmp->valuestring);
	else
		return NULL;

    tmp = cJSON_GetObjectItemCaseSensitive(part, "mass");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->mass = tmp->valuedouble;
    }
	else
		return NULL;

    tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->cost = tmp->valuedouble;
        else
            obj->cost = (double) tmp->valueint;
    }
	else
		return NULL;

    cJSON_Delete(file);
    return obj;
}

int load_parts(Datas *d)
{
    d->tanks = load_Tanks("bdd/FuelTank");
    size_t nbr_tanks;
    for (nbr_tanks = 0; d->tanks[nbr_tanks] != NULL; nbr_tanks++);
    d->nbr_tanks = nbr_tanks;

    d->engines = load_Engines("bdd/Engine");
    size_t nbr_engines;
    for (nbr_engines = 0; d->engines[nbr_engines] != NULL; nbr_engines++);
    d->nbr_engines = nbr_engines;

    d->decouplers = load_Decouplers("bdd/Coupling");
    size_t nbr_decouplers;
    for (nbr_decouplers = 0; d->decouplers[nbr_decouplers] != NULL; nbr_decouplers++);
    d->nbr_decouplers = nbr_decouplers;
    return 1;
}

cJSON* export_to_json_Decoupler(Decoupler* obj)
{
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateString(obj->name);
	cJSON_AddItemToObject(result, "name", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->mass);
	cJSON_AddItemToObject(result, "mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->cost);
	cJSON_AddItemToObject(result, "cost", tmp_value);

	return result;
}

cJSON* export_to_json_Tank(Tank* obj)
{
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateString(obj->name);
	cJSON_AddItemToObject(result, "name", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->empty_mass);
	cJSON_AddItemToObject(result, "empty_mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->full_mass);
	cJSON_AddItemToObject(result, "full_mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->empty_cost);
	cJSON_AddItemToObject(result, "empty_cost", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->full_cost);
	cJSON_AddItemToObject(result, "full_cost", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->top_diam);
	cJSON_AddItemToObject(result, "top_diam", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->down_diam);
	cJSON_AddItemToObject(result, "down_diam", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->fuel);
	cJSON_AddItemToObject(result, "fuel", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->quantity_fuel1);
	cJSON_AddItemToObject(result, "quantity_fuel1", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->quantity_fuel2);
	cJSON_AddItemToObject(result, "quantity_fuel2", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->radial_fitting);
	cJSON_AddItemToObject(result, "radial_fitting", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->radial_part);
	cJSON_AddItemToObject(result, "radial_part", tmp_value);
	
	return result;
}

cJSON* export_to_json_Engine(Engine* obj)
{
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateString(obj->name);
	cJSON_AddItemToObject(result, "name", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->mass);
	cJSON_AddItemToObject(result, "mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->cost);
	cJSON_AddItemToObject(result, "cost", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->fuel);
	cJSON_AddItemToObject(result, "fuel", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->diam);
	cJSON_AddItemToObject(result, "diam", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->ISP_atm);
	cJSON_AddItemToObject(result, "ISP_atm", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->ISP_vac);
	cJSON_AddItemToObject(result, "ISP_vac", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->thrust_atm);
	cJSON_AddItemToObject(result, "thrust_atm", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->thrust_vac);
	cJSON_AddItemToObject(result, "thrust_vac", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->consumption);
	cJSON_AddItemToObject(result, "consumption", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->gimbal);
	cJSON_AddItemToObject(result, "gimbal", tmp_value);

	return result;
}

cJSON* export_to_json_Stage(Stage* obj)
{
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateNumber(obj->mass_full);
	cJSON_AddItemToObject(result, "mass_full", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->mass_dry);
	cJSON_AddItemToObject(result, "mass_dry", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->cost);
	cJSON_AddItemToObject(result, "cost", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->fuel);
	cJSON_AddItemToObject(result, "fuel", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->quantity_fuel1);
	cJSON_AddItemToObject(result, "quantity_fuel1", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->quantity_fuel2);
	cJSON_AddItemToObject(result, "quantity_fuel2", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->DeltaV);
	cJSON_AddItemToObject(result, "DeltaV", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->total_thrust_atm_min);
	cJSON_AddItemToObject(result, "total_thrust_atm_min", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->total_thrust_atm_max);
	cJSON_AddItemToObject(result, "total_thrust_atm_max", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->total_thrust_vac_min);
	cJSON_AddItemToObject(result, "total_thrust_vac_min", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->total_thrust_vac_max);
	cJSON_AddItemToObject(result, "total_thrust_vac_max", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->ISP_atm);
	cJSON_AddItemToObject(result, "ISM_atm", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->ISP_vac);
	cJSON_AddItemToObject(result, "ISM_vac", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->TWR_min);
	cJSON_AddItemToObject(result, "TWR_min", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->TWR_max);
	cJSON_AddItemToObject(result, "TWR_max", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->consumption);
	cJSON_AddItemToObject(result, "consumption", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->nbr_engines);
	cJSON_AddItemToObject(result, "nbr_engines", tmp_value);

	tmp_value = export_to_json_Part(obj->first_tank, 0);
	cJSON_AddItemToObject(result, "first_tank", tmp_value);

	tmp_value = export_to_json_Part(obj->engine, 1);
	cJSON_AddItemToObject(result, "engine", tmp_value);

	tmp_value = export_to_json_Part(obj->decoupler, 2);
	cJSON_AddItemToObject(result, "decoupler", tmp_value);

	tmp_value = export_to_json_Stage(obj->prev);
	cJSON_AddItemToObject(result, "prev", tmp_value);

	tmp_value = export_to_json_Stage(obj->next);
	cJSON_AddItemToObject(result, "next", tmp_value);

	return result;
}

cJSON* export_to_json_Part(Part* obj, int type)
{
	// type = 0 si tank
	// type = 1 si engine
	// type = 2 si decoupler
	
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	if (type == 0)
	{
		tmp_value = export_to_json_Tank(obj->part_type);
	}
	else if (type == 1)
	{
		tmp_value = export_to_json_Engine(obj->part_type);
	}
	else if (type == 2)
	{
		tmp_value = export_to_json_Decoupler(obj->part_type);
	}
	cJSON_AddItemToObject(result, "part_type", tmp_value);

	tmp_value = cJSON_CreateString(obj->name);
	cJSON_AddItemToObject(result, "name", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->mass);
	cJSON_AddItemToObject(result, "mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->cost);
	cJSON_AddItemToObject(result, "cost", tmp_value);

	tmp_value = export_to_json_Part(obj->prev, type);
	cJSON_AddItemToObject(result, "prev", tmp_value);

	tmp_value = export_to_json_Part(obj->next, type);
	cJSON_AddItemToObject(result, "next", tmp_value);

	return result;
}

cJSON* export_to_json_Rocket(Rocket* obj)
{
	if (obj == NULL)
		return cJSON_CreateNull();
	
	cJSON* result = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateNumber(obj->mass_payload);
	cJSON_AddItemToObject(result, "mass_payload", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->total_mass);
	cJSON_AddItemToObject(result, "total_mass", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->DeltaV);
	cJSON_AddItemToObject(result, "DeltaV", tmp_value);

	tmp_value = cJSON_CreateNumber(obj->cost);
	cJSON_AddItemToObject(result, "cost", tmp_value);

	tmp_value = export_to_json_Stage(obj->first_stage);
	cJSON_AddItemToObject(result, "first_stage", tmp_value);

	return result;
}

int generate_datas(Datas *d, char* path)
{
	cJSON* object = cJSON_CreateObject();
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_CreateNumber(d->deltaV_min);
	cJSON_AddItemToObject(object, "deltaV_min", tmp_value);

	tmp_value = cJSON_CreateNumber(d->mass_payload);
	cJSON_AddItemToObject(object, "mass_payload", tmp_value);

	tmp_value = cJSON_CreateNumber(d->mass_max);
	cJSON_AddItemToObject(object, "mass_max", tmp_value);

	tmp_value = cJSON_CreateNumber(d->TWR_min);
	cJSON_AddItemToObject(object, "TWR_min", tmp_value);

	tmp_value = cJSON_CreateNumber(d->TWR_max);
	cJSON_AddItemToObject(object, "TWR_max", tmp_value);

	tmp_value = cJSON_CreateNumber(d->diameter_payload);
	cJSON_AddItemToObject(object, "diameter_value", tmp_value);

	tmp_value = cJSON_CreateNumber(d->nbr_tanks);
	cJSON_AddItemToObject(object, "nbr_tanks", tmp_value);

	tmp_value = cJSON_CreateNumber(d->nbr_engines);
	cJSON_AddItemToObject(object, "nbr_engines", tmp_value);

	tmp_value = cJSON_CreateNumber(d->nbr_decouplers);
	cJSON_AddItemToObject(object, "nbr_decouplers", tmp_value);

	tmp_value = cJSON_CreateNumber(d->beta);
	cJSON_AddItemToObject(object, "beta", tmp_value);
	
	tmp_value = cJSON_CreateArray();
	for (size_t i = 0; i < d->nbr_decouplers; i++)
	{
		cJSON* j = export_to_json_Decoupler(d->decouplers[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "engines", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (size_t i = 0; i < d->nbr_engines; i++)
	{
		cJSON* j = export_to_json_Engine(d->engines[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "engines", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (size_t i = 0; i < d->nbr_tanks; i++)
	{
		cJSON* j = export_to_json_Tank(d->tanks[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "tanks", tmp_value);

	tmp_value = export_to_json_Rocket(d->best_rocket);
	cJSON_AddItemToObject(object, "best_rocket", tmp_value);

	// faut cJSON_Print(object) et écrire le résultat dans le fichier

	FILE* ptr = fopen(path,"w");
	fclose(ptr);
	return 0;
}
