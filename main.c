#include <stdio.h>
#include "utils.h"

#if DEBUG
#include "test.h"
#endif

/* int main(int argc, char* argv[]) */
int main()
{
#if DEBUG
    // For test functions only
    test_brutforce();
    //coni();
    return 0;
#else
    return 0;
#endif
}
