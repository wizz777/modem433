#include "PrincetonPT2240.h"
//#include "stm32f1xx_hal.h"
#include "433Modem.h"
#include "dwt_stm32_delay.h"


ProtocolStruct PRINCETON_PT2240;

void Init_PRINCETON_PT2240 (void){
snprintf(PRINCETON_PT2240.Name_Protocol , sizeof(PRINCETON_PT2240.Name_Protocol ),"PRINCETON_PT2240");
PRINCETON_PT2240.MIN_Count_BIT=12;
PRINCETON_PT2240.Interval_SMALL = 450; //mks
PRINCETON_PT2240.Interval_LONG =1350;	  //mks
PRINCETON_PT2240.Interval_DELTA = 200;  //+- plavaet signal
}



void Send_Bit_PRINCETON_PT2240 (uint8_t bit){
	if (bit) {
		Pin_TX(1);
		DWT_Delay_us(PRINCETON_PT2240.Interval_LONG); //1
		Pin_TX(0);
		DWT_Delay_us(PRINCETON_PT2240.Interval_SMALL);	
	} else {
		Pin_TX(1);
		DWT_Delay_us(PRINCETON_PT2240.Interval_SMALL); //0
		Pin_TX(0);
		DWT_Delay_us(PRINCETON_PT2240.Interval_LONG);	
	}
}	


void Send_PRINCETON_PT2240 (uint64_t key, uint8_t bit, uint8_t repeat){
Pin_TX_EN(1);
while(repeat--){
  for (uint8_t i=bit;i>0;i--){        
  Send_Bit_PRINCETON_PT2240(bitRead(key, i-1)); 
		}
	Send_Bit_PRINCETON_PT2240(0); //START	
	DWT_Delay_us(PRINCETON_PT2240.Interval_SMALL*33);	 
	}
Pin_TX_EN(0);
}

void Analysis_bit_PRINCETON_PT2240 (Tbuf data){
	switch (PRINCETON_PT2240.State){
		case 0:{
			if ((data.Status == 0) && ((data.Time >= (PRINCETON_PT2240.Interval_SMALL*29 - PRINCETON_PT2240.Interval_DELTA)) && (data.Time <= (PRINCETON_PT2240.Interval_SMALL*49 + PRINCETON_PT2240.Interval_DELTA)))){
			PRINCETON_PT2240.State=1; //Find Preambula
			PRINCETON_PT2240.Code=0;
			PRINCETON_PT2240.Count_BIT=0;
			} else PRINCETON_PT2240.State=0;
		}
		break;
		case 1:{
			 if (data.Status == 1) {
			 PRINCETON_PT2240.Last_Time_Itnetval=data.Time;
			 PRINCETON_PT2240.State=2;
			 } 
		}
			 break;
		case 2:{
			if (data.Status==0 ) { //save interval
				if (data.Time >= (PRINCETON_PT2240.Interval_SMALL*10 + PRINCETON_PT2240.Interval_DELTA)){
				PRINCETON_PT2240.State=1;
			   if (PRINCETON_PT2240.Count_BIT >=PRINCETON_PT2240.MIN_Count_BIT){
					 
					Print_Code(&PRINCETON_PT2240);//Print_PRINCETON_PT2240(); 
				  Print_Code_LCD(&PRINCETON_PT2240 ,0,0);
				 }
					PRINCETON_PT2240.Code=0;
					PRINCETON_PT2240.Count_BIT=0;
					break;
				}
				
				if ((Check_Interval(&PRINCETON_PT2240, PRINCETON_PT2240.Interval_SMALL, PRINCETON_PT2240.Last_Time_Itnetval) && Check_Interval(&PRINCETON_PT2240, PRINCETON_PT2240.Interval_LONG, data.Time))){
							 Add_Bit(&PRINCETON_PT2240,0);
							 PRINCETON_PT2240.State=1;
						 } else if ((Check_Interval(&PRINCETON_PT2240,PRINCETON_PT2240.Interval_LONG,PRINCETON_PT2240.Last_Time_Itnetval) && Check_Interval(&PRINCETON_PT2240,PRINCETON_PT2240.Interval_SMALL, data.Time))){
											 Add_Bit(&PRINCETON_PT2240,1);
											 PRINCETON_PT2240.State=1;
										 }else PRINCETON_PT2240.State=0;
			} else PRINCETON_PT2240.State=0;
		}
		break;
	}
}








