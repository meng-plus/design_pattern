#include <stdio.h>
#include "observer.h"

// 注册观察者
void subject_register_observer(Subject *subject, Observer *observer)
{
    if (observer->update)
    {
        mm_list_insert_after(&subject->observers, &observer->node);
    }
}

// 移除观察者
void subject_remove_observer(Subject *subject, Observer *observer)
{
    mm_list_remove(&observer->node);
}

void subject_set_data(Subject *subject, observer_data_t *data)
{ /* 遍历链表 */
    mm_list_t *pos;
    subject->data = data;
    mm_list_for_each(pos, &subject->observers)
    {
        Observer *curr_ptr = MM_LIST_ENTRY(pos, Observer, node);
        curr_ptr->data     = data;
        if (curr_ptr->update)
        {
            curr_ptr->update(curr_ptr);
        }
    }
}

// 通知观察者
void subject_notify_observers(Subject *subject)
{
    /* 遍历链表 */
    mm_list_t *pos;
    mm_list_for_each(pos, &subject->observers)
    {
        Observer *curr_ptr = MM_LIST_ENTRY(pos, Observer, node);
        curr_ptr->data     = subject->data;
        curr_ptr->update(curr_ptr);
    }
}
