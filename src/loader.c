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

Engine*** load_Engines(char* path)
{
	int default_size = 50;
	int position = 0;
	char** files = malloc(sizeof(char*) * default_size);
	files[0] = NULL;
	listFilesRecursively(path, files, &default_size, &position);
	Engine*** engines = malloc(sizeof(Engine**) * (NBR_DIAMS+1));
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        engines[i] = malloc(sizeof(Engine*) * (position + 1));
    }
	engines[NBR_DIAMS] = NULL;
	int cur[NBR_DIAMS] = {0};

	for (int i = 0; files[i] != NULL; i++)
	{
		Engine* tmp = load_Engine(files[i]);
		if (tmp != NULL)
		{
            if (tmp->diam >  X)
                err(2, "tmp->name = %s, tmp->diam = %i", tmp->name, tmp->diam);
			engines[tmp->diam][cur[tmp->diam]++] = tmp;
			free(files[i]);
		}
	}
	free(files);
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        engines[i][cur[i]] = NULL;
    }
	return engines;
}

Decoupler*** load_Decouplers(char* path) {
    printf("fwefwf\n");
    int default_size = 50;
    int position = 0;
    char **files = malloc(sizeof(char *) * default_size);
    files[0] = NULL;
    listFilesRecursively(path, files, &default_size, &position);
    Decoupler ***decouplers = malloc(sizeof(Decoupler * ) * (NBR_DIAMS + 1));
    for (size_t i = 0; i < NBR_DIAMS; i++) {
        decouplers[i] = malloc(sizeof(Decoupler * ) * (position + 1));
    }
	decouplers[NBR_DIAMS] = NULL;
    int cur[NBR_DIAMS] = {0};

    for (int i = 0; files[i] != NULL; i++) {
        Decoupler *tmp = load_Decoupler(files[i]);
        if (tmp != NULL) {
            printf("tmp->diam: %u\n", tmp->diam);
            decouplers[tmp->diam][cur[tmp->diam]++] = tmp;
        }
        free(files[i]);
    }
    free(files);
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        decouplers[i][cur[i]] = NULL;
    }
	return decouplers;
}

Tank*** load_Tanks(char* path) {
    int default_size = 50;
    int position = 0;
    char **files = malloc(sizeof(char *) * default_size);
    files[0] = NULL;
    listFilesRecursively(path, files, &default_size, &position);
    Tank ***tanks = malloc(sizeof(Tank * *) * (NBR_DIAMS + 1));
    for (size_t i = 0; i < NBR_DIAMS; i++) {
        tanks[i] = malloc(sizeof(Tank * ) * (position + 1));
    }
	tanks[NBR_DIAMS] = NULL;
    int cur[NBR_DIAMS] = {0};

    for (int i = 0; files[i] != NULL; i++) {
        Tank *tmp = load_Tank(files[i]);
        if (tmp != NULL) {
            if (tmp->top_diam != tmp->down_diam)
            tanks[tmp->down_diam][cur[tmp->down_diam]++] = tmp;
            tanks[tmp->top_diam][cur[tmp->top_diam]++] = tmp;
        }
        free(files[i]);
    }
    free(files);
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        tanks[i][cur[i]] = NULL;
    }
	return tanks;
}

void insert_sort_Tanks(Tank** tanks, Tank* elt)
{
	int i = 0;
	while (tanks[i] != NULL && elt->full_mass < tanks[i]->full_mass)
		i++;

	int j;
	for (j = 0; tanks[j] != NULL; j++);
	tanks[j+1] = NULL;
	for (; j > i; j--)
	{
		tanks[j] = tanks[j-1];
	}
	tanks[i] = elt;
}

