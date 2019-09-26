#include "StarLine.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"
#include "dwt_stm32_delay.h"
#include <string.h>
extern 	char BufTX [100];
extern UART_HandleTypeDef huart1;


ProtocolStruct STARLINE;
//uint8_t STARLINE_Count_Preambula=0;

void Init_STARLINE (void){
snprintf(STARLINE.Name_Protocol , sizeof(STARLINE.Name_Protocol ),"STARLINE");
STARLINE.MIN_Count_BIT=64;
STARLINE.Interval_SMALL = 250;
STARLINE.Interval_LONG =500;	
STARLINE.Interval_DELTA = 120;
}

void STARLINE_Reverse_Code (uint64_t Code){
uint32_t Key1,Key2;
uint64_t Key;	
for (uint8_t i=0;i<64;i++){ 
Key=Key<<1|bitRead(Code, i);	
//if (i<31) Key1=Key1<<1|bitRead(Code, i); else Key2=Key2<<1|bitRead(Code, i-32);	
}
Key1=Key>>32;
Key2=Key&0x00000000FFFFFFFF;

Print_Code_LCD(&STARLINE,Key2,Key1);

	 snprintf(BufTX, sizeof(BufTX),"Reverse                   Key:0x%X | 0x%X HEX  \n\r", Key1, Key2);
		//HAL_UART_Transmit_IT(&huart1, (uint8_t*) BufTX, strlen(BufTX));//, 1000);
	HAL_UART_Transmit(&huart1, (uint8_t*) BufTX, strlen(BufTX), 1000);
STARLINE.Code_LAST=0;
}


void Send_Bit_STARLINE (uint8_t bit){
	if (bit) {
		Pin_TX(1);
		DWT_Delay_us(STARLINE.Interval_SMALL); //1
		Pin_TX(0);
		DWT_Delay_us(STARLINE.Interval_SMALL);	
	} else {
		Pin_TX(1);
		DWT_Delay_us(STARLINE.Interval_LONG); //0
		Pin_TX(0);
		DWT_Delay_us(STARLINE.Interval_LONG);	
	}
}	


void Send_STARLINE (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1); //Enable TX
    Pin_TX(1);
		DWT_Delay_us(STARLINE.Interval_LONG*2); // START
		Pin_TX(0);
		DWT_Delay_us(STARLINE.Interval_LONG*2);
while(repeat--){	
for (uint8_t i=5;i>0;i--){
    Pin_TX(1);
		DWT_Delay_us(STARLINE.Interval_LONG*2); //PREAMBULA
		Pin_TX(0);
		DWT_Delay_us(STARLINE.Interval_LONG*2);
}	

  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_STARLINE(bitRead(key, i-1)); 
		}
		Pin_TX(1);
		DWT_Delay_us(STARLINE.Interval_LONG*2); //END
		Pin_TX(0);
		DWT_Delay_us(STARLINE.Interval_LONG*2); //END
	}
Pin_TX_EN(0); //Disable TX
}

void Analysis_bit_STARLINE (Tbuf data){
	switch (STARLINE.State){ 
		case 0:{
			if (data.Status == 1){ 
				if ( Check_Interval(&STARLINE,STARLINE.Interval_LONG*2, data.Time)){
			STARLINE.State=1; 
			STARLINE.Count_Preambula++;
			} else 
				if (STARLINE.Count_Preambula>4 ){
						//STARLINE.State=1; 
						STARLINE.Code=0;
						STARLINE.Count_BIT=0;
					  
					  STARLINE.Last_Time_Itnetval=data.Time;
						STARLINE.State=3;
					}
			
				} else {
					STARLINE.State=0;
					STARLINE.Count_Preambula=0;
		}
	}
		break;
		case 1:{
			if ((data.Status == 0)&&  (Check_Interval(&STARLINE,STARLINE.Interval_LONG*2, data.Time))){//Find Preambula
			  STARLINE.State=0;
				
				//STARLINE.State=2; //Find Preambula
			} else {
				
			STARLINE.State=0;
			STARLINE.Count_Preambula=0;
			}
	 }
		break;
	 
		case 2:{
			 if (data.Status == 1) {
				if (data.Time > (STARLINE.Interval_LONG + STARLINE.Interval_DELTA)){ //end posilka
					STARLINE.State=0;
						//Count_Preambula=0;
						if (STARLINE.Count_BIT >0 && (STARLINE.Code_LAST != STARLINE.Code)) {
							STARLINE.Code_LAST=STARLINE.Code;
							Print_Code(&STARLINE);
							STARLINE_Reverse_Code(STARLINE.Code);
							STARLINE.Code=0;
							STARLINE.Count_BIT=0;
							STARLINE.Count_Preambula=0;
							STARLINE.State=0;
						}
						break;
				}else{					
			 STARLINE.Last_Time_Itnetval=data.Time;
			 STARLINE.State=3;
			 } 
		}	
	}			 
		break;
		case 3:{
			if (data.Status==0){
					if (Check_Interval(&STARLINE, STARLINE.Interval_SMALL, STARLINE.Last_Time_Itnetval) && Check_Interval(&STARLINE, STARLINE.Interval_SMALL, data.Time)){
							if(STARLINE.Count_BIT<STARLINE.MIN_Count_BIT) Add_Bit(&STARLINE,1);
							 STARLINE.State=2;
						 } else if (Check_Interval(&STARLINE,STARLINE.Interval_LONG,STARLINE.Last_Time_Itnetval) && Check_Interval(&STARLINE,STARLINE.Interval_LONG, data.Time)){
											 if(STARLINE.Count_BIT<STARLINE.MIN_Count_BIT) Add_Bit(&STARLINE,0);
											 STARLINE.State=2;
										 }else {
											 STARLINE.State=0;
											 STARLINE.Count_Preambula=0;	
										 }
			} else {
				STARLINE.State=0;
				STARLINE.Count_Preambula=0;		
			}
		}
		break;
	}
	
}
