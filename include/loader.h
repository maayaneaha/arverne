#include "utils.h"

Engine* load_Engine(char* filename);
Decoupler* load_Decoupler(char* filename);
Tank* load_Tank(char* filename);
cJSON* json_findElement(cJSON* file, char** key, int src);
