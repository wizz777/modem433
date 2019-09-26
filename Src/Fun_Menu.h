
#ifndef __FUN_MEMU_H
#define __FUN_MEMU_H

#include "stm32f1xx_hal.h"

enum {
    KEY_OK=1,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGTH
};

typedef struct 
{
	uint8_t algo;
	uint32_t code;
	uint8_t bit;
	uint8_t repeat;
	uint8_t status;
} BRUT ;

typedef struct 
{
	uint8_t ind1,ind2,ind3;
} TNavigation ; 
extern BRUT Brut;
void SCAN_(uint8_t par);
void Input_PIN(uint8_t key);
void Key_Menu (void);
void Show_Status (void);
void BruteForce (void);
void SendCode(void);

#endif /* __FUN_MEMU_H */
