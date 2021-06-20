#include "EF_EFC24_Library.h"

/* Global Variables */


/* Global_Functions */


void prg_pinmode_setup(void) {
	
		
	/**********************************************	Pin Mode I/O Definitions ***********************************************************************************/
	/* Analogical Inputs */
	//pinMode(IW0_1,INPUT); /* _ANALOG_INPUT_1 */
	//pinMode(IW0_2,INPUT); /* _ANALOG_INPUT_2 */
	//pinMode(IW0_3,INPUT); /* _ANALOG_INPUT_3 */
//	pinMode(IW0_4,INPUT); /* _ANALOG_INPUT_4 */
//	pinMode(IW0_5,INPUT); /* _ANALOG_INPUT_5 */
		
	/********************************************************************************************************************************/
	/* Digital Inputs */
	pinMode(IX0_1,INPUT); /* _DIGITAL_INPUT_1 */
	pinMode(IX_K15,INPUT); /* _DIGITAL_INPUT_2 */
	
	
	/*********************************************************************************************************************************/
	/* PWM Outputs */
	pinMode(QP0_1,OUTPUT); /* _PWM_OUTPUT_1 */
	pinMode(QP0_2,OUTPUT); /* _PWM_OUTPUT_2 */
	pinMode(QP0_3,OUTPUT); /* _PWM_OUTPUT_3 */
	pinMode(QP0_4,OUTPUT); /* _PWM_OUTPUT_4 */
	pinMode(QP0_5,OUTPUT); /* _PWM_OUTPUT_5 */
	pinMode(QP0_6,OUTPUT); /* _PWM_OUTPUT_6 */
	pinMode(QP0_7,OUTPUT); /* _PWM_OUTPUT_7 */
	pinMode(QP0_8,OUTPUT); /* _PWM_OUTPUT_8 */
	
	pinMode(SPI_CS_PIN,OUTPUT);
	/*********************************************************************************************************************************/
	/* Analogical Outputs */
	
	
	/*********************************************************************************************************************************/
	/* Current/Voltage Input Mode Selection Outputs */
	/* Default Configuration is 0-32V for all inputs */
	/* change parameters like below for 4-20 mA*/
	/* digitalWrite(QY0_1,LOW);	/*_ANALOG_INPUT_1_0_32_V_ENABLE_PIN */
	/* digitalWrite(QY0_11,HIGH);	/*_ANALOG_INPUT_1_4_20_mA__ENABLE_PIN */

	pinMode(QY0_1,OUTPUT);		/*_ANALOG_INPUT_1_0_32_V_ENABLE_PIN */
	pinMode(QY0_11,OUTPUT);	/*_ANALOG_INPUT_1_4_20_mA__ENABLE_PIN */
	digitalWrite(QY0_1,HIGH);	/*_ANALOG_INPUT_1_0_32_V_ENABLE_PIN */
	digitalWrite(QY0_11,LOW);	/*_ANALOG_INPUT_1_4_20_mA__ENABLE_PIN */
	
	pinMode(QY0_2,OUTPUT); 	/*_ANALOG_INPUT_2_0_32_V_ENABLE_PIN */
	pinMode(QY0_12,OUTPUT);	/*_ANALOG_INPUT_2_4_20_mA__ENABLE_PIN */
	digitalWrite(QY0_2,HIGH);	/*_ANALOG_INPUT_2_0_32_V_ENABLE_PIN */
	digitalWrite(QY0_12,LOW);	/*_ANALOG_INPUT_2_4_20_mA__ENABLE_PIN */
	
	pinMode(QY0_3,OUTPUT); 	/*_ANALOG_INPUT_3_0_32_V_ENABLE_PIN */
	pinMode(QY0_13,OUTPUT);	/*_ANALOG_INPUT_3_4_20_mA__ENABLE_PIN */
	digitalWrite(QY0_3,HIGH);	/*_ANALOG_INPUT_3_0_32_V_ENABLE_PIN */
	digitalWrite(QY0_13,LOW);	/*_ANALOG_INPUT_3_4_20_mA__ENABLE_PIN */
	
	pinMode(QY0_4,OUTPUT); 	/*_ANALOG_INPUT_4_0_32_V_ENABLE_PIN */
	pinMode(QY0_14,OUTPUT);	/*_ANALOG_INPUT_4_4_20_mA__ENABLE_PIN */
	digitalWrite(QY0_4,HIGH);	/*_ANALOG_INPUT_4_0_32_V_ENABLE_PIN */
	digitalWrite(QY0_14,LOW);	/*_ANALOG_INPUT_4_4_20_mA__ENABLE_PIN */
	
	pinMode(QY0_5,OUTPUT); 	/*_ANALOG_INPUT_5_0_32_V_ENABLE_PIN */
	digitalWrite(QY0_5,HIGH);	/*_ANALOG_INPUT_5_0_32_V_ENABLE_PIN */

}

