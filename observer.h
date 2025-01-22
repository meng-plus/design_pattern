/**
 * @file observer.h
 * COPYRIGHT (C) 2022,  chengmeng_2@outlook.com Co., Ltd.
 * All rights reserved.
 * @brief 观察者模式
 * @details    C语言版不开销heap的设计模式
 *
 * @version ver1.0
 * @date 2023年4月6日
 * @author cmm
 * @note chatgpt生成
 */

#ifndef __OBSERVER_H_
#define __OBSERVER_H_


#include "mm_list.h"
#include <stdio.h>

/**
 * @brief 数据基类
 * 使用时根据主题继承情况处理
 */
typedef struct OBSERVER_DATA_BASE
{
    uint32_t type; /*!< 基类用于标识数据类型 */
} observer_data_t;

// 观察者接口
typedef struct Observer
{
    void (*update)(struct Observer *);
    observer_data_t *data; /*!< 通知的数据 */
    mm_list_t node;
} Observer;

// 主题接口
typedef struct Subject
{
    mm_list_t observers;   /*!< 订阅者清单 */
    observer_data_t *data; /*!< 通知的数据 */
} Subject;

// 注册观察者
void subject_register_observer(Subject *subject, Observer *observer);

// 移除观察者
void subject_remove_observer(Subject *subject, Observer *observer);

/**
 * @brief 通知观察者 数据采用subject内置数据
 *
 * @param subject
 */
void subject_notify_observers(Subject *subject);

/**
 * @brief 设置主题的状态并通知观察者
 *
 * @param subject 主题句柄
 * @param data 要发送的数据基类
 */
void subject_set_data(Subject *subject, observer_data_t *data);

#endif
