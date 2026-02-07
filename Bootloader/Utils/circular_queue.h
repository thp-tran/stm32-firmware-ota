#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint8_t *buffer; // buffer storage
    uint16_t size;   // buffer size
    uint16_t head;   // write index
    uint16_t tail;   // read index
    uint16_t count;  // number of stored bytes
} circular_queue_t;

/* Initialize circular queue */
void cq_init(circular_queue_t *q, uint8_t *buf, uint16_t size);

/* Push 1 byte into queue (return false if full) */
bool cq_push(circular_queue_t *q, uint8_t data);

/* Pop 1 byte from queue (return false if empty) */
bool cq_pop(circular_queue_t *q, uint8_t *data);

/* Check if queue is empty */
bool cq_is_empty(circular_queue_t *q);

/* Check if queue is full */
bool cq_is_full(circular_queue_t *q);

/* Get number of stored bytes */
uint16_t cq_count(circular_queue_t *q);
