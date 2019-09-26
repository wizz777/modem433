
#ifndef __433MODEM_H
#define __433MODEM_H

#include "stm32f1xx_hal.h"
//#include <string.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

typedef struct 
{
 volatile uint16_t Time;        //??????
 volatile uint8_t Status;        //??????????? ??????
} Tbuf ; 


typedef struct 
{
	char Name_Protocol [20]; 	//Name Protocol
	uint16_t Interval_LONG;		//mks
	uint16_t Interval_SMALL;	//mks
	uint16_t Interval_DELTA;	//+- plavaet signal        
  uint64_t Code;        		//FindCode
	uint64_t Code_LAST;				//Last Find Code
	uint8_t  Count_BIT;				// Counter bits
	uint8_t MIN_Count_BIT;		//minimum dlia priema
	uint8_t  State;						
	uint16_t Last_Time_Itnetval;
	uint8_t Count_Preambula;
	
} ProtocolStruct ; 



void Tim_Init(uint8_t par);									//start/stop SCAN
uint16_t bufDiff(void);											//Skol'co dannix dliya obrabotki
void addToBuf(uint16_t sb, uint8_t st) ;		// dobavit' izmerennii interval v byfer
Tbuf readFromBuf(void);											// pro4itat' iz byfera
void clearBuf(void);												// ot4istit' byfer


void Add_Bit(ProtocolStruct *pr, uint8_t bit); //dobavit' raskodirovannii bit v Find Code
void Pin_TX (uint8_t bit); //Pin
void Pin_TX_EN (uint8_t bit); // Active TX
void Print_Code_LCD (ProtocolStruct *pr, uint32_t HOP, uint32_t FIX );
void Print_Code (ProtocolStruct *pr);					// Print Find Code & Protocol & Count bit
uint8_t Check_Interval (ProtocolStruct *pr, uint16_t interval, uint16_t intervel_check); 

#endif /* __433MODEM_H */
