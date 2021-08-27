#include "cpu.h"
#include "version.h"
#include <stdio.h>
int main (int argc, char* argv[]) {
    if(argc > 1) {
        run(argv[1]);
    } else {
        printf("%s Version: %d.%d\n", argv[0], vmBASIC_VERSION_MAJOR, vmBASIC_VERSION_MINOR);
        printf("Please specify ROM file\n");
    }
    return 0;
}
