
#ifndef __STARLINE_H
#define __STARLINE_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"



void Analysis_bit_STARLINE (Tbuf data);
void Init_STARLINE (void);
void Send_STARLINE (uint64_t key, uint8_t bit, uint8_t repeat);

#endif /* __STARLINE_H */
