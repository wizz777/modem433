
#ifndef __KEELOG_H
#define __KEELOG_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"



void Analysis_bit_KEELOG (Tbuf data);
void Init_KEELOG (void);
void Send_KEELOG (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __KEELOG_H */
