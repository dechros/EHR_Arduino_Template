 /*************************************************************************************************
    @file    	Analog_Inputs.cpp
    @authors  	Oktay KARAKAYA
    @version 	V1
    @date    	13/12/2018
    @brief		SAMD21G18 controller için yazýlýmda tüm analog giriþleri okuyup Array'a alýr.
	Bu þekilde okuma standart Arduino AnalogRead veya benzer metodlar ile yapýlan tek tek okuma
	metodundan daha hýzlýdýr.	
	@copyright	Copyright (c) 2017, EHR Elektronik Hidrolik ve Otomasyon A.Þ.
  ************************************************************************************************
    @attention
  ************************************************************************************************
    @revision list
  				V1-24/11/2018: ilk versiyon.
  ************************************************************************************************
*/
 /*  Include Files	 */
#include "Arduino.h"
#include "Global_Variables.h"
#include "wiring_private.h"
#include "Analog_Inputs.h"
#include "EF_EFC24_Library.h"
#include "EF_Control_Library.h"
/* Define Variables */
#define ANALOGINPUTS_CHECK_ADC_IS_FINISH()			(!( ADC->INTFLAG.bit.RESRDY == 0 ))

#define ADC_RES_12BIT		0x00
#define ADC_RES_16BIT		0x01
#define ADC_RES_10BIT		0x02
#define ADC_RES_8BIT		0x03
/* Define Functions */
void ANALOGINPUTS_StartADCReading(uint8_t ADCChannel);
void fb_low_pass_adc_filter(uint16_t Non_Filtered_Value, uint8_t Filter_Factor, uint16_t *Filtered_Value);

/* Global Variables */

// This is an C/C++ code to insert repetitive code sections in-line pre-compilation
// Wait for synchronization of registers between the clock domains
// ADC
static __inline__ void ADCsync() __attribute__((always_inline, unused));
static void   ADCsync() {
	while (ADC->STATUS.bit.SYNCBUSY == 1);
}
// GCLK
static __inline__ void syncADC() __attribute__((always_inline, unused));
static void syncGCLK() {
	while (GCLK->STATUS.bit.SYNCBUSY == 1); //Just wait till the ADC is free
}
/* Local Variables */
tANALOGINPUTS_Results ANALOGINPUTS_Results;
//const uint8_t ANALOGINPUTS_ADC_Channels[TOTAL_ANALOG_INPUTS_NUMBER] = {IW0_1, IW0_2, IW0_3, IW0_4, IW0_5, IW0_6, IW0_7,IX0_1};
const uint8_t ANALOGINPUTS_ADC_Channels[TOTAL_ANALOG_INPUTS_NUMBER] = {1, 0, 2, 3, 4,11};


/******************************** Analog input Setup Loop ***************************************
    @brief      	ANALOGINPUTS_Setup : Ana program çalýþtýrýldýðýnda baþlangýçta bir defa
	çalýþtýrýlmasý gereken kurulum programýdýr.
    @param[in]		None
    @retval     	None
    @attention
***********************************************************************************************/
void ANALOGINPUTS_Setup(void){
	/* setup pre sets */
	uint8_t i=0;
	for(i=0; i<TOTAL_ANALOG_INPUTS_NUMBER; i++) {
		ANALOGINPUTS_Results.ADCReadingResults[i] = 0;
	}
	ANALOGINPUTS_Results.CurrentADCChannel = 0;
	
	ADCsync();
	ADC->INPUTCTRL.bit.MUXNEG = 0x18;

	//Set input control register
	ADCsync();
	ADC->INPUTCTRL.bit.GAIN = ADC_INPUTCTRL_GAIN_1X_Val;      // Gain select as 1X
	//Set ADC reference source
	ADCsync();
	ADC->REFCTRL.bit.REFSEL = AR_DEFAULT;   // ref is 1 V band gap internal
	// Set sample length and averaging
	ADCsync();
	ADC->AVGCTRL.reg = 0x00 ;       //Single conversion no averaging
	ADCsync();
	ADC->SAMPCTRL.reg = 0x00;       //Minimal sample length is 1/2 CLK_ADC cycle
	// Set up clock to 8 Mhz oscillator
	syncGCLK();
	GCLK->CLKCTRL.reg = 0x431E; //enable GGCLK for ADC, CLKGEN3 = 8 MHz oscillator 
	syncGCLK();
	//Control B register
	ADCsync();
	ADC->CTRLB.reg =  ADC_RES_12BIT     ; // Prescale 4, 16 bit resolution, single conversion
	// Enable ADC in control B register
	ADCsync();
	ADC->CTRLA.bit.ENABLE = 0x01;  
  
	ANALOGINPUTS_StartADCReading(ANALOGINPUTS_Results.CurrentADCChannel);
}
	

