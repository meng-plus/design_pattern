#include "factory_observer.h"
#include <string.h>

// 定义工厂主题结构体
typedef struct factory_Subject
{
    Subject sub;
    mm_list_t node;
} factory_subject_t;

// 初始化主题列表
static mm_list_t subject_list = MM_LIST_INIT(subject_list);

// 比较两个主题节点的名称
int subject_cmp(mm_list_t *node1, mm_list_t *node2)
{
    factory_subject_t *f_sub1 = MM_LIST_ENTRY(node1, factory_subject_t, node);
    factory_subject_t *f_sub2 = MM_LIST_ENTRY(node2, factory_subject_t, node);
    return strcmp(f_sub1->sub.name, f_sub2->sub.name); // 根据名字来判断是否相同
}

// 在主题列表中查找指定名称的主题
static factory_subject_t *find_subject_in_list(const char *name)
{
    mm_list_t *pos;
    factory_subject_t *fsub = NULL;

    mm_list_for_each(pos, &subject_list)
    {
        fsub = OBSERVER_GET_STRUCT_PTR(pos, factory_subject_t, node);
        if (strcmp(fsub->sub.name, name) == 0)
        {
            return fsub;
        }
    }
    return NULL;
}

// 创建并初始化一个新的主题
static factory_subject_t *create_new_subject(const char *name)
{
    factory_subject_t *fsub = OBS_MALLOC(sizeof(factory_subject_t));
    if (fsub == NULL)
    {
        return NULL;
    }

    subject_init(&fsub->sub);
    mm_list_init(&fsub->node);
    fsub->sub.name = name;
    mm_list_insert_after(&subject_list, &fsub->node);

    return fsub;
}

// 获取指定名称的主题实例
Subject *subject_get_instance(const char *name)
{
    factory_subject_t *fsub = NULL;

    OBS_ENTER_CRITICAL();

    // 查找是否已经存在该主题
    fsub = find_subject_in_list(name);
    if (fsub != NULL)
    {
        OBS_EXIT_CRITICAL();
        return &fsub->sub;
    }

    // 创建新的主题
    fsub = create_new_subject(name);
    if (fsub == NULL)
    {
        OBS_EXIT_CRITICAL();
        return NULL; // 内存分配失败
    }

    OBS_EXIT_CRITICAL();
    return &fsub->sub;
}

// 创建主题
int subject_create(Subject **subject, const char *name)
{
    if (subject == NULL)
    {
        return OBS_ERROR_NULL_POINTER;
    }

    OBS_ENTER_CRITICAL();

    *subject = OBS_MALLOC(sizeof(Subject));
    if (*subject == NULL)
    {
        OBS_EXIT_CRITICAL();
        return OBS_ERROR_MALLOC_FAILED;
    }

    (*subject)->name = name;
    subject_init(*subject);

    OBS_EXIT_CRITICAL();
    return 0;
}

// 销毁主题
int subject_destroy(Subject *subject)
{
    if (subject == NULL)
    {
        return OBS_ERROR_NULL_POINTER;
    }

    OBS_ENTER_CRITICAL();
    OBS_FREE(subject);
    OBS_EXIT_CRITICAL();

    return 0;
}
