
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "timer.h"

void say_hello(void *arg);
void print_value(void *arg);
void print_rectangle(void *arg);
void sigintr_handler(int signo);

int main(void)
{
    /* 初始化闹钟 */
    if (timer_init() != 0) {
       fprintf(stderr, "init timer failed.\n");
       exit(1);
    }

    /* 安装中断信号处理函数 */
    if (signal(SIGINT, sigintr_handler) == SIG_ERR)
    {
        fprintf(stderr, "set SIGINT handler failed.\n");
        exit(2);
    }
    
    /* 添加3个任务 */
    TimerId_t ids[3];

    ids[0] = timer_add(2,say_hello,NULL,LOOP);
    if(ids[0] == ADD_FAILED)
    {
        fprintf(stderr, "add timer failed.\n");
        timer_destroy();
        return 2;
    }

    int value = 555;
    ids[1] = timer_add(5, print_value, &value, ONCE);

    ids[2] = timer_add(3, print_rectangle, NULL, LOOP);

    /* 删除第0个任务 */
    getchar();
    printf("\033[31mDel A Timer.\n\033[0m");
    timer_del(ids[0]);

    /* 销毁所有任务 */
    printf("Enter Destory Timer.\n");
    getchar();
    timer_destroy();

    return 0;
}

void sigintr_handler(int signo)
{
    if (signo == SIGINT) {
        printf("\n\033[0m");
        exit(22);
    }
}
void say_hello(void *arg)
{
     printf("\033[36mHello,Hello...\033[0m\n");
}

void print_value(void *arg)
{
     printf("\033[31m=====value:%d======\n\033[0m", *(int *)arg);
}

void print_rectangle(void *arg)
{
     int i, j;

     for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            printf("\033[32m* \033[0m");
        }
        putchar('\n');
     }
}
