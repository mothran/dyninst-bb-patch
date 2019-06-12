#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/syscall.h>

static sem_t sem_lock;
static bool first = true;
pthread_key_t tid_key;

#define LOG_TYPE_1				(0)
#define LOG_TYPE_2				(1)
#define LOG_PIDTID(ty, pi, ti)	((((uint64_t) (ty)) << 62) | (((uint64_t) (pi)) << 32) | (ti))


typedef struct bbtrace {
    uint64_t pid;
    uint64_t addr;
} __attribute__((packed)) bbtrace_t;

static inline uint64_t get_pidtid(uint32_t type)
{
    pid_t tid;
    pid_t pid;

    tid = (pid_t) (intptr_t) pthread_getspecific(tid_key);
    if(!tid)
    {
        tid = syscall(SYS_gettid);
        pthread_setspecific(tid_key, (void *) (intptr_t) tid);
        pid = getpid();
    }

    return LOG_PIDTID(type, pid, tid);
}


static inline void *try_log(uint32_t type, uint32_t len)
{
    bbtrace_t *ret = malloc(sizeof(bbtrace_t));

    if (first) {
        sem_init(&sem_lock, 1, 1);
        first = false;
    }

    sem_wait(&sem_lock);
    sem_post(&sem_lock);
    ret->pid = get_pidtid(type);

    return (void *) ret;
}
void bb_callback(uint64_t comp_id)
{
    bbtrace_t *bbent;
    bbent = (bbtrace_t *) try_log(LOG_TYPE_1, sizeof(*bbent));
    if (bbent) {
        bbent->addr = comp_id;
        // printf("bb: %lx\n", bbent->addr);
        free(bbent);
    }
    return;
}