 /*************************************************************************************************
    @file    	Analog_Inputs.h
    @authors  	Oktay KARAKAYA
    @version 	V1
    @date    	31/08/2019
    @brief		SAMD21G18 controller için tüm analog girişleri okuyup Array'a alan Analog_Inputs.cpp
	library'sine ait header dosyasıdır.
	Detaylı açıklamalar Analog_Inputs.cpp dosyası içerisindedir.
	@copyright	Copyright (c) 2017, EHR Elektronik Hidrolik ve Otomasyon A.Ş.
  ************************************************************************************************
    @attention
  ************************************************************************************************
    @revision list
  				V1-31/08/2019: ilk versiyon.
  ************************************************************************************************
*/

#ifndef _ANALOGINPUTS_H_
#define _ANALOGINPUTS_H_
#include "Arduino.h"


#define TOTAL_ANALOG_INPUTS_NUMBER			6 // Toplam Analog Giriş-1 girilecek
											
typedef struct tsANALOGINPUTS_Results {
	uint16_t ADCReadingResults[TOTAL_ANALOG_INPUTS_NUMBER];
	uint8_t CurrentADCChannel;
} tANALOGINPUTS_Results;
extern tANALOGINPUTS_Results ANALOGINPUTS_Results;


void ANALOGINPUTS_Setup(void);
void ANALOGINPUTS_Executer(void);
#endif //_ANALOGINPUTS_H_