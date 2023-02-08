#define _GNU_SOURCE

#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include "utils.h"
#include "cjson/cJSON.h"

cJSON* json_ParseFile(char* filename)
{
    cJSON* parsed = NULL;
    int c;
    int i = 0;
    char* content = NULL;
    FILE* fp = fopen(filename, "r");

    while ((c = getc(fp)) != EOF)
        i++;

    fseek(fp, 0, 0);
    content = malloc((i + 1) * sizeof(char));

    i = 0;
    while ((c = getc(fp)) != EOF)
        content[i++] = c;
    content[i] = '\0';
    fclose(fp);
    parsed = cJSON_Parse(content);

    free(content);

    return parsed;
}

Decoupler* load_Decoupler(char* filename)
{
    Decoupler* obj = malloc(sizeof(Decoupler));
    cJSON* file = json_ParseFile(filename);
    cJSON* part = cJSON_GetObjectItemCaseSensitive(file, "PART");
    cJSON* tmp = NULL;
    
    if (!part)
        errx(1, "Incorrect decoupler file");

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

    tmp = cJSON_GetObjectItemCaseSensitive(part, "maxTemp");
    if (tmp)
    {
        if (tmp->valueint)
            obj->max_temp = tmp->valueint;
    }
    
    tmp = cJSON_GetObjectItemCaseSensitive(part, "crashTolerance");
    if (tmp)
    {
        if (tmp->valueint)
            obj->tolerance_ms = tmp->valueint;
    }

    tmp = cJSON_GetObjectItemCaseSensitive(part, "MODULE");
    if (tmp)
    {
        if (cJSON_IsArray(tmp))
        {
            cJSON* i = NULL;
            cJSON* j = NULL;
            cJSON_ArrayForEach(i, tmp)
            {
                /* j = cJSON_GetObjectItemCaseSensitive(i, "name"); */
                /* if (cJSON_IsString(j)) */
                /* { */
                j = cJSON_GetObjectItemCaseSensitive(i, "ejectionForce");
                if (j)
                {
                    if (j->valueint)
                        obj->ejection = j->valueint;
                    break;
                }
                /* } */
            }
        }
    }

    cJSON_Delete(file);
    return obj;
}
