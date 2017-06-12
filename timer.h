#ifndef  _TIMER_H_
#define  _TIMER_H_

#ifdef __cplusplus
extern "C" { /* C++ will be happy */
#endif

/*
 * Using only a single timer (either alarm or the higher-precision setitimer),
 * provide a set of fuctions that allows a process to set any number of timers.
 *
 * 用一个alarm闹钟实现管理多个定时任务的闹钟
 */


/*
 * 类型定义：Timer标识类型, Timer类型
 */
typedef    long         TimerId_t;
#define    ADD_FAILED   ((TimerId_t)(-1))
enum TimerType {
      LOOP, /* 循环型闹钟 */
      ONCE, /* 一次性闹钟 */
      /* 
       ... 其他类型
      */
};


/**
 * 初始化Timer，成功返回0，失败返回-1
 */
int timer_init(void);

/**
 * 添加Timer，成功返回新添加的Timer标识，失败返回ADD_FAILED
 * @sec:  Timer触发的时间间隔
 * @task: 绑定Timer的任务
 * @arg:  任务传参
 * @type: Timer类型
 */
typedef   void Task_t(void *);
TimerId_t timer_add(int sec, Task_t *task, void *arg, enum TimerType type);

/**
 * 删除指定的闹钟
 * @id：闹钟标识
 */
void timer_del(TimerId_t id);

/**
 * 销毁所有闹钟
 */
void timer_destroy(void);

#ifdef __cplusplus
}
#endif

#endif //_TIMER_H_

