
//#include "GLOBALS.h"
//#include "stm32f1xx_hal.h"
#include "CC1101.h"


#define UP_CS_CC1101    HAL_GPIO_WritePin(CS_PORT,CS_PIN,GPIO_PIN_SET); 
#define DOWN_CS_CC1101   HAL_GPIO_WritePin(CS_PORT,CS_PIN,GPIO_PIN_RESET);

uint32_t fr=0;

const uint8_t	Cfg_AM_433[]={
  IOCFG2,   0x0D,        //RX PIN	
	IOCFG0,   0x2E, //0x0D //TX PIN  0x2E - 3-e sostoyanie
	FIFOTHR,  0x07,
	PKTCTRL0, 0x32, //<-- Asinxronii rejim
	FSCTRL1,  0x0E,
	FREQ2,    0x10,
	FREQ1,    0xB0,
	FREQ0,    0x7F,
	MDMCFG4,  0x17,
	MDMCFG3,  0x32,
	MDMCFG2,  0x30, //<---OOK/ASK
	MDMCFG1,  0x23,
	MDMCFG0,  0xF8,
	MCSM0,    0x18,
	FOCCFG,   0x18,
	AGCCTRL2, 0x07,
	AGCCTRL1, 0x00,
	AGCCTRL0, 0x91,
	WORCTRL,  0xFB,
	FREND1,   0xB6,
	FREND0,   0x11,
	FSCAL3,   0xE9,
	FSCAL2,   0x2A,
	FSCAL1,   0x00,
	FSCAL0,   0x1F,
	TEST2,    0x88,
	TEST1,    0x31,
	TEST0,    0x09,
	PATABLE,  0x00,
	PATABLE,  0xC0,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00
};


const uint8_t Cfg_FM_433[]={
  IOCFG2,   0x0D,
	IOCFG0,   0x2E,
	FIFOTHR,  0x47,
	PKTCTRL0, 0x32,
	FSCTRL1,  0x06,
	FREQ2,    0x10,
	FREQ1,    0xB0,
	FREQ0,    0x7F,//0x40, /* 0xAD */
	MDMCFG4,  0xC7,
	MDMCFG3,  0x32,
	MDMCFG1,  0x23,
	MDMCFG2,  0x00,
	MDMCFG0,  0xF8,
	DEVIATN,  0x04,
	MCSM0,    0x18,
	FOCCFG,   0x19,
	AGCCTRL2, 0x07,
	AGCCTRL1, 0x00,
	AGCCTRL0, 0x91,
	WORCTRL,  0xFB,
	FREND1,   0xB6,
	FREND0,   0x10,
	FSCAL3,   0xE9,
	FSCAL2,   0x2A,
	FSCAL1,   0x00,
	FSCAL0,   0x1F,
	TEST2,    0x81,
	TEST1,    0x35,	
	TEST0,    0x09,
	PATABLE,  0xC0,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00,
	PATABLE,  0x00
};

const uint8_t Cfg_WFM_313[]={
  0
};

const uint8_t Cfg_WFM_433[]={
  0
};

// Hold pointer to inited HAL SPI device
static SPI_HandleTypeDef * CC1101_SPI;
static GPIO_TypeDef * CS_PORT;
uint16_t CS_PIN;


uint8_t CC1101_Init (SPI_HandleTypeDef * hspi, GPIO_TypeDef * cs_port, uint16_t cs_pin){
	CC1101_SPI = hspi;
	CS_PORT = cs_port;
	CS_PIN = cs_pin;
	
	if (HAL_SPI_Init(CC1101_SPI)!=HAL_OK) return(0);
	
	return(1);
}
void CC1101_Strobe (uint8_t strob){
	
	DOWN_CS_CC1101;
	HAL_SPI_Transmit(CC1101_SPI, &strob, 1,100);
	UP_CS_CC1101;
}
//void CC1101_Config1 (const uint8_t *Cfg, uint8_t size){
//	
//	HAL_Delay(40);
//	CC1101_Strobe(SIDLE);
//	HAL_Delay(40);
//	CC1101_Strobe(SRES);
//	HAL_Delay(100);
//	
//	DOWN_CS_CC1101;
//	HAL_SPI_Transmit(CC1101_SPI, (uint8_t*)Cfg, size,100);
//	UP_CS_CC1101;
//	//	CC1101_Config1(Cfg_AM_433, sizeof(Cfg_AM_433));
//	 HAL_Delay(100);
//	 CC1101_Strobe(SRX);
//}

uint8_t CC1101_ReadData (uint8_t strob){
	uint8_t data;
	strob|=0x80;
	 
	DOWN_CS_CC1101;
	 HAL_SPI_Transmit(CC1101_SPI, &strob, 1, 100); 
	 HAL_SPI_Receive (CC1101_SPI, &data, 1, 100);
	UP_CS_CC1101;
 
	return (data); 
 }
 
float  CC1101_GET_Freq (void){
	uint32_t freq = (CC1101_ReadData(FREQ2)<<16)|(CC1101_ReadData(FREQ1)<<8)|(CC1101_ReadData(FREQ0));
	return((((float) freq*Fxosc/0x10000))/1000000); 
} 

