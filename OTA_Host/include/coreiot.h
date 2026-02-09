#ifndef COREIOT_H
#define COREIOT_H
#include "global.h"
#include "min_handle.h"
typedef enum
{
    ATTRIBUTE,
    REQUEST,
} COREIOT_MSG_TYPE;

void coreiot_task(void *pvParameters);
#endif