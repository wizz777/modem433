#include "Fun_Menu.h"
//#include "stm32f1xx_hal.h"
//#include "433Modem.h"
//#include "dwt_stm32_delay.h"
#include "fonts.h"
#include "ssd1306.h"
#include "menu.h"
#include "came.h"
#include "nice.h"
#include "CC1101.h"

//volatile int8_t value1, value2, value0; //+-127
TNavigation Navig;
uint32_t cod=0;
 uint8_t e=0;
uint8_t Status_Menu=1;
BRUT Brut;
char CDCTX [64];
uint8_t SCAN=1;

void SCAN_(uint8_t par){
SCAN=par;
}


uint8_t Check_KEY (void){
	//uint8_t key;
//	HAL_Delay(100);
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)) {
	//key=KEY_OK;
		//value0++;	
	return (KEY_OK);
	}
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)) {
	//value1--;	
	return (KEY_UP);
	}
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2)) {
	//value1++;	
	return (KEY_DOWN);
	}
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1)) {
	//value2--;	
	return (KEY_LEFT);
	}
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)) {
	//value2++;	
	return (KEY_RIGTH);
	}
	return (0);
}

void Key_Menu (void){
	uint8_t key = Check_KEY ();

	switch (Status_Menu){
		case 0:
			break;
		case 1:
			//PIN
			Input_PIN(key);
			break;
		case 2:
			Check_KEY_Menu(key);
			break;
		case 3:
			if (key==KEY_LEFT) 
				Brut.status=2;
			if (key==KEY_RIGTH){ 
				if (Brut.status<10) Brut.status=10; else Brut.status++;
			}
			break;
	}

}

void Input_PIN(uint8_t key){
	HAL_Delay(10);
	
	switch (key){
		case KEY_OK:
			//start pin 
		
				
		
		
		
		
				Status_Menu=2;
				startMenu();
		
				Navig.ind1=0;
				Navig.ind2=0;
				Navig.ind3=0;
				if 	(Brut.algo){
						for(uint8_t i=sizeof(cod)*8; i>0; i-=4){
							//e = cod>>i;
							if (cod>>i) {
								Brut.bit=i+4;
							break;
							}
						}
				if (Brut.algo==1){
					Send_CAME(cod,Brut.bit,Brut.repeat);
					} else if (Brut.algo==2){
					Send_NICE(cod,Brut.bit,Brut.repeat);	
					}	
				Status_Menu=1;
				}
				return;
			//break;
		case KEY_UP:
			  Navig.ind2++;
				if (Navig.ind2>0x0F) Navig.ind2=0;
				break;
		case KEY_DOWN:
				Navig.ind2--;
				if (Navig.ind2>250) Navig.ind2=0x0F;
				break;
		case KEY_LEFT:
					Navig.ind1++;
					if (Navig.ind1>=sizeof(cod)*2) Navig.ind1=0;
					Navig.ind2 = (cod >>Navig.ind1*4) & 0x0F;
				break;
		case KEY_RIGTH:
					Navig.ind1--;
					if (Navig.ind1>250) Navig.ind1=(sizeof(cod)*2)-1;
					Navig.ind2 = (cod >>Navig.ind1*4) & 0x0F;
				break;
	}

	
	cod=cod&(0xFFFFFFFF^0xF<<Navig.ind1*4);
	cod= cod | Navig.ind2<<Navig.ind1*4;
	
	char LCDTX [24];
	//SSD1306_Fill(SSD1306_COLOR_BLACK);
	snprintf(LCDTX, sizeof(LCDTX),"PIN: %08X", cod);
	SSD1306_GotoXY(15,34);
  SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(30,44,106,44,SSD1306_COLOR_BLACK);
	SSD1306_DrawLine(99-(7*Navig.ind1),44,106-(7*Navig.ind1),44,SSD1306_COLOR_WHITE);
	SSD1306_UpdateScreen(); 
}

void SendCode(void){
	Status_Menu=1;
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_GotoXY(30,0);
	char LCDTX [24];
		if (Brut.algo==1){
					snprintf(LCDTX, sizeof(LCDTX),"Send Came");
					} else if (Brut.algo==2){
					snprintf(LCDTX, sizeof(LCDTX),"Send Nice" );		
					}
	SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);			
	SSD1306_DrawLine(0,10,128,10,SSD1306_COLOR_WHITE);
	
	
	SSD1306_UpdateScreen(); 
	
}

