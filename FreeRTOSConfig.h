
/* $Id$ */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define vPortSVCHandler         SVC_Handler
#define xPortPendSVHandler      PendSV_Handler
#define xPortSysTickHandler     SysTick_Handler


#define configMINIMAL_STACK_SIZE        ((unsigned short) 128)
#define configSYSTICK_CLOCK_HZ          (configCPU_CLOCK_HZ / 8)
#define configTICK_RATE_HZ              ((TickType_t) 1000)
#define configTOTAL_HEAP_SIZE           ((size_t) (16 * 1024))
#define configCPU_CLOCK_HZ              ((unsigned long) 72000000)

#define configIDLE_SHOULD_YIELD         1
#define configMAX_PRIORITIES            5
#define configMAX_TASK_NAME_LEN         16
#define configUSE_16_BIT_TICKS          0


#define configUSE_COUNTING_SEMAPHORES   1
#define configUSE_MUTEXES               1
#define configUSE_NEWLIB_REENTRANT      1
#define configUSE_PREEMPTION            1

#define configUSE_CO_ROUTINES           0
#define configMAX_CO_ROUTINE_PRIORITIES 2

#define configUSE_TICK_HOOK             0
#define configUSE_IDLE_HOOK             0
#define configUSE_TRACE_FACILITY        0

#define INCLUDE_uxTaskPriorityGet       1
#define INCLUDE_vTaskCleanUpResources   0
#define INCLUDE_vTaskDelay              1
#define INCLUDE_vTaskDelayUntil         1
#define INCLUDE_vTaskDelete             1
#define INCLUDE_vTaskPrioritySet        1
#define INCLUDE_vTaskSuspend            1

//#define INCLUDE_eTaskGetState                   0
//#define INCLUDE_uxTaskGetStackHighWaterMark     0
//#define INCLUDE_xQueueGetMutexHolder            0
//#define INCLUDE_xSemaphoreGetMutexHolder INCLUDE_xQueueGetMutexHolder
//#define INCLUDE_xTaskAbortDelay                 0
//#define INCLUDE_xTaskGetCurrentTaskHandle       0
//#define INCLUDE_xTaskGetHandle                  0
//#define INCLUDE_xTaskGetIdleTaskHandle          0
//#define INCLUDE_xTaskGetSchedulerState          0
//#define INCLUDE_xTaskResumeFromISR              1
//#define INCLUDE_xTimerPendFunctionCall          0

#define configKERNEL_INTERRUPT_PRIORITY         0xFF
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    0xB0
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15

//#define configAPPLICATION_ALLOCATED_HEAP              0
//#define configASSERT(x)
//#define configASSERT_DEFINED                          0
//#define configCHECK_FOR_STACK_OVERFLOW                0
//#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP         2
//#define configGENERATE_RUN_TIME_STATS                 0
//#define configNUM_THREAD_LOCAL_STORAGE_POINTERS       0
//#define configPOST_SLEEP_PROCESSING(x)
//#define configPRE_SLEEP_PROCESSING(x)
//#define configQUEUE_REGISTRY_SIZE                     0U
//#define configSUPPORT_DYNAMIC_ALLOCATION              1
//#define configSUPPORT_STATIC_ALLOCATION               0
//#define configUSE_APPLICATION_TASK_TAG                0
//#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES     0
//#define configUSE_MALLOC_FAILED_HOOK                  0
//#define configUSE_PORT_OPTIMISED_TASK_SELECTION       0
//#define configUSE_QUEUE_SETS                          0
//#define configUSE_RECURSIVE_MUTEXES                   0
//#define configUSE_STATS_FORMATTING_FUNCTIONS          0
//#define configUSE_TASK_NOTIFICATIONS                  1
//#define configUSE_TICKLESS_IDLE                       0
//#define configUSE_TIMERS                              0
//#define configUSE_TIME_SLICING                        1

#endif
/* EOF */
