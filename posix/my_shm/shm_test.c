#include <stdio.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>

#ifndef TANK_IPC_NAME
#define TANK_IPC_NAME "tank_socket"
#endif

// compiler cmd: gcc shm_test.c -o demo.out -lrt

/* 获取IPC名字 */
const char *posix_get_IPC_name(void)
{
    const char *IPC_name = TANK_IPC_NAME;
    return IPC_name;
}


int main(char argc, char *argv[])
{
    if(argc == 2){
        /* 读共享内存 */
        if(!strncmp(argv[1], "read", 1024)){
            printf("share memory read demo\n");
            int fd = 0;
            const char *posix_name = posix_get_IPC_name();
            /* 创建新的/打开存在的 共享内存对象，并返回文件标识符
            ** O_RDWR 文件打开方式
            ** S_IRUSR 若文件被创建，指定文件权限
            */
            /* 创建的共享内存对象文件可在/dev/shm中查看,本质就是一个文件 */
            fd = shm_open(posix_name, O_RDWR, S_IRUSR|S_IWUSR);
            if(fd < 0){
                perror("read ERROR, file not creat");
                exit(1);
            }
            printf("file descriptor:%d\n",fd);
            /* 映射文件到进程内存 */
            char *buf = (char*)mmap(NULL, 1024, PROT_READ, MAP_SHARED, fd, 0);
            if (!buf) {
                printf("mmap failed\n");
                close(fd);
                exit(1);
            }
            printf("mmap address:%p\n", buf);
            printf("message:%s\n", buf);
            /* 取消映射,通过指针地址将不能访问 */
            munmap(buf, 1024);
            /* 关闭文件标识符 */
            close(fd);
            /* 关闭文件对内存对象的映射，并删除文件 */
            shm_unlink(posix_name);
            goto exit;
            /* 写共享内存 */
        }else if(!strncmp(argv[1], "write", 1024)){
            printf("share memory write demo\n");
            int fd = 0;
            const char *posix_name = posix_get_IPC_name();
            /* 创建新的/打开存在的 共享内存对象，并返回文件标识符
            ** O_RDWR 文件打开方式
            ** S_IRUSR 若文件被创建，指定文件权限
            */
            /* 创建的共享内存对象文件可在/dev/shm中查看,本质就是一个文件 */
            fd = shm_open(posix_name, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
            if(fd < 0){
                perror("creat ERROR");
                exit(1);
            }
            printf("file descriptor:%d\n",fd);
            /* 更改文件长度，要比共享内存写入区域大 */
            ftruncate(fd, 1024);
            char *buf = (char*)mmap(NULL, 1024, PROT_WRITE, MAP_SHARED, fd, 0);
            if (!buf) {
                printf("mmap failed\n");
                close(fd);
                exit(1);
            }
            printf("mmap address:%p\n", buf);
            snprintf(buf, 1024, "This is a share memory!");
            printf("message:%s\n", buf);

            munmap(buf, 1024);
            close(fd);
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
