#include "factory_observer.h"
#include <string.h>
static mm_list_t subject_list = MM_LIST_INIT(subject_list);

Subject *subject_get_instance(const char *name)
{
    mm_list_t *pos;
    Subject *sub = NULL;
    mm_list_for_each(pos, &subject_list)
    {
        sub = OBSERVER_DATA_ENTRY(pos, Subject, node);
        if (strcmp(sub->name, name) == 0)
        {
            return sub;
        }
    }
    OBS_ENTER_CRITICAL();
    sub = OBS_MALLOC(sizeof(Subject));
    if (sub == NULL)
    {
        OBS_EXIT_CRITICAL();
        return sub;
    }
    sub->name = name;
    mm_list_init(&sub->observers);
    mm_list_insert_after(&subject_list, &sub->node);
    OBS_EXIT_CRITICAL();
    return sub;
}

int subject_create(Subject **subject, const char *name)
{
    if (subject == NULL)
    {
        return OBS_ERROR_NULL_POINTER;
    }
    OBS_ENTER_CRITICAL();

    Subject *sub = NULL;
    mm_list_t *pos;
    mm_list_for_each(pos, &subject_list)
    {
        sub = OBSERVER_DATA_ENTRY(pos, Subject, node);
        if (strcmp(sub->name, name) == 0)
        {
            *subject = sub;
            OBS_EXIT_CRITICAL();
            return OBS_ERROR_DUPLICATE;
        }
    }

    *subject = OBS_MALLOC(sizeof(Subject));
    if (*subject == NULL)
    {
        OBS_EXIT_CRITICAL();
        return OBS_ERROR_MALLOC_FAILED;
    }
    (*subject)->name = name;
    mm_list_init(&(*subject)->observers);
    mm_list_insert_after(&subject_list, &(*subject)->node);
    OBS_EXIT_CRITICAL();
    return 0;
}

int subject_destroy(Subject *subject)
{
    if (subject == NULL)
    {
        return OBS_ERROR_NULL_POINTER;
    }
    OBS_ENTER_CRITICAL();
    mm_list_remove(&subject->node);
    OBS_FREE(subject);
    OBS_EXIT_CRITICAL();
    return 0;
}
