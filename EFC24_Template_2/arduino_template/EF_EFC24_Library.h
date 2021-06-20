/**
  ******************************************************************************
    @attention 	Detaylı açıklamalar .cpp dosyasındadır.
  ******************************************************************************
*/

#ifndef __EF_EFC24_Library_H
#define	__EF_EFC24_Library_H
#include "Arduino.h"

/**********************************************************************************************
 *				  INPUT & DEFINITIONS                 										  *
 *                                                   										  *
 * Here we are associating inputs and outputs of EF_EFC39 Controller  with a number			  *
 *																							  *
 **********************************************************************************************
*/// EFC24

// *** ANALOG GİRİŞLER **
// ANALOG GİRİŞ 1 - PA03 - 42
// ANALOG GİRİŞ 2 - PA02 - 14
// ANALOG GİRİŞ 3 - PB08 - 15
// ANALOG GİRİŞ 4 - PB09 - 16
// ANALOG GİRİŞ 5 - PA04 - 17

// *** DİJİTAL GİRİŞLER **
// DİJİTAL VEYA RPM GİRİŞ 1 - PB03 - 25
// K15 GİRİŞİ				- PA28 - 27

// *** Dijital Çıkışlar **
// GİRİŞ 1 0-32V SEÇME - PA06	-	8
// GİRİŞ 2 0-32V SEÇME - PA07	-	9
// GİRİŞ 3 0-32V SEÇME - PA09	-	4
// GİRİŞ 4 0-32V SEÇME - PA21	-	39
// GİRİŞ 5 0-32V SEÇME (VEYA REZİSTİF)- PA24 -	28
// GİRİŞ 1 4-20mA SEÇME -  PA25	-	29
// GİRİŞ 2 4-20mA SEÇME -  PB22	-	30
// GİRİŞ 3 4-20mA SEÇME -  PB23 -	31
// GİRİŞ 4 4-20mA SEÇME -  PA27 -	26
// K15 ÇIKIŞI			-  PB02 -   19

// *** PWM Çıkışlar **
//PWM1 -  PA13	-	38
//PWM2 -  PA20	-	6
//PWM3 -  PA23	-	21
//PWM4 -  PA22	-	20
//PWM5 -  PA19	-	12
//PWM6 -  PA08	-	4
//PWM7 -  PA05	-	18
//PWM8 -  PA16	-	11


/* Define Variables */

/* Analog Inputs */
#define IW0_1 						42 /* ANALOG_INPUT_1 */      
#define IW0_2  						14 /* ANALOG_INPUT_2 */
#define IW0_3						15 /* ANALOG_INPUT_3 */
#define IW0_4						16 /* ANALOG_INPUT_4 */
#define IW0_5						17 /* ANALOG_INPUT_5 */


/* Digital Inputs */
#define IX0_1						25 /* MiniController_DIGITAL_INPUT_1 */							
#define IX_K15						27 /* MiniController_K15_IGNITION_INPUT */						
             
/* PWM Outputs */
#define QP0_1						38 /* MiniController_PWM_OUTPUT_1 */							
#define QP0_2						6  /* MiniController_PWM_OUTPUT_2 */							
#define QP0_3						21 /* MiniController_PWM_OUTPUT_3 */							
#define QP0_4						20 /* MiniController_PWM_OUTPUT_4 */							
#define QP0_5						12 /* MiniController_PWM_OUTPUT_5 */							
#define QP0_6						4  /* MiniController_PWM_OUTPUT_6 */							
#define QP0_7						18 /* MiniController_PWM_OUTPUT_7 */							
#define QP0_8						11 /* MiniController_PWM_OUTPUT_8 */							

/* Digital Outputs */
#define QY0_1						8	/* MiniController_ANALOG_INPUT_1_0_32_V_ENABLE_PIN */
#define QY0_2						9	/* MiniController_ANALOG_INPUT_2_0_32_V_ENABLE_PIN */
#define QY0_3						3	/* MiniController_ANALOG_INPUT_3_0_32_V_ENABLE_PIN */
#define QY0_4						39	/* MiniController_ANALOG_INPUT_4_0_32_V_ENABLE_PIN */
#define QY0_5						28	/* MiniController_ANALOG_INPUT_5_0_32_V_OR_RESISTIVE_ENABLE_PIN */


#define QY0_11						29	/* MiniController_ANALOG_INPUT_1_4_20_mA__ENABLE_PIN */
#define QY0_12						30	/* MiniController_ANALOG_INPUT_2_4_20_mA__ENABLE_PIN */
#define QY0_13						31	/* MiniController_ANALOG_INPUT_3_4_20_mA__ENABLE_PIN */
#define QY0_14						26  /* MiniController_ANALOG_INPUT_4_4_20_mA__ENABLE_PIN */		
#define Q_K15_ENABLE				19  /* MiniController_K15_ENABLE_OUTPUT_PIN */
	                  

#define DEFAULT_ADC_MIN_VALUE								0
#define DEFAULT_ADC_MAX_VALUE								461 // Donanıma bağlı 16V besleme ile hesaplanmış olan adc değeridir.
#define DEFAULT_MAX_VOLTAGE									4800 // mV cinsinden DEFAULT_ADC_MAX_VALUE'ya karşılık gelen voltaj değeri
#define FILTER_FACTOR										2 // Filtre Katsayısını belirler
#define MIN_ADC_FOR_ANALOG_TO_DIGITAL						50

#define SPI_CS_PIN  36
/* Global Variables */



	
/* Global Functions */
void prg_pinmode_setup(void);


#endif /*__EF_EFC24_Library__*/
