
#ifndef __FLASH_H
#define __FLASH_H

#include "stm32f1xx_hal.h"


//typedef struct
//{
//  uint8_t Korrec;							/*% сезонной корректировки */
//	
//	uint8_t WeekDay[7];            /*Дни работы программы */

//  uint16_t TimeWork[4];        /*Время запуска программы*/
//  
//  uint8_t TimeZone[15];          /*Время работы зон*/
//  
//}NastoikaDef; 





//uint32_t flash_read(uint32_t address);
//void flash_erase_all_pages(void);
//void flash_erase_page(uint32_t address);
//void flash_unlock(void);
//void flash_write(uint32_t address,uint32_t data);

void Flash_Read_Data(void); 
void Flash_Erase_Data(void); //CLEAR SETTING

void Read(void);//LOAD SETTUNG
void Save(void);//SAVE SETTING

void Flash_Write_Data (void); 
#endif /* __FLASH_H */
