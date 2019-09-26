#include "Alligator.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"
#include "dwt_stm32_delay.h"
#include <string.h>
extern 	char BufTX [100];
extern UART_HandleTypeDef huart1;


ProtocolStruct ALLIGATOR;
//uint8_t STARLINE_Count_Preambula=0;

void Init_ALLIGATOR (void){
snprintf(ALLIGATOR.Name_Protocol , sizeof(ALLIGATOR.Name_Protocol ),"ALLIGATOR");
ALLIGATOR.MIN_Count_BIT=48;
ALLIGATOR.Interval_SMALL = 600;
ALLIGATOR.Interval_LONG =1000;	
ALLIGATOR.Interval_DELTA = 120;
}

void ALLIGATOR_Reverse_Code (uint64_t Code){
uint32_t Key1,Key2;
uint64_t Key;	
for (uint8_t i=0;i<64;i++){ 
Key=Key<<1|bitRead(Code, i);	
//if (i<31) Key1=Key1<<1|bitRead(Code, i); else Key2=Key2<<1|bitRead(Code, i-32);	
}
Key1=Key>>32;
Key2=Key&0x00000000FFFFFFFF;

Print_Code_LCD(&ALLIGATOR,Key2,Key1);

	 snprintf(BufTX, sizeof(BufTX),"Reverse                   Key:0x%X | 0x%X HEX  \n\r", Key1, Key2);
		//HAL_UART_Transmit_IT(&huart1, (uint8_t*) BufTX, strlen(BufTX));//, 1000);
	HAL_UART_Transmit(&huart1, (uint8_t*) BufTX, strlen(BufTX), 1000);
ALLIGATOR.Code_LAST=0;
}


void Send_Bit_ALLIGATOR (uint8_t bit){
	if (bit) {
		Pin_TX(1);
		DWT_Delay_us(ALLIGATOR.Interval_LONG); //1
		Pin_TX(0);
		DWT_Delay_us(ALLIGATOR.Interval_LONG);	
	} else {
		Pin_TX(1);
		DWT_Delay_us(ALLIGATOR.Interval_SMALL); //0
		Pin_TX(0);
		DWT_Delay_us(ALLIGATOR.Interval_SMALL);	
	}
}	


void Send_ALLIGATOR (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1); //Enable TX
    Pin_TX(1);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2); // START
		Pin_TX(0);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2);
while(repeat--){	
for (uint8_t i=5;i>0;i--){
    Pin_TX(1);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2); //PREAMBULA
		Pin_TX(0);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2);
}	

  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_ALLIGATOR(bitRead(key, i-1)); 
		}
		Pin_TX(1);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2); //END
		Pin_TX(0);
		DWT_Delay_us(ALLIGATOR.Interval_LONG*2); //END
	}
Pin_TX_EN(0); //Disable TX
}

void Analysis_bit_ALLIGATOR (Tbuf data){
	switch (ALLIGATOR.State){ 
		case 0:{
			if (data.Status == 1){ 
				if ( Check_Interval(&ALLIGATOR,ALLIGATOR.Interval_SMALL*4, data.Time)){
			ALLIGATOR.State=1; 
			ALLIGATOR.Count_Preambula++;
			} else 
				if (ALLIGATOR.Count_Preambula>2 ){
						
						ALLIGATOR.Code=0;
						ALLIGATOR.Count_BIT=0;
					  
					  ALLIGATOR.Last_Time_Itnetval=data.Time;
						ALLIGATOR.State=3;
					}
			
				} else {
					ALLIGATOR.State=0;
					ALLIGATOR.Count_Preambula=0;
		}
	}
		break;
		case 1:{
			if ((data.Status == 0)&&  (Check_Interval(&ALLIGATOR,ALLIGATOR.Interval_LONG, data.Time))){//Find Preambula
			  ALLIGATOR.State=0;
				
				//STARLINE.State=2; //Find Preambula
			} else {
				
			ALLIGATOR.State=0;
			ALLIGATOR.Count_Preambula=0;
			}
	 }
		break;
	 
		case 2:{
			 if (data.Status == 1) {
				if (data.Time > (ALLIGATOR.Interval_LONG + ALLIGATOR.Interval_DELTA)){ //end posilka
					ALLIGATOR.State=0;
						//Count_Preambula=0;
						if (ALLIGATOR.Count_BIT >=ALLIGATOR.MIN_Count_BIT && (ALLIGATOR.Code_LAST != ALLIGATOR.Code)) {
							ALLIGATOR.Code_LAST=ALLIGATOR.Code;
							Print_Code(&ALLIGATOR);
							ALLIGATOR_Reverse_Code(ALLIGATOR.Code);
							ALLIGATOR.Code=0;
							ALLIGATOR.Count_BIT=0;
							ALLIGATOR.Count_Preambula=0;
							ALLIGATOR.State=0;
						}
						break;
				}else{					
			 ALLIGATOR.Last_Time_Itnetval=data.Time;
			 ALLIGATOR.State=3;
			 } 
		}	
	}			 
		break;
		case 3:{
			if (data.Status==0){
					if (Check_Interval(&ALLIGATOR, ALLIGATOR.Interval_SMALL, ALLIGATOR.Last_Time_Itnetval) && Check_Interval(&ALLIGATOR, ALLIGATOR.Interval_SMALL, data.Time)){
							//if(ALLIGATOR.Count_BIT<64) 
								Add_Bit(&ALLIGATOR,0);
							 ALLIGATOR.State=2;
						 } else if (Check_Interval(&ALLIGATOR,ALLIGATOR.Interval_LONG,ALLIGATOR.Last_Time_Itnetval) && Check_Interval(&ALLIGATOR,ALLIGATOR.Interval_LONG, data.Time)){
											// if(ALLIGATOR.Count_BIT<64) 
												 Add_Bit(&ALLIGATOR,1);
											 ALLIGATOR.State=2;
										 }else {
											 ALLIGATOR.State=0;
											 ALLIGATOR.Count_Preambula=0;	
										 }
			} else {
				ALLIGATOR.State=0;
				ALLIGATOR.Count_Preambula=0;		
			}
		}
		break;
	}
	
}
