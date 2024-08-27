#include "src/rsh_init.h"
#include "src/rsh_loop.h"

int main(int argc, char *argv[]) {
    rsh_init(argc, argv);
    rsh_loop();

    return 0;
}