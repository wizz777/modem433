#include "433Modem.h"
#include "stm32f1xx_hal.h"
#include <string.h>
#include "fonts.h"
#include "ssd1306.h"
#include "Fun_Menu.h"

//#######DATA BUF
#define BUFFER_SIZE 512// 254
uint16_t in=0;
volatile Tbuf buf[BUFFER_SIZE];
uint16_t ind_write = 0;
uint16_t ind_read = 0;

//############

//timer
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;
extern 	char BufTX [100];
//uint16_t Sig_HIGH, Sig_LOW, Sig_ALL;
#define CORRECT 0 //35

void Tim_Init(uint8_t par){
	if (par){
		HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1); //Start_Tim4_Canal1
		HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2); //Start_Tim4_Canal1
	} else {
		HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_1); //Stop_Tim4_Canal1
		HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_2); //Stop_Tim4_Canal1	
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM4){ // PB6
		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1){
			addToBuf(HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1)-CORRECT-in,0);// LOW
			}else if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2){
				in=HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2)+CORRECT;//HIGH
				addToBuf(in,1); 
			}
	}
}	
uint16_t bufDiff(void) {							//Skolko est' dannix dlia analiza
  if (ind_write >= ind_read) {
    return (ind_write - ind_read);
  } else {
    return ((BUFFER_SIZE - ind_read) + ind_write);
  }
}


void Print (void){
	Tbuf t;
	for ( uint8_t i=0; i<254; i++){
		
		t=readFromBuf();	
		
		snprintf(BufTX, sizeof(BufTX),"Time:%d Status:%d  \n\r", t.Time,t.Status);
		HAL_UART_Transmit(&huart1, (uint8_t*) BufTX, strlen(BufTX), 1000);
		//Analysis_bit_CAME(t);
	}

}

void addToBuf(uint16_t sb, uint8_t st) {    //dobavit'
  ind_write++;
  if (ind_write >= BUFFER_SIZE) {
    ind_write = 0;
	//Print();
  }
  buf[ind_write].Time = sb;
	buf[ind_write].Status= st;
}


Tbuf readFromBuf() {				//pro4itat'
  ind_read++;
  if (ind_read >= BUFFER_SIZE) {
    ind_read = 0;
  }
  return buf[ind_read];
}



void clearBuf() {							//ot4istit'
  ind_read = ind_write;
}


void Add_Bit(ProtocolStruct *pr, uint8_t bit){
pr->Code = pr->Code <<1 | bit;
pr->Count_BIT++;	
}

void Pin_TX (uint8_t bit){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, bit);
}
void Pin_TX_EN (uint8_t bit){
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, bit);
}


void Print_Code_LCD (ProtocolStruct *pr, uint32_t HOP, uint32_t FIX ){	
	char LCDTX [24];
	
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	Show_Status(); 
	
	snprintf(LCDTX, sizeof(LCDTX),"%s:", pr->Name_Protocol);
	SSD1306_GotoXY(10,15);
	SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_GotoXY(80,15);
	snprintf(LCDTX, sizeof(LCDTX),"%d Bit",  pr->Count_BIT);
	SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	
		snprintf(LCDTX, sizeof(LCDTX),"%llX",pr->Code);
		SSD1306_GotoXY(10,26);
		SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	if (HOP || FIX){	
		snprintf(LCDTX, sizeof(LCDTX),"FIX --> %X",FIX);
		SSD1306_GotoXY(10,37);
		SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
		
		snprintf(LCDTX, sizeof(LCDTX),"HOP --> %X",HOP);
		SSD1306_GotoXY(10,48);
		SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	}
	SSD1306_UpdateScreen();
	
}


void Print_Code (ProtocolStruct *pr){	
	snprintf(BufTX, sizeof(BufTX),"Protocol %s: %d Bit | KEY:0x%llX HEX  \n\r", pr->Name_Protocol, pr->Count_BIT, pr->Code);
		//HAL_UART_Transmit_IT(&huart1, (uint8_t*) BufTX, strlen(BufTX));//, 1000);
	HAL_UART_Transmit(&huart1, (uint8_t*) BufTX, strlen(BufTX), 1000);
}

uint8_t Check_Interval (ProtocolStruct *pr,uint16_t interval, uint16_t intervel_check){
	if ((intervel_check >= (interval - pr->Interval_DELTA))&&(intervel_check <= (interval + pr->Interval_DELTA))){
	return 1;
	} else return 0;
}
