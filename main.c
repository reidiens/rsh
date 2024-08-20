#include "src/tsh_core.h"

int main(int argc, char *argv[]) {
    tsh_init(argc, argv);
    tsh_loop();

    return 0;
}