void CC1101_SET_Freq (float freq){
  uint32_t Set_Freq=((float) (freq*1000000)/Fxosc * 0x10000);
	
	uint8_t Buf_Freq[]={
		FREQ2, Set_Freq>>16,
		FREQ1, Set_Freq>>8,
		FREQ0, Set_Freq	
	};
	
	CC1101_Strobe(SIDLE);
	HAL_Delay(40);
	DOWN_CS_CC1101;
	HAL_SPI_Transmit(CC1101_SPI, (uint8_t*)Buf_Freq, sizeof(Buf_Freq),100);
	UP_CS_CC1101;
	HAL_Delay(40);
}


void CC1101_Config (uint8_t Cfg){

	
	HAL_Delay(40);
	CC1101_Strobe(SIDLE);
	HAL_Delay(40);
	CC1101_Strobe(SRES);
	HAL_Delay(100);
	CC1101_Strobe(SIDLE);
	HAL_Delay(10);
	
	DOWN_CS_CC1101;
	 switch (Cfg){
		 case AM_433:
				 HAL_SPI_Transmit(CC1101_SPI, (uint8_t*)Cfg_AM_433, sizeof(Cfg_AM_433),100);
			break;
		 case FM_433:
				 HAL_SPI_Transmit(CC1101_SPI, (uint8_t*)Cfg_FM_433, sizeof(Cfg_FM_433),100);
			break;
	 }
	UP_CS_CC1101;
	 
	HAL_Delay(10);
	CC1101_Strobe(SIDLE);
	 
	 
	 HAL_Delay(100);
	//fr=CC1101_GET_Freq();
	 //CC1101_SET_Freq (300.100);
	 //CC1101_ReadData (FREQ2);
	 
	 //CC1101_Strobe(SRX);
	 
	 
}













///*----------------------------------------------------------------------------
// Function : Config_CC1101 Заливка конфига для СС1101 
//*----------------------------------------------------------------------------*/
//void Config_CC1101 (uint8_t Cfg)
//{	
//	uint8_t i;
//	
//  DOWN_CS_CC1101;
//	while (GPIOB->IDR & GPIO_IDR_IDR14){};     /* Ждем пока на линии SPI MISO будет низкий уровень */
//	  Send_Strobe_CC1101(SIDLE);
//	while (SPI2->SR & SPI_SR_BSY){}; 	         /* Ожидание окончания передачи */
//	
//	  switch (Cfg) {
//			
//			case AM_433: for (i = 0; i < sizeof(Cfg_AM_433); i++) { 
//	                  SPI2->DR = (Cfg_AM_433[i]);
//	    while (SPI2->SR & SPI_SR_BSY); 	     /* Ожидание окончания передачи */
//	  }
//		    break;
//		
//			case FM_433: for (i = 0; i < sizeof(Cfg_FM_433); i++) { 
//	                  SPI2->DR = (Cfg_FM_433[i]);
//	    while (SPI2->SR & SPI_SR_BSY); 	     /* Ожидание окончания передачи */
//	  }
//		    break;
//		
//			case WFM_313: for (i = 0; i < sizeof(Cfg_WFM_313); i++) { 
//	                  SPI2->DR = (Cfg_WFM_313[i]);
//	    while (SPI2->SR & SPI_SR_BSY); 	     /* Ожидание окончания передачи */
//	  }
//		    break;
//		
//			case WFM_433: for (i = 0; i < sizeof(Cfg_WFM_433); i++) { 
//	                  SPI2->DR = (Cfg_WFM_433[i]);
//	    while (SPI2->SR & SPI_SR_BSY); 	     /* Ожидание окончания передачи */
//	  }
//		    break;
//		
//		  default:
//				break;
//	}
//  UP_CS_CC1101;
//}

///*----------------------------------------------------------------------------
// Function : Send_Strobe_CC1101 посылка строба в СС1101
//*----------------------------------------------------------------------------*/
//void Send_Strobe_CC1101 (uint8_t strobe)
//{
//  DOWN_CS_CC1101;
//	while (GPIOB->IDR & GPIO_IDR_IDR14){};  /* Ждем пока на линии SPI MISO будет низкий уровень */
//  SPI2->DR = strobe;
//  while (SPI2->SR & SPI_SR_BSY);        /* ожидание окончания передачи */ 
//  UP_CS_CC1101;
//}

///*----------------------------------------------------------------------------
// Function Send_Data_CC1101 посылка данных в СС1101
//*----------------------------------------------------------------------------*/
//void Send_Data_CC1101 (uint8_t data)
//{
//  DOWN_CS_CC1101;
//	while (GPIOB->IDR & GPIO_IDR_IDR14){}; /* Ждем пока на линии SPI MISO будет низкий уровень */
//  SPI2->DR = data;
//  while (SPI2->SR & SPI_SR_BSY);       /* ожидание окончания передачи */ 
//  UP_CS_CC1101;
//}

///*----------------------------------------------------------------------------
// Function Read_CC1101 чтение данных из СС1101
//*----------------------------------------------------------------------------*/
//uint8_t Read_CC1101 (void)
//{
//	DOWN_CS_CC1101;
//	while (GPIOB->IDR & GPIO_IDR_IDR14); /* Ждем пока на линии SPI MISO будет низкий уровень */
//	while (SPI2->SR & SPI_SR_BSY);       /* ожидание окончания передачи */
//	UP_CS_CC1101;
//  return (SPI2->DR);
//}
