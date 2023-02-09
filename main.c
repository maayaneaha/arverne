#include <stdio.h>
#include "utils.h"
#include "test.h"

/* int main(int argc, char* argv[]) */
int main()
{
#if DEBUG
    // For test functions only
    test_basic_display();
    return 0;
#endif

    return 0;
}
