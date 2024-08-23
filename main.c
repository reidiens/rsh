#include "src/rsh_core.h"

int main(int argc, char *argv[]) {
    rsh_init(argc, argv);
    rsh_loop();

    return 0;
}