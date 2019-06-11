#include <stdio.h>
#include <stdint.h>

void bb_callback(uint64_t comp_id)
{
    printf("bb: %lx\n", comp_id);
}