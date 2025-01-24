#include "observer.h"

#include <stdio.h>

/**
 * @brief pm主题的数据类型
 *
 */
typedef struct OBSERVER_PM_DATA
{
    observer_data_t base;

    uint32_t lv; /*!< 各供电状态 */
} observer_pm_data_t;
observer_pm_data_t observer_pm_data;
// 观察者更新
static void observer_update(const Observer *observer)
{
    printf("observer update\n");
}
// 观察者更新
static void observer_update2(const Observer *observer)
{
    printf("observer update2\n");
}

void observer_pm_update(const Observer *obs)
{
    observer_pm_data_t *pm_data = (observer_pm_data_t *)obs->data;
    printf("observer update\n");
    printf("\tobserver_pm :0x %x \r\n", pm_data->lv);
    printf(("\t*pm_data= 0x%x\n"), pm_data);
}

int main()
{
    static Subject Subject_pm;
    mm_list_init(&Subject_pm.observers);
    Subject_pm.data = &observer_pm_data.base;

    Observer Observer_pm;

    Observer Observer_pm2;

    // 注册观察者
    printf("注册观察者\n");
    Observer_pm.update = observer_pm_update;
    subject_register_observer(&Subject_pm, &Observer_pm);
    observer_pm_data.lv = 0x12345678;
    printf("observer_pm_data.lv = 0x%x\n", observer_pm_data.lv);
    printf("&observer_pm_data= 0x%x\n", &observer_pm_data);
    // 通知观察者
    printf("通知观察者\n");
    subject_notify_observers(&Subject_pm);

    // 注册观察者2
    printf("注册观察者2\n");
    Observer_pm2.update = observer_update2;
    subject_register_observer(&Subject_pm, &Observer_pm2);

    printf("通知观察者\n");
    observer_pm_data.lv = 0x5555;
    subject_notify_observers(&Subject_pm);
    // 移除观察者
    printf("通知观察者1\n");
    subject_remove_observer(&Subject_pm, &Observer_pm);
    // 通知观察者
    printf("通知观察者\n");
    subject_notify_observers(&Subject_pm);
    // 通知观察者
    printf("通知观察者\n");
    subject_notify_observers(&Subject_pm);
    return 0;
}
