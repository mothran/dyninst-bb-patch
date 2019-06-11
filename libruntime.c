#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

static sem_t sem_lock;
static bool first = true;

typedef struct bbtrace {
    uint64_t pid;
    uint64_t addr;
} __attribute__((packed)) bbtrace_t;

static inline void *try_log(uint32_t len)
{
    bbtrace_t *ret = malloc(sizeof(bbtrace_t));

    if (first) {
        sem_init(&sem_lock, 1, 1);
        first = false;
    }

    sem_wait(&sem_lock);
    ret->pid = getpid();
    sem_post(&sem_lock);

    return (void *) ret;
}
void bb_callback(uint64_t comp_id)
{
    bbtrace_t *bbent;
    bbent = (bbtrace_t *) try_log(sizeof(*bbent));
    if (bbent) {
        bbent->addr = comp_id;
        printf("bb: %lx\n", bbent->addr);
        free(bbent);
    }
}