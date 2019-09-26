
#ifndef __CAME_H
#define __CAME_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"



void Analysis_bit_CAME (Tbuf data);
void Init_CAME (void);
void Send_CAME (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __CAME_H */
