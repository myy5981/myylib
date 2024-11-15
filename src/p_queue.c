#include <myy/p_queue.h>
#include <string.h>

#define swap(ida, idb)                                                                                 \
    do {                                                                                               \
        size_t i;                                                                                      \
        for (i = 0; i < size / 8; i++) {                                                               \
            uint64_t t = ((uint64_t*)(queue + (ida) * size))[i];                                         \
            ((uint64_t*)(queue + (ida) * size))[i] = ((uint64_t*)(queue + (idb) * size))[i];               \
            ((uint64_t*)(queue + (idb) * size))[i] = t;                                                  \
        }                                                                                              \
        for (size_t j = 0; j < size % 8; j++) {                                                        \
            uint8_t t = *(uint8_t*)(queue + (ida) * size + i * 8 + j);                                   \
            *(uint8_t*)(queue + (ida) * size + i * 8 + j) = *(uint8_t*)(queue + (idb) * size + i * 8 + j); \
            *(uint8_t*)(queue + (idb) * size + i * 8 + j) = t;                                           \
        }                                                                                              \
    } while (0)

#define q(idx) queue + (idx) * size

size_t pqueue_in(void* queue, size_t nitems, size_t size, const void* ele, int (*cmp)(const void*, const void*)) {
    if (nitems == 0) {
        memcpy(queue, ele, size);
        return 1;
    }
    size_t ret_v = nitems + 1;
    memcpy(q(nitems), ele, size);
    size_t idx;
    do {
        if ((nitems & 0x01) == 0) {
            idx = (nitems >> 1) - 1;
        } else {
            idx = (nitems - 1) >> 1;
        }
        if (cmp(q(idx), q(nitems)) > 0) {
            swap(idx, nitems);
        } else {
            break;
        }
        nitems = idx;
    } while (idx != 0);
    return ret_v;
}

#define left (2 * idx + 1)
#define right (2 * idx + 2)

size_t pqueue_out(void* queue, size_t nitems, size_t size, void* ele, int (*cmp)(const void*, const void*)) {
    if (nitems == 0) {
        return 0;
    }
    size_t idx = 0;
    memcpy(ele, queue, size);
    nitems = nitems - 1;
    memcpy(queue, q(nitems), size);
    while (left < nitems) {
        if (right < nitems) {
            void* little = cmp(q(idx), q(left)) > 0 ? (cmp(q(left), q(right)) > 0 ? q(right) : q(left))
                                                    : (cmp(q(idx), q(right)) > 0 ? q(right) : q(idx));
            if (little == q(left)) {
                swap(idx, left);
                idx = left;
            } else if (little == q(right)) {
                swap(idx, right);
                idx = right;
            } else {
                break;
            }
        } else {
            // 只有左子节点
            if (cmp(q(idx), q(left)) > 0) {
                swap(idx, left);
            }
            break;
        }
    }
    return nitems;
}

#undef left
#undef right
#undef swap
#undef q