Tank*** sort_Tanks(Tank** tanks)
{
	int count_size[8] = {1, 1, 1, 1, 1, 1, 1, 1};

	Tank*** result = malloc(sizeof(Tank**) * (8));
	for (int i = 0; tanks[i] != NULL; i++)
	{
		if (tanks[i]->top_diam == tanks[i]->down_diam)
		{
			count_size[tanks[i]->top_diam]++;
		}
		else
		{
			count_size[tanks[i]->top_diam]++;
			count_size[tanks[i]->down_diam]++;
		}
	}

	for (int i = 0; i < 8; i++)
	{
		result[i] = malloc(sizeof(Tank*) * (count_size[i]));
		result[i][0] = NULL;
	}

	for (int i = 0; tanks[i] != NULL; i++)
	{
		if (tanks[i]->top_diam == tanks[i]->down_diam)
		{
			insert_sort_Tanks(result[tanks[i]->top_diam], tanks[i]);
		}
		else
		{
			insert_sort_Tanks(result[tanks[i]->top_diam], tanks[i]);
			insert_sort_Tanks(result[tanks[i]->down_diam], tanks[i]);
		}
	}

	return result;
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
	   if (cJSON_GetArraySize(tmp) < 7)
	   {
		   obj->top_diam = 1;
	   }
	   else
	   {
		  tmp = cJSON_GetArrayItem(tmp,6);
		  if (tmp->valueint == 0)
			 obj->top_diam = TINY;
		  if (tmp->valueint == 1)
			 obj->top_diam = SMALL;
		  if (tmp->valueint == 2)
			 obj->top_diam = LARGE;
		  if (tmp->valueint == 3)
			 obj->top_diam = EXTRALARGE;
	   }
   }
   else
	   return NULL;
      /*errx(1,"node_stack_top is not found");   */
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_bottom");
   if(cJSON_IsArray(tmp))
   {
	   if (cJSON_GetArraySize(tmp) < 7)
	   {
		   obj->down_diam = 1;
	   }
	   else
	   {
		  tmp = cJSON_GetArrayItem(tmp,6);
		  if (tmp->valueint == 0)
			obj->down_diam = TINY;
		  if (tmp->valueint == 1)
			obj->down_diam = SMALL;
		  if (tmp->valueint == 2)
			obj->down_diam = LARGE;
		  if (tmp->valueint == 3)
			obj->down_diam = EXTRALARGE;
	   }
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
	   if (cJSON_GetArraySize(tmp) < 7)
	   {
		   obj->diam = 1;
	   }
	   else
	   {
			tmp = cJSON_GetArrayItem(tmp, 6);
			  if (tmp->valueint == 0)
				obj->diam = TINY;
			  if (tmp->valueint == 1)
				obj->diam = SMALL;
			  if (tmp->valueint == 2)
				obj->diam = LARGE;
			  if (tmp->valueint == 3)
				obj->diam = EXTRALARGE;
	   }
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

    tmp = cJSON_GetObjectItemCaseSensitive(part, "node_stack_top");
    if (cJSON_IsArray(tmp))
	{
	   if (cJSON_GetArraySize(tmp) < 7)
	   {
		   obj->diam = 1;
	   }
	   else
	   {
			tmp = cJSON_GetArrayItem(tmp, 6);
			obj->diam = tmp->valueint;
    		cJSON* tmp2 = cJSON_GetObjectItemCaseSensitive(part, "node_stack_down");
			if (cJSON_IsArray(tmp))
			{
			   if (cJSON_GetArraySize(tmp) >= 7)
			   {
					tmp = cJSON_GetArrayItem(tmp2, 6);
					if ((enum diameter) tmp->valueint != obj->diam)
						errx(1, "diameter in coupling component with different sizes");
			   }
			}
	   }
	}
	else
		return NULL;

    cJSON_Delete(file);
    return obj;
}

