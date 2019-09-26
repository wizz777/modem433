#ifndef _MENU_H_
#define _MENU_H_

#include "stm32f1xx_hal.h"
enum {
    MENU_CANCEL=1,
    MENU_RESET,
    MENU_SCAN_ON,
    MENU_SCAN_OFF,
		BRUTEFORCECAME,
		BRUTEFORCENICE,
		NOISE,
    FREQAM433,
		FREQFM433,
    PINCODE,
    C8_bit,
		C12_bit,
		C16_bit,
		C20_bit,
		C24_bit,
		C28_bit,
		C32_bit,
		SENDCAME,
		N8_bit,
		N12_bit,
		N16_bit,
		N20_bit,
		N24_bit,
		N28_bit,
		N32_bit,
		SENDNICE,
    MENU_PROCESS
};

uint8_t startMenu(void);
void initMenu(void);
void Check_KEY_Menu(uint8_t key);
uint8_t selectMenu(uint8_t par);
#endif
