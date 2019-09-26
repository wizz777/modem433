#include "Fun_Menu.h"
#include "menu.h"
#include "fonts.h"
#include "ssd1306.h"
#include "433Modem.h"
#include "Fun_Menu.h"
#include "CC1101.h"


//extern BRUT Brut;


typedef struct {
	void       *Next;
	void       *Previous;
	void       *Parent;
	void       *Child;
	uint8_t     Select;
	const char  Text[20];
} menuItem;

menuItem* selectedMenuItem; // Tekushee menu
char* ParentText[20];

//menuItem* menuStack[10];
//volatile uint8_t menuStackTop;


#define MAKE_MENU(Name, Next, Previous, Parent, Child, Select, Text) \
  extern menuItem Next;     \
	extern menuItem Previous; \
	extern menuItem Parent;   \
	extern menuItem Child;  \
	menuItem Name = {(void*)&Next, (void*)&Previous, (void*)&Parent, (void*)&Child, (uint8_t)Select, { Text }}


#define PREVIOUS   ((menuItem*)(selectedMenuItem->Previous))
#define NEXT       ((menuItem*)(selectedMenuItem->Next))
#define PARENT     ((menuItem*)(selectedMenuItem->Parent))
#define CHILD      ((menuItem*)(selectedMenuItem->Child))
#define SELECT		 ((selectedMenuItem->Select))
 
	
char strNULL[] = "";

#define NULL_ENTRY Null_Menu
menuItem        Null_Menu = {(void*)0, (void*)0, (void*)0, (void*)0, 0, {0x00}};

//                 NEXT,      PREVIOUS     PARENT,     CHILD
MAKE_MENU(m_s1i1,  m_s1i2,    NULL_ENTRY,  NULL_ENTRY, m_s2i1,       0, "Work");
MAKE_MENU(m_s1i2,  m_s1i3,    m_s1i1,      NULL_ENTRY, m_s3i1,       0, "Settings");
MAKE_MENU(m_s1i3,  NULL_ENTRY,m_s1i2,      NULL_ENTRY, NULL_ENTRY,   MENU_RESET, "Reset");

// Start Work
MAKE_MENU(m_s2i1,  m_s2i2,    	NULL_ENTRY,  m_s1i1,     NULL_ENTRY,   MENU_SCAN_ON, "Scan_ON");
MAKE_MENU(m_s2i2,  m_s2i3, 		m_s2i1,      m_s1i1,     NULL_ENTRY,   MENU_SCAN_OFF, "Scan_Off");
MAKE_MENU(m_s2i3,  m_s2i4,    m_s2i2,      m_s1i1,     m_s5i1,   0, "Bruteforce");
MAKE_MENU(m_s2i4,  NULL_ENTRY,m_s2i3,      m_s1i1,     NULL_ENTRY,   NOISE, "Noise");

// Settings MENU
MAKE_MENU(m_s3i1,  m_s3i2,    NULL_ENTRY,  m_s1i2,     m_s4i1,       0, "Frequency");
MAKE_MENU(m_s3i2,  NULL_ENTRY,m_s3i1,      m_s1i2,     NULL_ENTRY,       PINCODE, "Pin");


// Pressure Frequency
MAKE_MENU(m_s4i1,  m_s4i2,    NULL_ENTRY,  m_s3i1,     NULL_ENTRY,   FREQAM433, "AM 433.92");
MAKE_MENU(m_s4i2,  NULL_ENTRY,m_s4i1,      m_s3i1,     NULL_ENTRY,   FREQFM433, "FM 433.92");

// Time BRUTEFORCE
MAKE_MENU(m_s5i1,  m_s5i2,    NULL_ENTRY,  m_s2i4,     m_s6i3,   0, "CAME");
MAKE_MENU(m_s5i2,  NULL_ENTRY,m_s5i1,      m_s2i4,     m_s7i3,   0, "NICE");

// Pressure CAME
MAKE_MENU(m_s6i1,  m_s6i2,	NULL_ENTRY,    m_s5i1,     NULL_ENTRY,   SENDCAME, "SendCame");
MAKE_MENU(m_s6i2,  m_s6i3,    m_s6i1,  		 m_s5i1,     NULL_ENTRY,   C8_bit, "08 bit");
MAKE_MENU(m_s6i3,  m_s6i4,		m_s6i2,      m_s5i1,     NULL_ENTRY,   C12_bit, "12 bit");
MAKE_MENU(m_s6i4,  m_s6i5,		m_s6i3,      m_s5i1,     NULL_ENTRY,   C16_bit, "16 bit");
MAKE_MENU(m_s6i5,  m_s6i6,		m_s6i4,      m_s5i1,     NULL_ENTRY,   C20_bit, "20 bit");
MAKE_MENU(m_s6i6,  m_s6i7,		m_s6i5,      m_s5i1,     NULL_ENTRY,   C24_bit, "24 bit");
MAKE_MENU(m_s6i7,  m_s6i8,		m_s6i6,      m_s5i1,     NULL_ENTRY,   C28_bit, "28 bit");
MAKE_MENU(m_s6i8,  NULL_ENTRY,m_s6i7,      m_s5i1,     NULL_ENTRY,   C32_bit, "32 bit");


