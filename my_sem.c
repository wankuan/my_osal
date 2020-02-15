#include "my_sem.h"




int my_sem_init(int sem_num)
{
    const char *filename = "main.c";
    int project_id = 0x03;
    key_t key_id = ftok(filename, project_id);
    printf("key id is %d\n",key_id);
    int sem_id = semget(key_id, 1,IPC_CREAT|0666);
    if (sem_id == -1){
        printf("[ERROR]code:%d\n",errno);
        goto fail;
    }else{
        printf("sem_id:%d\n",sem_id);
        union semun cmd;
        cmd.val = sem_num;
        int status = semctl(sem_id,0,SETVAL,cmd);
        if(status < 0){
            goto fail;
        }
    }
    return sem_id;
fail:
    return -1;
}

int sem_p(int sem_id)
{
  struct sembuf sem_buf;
  sem_buf.sem_num=0;
  sem_buf.sem_op=-1;
  sem_buf.sem_flg=SEM_UNDO;
  //p_buf = {0,-1,SEM_UNDO};
  if (semop(sem_id, &sem_buf, 1)==-1)
  {
    printf("p-sem fail\n");
    goto fail;
  }
    return 0;
fail:
    return -1;
}

int sem_v(int sem_id)
{
  struct sembuf sem_buf;
  sem_buf.sem_num = 0;
  sem_buf.sem_op  = 1;
  sem_buf.sem_flg = SEM_UNDO;
  if (semop(sem_id, &sem_buf, 1)==-1)
  {
    printf("v-sem fail\n");
    goto fail;
  }
    return 0;
fail:
    return -1;
}


int get_nsem(int sem_id, int sem_num)
{
    int nsem = 0;
    nsem = semctl(sem_id, sem_num, GETVAL);
    //printf("sem_id:%d sem_num:%d value is %d",sem_id, sem_num, nsem);
    return nsem;
}

int rm_sem(int sem_id, int sem_num)
{
    int status = 0;
    status = semctl(sem_id, sem_num, IPC_RMID);
    //printf("sem_id:%d sem_num:%d has been removed\n",sem_id, sem_num);
    return status;
}

