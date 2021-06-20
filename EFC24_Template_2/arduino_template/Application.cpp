/*
 * CPPFile1.cpp
 *
 * Created: 26.09.2018 21:16:11
 *  Author: Oktay Karakaya
 */ 
#include <Arduino.h>
#include <Wire.h>
#include "FlashAsEEPROM.h"
#include "EF_Standart_Library.h"
#include "EF_EFC24_Library.h"
#include "Global_Variables.h"
#include "Application.h"
#include "Analog_Inputs.h"
#include "mcp_can.h"
#include "SPI.h"
#include "PCCommunicationCan.h"
#include "PCCommunicationSerial.h"

#define SPI_CS_PIN  36

unsigned char stmp0[8];
unsigned char stmp1[2];



MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin
TON Extended_Can_Message_Timer (19);
TON Serial_Port_Trasnmit_Delay (500);
TON high_time_timeout_timer(2000);
TON low_time_timeout_timer(2000);
R_TRIG rising_edge;
F_TRIG falling_edge;
boolean flg_rising_started = false; 
boolean flg_falling_started = false;
cPCCommunicationCan PCCan(&CAN,0x400);
cPCCommunicationSerial PCSerial(&Serial1,0x400);
int16_t test_variables[50];
int16_t test_parameters[50] ;
void prg_application_setup(void){
	
	for(int i=0; i<sizeof(test_variables)/2;i++){
		
		test_variables[i] = 500*i;
	}
	
	for(int i=0;i<sizeof(test_parameters)/2;i++){
		test_parameters[i]=3*i;
	}
	
	PCCan.FillAdressField(PCCOMMUNICATIONCAN_PARAMETRE,test_parameters,sizeof(test_parameters));
	PCCan.FillAdressField(PCCOMMUNICATIONCAN_VARIABLE,test_variables,sizeof(test_variables));
	PCSerial.FillAdressField(PCCOMMUNICATIONSERIAL_PARAMETRE,test_parameters,sizeof(test_parameters));
	PCSerial.FillAdressField(PCCOMMUNICATIONSERIAL_VARIABLE,test_variables,sizeof(test_variables));
	
	
	

	memset(stmp0,0,sizeof(stmp0));
	memset(stmp1,0,sizeof(stmp1));
	 ANALOGINPUTS_Setup();
	 Serial1.begin(9600);
	 

	 while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
	 {
		 
		 delay(100);
		 Serial1.println("CAN BUS Shield init fail");
		 Serial1.println(" Init CAN BUS Shield again");
		 
	 }
	 Serial1.println("CAN BUS Shield init ok!");
}



  
String my_name;
void project (void){
	if (Serial1.available()) {
		int inByte = Serial1.read();
		
		PCSerial.NewDataReceived(inByte);
	}
	for(int i=0;i<sizeof(test_variables);i++){
		test_variables[i]++;
	}
	
	PCCan.Executer();
	PCSerial.Executer();
	
 }

void project_set_can_tx_buffer(void){
	
	stmp0[0] = g_.analog_input[0]>>8;
	stmp0[1] = g_.analog_input[0];
	stmp0[2] = g_.analog_input[1]>>8;
	stmp0[3] = g_.analog_input[1];
	stmp0[4] = g_.analog_input[2]>>8;
	stmp0[5] = g_.analog_input[2];
	stmp0[6] = g_.analog_input[3]>>8;
	stmp0[7] = g_.analog_input[3];
	
	stmp1[0] = g_.analog_input[4]>>8;
	stmp1[1] = g_.analog_input[4];
	
}

void fun_average_filter(int16_t *Non_Filtered_Value, int16_t *Filtered_Value,uint8_t Filter_Factor)
{
	int16_t adc_value=0;
	int16_t avg_value=0;
	for (int i=0;i<FILTER_FACTOR;i++)
	{
		adc_value = *Non_Filtered_Value;
		avg_value+=adc_value;
	}
	
	adc_value=0;
	adc_value=avg_value/FILTER_FACTOR;
	*Non_Filtered_Value=adc_value;
	*Filtered_Value=(*Filtered_Value*0.8)+(adc_value*0.2); 
	
}	


boolean fun_read_eeprom(boolean read_eeprom_enable){
	uint8_t *StructStart = (uint8_t*)&p_;
	uint16_t i;
	
	if(read_eeprom_enable==true){
		for(i=0; i<sizeof(p_); i++) {
			StructStart[i] = EEPROM.read(i);
		}
		return true;
	}
	else {
		return false;
	}
}

boolean fun_write_eeprom(boolean write_eeprom_enable){
	uint8_t *StructStart = (uint8_t*)&p_;
	uint16_t i;
	
	if(write_eeprom_enable==true){
		for(i=0; i<sizeof(p_); i++) {
			EEPROM.update(i, StructStart[i]);
			
		}
		return true;
	}
	else{
		
		return false;
	}
}












void prg_default_parameters (void)
{
	
		
}

 /*int16_t XData, YData, ZData;
/* 1/tan(x) formülünün 0..6,8 derece arasý dereceler için çözülmüþ halidir. Hesaplama yapýlýrken
 * (1*Ax^2)/(Ay^2+Az^2) formülü çözülüyor. Bu deðeri arctan(1/x) te x yerine yazýlýrse açý deðeri
 * bulunuyor. Bu iþlemcide arctan bulunamadýðýndan her derece için, arctan içi hesaplanýyor.  
const uint16_t AngleList[68] = {0xFFFF, 0xFFFF, 0xFFFF, 36475, 20517, 13131, 9118, 6699, 5129, 4052,
        3282, 2712, 2279, 1942, 1674, 1458, 1282, 1135, 1013, 909, 820, 744, 678, 620, 569, 525, 485,
        450, 418, 390, 364, 341, 320, 301, 283, 267, 253, 239, 227, 215, 205, 195, 185, 177, 169, 161,
        154, 148, 142, 136, 131, 126, 121, 116, 112, 108, 104, 100, 97, 94, 91, 88, 85, 82, 79, 77};
        */	



/* demo: CAN-BUS Shield, send data
#include "mcp_can.h"
#include "SPI.h"
#include "EF_Standart_Library.h"
#define SPI_CS_PIN  36

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin

void setup()
{
    Serial1.begin(115200);

    while (CAN_OK != CAN.begin(CAN_250KBPS))              // init can bus : baudrate = 500k
    {
		   delay(100);
        Serial1.println("CAN BUS Shield init fail");
        Serial1.println(" Init CAN BUS Shield again");
     
    }
    Serial1.println("CAN BUS Shield init ok!");
	
}

unsigned char stmp[8] = {0, 1, 2, 3, 4, 5, 6, 7};

unsigned char stmp1[2] = {1, 0};

void loop()
{

    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    CAN.sendMsgBuf(0x0CF00400, 1, 8, stmp);
    delay(50);                       // send data per 100ms
    CAN.sendMsgBuf(0x181, 0, 2, stmp1);
    delay(50);                       // send data per 100ms
}
*/

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/