//void Analysis_bit_PRINCETON_PT2240 (Tbuf data){
//	switch (State_PRINCETON_PT2240){
//		case 0:{
//			if ((data.Status == 0) && ((data.Time >= (INTERVEL_PRINCETON_SMALL_PT2240*29 - DELTA_PRINCETON_PT2240)) && (data.Time <= (INTERVEL_PRINCETON_SMALL_PT2240*49 + DELTA_PRINCETON_PT2240)))){
//			State_PRINCETON_PT2240=1; //Find Preambula
//			Code_PRINCETON_PT2240=0;
//			Count_bit_PRINCETON_PT2240=0;
//				//snprintf(BufTX, sizeof(BufTX),"Find Preambula 1 \n\r");
//				//HAL_UART_Transmit (&huart1, (uint8_t*) BufTX, strlen(BufTX),1000);
//			} else State_PRINCETON_PT2240=0;
//		}
//		break;
//		case 1:{
//			 if (data.Status == 1) {
//			 Last_Time_Itnetval_PRINCETON_PT2240=data.Time;
//			 State_PRINCETON_PT2240=2;
//			 } 
//		}
//			 break;
//		case 2:{
//			if (data.Status==0 ) { //save interval
//				if (data.Time >= (INTERVEL_PRINCETON_SMALL_PT2240*10 + DELTA_PRINCETON_PT2240)){
//				State_PRINCETON_PT2240=1;
//			   if (Count_bit_PRINCETON_PT2240 >0)Print_PRINCETON_PT2240(); 
//					Code_PRINCETON_PT2240=0;
//					Count_bit_PRINCETON_PT2240=0;
//					break;
//				}
//				
//				if (((Last_Time_Itnetval_PRINCETON_PT2240 >= (INTERVEL_PRINCETON_SMALL_PT2240 - DELTA_PRINCETON_PT2240)) && (Last_Time_Itnetval_PRINCETON_PT2240 <= (INTERVEL_PRINCETON_SMALL_PT2240 + DELTA_PRINCETON_PT2240))) && 
//					   ((data.Time >= (INTERVEL_PRINCETON_LONG_PT2240 - DELTA_PRINCETON_PT2240)) && (data.Time <= (INTERVEL_PRINCETON_LONG_PT2240 + DELTA_PRINCETON_PT2240)))) {
//							 Add_Bit_PRINCETON_PT2240(0);
//							 State_PRINCETON_PT2240=1;
//						 } else if (((Last_Time_Itnetval_PRINCETON_PT2240 >= (INTERVEL_PRINCETON_LONG_PT2240 - DELTA_PRINCETON_PT2240)) && (Last_Time_Itnetval_PRINCETON_PT2240 <= (INTERVEL_PRINCETON_LONG_PT2240 + DELTA_PRINCETON_PT2240))) && 
//					           ((data.Time >= (INTERVEL_PRINCETON_SMALL_PT2240 - DELTA_PRINCETON_PT2240)) && (data.Time <= (INTERVEL_PRINCETON_SMALL_PT2240 + DELTA_PRINCETON_PT2240)))) {
//											 Add_Bit_PRINCETON_PT2240(1);
//											 State_PRINCETON_PT2240=1;
//										 }else State_PRINCETON_PT2240=0;
//			} else State_PRINCETON_PT2240=0;
//		}
//		break;
//	}
//}

