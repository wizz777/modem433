
#ifndef __PRINCETON_H
#define __PRINCETON_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"




void Analysis_bit_PRINCETON (Tbuf data);
void Init_PRINCETON (void);
void Send_PRINCETON (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __PRINCETON_H */
