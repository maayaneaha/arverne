#include <stdio.h>
#include "utils.h"

void coni()
{
    Decoupler* pute = load_Decoupler("Decoupler_0.json");    
    printf("%s\n", pute->name);
    printf("%f\n", pute->mass);
    printf("%f\n", pute->cost);
    printf("%d\n", pute->max_temp);
    printf("%d\n", pute->ejection);
    return;
}
