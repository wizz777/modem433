#include "came.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"
#include "dwt_stm32_delay.h"


ProtocolStruct CAME;

void Init_CAME (void){
snprintf(CAME.Name_Protocol , sizeof(CAME.Name_Protocol ),"CAME");
CAME.MIN_Count_BIT = 8;
CAME.Interval_SMALL = 320;
CAME.Interval_LONG =640;	
CAME.Interval_DELTA = 100;
}

void Send_Bit_CAME (uint8_t bit){
	if (bit) {
		Pin_TX(0);
		DWT_Delay_us(CAME.Interval_LONG); //1
		Pin_TX(1);
		DWT_Delay_us(CAME.Interval_SMALL);	
	} else {
		Pin_TX(0);
		DWT_Delay_us(CAME.Interval_SMALL); //0
		Pin_TX(1);
		DWT_Delay_us(CAME.Interval_LONG);	
	}
}	


void Send_CAME (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1);
while(repeat--){
	Send_Bit_CAME(1); //START
  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_CAME(bitRead(key, i-1)); 
		}
	Pin_TX(0);
	DWT_Delay_us(CAME.Interval_SMALL*34);	 //+2 interval v bit 1 	
	}
Pin_TX_EN(0);
}

void Analysis_bit_CAME (Tbuf data){
	switch (CAME.State){ 
		case 0:{
			if ((data.Status == 0) && ((data.Time >= (CAME.Interval_SMALL*35 - CAME.Interval_DELTA)) && (data.Time <= (CAME.Interval_SMALL*55 + CAME.Interval_DELTA)))){
			CAME.State=1; //Find Preambula
			} else CAME.State=0;
		}
		break;
		case 1:{
			 if (data.Status == 0) {
					break;
			 }else if ((data.Time >= (CAME.Interval_SMALL - CAME.Interval_DELTA)) && (data.Time <= (CAME.Interval_SMALL + CAME.Interval_DELTA))){
				CAME.State=2; //Find 1 level
				 CAME.Code=0;
				 CAME.Count_BIT=0;
			 } else CAME.State=0;	
		 }
		break;
		case 2:{
			if (data.Status==0 ) { //save interval
				if (data.Time >= (CAME.Interval_SMALL*2 + CAME.Interval_DELTA)){
				CAME.State=1;
			   if (CAME.Count_BIT >=CAME.MIN_Count_BIT) {
					 Print_Code(&CAME);
					Print_Code_LCD(&CAME ,0,0);
				 }
					 break;
				}
			CAME.Last_Time_Itnetval=data.Time;	
			CAME.State=3;
			} else CAME.State=0;
		}
		break;
		case 3:{
			if (data.Status==1){
				if (Check_Interval(&CAME, CAME.Interval_SMALL, CAME.Last_Time_Itnetval) && Check_Interval(&CAME, CAME.Interval_LONG, data.Time)){
							 Add_Bit(&CAME,0);
							 CAME.State=2;
						 } else if (Check_Interval(&CAME,CAME.Interval_LONG,CAME.Last_Time_Itnetval) && Check_Interval(&CAME,CAME.Interval_SMALL, data.Time)){
											 Add_Bit(&CAME,1);
											 CAME.State=2;
										 }else CAME.State=0;
			} else CAME.State=0;
		}
	}
}
