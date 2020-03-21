#include "my_msgq.h"

static int msgq_get_id(void)
{
    int key_id = osal_get_keyid();
    int msgq_id = msgget(key_id+2, IPC_CREAT|0666);
    printf("msgq_id is %d\n",msgq_id);
    return msgq_id;
}
msgq_status_t msgq_constuctor(my_msgq_t *my_msgq)
{
    my_msgq->id = msgq_get_id();
    if (!IS_MSGQ_ID_VALID(my_msgq->id )){
        printf("[ERROR]code:%d\n",errno);
        goto fail;
    }
    return SUCCESS;
fail:
    return FAIL;
}
msgq_status_t msgq_destuctor(my_msgq_t *my_msgq)
{
    my_msgq->id = 0;
    return SUCCESS;
}

msgq_status_t msgq_send(my_msgq_t *my_msgq, void *addr, int size, long type)
{
    msgq_message_t buf;
    buf.type = type;
    memcpy(buf.message, addr, size);
    if(msgsnd(my_msgq->id, &buf, size, IPC_NOWAIT)){
        printf("[sender]fail\n");
        goto fail;
    }else{
        printf("[sender]success\n");
    }
    return SUCCESS;
fail:
    return FAIL;
}
msgq_status_t msgq_rcv(my_msgq_t *my_msgq, void *addr, int size, long type)
{
    msgq_message_t buf;
    int status = msgrcv(my_msgq->id, &buf, size, type, MSG_NOERROR|IPC_NOWAIT);
    if(status==-1){
        if (errno == ENOMSG) {
            printf("[receiver]No message!\n");
        }else{
            printf("[ERROR]code:errno\n");
        }
        goto fail;
    }else{
        memcpy(addr, buf.message, size);
        printf("[receiver]bytes:%d\n",status);
    }
    return SUCCESS;
fail:
    return FAIL;
}

msgq_status_t msgq_delete(my_msgq_t *my_msgq)
{
    struct msqid_ds buf;
    if(msgctl(my_msgq->id,IPC_RMID,&buf)==0){
        printf("delete msgq success id:%d\n",my_msgq->id);
    }else{
        printf("delete error\n");
        goto fail;
    }
    return SUCCESS;
fail:
    return FAIL;
}

