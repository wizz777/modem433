#ifndef __CC1101_H_
#define __CC1101_H_

#include "stm32f1xx_hal.h"


enum{
	AM_433=0x01,
	FM_433,
	WFM_313,
	WFM_433,
};

#define Fxosc 26000000     			//Freq Crystal

//#define AM_433  0x01
//#define FM_433  0x02
//#define WFM_313 0x03
//#define WFM_433 0x04

// CC1101 STROBE, CONTROL AND STATUS REGSITER
#define IOCFG2       0x00        // GDO2 output pin configuration
#define IOCFG1       0x01        // GDO1 output pin configuration
#define IOCFG0       0x02        // GDO0 output pin configuration
#define FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define SYNC1        0x04        // Sync word, high byte
#define SYNC0        0x05        // Sync word, low byte
#define PKTLEN       0x06        // Packet length
#define PKTCTRL1     0x07        // Packet automation control
#define PKTCTRL0     0x08        // Packet automation control
#define ADDR         0x09        // Device address
#define CHANNR       0x0A        // Channel number
#define FSCTRL1      0x0B        // Frequency synthesizer control
#define FSCTRL0      0x0C        // Frequency synthesizer control
#define FREQ2        0x0D        // Frequency control word, high byte
#define FREQ1        0x0E        // Frequency control word, middle byte
#define FREQ0        0x0F        // Frequency control word, low byte
#define MDMCFG4      0x10        // Modem configuration
#define MDMCFG3      0x11        // Modem configuration
#define MDMCFG2      0x12        // Modem configuration
#define MDMCFG1      0x13        // Modem configuration
#define MDMCFG0      0x14        // Modem configuration
#define DEVIATN      0x15        // Modem deviation setting
#define MCSM2        0x16        // Main Radio Control State Machine configuration
#define MCSM1        0x17        // Main Radio Control State Machine configuration
#define MCSM0        0x18        // Main Radio Control State Machine configuration
#define FOCCFG       0x19        // Frequency Offset Compensation configuration
#define BSCFG        0x1A        // Bit Synchronization configuration
#define AGCCTRL2     0x1B        // AGC control
#define AGCCTRL1     0x1C        // AGC control
#define AGCCTRL0     0x1D        // AGC control
#define WOREVT1      0x1E        // High byte Event 0 timeout
#define WOREVT0      0x1F        // Low byte Event 0 timeout
#define WORCTRL      0x20        // Wake On Radio control
#define FREND1       0x21        // Front end RX configuration
#define FREND0       0x22        // Front end TX configuration
#define FSCAL3       0x23        // Frequency synthesizer calibration
#define FSCAL2       0x24        // Frequency synthesizer calibration
#define FSCAL1       0x25        // Frequency synthesizer calibration
#define FSCAL0       0x26        // Frequency synthesizer calibration
#define RCCTRL1      0x27        // RC oscillator configuration
#define RCCTRL0      0x28        // RC oscillator configuration
#define FSTEST       0x29        // Frequency synthesizer calibration control
#define PTEST        0x2A        // Production test
#define AGCTEST      0x2B        // AGC test
#define TEST2        0x2C        // Various test settings
#define TEST1        0x2D        // Various test settings
#define TEST0        0x2E        // Various test settings

// Strobe commands
#define SRES         0x30        // Reset chip.
#define SFSTXON      0x31        // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
                                 // If in RX/TX: Go to a wait state where only the synthesizer is
                                 // running (for quick RX / TX turnaround).
#define SXOFF        0x32        // Turn off crystal oscillator.
#define SCAL         0x33        // Calibrate frequency synthesizer and turn it off
                                 // (enables quick start).
#define SRX          0x34        // Enable RX. Perform calibration first if coming from IDLE and
                                 // MCSM0.FS_AUTOCAL=1.
#define STX          0x35        // In IDLE state: Enable TX. Perform calibration first if
                                 // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                 // Only go to TX if channel is clear.
#define SIDLE        0x36        // Exit RX / TX, turn off frequency synthesizer and exit
                                 // Wake-On-Radio mode if applicable.
#define SAFC         0x37        // Perform AFC adjustment of the frequency synthesizer
#define SWOR         0x38        // Start automatic RX polling sequence (Wake-on-Radio)
#define SPWD         0x39        // Enter power down mode when CSn goes high.
#define SFRX         0x3A        // Flush the RX FIFO buffer.
#define SFTX         0x3B        // Flush the TX FIFO buffer.
#define SWORRST      0x3C        // Reset real time clock.
#define SNOP         0x3D        // No operation. May be used to pad strobe commands to two
                                 // bytes for simpler software.
#define PATABLE      0x3E
#define TXFIFO       0x3F
#define RXFIFO       0x3F









uint8_t CC1101_Init (SPI_HandleTypeDef * hspi, GPIO_TypeDef * cs_port, uint16_t cs_pin);
//void CC1101_Config (const uint8_t *Cfg, uint8_t size);
void CC1101_Config (uint8_t Cfg);
void CC1101_Strobe (uint8_t strob);
float  CC1101_GET_Freq (void);
void CC1101_SET_Freq (float freq);

//void Send_Strobe_CC1101(uint8_t strobe);
//void Config_CC1101(uint8_t Cfg);
//void Send_Data_CC1101(uint8_t data);
//uint8_t read_CC1101(void);

#endif /* __CC101_H_ */
