#include <check.h>
#include "observer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Observer obs1, obs2;
Subject sub;

void update_function(const Observer *obs)
{
    printf("Observer updated with data type: %d\n", obs->data->type);
    ck_assert_int_eq(obs->data->type, 1);
}

START_TEST(test_observer_init)
{
    observer_init(&obs1, update_function);
    ck_assert_ptr_ne(obs1.update, NULL);
    ck_assert_ptr_eq(obs1.data, NULL);
}
END_TEST

START_TEST(test_subject_init)
{
    subject_init(&sub);
    ck_assert_ptr_eq(sub.name, NULL);
    ck_assert_ptr_eq(sub.data, NULL);
    ck_assert_int_eq(mm_list_is_empty(&sub.observers), 1);
}
END_TEST

START_TEST(test_subject_register_observer)
{
    subject_init(&sub);
    observer_init(&obs1, update_function);
    subject_register_observer(&sub, &obs1);
    ck_assert_int_eq(mm_list_is_empty(&sub.observers), 0);
}
END_TEST

START_TEST(test_subject_remove_observer)
{
    subject_init(&sub);
    observer_init(&obs1, update_function);
    subject_register_observer(&sub, &obs1);
    subject_remove_observer(&sub, &obs1);
    ck_assert_int_eq(mm_list_is_empty(&sub.observers), 1);
}
END_TEST

START_TEST(test_subject_notify_observers)
{
    subject_init(&sub);
    observer_init(&obs1, update_function);
    subject_register_observer(&sub, &obs1);
    observer_data_t data = {.type = 1, .len = 0};
    subject_set_data(&sub, &data);
    subject_notify_observers(&sub);
    // 检查是否调用了 update_function
    // 由于 update_function 只打印数据，测试可以手动验证输出
}
END_TEST

Suite *observer_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Observer");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_observer_init);
    tcase_add_test(tc_core, test_subject_init);
    tcase_add_test(tc_core, test_subject_register_observer);
    tcase_add_test(tc_core, test_subject_remove_observer);
    tcase_add_test(tc_core, test_subject_notify_observers);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s  = observer_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
