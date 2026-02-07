#include "circular_queue.h"

void cq_init(circular_queue_t *q, uint8_t *buf, uint16_t size)
{
    q->buffer = buf;
    q->size = size;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

bool cq_is_empty(circular_queue_t *q)
{
    return (q->count == 0);
}

bool cq_is_full(circular_queue_t *q)
{
    return (q->count == q->size);
}

uint16_t cq_count(circular_queue_t *q)
{
    return q->count;
}

bool cq_push(circular_queue_t *q, uint8_t data)
{
    if (cq_is_full(q))
    {
        return false; // buffer full
    }

    q->buffer[q->head] = data;
    q->head++;

    if (q->head >= q->size)
        q->head = 0;

    q->count++;
    return true;
}

bool cq_pop(circular_queue_t *q, uint8_t *data)
{
    if (cq_is_empty(q))
    {
        return false; // buffer empty
    }

    *data = q->buffer[q->tail];
    q->tail++;

    if (q->tail >= q->size)
        q->tail = 0;

    q->count--;
    return true;
}
