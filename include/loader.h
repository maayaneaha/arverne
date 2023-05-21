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
cJSON* export_to_json_Part(Part* obj, int type);
cJSON* export_to_json_Rocket(Rocket* obj);
cJSON* export_to_json_Stage(Stage* obj);
cJSON* export_to_json_Engine(Engine* obj);
cJSON* export_to_json_Tank(Tank* obj);
cJSON* export_to_json_Decoupler(Decoupler* obj);
int generate_file(Datas *d, char* path);
Tank*** sort_Tanks(Tank** tanks);
void insert_sort_Tanks(Tank** tanks, Tank* elt);