#include "src/rsh_init.h"
#include "src/rsh_loop.h"

int main(int argc, char *argv[]) {
    if (rsh_init(argc, argv) != EXIT_SUCCESS)
        return -1;
    rsh_loop();

    return 0;
}