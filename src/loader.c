#include <string.h>
#include <string.h>
#include <stdio.h>
#include <err.h>
#include <stdlib.h>

#include "cjson/cJSON.h"
#include "utils.h"


load_Tank(char* filename)
{
   Tank* obj = malloc(sizeof(Tank));
   cJSON* file = json_ParseFile(filename);
   cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
   cJSON* tmp = NULL;
   
   if (!part)
      errx(1,"Incorrect tank file");
   tmp = cJSON_GetObjectItemCaseSensitive(part,"name");
   if (tmp)
      asprintf(&obj->name, "%s",tmp->valuestring)
   else
      errx(1,"name is not found");
   tmp = cJSON_GetObjectItemCaseSenstive(part,"mass");
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
         obj->cost = tmp->valuedouble;
      else
         obj->cost = (double) tmp->valueint;
   }
   else
      errx(1,"cost is not found");
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_top");
   if(cJSON_IsArray(tmp))
   {
      tmp = cJSON_GetArrayItem(tmp,6);
      if tmp->valueint == 0;
	 obj->top_diam = TINY;
      if tmp->valueint == 1;
	 obj->top_diam = SMALL;
      if tmp->valueint == 2;
	 obj->top_diam = LARGE;
      if tmp->valueint == 3;
	 obj->top_diam = EXTRALARGE;
   }
   else
      errx(1,"node_stack_top is not found");   
   tmp = cJSON_GetObjectItemCaseSensitive(part,"node_stack_bottom");
   if(cJSON_IsArray(tmp))
   {
      tmp = cJSON_GetArrayItem(tmp,6);
      if tmp->valueint == 0;
	 obj->top_diam = TINY;
      if tmp->valueint == 1;
	 obj->top_diam = SMALL;
      if tmp->valueint == 2;
	 obj->top_diam = LARGE;
      if tmp->valueint == 3;
	 obj->top_diam = EXTRALARGE;
   }
   else
      errx(1,"node_stack_bottom is not found");
   obj->fuel = FUELOX;
   tmp = cJSON_GetObjectItemCaseSensitive(part, "RESSOURCE");
   if(cJSON_IsArray(res))
   {
      tmp = cJSON_GetArrayItem(tmp,0);
      obj->quantity_fuel1 =(cJSON_GetObjectItemCaseSensitive(tmp, "maxAmount"))->valueint;
      cJSON* tmp = cJSON_GetObjectItemCaseSensitive(part, "RESSOURCE");
      tmp = cJson_GetArrayItem(tmp,1);
      obj->quantity_fuel2 =(cJSON_GetObjectItemCaseSensitive(tmp, "maxAmount"))->valueint;
   }
   else
      errx(1,"RESOURCE is not found");
   tmp = cJSON_GetObjectItemCaseSensitive(part,"attachRules")
   if(cJSON_IsArray(tmp))
   {
      obj->radial_fitting = (cJSON_Get_ArrayItem(tmp,3))->valueint; 
      obj->radial_part = (cJSON_Get_ArrayItem(tmp,0))->valueint;
   }
