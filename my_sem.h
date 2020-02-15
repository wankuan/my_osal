#ifndef __MY_SEM_H__
#define __MY_SEM_H___

#include <sys/ipc.h>
#include <sys/sem.h>
#include "lib_pub.h"
#include <errno.h>

union semun
{
    int val;				//<= value for SETVAL
    struct semid_ds *buf;		//<= buffer for IPC_STAT & IPC_SET
    unsigned short int *array;		//<= array for GETALL & SETALL
    struct seminfo *__buf;		//<= buffer for IPC_INFO
};


int my_sem_init(int sem_num);

int sem_v(int sem_id);
int sem_p(int sem_id);
int get_nsem(int sem_id, int sem_num);
int rm_sem(int sem_id, int sem_num);    
#endif


