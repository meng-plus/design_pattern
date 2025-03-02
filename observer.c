#include "observer.h"
#include <stdio.h>
#include <string.h>


int observer_cmp(mm_list_t *node1, mm_list_t *node2)
{
    Observer *obs1 = MM_LIST_ENTRY(node1, Observer, node);
    Observer *obs2 = MM_LIST_ENTRY(node2, Observer, node);
    return obs1 == obs2 ? 0 : (obs1 < obs2 ? 1 : -1); // 根据地址来判断是否相同
}
void observer_init(Observer *obs_ptr, void (*update)(const Observer *))
{
    if (obs_ptr)
    {
        memset(obs_ptr, 0, sizeof(Observer));
        mm_list_init(&obs_ptr->node);
        obs_ptr->update = update;
    }
}

void subject_init(Subject *sub_ptr)
{
    if (sub_ptr)
    {
        memset(sub_ptr, 0, sizeof(Subject));
        mm_list_init(&sub_ptr->observers);
    }
}

// 注册观察者
void subject_register_observer(Subject *subject, Observer *observer)
{
    if (subject == NULL || observer == NULL || observer->update == NULL)
    {
        return; // 参数检查
    }
    /** 避免重复注册 */
    mm_list_insert_unique(&subject->observers, &observer->node, observer_cmp);
}

// 移除观察者
void subject_remove_observer(Subject *subject, Observer *observer)
{
    if (subject == NULL || observer == NULL)
    {
        return; // 参数检查
    }
    mm_list_remove(&observer->node);
}
// 设置主题数据
void subject_set_data(Subject *subject, const observer_data_t *data)
{
    if (subject == NULL || data == NULL)
    {
        return; // 参数检查
    }

    OBS_ENTER_CRITICAL();

    if (subject->data == NULL)
    {
#ifdef OBS_MALLOC
        // 如果主题数据为空，直接分配内存
        subject->data = OBS_MALLOC(sizeof(observer_data_t) + data->len);
        if (subject->data == NULL)
        {
            OBS_EXIT_CRITICAL();
            return; // 内存分配失败
        }
        memcpy(subject->data, data, sizeof(observer_data_t) + data->len);
#else
        subject->data = data;
#endif
    }
    else
    {
        memcpy(subject->data, data, sizeof(observer_data_t) + data->len);
    }

    OBS_EXIT_CRITICAL();
}

// 获取主题数据
uint16_t subject_get_data(Subject *subject, void *data, uint16_t len)
{
    if (subject == NULL || data == NULL || subject->data == NULL)
    {
        return 0; // 参数检查
    }
    OBS_ENTER_CRITICAL();
    memcpy(data, subject->data, len);
    OBS_EXIT_CRITICAL();
    return 0;
}

// 通知观察者
void subject_notify_observers(Subject *subject)
{
    subject_notify_observers_data(subject, subject->data);
}

void subject_notify_observers_data(Subject *subject, observer_data_t *data)
{
    if (subject == NULL || data == NULL)
    {
        return; // 参数检查
    }
    /* 遍历链表 */
    mm_list_t *pos, *n;
    mm_list_for_each_safe(pos, n, &subject->observers)
    {
        Observer *curr_ptr = MM_LIST_ENTRY(pos, Observer, node);
        curr_ptr->data     = data;
        curr_ptr->update(curr_ptr);
    }
}
