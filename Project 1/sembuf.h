
#ifndef SEMBUF_H
#define SEMBUF_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union senum {
    int val;
    struct semid_ds *buff;
    unsigned short *array;
};

#endif /* SEMBUF_H */

