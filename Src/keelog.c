#include "keelog.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"
#include "dwt_stm32_delay.h"
#include <string.h>
extern 	char BufTX [100];
extern UART_HandleTypeDef huart1;


ProtocolStruct KEELOG;
uint8_t Count_Preambula=0;

void Init_KEELOG (void){
snprintf(KEELOG.Name_Protocol , sizeof(KEELOG.Name_Protocol ),"KEELOG");
KEELOG.MIN_Count_BIT=64;
KEELOG.Interval_SMALL = 400;
KEELOG.Interval_LONG =800;	
KEELOG.Interval_DELTA = 140;
}

void Reverse_Code (uint64_t Code){
uint32_t Key1,Key2;
uint64_t Key;	
for (uint8_t i=0;i<64;i++){ 
Key=Key<<1|bitRead(Code, i);	
//if (i<31) Key1=Key1<<1|bitRead(Code, i); else Key2=Key2<<1|bitRead(Code, i-32);	
}
Key1=Key>>32;
Key2=Key&0x00000000FFFFFFFF;

Print_Code_LCD(&KEELOG,Key2,Key1);

	 snprintf(BufTX, sizeof(BufTX),"Reverse                   Key:0x%X | 0x%X HEX  \n\r", Key1, Key2);
		//HAL_UART_Transmit_IT(&huart1, (uint8_t*) BufTX, strlen(BufTX));//, 1000);
	HAL_UART_Transmit(&huart1, (uint8_t*) BufTX, strlen(BufTX), 1000);
}


void Send_Bit_KEELOG (uint8_t bit){
	if (bit) {
		Pin_TX(1);
		DWT_Delay_us(KEELOG.Interval_SMALL); //1
		Pin_TX(0);
		DWT_Delay_us(KEELOG.Interval_LONG);	
	} else {
		Pin_TX(1);
		DWT_Delay_us(KEELOG.Interval_LONG); //0
		Pin_TX(0);
		DWT_Delay_us(KEELOG.Interval_SMALL);	
	}
}	


void Send_KEELOG (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1); //Enable TX
while(repeat--){	
for (uint8_t i=11;i>0;i--){
    Pin_TX(1);
		DWT_Delay_us(KEELOG.Interval_SMALL); //0
		Pin_TX(0);
		DWT_Delay_us(KEELOG.Interval_SMALL);
}	
DWT_Delay_us(KEELOG.Interval_SMALL*9); //+1 vishe   SEND Preambule

  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_KEELOG(bitRead(key, i-1)); 
		}
	Send_Bit_KEELOG(0); //+2 Status bit
	Send_Bit_KEELOG(0);
		
	Send_Bit_KEELOG(0); //NEDO bit 
	DWT_Delay_us(KEELOG.Interval_SMALL*2);	 //+2 interval END SEND 	
	}
Pin_TX_EN(0); //Disable TX
}

void Analysis_bit_KEELOG (Tbuf data){
	switch (KEELOG.State){ 
		case 0:{
			if ((data.Status == 1) && Check_Interval(&KEELOG,KEELOG.Interval_SMALL, data.Time)){
			KEELOG.State=1; 
			Count_Preambula++;
			} else KEELOG.State=0;
		}
		break;
		case 1:{
			 if ((data.Status == 0) &&  (Check_Interval(&KEELOG,KEELOG.Interval_SMALL, data.Time))){
				KEELOG.State=0;
			  break;
			 }
			if (Count_Preambula>2 && ((data.Time>=KEELOG.Interval_SMALL*9)&& (data.Time<=KEELOG.Interval_SMALL*11))){
				 KEELOG.State=2; //Find Preambula
				 KEELOG.Code=0;
				 KEELOG.Count_BIT=0;
			 } else {
				 KEELOG.State=0;
				 Count_Preambula=0;
			 }
		 }
		break;
		case 2:{
			 if (data.Status == 1) {
			 KEELOG.Last_Time_Itnetval=data.Time;
			 KEELOG.State=3;
			 } 
		}		
		break;
		case 3:{
			if (data.Status==0){
					if (data.Time >= (KEELOG.Interval_SMALL*2 + KEELOG.Interval_DELTA)){ //end posilka
						KEELOG.State=0;
						//Count_Preambula=0;
						if (KEELOG.Count_BIT >0 && (KEELOG.Code_LAST != KEELOG.Code)) {
							KEELOG.Code_LAST=KEELOG.Code;
							Print_Code(&KEELOG);
							Reverse_Code(KEELOG.Code);
						KEELOG.Code=0;
				    KEELOG.Count_BIT=0;
						Count_Preambula=0;	
						}
						break;
					} else if (Check_Interval(&KEELOG, KEELOG.Interval_SMALL, KEELOG.Last_Time_Itnetval) && Check_Interval(&KEELOG, KEELOG.Interval_LONG, data.Time)){
							if(KEELOG.Count_BIT<KEELOG.MIN_Count_BIT) Add_Bit(&KEELOG,1);
							 KEELOG.State=2;
						 } else if (Check_Interval(&KEELOG,KEELOG.Interval_LONG,KEELOG.Last_Time_Itnetval) && Check_Interval(&KEELOG,KEELOG.Interval_SMALL, data.Time)){
											 if(KEELOG.Count_BIT<KEELOG.MIN_Count_BIT) Add_Bit(&KEELOG,0);
											 KEELOG.State=2;
										 }else {
											 KEELOG.State=0;
											 Count_Preambula=0;	
										 }
			} else {
				KEELOG.State=0;
				Count_Preambula=0;		
			}
		}
	}
}
