#include "Princeton.h"
//#include "stm32f1xx_hal.h"
//#include "433Modem.h"
#include "dwt_stm32_delay.h"


ProtocolStruct PRINCETON;

void Init_PRINCETON (void){
snprintf(PRINCETON.Name_Protocol , sizeof(PRINCETON.Name_Protocol ),"PRINCETON");
PRINCETON.MIN_Count_BIT=12;
PRINCETON.Interval_SMALL = 150; //mks
PRINCETON.Interval_LONG =450;	  //mks
PRINCETON.Interval_DELTA = 50;  //+- plavaet signal
}

void Send_Bit_PRINCETON (uint8_t bit){
	if (bit) {
		Pin_TX(1);
		DWT_Delay_us(PRINCETON.Interval_LONG); //1
		Pin_TX(0);
		DWT_Delay_us(PRINCETON.Interval_SMALL);	
	} else {
		Pin_TX(1);
		DWT_Delay_us(PRINCETON.Interval_SMALL); //0
		Pin_TX(0);
		DWT_Delay_us(PRINCETON.Interval_LONG);	
	}
}	


void Send_PRINCETON (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1);
while(repeat--){
  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_PRINCETON(bitRead(key, i-1)); 
		}
	Send_Bit_PRINCETON(0); //START	
	DWT_Delay_us(PRINCETON.Interval_SMALL*33);	 
	}
Pin_TX_EN(0);
}

void Analysis_bit_PRINCETON (Tbuf data){
	switch (PRINCETON.State){
		case 0:{
			if ((data.Status == 0) && ((data.Time >= (PRINCETON.Interval_SMALL*29 - PRINCETON.Interval_DELTA)) && (data.Time <= (PRINCETON.Interval_SMALL*49 + PRINCETON.Interval_DELTA)))){
			PRINCETON.State=1; //Find Preambula
			PRINCETON.Code=0;
			PRINCETON.Count_BIT=0;
			} else PRINCETON.State=0;
		}
		break;
		case 1:{
			 if (data.Status == 1) {
			 PRINCETON.Last_Time_Itnetval=data.Time;
			 PRINCETON.State=2;
			 } 
		}
			 break;
		case 2:{
			if (data.Status==0 ) { //save interval
				if (data.Time >= (PRINCETON.Interval_SMALL*10 + PRINCETON.Interval_DELTA)){
				PRINCETON.State=1;
			   if (PRINCETON.Count_BIT >=PRINCETON.MIN_Count_BIT){
					 
					 Print_Code(&PRINCETON);//Print_PRINCETON(); 
				 Print_Code_LCD(&PRINCETON ,0,0);
				 }
					 PRINCETON.Code=0;
					PRINCETON.Count_BIT=0;
					break;
				}
				
				if ((Check_Interval(&PRINCETON, PRINCETON.Interval_SMALL, PRINCETON.Last_Time_Itnetval) && Check_Interval(&PRINCETON, PRINCETON.Interval_LONG, data.Time))){
							 Add_Bit(&PRINCETON,0);
							 PRINCETON.State=1;
						 } else if ((Check_Interval(&PRINCETON,PRINCETON.Interval_LONG,PRINCETON.Last_Time_Itnetval) && Check_Interval(&PRINCETON,PRINCETON.Interval_SMALL, data.Time))){
											 Add_Bit(&PRINCETON,1);
											 PRINCETON.State=1;
										 }else PRINCETON.State=0;
			} else PRINCETON.State=0;
		}
		break;
	}
}