/*************************** ANALOGINPUTS_Executer_Loop ****************************************
    @brief      	ANALOGINPUTS_Executer : Ana yazýlýmýn içerisinde sadece ilk baþlangýçta 
	kullanýlacak programlar ve execute yapýsýnýn setup bölümleri gibi ilk seferde ve tek sefer 
	çalýþtýrýlacak program ve tanýmlamalar burada çaðýrýlýr.
    @param[in]		None
    @retval     	None
    @attention
***********************************************************************************************/
void ANALOGINPUTS_Executer(void) {		

	if(ANALOGINPUTS_CHECK_ADC_IS_FINISH()) {
		//Serial1.println(ANALOGINPUTS_Results.CurrentADCChannel);
		ANALOGINPUTS_Results.ADCReadingResults[ANALOGINPUTS_Results.CurrentADCChannel] = ADC->RESULT.reg;
		g_.analog_input[ANALOGINPUTS_Results.CurrentADCChannel]= ADC->RESULT.reg;
		//Serial1.println(ANALOGINPUTS_Results.ADCReadingResults[ANALOGINPUTS_Results.CurrentADCChannel]);
		ADCsync();
		ADC->SWTRIG.reg = 0x01;                    //  and flush for good measure
		
		ANALOGINPUTS_Results.CurrentADCChannel++;
		if(ANALOGINPUTS_Results.CurrentADCChannel >= TOTAL_ANALOG_INPUTS_NUMBER) {
			ANALOGINPUTS_Results.CurrentADCChannel = 0;
		}
		ANALOGINPUTS_StartADCReading(ANALOGINPUTS_Results.CurrentADCChannel);
	} 
}

/******************************** ADC start to Reading*******************************************
    @brief      	ANALOGINPUTS_StartADCReading : 
    @param[in]		uint8_t ADCChannel:
    @retval     	None
    @attention
***********************************************************************************************/
//##############################################################################
// Fast analogue read anaRead()
// This is a stripped down version of analogRead() where the set-up commands
// are executed during setup()
// ulPin is the analog input pin number to be read.
//  Mk. 2 - has some more bits removed for speed up
///##############################################################################

void ANALOGINPUTS_StartADCReading(uint8_t ADCChannel) {
	
	ADCsync();
	ADC->INPUTCTRL.bit.MUXPOS = ANALOGINPUTS_ADC_Channels[ADCChannel];
	ADCsync();
	ADC->INTFLAG.bit.RESRDY = 1;              // Data ready flag cleared
	ADCsync();
	ADC->SWTRIG.bit.START = 1;                // Start ADC conversion	
}


void fb_low_pass_adc_filter(uint16_t Non_Filtered_Value, uint8_t Filter_Factor, uint16_t *Filtered_Value)
{
	uint32_t Result = Non_Filtered_Value;
	for(uint8_t i=0; i<FILTER_FACTOR; i++) {
		Result = Result + (*Filtered_Value);
		Result = Result / 2;
	}
	*Filtered_Value = (uint16_t)Result;
}