// Pressure NICE
MAKE_MENU(m_s7i1,  m_s7i1,NULL_ENTRY,      m_s5i2,     NULL_ENTRY,   SENDNICE, "SendNice");
MAKE_MENU(m_s7i2,  m_s7i2,    m_s7i1,  		 m_s5i2,     NULL_ENTRY,   N8_bit, "08 bit");
MAKE_MENU(m_s7i3,  m_s7i3,		m_s7i2,      m_s5i2,     NULL_ENTRY,   N12_bit, "12 bit");
MAKE_MENU(m_s7i4,  m_s7i4,		m_s7i3,      m_s5i2,     NULL_ENTRY,   N16_bit, "16 bit");
MAKE_MENU(m_s7i5,  m_s7i5,		m_s7i4,      m_s5i2,     NULL_ENTRY,   N20_bit, "20 bit");
MAKE_MENU(m_s7i6,  m_s7i6,		m_s7i5,      m_s5i2,     NULL_ENTRY,   N24_bit, "24 bit");
MAKE_MENU(m_s7i7,  m_s7i7,		m_s7i6,      m_s5i2,     NULL_ENTRY,   N28_bit, "28 bit");
MAKE_MENU(m_s7i8,  NULL_ENTRY,m_s7i7,      m_s5i2,     NULL_ENTRY,   N32_bit, "32 bit");
//MAKE_MENU(m_s7i8,  NULL_ENTRY,m_s7i7,      m_s5i2,     NULL_ENTRY,   SENDNICE, "SendNice");

//// Settings MENU
//MAKE_MENU(m_s3i1,  m_s3i2,    NULL_ENTRY,  m_s1i2,     m_s4i1,       0, "Pressure");
//MAKE_MENU(m_s3i2,  NULL_ENTRY,m_s3i1,      m_s1i2,     m_s5i1,       0, "Time");

//// Pressure MENU
//MAKE_MENU(m_s4i1,  m_s4i2,    NULL_ENTRY,  m_s3i1,     NULL_ENTRY,   MENU_SENS1, "Sensor 1");
//MAKE_MENU(m_s4i2,  NULL_ENTRY,m_s4i1,      m_s3i1,     NULL_ENTRY,   MENU_SENS2, "Sensor 2");

//// Time MENU
//MAKE_MENU(m_s5i1,  m_s5i2,    NULL_ENTRY,  m_s3i2,     NULL_ENTRY,   MENU_WARM, "Warm");
//MAKE_MENU(m_s5i2,  NULL_ENTRY,m_s5i1,      m_s3i2,     NULL_ENTRY,   MENU_PROCESS, "Process");


void menuChange(menuItem* NewMenu)
{
	if ((void*)NewMenu == (void*)&NULL_ENTRY)
	  return;
  //ParentText =((char *)NewMenu->Text);
	selectedMenuItem = NewMenu;
}


char* menuText(int8_t menuShift)
{
	int8_t i;
	menuItem* tempMenu;
 
	if ((void*)selectedMenuItem == (void*)&NULL_ENTRY)
	  return strNULL;
 
	i = menuShift;
	tempMenu = selectedMenuItem;
	if (i>0) {
		while( i!=0 ) {
			if ((void*)tempMenu != (void*)&NULL_ENTRY) {
				tempMenu = (menuItem*)(tempMenu->Next);
			}
			i--;
		}
	} else {
		while( i!=0 ) {
			if ((void*)tempMenu != (void*)&NULL_ENTRY) {
				tempMenu = (menuItem*)(tempMenu->Previous);
			}
			i++;
		}
	}
 
	if ((void*)tempMenu == (void*)&NULL_ENTRY) {
		return strNULL;
	} else {
		return ((char *)tempMenu->Text);
	}
}


