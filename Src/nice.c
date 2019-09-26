#include "nice.h"
//#include "stm32f1xx_hal.h"

#include "433Modem.h"
#include "dwt_stm32_delay.h"

ProtocolStruct NICE;

void Init_NICE (void){
snprintf(NICE.Name_Protocol , sizeof(NICE.Name_Protocol ),"NICE");
NICE.MIN_Count_BIT = 8;
NICE.Interval_SMALL = 700;
NICE.Interval_LONG =1400;	
NICE.Interval_DELTA = 200;
}

void Send_Bit_NICE (uint8_t bit){
	if (bit) {
		Pin_TX(0);
		DWT_Delay_us(NICE.Interval_LONG); //1
		Pin_TX(1);
		DWT_Delay_us(NICE.Interval_SMALL);	
	} else {
		Pin_TX(0);
		DWT_Delay_us(NICE.Interval_SMALL); //0
		Pin_TX(1);
		DWT_Delay_us(NICE.Interval_LONG);	
	}
}	


void Send_NICE (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1);
while(repeat--){
	Send_Bit_NICE(1); //START
  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_NICE(bitRead(key, i-1)); 
		}
	Pin_TX(0);
	DWT_Delay_us(NICE.Interval_SMALL*34);	 //+2 interval v bit 1 	
	}
Pin_TX_EN(0);
}


void Analysis_bit_NICE (Tbuf data){
	switch (NICE.State){ 
		case 0:{
			if ((data.Status == 0) && ((data.Time >= (NICE.Interval_SMALL*35 - NICE.Interval_DELTA)) && (data.Time <= (NICE.Interval_SMALL*55 + NICE.Interval_DELTA)))){
			NICE.State=1; //Find Preambula
			} else NICE.State=0;
		}
		break;
		case 1:{
			 if (data.Status == 0) {
					break;
			 }else if ((data.Time >= (NICE.Interval_SMALL - NICE.Interval_DELTA)) && (data.Time <= (NICE.Interval_SMALL + NICE.Interval_DELTA))){
				NICE.State=2; //Find 1 level
				 NICE.Code=0;
				 NICE.Count_BIT=0;
			 } else NICE.State=0;	
		 }
		break;
		case 2:{
			if (data.Status==0 ) { //save interval
				if (data.Time >= (NICE.Interval_SMALL*2 + NICE.Interval_DELTA)){
				NICE.State=1;
			   if (NICE.Count_BIT >=NICE.MIN_Count_BIT) {
					 Print_Code(&NICE);
				 Print_Code_LCD(&NICE ,0,0);
				 }
					 break;
				}
			NICE.Last_Time_Itnetval=data.Time;	
			NICE.State=3;
			} else NICE.State=0;
		}
		break;
		case 3:{
			if (data.Status==1){
				if (Check_Interval(&NICE, NICE.Interval_SMALL, NICE.Last_Time_Itnetval) && Check_Interval(&NICE, NICE.Interval_LONG, data.Time)){
							 Add_Bit(&NICE,0);
							 NICE.State=2;
						 } else if (Check_Interval(&NICE,NICE.Interval_LONG,NICE.Last_Time_Itnetval) && Check_Interval(&NICE,NICE.Interval_SMALL, data.Time)){
											 Add_Bit(&NICE,1);
											 NICE.State=2;
										 }else NICE.State=0;
			} else NICE.State=0;
		}
	}
}
