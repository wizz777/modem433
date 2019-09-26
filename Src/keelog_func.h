
#ifndef __KEELOG_FUNC_H
#define __KEELOG_FUNC_H

//#include "stm32f1xx_hal.h"
#include "433Modem.h"


uint32_t KeeLoq_Encrypt (const uint32_t data, const uint64_t key);
uint32_t KeeLoq_Decrypt (const uint32_t data, const uint64_t key);
uint64_t Normal_Learning (uint32_t data, const uint64_t key); //data -serial nomber (28bit) key - manufacture (64bit)
uint64_t Reverse_BIT (uint64_t data);

#endif /* __KEELOG_FUNC_H */
