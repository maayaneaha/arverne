#include "utils.h"

Tank* load_Tank(char* filename);
Engine* load_Engine(char* filename);
Decoupler* load_Decoupler(char* filename);
Tank* load_Tank(char* filename);
cJSON* json_findElement(cJSON* file, char** key, int src);
Tank*** load_Tanks(char* path);
Engine*** load_Engines(char* path);
Decoupler*** load_Decouplers(char* path);
int load_parts(Datas *d);