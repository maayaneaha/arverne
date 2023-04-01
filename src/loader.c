#define _GNU_SOURCE

#include <string.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "utils.h"


Tank* load_Tank(char* filename)
{
   Tank* obj = malloc(sizeof(Tank));
   cJSON* file = json_ParseFile(filename);
   cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
   cJSON* tmp = NULL;
   
   if (!part)
      errx(1,"Incorrect tank file");

   tmp = cJSON_GetObjectItemCaseSensitive(part,"name");
   if (tmp)
   {
      asprintf(&obj->name, "%s",tmp->valuestring);
   }
   else
      errx(1,"name is not found");

   tmp = cJSON_GetObjectItemCaseSensitive(part,"mass");
   if (tmp)
   {
      if (tmp->valuedouble)
         obj->empty_mass = tmp->valuedouble;
   }
   else
      errx(1,"mass is not found");

   tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
   if (tmp)
   {
      if (tmp->valuedouble)
         obj->full_cost = tmp->valuedouble;
      else
         obj->full_cost = (double) tmp->valueint;
   }
   else
      errx(1,"cost is not found");
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_top");
   if(cJSON_IsArray(tmp))
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
   else
      errx(1,"node_stack_top is not found");   
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_bottom");
   if(cJSON_IsArray(tmp))
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
   else
      errx(1,"node_stack_bottom is not found");

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
      errx(1,"RESOURCE is not found");

   tmp = cJSON_GetObjectItemCaseSensitive(part,"attachRules");
   if(cJSON_IsArray(tmp))
   {
      obj->radial_fitting = (cJSON_GetArrayItem(tmp,3))->valueint; 
      obj->radial_part = (cJSON_GetArrayItem(tmp,0))->valueint;
   }
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
        errx(1, "Incorrect decoupler file");
	}

    tmp = cJSON_GetObjectItemCaseSensitive(part, "name");
    if (tmp)
	{
        asprintf(&obj->name, "%s", tmp->valuestring);
	}

    tmp = cJSON_GetObjectItemCaseSensitive(part, "mass");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->mass = tmp->valuedouble;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->cost = tmp->valuedouble;
        else
            obj->cost = (double) tmp->valueint;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "node_stack_top");
    if (tmp)
	{
		tmp = cJSON_GetArrayItem(tmp, 5);
		if (tmp->valuedouble)
			obj->diam = tmp->valuedouble;
	}
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
					  max_thrust = j->valueint;

				  j = cJSON_GetObjectItemCaseSensitive(i, "gimbalRange");
				  if (j)
				  {
					  obj->gimbal = j->valuedouble;
				  }

				  j = cJSON_GetObjectItemCaseSensitive(i, "EngineType");
				  if (j)
				  {
					  if (strcmp(j->valuestring, "LiquidFuel") == 0)
						  obj->fuel = LIQUIDFUEL;
					  else if (strcmp(j->valuestring, "Eletric") == 0)
						  obj->fuel = ELETRIC;
					  else if (strcmp(j->valuestring, "MonoProp") == 0)
						  obj->fuel = MONOPROPELLANT;
					  else if (strcmp(j->valuestring, "SolidBooster") == 0)
						  obj->fuel = SOLIDFUEL;
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
        errx(1, "Incorrect decoupler file");
	}

	tmp = cJSON_GetObjectItemCaseSensitive(part, "name");
    if (tmp)
        asprintf(&obj->name, "%s", tmp->valuestring);

    tmp = cJSON_GetObjectItemCaseSensitive(part, "mass");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->mass = tmp->valuedouble;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "cost");
    if (tmp)
    {
        if (tmp->valuedouble)
            obj->cost = tmp->valuedouble;
        else
            obj->cost = (double) tmp->valueint;
    }


    cJSON_Delete(file);
    return obj;
}
