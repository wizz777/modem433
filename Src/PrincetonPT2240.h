
#ifndef __PRINCETONPT2240_H
#define __PRINCETON_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"




void Analysis_bit_PRINCETON_PT2240 (Tbuf data);
void Init_PRINCETON_PT2240 (void);
void Send_PRINCETON_PT2240 (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __PRINCETONPT2240_H */
