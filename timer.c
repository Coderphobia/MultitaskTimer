
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "timer.h"

typedef struct node {
     TimerId_t id; /* 唯一标识 */
     int timeout;  /* 超时值 */ 
     int count;    /* 计时器 */    
     void (*action)(void *arg); /* 超时行为 */
     void *arg;
     enum TimerType type; /* 闹钟类型 */
     struct node  *prev, *next;
}Timer_t;

Timer_t   Head; 

/**
 * SIGALARM信号处理函数，检查闹钟任务是否有超时
 * @signo: 信号标识
 */
static void sighandler(int signo);

/**
 * 初始化Timer，成功返回0，失败返回-1
 */
int timer_init(void)
{
     Head.next = &Head;
     Head.prev = &Head;

     if (signal(SIGALRM, sighandler) == SIG_ERR)
         return -1;

     alarm(1);

     return 0;
}


/**
 * 添加Timer，成功返回新添加的Timer标识，失败返回-1
 * @sec:  Timer触发的时间间隔
 * @task: 绑定Timer的任务
 * @arg:  任务传参
 * @type: Timer类型
 */
TimerId_t timer_add(int sec, Task_t *task, void *arg, enum TimerType type)
{
     Timer_t *one;

     one = (Timer_t*)malloc(sizeof(Timer_t));
     if(one == NULL)
         return ADD_FAILED;

     one->timeout = sec;
     one->count = 0;
     one->arg = arg;
     one->action = task;
     one->type = type;
     
     one->id = (TimerId_t)one; /* 用节点地址作唯一标识 */
    
     one->next = &Head;
     one->prev = Head.prev;
     one->next->prev = one;
     one->prev->next = one;

     return one->id;
}


/**
 * 删除指定的闹钟
 * @id：闹钟标识
 */
void timer_del(TimerId_t id)
{
     Timer_t *i = Head.next;
     while (i != &Head) {
        if (i->id == id) {
           i->prev->next = i->next;
           i->next->prev = i->prev;
           free(i);
           break;
        }
        i = i->next;
     }
}


/**
 * 销毁所有闹钟
 */
void timer_destroy(void)
{
     Timer_t *i, *n;

     alarm(0); 
     signal(SIGALRM, SIG_DFL);
     
     for (i = Head.next; i != &Head; i = n) {
        n = i->next;
        free(i);
     }
}


/**
 * 检查闹钟任务是否有超时
 */
static void sighandler(int signo)
{
     Timer_t  *i, *n;

     if (signo != SIGALRM)
          return;

     for( i = Head.next; i != &Head; i = n) {
         n = i->next;
         i->count++;
         if (i->count >= i->timeout) {
             /* 闹钟时间到，执行设定好的任务 */
             i->action(i->arg);
             if (i->type == ONCE) {
                i->prev->next = i->next;
                i->next->prev = i->prev;
                free(i);
             } else {
                i->count = 0;
             }
         }
     }
     
     alarm(1);
}

