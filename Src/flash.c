#include "flash.h"




/////////////////////////////////////////////////////////
//#define FLASH_KEY1               ((uint32_t)0x45670123)
//#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
/////////////////////////////////////////////////////////

uint8_t buf_data [1024] ={0}; //buf data
uint8_t p_st=63; //Nomer Staranici In save 


//NastoikaDef Work[3];






uint8_t flash_ready(void) {
	return !(FLASH->SR & FLASH_SR_BSY);
}

void flash_erase_all_pages(void) {
    FLASH->CR |= FLASH_CR_MER;
    FLASH->CR |= FLASH_CR_STRT;
    while(!flash_ready()){};
    FLASH->CR &= FLASH_CR_MER;
}

//pageAddress - Nomer stranici dliya Clear
void flash_erase_page(unsigned int pageAddress) {
	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = pageAddress;
	FLASH->CR |= FLASH_CR_STRT;
	while (!(FLASH->SR & FLASH_SR_EOP)){};
	FLASH->SR = FLASH_SR_EOP;
	FLASH->CR &= ~FLASH_CR_PER;
}

/*
void flash_erase_page(uint32_t address) {
    FLASH->CR|= FLASH_CR_PER;
    FLASH->AR = address;
    FLASH->CR|= FLASH_CR_STRT;
    while(!flash_ready())
    	;
    FLASH->CR&= ~FLASH_CR_PER;
}
*/

void flash_unlock(void) {
	  FLASH->KEYR = FLASH_KEY1;
	  FLASH->KEYR = FLASH_KEY2;
}

void flash_lock() {
	FLASH->CR |= FLASH_CR_LOCK;
}

//WRITE
void flash_write(uint32_t address,uint32_t data) {

	FLASH->CR |= FLASH_CR_PG;
	while(!flash_ready()){};
    *(__IO uint16_t*)address = (uint16_t)data;
	while(!flash_ready()){};
	//address+=2;
	//data>>=16;
  //  *(__IO uint16_t*)address = (uint16_t)data;
//	while(!flash_ready())
//		;
    FLASH->CR &= ~(FLASH_CR_PG);

}

//READ
uint32_t flash_read(uint32_t address) {
	return (*(__IO uint32_t*) address);
}		

void Flash_Read_Data(void) //считать настройки
{
	uint32_t st_address = FLASH_BASE + p_st * 1024;
				
	uint16_t j;
					for (j=0;j<sizeof(buf_data);j+=4)  {
						buf_data[j]=flash_read(st_address);
						buf_data[j+1]=flash_read(st_address)>>8;
						buf_data[j+2]=flash_read(st_address)>>16;
						buf_data[j+3]=flash_read(st_address)>>24;
					
						st_address+=4;
				}
}	


void Flash_Write_Data (void) //SAVE BUF
	{
uint32_t st_address = FLASH_BASE + p_st * 1024;
uint16_t tmp, data;
		//for(tmp=0;tmp<250;tmp++){ buf_data[tmp]=tmp;}
		flash_unlock();
				
				for(tmp=0;tmp<1024;tmp+=2){
					if (tmp<sizeof(buf_data)){
							data	= buf_data[tmp+1];
							data<<=8;
							data	= data+buf_data[tmp];
						} else{
							data=0x0000;
						}	
				 flash_write(st_address+tmp,data);
			}
		flash_lock();
	}
	

//data - ????????? ?? ???????????? ??????
//address - ????? ?? flash
//count - ?????????? ???????????? ????, ?????? ???? ?????? 2
	
void Internal_Flash_Write(unsigned char* data, unsigned int count) {
	unsigned int i; 
	unsigned int address;
address = FLASH_BASE + p_st * 1024;
	
	flash_unlock();
	
	while (FLASH->SR & FLASH_SR_BSY);
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR |= FLASH_CR_PG;

	for (i = 0; i < count; i += 2) {
		*(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
		while (!(FLASH->SR & FLASH_SR_EOP));
		FLASH->SR = FLASH_SR_EOP;
	}

	FLASH->CR &= ~(FLASH_CR_PG);
	
	flash_lock();
	
}	



void Flash_Erase_Data(void) //Clear Settung
{
uint32_t  st_address = FLASH_BASE + p_st * 1024;
					flash_unlock();
					flash_erase_page(st_address);
					flash_lock();
					//send_to_uart('\r'); send_to_uart('\n');
				//	send_str("The page has been erased.");
					//send_to_uart('\r'); send_to_uart('\n');
}


void Save(void)
{
uint16_t s_address=0;
//Flash_Read_Data(); //считать
//Flash_Erase_Data(); //отчистить перед записью
//Flash_Write_Data(); //записать

//for(int y=0;y<3;y++)
//	{
//	
//buf_data[s_address]=Work[y].Korrec;
//s_address++;		
//for(int i=0;i<4;i++)
//	{	
//	
//buf_data[s_address]=(uint8_t)Work[y].TimeWork[i];
//s_address++;
//buf_data[s_address]=(uint8_t)(Work[y].TimeWork[i]>>8);
//s_address++;		
//	}
//	
//for(int i=0;i<7;i++)
//	{	
//		
//buf_data[s_address]=Work[y].WeekDay[i];
//s_address++;
//	}
//	
//for(int i=0;i<16;i++)
//	
//	{	
//		
//buf_data[s_address]=Work[y].TimeZone[i];
//s_address++;
//	}	

//}
	
Flash_Erase_Data();
HAL_Delay(100);
Flash_Write_Data();	
HAL_Delay(200);
	
}

void Read(void){
uint16_t s_address=0;
Flash_Read_Data();
	
//for(int y=0;y<3;y++)
//	{

//Work[y].Korrec=buf_data[s_address];
//s_address++;	
//		
//for(int i=0;i<8;i=i+2)
//	{	

//		
//Work[y].TimeWork[i/2]= buf_data[s_address+1];
//Work[y].TimeWork[i/2]<<=8;
//s_address++;	

//Work[y].TimeWork[i/2]=Work[y].TimeWork[i/2]+buf_data[s_address-1];
//s_address++;
//	}
//	
//for(int i=0;i<7;i++)
//	{	
//		
//Work[y].WeekDay[i]=buf_data[s_address];
//s_address++;
//	}
//	
//for(int i=0;i<16;i++)
//	
//	{	
//		
//Work[y].TimeZone[i]=buf_data[s_address];
//s_address++;
//	}	

//}


}

