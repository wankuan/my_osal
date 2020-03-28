#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

#include <mqueue.h>


#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>


#include <time.h>

#ifndef TANK_IPC_NAME
#define TANK_IPC_NAME "/tank_socket"
#endif

// compiler cmd: gcc shm_test.c -o demo.out -lrt

/* 获取IPC名字 */
const char *posix_get_IPC_name(void)
{
    const char *IPC_name = TANK_IPC_NAME;
    return IPC_name;
}
/* 如果队列对象满了，队列打开或者发送都会被阻塞,知道消息队列消息被获取 */
int main(char argc, char *argv[])
{
    if(argc == 2){
        /* 读共享内存 */
        if(!strncmp(argv[1], "read", 1024)){
            printf("share memory read demo\n");
            mqd_t mqID;
            const char *mq_name = posix_get_IPC_name();
            mqID = mq_open(mq_name, O_RDWR, S_IRUSR|S_IWUSR, NULL);
            if (mqID < 0)
            {
                perror("open message queue eroor\n");
                exit(1);
            }
            struct mq_attr mqAttr;
            mq_getattr(mqID, &mqAttr);
            printf("mq_flags: %ld\n",mqAttr.mq_flags);
            printf("mq_maxmsg: %ld\n",mqAttr.mq_maxmsg);
            printf("mq_msgsize: %ld\n",mqAttr.mq_msgsize);
            printf("mq_curmsgs: %ld\n",mqAttr.mq_curmsgs);
            char buf[10][1000];

            for (int i=0; i<mqAttr.mq_curmsgs; ++i)
            {
                if (mq_receive(mqID, buf[i], mqAttr.mq_msgsize, NULL) < 0)
                {
                    perror("error info:");
                }
                printf("message:%s\n", buf[i]);
            }
            goto exit;
            /* 写共享内存 */
        }else if(!strncmp(argv[1], "write", 1024)){
            printf("share memory write demo\n");

            mqd_t mqID;
            const char *mq_name = posix_get_IPC_name();
            mqID = mq_open(mq_name, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR, NULL);
            if (mqID < 0)
            {
                perror("open message queue eroor\n");
                exit(1);
            }
            struct mq_attr mqAttr;
            if (mq_getattr(mqID, &mqAttr) < 0)
            {
                perror("get the attr error\n");
                exit(1);
            }
            const char buffer[3][100]={"1st message","2nd message","3rd message"};
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += 1;
            for(int i=0; i<3; i++){
                // if (mq_send(mqID, buffer[i], 100, i) < 0)
                // {
                //     perror("error info:");
                // }
                mq_getattr(mqID, &mqAttr);
                printf("mq_curmsgs: %ld\n",mqAttr.mq_curmsgs);
                if (mq_timedsend(mqID, buffer[i], 100, i, &timeout) < 0)
                {
                    perror("error info:");
                }
                printf("message:%s\n", buffer[i]);
                sleep(1);
            }
            printf("mq_flags: %ld\n",mqAttr.mq_flags);
            printf("mq_maxmsg: %ld\n",mqAttr.mq_maxmsg);
            printf("mq_msgsize: %ld\n",mqAttr.mq_msgsize);

            goto exit;
        }else{
            goto error;
        }
    }
error:
    printf("input error\n ./a.out read/write\n");
exit:
    return 0;
}
