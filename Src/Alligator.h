
#ifndef __ALLIGATOR_H
#define __ALLIGATOR_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"



void Analysis_bit_ALLIGATOR (Tbuf data);
void Init_ALLIGATOR (void);
void Send_ALLIGATOR (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __ALLIGATOR_H */
