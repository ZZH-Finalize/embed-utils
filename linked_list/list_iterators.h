#ifndef __LIST_ITERATORS_H__
#define __LIST_ITERATORS_H__

#include <stddef.h>

#define ITER_LIST(iter_name, head)                             \
    for (typeof(head) iter_name = head, __next = NULL;         \
         (NULL != iter_name) && (__next = iter_name->next, 1); \
         iter_name = __next)

#endif // __LIST_ITERATORS_H__