void dispMenu(void) {
	char buf[25];
	
	SSD1306_Fill(SSD1306_COLOR_BLACK);
	      Show_Status();
				SSD1306_GotoXY(50,0);
				SSD1306_Puts("MENU", &Font_7x10, SSD1306_COLOR_WHITE);
	      SSD1306_DrawLine(0,10,128,10,SSD1306_COLOR_WHITE);
	
 		    snprintf(buf, sizeof(buf), "%s",menuText(-2)); 	
	      SSD1306_GotoXY(40,12);
				SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
	
				snprintf(buf, sizeof(buf), "%s",menuText(-1)); 	
	      SSD1306_GotoXY(40,22);
				SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
	
				snprintf(buf, sizeof(buf), ">>%s<<",menuText(0)); 	
	      SSD1306_GotoXY(26,32);
				SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
		    
	
	      snprintf(buf, sizeof(buf), "%s",menuText(1)); 	
	      SSD1306_GotoXY(40,42);
				SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
				snprintf(buf, sizeof(buf), "%s",menuText(2)); 	
	      SSD1306_GotoXY(40,52);
				SSD1306_Puts(buf, &Font_7x10, SSD1306_COLOR_WHITE);
	
	SSD1306_UpdateScreen(); 

}


uint8_t selectMenu(uint8_t par) {
	switch (par) {
	case MENU_SCAN_ON: { 
		Tim_Init(1);
		CC1101_Strobe(SRX);
		SCAN_(1);
		//selectedMenuItem = (menuItem*)&m_s1i1;
		break;
	}
	case MENU_SCAN_OFF: { 
	  Tim_Init(0);
		CC1101_Strobe(SIDLE);
		SCAN_(0);
		//selectedMenuItem = (menuItem*)&m_s1i1;
		break;
	}
	case C8_bit: {
		Brut.algo=1;
		Brut.bit=8;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C12_bit: {
		Brut.algo=1;
		Brut.bit=12;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C16_bit: {
		Brut.algo=1;
		Brut.bit=16;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C20_bit: {
		Brut.algo=1;
		Brut.bit=20;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C24_bit: {
		Brut.algo=1;
		Brut.bit=24;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C28_bit: {
		Brut.algo=1;
		Brut.bit=28;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case C32_bit: {
		Brut.algo=1;
		Brut.bit=32;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case SENDCAME: {
		Brut.algo=1;
		//Brut.bit=32;
		Brut.repeat=4;
		Brut.status=1;
		SendCode();
		break;
	}
	case SENDNICE: {
		Brut.algo=2;
		//Brut.bit=32;
		Brut.repeat=4;
		Brut.status=1;
		SendCode();
		break;
	}
		case N8_bit: {
		Brut.algo=2;
		Brut.bit=8;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N12_bit: {
		Brut.algo=2;
		Brut.bit=12;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N16_bit: {
		Brut.algo=2;
		Brut.bit=16;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N20_bit: {
		Brut.algo=2;
		Brut.bit=20;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N24_bit: {
		Brut.algo=2;
		Brut.bit=24;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N28_bit: {
		Brut.algo=2;
		Brut.bit=28;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
	case N32_bit: {
		Brut.algo=2;
		Brut.bit=32;
		Brut.repeat=4;
		Brut.status=1;
		BruteForce();
		break;
	}
//	case BRUTEFORCECAME: {
//		Brut.algo=1;
//		Brut.bit=12;
//		Brut.code=0xFF;
//		Brut.repeat=3;
//		Brut.status=1;
//		//BruteForce(1);
//		break;
//	}
//	case BRUTEFORCENICE: {
//		
//		break;
//	}
	case NOISE: { 
		
		break;
	}
	case FREQAM433: { 
		CC1101_Config(AM_433);
		CC1101_Strobe(SRX);
		SCAN_(1);
		break;
	}
	case FREQFM433: { 
		CC1101_Config(FM_433);
		CC1101_Strobe(SRX);
		SCAN_(1);
		break;
	}
	case PINCODE: { 
		
		break;
	}
//	case MENU_MODE3: { // ??? ?????????? ????? ?????? Mode 3
//		
//		break;
//	}
	case MENU_RESET: { // ??? ?????????? ????? ?????? Reset
	NVIC_SystemReset();
		break;
	}
	// ... ? ??? ????? ?? ???? ???????
	}

	// ????? ?????? ???????????? ? ??????? ????.
	//startMenu();
	return 1;
}

void Check_KEY_Menu(uint8_t key){
	switch (key){
		case KEY_OK:
			if (SELECT) {
				//START FUN
				selectMenu(SELECT);
				return ;
			} else {
				menuChange(CHILD);	
		} 
			break;
		case KEY_UP:
			menuChange(PREVIOUS);
			break;
		case KEY_DOWN:
			menuChange(NEXT);
			break;
		case KEY_LEFT:
			menuChange(PARENT);
			break;
		case KEY_RIGTH:
			menuChange(CHILD);
			break;
		
			
	}
	dispMenu();
}


uint8_t startMenu(void){
	selectedMenuItem = (menuItem*)&m_s1i1;
	dispMenu();
	return (1);
}

void initMenu() {
	selectedMenuItem = (menuItem*)&m_s1i1;
	//dispMenu();
}

