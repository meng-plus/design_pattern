#include "observer.h"
#include <stdio.h>
#include <string.h>

int subject_cmp(mm_list_t *node1, mm_list_t *node2)
{
    Subject *sub1 = MM_LIST_ENTRY(node1, Subject, node);
    Subject *sub2 = MM_LIST_ENTRY(node2, Subject, node);
    return strcmp(sub1->name, sub2->name); // 根据名字来判断是否相同
}

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
        obs_ptr->data   = NULL;
        obs_ptr->update = update;
        mm_list_init(&obs_ptr->node);
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
    if (observer->update)
    { /** 避免重复注册 */
        mm_list_insert_unique(&subject->observers, &observer->node, observer_cmp);
    }
}

// 移除观察者
void subject_remove_observer(Subject *subject, Observer *observer)
{
    mm_list_remove(&observer->node);
}

void subject_set_data(Subject *subject, observer_data_t *data)
{ /* 遍历链表 */

    OBS_ENTER_CRITICAL();
    if (subject->data)
    {
        memcpy(subject->data, data, data->len);
    }
    OBS_EXIT_CRITICAL();
}

uint16_t subject_get_data(Subject *subject, void *data, uint16_t len)
{
    OBS_ENTER_CRITICAL();
    if (subject->data)
    {
        memcpy(data, subject->data, len);
        return len;
    }
    OBS_EXIT_CRITICAL();
    return 0;
}

// 通知观察者
void subject_notify_observers(Subject *subject)
{
    subject_notify_observers_data(subject, subject->data);
}

void subject_notify_observers_data(Subject *subject, observer_data_t *data)
{ /* 遍历链表 */
    mm_list_t *pos, *n;
    mm_list_for_each_safe(pos, n, &subject->observers)
    {
        Observer *curr_ptr = MM_LIST_ENTRY(pos, Observer, node);
        curr_ptr->data     = data;
        curr_ptr->update(curr_ptr);
    }
}
