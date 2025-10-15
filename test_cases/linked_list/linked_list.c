#include "container_of.h"
#include "iterators.h"
#include "test_case_conf.h"
#include "test_frame.h"
#include "test_logging.h"
#include "linked_list/linked_list.h"
#include <errno.h>
#include <stdint.h>
#include <string.h>

#define LIST_TEST_LEN 6

typedef struct
{
    list_node_t node;

    test_case_arg_t* arg;

    int data;

} test_node_t;

int last_data = 0;

static int test_foreach_cb(list_node_t* node)
{
    test_node_t* test_node = container_of(node, test_node_t, node);

    TEST_INFO_TO(test_node->arg, "enter: %s", __PRETTY_FUNCTION__);

    if (0 != test_node->data) {
        TEST_ASSERT_TO(test_node->arg, test_node->data == last_data + 1,
                       "data does not increase");
    } else {
        TEST_ASSERT_TO(test_node->arg, 0 == last_data,
                       "first data is not zero");
    }

    TEST_INFO_TO(test_node->arg, "node->data: %d", test_node->data);

    last_data = test_node->data;

    return 0;
}

test_node_t* ll_get_static_list(test_case_arg_t* arg)
{
    static test_node_t test_arr[LIST_TEST_LEN];

    memset(test_arr, 0, sizeof(test_arr));

    TEST_INFO("setup node pool");

    FOR_ARRAY_I (test_arr) {
        test_arr[i].data = i;
        test_arr[i].node.next = &test_arr[i + 1].node;
        test_arr[i].arg = arg;

        TEST_INFO("node[%d]: {data: %d, next:%p}", i, test_arr[i].data,
                  test_arr[i].node.next);
    }

    test_arr[ARRAY_SIZE(test_arr) - 1].node.next = NULL;

    return &test_arr[0];
}

DEFINE_TESTCASE(ll_iter)
{
    test_node_t* head = ll_get_static_list(arg);

    ITER_LIST (iter, &head->node) {
        TEST_INFO("node->data: %d",
                  container_of(iter, test_node_t, node)->data);
    }

    TEST_INFO("test clear next pointer when iteration");

    uint32_t count = 0;
    ITER_LIST (iter, &head->node) {
        TEST_INFO("node->data: %d",
                  container_of(iter, test_node_t, node)->data);
        iter->next = NULL;

        count++;
    }

    TEST_ASSERT(NULL == head->node.next, "next is not cleared");
    TEST_ASSERT(LIST_TEST_LEN == count, "iterator does not run %d times, count: %d", LIST_TEST_LEN, count);

    return 0;
}

DEFINE_TESTCASE(ll_foreach)
{
    test_node_t* head = ll_get_static_list(arg);

    TEST_INFO("set lastnode->next to NULL\r\nenter foreach");

    last_data = 0;

    return list_foreach(&head->node, test_foreach_cb);
}

// DEFINE_TESTCASE(ll_append)
// {
//     return 0;
// }
