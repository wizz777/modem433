
#ifndef __NICE_H
#define __NICE_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"



void Analysis_bit_NICE(Tbuf data);
void Init_NICE (void);
void Send_NICE (uint64_t key, uint8_t bit, uint8_t repeat);
#endif /* __NICE_H */