void BruteForce (void){
	Status_Menu=3;
	selectMenu(MENU_SCAN_OFF);
	CC1101_Strobe(STX);
	
	
	char LCDTX [24];
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_GotoXY(30,0);
	SSD1306_Puts("BruteForce", &Font_7x10, SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(0,10,128,10,SSD1306_COLOR_WHITE);
	
	
	SSD1306_GotoXY(0,52);
	snprintf(LCDTX, sizeof(LCDTX),"STOP         PAUSE" );
	SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
	
	
	
	//get mask
	uint32_t mask=0;
	for(uint8_t i=0; i<Brut.bit/4; i++){
		mask= mask<<4|0xf;
	}
	
	
	//uint8_t pause=0;
	for(uint32_t i=0xFFFFFF&mask; i>0; i--){
		switch (Brut.status){
		case 1:
			if (Brut.algo==1){
			Send_CAME(i,Brut.bit,Brut.repeat);
			snprintf(LCDTX, sizeof(LCDTX),"CAME(%X,%i,%i) ", i, Brut.bit, Brut.repeat );
			} else if (Brut.algo==2){
			Send_NICE(i,Brut.bit,Brut.repeat);
			snprintf(LCDTX, sizeof(LCDTX),"NICE(%X,%i,%i) ", i, Brut.bit, Brut.repeat );		
			}
			SSD1306_GotoXY(0,34);
			SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
			SSD1306_UpdateScreen(); 
		break;
		
	
		case 2: //STOP
			i=1;
			break;
				
		case 10: //Start PAUSE
			Brut.status++;
				SSD1306_GotoXY(0,52);
				snprintf(LCDTX, sizeof(LCDTX),"STOP         START" );
				SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
				SSD1306_UpdateScreen(); 
		    HAL_Delay(500);
			break;
		case 11: //PAUSE
			i++;
			break;	
		case 12: //EXIT PAUSE
			i+=5;
			Brut.status=1;
		  	SSD1306_GotoXY(0,52);
				snprintf(LCDTX, sizeof(LCDTX),"STOP         PAUSE" );
				SSD1306_Puts(LCDTX, &Font_7x10, SSD1306_COLOR_WHITE);
				SSD1306_UpdateScreen(); 
				HAL_Delay(500);
			break;		
	}
		
		Key_Menu();
		
	}
	
	selectMenu(MENU_SCAN_ON);
	Status_Menu=2;
	startMenu();
}

void Show_Status (void){
//	char LCDTX [24];
	//SSD1306_Fill(SSD1306_COLOR_BLACK);
	SSD1306_DrawLine(0,10,128,10,SSD1306_COLOR_WHITE);
	uint8_t BAT=3;
	
	if (BAT){
	SSD1306_DrawRectangle (105,0, 18,8 ,SSD1306_COLOR_WHITE);
	SSD1306_DrawCircle(124,4, 2,SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(122,2,	122,6 ,SSD1306_COLOR_BLACK);
	 uint8_t pos=107;
	for(uint8_t i=0; i<BAT; i++){
	 SSD1306_DrawFilledRectangle(pos+i*5,2, 3,4 ,SSD1306_COLOR_WHITE); 
	 }
	}
	
	
		if (SCAN){
	SSD1306_DrawRectangle (4,1, 6,6 ,SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(5,0,	5,1 ,SSD1306_COLOR_WHITE);	
	SSD1306_DrawLine(9,0,	9,1 ,SSD1306_COLOR_WHITE);	
			SSD1306_DrawPixel(6,3,SSD1306_COLOR_WHITE);
		  SSD1306_DrawPixel(8,3,SSD1306_COLOR_WHITE);
	
	SSD1306_DrawLine(2,3,	4,3 ,SSD1306_COLOR_WHITE);	
  SSD1306_DrawLine(2,6,	4,6 ,SSD1306_COLOR_WHITE);
	SSD1306_DrawLine(10,3,	12,3 ,SSD1306_COLOR_WHITE);	
  SSD1306_DrawLine(10,6,	12,6 ,SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(0,0);
  snprintf(CDCTX, sizeof(CDCTX),"%0.2f", CC1101_GET_Freq());//GET_SerialID())
    SSD1306_Puts(CDCTX, &Font_7x10, 1);
//			
	//SSD1306_DrawCircle(124,4, 2,SSD1306_COLOR_WHITE);
//	SSD1306_DrawLine(122,2,	122,6 ,SSD1306_COLOR_BLACK);
//	 uint8_t pos=107;
//	for(uint8_t i=0; i<BAT; i++){
//	 
//		SSD1306_DrawFilledRectangle(pos+i*5,2, 3,4 ,SSD1306_COLOR_WHITE); 
//	 
//	 }

   if (SCAN==1){
		
		SSD1306_DrawPixel(2,4,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(2,7,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(12,2,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(12,5,SSD1306_COLOR_BLACK);
		 
		 
		SSD1306_DrawPixel(2,2,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(2,5,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(12,4,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(12,7,SSD1306_COLOR_WHITE);
		 SCAN=2;
	 } else {
		SSD1306_DrawPixel(2,2,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(2,5,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(12,4,SSD1306_COLOR_BLACK);
		SSD1306_DrawPixel(12,7,SSD1306_COLOR_BLACK);
		 
		 
		SSD1306_DrawPixel(2,4,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(2,7,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(12,2,SSD1306_COLOR_WHITE);
		SSD1306_DrawPixel(12,5,SSD1306_COLOR_WHITE);
	   SCAN=1;
	 }
   
	}
	
	
	SSD1306_UpdateScreen();
	
}