int load_parts(Datas *d)
{
    d->tanks = load_Tanks("bdd/FuelTank");
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        size_t nbr_tanks;
        for (nbr_tanks = 0; d->tanks[i][nbr_tanks] != NULL; nbr_tanks++);
        d->nbr_tanks[i] = nbr_tanks;
    }
    d->engines = load_Engines("bdd/Engine");
    printf("q0\n");
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        size_t nbr_engines;
        for (nbr_engines = 0; d->engines[i][nbr_engines] != NULL; nbr_engines++);
        d->nbr_engines[i] = nbr_engines;
    }
    printf("q1\n");
    d->decouplers = load_Decouplers("bdd/Coupling");
    for (size_t i = 0; i < NBR_DIAMS; i++)
    {
        printf("ewfwe%zu\n", i);
        size_t nbr_decouplers;
        for (nbr_decouplers = 0; d->decouplers[i][nbr_decouplers] != NULL; nbr_decouplers++);
        d->nbr_decouplers[i] = nbr_decouplers;
    }
    printf("q2\n");
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

Tank* import_json_Tank(cJSON* obj)
{
	if (obj == NULL)
		return NULL;

	Tank* result = malloc(sizeof(Tank));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "name");
	result->name = tmp_value->valuestring;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "empty_mass");
	result->empty_mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "full_mass");
	result->full_mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "empty_cost");
	result->empty_cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "full_cost");
	result->full_cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "top_diam");
	result->top_diam = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "down_diam");
	result->down_diam = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "fuel");
	result->fuel = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "quantity_fuel1");
	result->quantity_fuel1 = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "quantity_fuel2");
	result->quantity_fuel2 = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "radial_part");
	result->radial_part = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "radial_fitting");
	result->radial_fitting = tmp_value->valueint;

	return result;
}

Engine* import_json_Engine(cJSON* obj)
{
	if (obj == NULL)
		return NULL;

	Engine* result = malloc(sizeof(Engine));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "name");
	result->name = tmp_value->valuestring;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass");
	result->mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "cost");
	result->cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "diam");
	result->diam = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "thrust_atm");
	result->thrust_atm = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "thrust_vac");
	result->thrust_vac = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "consumption");
	result->consumption = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "gimbal");
	result->gimbal = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "fuel");
	result->fuel = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "ISP_atm");
	result->ISP_atm = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "ISP_vac");
	result->ISP_vac = tmp_value->valueint;

	return result;
}

Decoupler* import_json_Decoupler(cJSON* obj)
{
	Decoupler* result = malloc(sizeof(Decoupler));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "name");
	result->name = tmp_value->valuestring;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass");
	result->mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "cost");
	result->cost = tmp_value->valuedouble;

	return result;
}

Part* import_json_Part(cJSON* obj, int type)
{
	// type = 0 si tank
	// type = 1 si engine
	// type = 2 si decoupler
	//
	if (obj == NULL)
		return NULL;

	Part* result = malloc(sizeof(Part));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "name");
	result->name = tmp_value->valuestring;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass");
	result->mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "cost");
	result->cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "prev");
	result->prev = import_json_Part(tmp_value, type);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "next");
	result->next = import_json_Part(tmp_value, type);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "part_type");
	if (type == 0)
	{
		result->part_type = import_json_Tank(tmp_value);
	}
	else if (type == 1)
	{
		result->part_type = import_json_Engine(tmp_value);
	}
	else if (type == 2)
	{
		result->part_type = import_json_Decoupler(tmp_value);
	}
	return result;
	

	return result;
}

Stage* import_json_Stage(cJSON* obj)
{
	if (obj == NULL)
		return NULL;

	Stage* result = malloc(sizeof(Stage));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass_full");
	result->mass_full = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass_dry");
	result->mass_dry = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "cost");
	result->cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "fuel");
	result->fuel = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "quantity_fuel1");
	result->quantity_fuel1 = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "quantity_fuel2");
	result->quantity_fuel2 = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "DeltaV");
	result->DeltaV = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "total_thrust_atm_min");
	result->total_thrust_atm_min = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "total_thrust_atm_max");
	result->total_thrust_atm_max = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "total_thrust_vac_min");
	result->total_thrust_vac_min = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "total_thrust_vac_max");
	result->total_thrust_vac_max = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "ISP_atm");
	result->ISP_atm = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "ISP_vac");
	result->ISP_vac = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "TWR_min");
	result->TWR_min = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "TWR_max");
	result->TWR_max = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "consumption");
	result->consumption = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "nbr_engines");
	result->nbr_engines = tmp_value->valueint;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "first_tank");
	result->first_tank = import_json_Part(tmp_value, 0);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "engine");
	result->engine = import_json_Part(tmp_value, 1);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "decoupler");
	result->decoupler = import_json_Part(tmp_value, 2);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "prev");
	result->next = import_json_Stage(tmp_value);

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "next");
	result->next = import_json_Stage(tmp_value);
	
	return result;
}

