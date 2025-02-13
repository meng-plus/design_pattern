/**
 * @file factory_observer.h
 * @author 蒙蒙plus (chengmeng_2@outlook.com)
 * @brief 通知主题的生产工厂
 * @version 0.1
 * @date 2025-01-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef __FACTORY_OBSERVER_H_
#define __FACTORY_OBSERVER_H_

#include "observer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获得主题实例，如果不存在则返回NULL
 *
 * @param name 主题名称
 * @return Subject* 主题实例
 */
Subject *subject_get_instance(const char *name);

/**
 * @brief 创建主题，不可重名
 *
 * @param subject
 * @param name
 * @return int 创建成功返回0  创建失败返回 负值
 */
int subject_create(Subject **subject, const char *name);

int subject_destroy(Subject *subject);
#ifdef __cplusplus
}
#endif


#endif //__FACTORY_OBSERVER_H_
