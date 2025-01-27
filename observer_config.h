/**
 * @file observer_config.h
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 观察者模式的配置文件
 * @version 0.1
 * @date 2025-01-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __OBSERVER_CONFIG_H_
#define __OBSERVER_CONFIG_H_

#ifdef __RT_THREAD_H__

#define OBS_ASSERT         RT_ASSERT
#define OBS_ENTER_CRITICAL rt_enter_critical
#define OBS_EXIT_CRITICAL  rt_exit_critical
#define OBS_MALLOC         rt_malloc
#define OBS_FREE           rt_free

#elif defined __FREERTOS_H__
#define OBS_ASSERT         configASSERT
#define OBS_ENTER_CRITICAL taskENTER_CRITICAL
#define OBS_EXIT_CRITICAL  taskEXIT_CRITICAL
#define OBS_MALLOC         pvPortMalloc
#define OBS_FREE           vPortFree
#else
#include <stdlib.h>
#endif

#ifndef OBS_ERROR_BASE
#define OBS_ERROR_BASE 0
#endif


#ifndef OBS_MALLOC
#define OBS_MALLOC malloc
#endif
#ifndef OBS_FREE
#define OBS_FREE free
#endif

#ifndef OBS_ASSERT
#define OBS_ASSERT
#endif

#ifndef OBS_ENTER_CRITICAL
#define OBS_ENTER_CRITICAL()
#endif

#ifndef OBS_EXIT_CRITICAL
#define OBS_EXIT_CRITICAL()
#endif

#ifndef OBS_MUTEX
#define OBS_MUTEX
#endif

#ifndef OBS_MUTEX_INIT
#define OBS_MUTEX_INIT
#endif


#endif // __OBSERVER_CONFIG_H_