Rocket* import_json_Rocket(cJSON* obj)
{
	if (obj == NULL)
		return NULL;

	Rocket* result = malloc(sizeof(Rocket));
	cJSON* tmp_value = NULL;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "mass_payload");
	result->mass_payload = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "total_mass");
	result->total_mass = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "DeltaV");
	result->DeltaV = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "cost");
	result->cost = tmp_value->valuedouble;

	tmp_value = cJSON_GetObjectItemCaseSensitive(obj, "first_stage");
	result->first_stage = import_json_Stage(tmp_value);

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
    
	tmp_value = cJSON_CreateArray();
	for (int i = 0; i < NBR_DIAMS; i++)
	{
		cJSON* j = cJSON_CreateNumber(d->nbr_tanks[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "nbr_tanks", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (int i = 0; i < NBR_DIAMS; i++)
	{
		cJSON* j = cJSON_CreateNumber(d->nbr_engines[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "nbr_engines", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (int i = 0; i < NBR_DIAMS; i++)
	{
		cJSON* j = cJSON_CreateNumber(d->nbr_decouplers[i]);
		cJSON_AddItemToArray(tmp_value, j);
	}
	cJSON_AddItemToObject(object, "nbr_decouplers", tmp_value);

	tmp_value = cJSON_CreateNumber(d->beta);
	cJSON_AddItemToObject(object, "beta", tmp_value);
	
	tmp_value = cJSON_CreateArray();
	for (int k = 0; k < NBR_DIAMS; k++)
	{
		cJSON* tmp_value2 = cJSON_CreateArray();
		for (size_t i = 0; i < d->nbr_decouplers[k]; i++)
		{
			cJSON* j = export_to_json_Decoupler(d->decouplers[k][i]);
			cJSON_AddItemToArray(tmp_value2, j);
		}
		cJSON_AddItemToArray(tmp_value, tmp_value2);
	}
	cJSON_AddItemToObject(object, "decouplers", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (int k = 0; k < NBR_DIAMS; k++)
	{
		cJSON* tmp_value2 = cJSON_CreateArray();
		for (size_t i = 0; i < d->nbr_engines[k]; i++)
		{
			cJSON* j = export_to_json_Engine(d->engines[k][i]);
			cJSON_AddItemToArray(tmp_value2, j);
		}
		cJSON_AddItemToArray(tmp_value, tmp_value2);
	}
	cJSON_AddItemToObject(object, "engines", tmp_value);

	tmp_value = cJSON_CreateArray();
	for (int k = 0; k < NBR_DIAMS; k++)
	{
		cJSON* tmp_value2 = cJSON_CreateArray();
		for (size_t i = 0; i < d->nbr_tanks[k]; i++)
		{
			cJSON* j = export_to_json_Tank(d->tanks[k][i]);
			cJSON_AddItemToArray(tmp_value2, j);
		}
		cJSON_AddItemToArray(tmp_value, tmp_value2);
	}
	cJSON_AddItemToObject(object, "tanks", tmp_value);

	tmp_value = export_to_json_Rocket(d->best_rocket);
	cJSON_AddItemToObject(object, "best_rocket", tmp_value);

	// faut cJSON_Print(object) et écrire le résultat dans le fichier

	FILE* ptr = fopen(path,"w");
	char* result = cJSON_Print(object);
	if (result == NULL)
	{
		fclose(ptr);
		return 1;
	}

	fprintf(ptr,"%s", result);
	fclose(ptr);
	return 0;
}
