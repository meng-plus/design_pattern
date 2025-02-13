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
#include "observer_config.h"
#include <stdio.h>


enum OBSERVER_ERROR
{
    OBS_ERROR_OK           = 0,
    OBS_ERROR_NULL_POINTER = OBS_ERROR_BASE + 1,
    OBS_ERROR_DUPLICATE,
    OBS_ERROR_MALLOC_FAILED,
    OBS_ERROR_MUTEX_FAILED,
};
/**
 * @brief 数据基类
 * 使用时根据主题继承情况处理
 */
typedef struct OBSERVER_ARG
{
    uint16_t type; /*!< 基类用于标识数据类型 */
    uint16_t len;  /*!< 消息长度 */
} observer_data_t;

/** 类型转换 获取包含某成员的结构体指针
 * @example OBSERVER_GET_STRUCT_PTR(base,observer_data_t,base)
 */
#define OBSERVER_GET_STRUCT_PTR(member_ptr, struct_type, member_name) \
    ((struct_type *)((char *)(member_ptr) - offsetof(struct_type, member_name)))

// 观察者接口
typedef struct Observer
{
    void (*update)(const struct Observer *);
    observer_data_t *data; /*!< 通知的数据 */
    mm_list_t node;
} Observer;

// 主题接口
typedef struct Subject
{
    const char *name;      /*!< 观察者名称 */
    mm_list_t observers;   /*!< 订阅者清单 */
    observer_data_t *data; /*!< 通知的数据 */
} Subject;

void observer_init(Observer *obs_ptr, void (*update)(const struct Observer *));
void subject_init(Subject *sub_ptr);

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
void subject_notify_observers_data(Subject *subject, observer_data_t *data);

/**
 * @brief 拷贝数据到主题
 * 需要主题根据需要定义数据结构基类 @see observer_data_t
 * @param subject 主题句柄
 * @param data 要发送的数据基类
 */
void subject_set_data(Subject *subject, const observer_data_t *data);
/**
 * @brief 观察者通过主题获取数据
 *
 * @param subject 主题句柄
 * @param data  数据
 * @param len  数据长度
 */
uint16_t subject_get_data(Subject *subject, void *data, uint16_t len);

/**
 * @brief 两者比较函数 用于查找
 *
 * @param node1
 * @param node2
 * @return int 1相等
 */
int subject_cmp(mm_list_t *node1, mm_list_t *node2);
int observer_cmp(mm_list_t *node1, mm_list_t *node2);

#endif
