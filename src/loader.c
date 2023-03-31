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

